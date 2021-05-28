#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
//#define nfile 4
#define nfile 8
void xDrawCutflow_GJet(){
  ofstream ftext;
  /*
  TString rootname[5] = {"/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_GJet_Pt-15To6000_TuneCUETP8M1-Flat_13TeV_pythia8_20M_0000/210505_040352/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_WGToLNuG_01J_5f_PtG_130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0000/210505_082513/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_ZLLGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph_0000/210505_064501/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_ZGTo2NuG_PtG-130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0000/210505_064756/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]
  */
  
  TString rootname[10] = {
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_CRABdata_sigMC_HT-100To200_13TeV-madgraphMLM-pythia8_v3_ext1-v2_16_05_2021/210526_104024/output_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_CRABdata_sigMC_HT-100To200_13TeV-madgraphMLM-pythia8_v3-v2_16_05_2021/210526_110049/output_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_CRABdata_sigMC_HT-200To400_13TeV-madgraphMLM-pythia8_v3_ext1-v2_16_05_2021/210526_112038/output_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_CRABdata_sigMC_HT-200To400_13TeV-madgraphMLM-pythia8_v3-v2_16_05_2021/210526_114235/output_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_CRABdata_sigMC_HT-400To600_13TeV-madgraphMLM-pythia8_v3_ext1-v2_16_05_2021/210526_120446/output_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_CRABdata_sigMC_HT-400To600_13TeV-madgraphMLM-pythia8_v3-v2_16_05_2021/210526_122518/output_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_CRABdata_sigMC_HT-600ToInf_13TeV-madgraphMLM-pythia8_v3_ext1-v2_16_05_2021/210526_124750/output_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_CRABdata_sigMC_HT-600ToInf_13TeV-madgraphMLM-pythia8_v3-v2_16_05_2021/210526_131104/output_ggtree.root"
			 
  };//8 gjet
  
 

  Float_t mcXsec[10] = {9319, 9155, 2323, 2314, 278.5, 271.8, 93.85, 94.7};
  //Float_t mcXsec[10] = {2323, 2314, 278.5, 271.8, 93.85, 94.7};
  Float_t lumi16 = 36.33;
  Float_t Xsec = 2323.0;

  string hexcolor[8] = {"#7FB5F5","#2F81A3","#1C2A60","#FA6D5F","#91A81E","#DCF563", "#1C6040","#EFE2C9"};
  TColor *color[8];
  Int_t cnum[8];
  for(Int_t j=0; j<8; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  }

  TH1F *H_Events[nfile];
  TH1F *H_phoEB_pt[nfile];
  TH1F *H_phoEB_pt_HLT[nfile];
  TH1F *H_phoEB_pt_200[nfile];
  TH1F *H_phoEB_pt_M[nfile];
  //TH1F *H_phoEB_pt_r9[nfile];
  TH1F *H_phoEB_pt_dphoMETPhi[nfile];
  TH1F *H_phoEB_pt_MET[nfile];

  Double_t ptbin[30] = {22, 30, 36, 50, 75, 90, 120, 170, 175, 180, 185, 190, 200,
		     210, 220, 230, 250, 300, 350, 400, 500, 750, 1000, 1500, 2000, 3000, 10000};//24 bins, 2016
  TH1F *HA_phoEB_pt = new TH1F("HA_phoEB_pt", "HA_phoEB_pt", 24, ptbin);
  TH1F *HA_phoEB_pt_HLT = new TH1F("HA_phoEB_pt_HLT", "HA_phoEB_pt_HLT", 24, ptbin);
  TH1F *HA_phoEB_pt_200 = new TH1F("HA_phoEB_pt_200", "HA_phoEB_pt_200", 24, ptbin);
  TH1F *HA_phoEB_pt_M = new TH1F("HA_phoEB_pt_M", "HA_phoEB_pt_M", 24, ptbin);
  TH1F *HA_phoEB_pt_dphoMETPhi = new TH1F("HA_phoEB_pt_dphoMETPhi", "HA_phoEB_pt_dphoMETPhi", 24, ptbin);
  TH1F *HA_phoEB_pt_MET = new TH1F("HA_phoEB_pt_MET", "HA_phoEB_pt_MET", 24, ptbin);

  TGraphAsymmErrors *tgAE_phoEB_pt_HLT;
  TGraphAsymmErrors *tgAE_phoEB_pt_200;
  TGraphAsymmErrors *tgAE_phoEB_pt_M;
  TGraphAsymmErrors *tgAE_phoEB_pt_r9;
  TGraphAsymmErrors *tgAE_phoEB_pt_dphoMETPhi;
  TGraphAsymmErrors *tgAE_phoEB_pt_MET;

  TGraphAsymmErrors *tgAE_phoEB_pt_Meff;

  TLegend *ltgAE = new TLegend(0.65,0.65, 0.75, 0.75);
  ltgAE->SetBorderSize(0);
  ltgAE->SetFillStyle(0);
  TArrow *ArtgAE = new TArrow(205, 0.001, 205, 0.90, 0.03, "|>");
  ArtgAE->SetAngle(40);
  ArtgAE->SetLineWidth(1);
  
  
  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  TTree *t;
  Int_t entries;
  Float_t outentries = 0.;
  Float_t scale = 0.;
  
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    t = (TTree*)fopen->Get("t");

    H_Events[i] = (TH1F*)fopen->Get("hEvents");
    entries = H_Events[i]->GetBinContent(1);
    outentries = (Xsec/mcXsec[i])*entries;
    scale = outentries/entries;
     
    H_phoEB_pt[i] = (TH1F*)fopen->Get("h_phoEB_pt");
    H_phoEB_pt_HLT[i] = (TH1F*)fopen->Get("h_phoEB_pt_HLT");
    H_phoEB_pt_200[i] = (TH1F*)fopen->Get("h_phoEB_pt_200");
    H_phoEB_pt_M[i] = (TH1F*)fopen->Get("h_phoEB_pt_M");
    //H_phoEB_pt_r9[i] = (TH1F*)fopen->Get("h_phoEB_pt_r9");
    H_phoEB_pt_dphoMETPhi[i] = (TH1F*)fopen->Get("h_phoEB_pt_dphoMETPhi");
    H_phoEB_pt_MET[i] = (TH1F*)fopen->Get("h_phoEB_pt_MET");

    H_phoEB_pt[i]->Scale(scale);
    H_phoEB_pt_HLT[i]->Scale(scale);
    H_phoEB_pt_200[i]->Scale(scale);
    H_phoEB_pt_M[i]->Scale(scale);
    //H_phoEB_pt_r9[i] = (TH1F*)fopen->Get("h_phoEB_pt_r9");
    H_phoEB_pt_dphoMETPhi[i]->Scale(scale);
    H_phoEB_pt_MET[i]->Scale(scale);
  }

  for(Int_t i=0; i<nfile; i++){
    HA_phoEB_pt->Add(H_phoEB_pt[i]);
    HA_phoEB_pt_200->Add(H_phoEB_pt_200[i]);
    HA_phoEB_pt_M->Add(H_phoEB_pt_M[i]);
    HA_phoEB_pt_dphoMETPhi->Add(H_phoEB_pt_dphoMETPhi[i]);
    HA_phoEB_pt_MET->Add(H_phoEB_pt_MET[i]);
    HA_phoEB_pt_HLT->Add(H_phoEB_pt_HLT[i]);
  }

  HA_phoEB_pt->Draw();
  title = HA_phoEB_pt->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  HA_phoEB_pt_200->Draw();
  title = HA_phoEB_pt_200->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  HA_phoEB_pt_M->Draw();
  title = HA_phoEB_pt_M->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  HA_phoEB_pt_dphoMETPhi->Draw();
  title = HA_phoEB_pt_dphoMETPhi->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  HA_phoEB_pt_MET->Draw();
  title = HA_phoEB_pt_MET->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  HA_phoEB_pt_HLT->Draw();
  title = HA_phoEB_pt_HLT->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  tgAE_phoEB_pt_200 = new TGraphAsymmErrors();
  tgAE_phoEB_pt_M = new TGraphAsymmErrors();
  tgAE_phoEB_pt_dphoMETPhi = new TGraphAsymmErrors();
  tgAE_phoEB_pt_MET = new TGraphAsymmErrors();
  tgAE_phoEB_pt_Meff = new TGraphAsymmErrors();
  tgAE_phoEB_pt_HLT = new TGraphAsymmErrors();

  tgAE_phoEB_pt_200->Divide(HA_phoEB_pt_200, HA_phoEB_pt);
  tgAE_phoEB_pt_M->Divide(HA_phoEB_pt_M, HA_phoEB_pt);
  tgAE_phoEB_pt_dphoMETPhi->Divide(HA_phoEB_pt_dphoMETPhi, HA_phoEB_pt);
  tgAE_phoEB_pt_MET->Divide(HA_phoEB_pt_MET, HA_phoEB_pt);
  tgAE_phoEB_pt_Meff->Divide(HA_phoEB_pt_M, HA_phoEB_pt_200);
  tgAE_phoEB_pt_HLT->Divide(HA_phoEB_pt_HLT, HA_phoEB_pt_MET);
  
  tgAE_phoEB_pt_200->SetNameTitle("tgAE_phoEB_pteff_200", "pho cut pt200");
  tgAE_phoEB_pt_M->SetNameTitle("tgAE_phoEB_pteff_M", "pho cut M ID");
  tgAE_phoEB_pt_dphoMETPhi->SetNameTitle("tgAE_phoEB_pteff_dphoMETPhi", "pho cut dphoMETPhi");
  tgAE_phoEB_pt_MET->SetNameTitle("tgAE_phoEB_pteff_MET", "pho cut MET");
  tgAE_phoEB_pt_Meff->SetNameTitle("tgAE_phoEB_pt_Meff", "pho M ID bit eff");
  tgAE_phoEB_pt_HLT->SetNameTitle("tgAE_phoEB_pteff_HLT", "pho cut HLT");

  tgAE_phoEB_pt_HLT->SetMarkerStyle(8);
  tgAE_phoEB_pt_200->SetMarkerStyle(8);
  tgAE_phoEB_pt_M->SetMarkerStyle(8);
  //tgAE_phoEB_pt_r9->SetMarkerStyle(8);
  tgAE_phoEB_pt_dphoMETPhi->SetMarkerStyle(8);
  tgAE_phoEB_pt_MET->SetMarkerStyle(8);
  tgAE_phoEB_pt_Meff->SetMarkerStyle(8);
    
  tgAE_phoEB_pt_HLT->SetMarkerColor(cnum[4]);
  tgAE_phoEB_pt_200->SetMarkerColor(cnum[0]);
  tgAE_phoEB_pt_M->SetMarkerColor(cnum[1]);
  tgAE_phoEB_pt_dphoMETPhi->SetMarkerColor(cnum[2]);
  tgAE_phoEB_pt_MET->SetMarkerColor(cnum[3]);
  tgAE_phoEB_pt_Meff->SetMarkerColor(cnum[1]);

  tgAE_phoEB_pt_HLT->Draw("AP");
  tgAE_phoEB_pt_HLT->GetXaxis()->SetTitle("P_{T}^{#gamma}");
  tgAE_phoEB_pt_HLT->GetYaxis()->SetTitle("Efficiency");
  tgAE_phoEB_pt_HLT->GetXaxis()->SetLimits(0, 500);
  tgAE_phoEB_pt_HLT->SetMaximum(1.05);
  tgAE_phoEB_pt_HLT->SetMinimum(0.0);
  ltgAE->Clear();
  ltgAE->AddEntry(tgAE_phoEB_pt_HLT, "HLT_175", "PE");
  ltgAE->Draw("SAME");
  //tgAE_phoEB_pt_HLT->GetPoint(turnpoint, x, y);
  //ArtgAE = new TArrow(x, 0.001, x, y-0.01, 0.03, "|>");
  //ArtgAE->SetAngle(40);
  //ArtgAE->SetLineWidth(1);
  //ArtgAE->Draw();
  title = tgAE_phoEB_pt_HLT->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  tgAE_phoEB_pt_200->Draw("AP");
  title = tgAE_phoEB_pt_200->GetName();
  tgAE_phoEB_pt_M->Draw("AP");
  title = tgAE_phoEB_pt_M->GetName();
  tgAE_phoEB_pt_dphoMETPhi->Draw("AP");
  title = tgAE_phoEB_pt_dphoMETPhi->GetName(); 
  tgAE_phoEB_pt_MET->Draw("AP");
  title = tgAE_phoEB_pt_MET->GetName();
  
  tgAE_phoEB_pt_Meff->Draw("AP");
  tgAE_phoEB_pt_Meff->GetXaxis()->SetTitle("P_{T}^{#gamma}");
  tgAE_phoEB_pt_Meff->GetYaxis()->SetTitle("Efficiency");
  tgAE_phoEB_pt_Meff->GetXaxis()->SetLimits(0, 500);
  tgAE_phoEB_pt_Meff->SetMaximum(1.05);
  tgAE_phoEB_pt_Meff->SetMinimum(0.0);
  ltgAE->Clear();
  ltgAE->AddEntry(tgAE_phoEB_pt_Meff, "pho M ID", "PE");
  ltgAE->Draw("SAME");
  title = tgAE_phoEB_pt_Meff->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TMultiGraph *mg;
  TLegend *lmg = new TLegend(0.15,0.70, 0.25, 0.88);
  lmg->SetBorderSize(0);
  lmg->SetFillStyle(0);

  TF1 *fitresult = new TF1("fitresult", "pol0", 200, 500);
  Double_t yinter[10], err[10];
  
  mg = new TMultiGraph();
  //mg->Add(tgAE_phoEB_pt_HLT);
  tgAE_phoEB_pt_200->SetMarkerColor(cnum[0]);
  mg->Add(tgAE_phoEB_pt_200);
  tgAE_phoEB_pt_M->SetMarkerColor(cnum[1]);
  mg->Add(tgAE_phoEB_pt_M);
  //tgAE_phoEB_pt_r9->SetMarkerColor(cnum[1]);
  //mg->Add(tgAE_phoEB_pt_r9);
  tgAE_phoEB_pt_dphoMETPhi->SetMarkerColor(cnum[2]);
  mg->Add(tgAE_phoEB_pt_dphoMETPhi);
  tgAE_phoEB_pt_MET->SetMarkerColor(cnum[3]);
  mg->Add(tgAE_phoEB_pt_MET);

  mg->GetXaxis()->SetTitle("phoEt#[]{GeV}");
  mg->GetYaxis()->SetTitle("Efficiency");
  mg->GetXaxis()->SetLimits(-1, 500);
  mg->SetMaximum(1.05);
  mg->SetTitle("cutflow");
  mg->Draw("AP");
  lmg->Clear();
  //lmg->AddEntry(tgAE_phoEB_pt_HLT, "HLT_175", "PE");
  lmg->AddEntry(tgAE_phoEB_pt_200, "pt > 200", "PE");
  lmg->AddEntry(tgAE_phoEB_pt_M, "Medium ID", "PE");
  lmg->AddEntry(tgAE_phoEB_pt_dphoMETPhi, "#Delta#phi(#gamma, MET) > 1.2", "PE");
  lmg->AddEntry(tgAE_phoEB_pt_MET, "MET > 80 GeV", "PE");
  lmg->Draw("SAME");
  title = mg->GetTitle();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  tgAE_phoEB_pt_HLT->Fit(fitresult,"", "", 200, 500);
  yinter[0] = fitresult->GetParameter(0);
  err[0] = fitresult->GetParError(0);
  tgAE_phoEB_pt_200->Fit(fitresult,"", "", 200, 500);
  yinter[1] = fitresult->GetParameter(0);
  err[1] = fitresult->GetParError(0);
  tgAE_phoEB_pt_M->Fit(fitresult,"", "", 200, 500);
  yinter[2] = fitresult->GetParameter(0);
  err[2] = fitresult->GetParError(0);
  tgAE_phoEB_pt_dphoMETPhi->Fit(fitresult,"", "", 200, 500);
  yinter[3] = fitresult->GetParameter(0);
  err[3] = fitresult->GetParError(0);
  tgAE_phoEB_pt_MET->Fit(fitresult,"", "", 200, 500);
  yinter[4] = fitresult->GetParameter(0);
  err[4] = fitresult->GetParError(0);
  tgAE_phoEB_pt_Meff->Fit(fitresult,"", "", 200, 500);
  yinter[5] = fitresult->GetParameter(0);
  err[5] = fitresult->GetParError(0);


  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  
  for(Int_t j=0; j<6; j++){
    ftext << j << ") cut efficiency: " << yinter[j] << " error: " << err[j] << endl;
  }  
  
  ftext.close();
  
  
}
