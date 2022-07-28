#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "../Drawlumi/CMS_lumi.C"

using namespace std;
#define nfile 12
#define nhisto 2
#define njet 2
#define ncolor 20

TH1F* ratioplot(THStack* st, TH1F* h1){

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
  
    
  TH1F *ratio = (TH1F*)h1->Clone("ratio");
  ratio->SetLineColor(kBlack);
  ratio->SetMinimum(-0.4);
  ratio->SetMaximum(3.4);
  ratio->Sumw2();
  ratio->Divide(tmpHist);
  
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerSize(0.8);
  ratio->Draw("E3 P");
  
  ratio->SetTitle("");
  ratio->GetYaxis()->SetTitle("Data/MC");
  ratio->GetYaxis()->SetTitleFont(42);
  ratio->GetYaxis()->SetTitleSize(0.1);
  ratio->GetYaxis()->SetTitleOffset(0.4);
  ratio->GetYaxis()->SetLabelFont(42);
  ratio->GetYaxis()->SetLabelOffset(0.008);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetYaxis()->SetTickSize(0.032);
  
  ratio->GetXaxis()->SetTitleFont(42);
  ratio->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetTitleOffset(1.1);
  ratio->GetXaxis()->SetLabelFont(42);
  ratio->GetXaxis()->SetLabelOffset(0.03);
  ratio->GetXaxis()->SetLabelSize(0.1);
  ratio->GetXaxis()->SetTickSize(0.1);
  ratio->GetXaxis()->SetNdivisions(510);
  ratio->GetXaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");

  return ratio;
}

void xDrawStack_ptoverMET_SR(Int_t year){
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
  
  //Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 1.41};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, ZgEWK]
  //Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 0.296};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, Znng]
  Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 0.1817};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, ZnngVBS]
  
  Float_t filter[20] = {1., 1., 1., 1., 1., 1., 1.};
  Float_t kfactor[20] = {1., 1., 1., 1., 1., 1., 1.};
  
  
  if(year==2016){
    rootname[0] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/mc/job_summer16_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/mc/job_summer16_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/mc/job_summer16_TT_aMCatNLO/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    //rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/mc/job_summer16_Zg_EWK/output_ggtree.root";
    rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220324/summer16/data/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/GJet/output_merge_GJet.root";
    rootname[0] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/GJet/GJet_ptoverMET_CR.root";
    rootname[1] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/job_fall17_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/job_fall17_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/job_fall17_TT_aMCatNLO/output_ggtree.root";
    rootname[10] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    rootname[11] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/data/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/mc/job_autumn18_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/mc/job_autumn18_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/mc/job_autumn18_TT_aMCatNLO_ext1/output_ggtree.root";
    rootname[10] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/mc/job_UL18_Znunug_VBS/output_ggtree.root";
    rootname[11] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/data/output_ggtree.root";
  }

  for(Int_t i=0; i<nfile-1; i++){
    //if(i==10) continue;//pass Zvv signal
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
  //Float_t mclumi[5] = {0.31,311.6,6750,989.7};//(fb-1) [0,1,2,3,4][GJet,WG,ZG,ZNuNu]
  //Float_t mclumi[5] = {311.6,6750,989.7};//(fb-1) [1,2,3,4][WG,ZG,ZNuNu]
  
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

  TH1F *H_phoEB_ptcut_ptoverMET_SR[nfile];
  TH1F *H_jetpt_ptoverMET_SR[nfile];
  TH1F *H_MET_ptoverMET_SR[nfile];
  TH1F *H_dphoMETPhi_ptoverMET_SR[nfile];
  TH1F *H_dijetMass_ptoverMET_SR[nfile];
  TH1F *H_dEta_jetjet_ptoverMET_SR[nfile];
  TH1F *H_minJMETdPhi_ptoverMET_SR[nfile];

  TH1F *HA_phoEB_ptcut_ptoverMET_SR;
  TH1F *HA_jetpt_ptoverMET_SR;
  TH1F *HA_MET_ptoverMET_SR;
  TH1F *HA_dphoMETPhi_ptoverMET_SR;
  TH1F *HA_dijetMass_ptoverMET_SR;
  TH1F *HA_dEta_jetjet_ptoverMET_SR;
  TH1F *HA_minJMETdPhi_ptoverMET_SR;

  TH1F *HSig_phoEB_ptcut_ptoverMET_SR;
  TH1F *HSig_jetpt_ptoverMET_SR;
  TH1F *HSig_MET_ptoverMET_SR;
  TH1F *HSig_dphoMETPhi_ptoverMET_SR;
  TH1F *HSig_dijetMass_ptoverMET_SR;
  TH1F *HSig_dEta_jetjet_ptoverMET_SR;
  TH1F *HSig_minJMETdPhi_ptoverMET_SR;

  const char *title;
  const char *saveto = ".";

  Float_t integral[nfile][7];
  
  for(Int_t i=0; i<nfile; i++){

    fopen = new TFile(rootname[i]);
    if(i==nfile-1) scale[i] = 1.0;

    if(i==0){

      HA_phoEB_ptcut_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_phoEB_ptcut_ptoverMET_SR")->Clone();
      HA_jetpt_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_jetpt_ptoverMET_SR")->Clone();
      HA_MET_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_MET_ptoverMET_SR")->Clone();
      HA_dphoMETPhi_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_dphoMETPhi_ptoverMET_SR")->Clone();
      HA_dijetMass_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_dijetMass_ptoverMET_SR")->Clone();
      HA_dEta_jetjet_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_dEta_jetjet_ptoverMET_SR")->Clone();
      HA_minJMETdPhi_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_minJMETdPhi_ptoverMET_SR")->Clone();

      ///
      HA_phoEB_ptcut_ptoverMET_SR->Scale(scale[i]);
      HA_jetpt_ptoverMET_SR->Scale(scale[i]);
      HA_MET_ptoverMET_SR->Scale(scale[i]);
      HA_dphoMETPhi_ptoverMET_SR->Scale(scale[i]);
      HA_dijetMass_ptoverMET_SR->Scale(scale[i]);
      HA_dEta_jetjet_ptoverMET_SR->Scale(scale[i]);
      HA_minJMETdPhi_ptoverMET_SR->Scale(scale[i]);

      ///
      HA_phoEB_ptcut_ptoverMET_SR->SetFillStyle(3145);
      HA_jetpt_ptoverMET_SR->SetFillStyle(3145);
      HA_MET_ptoverMET_SR->SetFillStyle(3145);
      HA_dphoMETPhi_ptoverMET_SR->SetFillStyle(3145);
      HA_dijetMass_ptoverMET_SR->SetFillStyle(3145);
      HA_dEta_jetjet_ptoverMET_SR->SetFillStyle(3145);
      HA_minJMETdPhi_ptoverMET_SR->SetFillStyle(3145);

      ///
      HA_phoEB_ptcut_ptoverMET_SR->SetFillColor(12);
      HA_jetpt_ptoverMET_SR->SetFillColor(12);
      HA_MET_ptoverMET_SR->SetFillColor(12);
      HA_dphoMETPhi_ptoverMET_SR->SetFillColor(12);
      HA_dijetMass_ptoverMET_SR->SetFillColor(12);
      HA_dEta_jetjet_ptoverMET_SR->SetFillColor(12);
      HA_minJMETdPhi_ptoverMET_SR->SetFillColor(12);

      ///
      HA_phoEB_ptcut_ptoverMET_SR->SetMarkerSize(0.1);
      HA_jetpt_ptoverMET_SR->SetMarkerSize(0.1);
      HA_MET_ptoverMET_SR->SetMarkerSize(0.1);
      HA_dphoMETPhi_ptoverMET_SR->SetMarkerSize(0.1);
      HA_dijetMass_ptoverMET_SR->SetMarkerSize(0.1);
      HA_dEta_jetjet_ptoverMET_SR->SetMarkerSize(0.1);
      HA_minJMETdPhi_ptoverMET_SR->SetMarkerSize(0.1);
      
    }

    if(i==nfile-2){

      HSig_phoEB_ptcut_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_phoEB_ptcut_ptoverMET_SR")->Clone();
      HSig_jetpt_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_jetpt_ptoverMET_SR")->Clone();
      HSig_MET_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_MET_ptoverMET_SR")->Clone();
      HSig_dphoMETPhi_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_dphoMETPhi_ptoverMET_SR")->Clone();
      HSig_dijetMass_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_dijetMass_ptoverMET_SR")->Clone();
      HSig_dEta_jetjet_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_dEta_jetjet_ptoverMET_SR")->Clone();
      HSig_minJMETdPhi_ptoverMET_SR = (TH1F*)fopen->Get("ptoverMET_SR/h_minJMETdPhi_ptoverMET_SR")->Clone();

      ///
      HSig_phoEB_ptcut_ptoverMET_SR->Scale(scale[i]);
      HSig_jetpt_ptoverMET_SR->Scale(scale[i]);
      HSig_MET_ptoverMET_SR->Scale(scale[i]);
      HSig_dphoMETPhi_ptoverMET_SR->Scale(scale[i]);
      HSig_dijetMass_ptoverMET_SR->Scale(scale[i]);
      HSig_dEta_jetjet_ptoverMET_SR->Scale(scale[i]);
      HSig_minJMETdPhi_ptoverMET_SR->Scale(scale[i]);

      ///
      HSig_phoEB_ptcut_ptoverMET_SR->SetLineColor(kRed);
      HSig_jetpt_ptoverMET_SR->SetLineColor(kRed);
      HSig_MET_ptoverMET_SR->SetLineColor(kRed);
      HSig_dphoMETPhi_ptoverMET_SR->SetLineColor(kRed);
      HSig_dijetMass_ptoverMET_SR->SetLineColor(kRed);
      HSig_dEta_jetjet_ptoverMET_SR->SetLineColor(kRed);
      HSig_minJMETdPhi_ptoverMET_SR->SetLineColor(kRed);

      ///
      HSig_phoEB_ptcut_ptoverMET_SR->SetLineWidth(2);
      HSig_jetpt_ptoverMET_SR->SetLineWidth(2);
      HSig_MET_ptoverMET_SR->SetLineWidth(2);
      HSig_dphoMETPhi_ptoverMET_SR->SetLineWidth(2);
      HSig_dijetMass_ptoverMET_SR->SetLineWidth(2);
      HSig_dEta_jetjet_ptoverMET_SR->SetLineWidth(2);
      HSig_minJMETdPhi_ptoverMET_SR->SetLineWidth(2);
      
    }

    H_phoEB_ptcut_ptoverMET_SR[i] = (TH1F*)fopen->Get("ptoverMET_SR/h_phoEB_ptcut_ptoverMET_SR");
    H_jetpt_ptoverMET_SR[i] = (TH1F*)fopen->Get("ptoverMET_SR/h_jetpt_ptoverMET_SR");
    H_MET_ptoverMET_SR[i] = (TH1F*)fopen->Get("ptoverMET_SR/h_MET_ptoverMET_SR");
    H_dphoMETPhi_ptoverMET_SR[i] = (TH1F*)fopen->Get("ptoverMET_SR/h_dphoMETPhi_ptoverMET_SR");
    H_dijetMass_ptoverMET_SR[i] = (TH1F*)fopen->Get("ptoverMET_SR/h_dijetMass_ptoverMET_SR");
    H_dEta_jetjet_ptoverMET_SR[i] = (TH1F*)fopen->Get("ptoverMET_SR/h_dEta_jetjet_ptoverMET_SR");
    H_minJMETdPhi_ptoverMET_SR[i] = (TH1F*)fopen->Get("ptoverMET_SR/h_minJMETdPhi_ptoverMET_SR");

    H_phoEB_ptcut_ptoverMET_SR[i]->Scale(scale[i]);
    H_jetpt_ptoverMET_SR[i]->Scale(scale[i]);
    H_MET_ptoverMET_SR[i]->Scale(scale[i]);
    H_dphoMETPhi_ptoverMET_SR[i]->Scale(scale[i]);
    H_dijetMass_ptoverMET_SR[i]->Scale(scale[i]);
    H_dEta_jetjet_ptoverMET_SR[i]->Scale(scale[i]);
    H_minJMETdPhi_ptoverMET_SR[i]->Scale(scale[i]);

    integral[i][0] = H_phoEB_ptcut_ptoverMET_SR[i]->Integral(1, -1);
    integral[i][1] = H_jetpt_ptoverMET_SR[i]->Integral(1, -1);
    integral[i][2] = H_MET_ptoverMET_SR[i]->Integral(1, -1);
    integral[i][3] = H_dphoMETPhi_ptoverMET_SR[i]->Integral(1, -1);
    integral[i][4] = H_dijetMass_ptoverMET_SR[i]->Integral(1, -1);
    integral[i][5] = H_dEta_jetjet_ptoverMET_SR[i]->Integral(1, -1);
    integral[i][6] = H_minJMETdPhi_ptoverMET_SR[i]->Integral(1, -1);

    if(i<nfile-1){

      H_phoEB_ptcut_ptoverMET_SR[i]->SetFillColor(cnum[i]);
      H_jetpt_ptoverMET_SR[i]->SetFillColor(cnum[i]);
      H_MET_ptoverMET_SR[i]->SetFillColor(cnum[i]);
      H_dphoMETPhi_ptoverMET_SR[i]->SetFillColor(cnum[i]);
      H_dijetMass_ptoverMET_SR[i]->SetFillColor(cnum[i]);
      H_dEta_jetjet_ptoverMET_SR[i]->SetFillColor(cnum[i]);
      H_minJMETdPhi_ptoverMET_SR[i]->SetFillColor(cnum[i]);

    }

    ///    
    H_phoEB_ptcut_ptoverMET_SR[i]->SetLineColor(1);
    H_jetpt_ptoverMET_SR[i]->SetLineColor(1);
    H_MET_ptoverMET_SR[i]->SetLineColor(1);
    H_dphoMETPhi_ptoverMET_SR[i]->SetLineColor(1);
    H_dijetMass_ptoverMET_SR[i]->SetLineColor(1);
    H_dEta_jetjet_ptoverMET_SR[i]->SetLineColor(1);
    H_minJMETdPhi_ptoverMET_SR[i]->SetLineColor(1);

    ///
    H_phoEB_ptcut_ptoverMET_SR[i]->SetLineWidth(1);
    H_jetpt_ptoverMET_SR[i]->SetLineWidth(1);
    H_MET_ptoverMET_SR[i]->SetLineWidth(1);
    H_dphoMETPhi_ptoverMET_SR[i]->SetLineWidth(1);
    H_dijetMass_ptoverMET_SR[i]->SetLineWidth(1);
    H_dEta_jetjet_ptoverMET_SR[i]->SetLineWidth(1);
    H_minJMETdPhi_ptoverMET_SR[i]->SetLineWidth(1);

    if(i==nfile-1){

      ///
      H_phoEB_ptcut_ptoverMET_SR[i]->SetLineWidth(2);
      H_jetpt_ptoverMET_SR[i]->SetLineWidth(2);
      H_MET_ptoverMET_SR[i]->SetLineWidth(2);
      H_dphoMETPhi_ptoverMET_SR[i]->SetLineWidth(2);
      H_dijetMass_ptoverMET_SR[i]->SetLineWidth(2);
      H_dEta_jetjet_ptoverMET_SR[i]->SetLineWidth(2);
      H_minJMETdPhi_ptoverMET_SR[i]->SetLineWidth(2);

      ///
      H_phoEB_ptcut_ptoverMET_SR[i]->SetMarkerStyle(8);
      H_jetpt_ptoverMET_SR[i]->SetMarkerStyle(8);
      H_MET_ptoverMET_SR[i]->SetMarkerStyle(8);
      H_dphoMETPhi_ptoverMET_SR[i]->SetMarkerStyle(8);
      H_dijetMass_ptoverMET_SR[i]->SetMarkerStyle(8);
      H_dEta_jetjet_ptoverMET_SR[i]->SetMarkerStyle(8);
      H_minJMETdPhi_ptoverMET_SR[i]->SetMarkerStyle(8);

      ///
      H_phoEB_ptcut_ptoverMET_SR[i]->SetMarkerSize(0.9);
      H_jetpt_ptoverMET_SR[i]->SetMarkerSize(0.9);
      H_MET_ptoverMET_SR[i]->SetMarkerSize(0.9);
      H_dphoMETPhi_ptoverMET_SR[i]->SetMarkerSize(0.9);
      H_dijetMass_ptoverMET_SR[i]->SetMarkerSize(0.9);
      H_dEta_jetjet_ptoverMET_SR[i]->SetMarkerSize(0.9);
      H_minJMETdPhi_ptoverMET_SR[i]->SetMarkerSize(0.9);

    }
  }

  for(Int_t i=0; i<nfile-1; i++){

    if(i>0){

      HA_phoEB_ptcut_ptoverMET_SR->Add(H_phoEB_ptcut_ptoverMET_SR[i]);
      HA_jetpt_ptoverMET_SR->Add(H_jetpt_ptoverMET_SR[i]);
      HA_MET_ptoverMET_SR->Add(H_MET_ptoverMET_SR[i]);
      HA_dphoMETPhi_ptoverMET_SR->Add(H_dphoMETPhi_ptoverMET_SR[i]);
      HA_dijetMass_ptoverMET_SR->Add(H_dijetMass_ptoverMET_SR[i]);
      HA_dEta_jetjet_ptoverMET_SR->Add(H_dEta_jetjet_ptoverMET_SR[i]);
      HA_minJMETdPhi_ptoverMET_SR->Add(H_minJMETdPhi_ptoverMET_SR[i]);

    }
  }

  THStack *HS_phoEB_ptcut_ptoverMET_SR;
  THStack *HS_jetpt_ptoverMET_SR;
  THStack *HS_MET_ptoverMET_SR;
  THStack *HS_dphoMETPhi_ptoverMET_SR;
  THStack *HS_dijetMass_ptoverMET_SR;
  THStack *HS_dEta_jetjet_ptoverMET_SR;
  THStack *HS_minJMETdPhi_ptoverMET_SR;

  
  HS_phoEB_ptcut_ptoverMET_SR = new THStack("HS_phoEB_ptcut_ptoverMET_SR", "HS_phoEB_ptcut_ptoverMET_SR");
  HS_jetpt_ptoverMET_SR = new THStack("HS_jetpt_ptoverMET_SR", "HS_jetpt_ptoverMET_SR");
  HS_MET_ptoverMET_SR = new THStack("HS_MET_ptoverMET_SR", "HS_MET_ptoverMET_SR");
  HS_dphoMETPhi_ptoverMET_SR = new THStack("HS_dphoMETPhi_ptoverMET_SR", "HS_dphoMETPhi_ptoverMET_SR");
  HS_dijetMass_ptoverMET_SR = new THStack("HS_dijetMass_ptoverMET_SR", "HS_dijetMass_ptoverMET_SR");
  HS_dEta_jetjet_ptoverMET_SR = new THStack("HS_dEta_jetjet_ptoverMET_SR", "HS_dEta_jetjet_ptoverMET_SR");
  HS_minJMETdPhi_ptoverMET_SR = new THStack("HS_minJMETdPhi_ptoverMET_SR", "HS_minJMETdPhi_ptoverMET_SR");

  for(Int_t i=0; i<nfile-1; i++){

    HS_phoEB_ptcut_ptoverMET_SR->Add(H_phoEB_ptcut_ptoverMET_SR[i]);
    HS_jetpt_ptoverMET_SR->Add(H_jetpt_ptoverMET_SR[i]);
    HS_MET_ptoverMET_SR->Add(H_MET_ptoverMET_SR[i]);
    HS_dphoMETPhi_ptoverMET_SR->Add(H_dphoMETPhi_ptoverMET_SR[i]);
    HS_dijetMass_ptoverMET_SR->Add(H_dijetMass_ptoverMET_SR[i]);
    HS_dEta_jetjet_ptoverMET_SR->Add(H_dEta_jetjet_ptoverMET_SR[i]);
    HS_minJMETdPhi_ptoverMET_SR->Add(H_minJMETdPhi_ptoverMET_SR[i]);

  }

  TLegend *lhs = new TLegend(0.50, 0.66, 0.90, 0.90);//VBS
  lhs->SetTextSize(0.035);
  lhs->SetFillStyle(0);
  //TString ltext[20] = {"#gamma+jet", "W(#rightarrowl#nu)+jets", "Z(#rightarrow#nu#nu)+jets", "Z(#rightarrowll)+jets", "Monoboson", "Diboson", "Triboson", "Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug", "DATA"};//Znunug QCD
  TString ltext[20] = {"#gamma+jet", "W(#rightarrowl#nu)+jets", "Z(#rightarrow#nu#nu)+jets", "Z(#rightarrowll)+jets", "Monoboson", "Diboson", "Triboson", "Wg#rightarrowl#nu#gamma", "Z#gamma#rightarrowll#gamma", "ttJets", "Z#gamma#rightarrow#nu#nu#gamma(VBS)", "DATA"};//Znunug VBS

  TH1F *hratio;
  Double_t x[3] = {-2000, 0.0, 2000};
  Double_t y[3] = {1, 1, 1};
  TGraph *tg = new TGraph(3, x, y);
  tg->SetLineColor(kRed);
  tg->SetLineWidth(1);
  tg->SetLineStyle(8);

  TPad *pad1 = new TPad("pad1", "", 0., 0.3, 1., 1.);
  //pad1->SetTopMargin(0.03);
  pad1->SetBottomMargin(0);
  //pad1->SetLogy(1);
  pad1->Draw();
  
  TPad *pad2 = new TPad("pad2", "", 0., 0.02, 1., 0.3);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.3);
  pad2->Draw();

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
  
  //save plots//

  pad1->cd();
  HS_phoEB_ptcut_ptoverMET_SR->Draw("HIST");
  H_phoEB_ptcut_ptoverMET_SR[nfile-1]->Draw("SAME");
  HSig_phoEB_ptcut_ptoverMET_SR->Draw("HISTSAME");
  HA_phoEB_ptcut_ptoverMET_SR->Draw("E2 SAME");
  HS_phoEB_ptcut_ptoverMET_SR->GetYaxis()->SetTitle("Events");
  HS_phoEB_ptcut_ptoverMET_SR->GetXaxis()->SetTitle("");
  //HS_phoEB_ptcut_ptoverMET_SR->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_phoEB_ptcut_ptoverMET_SR->GetXaxis()->SetLabelSize(0);
  HS_phoEB_ptcut_ptoverMET_SR->GetXaxis()->SetRangeUser(210, 1000);
  HS_phoEB_ptcut_ptoverMET_SR->SetMaximum(150);
  HS_phoEB_ptcut_ptoverMET_SR->SetMinimum(0);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_phoEB_ptcut_ptoverMET_SR[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_phoEB_ptcut_ptoverMET_SR, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_phoEB_ptcut_ptoverMET_SR[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_phoEB_ptcut_ptoverMET_SR->GetName();
  pad2->cd();
  hratio = ratioplot(HS_phoEB_ptcut_ptoverMET_SR, H_phoEB_ptcut_ptoverMET_SR[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("P_{T}^{#gamma}[GeV]");
  hratio->GetXaxis()->SetRangeUser(210, 1000);
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_jetpt_ptoverMET_SR->Draw("HIST");
  H_jetpt_ptoverMET_SR[nfile-1]->Draw("SAME");
  HSig_jetpt_ptoverMET_SR->Draw("HISTSAME");
  HA_jetpt_ptoverMET_SR->Draw("E2 SAME");
  HS_jetpt_ptoverMET_SR->GetYaxis()->SetTitle("Events");
  HS_jetpt_ptoverMET_SR->GetXaxis()->SetTitle("");
  //HS_jetpt_ptoverMET_SR->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_jetpt_ptoverMET_SR->GetXaxis()->SetLabelSize(0);
  HS_jetpt_ptoverMET_SR->SetMaximum(300);
  HS_jetpt_ptoverMET_SR->SetMinimum(0);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_jetpt_ptoverMET_SR[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_jetpt_ptoverMET_SR, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_jetpt_ptoverMET_SR[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_jetpt_ptoverMET_SR->GetName();
  pad2->cd();
  hratio = ratioplot(HS_jetpt_ptoverMET_SR, H_jetpt_ptoverMET_SR[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("P_{T}^{jet}[GeV]");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_MET_ptoverMET_SR->Draw("HIST");
  H_MET_ptoverMET_SR[nfile-1]->Draw("SAME");
  HSig_MET_ptoverMET_SR->Draw("HISTSAME");
  HA_MET_ptoverMET_SR->Draw("E2 SAME");
  HS_MET_ptoverMET_SR->GetYaxis()->SetTitle("Events");
  HS_MET_ptoverMET_SR->GetXaxis()->SetTitle("");
  //HS_MET_ptoverMET_SR->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_MET_ptoverMET_SR->GetXaxis()->SetLabelSize(0);
  HS_MET_ptoverMET_SR->SetMaximum(500);
  //else if(year==2017)HS_MET_ptoverMET_SR->SetMaximum(100);
  HS_MET_ptoverMET_SR->SetMinimum(0);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_MET_ptoverMET_SR[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_MET_ptoverMET_SR, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_MET_ptoverMET_SR[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_MET_ptoverMET_SR->GetName();
  pad2->cd();
  hratio = ratioplot(HS_MET_ptoverMET_SR, H_MET_ptoverMET_SR[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("MET [GeV]");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_dphoMETPhi_ptoverMET_SR->Draw("HIST");
  H_dphoMETPhi_ptoverMET_SR[nfile-1]->Draw("SAME");
  HSig_dphoMETPhi_ptoverMET_SR->Draw("HISTSAME");
  HA_dphoMETPhi_ptoverMET_SR->Draw("E2 SAME");
  HS_dphoMETPhi_ptoverMET_SR->GetYaxis()->SetTitle("Events");
  HS_dphoMETPhi_ptoverMET_SR->GetXaxis()->SetTitle("");
  //HS_dphoMETPhi_ptoverMET_SR->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_dphoMETPhi_ptoverMET_SR->GetXaxis()->SetLabelSize(0);
  HS_dphoMETPhi_ptoverMET_SR->SetMaximum(100);
  HS_dphoMETPhi_ptoverMET_SR->SetMinimum(0);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_dphoMETPhi_ptoverMET_SR[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dphoMETPhi_ptoverMET_SR, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dphoMETPhi_ptoverMET_SR[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_dphoMETPhi_ptoverMET_SR->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dphoMETPhi_ptoverMET_SR, H_dphoMETPhi_ptoverMET_SR[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#Delta#phi(#gamma, MET)");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_dijetMass_ptoverMET_SR->Draw("HIST");
  H_dijetMass_ptoverMET_SR[nfile-1]->Draw("SAME");
  HSig_dijetMass_ptoverMET_SR->Draw("HISTSAME");
  HA_dijetMass_ptoverMET_SR->Draw("E2 SAME");
  HS_dijetMass_ptoverMET_SR->GetYaxis()->SetTitle("Events");
  HS_dijetMass_ptoverMET_SR->GetXaxis()->SetTitle("");
  HS_dijetMass_ptoverMET_SR->GetXaxis()->SetRangeUser(0, 2000);
  //HS_dijetMass_ptoverMET_SR->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_dijetMass_ptoverMET_SR->GetXaxis()->SetLabelSize(0);
  HS_dijetMass_ptoverMET_SR->SetMaximum(100);
  HS_dijetMass_ptoverMET_SR->SetMinimum(0);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_dijetMass_ptoverMET_SR[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dijetMass_ptoverMET_SR, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dijetMass_ptoverMET_SR[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_dijetMass_ptoverMET_SR->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dijetMass_ptoverMET_SR, H_dijetMass_ptoverMET_SR[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("Mass^{jj}[GeV]");
  hratio->GetXaxis()->SetRangeUser(0, 2000);
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_dEta_jetjet_ptoverMET_SR->Draw("HIST");
  H_dEta_jetjet_ptoverMET_SR[nfile-1]->Draw("SAME");
  HSig_dEta_jetjet_ptoverMET_SR->Draw("HISTSAME");
  HA_dEta_jetjet_ptoverMET_SR->Draw("E2 SAME");
  HS_dEta_jetjet_ptoverMET_SR->GetYaxis()->SetTitle("Events");
  HS_dEta_jetjet_ptoverMET_SR->GetXaxis()->SetTitle("");
  //HS_dEta_jetjet_ptoverMET_SR->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_dEta_jetjet_ptoverMET_SR->GetXaxis()->SetLabelSize(0);
  HS_dEta_jetjet_ptoverMET_SR->SetMaximum(200);
  HS_dEta_jetjet_ptoverMET_SR->SetMinimum(0);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_dEta_jetjet_ptoverMET_SR[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dEta_jetjet_ptoverMET_SR, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dEta_jetjet_ptoverMET_SR[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_dEta_jetjet_ptoverMET_SR->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dEta_jetjet_ptoverMET_SR, H_dEta_jetjet_ptoverMET_SR[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#Delta#eta(j_{1}, j_{2})");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_minJMETdPhi_ptoverMET_SR->Draw("HIST");
  H_minJMETdPhi_ptoverMET_SR[nfile-1]->Draw("SAME");
  HSig_minJMETdPhi_ptoverMET_SR->Draw("HISTSAME");
  HA_minJMETdPhi_ptoverMET_SR->Draw("E2 SAME");
  HS_minJMETdPhi_ptoverMET_SR->GetYaxis()->SetTitle("Events");
  HS_minJMETdPhi_ptoverMET_SR->GetXaxis()->SetTitle("");
  //HS_minJMETdPhi_ptoverMET_SR->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_minJMETdPhi_ptoverMET_SR->GetXaxis()->SetLabelSize(0);
  HS_minJMETdPhi_ptoverMET_SR->SetMaximum(150);
  HS_minJMETdPhi_ptoverMET_SR->SetMinimum(0);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_minJMETdPhi_ptoverMET_SR[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_minJMETdPhi_ptoverMET_SR, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_minJMETdPhi_ptoverMET_SR[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_minJMETdPhi_ptoverMET_SR->GetName();
  pad2->cd();
  hratio = ratioplot(HS_minJMETdPhi_ptoverMET_SR, H_minJMETdPhi_ptoverMET_SR[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#Delta#phi_{min}(j_{1,2}, MET)");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  ofstream ftext;
  
  ftext.open("ptoverMET_SR_ratio.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  Float_t sig[7] = {0};
  Float_t bkg[7] = {0};
  Float_t sqrtbkg[7] = {0};
  TString histname[7] = {"phoEB_ptcut",
			 "jetpt",
			 "MET",
			 "dphoMETPhi",
			 "dijetMass",
			 "dEta",
			 "minJMETdPhi"
  };

  for(Int_t ihist=0; ihist<7; ihist++){
    sig[ihist] = integral[10][ihist];
    //bkg[ihist] = (integral[11][ihist] - integral[][ihist]);
    //sqrtbkg[ihist] = sqrt(integral[11][ihist] - integral[10][ihist]);

    for(Int_t ifile=0; ifile<nfile-2; ifile++){
      bkg[ihist] += integral[ifile][ihist]; 
    }
    sqrtbkg[ihist] = sqrt(bkg[ihist]);
    
    ftext << ihist << ") " << histname[ihist] << " : sig_yield = " << sig[ihist] << ", bkg_yield = " << bkg[ihist] << ", S/sqrt(B) = " << sig[ihist]/sqrtbkg[ihist] << endl;
  }
  
  ftext.close();
}
