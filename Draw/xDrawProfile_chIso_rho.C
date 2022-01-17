#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
#define nfile 1
#define nhisto 2

void xDrawProfile_chIso_rho(){
 TString rootname[5] = {
			 "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]
  
  string hexcolor[8] = {"#03a9f4","#2F81A3","#1C2A60","#FA6D5F","#91A81E","#DCF563", "#1C6040","#EFE2C9"};
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

  TH2F *H2_rho_chIso_HLT[nfile][nhisto];
  TH2F *H2_rho_chworst_HLT[nfile][nhisto];

  TProfile *tp_rho_chIso_HLT[nfile][nhisto];
  TProfile *tp_rho_chworst_HLT[nfile][nhisto];
  Float_t EAbin[10] = {0.0, 1.0, 1.479, 2.0, 2.2, 2.3, 2.4, 3};//8 bins
  Double_t slope, err;
  Double_t par[3];
  TF1 *fitresult = new TF1("fitresult", "pol2", 3, 25);

  TLatex *latp = new TLatex();
  
  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    t = (TTree*)fopen->Get("t");

    for(Int_t j=0; j<nhisto; j++){
      H2_rho_chIso_HLT[i][j] = (TH2F*)fopen->Get(Form("SMandVBS/h2_rho_chIso_HLT_%i", j));
      H2_rho_chworst_HLT[i][j] = (TH2F*)fopen->Get(Form("SMandVBS/h2_rho_chWorstIso_HLT_%i", j));

      tp_rho_chIso_HLT[i][j] = H2_rho_chIso_HLT[i][j]->ProfileX(Form("tp_rho_chIso_HLT_%i", j), 1, 30);
      tp_rho_chworst_HLT[i][j] = H2_rho_chworst_HLT[i][j]->ProfileX(Form("tp_rho_chWorstIso_HLT_%i", j), 1, 30);

      tp_rho_chIso_HLT[i][j]->SetMarkerStyle(8);
      tp_rho_chworst_HLT[i][j]->SetMarkerStyle(8);

      tp_rho_chIso_HLT[i][j]->SetMarkerColor(1);
      tp_rho_chworst_HLT[i][j]->SetMarkerColor(1);
      
      tp_rho_chIso_HLT[i][j]->SetTitle(Form("tp_rho_chIso_HLT%i-%i", i, j));
      tp_rho_chworst_HLT[i][j]->SetTitle(Form("tp_rho_chworst_HLT%i-%i", i, j));

      //tp_rho_chIso_HLT[i][j]->GetXaxis()->SetTitle("rho");
      //tp_rho_chworst_HLT[i][j]->GetXaxis()->SetTitle("rho");
      
      //tp_rho_chIso_HLT[i][j]->GetYaxis()->SetTitle("Average charge Iso");
      //tp_rho_chworst_HLT[i][j]->GetYaxis()->SetTitle("Average worst charge Iso");

      H2_rho_chIso_HLT[i][j]->GetXaxis()->SetTitle("rho");
      H2_rho_chworst_HLT[i][j]->GetXaxis()->SetTitle("rho");
      
      H2_rho_chIso_HLT[i][j]->GetYaxis()->SetTitle("Average charge Iso");
      H2_rho_chworst_HLT[i][j]->GetYaxis()->SetTitle("Average worst charge Iso");

      H2_rho_chIso_HLT[i][j]->Draw("colZ");
      H2_rho_chIso_HLT[i][j]->SetMinimum(0);
      H2_rho_chIso_HLT[i][j]->SetMaximum(1);
      H2_rho_chIso_HLT[i][j]->GetXaxis()->SetRangeUser(0, 30);
      tp_rho_chIso_HLT[i][j]->Draw("SAME");
      //tp_rho_chIso_HLT[i][j]->SetMinimum(0);
      //tp_rho_chIso_HLT[i][j]->SetMaximum(1);
      //tp_rho_chIso_HLT[i][j]->GetXaxis()->SetRangeUser(0, 30);
      //tp_rho_chIso_HLT[i][j]->Fit(fitresult, "", "", 5, 25);
      
      //slope = fitresult->GetParameter(1);
      //err = 1000*(fitresult->GetParError(1));
      //for(Int_t i=0; i<3; i++){par[i] = fitresult->GetParameter(i);}
      //latp->SetTextAlign(12);
      //latp->SetTextSize(0.035);
      //latp->DrawLatex(2, .8, Form("fit = %.3frho^{2}+%.3frho+%.3f", par[2], par[1], par[0]));
      title = tp_rho_chIso_HLT[i][j]->GetTitle();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));

      H2_rho_chworst_HLT[i][j]->Draw("colZ");
      H2_rho_chworst_HLT[i][j]->SetMinimum(0);
      H2_rho_chworst_HLT[i][j]->SetMaximum(2);
      H2_rho_chworst_HLT[i][j]->GetXaxis()->SetRangeUser(0, 25);
      tp_rho_chworst_HLT[i][j]->Draw("SAME");
      //tp_rho_chworst_HLT[i][j]->SetMinimum(0);
      //tp_rho_chworst_HLT[i][j]->SetMaximum(2);
      //tp_rho_chworst_HLT[i][j]->GetXaxis()->SetRangeUser(0, 25);
      //tp_rho_chworst_HLT[i][j]->Fit(fitresult, "", "", 5, 25);
      
      //slope = fitresult->GetParameter(1);
      //err = 1000*(fitresult->GetParError(1));
      //for(Int_t i=0; i<3; i++){par[i] = fitresult->GetParameter(i);}
      //latp->SetTextAlign(12);
      //latp->SetTextSize(0.035);
      //latp->DrawLatex(2, 1.6, Form("fit = %.3frho^{2}+%.3frho+%.3f", par[2], par[1], par[0]));
      title = tp_rho_chworst_HLT[i][j]->GetTitle();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));
      
    }
  }
}
