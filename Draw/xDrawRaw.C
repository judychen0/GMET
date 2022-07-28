#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
#define nfile 1

void xDrawRaw(){
  ofstream ftext;

  TString rootname[5] = {
			 "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]

  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  TTree *t;

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  Float_t integral[15][nfile];
  
  TH1F *h_phoEB_pt_210[nfile];
  TH1F *h_phoEB_pt_M[nfile];
  TH1F *h_phoEB_pt_chworst[nfile];
  TH1F *h_phoEB_pt_SeedTime[nfile];
  TH1F *h_phoEB_pt_leptonveto[nfile];
  TH1F *h_phoEB_pt_MET[nfile];
  TH1F *h_phoEB_pt_dphoMETPhi[nfile];
  TH1F *h_phoEB_pt_djetMETPhi[nfile];
  TH1F *h_phoEB_pt_jetveto[nfile];
  TH1F *h_phoEB_pt_HLT[nfile];

  for(Int_t j=0; j<nfile; j++){
    fopen = new TFile(rootname[j]);
    t = (TTree*)fopen->Get("t");
    
    h_phoEB_pt_210[j] = new TH1F(Form("h_phoEB_pt_210_%i", j), "matched phoEB pt pt200 cut", 20, 200, 1200);
    h_phoEB_pt_M[j] = new TH1F(Form("h_phoEB_pt_M_%i", j), "matched phoEB pt M IDcut", 20, 200, 1200);
    h_phoEB_pt_chworst[j] = new TH1F(Form("h_phoEB_pt_chworst_%i", j), "matched phoEB pt chworst cut", 20, 200, 1200);
    h_phoEB_pt_SeedTime[j] = new TH1F(Form("h_phoEB_pt_SeedTime_%i", j), "matched phoEB pt SeedTime cut", 20, 200, 1200);
    h_phoEB_pt_leptonveto[j] = new TH1F(Form("h_phoEB_pt_leptonveto_%i", j), "leptonveto cut", 20, 200, 1200);
    h_phoEB_pt_MET[j] = new TH1F(Form("h_phoEB_pt_MET_%i", j), "matched phoEB pt MET cut", 20, 200, 1200);
    h_phoEB_pt_dphoMETPhi[j] = new TH1F(Form("h_phoEB_pt_dphoMETPhi_%i", j), "matched phoEB pt dphoMETPhi cut", 20, 200, 1200);
    h_phoEB_pt_djetMETPhi[j] = new TH1F(Form("h_phoEB_pt_djetMETPhi_%i", j), "matched phoEB pt djetMETPhi cut", 20, 200, 1200);
    h_phoEB_pt_jetveto[j] = new TH1F(Form("h_phoEB_pt_jetveto_%i", j), Form("h_phoEB_pt_jetveto_%i", j), 20, 200, 1200);  
    h_phoEB_pt_HLT[j] = new TH1F(Form("h_phoEB_pt_HLT_%i", j), "matched phoEB pt HLT trgs", 20, 200, 1200);
    
    t->Draw(Form("phoEt_210>>h_phoEB_pt_210_%i"			, j));
    t->Draw(Form("phoEt_M>>h_phoEB_pt_M_%i"			, j));
    t->Draw(Form("phoEt_chworst>>h_phoEB_pt_chworst_%i"		, j));
    t->Draw(Form("phoEt_SeedTime>>h_phoEB_pt_SeedTime_%i"		, j));
    t->Draw(Form("phoEt_leptonveto>>h_phoEB_pt_leptonveto_%i", j));
    t->Draw(Form("phoEt_MET>>h_phoEB_pt_MET_%i"			, j));
    t->Draw(Form("phoEt_dphoMETPhi>>h_phoEB_pt_dphoMETPhi_%i"	, j));
    t->Draw(Form("phoEt_djetMETPhi>>h_phoEB_pt_djetMETPhi_%i"	, j));
    t->Draw(Form("phoEt_jetveto>>h_phoEB_pt_jetveto_%i", j));
    t->Draw(Form("phoEt_HLT>>h_phoEB_pt_HLT_%i"			, j));

    integral[0][j] = h_phoEB_pt_210[j]->Integral(1, -1);
    integral[1][j] = h_phoEB_pt_M[j]->Integral(1, -1);
    integral[2][j] = h_phoEB_pt_chworst[j]->Integral(1, -1);
    integral[3][j] = h_phoEB_pt_SeedTime[j]->Integral(1, -1);
    integral[4][j] = h_phoEB_pt_leptonveto[j]->Integral(1, -1);
    integral[5][j] = h_phoEB_pt_MET[j]->Integral(1, -1);
    integral[6][j] = h_phoEB_pt_dphoMETPhi[j]->Integral(1, -1);
    integral[7][j] = h_phoEB_pt_djetMETPhi[j]->Integral(1, -1);
    integral[8][j] = h_phoEB_pt_jetveto[j]->Integral(1, -1);
    integral[9][j] = h_phoEB_pt_HLT[j]->Integral(1, -1);
    
  }

  ftext.open("rawevents.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  for(Int_t i=0; i<nfile; i++){
      ftext << "file " << i << endl;
      for(Int_t j=0; j<10; j++){
	ftext << j << ") raw event yield: " << integral[j][i] << endl;
      }
    }
    
 

  
  
}
