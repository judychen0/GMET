#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
#define nhisto 6
#define nptbin 6

void xDrawMETcuteff(){
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

  TH1F *H_phoEB_pt_MET_vcut[nhisto];
  TH1F *H_phoEB_pt_chworst;
  TGraphAsymmErrors *tgAE_phoEB_pt_MET_vcut[nhisto];
  
  TGraph *tg_phoEB_pt_MET_vcut[nptbin];

  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  TTree *t;

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  Double_t METbin[6] = {90, 100, 110, 120, 130, 140};
  Double_t x[nhisto], yeff[nhisto];

  fopen = new TFile(rootname[0]);

  H_phoEB_pt_chworst = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_chworst_%i", 0));
  for(Int_t j=0; j<nhisto; j++){
    H_phoEB_pt_MET_vcut[j] = (TH1F*)fopen->Get(Form("h_phoEB_pt_MET_vcut/h_phoEB_pt_MET_vcut_%i", j));

    tgAE_phoEB_pt_MET_vcut[j] = new TGraphAsymmErrors();

    tgAE_phoEB_pt_MET_vcut[j]->Divide(H_phoEB_pt_MET_vcut[j], H_phoEB_pt_chworst);

    tgAE_phoEB_pt_MET_vcut[j]->SetMarkerStyle(8);

    tgAE_phoEB_pt_MET_vcut[j]->Draw("AP");

    tgAE_phoEB_pt_MET_vcut[j]->SetNameTitle(Form("tgAE_phoEB_pt_MET_vcut_%i", j), Form("MET vcut %i", j));

    tgAE_phoEB_pt_MET_vcut[j]->GetXaxis()->SetRangeUser(0, 500);
    tgAE_phoEB_pt_MET_vcut[j]->SetMinimum(0);
    tgAE_phoEB_pt_MET_vcut[j]->SetMaximum(1.05);

    //tgAE_phoEB_pt_MET_vcut[j]->GetPoint(0, x, y);

    //cout<< "point0 " << x << " " << y << " " << endl;

    title = tgAE_phoEB_pt_MET_vcut[j]->GetName();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  }

  TF1 *fitresult = new TF1("fitresult", "pol1", 90, 140);
  Double_t yinter[nptbin], slope[nptbin];

  TLatex *lattg = new TLatex();

  for(Int_t i=0; i<nptbin; i++){
    tg_phoEB_pt_MET_vcut[i] = new TGraph();
    for(Int_t j=0; j<nhisto; j++){
      tgAE_phoEB_pt_MET_vcut[j]->GetPoint(i, x[j], yeff[j]);

      tg_phoEB_pt_MET_vcut[i]->SetPoint(j, METbin[j], yeff[j]);
    }
    tg_phoEB_pt_MET_vcut[i]->SetMarkerStyle(8);

    tg_phoEB_pt_MET_vcut[i]->SetMarkerColor(cnum[0]);

    tg_phoEB_pt_MET_vcut[i]->Draw("AP");

    tg_phoEB_pt_MET_vcut[i]->SetNameTitle(Form("tg_phoEB_pt_MET_vcut_%i", i), Form("MET vcut ptbin %i", i));

    tg_phoEB_pt_MET_vcut[i]->Fit(fitresult, "", "", 90, 130);
    yinter[i] = fitresult->GetParameter(0);
    slope[i] = fitresult->GetParameter(1);

    lattg->SetTextAlign(12);
    lattg->SetTextSize(0.035);
    lattg->DrawLatex(100, .82, Form("fit = %.4fMET+%.3f", slope[i], yinter[i]));
    title = tg_phoEB_pt_MET_vcut[i]->GetName();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

   
  }
}
