#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "./Drawlumi/CMS_lumi.C"

using namespace std;
#define nfile 5
#define nhisto 2
#define ncolor 10

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
  ratio->Draw("EP");
  
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

void xDrawStack(Int_t year){
  TString rootname[10];
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/output_merge_GJet.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Wg_aMCatNLO/output_ggtree.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Zg_aMCatNLO/output_ggtree.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/output_ggtree.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/output_merge_GJet.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Wg_aMCatNLO/output_ggtree.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Zg_aMCatNLO/output_ggtree.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/output_ggtree.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/fall17/data/output_ggtree.root";
  }
  
  Float_t lumi16 = 36.33;
  Float_t lumi17 = 41.48;
  Float_t lumi = 1.0;
  if(year==2016){
    lumi = lumi16;
  }
  else if(year==2017){
    lumi = lumi17;
  }
 
  Float_t xsec[10] = {1, 192.3, 123.3, 0.296};//[GJet, Wg, Zg, Znng]
  Float_t filter[10] = {1., 1., 1., 1.};
  Float_t kfactor[10] = {1.,  1., 1., 1.};
  //color name
  string hexcolor[ncolor] = {"#fff176", "#03a9f4", "#ff3360", "#ff9e00", "#00FFFF", "##8d94d8", "#67ccc1", "#0066ff", "#4caf50"};
  TColor *color[ncolor];
  Int_t cnum[ncolor];
   
  for(Int_t j=0; j<ncolor; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  } 

  
  
  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  //c1->Divide(1, 2);
  TTree *t;
  
  Float_t entries = 1.0;
  Float_t outentries = 0.;
  Float_t scale = 0.;

  TH1F *H_Events[10];
  TH1F *HSumofGenW[nfile];
  
  TH1F *H_MET_cut[nfile][nhisto];
  TH1F *H_dphoMETPhi_cut[nfile][nhisto];
  TH1F *H_njet_cut[nfile][nhisto];
  TH1F *H_jetpt_cut[nfile][nhisto];
  TH1F *H_djetMETPhi_cut[nfile][nhisto];
  TH1F *H_nvtx_cut[nfile][nhisto];

  TH1F *H_phoEB_ptcut[nfile][nhisto];
  TH1F *H_phoEB_ptoverMET[nfile][nhisto];
  TH1F *H_phoEB_ptoverjetpt[nfile][nhisto];

  TH1F *H_dr_phojet[nfile][nhisto];
  TH1F *H_dEta_phojet[nfile][nhisto];
  TH1F *H_dPhi_phojet[nfile][nhisto];
    
  
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";
  
  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    for(Int_t jj=0; jj<nhisto; jj++){
      if((i<nfile-1) && i>2){
	H_Events[i] = (TH1F*)fopen->Get("hEvents");
	HSumofGenW[i] = (TH1F*)fopen->Get("hSumofGenW");
	//entries = H_Events[i]->GetBinContent(1);
	entries = 1.0;
	entries = HSumofGenW[i]->GetBinContent(1);
	outentries = xsec[i]*kfactor[i]*1000*lumi*filter[i];
	scale = fabs(outentries/entries);
	cout << "print " << entries << " " << outentries << " " << scale<< endl;
      }
      if(i==nfile-1 || i<3) scale = 1.0;
      
      H_MET_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", jj));
      H_dphoMETPhi_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", jj));
      H_njet_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", jj));
      H_jetpt_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_jetpt_cut_%i", jj));
      H_djetMETPhi_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_djetMETPhi_cut_%i", jj));
      H_nvtx_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", jj));
    
      H_phoEB_ptcut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", jj));
      H_phoEB_ptoverMET[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverMET_cut_%i", jj));
      H_phoEB_ptoverjetpt[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverjetpt_%i", jj));
      H_dr_phojet[i][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_dr_phojet_jet%i", jj));
      H_dEta_phojet[i][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_dEta_phojet_jet%i", jj));
      H_dPhi_phojet[i][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_dPhi_phojet_jet%i", jj));
    
      //if(i< 4){
      H_MET_cut[i][jj]->Scale(scale);
      H_dphoMETPhi_cut[i][jj]->Scale(scale);
      H_njet_cut[i][jj]->Scale(scale);
      H_jetpt_cut[i][jj]->Scale(scale);
      H_djetMETPhi_cut[i][jj]->Scale(scale);
      H_nvtx_cut[i][jj]->Scale(scale);
      
      H_phoEB_ptcut[i][jj]->Scale(scale);
      H_phoEB_ptoverMET[i][jj]->Scale(scale);
      H_phoEB_ptoverjetpt[i][jj]->Scale(scale);
      H_dr_phojet[i][jj]->Scale(scale);
      H_dEta_phojet[i][jj]->Scale(scale);
      H_dPhi_phojet[i][jj]->Scale(scale);
      //}
    
      //fopen->Close();
      if(i<nfile-1){
	H_MET_cut[i][jj]->SetFillColor(cnum[i]);
	H_dphoMETPhi_cut[i][jj]->SetFillColor(cnum[i]);
	H_njet_cut[i][jj]->SetFillColor(cnum[i]);
	H_jetpt_cut[i][jj]->SetFillColor(cnum[i]);
	H_djetMETPhi_cut[i][jj]->SetFillColor(cnum[i]);
	H_nvtx_cut[i][jj]->SetFillColor(cnum[i]);
    
	H_phoEB_ptcut[i][jj]->SetFillColor(cnum[i]);
	H_phoEB_ptoverMET[i][jj]->SetFillColor(cnum[i]);
	H_phoEB_ptoverjetpt[i][jj]->SetFillColor(cnum[i]);
	H_dr_phojet[i][jj]->SetFillColor(cnum[i]);
	H_dEta_phojet[i][jj]->SetFillColor(cnum[i]);
	H_dPhi_phojet[i][jj]->SetFillColor(cnum[i]);
      }
      
	H_MET_cut[i][jj]->SetLineColor(1);
	H_dphoMETPhi_cut[i][jj]->SetLineColor(1);
	H_njet_cut[i][jj]->SetLineColor(1);
	H_jetpt_cut[i][jj]->SetLineColor(1);
	H_djetMETPhi_cut[i][jj]->SetLineColor(1);
	H_nvtx_cut[i][jj]->SetLineColor(1);
    
	H_phoEB_ptcut[i][jj]->SetLineColor(1);
	H_phoEB_ptoverMET[i][jj]->SetLineColor(1);
	H_phoEB_ptoverjetpt[i][jj]->SetLineColor(1);
	H_dr_phojet[i][jj]->SetLineColor(1);
	H_dEta_phojet[i][jj]->SetLineColor(1);
	H_dPhi_phojet[i][jj]->SetLineColor(1);
      
      if(i==nfile-1){
	H_MET_cut[i][jj]->SetLineWidth(2);
	H_dphoMETPhi_cut[i][jj]->SetLineWidth(2);
	H_njet_cut[i][jj]->SetLineWidth(2);
	H_jetpt_cut[i][jj]->SetLineWidth(2);
	H_djetMETPhi_cut[i][jj]->SetLineWidth(2);
	H_nvtx_cut[i][jj]->SetLineWidth(2);
	H_phoEB_ptcut[i][jj]->SetLineWidth(2);
	H_phoEB_ptoverMET[i][jj]->SetLineWidth(2);
	H_phoEB_ptoverjetpt[i][jj]->SetLineWidth(2);
	H_dr_phojet[i][jj]->SetLineWidth(2);
	H_dEta_phojet[i][jj]->SetLineWidth(2);
	H_dPhi_phojet[i][jj]->SetLineWidth(2);

	H_MET_cut[i][jj]->SetMarkerStyle(8);
	H_dphoMETPhi_cut[i][jj]->SetMarkerStyle(8);
	H_njet_cut[i][jj]->SetMarkerStyle(8);
	H_jetpt_cut[i][jj]->SetMarkerStyle(8);
	H_djetMETPhi_cut[i][jj]->SetMarkerStyle(8);
	H_nvtx_cut[i][jj]->SetMarkerStyle(8);
	H_phoEB_ptcut[i][jj]->SetMarkerStyle(8);
	H_phoEB_ptoverMET[i][jj]->SetMarkerStyle(8);
	H_phoEB_ptoverjetpt[i][jj]->SetMarkerStyle(8);
	H_dr_phojet[i][jj]->SetMarkerStyle(8);
	H_dEta_phojet[i][jj]->SetMarkerStyle(8);
	H_dPhi_phojet[i][jj]->SetMarkerStyle(8);
	
	H_MET_cut[i][jj]->SetMarkerSize(0.9);
	H_dphoMETPhi_cut[i][jj]->SetMarkerSize(0.9);
	H_njet_cut[i][jj]->SetMarkerSize(0.9);
	H_jetpt_cut[i][jj]->SetMarkerSize(0.9);
	H_djetMETPhi_cut[i][jj]->SetMarkerSize(0.9);
	H_nvtx_cut[i][jj]->SetMarkerSize(0.9);
	H_phoEB_ptcut[i][jj]->SetMarkerSize(0.9);
	H_phoEB_ptoverMET[i][jj]->SetMarkerSize(0.9);
	H_phoEB_ptoverjetpt[i][jj]->SetMarkerSize(0.9);
	H_dr_phojet[i][jj]->SetMarkerSize(0.9);
	H_dEta_phojet[i][jj]->SetMarkerSize(0.9);
	H_dPhi_phojet[i][jj]->SetMarkerSize(0.9);
	
	H_MET_cut[i][jj]->SetLineColor(kBlack);
	H_dphoMETPhi_cut[i][jj]->SetLineColor(kBlack);
	H_njet_cut[i][jj]->SetLineColor(kBlack);
	H_jetpt_cut[i][jj]->SetLineColor(kBlack);
	H_djetMETPhi_cut[i][jj]->SetLineColor(kBlack);
	H_nvtx_cut[i][jj]->SetLineColor(kBlack);
	H_phoEB_ptcut[i][jj]->SetLineColor(kBlack);
	H_phoEB_ptoverMET[i][jj]->SetLineColor(kBlack);
	H_phoEB_ptoverjetpt[i][jj]->SetLineColor(kBlack);
	H_dr_phojet[i][jj]->SetLineColor(kBlack);
	H_dEta_phojet[i][jj]->SetLineColor(kBlack);
	H_dPhi_phojet[i][jj]->SetLineColor(kBlack);
      }
    }
  }

  THStack *HS_MET_cut[2];
  THStack *HS_dphoMETPhi_cut[2];
  THStack *HS_njet_cut[2];
  THStack *HS_jetpt_cut[2];
  THStack *HS_djetMETPhi_cut[2];
  THStack *HS_nvtx_cut[2];

  THStack *HS_phoEB_ptcut[2];
  THStack *HS_phoEB_ptoverMET[2];
  THStack *HS_phoEB_ptoverjetpt[2];
  THStack *HS_dr_phojet[2];
  THStack *HS_dEta_phojet[2];
  THStack *HS_dPhi_phojet[2];
    
  for(Int_t jj=0; jj<2; jj++){					       
    HS_MET_cut[jj] = new THStack(Form("HS_MET_cut_%i", jj), "HS_MET_cut");
    HS_dphoMETPhi_cut[jj] = new THStack(Form("HS_dphoMETPhi_cut_%i", jj), "HS_dphoMETPhi_cut");
    HS_njet_cut[jj] = new THStack(Form("HS_njet_cut_%i", jj), "HS_njet_cut");
    HS_jetpt_cut[jj] = new THStack(Form("HS_jetpt_cut_%i", jj), "HS_jetpt_cut");
    HS_djetMETPhi_cut[jj] = new THStack(Form("HS_djetMETPhi_cut_%i", jj), "HS_djetMETPhi_cut");
    HS_nvtx_cut[jj] = new THStack(Form("HS_nvtx_cut_%i", jj), "HS_nvtx_cut");

    HS_phoEB_ptcut[jj] = new THStack(Form("HS_phoEB_ptcut_%i", jj), "HS_phoEB_ptcut");
    HS_phoEB_ptoverMET[jj] = new THStack(Form("HS_phoEB_ptoverMET_cut_%i", jj), "HS_phoEB_ptoverMET");
    HS_phoEB_ptoverjetpt[jj] = new THStack(Form("HS_phoEB_ptoverjetpt_%i", jj), "HS_phoEB_ptoverjetpt");
  }
					       
  
  for(Int_t i=0; i<nfile-1; i++){
    //if(i==4)continue;
    for(Int_t jj=0; jj<nhisto; jj++){
      HS_MET_cut[jj]->Add(H_MET_cut[i][jj]);
      HS_njet_cut[jj]->Add(H_njet_cut[i][jj]);
      //cout << "njets : " << H_njet_cut[i][0]->GetBinContent(1) << " " << H_njet_cut[i][0]->GetBinContent(2) << endl;
      HS_jetpt_cut[jj]->Add(H_jetpt_cut[i][jj]);
      HS_phoEB_ptcut[jj]->Add(H_phoEB_ptcut[i][jj]);
      HS_nvtx_cut[jj]->Add(H_nvtx_cut[i][jj]);
    
      c1->SetLogy();
      c1->Update();

      Double_t ufbin = H_dphoMETPhi_cut[i][jj]->GetBinContent(0) + H_dphoMETPhi_cut[i][jj]->GetBinContent(1);
      Double_t ofbin = H_dphoMETPhi_cut[i][jj]->GetBinContent(61) + H_dphoMETPhi_cut[i][jj]->GetBinContent(60);
      H_dphoMETPhi_cut[i][jj]->SetBinContent(1,ufbin);
      H_dphoMETPhi_cut[i][jj]->SetBinContent(60,ofbin);
      
      HS_dphoMETPhi_cut[jj]->Add(H_dphoMETPhi_cut[i][jj]);
      HS_djetMETPhi_cut[jj]->Add(H_djetMETPhi_cut[i][jj]);
      HS_phoEB_ptoverMET[jj]->Add(H_phoEB_ptoverMET[i][jj]);    
      HS_phoEB_ptoverjetpt[jj]->Add(H_phoEB_ptoverjetpt[i][jj]);
    }
  }

  TLegend *lhs = new TLegend(0.63,0.63, 0.75, 0.90);
  lhs->SetTextSize(0.04);
  lhs->SetFillStyle(0);
  TString ltext[10] = {"#gamma+jet", "W+jets", "Z+jets","Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug", "DATA"};
  
  //HS_dphoMETPhi->Draw("HIST");
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
  for(Int_t jj=0; jj<2; jj++){
    pad1->cd();
    HS_MET_cut[jj]->Draw("HIST");
    H_MET_cut[nfile-1][jj]->Draw("SAME");
    HS_MET_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_MET_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_MET_cut[jj]->GetXaxis()->SetTitle("");
    HS_MET_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_MET_cut[jj]->SetMaximum(1000);
    HS_MET_cut[jj]->SetMinimum(1);
    if(jj==1){HS_MET_cut[jj]->SetMaximum(100); HS_MET_cut[jj]->SetMinimum(0.001);}
    lhs->Clear();
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile < nfile-1) lhs->AddEntry(H_MET_cut[ifile][jj], ltext[ifile], "F");
      if(ifile == nfile-1) lhs->AddEntry(H_MET_cut[ifile][jj], ltext[ifile], "PL");
    }
    //lhs->AddEntry(H_MET_cut[0][jj], "#gamma+jet", "F");
    //lhs->AddEntry(H_MET_cut[1][jj], "W+jets", "F");
    //lhs->AddEntry(H_MET_cut[2][jj], "Z+jets", "F");
    //lhs->AddEntry(H_MET_cut[3][jj], "Wg#rightarrowl#nug", "F");
    //lhs->AddEntry(H_MET_cut[4][jj], "Zg#rightarrowllg", "F");
    //lhs->AddEntry(H_MET_cut[5][jj], "Zg#rightarrow#nu#nug", "F");
    //lhs->AddEntry(H_MET_cut[6][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_MET_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_MET_cut[jj], H_MET_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("MET");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    //c1->GetFrame()->Draw();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();  
    HS_dphoMETPhi_cut[jj]->Draw("HIST");
    H_dphoMETPhi_cut[nfile-1][jj]->Draw("SAME");
    HS_dphoMETPhi_cut[jj]->SetMinimum(1);
    HS_dphoMETPhi_cut[jj]->SetMaximum(1000);//2016
    HS_dphoMETPhi_cut[jj]->SetMaximum(10000);//2017
    //if(jj==1){HS_dphoMETPhi_cut[jj]->SetMaximum(10); HS_dphoMETPhi_cut[jj]->SetMinimum(0.001);}
    if(jj==1){HS_dphoMETPhi_cut[jj]->SetMaximum(100); HS_dphoMETPhi_cut[jj]->SetMinimum(0.001);}//2017
    HS_dphoMETPhi_cut[jj]->GetXaxis()->SetTitle("");
    HS_dphoMETPhi_cut[jj]->GetYaxis()->SetTitle("Events");
    //HS_dphoMETPhi_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    lhs->Clear();
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile < nfile-1) lhs->AddEntry(H_dphoMETPhi_cut[ifile][jj], ltext[ifile], "F");
      if(ifile == nfile-1) lhs->AddEntry(H_dphoMETPhi_cut[ifile][jj], ltext[ifile], "PL");
    }
    //lhs->AddEntry(H_dphoMETPhi_cut[0][jj], "#gamma+jet", "F");
    //lhs->AddEntry(H_dphoMETPhi_cut[1][jj], "Wg#rightarrowl#nug", "F");
    //lhs->AddEntry(H_dphoMETPhi_cut[2][jj], "Zg#rightarrowllg", "F");
    //lhs->AddEntry(H_dphoMETPhi_cut[3][jj], "Zg#rightarrow#nu#nug", "F");
    //lhs->AddEntry(H_dphoMETPhi_cut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_dphoMETPhi_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dphoMETPhi_cut[jj], H_dphoMETPhi_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#Delta#phi(#gamma,MET)");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_njet_cut[jj]->Draw("HIST");
    H_njet_cut[nfile-1][jj]->Draw("SAME");
    HS_njet_cut[jj]->GetXaxis()->SetTitle("");
    HS_njet_cut[jj]->GetYaxis()->SetTitle("Events");
    //HS_njet_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_njet_cut[jj]->SetMaximum(5000);
    HS_njet_cut[jj]->SetMinimum(10);
    if(jj==1){HS_njet_cut[jj]->SetMaximum(1000); HS_njet_cut[jj]->SetMinimum(0.01);}
    lhs->Clear();
     for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile < nfile-1) lhs->AddEntry(H_njet_cut[ifile][jj], ltext[ifile], "F");
      if(ifile == nfile-1) lhs->AddEntry(H_njet_cut[ifile][jj], ltext[ifile], "PL");
     }
    //lhs->AddEntry(H_njet_cut[0][jj], "#gamma+jet", "F");
    //lhs->AddEntry(H_njet_cut[1][jj], "Wg#rightarrowl#nug", "F");
    //lhs->AddEntry(H_njet_cut[2][jj], "Zg#rightarrowllg", "F");
    //lhs->AddEntry(H_njet_cut[3][jj], "Zg#rightarrow#nu#nug", "F");
    //lhs->AddEntry(H_njet_cut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_njet_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_njet_cut[jj], H_njet_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("njet");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    
    pad1->cd();
    HS_jetpt_cut[jj]->Draw("HIST");
    H_jetpt_cut[nfile-1][jj]->Draw("SAME");
    HS_jetpt_cut[jj]->GetXaxis()->SetTitle("");
    HS_jetpt_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_jetpt_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_jetpt_cut[jj]->SetMaximum(1000);
    HS_jetpt_cut[jj]->SetMinimum(0.1);
    lhs->Clear();
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile < nfile-1) lhs->AddEntry(H_jetpt_cut[ifile][jj], ltext[ifile], "F");
      if(ifile == nfile-1) lhs->AddEntry(H_jetpt_cut[ifile][jj], ltext[ifile], "PL");
    }
    //lhs->AddEntry(H_jetpt_cut[0][jj], "#gamma+jet", "F");
    //lhs->AddEntry(H_jetpt_cut[1][jj], "Wg#rightarrowl#nug", "F");
    //lhs->AddEntry(H_jetpt_cut[2][jj], "Zg#rightarrowllg", "F");
    //lhs->AddEntry(H_jetpt_cut[3][jj], "Zg#rightarrow#nu#nug", "F");
    //lhs->AddEntry(H_jetpt_cut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_jetpt_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_jetpt_cut[jj], H_jetpt_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("P_{T}^{jet}");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_djetMETPhi_cut[jj]->Draw("HIST");
    H_djetMETPhi_cut[nfile-1][jj]->Draw("SAME");
    HS_djetMETPhi_cut[jj]->GetXaxis()->SetTitle("");
    HS_djetMETPhi_cut[jj]->GetYaxis()->SetTitle("Events");
    // HS_djetMETPhi_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    //HS_djetMETPhi_cut[jj]->SetMaximum(1000);//2016
    HS_djetMETPhi_cut[jj]->SetMaximum(10000);//2017
    
    HS_djetMETPhi_cut[jj]->SetMinimum(0.5);
    lhs->Clear();
     for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile < nfile-1) lhs->AddEntry(H_djetMETPhi_cut[ifile][jj], ltext[ifile], "F");
      if(ifile == nfile-1) lhs->AddEntry(H_djetMETPhi_cut[ifile][jj], ltext[ifile], "PL");
     }
    //lhs->AddEntry(H_djetMETPhi_cut[0][jj], "#gamma+jet", "F");
    //lhs->AddEntry(H_djetMETPhi_cut[1][jj], "Wg#rightarrowl#nug", "F");
    //lhs->AddEntry(H_djetMETPhi_cut[2][jj], "Zg#rightarrowllg", "F");
    //lhs->AddEntry(H_djetMETPhi_cut[3][jj], "Zg#rightarrow#nu#nug", "F");
    //lhs->AddEntry(H_djetMETPhi_cut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_djetMETPhi_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_djetMETPhi_cut[jj], H_djetMETPhi_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#Delta#phi(jet, MET)");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    //HS_dphoMETPhi->Draw("HIST");

    pad1->cd();
    HS_nvtx_cut[jj]->Draw("HIST");
    H_nvtx_cut[nfile-1][jj]->Draw("SAME");
    HS_nvtx_cut[jj]->GetYaxis()->SetTitle("Events");
    //HS_nvtx_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_nvtx_cut[jj]->GetXaxis()->SetTitle("");
    HS_nvtx_cut[jj]->GetXaxis()->SetLabelSize(0);
    //HS_nvtx_cut[jj]->SetMaximum(1000);//2016
    HS_nvtx_cut[jj]->SetMaximum(10000);//2017
    HS_nvtx_cut[jj]->SetMinimum(1);
    //if(jj==1){HS_nvtx_cut[jj]->SetMaximum(100); HS_nvtx_cut[jj]->SetMinimum(0.001);}
    if(jj==1){HS_nvtx_cut[jj]->SetMaximum(1000); HS_nvtx_cut[jj]->SetMinimum(0.001);}//2017
    lhs->Clear();
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile < nfile-1) lhs->AddEntry(H_nvtx_cut[ifile][jj], ltext[ifile], "F");
      if(ifile == nfile-1) lhs->AddEntry(H_nvtx_cut[ifile][jj], ltext[ifile], "PL");
    }
    //lhs->AddEntry(H_nvtx_cut[0][jj], "#gamma+jet", "F");
    //lhs->AddEntry(H_nvtx_cut[1][jj], "Wg#rightarrowl#nug", "F");
    //lhs->AddEntry(H_nvtx_cut[2][jj], "Zg#rightarrowllg", "F");
    //lhs->AddEntry(H_nvtx_cut[3][jj], "Zg#rightarrow#nu#nug", "F");
    //lhs->AddEntry(H_nvtx_cut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_nvtx_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_nvtx_cut[jj], H_nvtx_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("nVtx");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    /*
    pad1->cd();
    HS_phoEB_ptoverjetpt[jj]->Draw("HIST");
    H_phoEB_ptoverjetpt[nfile-1][jj]->Draw("APE SAME");
    HS_phoEB_ptoverjetpt[jj]->GetXaxis()->SetTitle("");
    HS_phoEB_ptoverjetpt[jj]->GetYaxis()->SetTitle("Events");
    HS_phoEB_ptoverjetpt[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_ptoverjetpt[jj]->SetMaximum(1000);
    HS_phoEB_ptoverjetpt[jj]->SetMinimum(0.01);
    lhs->Clear();
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile < nfile-1) lhs->AddEntry(H_phoEB_ptoverjetpt[ifile][jj], ltext[ifile], "F");
      if(ifile == nfile-1) lhs->AddEntry(H_phoEB_ptoverjetpt[ifile][jj], ltext[ifile], "PL");
    }
    //lhs->AddEntry(H_phoEB_ptoverjetpt[0][jj], "#gamma+jet", "F");
    //lhs->AddEntry(H_phoEB_ptoverjetpt[1][jj], "Wg#rightarrowl#nug", "F");
    //lhs->AddEntry(H_phoEB_ptoverjetpt[2][jj], "Zg#rightarrowllg", "F");
    //lhs->AddEntry(H_phoEB_ptoverjetpt[3][jj], "Zg#rightarrow#nu#nug", "F");
    //lhs->AddEntry(H_phoEB_ptoverjetpt[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    //HS_phoEB_ptoverjetpt[jj]->GetYaxis()->SetRangeUser(0.01, 100);
    title = HS_phoEB_ptoverjetpt[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_ptoverjetpt[jj], H_phoEB_ptoverjetpt[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("P_{T}^{#gamma}/P_{T}^{jet}");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title)); 
    */
  
    pad1->cd();
    HS_phoEB_ptcut[jj]->Draw("HIST");
    H_phoEB_ptcut[nfile-1][jj]->Draw("AP SAME");
    HS_phoEB_ptcut[jj]->GetXaxis()->SetTitle("");
    HS_phoEB_ptcut[jj]->GetYaxis()->SetTitle("Events");
    //HS_phoEB_ptcut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_ptcut[jj]->GetXaxis()->SetRangeUser(100, 900);
    HS_phoEB_ptcut[jj]->SetMaximum(10000);
    HS_phoEB_ptcut[jj]->SetMinimum(0.1);
    if(jj==1){HS_phoEB_ptcut[jj]->SetMinimum(0.001);}
    lhs->Clear();
     for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile < nfile-1) lhs->AddEntry(H_phoEB_ptcut[ifile][jj], ltext[ifile], "F");
      if(ifile == nfile-1) lhs->AddEntry(H_phoEB_ptcut[ifile][jj], ltext[ifile], "PL");
     }
    //lhs->AddEntry(H_phoEB_ptcut[0][jj], "#gamma+jet", "F");
    //lhs->AddEntry(H_phoEB_ptcut[1][jj], "Wg#rightarrowl#nug", "F");
    //lhs->AddEntry(H_phoEB_ptcut[2][jj], "Zg#rightarrowllg", "F");
    //lhs->AddEntry(H_phoEB_ptcut[3][jj], "Zg#rightarrow#nu#nug", "F");
    //lhs->AddEntry(H_phoEB_ptcut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_phoEB_ptcut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_ptcut[jj], H_phoEB_ptcut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("P_{T}^{#gamma}");
    hratio->Draw("EP");
    hratio->GetXaxis()->SetRangeUser(100, 900);
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_phoEB_ptoverMET[jj]->Draw("HIST");
    H_phoEB_ptoverMET[nfile-1][jj]->Draw("APE SAME");
    HS_phoEB_ptoverMET[jj]->GetXaxis()->SetTitle("");
    HS_phoEB_ptoverMET[jj]->GetYaxis()->SetTitle("Events");
    HS_phoEB_ptoverMET[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_ptoverMET[jj]->SetMaximum(50000);
    HS_phoEB_ptoverMET[jj]->SetMinimum(0.1);
    if(jj==1){HS_phoEB_ptoverMET[jj]->SetMaximum(1000); HS_phoEB_ptoverMET[jj]->SetMinimum(0.001);}
    lhs->Clear();
     for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile < nfile-1) lhs->AddEntry(H_phoEB_ptoverMET[ifile][jj], ltext[ifile], "F");
      if(ifile == nfile-1) lhs->AddEntry(H_phoEB_ptoverMET[ifile][jj], ltext[ifile], "PL");
     }
    //lhs->AddEntry(H_phoEB_ptoverMET[0][jj], "#gamma+jet", "F");
    //lhs->AddEntry(H_phoEB_ptoverMET[1][jj], "Wg#rightarrowl#nug", "F");
    //lhs->AddEntry(H_phoEB_ptoverMET[2][jj], "Zg#rightarrowllg", "F");
    //lhs->AddEntry(H_phoEB_ptoverMET[3][jj], "Zg#rightarrow#nu#nug", "F");
    //lhs->AddEntry(H_phoEB_ptoverMET[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    //HS_phoEB_ptoverMET[jj]->GetYaxis()->SetRangeUser(0.01, 100);
    title = HS_phoEB_ptoverMET[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_ptoverMET[jj], H_phoEB_ptoverMET[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("P_{T}^{#gamma}/MET");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  }  
}
