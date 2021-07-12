#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
#define nhisto 12
#define nptbin 6


void xDrawphoMETcuteff(){
  ofstream ftext;
  TString rootname[5] = {
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_Znunug_pt130/210702_120105/output_ggtree.root"
  };

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

  TH1F *H_phoEB_pt_phoMET_vcut[nhisto];
  TH1F *H_phoEB_pt_chworst;
  TGraphAsymmErrors *tgAE_phoEB_pt_phoMET_vcut[nhisto];
  
  TGraph *tg_phoEB_pt_phoMET_vcut[nptbin];

  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  TTree *t;

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  Double_t ptoverMETbin[20] = {1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4};
  Double_t x[nhisto], yeff[nhisto];
  fopen = new TFile(rootname[0]);

  H_phoEB_pt_chworst = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_chworst_%i", 0));
  for(Int_t j=0; j<nhisto; j++){
    H_phoEB_pt_phoMET_vcut[j] = (TH1F*)fopen->Get(Form("h_phoEB_pt_phoMET_vcut/h_phoEB_pt_phoMET_vcut_%i", j));

    tgAE_phoEB_pt_phoMET_vcut[j] = new TGraphAsymmErrors();

    tgAE_phoEB_pt_phoMET_vcut[j]->Divide(H_phoEB_pt_phoMET_vcut[j], H_phoEB_pt_chworst);

    tgAE_phoEB_pt_phoMET_vcut[j]->SetMarkerStyle(8);

    tgAE_phoEB_pt_phoMET_vcut[j]->Draw("AP");

    tgAE_phoEB_pt_phoMET_vcut[j]->SetNameTitle(Form("tgAE_phoEB_pt_phoMET_vcut_%i", j), Form("phoMET vcut %i", j));

    tgAE_phoEB_pt_phoMET_vcut[j]->GetXaxis()->SetRangeUser(0, 500);
    tgAE_phoEB_pt_phoMET_vcut[j]->SetMinimum(0);
    tgAE_phoEB_pt_phoMET_vcut[j]->SetMaximum(1.05);

    //tgAE_phoEB_pt_phoMET_vcut[j]->GetPoint(0, x, y);

    //cout<< "point0 " << x << " " << y << " " << endl;

    title = tgAE_phoEB_pt_phoMET_vcut[j]->GetName();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  }

  TF1 *fitresult = new TF1("fitresult", "pol2", 1, 4);
  TF1 *fitresult1 = new TF1("fitresult1", "pol1", 1, 4);
  Double_t yinter[2][nptbin], slope[2][nptbin];
  Double_t par[3][nptbin];
  
  TLatex *lattg = new TLatex();

  for(Int_t i=0; i<nptbin; i++){
    tg_phoEB_pt_phoMET_vcut[i] = new TGraph();
    for(Int_t j=0; j<nhisto; j++){
      tgAE_phoEB_pt_phoMET_vcut[j]->GetPoint(i, x[j], yeff[j]);

      tg_phoEB_pt_phoMET_vcut[i]->SetPoint(j, ptoverMETbin[j], yeff[j]);
    }
    tg_phoEB_pt_phoMET_vcut[i]->SetMarkerStyle(8);

    tg_phoEB_pt_phoMET_vcut[i]->SetMarkerColor(cnum[0]);

    tg_phoEB_pt_phoMET_vcut[i]->Draw("AP");

    tg_phoEB_pt_phoMET_vcut[i]->SetNameTitle(Form("tg_phoEB_pt_phoMET_vcut_%i", i), Form("MET vcut ptbin %i", i));
    tg_phoEB_pt_phoMET_vcut[i]->SetMinimum(0.4);
    tg_phoEB_pt_phoMET_vcut[i]->SetMaximum(1.0);
    tg_phoEB_pt_phoMET_vcut[i]->GetYaxis()->SetTitle("Efficiency");
    tg_phoEB_pt_phoMET_vcut[i]->GetXaxis()->SetTitle("P_{T}^{#gamma}/MET");

    tg_phoEB_pt_phoMET_vcut[i]->Fit(fitresult, "", "", 1.5, 3.2);
    par[0][i] = fitresult->GetParameter(0);
    par[1][i] = fitresult->GetParameter(1);
    par[2][i] = fitresult->GetParameter(2);
    lattg->SetTextAlign(12);
    lattg->SetTextSize(0.035);
    lattg->DrawLatex(1.2, .8, Form("fit = %.3fx^{2}+%.3fx+%.3f", par[2][i], par[1][i], par[0][i]));
    //lattg->DrawLatex(1.5, .75, Form("fit = %.3fx+%.3f", par[1][i], par[0][i]));

    //tg_phoEB_pt_phoMET_vcut[i]->Fit(fitresult1, "", "", 2.2, 3.2);
    //par[0][i] = fitresult1->GetParameter(0);
    //par[1][i] = fitresult1->GetParameter(1);
    //par[2][i] = fitresult1->GetParameter(2);
    //lattg->SetTextAlign(12);
    //lattg->SetTextSize(0.035);
    //lattg->DrawLatex(1.5, .75, Form("fit = %.3fx^{2}+%.3fx+%.3f", par[2][i], par[1][i], par[0][i]));
    //lattg->DrawLatex(1.5, .72, Form("fit1 = %.3fx+%.3f", par[1][i], par[0][i]));
    
    title = tg_phoEB_pt_phoMET_vcut[i]->GetName();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));  
  }
}
