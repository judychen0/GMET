#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TColor.h"

using namespace std;

#define nfile 11
#define ncolor 10

void xDraw_h2plots(Int_t year){
  TString rootname[20];
  TFile *fopen;

  Float_t entries = 1.0;
  Float_t outentries = 0.;
  Float_t scale[nfile]  = {0};
  TH1F *H_Events[nfile];
  TH1F *HSumofGenW[nfile];

  Float_t lumi16 = 36.33;
  Float_t lumi17 = 41.48;
  Float_t lumi18 = 59.83;
  Float_t lumi = 1.0;
  if(year==2016){
    lumi = lumi16;
  }
  else if(year==2017){
    lumi = lumi17;
  }
  else if(year==2018){
    lumi = lumi18;
  }

  Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 0.1817};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, ZnngVBS]
  Float_t filter[20] = {1., 1., 1., 1., 1., 1., 1.};
  Float_t kfactor[20] = {1., 1., 1., 1., 1., 1., 1.};

  if(year==2016){
    rootname[0] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/mc/job_summer16_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/mc/job_summer16_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/mc/job_summer16_TT_aMCatNLO/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    //rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/mc/job_summer16_Zg_EWK/output_ggtree.root";
    rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/summer16/data/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/mc/job_fall17_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/mc/job_fall17_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/mc/job_fall17_TT_aMCatNLO/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    //rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/data/output_ggtree.root";
    rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/fall17/data/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/mc/job_autumn18_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/mc/job_autumn18_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/mc/job_autumn18_TT_aMCatNLO_ext1/output_ggtree.root";
    //rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/mc/job_autumn18_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/mc/job_UL18_Znunug_VBS/output_ggtree.root";
    rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220318/autumn18/data/output_ggtree.root";
  }

  for(Int_t i=0; i<nfile-1; i++){
    fopen = new TFile(rootname[i]);
    if((i<nfile-1) && i>=7){
      H_Events[i] = (TH1F*)fopen->Get("hEvents");
      //HSumofGenW[i] = (TH1F*)fopen->Get("hSumofGenW");
      HSumofGenW[i] = (TH1F*)fopen->Get("hSumGenWeight");
      entries = 1.0;
      //entries = H_Events[i]->GetBinContent(1);
      entries = HSumofGenW[i]->GetBinContent(1);
      outentries = xsec[i]*1000*lumi;
      scale[i] = 0;
      scale[i] = fabs(outentries/entries);

      cout << "print " << entries << " " << outentries << " " << scale[i]<< endl;
    }
    //if(i==nfile-1 || i<3) scale = 1.0;
    if(i<7) scale[i] = 1.0;
  }

  //color name
  string hexcolor[ncolor] = {"#fff176", "#03a9f4", "#ff3360", "#3d5afe", "#b23b8c", "#96ff5a", "#a983d3", "#ff9e00", "#00FFFF", "#FFCCCC", "#67ccc1"};
  TColor *color[ncolor];
  Int_t cnum[ncolor];
   
  for(Int_t j=0; j<ncolor; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  } 
  
  TCanvas *c1 = new TCanvas("c1");

  TH2F *H2_MET_minJMETdPhi[nfile];
  TH2F *H2_MET_dijetMass[nfile];
  TH2F *H2_MET_ptoverMET[nfile];
  TH2F *H2_dijetMass_minJMETdPhi[nfile];
  TH2F *H2_dijetMass_ptoverMET[nfile];
    
  TH2F *HA2_MET_minJMETdPhi;
  TH2F *HA2_MET_dijetMass;
  TH2F *HA2_MET_ptoverMET;
  TH2F *HA2_dijetMass_minJMETdPhi;
  TH2F *HA2_dijetMass_ptoverMET;
  
  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i], "READ");

    H2_MET_minJMETdPhi[i] = (TH2F*)fopen->Get("h2_plots/h2_MET_minJMETdPhi");
    H2_MET_dijetMass[i] = (TH2F*)fopen->Get("h2_plots/h2_MET_dijetMass");
    H2_MET_ptoverMET[i] = (TH2F*)fopen->Get("h2_plots/h2_MET_ptoverMET");
    H2_dijetMass_minJMETdPhi[i] = (TH2F*)fopen->Get("h2_plots/h2_dijetMass_minJMETdPhi");
    H2_dijetMass_ptoverMET[i] = (TH2F*)fopen->Get("h2_plots/h2_dijetMass_ptoverMET");

    if(i<nfile-1){
      HA2_MET_minJMETdPhi->Add(H2_MET_minJMETdPhi[i]);
      HA2_MET_dijetMass->Add(H2_MET_dijetMass[i]);
      HA2_MET_ptoverMET->Add(H2_MET_ptoverMET[i]);
      HA2_dijetMass_minJMETdPhi->Add(H2_dijetMass_minJMETdPhi[i]);
      HA2_dijetMass_ptoverMET->Add(H2_dijetMass_ptoverMET[i]);
    }
  }

  HA2_MET_minJMETdPhi->Draw("colz");
  HA2_MET_minJMETdPhi->GetXaxis()->SetTitle("MET [GeV]");
  HA2_MET_minJMETdPhi->GetYaxis()->SetTitle("#Delta#phi_{min}(jet,MET)");
  
  
  H2_MET_minJMETdPhi[nfile-1]->Draw("BOX SAME");
  
  HA2_MET_dijetMass;
  HA2_MET_ptoverMET;
  HA2_dijetMass_minJMETdPhi;
  HA2_dijetMass_ptoverMET;

  
}
