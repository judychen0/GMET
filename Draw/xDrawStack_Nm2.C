#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "./Drawlumi/CMS_lumi.C"

using namespace std;
#define nfile 12
#define nhisto 2
#define njet 2
#define ncolor 20

TH1F* ratioplot(THStack* st, TH1F* h1){

  if (!st) {
       Warning("TRatioPlot", "Need only a stack");
       //return;
    }
  
  TList *stackHists = st->GetHists();
  
  if (stackHists->GetSize() == 0) {
    Warning("TRatioPlot", "Stack does not have histograms");
    //return;
  }
  
  TH1F* tmpHist = (TH1F*)stackHists->At(0)->Clone();
  tmpHist->Reset();
  
  for (int i=0;i<stackHists->GetSize();++i) {
    tmpHist->Add((TH1*)stackHists->At(i));
  }
  
    
  TH1F *ratio = (TH1F*)h1->Clone("ratio");
  ratio->SetLineColor(kBlack);
  ratio->SetMinimum(-0.4);
  ratio->SetMaximum(3.4);
  ratio->Sumw2();
  ratio->Divide(tmpHist);
  
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerSize(0.8);
  ratio->Draw("E3 P");
  
  ratio->SetTitle("");
  ratio->GetYaxis()->SetTitle("Data/MC");
  ratio->GetYaxis()->SetTitleFont(42);
  ratio->GetYaxis()->SetTitleSize(0.1);
  ratio->GetYaxis()->SetTitleOffset(0.4);
  ratio->GetYaxis()->SetLabelFont(42);
  ratio->GetYaxis()->SetLabelOffset(0.008);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetYaxis()->SetTickSize(0.032);
  
  ratio->GetXaxis()->SetTitleFont(42);
  ratio->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetTitleOffset(1.1);
  ratio->GetXaxis()->SetLabelFont(42);
  ratio->GetXaxis()->SetLabelOffset(0.03);
  ratio->GetXaxis()->SetLabelSize(0.1);
  ratio->GetXaxis()->SetTickSize(0.1);
  ratio->GetXaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");

  return ratio;
}

void xDrawStack_Nm2(Int_t year){

  TString rootname[20];
  TFile *fopen;

  Float_t entries = 1.0;
  Float_t outentries = 0.;
  Float_t scale[nfile]  = {0};
  TH1F *H_Events[nfile];
  TH1F *HSumofGenW[nfile];

  Float_t lumi16 = 36.33;
  Float_t lumi17 = 41.48;
  Float_t lumi18 = 59.83;
  Float_t lumi = 1.0;
  if(year==2016){
    lumi = lumi16;
  }
  else if(year==2017){
    lumi = lumi17;
  }
  else if(year==2018){
    lumi = lumi18;
  }

  //Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 0.296};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, Znng]
  Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 0.018};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, ZnngVBS]
  
  Float_t filter[20] = {1., 1., 1., 1., 1., 1., 1.};
  Float_t kfactor[20] = {1., 1., 1., 1., 1., 1., 1.};
  
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Wg_aMCatNLO/output_ggtree.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Zg_aMCatNLO/output_ggtree.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_TT_aMCatNLO/output_ggtree.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/output_ggtree.root";
    rootname[8] = "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_TT_aMCatNLO/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    rootname[11] = "/home/judy/ntuhep/GMET/output_file/fall17/data/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/job_autumn18_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/job_autumn18_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/job_autumn18_TT_aMCatNLO/output_ggtree.root";
    rootname[10] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/job_autumn18_Znunug_pt130/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/job_UL18_Znunug_VBS/output_ggtree.root";
    rootname[11] = "/home/judy/ntuhep/GMET/output_file/autumn18/data/output_ggtree.root";
  }

  for(Int_t i=0; i<nfile-1; i++){
    fopen = new TFile(rootname[i]);
    if((i<nfile-1) && i>=7){
      H_Events[i] = (TH1F*)fopen->Get("hEvents");
      //HSumofGenW[i] = (TH1F*)fopen->Get("hSumofGenW");
      HSumofGenW[i] = (TH1F*)fopen->Get("hSumGenWeight");
      entries = 1.0;
      //entries = H_Events[i]->GetBinContent(1);
      entries = HSumofGenW[i]->GetBinContent(1);
      outentries = xsec[i]*1000*lumi;
      scale[i] = 0;
      scale[i] = fabs(outentries/entries);

      cout << "print " << entries << " " << outentries << " " << scale[i]<< endl;
    }
    //if(i==nfile-1 || i<3) scale = 1.0;
    if(i<7) scale[i] = 1.0;
  }
  
  string hexcolor[ncolor] = {"#fff176", "#03a9f4", "#ff3360", "#3d5afe", "#b23b8c", "#96ff5a", "#a983d3", "#ff9e00", "#00FFFF", "#FFCCCC", "#67ccc1"};
  //string hexcolor[ncolor] = {"#fff176", "#03a9f4", "#ff3360", "#c144ae", "#3d5afe", "#ff9e00", "#00FFFF", "#FFCCCC", "#67ccc1"};//QCD
  TColor *color[ncolor];
  Int_t cnum[ncolor];

  for(Int_t j=0; j<ncolor; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  } 
  
  TCanvas *c1 = new TCanvas("c1");
  //c1->Divide(1, 2);
  TTree *t;

  TH1F *H_MET_Nm1_djetMETPhim0p5[nfile][nhisto];
  TH1F *H_MET_Nm1_METm100[nfile][nhisto];

  TH1F *HA_MET_Nm1_djetMETPhim0p5[nhisto];
  TH1F *HA_MET_Nm1_METm100[nhisto];

  TH1F *HSig_MET_Nm1_djetMETPhim0p5[nhisto];
  TH1F *HSig_MET_Nm1_METm100[nhisto];

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    if(i==nfile-1) scale[i] = 1.0;

    for(Int_t jj=0; jj<nhisto; jj++){
      cout<< "nfile " << i<< endl;
      //H_MET_Nm1_djetMETPhim0p5[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_djetMETPhim0p5_%i", jj));
      H_MET_Nm1_djetMETPhim0p5[i][jj] = (TH1F*)fopen->Get(Form("h_MET_Nm1/h_MET_Nm1_djetMETPhi_SB0p5_%i", jj));
      //H_MET_Nm1_METm100[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_METm100_%i", jj));

      if(i==0){
	//HA_MET_Nm1_djetMETPhim0p5[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_djetMETPhim0p5_%i", jj))->Clone();
	HA_MET_Nm1_djetMETPhim0p5[jj] = (TH1F*)fopen->Get(Form("h_MET_Nm1/h_MET_Nm1_djetMETPhi_SB0p5_%i", jj))->Clone();
	//HA_MET_Nm1_METm100[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_METm100_%i", jj))->Clone();

	HA_MET_Nm1_djetMETPhim0p5[jj]->Scale(scale[i]);
	//HA_MET_Nm1_METm100[jj]->Scale(scale[i]);

	HA_MET_Nm1_djetMETPhim0p5[jj]->SetFillStyle(3145);
	//HA_MET_Nm1_METm100[jj]->SetFillStyle(3145);

	HA_MET_Nm1_djetMETPhim0p5[jj]->SetFillColor(12);
	//HA_MET_Nm1_METm100[jj]->SetFillColor(12);

	HA_MET_Nm1_djetMETPhim0p5[jj]->SetMarkerSize(0.1);
      }

      if(i==nfile-2){
	//HSig_MET_Nm1_djetMETPhim0p5[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_djetMETPhim0p5_%i", jj))->Clone();
	HSig_MET_Nm1_djetMETPhim0p5[jj] = (TH1F*)fopen->Get(Form("h_MET_Nm1/h_MET_Nm1_djetMETPhi_SB0p5_%i", jj))->Clone();
	//HSig_MET_Nm1_METm100[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_METm100_%i", jj))->Clone();

	HSig_MET_Nm1_djetMETPhim0p5[jj]->Scale(scale[i]);
	//HSig_MET_Nm1_METm100[jj]->Scale(scale[i]);

	HSig_MET_Nm1_djetMETPhim0p5[jj]->SetLineColor(kRed);
	//HSig_MET_Nm1_METm100[jj]->SetLineColor(kRed);

	HSig_MET_Nm1_djetMETPhim0p5[jj]->SetLineWidth(2);
	//HSig_MET_Nm1_METm100[jj]->SetLineWidth(2);
      }

      H_MET_Nm1_djetMETPhim0p5[i][jj]->Scale(scale[i]);
      //H_MET_Nm1_METm100[i][jj]->Scale(scale[i]);

      if(i<nfile-1){
	H_MET_Nm1_djetMETPhim0p5[i][jj]->SetFillColor(cnum[i]);
	//H_MET_Nm1_METm100[i][jj]->SetFillColor(cnum[i]);
      }

      H_MET_Nm1_djetMETPhim0p5[i][jj]->SetLineColor(1);
      //H_MET_Nm1_METm100[i][jj]->SetLineColor(1);

      if(i == nfile-1){
	H_MET_Nm1_djetMETPhim0p5[i][jj]->SetLineWidth(2);
	//H_MET_Nm1_METm100[i][jj]->SetLineWidth(2);

	H_MET_Nm1_djetMETPhim0p5[i][jj]->SetMarkerStyle(8);
	//H_MET_Nm1_METm100[i][jj]->SetMarkerStyle(8);

	H_MET_Nm1_djetMETPhim0p5[i][jj]->SetMarkerSize(0.9);
	//H_MET_Nm1_METm100[i][jj]->SetMarkerSize(0.9);
      }
    }
    
  }

  for(Int_t i=0; i<nfile-1; i++){
    if(i>0){
      for(Int_t jj=0; jj<2; jj++){
	HA_MET_Nm1_djetMETPhim0p5[jj]->Add(H_MET_Nm1_djetMETPhim0p5[i][jj]);
	//HA_MET_Nm1_METm100[jj]->Add(H_MET_Nm1_METm100[i][jj]);
	
      }
    }
  }

  THStack *HS_MET_Nm1_djetMETPhim0p5[nhisto];
  THStack *HS_MET_Nm1_METm100[nhisto];

  for(Int_t jj=0; jj<2; jj++){
    //HS_MET_Nm1_djetMETPhim0p5[jj] = new THStack(Form("HS_MET_Nm1_djetMETPhim0p5_%i", jj), "HS_MET_Nm1_djetMETPhim0p5");
    HS_MET_Nm1_djetMETPhim0p5[jj] = new THStack(Form("HS_MET_Nm1_djetMETPhi_SB0p5_%i", jj), "HS_MET_Nm1_djetMETPhim0p5");
    //HS_MET_Nm1_METm100[jj] = new THStack(Form("HS_MET_Nm1_METm100_%i", jj), "HS_MET_Nm1_METm100");
  }

  for(Int_t i=0; i<nfile-1; i++){
    for(Int_t jj=0; jj<nhisto; jj++){
      HS_MET_Nm1_djetMETPhim0p5[jj]->Add(H_MET_Nm1_djetMETPhim0p5[i][jj]);
      //HS_MET_Nm1_METm100[jj]->Add(H_MET_Nm1_METm100[i][jj]);
    }
  }

  TLegend *lhs = new TLegend(0.52, 0.66, 0.92, 0.90);//VBS//VBS
  lhs->SetTextSize(0.035);
  lhs->SetFillStyle(0);
  //TString ltext[20] = {"#gamma+jet", "W(#rightarrowl#nu)+jets", "Z(#rightarrow#nu#nu)+jets", "Z(#rightarrowll)+jets", "Monoboson", "Diboson", "Triboson", "Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug", "DATA"};//Znunug QCD
  TString ltext[20] = {"#gamma+jet", "W(#rightarrowl#nu)+jets", "Z(#rightarrow#nu#nu)+jets", "Z(#rightarrowll)+jets", "Monoboson", "Diboson", "Triboson", "Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug(VBS)", "DATA"};//Znunug VBS

  TH1F *hratio;
  Double_t x[3] = {-2000, 0.0, 2000};
  Double_t y[3] = {1, 1, 1};
  TGraph *tg = new TGraph(3, x, y);
  tg->SetLineColor(kRed);
  tg->SetLineWidth(1);
  tg->SetLineStyle(8);

  TPad *pad1 = new TPad("pad1", "", 0., 0.3, 1., 1.);
  //pad1->SetTopMargin(0.03);
  pad1->SetBottomMargin(0);
  pad1->SetLogy(1);
  pad1->Draw();
  
  TPad *pad2 = new TPad("pad2", "", 0., 0.02, 1., 0.3);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.3);
  pad2->Draw();

  writeExtraText = true;
  Int_t period;
  if(year==2016){
    period = 1;
  }
  else if(year==2017){
    period = 2;
  }
  else if(year==2018){
    period = 3;
  }

  for(Int_t jj=0; jj<nhisto; jj++){

    pad1->cd();
    HS_MET_Nm1_djetMETPhim0p5[jj]->Draw("HIST");
    H_MET_Nm1_djetMETPhim0p5[nfile-1][jj]->Draw("SAME");
    HSig_MET_Nm1_djetMETPhim0p5[jj]->Draw("HISTSAME");
    HA_MET_Nm1_djetMETPhim0p5[jj]->Draw("E2 SAME");
    HS_MET_Nm1_djetMETPhim0p5[jj]->GetYaxis()->SetTitle("Events");
    HS_MET_Nm1_djetMETPhim0p5[jj]->GetXaxis()->SetTitle("");
    HS_MET_Nm1_djetMETPhim0p5[jj]->SetMaximum(100000);
    HS_MET_Nm1_djetMETPhim0p5[jj]->SetMinimum(1);
    if(jj==1){HS_MET_Nm1_djetMETPhim0p5[jj]->SetMaximum(100000); HS_MET_Nm1_djetMETPhim0p5[jj]->SetMinimum(1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      
      if(ifile == nfile-1) lhs->AddEntry(H_MET_Nm1_djetMETPhim0p5[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_MET_Nm1_djetMETPhim0p5[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_MET_Nm1_djetMETPhim0p5[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_MET_Nm1_djetMETPhim0p5[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_MET_Nm1_djetMETPhim0p5[jj], H_MET_Nm1_djetMETPhim0p5[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("MET(GeV)");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    /*
    pad1->cd();
    HS_MET_Nm1_METm100[jj]->Draw("HIST");
    H_MET_Nm1_METm100[nfile-1][jj]->Draw("SAME");
    HA_MET_Nm1_METm100[jj]->Draw("E2 SAME");
    HS_MET_Nm1_METm100[jj]->GetYaxis()->SetTitle("Events");
    HS_MET_Nm1_METm100[jj]->GetXaxis()->SetTitle("");
    HS_MET_Nm1_METm100[jj]->SetMaximum(100000);
    HS_MET_Nm1_METm100[jj]->SetMinimum(1);
    if(jj==1){HS_MET_Nm1_METm100[jj]->SetMaximum(100000); HS_MET_Nm1_METm100[jj]->SetMinimum(1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile < nfile-1) lhs->AddEntry(H_MET_Nm1_METm100[ifile][jj], ltext[ifile], "F");
      if(ifile == nfile-1) lhs->AddEntry(H_MET_Nm1_METm100[ifile][jj], ltext[ifile], "PL");
    }
    lhs->Draw("SAME");
    title = HS_MET_Nm1_METm100[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_MET_Nm1_METm100[jj], H_MET_Nm1_METm100[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("MET(GeV)");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    */
  }
  
}
