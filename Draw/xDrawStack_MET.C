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

void xDrawStack_MET(Int_t year){
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
 
  Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 0.296};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, Znng]
  //Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 0.018};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, ZnngVBS]
  
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
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/GJet/GJet_template.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_TT_aMCatNLO/output_ggtree.root";
    rootname[10] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_UL17_Znunug_VBS/output_ggtree.root";
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

  TH1F *H_MET_cut[nfile][nhisto];
  TH1F *HA_MET_cut[nhisto];

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    if(i==nfile-1) scale[i] = 1.0;

    for(Int_t jj=0; jj<nhisto; jj++){
      if(i==0) H_MET_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_corr_%i", jj));
      else H_MET_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", jj));

      if(i==0){
	HA_MET_cut[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_corr_%i", jj))->Clone();

	HA_MET_cut[jj]->Scale(scale[i]);
	HA_MET_cut[jj]->SetFillStyle(3145);
	HA_MET_cut[jj]->SetFillColor(12);
	HA_MET_cut[jj]->SetMarkerSize(0.1);
      }
      
      H_MET_cut[i][jj]->Scale(scale[i]);
      if(i<nfile-1){
	H_MET_cut[i][jj]->SetFillColor(cnum[i]);
      }
      H_MET_cut[i][jj]->SetLineColor(1);
      H_MET_cut[i][jj]->SetLineWidth(1);
      if(i == nfile-1){
	H_MET_cut[i][jj]->SetLineWidht(2);
	H_MET_cut[i][jj]->SetMarkerStyel(8);
	H_MET_cut[i][jj]->SetMarkerSize(0.9);
      }
    }
    
  }

  for(Int_t i=0; i<nfile-1; i++){
    if(i>0){
      for(Int_t jj=0; jj<2; jj++){
	HA_MET_cut[jj]->Add(H_MET_cut[i][jj];)
      }
    }
  }

  THStack *HS_MET_cut[nhisto];

  for(Int_t jj=0; jj<2; jj++){
    HS_MET_cut[jj] = new THStack(Form("HS_MET_cut_%i", jj), "HS_MET_cut");
    
  }

  for(Int_t i=0; i<nfile-1; i++){
    for(Int_t jj=0; jj<nhisto; jj++){
      HS_MET_cut[jj]->Add(H_MET_cut[i][jj]);
    }
  }

  TLegend *lhs = new TLegend(0.58, 0.7, 0.92, 0.90);//VBS
  lhs->SetTextSize(0.035);
  lhs->SetFillStyle(0);
  TString ltext[20] = {"#gamma+jet", "W(#rightarrowl#nu)+jets", "Z(#rightarrow#nu#nu)+jets", "Z(#rightarrowll)+jets", "Monoboson", "Diboson", "Triboson", "Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug", "DATA"};//Znunug QCD
  //TString ltext[20] = {"#gamma+jet", "W(#rightarrowl#nu)+jets", "Z(#rightarrow#nu#nu)+jets", "Z(#rightarrowll)+jets", "Monoboson", "Diboson", "Triboson", "Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug(VBS)", "DATA"};//Znunug VBS

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

  for(Int_t ifile=0; ifile<nfile; ifile++){
    pad1->cd();
    HS_MET_cut[jj]->Draw("HIST");
    H_MET_cut[nfile-1][jj]->Draw("SAME");
    HA_MET_cut[jj]->Draw("E2 SAME");
    HS_MET_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_MET_cut[jj]->GetXaxis()->SetTitle("");
    //HS_MET_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_MET_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_MET_cut[jj]->SetMaximum(10000);
    HS_MET_cut[jj]->SetMinimum(1);
    if(jj==1){HS_MET_cut[jj]->SetMaximum(1000); HS_MET_cut[jj]->SetMinimum(0.01);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile < nfile-1) lhs->AddEntry(H_MET_cut[ifile][jj], ltext[ifile], "F");
      if(ifile == nfile-1) lhs->AddEntry(H_MET_cut[ifile][jj], ltext[ifile], "PL");
    }
    lhs->Draw("SAME");
    title = HS_MET_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_MET_cut[jj], H_MET_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("E_{T}^{miss}");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  }
}
