#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
//#define nfile 4
#define nfile 5
void xDrawCutflow(){
  ofstream ftext;
  /*
  TString rootname[5] = {"/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_GJet_Pt-15To6000_TuneCUETP8M1-Flat_13TeV_pythia8_20M_0000/210505_040352/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_WGToLNuG_01J_5f_PtG_130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0000/210505_082513/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_ZLLGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph_0000/210505_064501/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_ZGTo2NuG_PtG-130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0000/210505_064756/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]
  */
TString rootname[5] = {
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_GJets_sigMC_HT_13TeV-madgraphMLM-pythia8/210527_150753/output_merge.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_WGToLNuG_01J_5f_PtG_130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0000/210527_065337/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZLLGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph_0000/210527_072853/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph-pythia8_0000/210527_064105/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]

 string hexcolor[8] = {"#7FB5F5","#91A81E","#1C2A60","#FA6D5F","#1C6040","#DCF563", "#2F81A3","#EFE2C9"};
  TColor *color[8];
  Int_t cnum[8];
  for(Int_t j=0; j<8; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  }

  TH1F *H_phoEB_pt[nfile];
  TH1F *H_phoEB_pt_HLT[nfile];
  TH1F *H_phoEB_pt_200[nfile];
  TH1F *H_phoEB_pt_M[nfile];
  TH1F *H_phoEB_pt_r9[nfile];
  TH1F *H_phoEB_pt_dphoMETPhi[nfile];
  TH1F *H_phoEB_pt_MET[nfile];

  TGraphAsymmErrors *tgAE_phoEB_pt_HLT[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_200[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_M[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_r9[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_dphoMETPhi[nfile];
  TGraphAsymmErrors *tgAE_phoEB_pt_MET[nfile];

  TGraphAsymmErrors *tgAE_phoEB_pt_Meff[nfile];

  TLegend *ltgAE = new TLegend(0.65,0.65, 0.75, 0.75);
  ltgAE->SetBorderSize(0);
  ltgAE->SetFillStyle(0);
  TArrow *ArtgAE = new TArrow(205, 0.001, 205, 0.90, 0.03, "|>");
  ArtgAE->SetAngle(40);
  ArtgAE->SetLineWidth(1);
  
  
  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  TTree *t;
  
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    t = (TTree*)fopen->Get("t");

    H_phoEB_pt[i] = (TH1F*)fopen->Get("h_phoEB_pt");
    H_phoEB_pt_HLT[i] = (TH1F*)fopen->Get("h_phoEB_pt_HLT");
    H_phoEB_pt_200[i] = (TH1F*)fopen->Get("h_phoEB_pt_200");
    H_phoEB_pt_M[i] = (TH1F*)fopen->Get("h_phoEB_pt_M");
    //H_phoEB_pt_r9[i] = (TH1F*)fopen->Get("h_phoEB_pt_r9");
    H_phoEB_pt_dphoMETPhi[i] = (TH1F*)fopen->Get("h_phoEB_pt_dphoMETPhi");
    H_phoEB_pt_MET[i] = (TH1F*)fopen->Get("h_phoEB_pt_MET");

    tgAE_phoEB_pt_HLT[i] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_200[i] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_M[i] = new TGraphAsymmErrors();
    //tgAE_phoEB_pt_r9[i] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_dphoMETPhi[i] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_MET[i] = new TGraphAsymmErrors();
    tgAE_phoEB_pt_Meff[i] = new TGraphAsymmErrors();
    
    tgAE_phoEB_pt_HLT[i]->Divide(H_phoEB_pt_HLT[i], H_phoEB_pt_MET[i]);
    tgAE_phoEB_pt_200[i]->Divide(H_phoEB_pt_200[i], H_phoEB_pt[i]);
    tgAE_phoEB_pt_M[i]->Divide(H_phoEB_pt_M[i], H_phoEB_pt[i]);
    //tgAE_phoEB_pt_r9[i]->Divide(H_phoEB_pt_r9[i], H_phoEB_pt[i]);
    tgAE_phoEB_pt_dphoMETPhi[i]->Divide(H_phoEB_pt_dphoMETPhi[i], H_phoEB_pt[i]);
    tgAE_phoEB_pt_MET[i]->Divide(H_phoEB_pt_MET[i], H_phoEB_pt[i]);
    tgAE_phoEB_pt_Meff[i]->Divide(H_phoEB_pt_M[i], H_phoEB_pt_200[i]);

    tgAE_phoEB_pt_HLT[i]->SetNameTitle("tgAE_phoEB_pteff_HLT", "pho cut HLT");
    tgAE_phoEB_pt_200[i]->SetNameTitle("tgAE_phoEB_pteff_200", "pho cut pt200");
    tgAE_phoEB_pt_M[i]->SetNameTitle("tgAE_phoEB_pteff_M", "pho cut M ID");
    //tgAE_phoEB_pt_r9[i]->SetNameTitle("tgAE_phoEB_pteff_r9", "pho cut r9");
    tgAE_phoEB_pt_dphoMETPhi[i]->SetNameTitle("tgAE_phoEB_pteff_dphoMETPhi", "pho cut dphoMETPhi");
    tgAE_phoEB_pt_MET[i]->SetNameTitle("tgAE_phoEB_pteff_MET", "pho cut MET");
    tgAE_phoEB_pt_Meff[i]->SetNameTitle("tgAE_phoEB_pt_Meff", "pho M ID bit eff");

    tgAE_phoEB_pt_HLT[i]->SetMarkerStyle(8);
    tgAE_phoEB_pt_200[i]->SetMarkerStyle(8);
    tgAE_phoEB_pt_M[i]->SetMarkerStyle(8);
    //tgAE_phoEB_pt_r9[i]->SetMarkerStyle(8);
    tgAE_phoEB_pt_dphoMETPhi[i]->SetMarkerStyle(8);
    tgAE_phoEB_pt_MET[i]->SetMarkerStyle(8);
    tgAE_phoEB_pt_Meff[i]->SetMarkerStyle(8);
    
    tgAE_phoEB_pt_HLT[i]->SetMarkerColor(cnum[4]);
    tgAE_phoEB_pt_200[i]->SetMarkerColor(cnum[0]);
    tgAE_phoEB_pt_M[i]->SetMarkerColor(cnum[1]);
    //tgAE_phoEB_pt_r9[i]->SetMarkerColor(cnum[i]);
    tgAE_phoEB_pt_dphoMETPhi[i]->SetMarkerColor(cnum[2]);
    tgAE_phoEB_pt_MET[i]->SetMarkerColor(cnum[3]);
    tgAE_phoEB_pt_Meff[i]->SetMarkerColor(cnum[1]);
    
    tgAE_phoEB_pt_HLT[i]->Draw("AP");
    tgAE_phoEB_pt_HLT[i]->GetXaxis()->SetTitle("P_{T}^{#gamma}");
    tgAE_phoEB_pt_HLT[i]->GetYaxis()->SetTitle("Efficiency");
    tgAE_phoEB_pt_HLT[i]->GetXaxis()->SetLimits(0, 500);
    tgAE_phoEB_pt_HLT[i]->SetMaximum(1.05);
    tgAE_phoEB_pt_HLT[i]->SetMinimum(0.0);
    ltgAE->Clear();
    ltgAE->AddEntry(tgAE_phoEB_pt_HLT[i], "HLT_175", "PE");
    ltgAE->Draw("SAME");
    tgAE_phoEB_pt_HLT[i]->GetPoint(turnpoint, x, y);
    //ArtgAE = new TArrow(x, 0.001, x, y-0.01, 0.03, "|>");
    //ArtgAE->SetAngle(40);
    //ArtgAE->SetLineWidth(1);
    //ArtgAE->Draw();
    title = tgAE_phoEB_pt_HLT[i]->GetName();
    c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    
    tgAE_phoEB_pt_200[i]->Draw("AP");
    
    title = tgAE_phoEB_pt_200[i]->GetName();
    //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    
    tgAE_phoEB_pt_M[i]->Draw("AP");
    title = tgAE_phoEB_pt_M[i]->GetName();
    //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    
    //tgAE_phoEB_pt_r9[i]->Draw("AP");
    //title = tgAE_phoEB_pt_r9[i]->GetName();
    //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    
    tgAE_phoEB_pt_dphoMETPhi[i]->Draw("AP");
    title = tgAE_phoEB_pt_dphoMETPhi[i]->GetName(); 
    //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    
    tgAE_phoEB_pt_MET[i]->Draw("AP");
    title = tgAE_phoEB_pt_MET[i]->GetName();
    //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));

    tgAE_phoEB_pt_Meff[i]->Draw("AP");
    tgAE_phoEB_pt_Meff[i]->GetXaxis()->SetTitle("P_{T}^{#gamma}");
    tgAE_phoEB_pt_Meff[i]->GetYaxis()->SetTitle("Efficiency");
    tgAE_phoEB_pt_Meff[i]->GetXaxis()->SetLimits(0, 500);
    tgAE_phoEB_pt_Meff[i]->SetMaximum(1.05);
    tgAE_phoEB_pt_Meff[i]->SetMinimum(0.0);
    ltgAE->Clear();
    ltgAE->AddEntry(tgAE_phoEB_pt_Meff[i], "pho M ID", "PE");
    ltgAE->Draw("SAME");
    title = tgAE_phoEB_pt_Meff[i]->GetName();
    c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
  }

  
  TMultiGraph *mg[nfile];
  TLegend *lmg = new TLegend(0.15,0.70, 0.25, 0.88);
  lmg->SetBorderSize(0);
  lmg->SetFillStyle(0);

  TF1 *fitresult = new TF1("fitresult", "pol0", 200, 500);
  Double_t yinter[10][3], err[10][3];
  

  for(Int_t i=0; i<nfile; i++){
    mg[i] = new TMultiGraph();
    //mg[i]->Add(tgAE_phoEB_pt_HLT[i]);
    tgAE_phoEB_pt_200[i]->SetMarkerColor(cnum[0]);
    mg[i]->Add(tgAE_phoEB_pt_200[i]);
    tgAE_phoEB_pt_M[i]->SetMarkerColor(cnum[1]);
    mg[i]->Add(tgAE_phoEB_pt_M[i]);
    //tgAE_phoEB_pt_r9[i]->SetMarkerColor(cnum[1]);
    //mg[i]->Add(tgAE_phoEB_pt_r9[i]);
    tgAE_phoEB_pt_dphoMETPhi[i]->SetMarkerColor(cnum[2]);
    mg[i]->Add(tgAE_phoEB_pt_dphoMETPhi[i]);
    tgAE_phoEB_pt_MET[i]->SetMarkerColor(cnum[3]);
    mg[i]->Add(tgAE_phoEB_pt_MET[i]);

    mg[i]->GetXaxis()->SetTitle("phoEt#[]{GeV}");
    mg[i]->GetYaxis()->SetTitle("Efficiency");
    mg[i]->GetXaxis()->SetLimits(-1, 500);
    mg[i]->SetMaximum(1.05);
    mg[i]->SetTitle(Form("cutflow%i", i));
    mg[i]->Draw("AP");
    lmg->Clear();
    //lmg->AddEntry(tgAE_phoEB_pt_HLT[i], "HLT_175", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_200[i], "pt > 200", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_M[i], "Medium ID", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_MET[i], "MET > 80 GeV", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_dphoMETPhi[i], "#Delta#phi(#gamma, MET) > 1.2", "PE");
    lmg->Draw("SAME");
    title = mg[i]->GetTitle();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    tgAE_phoEB_pt_HLT[i]->Fit(fitresult,"", "", 200, 500);
    yinter[0][i] = fitresult->GetParameter(0);
    err[0][i] = fitresult->GetParError(0);
    tgAE_phoEB_pt_200[i]->Fit(fitresult,"", "", 200, 500);
    yinter[1][i] = fitresult->GetParameter(0);
    err[1][i] = fitresult->GetParError(0);
    tgAE_phoEB_pt_M[i]->Fit(fitresult,"", "", 200, 500);
    yinter[2][i] = fitresult->GetParameter(0);
    err[2][i] = fitresult->GetParError(0);
    tgAE_phoEB_pt_dphoMETPhi[i]->Fit(fitresult,"", "", 200, 500);
    yinter[3][i] = fitresult->GetParameter(0);
    err[3][i] = fitresult->GetParError(0);
    tgAE_phoEB_pt_MET[i]->Fit(fitresult,"", "", 200, 500);
    yinter[4][i] = fitresult->GetParameter(0);
    err[4][i] = fitresult->GetParError(0);
    tgAE_phoEB_pt_Meff[i]->Fit(fitresult,"", "", 200, 500);
    yinter[5][i] = fitresult->GetParameter(0);
    err[5][i] = fitresult->GetParError(0);
  }

  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  for(Int_t i=0; i<nfile; i++){
    ftext << "file " << i << endl;
    for(Int_t j=0; j<6; j++){
      ftext << j << ") cut efficiency: " << yinter[j][i] << " error: " << err[j][i] << endl;
    }  
  }
  
  ftext.close();
  
  
}

 
