#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
#define nfile 5
#define ncolor 10

TH1D* ratioplot(THStack* st, TH1D* h1){

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
  
    
  TH1D *ratio = (TH1D*)h1->Clone("ratio");
  ratio->SetLineColor(kBlack);
  ratio->SetMinimum(-1);
  ratio->SetMaximum(6);
  ratio->Sumw2();
  ratio->Divide(tmpHist);
  
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerStyle(8);
  ratio->SetMarkerSize(0.9);
  ratio->Draw("EP");
  
  ratio->SetTitle("");
  ratio->GetYaxis()->SetTitle("Data/MC");
  ratio->GetYaxis()->SetTitleFont(42);
  ratio->GetYaxis()->SetTitleSize(0.1);
  ratio->GetYaxis()->SetTitleOffset(0.4);
  ratio->GetYaxis()->SetLabelFont(42);
  ratio->GetYaxis()->SetLabelOffset(0.01);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetYaxis()->SetTickSize(0.032);
  
  ratio->GetXaxis()->SetTitleFont(42);
  ratio->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetTitleOffset(0.95);
  ratio->GetXaxis()->SetLabelFont(42);
  ratio->GetXaxis()->SetLabelOffset(0.05);
  ratio->GetXaxis()->SetLabelSize(0.1);
  ratio->GetXaxis()->SetTickSize(0.1);
  ratio->GetXaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");

  return ratio;
}

void xDrawProfile_rhoeff(){

  TString rootname[5] = {
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_GJets_sigMC_HT_13TeV-madgraphMLM-pythia8/output_merge.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_WGToLNuG_01J_5f_PtG_130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/210614_115308/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZLLGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph/210614_123021/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph-pythia8/210614_100645/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]
  Float_t lumi16 = 36.33;
  Float_t Xsec = 2323.0;

  Float_t xsec[10] = {2323, 1.008, 0.148, 0.17};
  Float_t filter[10] = {1., 0.7772, 1., 1.};
  Float_t kfactor[10] = {1., 1., 1.067, 1.617};
  
  string hexcolor[ncolor] = {"#9297db", "#ffea00", "#4caf50", "#ff9e00", "#03a9f4", "#0066ff","#ff3360", "#F0F5E1","#DCF563", "#2F81A3" };
  TColor *color[8];
  Int_t cnum[8];
  for(Int_t j=0; j<8; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  }

  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  TTree *t;

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  
  TH2F *H2_rho_phoEB_pt_M[nfile];
  TH2F *H2_rho_phoEB_pt_chworst[nfile];
  TH2F *H2_rho_phoEB_pt_chworst_newEA[nfile];
  
  TH1D *h1_rho_phoEB_pt_M[nfile];
  TH1D *h1_rho_phoEB_pt_chworst[nfile];
  TH1D *h1_rho_phoEB_pt_chworst_newEA[nfile];

  
  Int_t entries;
  Float_t outentries = 0.;
  Float_t scale = 0.;

  TH1F *H_Events[10];
  
  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    t = (TTree*)fopen->Get("t");

    if(i<nfile-1){
      H_Events[i] = (TH1F*)fopen->Get("hEvents");
      entries = H_Events[i]->GetBinContent(1);
      outentries = xsec[i]*kfactor[i]*1000*lumi16*filter[i];
      scale = outentries/entries;
      cout << "print " << entries << " " << outentries << " " << scale<< endl;
      //if(i==4) scale*=100000;
    }
    if(i==4) scale = 1.9055;

    H2_rho_phoEB_pt_M[i] = (TH2F*)fopen->Get("h2_rho_phoEB_pt_M");
    H2_rho_phoEB_pt_chworst[i] = (TH2F*)fopen->Get("h2_rho_phoEB_pt_chworst");
    H2_rho_phoEB_pt_chworst_newEA[i] = (TH2F*)fopen->Get("h2_rho_phoEB_pt_chworst_newEA");

    h1_rho_phoEB_pt_M[i] = H2_rho_phoEB_pt_M[i]->ProjectionX("h_rho_phoEB_pt_M");
    h1_rho_phoEB_pt_chworst[i] = H2_rho_phoEB_pt_chworst[i]->ProjectionX("h_rho_phoEB_pt_chworst");
    h1_rho_phoEB_pt_chworst_newEA[i] = H2_rho_phoEB_pt_chworst_newEA[i]->ProjectionX("h_rho_phoEB_pt_chworst_newEA");
    
    h1_rho_phoEB_pt_M[i]->GetXaxis()->SetTitle("rho");
    h1_rho_phoEB_pt_chworst[i]->GetXaxis()->SetTitle("rho");
    h1_rho_phoEB_pt_chworst_newEA[i]->GetXaxis()->SetTitle("rho");
    
    h1_rho_phoEB_pt_M[i]->GetYaxis()->SetTitle("P_{T}*{#gamma}");
    h1_rho_phoEB_pt_chworst[i]->GetYaxis()->SetTitle("P_{T}*{#gamma}");
    h1_rho_phoEB_pt_chworst_newEA[i]->GetYaxis()->SetTitle("P_{T}*{#gamma}");

    h1_rho_phoEB_pt_M[i]->Sumw2();
    h1_rho_phoEB_pt_chworst[i]->Sumw2();
    h1_rho_phoEB_pt_chworst_newEA[i]->Sumw2();
  }

  //chworst eff
  TGraphAsymmErrors *tgAE_rho_phoEB_pt_chworst[nfile];
  TGraphAsymmErrors *tgAE_rho_phoEB_pt_chworst_newEA[nfile];

  TLegend *ltgAE = new TLegend(0.65,0.65, 0.75, 0.75);
  ltgAE->SetTextSize(0.03);
  ltgAE->SetFillStyle(0);
  

  TF1 *fitresult = new TF1("fitresult", "pol0", 0, 60);
  Double_t yinter[10][nfile], err[10][nfile];

  
  for(Int_t i=0; i<nfile; i++){
    tgAE_rho_phoEB_pt_chworst[i] = new TGraphAsymmErrors();
    tgAE_rho_phoEB_pt_chworst[i]->Divide(h1_rho_phoEB_pt_chworst[i], h1_rho_phoEB_pt_M[i]);
    tgAE_rho_phoEB_pt_chworst[i]->SetNameTitle("tgAE_rho_phoEB_pteff_chworst", "rho-pho cut chworst");
    tgAE_rho_phoEB_pt_chworst[i]->SetMarkerColor(cnum[1]);
    tgAE_rho_phoEB_pt_chworst[i]->Draw("AP");
    tgAE_rho_phoEB_pt_chworst[i]->GetXaxis()->SetTitle("rho");
    tgAE_rho_phoEB_pt_chworst[i]->GetXaxis()->SetLimits(0, 50);
    tgAE_rho_phoEB_pt_chworst[i]->GetYaxis()->SetTitle("Efficiency");
    tgAE_rho_phoEB_pt_chworst[i]->SetMaximum(1.05);
    tgAE_rho_phoEB_pt_chworst[i]->SetMinimum(0.0);
    ltgAE->Clear();
    ltgAE->AddEntry(tgAE_rho_phoEB_pt_chworst[i], "chworst < 1.3", "PE");
    ltgAE->Draw("SAME");
    title = tgAE_rho_phoEB_pt_chworst[i]->GetName();
    c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));

    tgAE_rho_phoEB_pt_chworst_newEA[i] = new TGraphAsymmErrors();
    tgAE_rho_phoEB_pt_chworst_newEA[i]->Divide(h1_rho_phoEB_pt_chworst_newEA[i], h1_rho_phoEB_pt_M[i]);
    tgAE_rho_phoEB_pt_chworst_newEA[i]->SetNameTitle("tgAE_rho_phoEB_pteff_chworst_newEA", "rho-pho cut chworst_newEA");
    tgAE_rho_phoEB_pt_chworst_newEA[i]->SetMarkerColor(cnum[1]);
    tgAE_rho_phoEB_pt_chworst_newEA[i]->Draw("AP");
    tgAE_rho_phoEB_pt_chworst_newEA[i]->GetXaxis()->SetTitle("rho");
    tgAE_rho_phoEB_pt_chworst_newEA[i]->GetXaxis()->SetLimits(0, 50);
    tgAE_rho_phoEB_pt_chworst_newEA[i]->GetYaxis()->SetTitle("Efficiency");
    tgAE_rho_phoEB_pt_chworst_newEA[i]->SetMaximum(1.05);
    tgAE_rho_phoEB_pt_chworst_newEA[i]->SetMinimum(0.0);
    ltgAE->Clear();
    ltgAE->AddEntry(tgAE_rho_phoEB_pt_chworst_newEA[i], "chworst_newEA < 1.3", "PE");
    ltgAE->Draw("SAME");
    title = tgAE_rho_phoEB_pt_chworst_newEA[i]->GetName();
    c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    
    
  }

  TLegend *lmg = new TLegend(0.75,0.65, 0.85, 0.75);
  lmg->SetTextSize(0.03);
  lmg->SetFillStyle(0);
  
  TMultiGraph *mg = new TMultiGraph();
  tgAE_rho_phoEB_pt_chworst_newEA[3]->SetMarkerColor(cnum[5]);
  tgAE_rho_phoEB_pt_chworst_newEA[4]->SetMarkerColor(cnum[1]);
  mg->Add(tgAE_rho_phoEB_pt_chworst_newEA[3]);
  mg->Add(tgAE_rho_phoEB_pt_chworst_newEA[4]);
  mg->GetXaxis()->SetTitle("rho");
  mg->GetYaxis()->SetTitle("Chworst Cut Efficiency");
  mg->GetXaxis()->SetLimits(0, 40);
  mg->SetMaximum(1.05);
  mg->SetMinimum(0.0);
  mg->SetTitle("mg_chworst_newEA_eff_mcdata");
  mg->Draw("AP");
  lmg->Clear();
  lmg->AddEntry(tgAE_rho_phoEB_pt_chworst_newEA[3], "MC", "PE");
  lmg->AddEntry(tgAE_rho_phoEB_pt_chworst_newEA[4], "Data", "PE");
  lmg->Draw("SAME");
  title = mg->GetTitle();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  TMultiGraph *mg1 = new TMultiGraph();
  tgAE_rho_phoEB_pt_chworst[4]->SetMarkerColor(cnum[5]);
  tgAE_rho_phoEB_pt_chworst_newEA[4]->SetMarkerColor(cnum[1]);
  mg1->Add(tgAE_rho_phoEB_pt_chworst[4]);
  mg1->Add(tgAE_rho_phoEB_pt_chworst_newEA[4]);
  mg1->GetXaxis()->SetTitle("rho");
  mg1->GetYaxis()->SetTitle("Chworst Cut Efficiiency");
  mg1->GetXaxis()->SetLimits(0, 40);
  mg1->SetMaximum(1.05);
  mg1->SetMinimum(0.0);
  mg1->SetTitle("mg1_chworst_newEA_eff_mcdata");
  mg1->Draw("AP");
  lmg->Clear();
  lmg->AddEntry(tgAE_rho_phoEB_pt_chworst[4], "oldEA", "PE");
  lmg->AddEntry(tgAE_rho_phoEB_pt_chworst_newEA[4], "newEA", "PE");
  lmg->Draw("SAME");
  title = mg1->GetTitle();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  for(Int_t i=0; i<nfile; i++){
    tgAE_rho_phoEB_pt_chworst[i]->Fit(fitresult, "", "", 5, 60);
    yinter[0][i] = fitresult->GetParameter(0);
    err[0][i] = fitresult->GetParError(0);

    tgAE_rho_phoEB_pt_chworst_newEA[i]->Fit(fitresult, "", "", 5, 60);
    yinter[1][i] = fitresult->GetParameter(0);
    err[1][i] = fitresult->GetParError(0); 
  }
}
