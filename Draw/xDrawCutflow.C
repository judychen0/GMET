#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "../Drawlumi/CMS_lumi.C"
using namespace std;
//#define nfile 4
#define nfile 8
#define nhisto 2
#define ncolor 10
void xDrawCutflow(Int_t year){
  ofstream ftext;
  TString rootname[20];
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Wg_aMCatNLO/output_ggtree.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Zg_aMCatNLO/output_ggtree.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_TT_aMCatNLO/output_ggtree.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/mc/job_fall17_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/mc/job_fall17_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/mc/job_fall17_TT_aMCatNLO/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    //rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/data/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/VJet/output_merge_VJet.root";
    rootname[2] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/VG/output_merge_VG.root";
    //rootname[2] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/ZJet/output_merge_ZJet.root";
    //rootname[3] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/DYJet/output_merge_DYJet.root";
    rootname[3] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/monoboson/output_merge_monoBoson.root";
    rootname[4] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/diboson/output_merge_diBoson.root";
    rootname[5] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/triboson/output_merge_triBoson.root";
    rootname[6] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/TT/output_merge_TT.root";
    //rootname[6] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/job_UL18_Wg_pt130/output_ggtree.root";
    //rootname[7] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/job_UL18_Zg_pt130/output_ggtree.root";
    //rootname[8] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/job_UL18_Znunug_pt130/output_ggtree.root";
    //rootname[6] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/VG/job_autumn18_Wg_aMCatNLO/output_ggtree.root";
    //rootname[7] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/VG/job_autumn18_Zg_aMCatNLO/output_ggtree.root";
    //rootname[8] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/VG/job_autumn18_Znunug_pt130/output_ggtree.root";
    rootname[7] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/job_UL18_Znunug_VBS/output_ggtree.root";
  }

  string hexcolor[ncolor] = {"#b23b8c", "#ffea00", "#03a9f4", "#ff9e00", "#4caf50","#DCF563", "#2F81A3", "#ff3360", "#0066ff", "#862D55"};
  
//string hexcolor[ncolor] = {"#7FB5F5", "#EA80FC", "#FF9100", "#FFF176", "#0066FF", "#69F0AE","#FF1744",  "#0066FF"};
  TColor *color[ncolor];
  Int_t cnum[ncolor];
  for(Int_t j=0; j<ncolor; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  }
  
  TH1F *H_phoEB_pt_130[nfile][nhisto];
  TH1F *H_phoEB_pt_M[nfile][nhisto];
  TH1F *H_phoEB_pt_leptonveto[nfile][nhisto];
  TH1F *H_phoEB_pt_MET[nfile][nhisto];
  TH1F *H_phoEB_pt_dphoMETPhi[nfile][nhisto]; // jetjetphodPhi
  TH1F *H_phoEB_pt_jetveto[nfile][nhisto];
  TH1F *H_phoEB_pt_jetjetdEta[nfile][nhisto];
  TH1F *H_phoEB_pt_jetjetdPhi[nfile][nhisto];
  TH1F *H_phoEB_pt_phojetdR[nfile][nhisto];
  TH1F *H_phoEB_pt_ptoverMET[nfile][nhisto]; // ptoverMET
  TH1F *H_phoEB_pt_mindJMETPhi[nfile][nhisto];
  TH1F *H_phoEB_pt_dijetMass[nfile][nhisto];

  TH1F *H_MET_130[nfile][nhisto];
  TH1F *H_MET_M[nfile][nhisto];
  TH1F *H_MET_leptonveto[nfile][nhisto];
  TH1F *H_MET_MET[nfile][nhisto];
  TH1F *H_MET_dphoMETPhi[nfile][nhisto]; // jetjetphodPhi
  TH1F *H_MET_jetveto[nfile][nhisto];
  TH1F *H_MET_jetjetdEta[nfile][nhisto];
  TH1F *H_MET_jetjetdPhi[nfile][nhisto];
  TH1F *H_MET_phojetdR[nfile][nhisto];
  TH1F *H_MET_ptoverMET[nfile][nhisto]; // ptoverMET
  TH1F *H_MET_mindJMETPhi[nfile][nhisto];
  TH1F *H_MET_dijetMass[nfile][nhisto];

  
  TGraphAsymmErrors *tgAE_phoEB_pt_130[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_M[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_leptonveto[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_MET[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_dphoMETPhi[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_jetveto[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_jetjetdEta[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_jetjetdPhi[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_phojetdR[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_ptoverMET[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_mindJMETPhi[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_dijetMass[nfile][nhisto];

  TGraphAsymmErrors *tgAE_MET_130[nfile][nhisto];
  TGraphAsymmErrors *tgAE_MET_M[nfile][nhisto];
  TGraphAsymmErrors *tgAE_MET_leptonveto[nfile][nhisto];
  TGraphAsymmErrors *tgAE_MET_MET[nfile][nhisto];
  TGraphAsymmErrors *tgAE_MET_dphoMETPhi[nfile][nhisto];
  TGraphAsymmErrors *tgAE_MET_jetveto[nfile][nhisto];
  TGraphAsymmErrors *tgAE_MET_jetjetdEta[nfile][nhisto];
  TGraphAsymmErrors *tgAE_MET_jetjetdPhi[nfile][nhisto];
  TGraphAsymmErrors *tgAE_MET_phojetdR[nfile][nhisto];
  TGraphAsymmErrors *tgAE_MET_ptoverMET[nfile][nhisto];
  TGraphAsymmErrors *tgAE_MET_mindJMETPhi[nfile][nhisto];
  TGraphAsymmErrors *tgAE_MET_dijetMass[nfile][nhisto];


  TLegend *ltgAE = new TLegend(0.65,0.65, 0.75, 0.75);
  ltgAE->SetBorderSize(0);
  ltgAE->SetFillStyle(0);
  
  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  
  const char *title;
  const char *saveto = ".";

  Float_t integral[20][nfile][nhisto];

  for(Int_t ii=0; ii<nfile; ii++){
    fopen = new TFile(rootname[ii]);
    for(Int_t jj=0; jj<nhisto; jj++){

      H_phoEB_pt_130[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_130_%i", jj));
      H_phoEB_pt_M[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_M_%i", jj));
      H_phoEB_pt_leptonveto[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_leptonveto_%i", jj));
      H_phoEB_pt_MET[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_MET_%i", jj));
      H_phoEB_pt_dphoMETPhi[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_dphoMETPhi_%i", jj));
      H_phoEB_pt_jetveto[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetveto_%i", jj));
      H_phoEB_pt_jetjetdEta[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetjetdEta_%i", jj));
      H_phoEB_pt_jetjetdPhi[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetjetdPhi_%i", jj));
      H_phoEB_pt_ptoverMET[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_ptoverMET_%i", jj));
      H_phoEB_pt_mindJMETPhi[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_mindJMETPhi_%i", jj));
      H_phoEB_pt_dijetMass[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_dijetMass_%i", jj));      

      H_MET_130[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_130_%i", jj));
      H_MET_M[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_M_%i", jj));
      H_MET_leptonveto[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_leptonveto_%i", jj));
      H_MET_MET[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_MET_%i", jj));
      H_MET_dphoMETPhi[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_dphoMETPhi_%i", jj));
      H_MET_jetveto[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_jetveto_%i", jj));
      H_MET_jetjetdEta[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_jetjetdEta_%i", jj));
      H_MET_jetjetdPhi[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_jetjetdPhi_%i", jj));
      H_MET_ptoverMET[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_ptoverMET_%i", jj));
      H_MET_mindJMETPhi[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_mindJMETPhi_%i", jj));
      H_MET_dijetMass[ii][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_dijetMass_%i", jj));      

      tgAE_phoEB_pt_130[ii][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_M[ii][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_leptonveto[ii][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_MET[ii][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_dphoMETPhi[ii][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_jetveto[ii][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_jetjetdEta[ii][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_jetjetdPhi[ii][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_ptoverMET[ii][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_mindJMETPhi[ii][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_dijetMass[ii][jj] = new TGraphAsymmErrors();      

      tgAE_MET_130[ii][jj] = new TGraphAsymmErrors();
      tgAE_MET_M[ii][jj] = new TGraphAsymmErrors();
      tgAE_MET_leptonveto[ii][jj] = new TGraphAsymmErrors();
      tgAE_MET_MET[ii][jj] = new TGraphAsymmErrors();
      tgAE_MET_dphoMETPhi[ii][jj] = new TGraphAsymmErrors();
      tgAE_MET_jetveto[ii][jj] = new TGraphAsymmErrors();
      tgAE_MET_jetjetdEta[ii][jj] = new TGraphAsymmErrors();
      tgAE_MET_jetjetdPhi[ii][jj] = new TGraphAsymmErrors();
      tgAE_MET_ptoverMET[ii][jj] = new TGraphAsymmErrors();
      tgAE_MET_mindJMETPhi[ii][jj] = new TGraphAsymmErrors();
      tgAE_MET_dijetMass[ii][jj] = new TGraphAsymmErrors();      

      H_phoEB_pt_130[ii][jj]->ClearUnderflowAndOverflow();
      H_phoEB_pt_M[ii][jj]->ClearUnderflowAndOverflow();
      H_phoEB_pt_leptonveto[ii][jj]->ClearUnderflowAndOverflow();
      H_phoEB_pt_MET[ii][jj]->ClearUnderflowAndOverflow();
      H_phoEB_pt_dphoMETPhi[ii][jj]->ClearUnderflowAndOverflow();
      H_phoEB_pt_jetveto[ii][jj]->ClearUnderflowAndOverflow();
      H_phoEB_pt_jetjetdEta[ii][jj]->ClearUnderflowAndOverflow();
      H_phoEB_pt_jetjetdPhi[ii][jj]->ClearUnderflowAndOverflow();
      H_phoEB_pt_ptoverMET[ii][jj]->ClearUnderflowAndOverflow();
      H_phoEB_pt_mindJMETPhi[ii][jj]->ClearUnderflowAndOverflow();
      H_phoEB_pt_dijetMass[ii][jj]->ClearUnderflowAndOverflow();      

      H_MET_130[ii][jj]->ClearUnderflowAndOverflow();
      H_MET_M[ii][jj]->ClearUnderflowAndOverflow();
      H_MET_leptonveto[ii][jj]->ClearUnderflowAndOverflow();
      H_MET_MET[ii][jj]->ClearUnderflowAndOverflow();
      H_MET_dphoMETPhi[ii][jj]->ClearUnderflowAndOverflow();
      H_MET_jetveto[ii][jj]->ClearUnderflowAndOverflow();
      H_MET_jetjetdEta[ii][jj]->ClearUnderflowAndOverflow();
      H_MET_jetjetdPhi[ii][jj]->ClearUnderflowAndOverflow();
      H_MET_ptoverMET[ii][jj]->ClearUnderflowAndOverflow();
      H_MET_mindJMETPhi[ii][jj]->ClearUnderflowAndOverflow();
      H_MET_dijetMass[ii][jj]->ClearUnderflowAndOverflow();      

      integral[0][ii][jj] = H_phoEB_pt_130[ii][jj]->Integral(1, -1);
      integral[1][ii][jj] = H_phoEB_pt_M[ii][jj]->Integral(1, -1);
      integral[2][ii][jj] = H_phoEB_pt_leptonveto[ii][jj]->Integral(1, -1);
      integral[3][ii][jj] = H_phoEB_pt_MET[ii][jj]->Integral(1, -1);
      integral[4][ii][jj] = H_phoEB_pt_dphoMETPhi[ii][jj]->Integral(1, -1);
      integral[5][ii][jj] = H_phoEB_pt_jetveto[ii][jj]->Integral(1, -1);
      integral[6][ii][jj] = H_phoEB_pt_jetjetdEta[ii][jj]->Integral(1, -1);
      integral[7][ii][jj] = H_phoEB_pt_jetjetdPhi[ii][jj]->Integral(1, -1);
      integral[8][ii][jj] = H_phoEB_pt_ptoverMET[ii][jj]->Integral(1, -1);
      integral[9][ii][jj] = H_phoEB_pt_mindJMETPhi[ii][jj]->Integral(1, -1);
      integral[10][ii][jj] = H_phoEB_pt_dijetMass[ii][jj]->Integral(1, -1);    
            
      //tgAE_phoEB_pt_130[ii][jj]->Divide(H_phoEB_pt_130[ii][jj], H_phoEB_pt_130[ii][jj]);
      tgAE_phoEB_pt_M[ii][jj]->Divide(H_phoEB_pt_M[ii][jj], H_phoEB_pt_130[ii][jj]);
      tgAE_phoEB_pt_leptonveto[ii][jj]->Divide(H_phoEB_pt_leptonveto[ii][jj], H_phoEB_pt_130[ii][jj]);
      tgAE_phoEB_pt_MET[ii][jj]->Divide(H_phoEB_pt_MET[ii][jj], H_phoEB_pt_130[ii][jj]);
      tgAE_phoEB_pt_dphoMETPhi[ii][jj]->Divide(H_phoEB_pt_dphoMETPhi[ii][jj], H_phoEB_pt_130[ii][jj]);
      tgAE_phoEB_pt_jetveto[ii][jj]->Divide(H_phoEB_pt_jetveto[ii][jj], H_phoEB_pt_130[ii][jj]);
      tgAE_phoEB_pt_jetjetdEta[ii][jj]->Divide(H_phoEB_pt_jetjetdEta[ii][jj], H_phoEB_pt_130[ii][jj]);
      tgAE_phoEB_pt_jetjetdPhi[ii][jj]->Divide(H_phoEB_pt_jetjetdPhi[ii][jj], H_phoEB_pt_130[ii][jj]);
      tgAE_phoEB_pt_ptoverMET[ii][jj]->Divide(H_phoEB_pt_ptoverMET[ii][jj], H_phoEB_pt_130[ii][jj]);
      tgAE_phoEB_pt_mindJMETPhi[ii][jj]->Divide(H_phoEB_pt_mindJMETPhi[ii][jj], H_phoEB_pt_130[ii][jj]);
      tgAE_phoEB_pt_dijetMass[ii][jj]->Divide(H_phoEB_pt_dijetMass[ii][jj], H_phoEB_pt_130[ii][jj]);      

      tgAE_MET_M[ii][jj]->Divide(H_MET_M[ii][jj], H_MET_130[ii][jj]);
      tgAE_MET_leptonveto[ii][jj]->Divide(H_MET_leptonveto[ii][jj], H_MET_130[ii][jj]);
      tgAE_MET_MET[ii][jj]->Divide(H_MET_MET[ii][jj], H_MET_130[ii][jj]);
      tgAE_MET_dphoMETPhi[ii][jj]->Divide(H_MET_dphoMETPhi[ii][jj], H_MET_130[ii][jj]);
      tgAE_MET_jetveto[ii][jj]->Divide(H_MET_jetveto[ii][jj], H_MET_130[ii][jj]);
      tgAE_MET_jetjetdEta[ii][jj]->Divide(H_MET_jetjetdEta[ii][jj], H_MET_130[ii][jj]);
      tgAE_MET_jetjetdPhi[ii][jj]->Divide(H_MET_jetjetdPhi[ii][jj], H_MET_130[ii][jj]);
      tgAE_MET_ptoverMET[ii][jj]->Divide(H_MET_ptoverMET[ii][jj], H_MET_130[ii][jj]);
      tgAE_MET_mindJMETPhi[ii][jj]->Divide(H_MET_mindJMETPhi[ii][jj], H_MET_130[ii][jj]);
      tgAE_MET_dijetMass[ii][jj]->Divide(H_MET_dijetMass[ii][jj], H_MET_130[ii][jj]);      

      tgAE_phoEB_pt_130[ii][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_130_%i_%i", ii, jj), Form("tgAE_phoEB_pteff_130_%i_%i", ii, jj));
      tgAE_phoEB_pt_M[ii][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_M_%i_%i", ii, jj), Form("tgAE_phoEB_pteff_M_%i_%i", ii, jj));
      tgAE_phoEB_pt_leptonveto[ii][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_leptonveto_%i_%i", ii, jj), Form("tgAE_phoEB_pteff_leptonveto_%i_%i", ii, jj));
      tgAE_phoEB_pt_MET[ii][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_MET_%i_%i", ii, jj), Form("tgAE_phoEB_pteff_MET_%i_%i", ii, jj));
      tgAE_phoEB_pt_dphoMETPhi[ii][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_dphoMETPhi_%i_%i", ii, jj), Form("tgAE_phoEB_pteff_dphoMETPhi_%i_%i", ii, jj));
      tgAE_phoEB_pt_jetveto[ii][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_jetveto_%i_%i", ii, jj), Form("tgAE_phoEB_pteff_jetveto_%i_%i", ii, jj));
      tgAE_phoEB_pt_jetjetdEta[ii][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_jetjetdEta_%i_%i", ii, jj), Form("tgAE_phoEB_pteff_jetjetdEta_%i_%i", ii, jj));
      tgAE_phoEB_pt_jetjetdPhi[ii][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_jetjetdPhi_%i_%i", ii, jj), Form("tgAE_phoEB_pteff_jetjetdPhi_%i_%i", ii, jj));
      tgAE_phoEB_pt_ptoverMET[ii][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_ptoverMET_%i_%i", ii, jj), Form("tgAE_phoEB_pteff_ptoverMET_%i_%i", ii, jj));
      tgAE_phoEB_pt_mindJMETPhi[ii][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_mindJMETPhi_%i_%i", ii, jj), Form("tgAE_phoEB_pteff_mindJMETPhi_%i_%i", ii, jj));
      tgAE_phoEB_pt_dijetMass[ii][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_dijetMass_%i_%i", ii, jj), Form("tgAE_phoEB_pteff_dijetMass_%i_%i", ii, jj));      

      tgAE_MET_130[ii][jj]->SetNameTitle(Form("tgAE_METeff_130_%i_%i", ii, jj), Form("tgAE_METeff_130_%i_%i", ii, jj));
      tgAE_MET_M[ii][jj]->SetNameTitle(Form("tgAE_METeff_M_%i_%i", ii, jj), Form("tgAE_METeff_M_%i_%i", ii, jj));
      tgAE_MET_leptonveto[ii][jj]->SetNameTitle(Form("tgAE_METeff_leptonveto_%i_%i", ii, jj), Form("tgAE_METeff_leptonveto_%i_%i", ii, jj));
      tgAE_MET_MET[ii][jj]->SetNameTitle(Form("tgAE_METeff_MET_%i_%i", ii, jj), Form("tgAE_METeff_MET_%i_%i", ii, jj));
      tgAE_MET_dphoMETPhi[ii][jj]->SetNameTitle(Form("tgAE_METeff_dphoMETPhi_%i_%i", ii, jj), Form("tgAE_METeff_dphoMETPhi_%i_%i", ii, jj));
      tgAE_MET_jetveto[ii][jj]->SetNameTitle(Form("tgAE_METeff_jetveto_%i_%i", ii, jj), Form("tgAE_METeff_jetveto_%i_%i", ii, jj));
      tgAE_MET_jetjetdEta[ii][jj]->SetNameTitle(Form("tgAE_METeff_jetjetdEta_%i_%i", ii, jj), Form("tgAE_METeff_jetjetdEta_%i_%i", ii, jj));
      tgAE_MET_jetjetdPhi[ii][jj]->SetNameTitle(Form("tgAE_METeff_jetjetdPhi_%i_%i", ii, jj), Form("tgAE_METeff_jetjetdPhi_%i_%i", ii, jj));
      tgAE_MET_ptoverMET[ii][jj]->SetNameTitle(Form("tgAE_METeff_ptoverMET_%i_%i", ii, jj), Form("tgAE_METeff_ptoverMET_%i_%i", ii, jj));
      tgAE_MET_mindJMETPhi[ii][jj]->SetNameTitle(Form("tgAE_METeff_mindJMETPhi_%i_%i", ii, jj), Form("tgAE_METeff_mindJMETPhi_%i_%i", ii, jj));
      tgAE_MET_dijetMass[ii][jj]->SetNameTitle(Form("tgAE_METeff_dijetMass_%i_%i", ii, jj), Form("tgAE_METeff_dijetMass_%i_%i", ii, jj));      

      tgAE_phoEB_pt_130[ii][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_M[ii][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_leptonveto[ii][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_MET[ii][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_dphoMETPhi[ii][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_jetveto[ii][jj]->SetMarkerStyle(8);                
      tgAE_phoEB_pt_jetjetdEta[ii][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_jetjetdPhi[ii][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_ptoverMET[ii][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_mindJMETPhi[ii][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_dijetMass[ii][jj]->SetMarkerStyle(8);      

      tgAE_MET_130[ii][jj]->SetMarkerStyle(8);
      tgAE_MET_M[ii][jj]->SetMarkerStyle(8);
      tgAE_MET_leptonveto[ii][jj]->SetMarkerStyle(8);
      tgAE_MET_MET[ii][jj]->SetMarkerStyle(8);
      tgAE_MET_dphoMETPhi[ii][jj]->SetMarkerStyle(8);
      tgAE_MET_jetveto[ii][jj]->SetMarkerStyle(8);                
      tgAE_MET_jetjetdEta[ii][jj]->SetMarkerStyle(8);
      tgAE_MET_jetjetdPhi[ii][jj]->SetMarkerStyle(8);
      tgAE_MET_ptoverMET[ii][jj]->SetMarkerStyle(8);
      tgAE_MET_mindJMETPhi[ii][jj]->SetMarkerStyle(8);
      tgAE_MET_dijetMass[ii][jj]->SetMarkerStyle(8);      

      tgAE_phoEB_pt_130[ii][jj]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_M[ii][jj]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_leptonveto[ii][jj]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_MET[ii][jj]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_dphoMETPhi[ii][jj]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_jetveto[ii][jj]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_jetjetdEta[ii][jj]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_jetjetdPhi[ii][jj]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_ptoverMET[ii][jj]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_mindJMETPhi[ii][jj]->SetMarkerSize(0.9);
      tgAE_phoEB_pt_dijetMass[ii][jj]->SetMarkerSize(0.9);      

      tgAE_MET_130[ii][jj]->SetMarkerSize(0.9);
      tgAE_MET_M[ii][jj]->SetMarkerSize(0.9);
      tgAE_MET_leptonveto[ii][jj]->SetMarkerSize(0.9);
      tgAE_MET_MET[ii][jj]->SetMarkerSize(0.9);
      tgAE_MET_dphoMETPhi[ii][jj]->SetMarkerSize(0.9);
      tgAE_MET_jetveto[ii][jj]->SetMarkerSize(0.9);
      tgAE_MET_jetjetdEta[ii][jj]->SetMarkerSize(0.9);
      tgAE_MET_jetjetdPhi[ii][jj]->SetMarkerSize(0.9);
      tgAE_MET_ptoverMET[ii][jj]->SetMarkerSize(0.9);
      tgAE_MET_mindJMETPhi[ii][jj]->SetMarkerSize(0.9);
      tgAE_MET_dijetMass[ii][jj]->SetMarkerSize(0.9);      

      tgAE_phoEB_pt_130[ii][jj]->SetLineWidth(1);
      tgAE_phoEB_pt_M[ii][jj]->SetLineWidth(1);
      tgAE_phoEB_pt_leptonveto[ii][jj]->SetLineWidth(1);
      tgAE_phoEB_pt_MET[ii][jj]->SetLineWidth(1);
      tgAE_phoEB_pt_dphoMETPhi[ii][jj]->SetLineWidth(1);
      tgAE_phoEB_pt_jetveto[ii][jj]->SetLineWidth(1);                
      tgAE_phoEB_pt_jetjetdEta[ii][jj]->SetLineWidth(1);
      tgAE_phoEB_pt_jetjetdPhi[ii][jj]->SetLineWidth(1);
      tgAE_phoEB_pt_ptoverMET[ii][jj]->SetLineWidth(1);
      tgAE_phoEB_pt_mindJMETPhi[ii][jj]->SetLineWidth(1);
      tgAE_phoEB_pt_dijetMass[ii][jj]->SetLineWidth(1);      

      tgAE_MET_130[ii][jj]->SetLineWidth(1);
      tgAE_MET_M[ii][jj]->SetLineWidth(1);
      tgAE_MET_leptonveto[ii][jj]->SetLineWidth(1);
      tgAE_MET_MET[ii][jj]->SetLineWidth(1);
      tgAE_MET_dphoMETPhi[ii][jj]->SetLineWidth(1);
      tgAE_MET_jetveto[ii][jj]->SetLineWidth(1);                
      tgAE_MET_jetjetdEta[ii][jj]->SetLineWidth(1);
      tgAE_MET_jetjetdPhi[ii][jj]->SetLineWidth(1);
      tgAE_MET_ptoverMET[ii][jj]->SetLineWidth(1);
      tgAE_MET_mindJMETPhi[ii][jj]->SetLineWidth(1);
      tgAE_MET_dijetMass[ii][jj]->SetLineWidth(1);      

      tgAE_phoEB_pt_M[ii][jj]->SetMarkerColor(cnum[0]);
      tgAE_phoEB_pt_leptonveto[ii][jj]->SetMarkerColor(cnum[1]);
      tgAE_phoEB_pt_MET[ii][jj]->SetMarkerColor(cnum[2]);
      tgAE_phoEB_pt_dphoMETPhi[ii][jj]->SetMarkerColor(cnum[3]);
      tgAE_phoEB_pt_jetveto[ii][jj]->SetMarkerColor(cnum[8]);
      tgAE_phoEB_pt_jetjetdEta[ii][jj]->SetMarkerColor(cnum[4]);
      tgAE_phoEB_pt_jetjetdPhi[ii][jj]->SetMarkerColor(cnum[5]);
      tgAE_phoEB_pt_ptoverMET[ii][jj]->SetMarkerColor(cnum[9]);
      tgAE_phoEB_pt_mindJMETPhi[ii][jj]->SetMarkerColor(cnum[6]);
      tgAE_phoEB_pt_dijetMass[ii][jj]->SetMarkerColor(cnum[7]);      

      tgAE_MET_M[ii][jj]->SetMarkerColor(cnum[0]);
      tgAE_MET_leptonveto[ii][jj]->SetMarkerColor(cnum[1]);
      tgAE_MET_MET[ii][jj]->SetMarkerColor(cnum[2]);
      tgAE_MET_dphoMETPhi[ii][jj]->SetMarkerColor(cnum[3]);
      tgAE_MET_jetveto[ii][jj]->SetMarkerColor(cnum[8]);
      tgAE_MET_jetjetdEta[ii][jj]->SetMarkerColor(cnum[4]);
      tgAE_MET_jetjetdPhi[ii][jj]->SetMarkerColor(cnum[5]);
      tgAE_MET_ptoverMET[ii][jj]->SetMarkerColor(cnum[9]);
      tgAE_MET_mindJMETPhi[ii][jj]->SetMarkerColor(cnum[6]);
      tgAE_MET_dijetMass[ii][jj]->SetMarkerColor(cnum[7]);      

    }					  
  }

  
  TMultiGraph *mg[nfile][nhisto];
  TMultiGraph *mg_MET[nfile][nhisto];
  TLegend *lmg = new TLegend(0.18,0.32, 0.30, 0.70);
  lmg->SetTextSize(0.03);

  //TF1 *fitresult = new TF1("fitresult", "pol0", 200, 500);
  TF1 *fitresult = new TF1("fitresult", "pol0", 0, 500); //Et75
  Double_t yinter[15][nfile][nhisto], err[15][nfile][nhisto];

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

  for(Int_t ii=0; ii<nfile; ii++){
    //if(i==1 || i==2) continue;
    for(Int_t jj=0; jj<nhisto; jj++){
    mg[ii][jj] = new TMultiGraph();
    mg_MET[ii][jj] = new TMultiGraph();

    mg[ii][jj]->Add(tgAE_phoEB_pt_M[ii][jj]);
    mg[ii][jj]->Add(tgAE_phoEB_pt_leptonveto[ii][jj]);
    mg[ii][jj]->Add(tgAE_phoEB_pt_MET[ii][jj]);
    mg[ii][jj]->Add(tgAE_phoEB_pt_dphoMETPhi[ii][jj]);
    mg[ii][jj]->Add(tgAE_phoEB_pt_jetveto[ii][jj]);
    mg[ii][jj]->Add(tgAE_phoEB_pt_jetjetdEta[ii][jj]);
    mg[ii][jj]->Add(tgAE_phoEB_pt_jetjetdPhi[ii][jj]);
    mg[ii][jj]->Add(tgAE_phoEB_pt_ptoverMET[ii][jj]);
    mg[ii][jj]->Add(tgAE_phoEB_pt_mindJMETPhi[ii][jj]);
    mg[ii][jj]->Add(tgAE_phoEB_pt_dijetMass[ii][jj]);    

    mg[ii][jj]->Draw("APE");
    mg[ii][jj]->GetXaxis()->SetTitle("p_{T}^{#gamma}#[]{GeV}");
    mg[ii][jj]->GetYaxis()->SetTitle("Efficiency");
    //mg[ii][jj]->GetXaxis()->SetLimits(-1, 500);
    mg[ii][jj]->GetXaxis()->SetLimits(-1, 500); // Et75
    mg[ii][jj]->SetMaximum(1.05);
    mg[ii][jj]->SetMinimum(0);
    //if(i==0) mg[i][jj]->SetMaximum(0.055);
    mg[ii][jj]->SetTitle(Form("cutflow_%i_%i", ii, jj));    
    
    lmg->Clear();
    lmg->AddEntry(tgAE_phoEB_pt_M[ii][jj], "Medium ID", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_leptonveto[ii][jj], "leptonveto", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_MET[ii][jj], "MET > 180GeV", "PE");
    //lmg->AddEntry(tgAE_phoEB_pt_MET[ii][jj], "phoEt/MET <2.4", "PE");
    
    if(jj==0){
      lmg->AddEntry(tgAE_phoEB_pt_dphoMETPhi[ii][jj], "|#Delta#phi(#gamma, MET)| > 1.2", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_jetveto[ii][jj], "jet SM", "PE");
    }
    else if(jj==1){
      lmg->AddEntry(tgAE_phoEB_pt_dphoMETPhi[ii][jj], "|#Delta#phi(j_{1}j_{2}, #gamma)| < 2.7", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_jetveto[ii][jj], "jet VBS", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_jetjetdEta[ii][jj], "#Delta#eta(j_{1},j_{2}) > 2.5", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_jetjetdPhi[ii][jj], "|#Delta#phi(j_{1},j_{2})| < 2.7", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_ptoverMET[ii][jj], "P_{T}^{#gamma}/MET < 2.4", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_mindJMETPhi[ii][jj], "|#Delta#phi_{min}(jet, MET)| > 0.5", "PE");
      lmg->AddEntry(tgAE_phoEB_pt_dijetMass[ii][jj], "Mass_{jj} > 250GeV", "PE");
    }
    lmg->Draw("SAME");
    title = mg[ii][jj]->GetTitle();
    CMS_lumi(c1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    mg_MET[ii][jj]->Add(tgAE_MET_M[ii][jj]);
    mg_MET[ii][jj]->Add(tgAE_MET_leptonveto[ii][jj]);
    mg_MET[ii][jj]->Add(tgAE_MET_MET[ii][jj]);
    mg_MET[ii][jj]->Add(tgAE_MET_dphoMETPhi[ii][jj]);
    mg_MET[ii][jj]->Add(tgAE_MET_jetveto[ii][jj]);
    mg_MET[ii][jj]->Add(tgAE_MET_jetjetdEta[ii][jj]);
    mg_MET[ii][jj]->Add(tgAE_MET_jetjetdPhi[ii][jj]);
    mg_MET[ii][jj]->Add(tgAE_MET_ptoverMET[ii][jj]);
    mg_MET[ii][jj]->Add(tgAE_MET_mindJMETPhi[ii][jj]);
    mg_MET[ii][jj]->Add(tgAE_MET_dijetMass[ii][jj]);    

    mg_MET[ii][jj]->Draw("APE");
    mg_MET[ii][jj]->GetXaxis()->SetTitle("MET#[]{GeV}");
    mg_MET[ii][jj]->GetYaxis()->SetTitle("Efficiency");
    //mg[ii][jj]->GetXaxis()->SetLimits(-1, 500);
    mg_MET[ii][jj]->GetXaxis()->SetLimits(-10, 500); // Et75
    mg_MET[ii][jj]->SetMaximum(1.05);
    mg_MET[ii][jj]->SetMinimum(0);
    //if(i==0) mg[i][jj]->SetMaximum(0.055);
    mg_MET[ii][jj]->SetTitle(Form("cutflowMET_%i_%i", ii, jj));    

    lmg->Clear();
    lmg->AddEntry(tgAE_MET_M[ii][jj], "Medium ID", "PE");
    lmg->AddEntry(tgAE_MET_leptonveto[ii][jj], "leptonveto", "PE");
    lmg->AddEntry(tgAE_MET_MET[ii][jj], "MET > 180GeV", "PE");
    //lmg->AddEntry(tgAE_MET_MET[ii][jj], "phoEt/MET <2.4", "PE");
     if(jj==0){
      lmg->AddEntry(tgAE_MET_dphoMETPhi[ii][jj], "|#Delta#phi(#gamma, MET)| > 1.2", "PE");
      lmg->AddEntry(tgAE_MET_jetveto[ii][jj], "jet SM", "PE");
    }
    else if(jj==1){
      lmg->AddEntry(tgAE_MET_dphoMETPhi[ii][jj], "|#Delta#phi(j_{1}j_{2}, #gamma)| < 2.7", "PE");
      lmg->AddEntry(tgAE_MET_jetveto[ii][jj], "jet VBS", "PE");
      lmg->AddEntry(tgAE_MET_jetjetdEta[ii][jj], "#Delta#eta(j_{1},j_{2}) > 2.5", "PE");
      lmg->AddEntry(tgAE_MET_jetjetdPhi[ii][jj], "|#Delta#phi(j_{1},j_{2})| < 2.7", "PE");
      lmg->AddEntry(tgAE_MET_ptoverMET[ii][jj], "P_{T}^{#gamma}/MET < 2.4", "PE");
      lmg->AddEntry(tgAE_MET_mindJMETPhi[ii][jj], "|#Delta#phi_{min}(jet, MET)| > 0.5", "PE");
      lmg->AddEntry(tgAE_MET_dijetMass[ii][jj], "Mass_{jj} > 250GeV", "PE");
    }
    lmg->Draw("SAME");
    title = mg_MET[ii][jj]->GetTitle();
    CMS_lumi(c1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    
    // Et75 [210, 500]->[0, 200]
    tgAE_phoEB_pt_M[ii][jj]->Fit(fitresult, "", "", 0, 500); 
    yinter[0][ii][jj] = fitresult->GetParameter(0);
    err[0][ii][jj] = fitresult->GetParError(0);
    tgAE_phoEB_pt_leptonveto[ii][jj]->Fit(fitresult, "", "", 0, 500);
    yinter[1][ii][jj] = fitresult->GetParameter(0);
    err[1][ii][jj] = fitresult->GetParError(0);
    tgAE_phoEB_pt_MET[ii][jj]->Fit(fitresult, "", "", 0, 500);
    yinter[2][ii][jj] = fitresult->GetParameter(0);
    err[2][ii][jj] = fitresult->GetParError(0);
    tgAE_phoEB_pt_dphoMETPhi[ii][jj]->Fit(fitresult, "", "", 0, 500);
    yinter[3][ii][jj] = fitresult->GetParameter(0);
    err[3][ii][jj] = fitresult->GetParError(0);
    tgAE_phoEB_pt_jetveto[ii][jj]->Fit(fitresult, "", "", 0, 500);
    yinter[4][ii][jj] = fitresult->GetParameter(0);
    err[4][ii][jj] = fitresult->GetParError(0);
    tgAE_phoEB_pt_jetjetdEta[ii][jj]->Fit(fitresult, "", "", 0, 500);
    yinter[5][ii][jj] = fitresult->GetParameter(0);
    err[5][ii][jj] = fitresult->GetParError(0);
    tgAE_phoEB_pt_jetjetdPhi[ii][jj]->Fit(fitresult, "", "", 0, 500);
    yinter[6][ii][jj] = fitresult->GetParameter(0);
    err[6][ii][jj] = fitresult->GetParError(0);
    tgAE_phoEB_pt_mindJMETPhi[ii][jj]->Fit(fitresult, "", "", 0, 500);
    yinter[7][ii][jj] = fitresult->GetParameter(0);
    err[7][ii][jj] = fitresult->GetParError(0);
    tgAE_phoEB_pt_dijetMass[ii][jj]->Fit(fitresult, "", "", 0, 500);
    yinter[8][ii][jj] = fitresult->GetParameter(0);
    err[8][ii][jj] = fitresult->GetParError(0);

    }
  }

  TString ltext[20] = {"pho MID", "lep veto", "MET", "dphoMETPhi", "jetPt", "jetjetdEta", "jetjetdPhi", "mindJMETPhi", "dijetMass"};
    
  //Float_t lumiscale[20] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.155635, 0.393159, 0.54324, 0.019488, 0.0081035};//[GJet, WJet, ZJet, DYJet, monoboson, diboson, triboson, Wg, Zg, ttJET, Znng, ZnngVBS]
  Float_t lumiscale[20] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.38889, 0.0134951};//[GJet, VJet, VG, monoboson, diboson, triboson, ttJET, ZnngVBS]
  
  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  for(Int_t i=0; i<nfile; i++){
    //if(i==1 || i==2) continue;
    ftext << "file " << i << endl;
    for(Int_t jj=0; jj<nhisto; jj++){
      for(Int_t j=0; j<9; j++){
	ftext << j << ")" << ltext[j] << " cut efficiency: " << yinter[j][i][jj] << " error: " << err[j][i][jj] << endl;
	//ftext << j << ") cut efficiency: " << (integral[j+1][i][jj]*lumiscale[i])/(integral[0][i][jj]*lumiscale[i]) << " error: " << err[j][i][jj] << endl;
      }
    }
  }
  
  ftext.close();

  ftext.open("eff_eachcut.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  for(Int_t i=0; i<nfile; i++){
    //if(i==1 || i==2) continue;
    ftext << "file " << i << endl;
    for(Int_t jj=0; jj<nhisto; jj++){
      for(Int_t j=0; j<9; j++){
	//ftext << j << ") cut efficiency: " << yinter[j][i][jj] << " error: " << err[j][i][jj] << endl;
	ftext << j << ")" << ltext[j] << " cut efficiency: " << (integral[j+1][i][jj]*lumiscale[i])/(integral[j][i][jj]*lumiscale[i]) << " error: " << err[j][i][jj] << endl;
      }
    }
  }
  
  ftext.close();
  
  
  ftext.open("events.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  for(Int_t i=0; i<nfile; i++){
    //if(i==1 || i==2) continue;
    ftext << "file " << i << endl;
    for(Int_t jj=0; jj<nhisto; jj++){
      for(Int_t j=0; j<10; j++){
	ftext << j << ") event yield: " << integral[j][i][jj]*lumiscale[i] << endl;
      }
    }
  }
  
  ftext.close();
  
}

 
