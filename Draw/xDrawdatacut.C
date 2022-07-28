#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "../Drawlumi/CMS_lumi.C"

using namespace std;
//#define nfile 8
#define nhisto 2
#define njet 2
#define ncolor 10

void xDrawdatacut(Int_t year){
  ofstream ftext;
  TString rootname[10];
  TFile *fopen;
  TTree *t;

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/data/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/data/output_ggtree.root";
  }

  Float_t integral[15][nhisto];
  
  TH1F *H_phoEB_pt_210[nhisto];
  TH1F *H_phoEB_pt_M[nhisto];
  TH1F *H_phoEB_pt_leptonveto[nhisto];
  TH1F *H_phoEB_pt_MET[nhisto];
  TH1F *H_phoEB_pt_dphoMETPhi[nhisto]; // jetjetphodPhi
  TH1F *H_phoEB_pt_jetveto[nhisto];
  TH1F *H_phoEB_pt_jetjetdEta[nhisto];
  TH1F *H_phoEB_pt_jetjetdPhi[nhisto];
  TH1F *H_phoEB_pt_phojetdR[nhisto];
  TH1F *H_phoEB_pt_ptoverMET[nhisto]; // ptoverMET
  TH1F *H_phoEB_pt_mindJMETPhi[nhisto];
  TH1F *H_phoEB_pt_dijetMass[nhisto];
  
  for(Int_t jj=0; jj<nhisto; jj++){
    fopen = new TFile(rootname[0]);

    H_phoEB_pt_210[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_210_%i", jj));
    H_phoEB_pt_M[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_M_%i", jj));
    H_phoEB_pt_leptonveto[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_leptonveto_%i", jj));
    H_phoEB_pt_MET[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_MET_%i", jj));
    H_phoEB_pt_dphoMETPhi[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_dphoMETPhi_%i", jj));
    H_phoEB_pt_jetjetdEta[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetjetdEta_%i", jj));
    H_phoEB_pt_jetjetdPhi[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetjetdPhi_%i", jj));
    H_phoEB_pt_phojetdR[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_phojetdR_%i", jj));
    H_phoEB_pt_ptoverMET[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_ptoverMET_%i", jj));
    H_phoEB_pt_mindJMETPhi[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_mindJMETPhi_%i", jj));
    H_phoEB_pt_dijetMass[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_dijetMass_%i", jj));
    H_phoEB_pt_jetveto[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetveto_%i", jj));
    
    integral[0][jj] = H_phoEB_pt_210[jj]->Integral(1, -1);
    integral[1][jj] = H_phoEB_pt_M[jj]->Integral(1, -1);
    integral[2][jj] = H_phoEB_pt_leptonveto[jj]->Integral(1, -1);
    integral[3][jj] = H_phoEB_pt_MET[jj]->Integral(1, -1);
    integral[4][jj] = H_phoEB_pt_dphoMETPhi[jj]->Integral(1, -1);
    integral[5][jj] = H_phoEB_pt_jetveto[jj]->Integral(1, -1);
    integral[6][jj] = H_phoEB_pt_jetjetdEta[jj]->Integral(1, -1);
    integral[7][jj] = H_phoEB_pt_jetjetdPhi[jj]->Integral(1, -1);
    integral[8][jj] = H_phoEB_pt_phojetdR[jj]->Integral(1, -1);
    integral[9][jj] = H_phoEB_pt_ptoverMET[jj]->Integral(1, -1);
    integral[10][jj] = H_phoEB_pt_mindJMETPhi[jj]->Integral(1, -1);
    integral[11][jj] = H_phoEB_pt_dijetMass[jj]->Integral(1, -1);
    

  }

  ftext.open("dataEvents.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  for(Int_t jj=0; jj<nhisto; jj++){
    for(Int_t j=0; j<12; j++){
      ftext << j << ") event yield: " << integral[j][jj] << endl;
    }
  }
   
  ftext.close();
}
