#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "TText.h"
#include "../Drawlumi/CMS_lumi.C"

using namespace std;
#define nfile 9
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

void xDrawStack(Int_t year){

  
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
  //Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 0.01817};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, ZnngVBS]
  //Float_t xsec[20] = {1, 1, 1, 1, 1, 750.5, 0.8145, 0.2064, 0.3008, 0.01817};//[GJet, VJet, monoB, diB, triB, ttjet, Wg+jet, Zg+jet, Zvvg+jet, ZvvgVBS]
  Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 0.01817};//[GJet, VJet, VG, monoB, diB, triB, ttjet, ZvvgVBS]
  
  Float_t filter[20] = {1., 1., 1., 1., 1., 1., 1.};
  Float_t kfactor[20] = {1., 1., 1., 1., 1., 1., 1.};
  
  
  if(year==2016){
    rootname[0] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/job_summer16_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/job_summer16_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/job_summer16_TT_aMCatNLO/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    //rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/job_summer16_Zg_EWK/output_ggtree.root";
    rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/data/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/wk_cms3/judychen/chip02/output_ana/220706/fall17/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/wk_cms3/judychen/chip02/output_ana/220706/fall17/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/wk_cms3/judychen/chip02/output_ana/220706/fall17/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/wk_cms3/judychen/chip02/output_ana/220706/fall17/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/wk_cms3/judychen/chip02/output_ana/220706/fall17/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/wk_cms3/judychen/chip02/output_ana/220706/fall17/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/wk_cms3/judychen/chip02/output_ana/220706/fall17/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/wk_cms3/judychen/chip02/output_ana/220706/fall17/mc/job_fall17_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/wk_cms3/judychen/chip02/output_ana/220706/fall17/mc/job_fall17_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/wk_cms3/judychen/chip02/output_ana/220706/fall17/mc/job_fall17_TT_aMCatNLO/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/wk_cms3/judychen/chip02/output_ana/220706/fall17/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    //rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/fall17/data/output_ggtree.root";
    rootname[11] = "/wk_cms3/judychen/chip02/output_ana/220706/fall17/data/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/VJet/output_merge_VJet.root";
    rootname[2] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/VG/output_merge_VG.root";
    //rootname[2] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/ZJet/output_merge_ZJet.root";
    //rootname[3] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/DYJet/output_merge_DYJet.root";
    rootname[3] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/monoboson/output_merge_monoBoson.root";
    rootname[4] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/diboson/output_merge_diBoson.root";
    rootname[5] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/triboson/output_merge_triBoson.root";
    rootname[6] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/TT/output_merge_TT.root";
    //rootname[6] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/job_UL18_Wg_pt130/output_ggtree.root";
    //rootname[7] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/job_UL18_Zg_pt130/output_ggtree.root";
    //rootname[8] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/job_UL18_Znunug_pt130/output_ggtree.root";
    //rootname[6] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/VG/job_autumn18_Wg_aMCatNLO/output_ggtree.root";
    //rootname[7] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/VG/job_autumn18_Zg_aMCatNLO/output_ggtree.root";
    //rootname[8] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/VG/job_autumn18_Znunug_pt130/output_ggtree.root";
    rootname[7] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/mc/job_UL18_Znunug_VBS/output_ggtree.root";
    rootname[8] = "/wk_cms3/judychen/chip02/output_ana/220706/autumn18/data/output_ggtree.root";
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
  //string hexcolor[ncolor] = {"#fff176", "#03a9f4", "#ff3360", "#b23b8c", "#96ff5a", "#a983d3", "#ff9e00", "#00FFFF", "#FFCCCC", "#3d5afe", "#67ccc1"};
  string hexcolor[ncolor] = {"#fff176", "#03a9f4", "#ff9e00", "#b23b8c", "#a983d3", "#96ff5a", "#FFCCCC", "#67ccc1"};
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
  
  TH1F *H_phoEB_ptcut[nfile][nhisto];
  TH1F *H_phoEB_Etacut[nfile][nhisto];
  TH1F *H_phoEB_Phicut[nfile][nhisto];
  TH1F *H_MIP_cut[nfile][nhisto];
  TH1F *H_MET_cut[nfile][nhisto];
  TH1F *H_METPhi_cut[nfile][nhisto];
  TH1F *H_dphoMETPhi_cut[nfile][nhisto];
  TH1F *H_nvtx_cut[nfile][nhisto];
  TH1F *H_njet_cut[nfile][nhisto];
  
  TH1F *H_jetpt_cut[nfile][nhisto];
  TH1F *H_jetEta_cut[nfile][nhisto];
  TH1F *H_jetPhi_cut[nfile][nhisto];
  TH1F *H_djetMETPhi_cut[nfile];
  TH1F *H_dr_phojet[nfile];
  TH1F *H_dEta_phojet[nfile];
  TH1F *H_dPhi_phojet[nfile];

  TH1F *H_minJMETdPhi_cut[nfile];
  TH1F *H_dr_jetjet_cut[nfile];
  TH1F *H_dEta_jetjet_cut[nfile];
  TH1F *H_dPhi_jetjet_cut[nfile];
  TH1F *H_dijetMass_cut[nfile];
  TH1F *H_ptoverMET_cut[nfile];
  TH1F *H_phoCentral_cut[nfile];
  TH1F *H_jetjetZdPhi_cut[nfile];
  TH1F *H_jetjetphodPhi_cut[nfile];
    
  // Add mc histos
  TH1F *HA_phoEB_ptcut[nhisto];		 
  TH1F *HA_phoEB_Etacut[nhisto];		 
  TH1F *HA_phoEB_Phicut[nhisto];		 
  TH1F *HA_MIP_cut[nhisto];		 
  TH1F *HA_MET_cut[nhisto];		 
  TH1F *HA_METPhi_cut[nhisto];		 
  TH1F *HA_dphoMETPhi_cut[nhisto];	 
  TH1F *HA_nvtx_cut[nhisto];		 
  TH1F *HA_njet_cut[nhisto];		 

  TH1F *HA_jetpt_cut[nhisto];
  TH1F *HA_jetEta_cut[nhisto];
  TH1F *HA_jetPhi_cut[nhisto];
  TH1F *HA_djetMETPhi_cut;
  TH1F *HA_dr_phojet;
  TH1F *HA_dEta_phojet;
  TH1F *HA_dPhi_phojet;

  TH1F *HA_minJMETdPhi_cut;
  TH1F *HA_dr_jetjet_cut;
  TH1F *HA_dEta_jetjet_cut;
  TH1F *HA_dPhi_jetjet_cut;
  TH1F *HA_dijetMass_cut;
  TH1F *HA_ptoverMET_cut;	 
  TH1F *HA_phoCentral_cut;
  TH1F *HA_jetjetZdPhi_cut;
  TH1F *HA_jetjetphodPhi_cut;

  // signal highlight
  TH1F *HSig_phoEB_ptcut[nhisto];
  TH1F *HSig_phoEB_Etacut[nhisto];
  TH1F *HSig_phoEB_Phicut[nhisto];
  TH1F *HSig_MIP_cut[nhisto];
  TH1F *HSig_MET_cut[nhisto];
  TH1F *HSig_METPhi_cut[nhisto];
  TH1F *HSig_dphoMETPhi_cut[nhisto];
  TH1F *HSig_nvtx_cut[nhisto];
  TH1F *HSig_njet_cut[nhisto];
  
  TH1F *HSig_jetpt_cut[nhisto];
  TH1F *HSig_jetEta_cut[nhisto];
  TH1F *HSig_jetPhi_cut[nhisto];
  TH1F *HSig_djetMETPhi_cut;
  TH1F *HSig_dr_phojet;
  TH1F *HSig_dEta_phojet;
  TH1F *HSig_dPhi_phojet;

  TH1F *HSig_minJMETdPhi_cut;
  TH1F *HSig_dr_jetjet_cut;
  TH1F *HSig_dEta_jetjet_cut;
  TH1F *HSig_dPhi_jetjet_cut;
  TH1F *HSig_dijetMass_cut;
  TH1F *HSig_ptoverMET_cut;
  TH1F *HSig_phoCentral_cut;
  TH1F *HSig_jetjetZdPhi_cut;
  TH1F *HSig_jetjetphodPhi_cut;
  
  const char *title;
  const char *saveto = ".";
  
  for(Int_t i=0; i<nfile; i++){
    //if(i==10) continue;//pass Zvv signal
    fopen = new TFile(rootname[i]);
    if(i==nfile-1) scale[i] = 1.0;
    
    if(i==0){
      
      HA_minJMETdPhi_cut = (TH1F*)fopen->Get("dijet/h_minJMETdPhi_cut")->Clone();
      HA_dr_jetjet_cut = (TH1F*)fopen->Get("dijet/h_dr_jetjet_cut")->Clone();
      HA_dEta_jetjet_cut = (TH1F*)fopen->Get("dijet/h_dEta_jetjet_cut")->Clone();
      HA_dPhi_jetjet_cut = (TH1F*)fopen->Get("dijet/h_dPhi_jetjet_cut")->Clone();
      HA_dijetMass_cut = (TH1F*)fopen->Get("dijet/h_dijetMass_cut")->Clone();
      HA_ptoverMET_cut	= (TH1F*)fopen->Get("dijet/h_ptoverMET_cut")->Clone();
      HA_phoCentral_cut = (TH1F*)fopen->Get("dijet/h_phoCentral_cut")->Clone();
      HA_jetjetZdPhi_cut = (TH1F*)fopen->Get("dijet/h_jetjetZdPhi_cut")->Clone();
      HA_jetjetphodPhi_cut = (TH1F*)fopen->Get("dijet/h_jetjetphodPhi_cut")->Clone();

      ///
      HA_minJMETdPhi_cut->Scale(scale[i]);
      HA_dr_jetjet_cut->Scale(scale[i]);
      HA_dEta_jetjet_cut->Scale(scale[i]);
      HA_dPhi_jetjet_cut->Scale(scale[i]);
      HA_dijetMass_cut->Scale(scale[i]);
      HA_ptoverMET_cut->Scale(scale[i]);
      HA_phoCentral_cut->Scale(scale[i]);
      HA_jetjetZdPhi_cut->Scale(scale[i]);
      HA_jetjetphodPhi_cut->Scale(scale[i]);

      ///
      HA_minJMETdPhi_cut->SetFillStyle(3145);
      HA_dr_jetjet_cut->SetFillStyle(3145);
      HA_dEta_jetjet_cut->SetFillStyle(3145);
      HA_dPhi_jetjet_cut->SetFillStyle(3145);
      HA_dijetMass_cut->SetFillStyle(3145);
      HA_ptoverMET_cut->SetFillStyle(3145);
      HA_phoCentral_cut->SetFillStyle(3145);
      HA_jetjetZdPhi_cut->SetFillStyle(3145);
      HA_jetjetphodPhi_cut->SetFillStyle(3145);

      ///
      HA_minJMETdPhi_cut->SetFillColor(12);
      HA_dr_jetjet_cut->SetFillColor(12);
      HA_dEta_jetjet_cut->SetFillColor(12);
      HA_dPhi_jetjet_cut->SetFillColor(12);
      HA_dijetMass_cut->SetFillColor(12);
      HA_ptoverMET_cut->SetFillColor(12);
      HA_phoCentral_cut->SetFillColor(12);
      HA_jetjetZdPhi_cut->SetFillColor(12);
      HA_jetjetphodPhi_cut->SetFillColor(12);
      

      ///
      HA_minJMETdPhi_cut->SetMarkerSize(0.1);
      HA_dr_jetjet_cut->SetMarkerSize(0.1);
      HA_dEta_jetjet_cut->SetMarkerSize(0.1);
      HA_dPhi_jetjet_cut->SetMarkerSize(0.1);
      HA_dijetMass_cut->SetMarkerSize(0.1);
      HA_ptoverMET_cut->SetMarkerSize(0.1);
      HA_phoCentral_cut->SetMarkerSize(0.1);
      HA_jetjetZdPhi_cut->SetMarkerSize(0.1);
      HA_jetjetphodPhi_cut->SetMarkerSize(0.1);

    }

    if(i==nfile-2){

      HSig_minJMETdPhi_cut = (TH1F*)fopen->Get("dijet/h_minJMETdPhi_cut")->Clone();
      HSig_dr_jetjet_cut = (TH1F*)fopen->Get("dijet/h_dr_jetjet_cut")->Clone();
      HSig_dEta_jetjet_cut = (TH1F*)fopen->Get("dijet/h_dEta_jetjet_cut")->Clone();
      HSig_dPhi_jetjet_cut = (TH1F*)fopen->Get("dijet/h_dPhi_jetjet_cut")->Clone();
      HSig_dijetMass_cut = (TH1F*)fopen->Get("dijet/h_dijetMass_cut")->Clone();
      HSig_ptoverMET_cut = (TH1F*)fopen->Get("dijet/h_ptoverMET_cut")->Clone();
      HSig_phoCentral_cut = (TH1F*)fopen->Get("dijet/h_phoCentral_cut")->Clone();
      HSig_jetjetZdPhi_cut = (TH1F*)fopen->Get("dijet/h_jetjetZdPhi_cut")->Clone();
      HSig_jetjetphodPhi_cut = (TH1F*)fopen->Get("dijet/h_jetjetphodPhi_cut")->Clone();

      ///
      HSig_minJMETdPhi_cut->Scale(scale[i]);
      HSig_dr_jetjet_cut->Scale(scale[i]);
      HSig_dEta_jetjet_cut->Scale(scale[i]);
      HSig_dPhi_jetjet_cut->Scale(scale[i]);
      HSig_dijetMass_cut->Scale(scale[i]);
      HSig_ptoverMET_cut->Scale(scale[i]);
      HSig_phoCentral_cut->Scale(scale[i]);
      HSig_jetjetZdPhi_cut->Scale(scale[i]);
      HSig_jetjetphodPhi_cut->Scale(scale[i]);

      ///
      HSig_minJMETdPhi_cut->SetLineColor(kRed);
      HSig_dr_jetjet_cut->SetLineColor(kRed);
      HSig_dEta_jetjet_cut->SetLineColor(kRed);
      HSig_dPhi_jetjet_cut->SetLineColor(kRed);
      HSig_dijetMass_cut->SetLineColor(kRed);
      HSig_ptoverMET_cut->SetLineColor(kRed);
      HSig_phoCentral_cut->SetLineColor(kRed);
      HSig_jetjetZdPhi_cut->SetLineColor(kRed);
      HSig_jetjetphodPhi_cut->SetLineColor(kRed);

      ///
      HSig_minJMETdPhi_cut->SetLineWidth(2);
      HSig_dr_jetjet_cut->SetLineWidth(2);
      HSig_dEta_jetjet_cut->SetLineWidth(2);
      HSig_dPhi_jetjet_cut->SetLineWidth(2);
      HSig_dijetMass_cut->SetLineWidth(2);
      HSig_ptoverMET_cut->SetLineWidth(2);
      HSig_phoCentral_cut->SetLineWidth(2);
      HSig_jetjetZdPhi_cut->SetLineWidth(2);
      HSig_jetjetphodPhi_cut->SetLineWidth(2);

    }
    
    H_minJMETdPhi_cut[i] = (TH1F*)fopen->Get("dijet/h_minJMETdPhi_cut");
    H_dr_jetjet_cut[i] = (TH1F*)fopen->Get("dijet/h_dr_jetjet_cut");
    H_dEta_jetjet_cut[i] = (TH1F*)fopen->Get("dijet/h_dEta_jetjet_cut");
    H_dPhi_jetjet_cut[i] = (TH1F*)fopen->Get("dijet/h_dPhi_jetjet_cut");
    H_dijetMass_cut[i] = (TH1F*)fopen->Get("dijet/h_dijetMass_cut");
    H_ptoverMET_cut[i]	= (TH1F*)fopen->Get("dijet/h_ptoverMET_cut");
    H_phoCentral_cut[i] = (TH1F*)fopen->Get("dijet/h_phoCentral_cut");
    H_jetjetZdPhi_cut[i] = (TH1F*)fopen->Get("dijet/h_jetjetZdPhi_cut");
    H_jetjetphodPhi_cut[i] = (TH1F*)fopen->Get("dijet/h_jetjetphodPhi_cut");

    H_minJMETdPhi_cut[i]->Scale(scale[i]);
    H_dr_jetjet_cut[i]->Scale(scale[i]);
    H_dEta_jetjet_cut[i]->Scale(scale[i]);
    H_dPhi_jetjet_cut[i]->Scale(scale[i]);
    H_dijetMass_cut[i]->Scale(scale[i]);
    H_ptoverMET_cut[i]->Scale(scale[i]);
    H_phoCentral_cut[i]->Scale(scale[i]);
    H_jetjetZdPhi_cut[i]->Scale(scale[i]);
    H_jetjetphodPhi_cut[i]->Scale(scale[i]);

    if(i<nfile-1){
      
      H_minJMETdPhi_cut[i]->SetFillColor(cnum[i]);
      H_dr_jetjet_cut[i]->SetFillColor(cnum[i]);
      H_dEta_jetjet_cut[i]->SetFillColor(cnum[i]);
      H_dPhi_jetjet_cut[i]->SetFillColor(cnum[i]);
      H_dijetMass_cut[i]->SetFillColor(cnum[i]);
      H_ptoverMET_cut[i]->SetFillColor(cnum[i]);
      H_phoCentral_cut[i]->SetFillColor(cnum[i]);
      H_jetjetZdPhi_cut[i]->SetFillColor(cnum[i]);
      H_jetjetphodPhi_cut[i]->SetFillColor(cnum[i]);

    }

    ///
    H_minJMETdPhi_cut[i]->SetLineColor(1);
    H_dr_jetjet_cut[i]->SetLineColor(1);
    H_dEta_jetjet_cut[i]->SetLineColor(1);
    H_dPhi_jetjet_cut[i]->SetLineColor(1);
    H_dijetMass_cut[i]->SetLineColor(1);
    H_ptoverMET_cut[i]->SetLineColor(1);
    H_phoCentral_cut[i]->SetLineColor(1);
    H_jetjetZdPhi_cut[i]->SetLineColor(1);
    H_jetjetphodPhi_cut[i]->SetLineColor(1);

    ///    
    H_minJMETdPhi_cut[i]->SetLineWidth(1);
    H_dr_jetjet_cut[i]->SetLineWidth(1);
    H_dEta_jetjet_cut[i]->SetLineWidth(1);
    H_dPhi_jetjet_cut[i]->SetLineWidth(1);
    H_dijetMass_cut[i]->SetLineWidth(1);
    H_ptoverMET_cut[i]->SetLineWidth(1);
    H_phoCentral_cut[i]->SetLineWidth(1);
    H_jetjetZdPhi_cut[i]->SetLineWidth(1);
    H_jetjetphodPhi_cut[i]->SetLineWidth(1);

    if(i == nfile-1){
      ///
      H_minJMETdPhi_cut[i]->SetLineWidth(2);
      H_dr_jetjet_cut[i]->SetLineWidth(2);
      H_dEta_jetjet_cut[i]->SetLineWidth(2);
      H_dPhi_jetjet_cut[i]->SetLineWidth(2);
      H_dijetMass_cut[i]->SetLineWidth(2);
      H_ptoverMET_cut[i]->SetLineWidth(2);
      H_phoCentral_cut[i]->SetLineWidth(2);
      H_jetjetZdPhi_cut[i]->SetLineWidth(2);
      H_jetjetphodPhi_cut[i]->SetLineWidth(2);

      ///
      H_minJMETdPhi_cut[i]->SetMarkerStyle(8);
      H_dr_jetjet_cut[i]->SetMarkerStyle(8);
      H_dEta_jetjet_cut[i]->SetMarkerStyle(8);
      H_dPhi_jetjet_cut[i]->SetMarkerStyle(8);
      H_dijetMass_cut[i]->SetMarkerStyle(8);
      H_ptoverMET_cut[i]->SetMarkerStyle(8);
      H_phoCentral_cut[i]->SetMarkerStyle(8);
      H_jetjetZdPhi_cut[i]->SetMarkerStyle(8);
      H_jetjetphodPhi_cut[i]->SetMarkerStyle(8);

      ///
      H_minJMETdPhi_cut[i]->SetMarkerSize(0.9);
      H_dr_jetjet_cut[i]->SetMarkerSize(0.9);
      H_dEta_jetjet_cut[i]->SetMarkerSize(0.9);
      H_dPhi_jetjet_cut[i]->SetMarkerSize(0.9);
      H_dijetMass_cut[i]->SetMarkerSize(0.9);
      H_ptoverMET_cut[i]->SetMarkerSize(0.9);
      H_phoCentral_cut[i]->SetMarkerSize(0.9);
      H_jetjetZdPhi_cut[i]->SetMarkerSize(0.9);
      H_jetjetphodPhi_cut[i]->SetMarkerSize(0.9);

    }
    
    for(Int_t jj=0; jj<nhisto; jj++){
      H_phoEB_ptcut[i][jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", jj));
      H_phoEB_Etacut[i][jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Etacut_%i", jj));
      H_phoEB_Phicut[i][jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Phicut_%i", jj));
      H_MET_cut[i][jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", jj));
      //H_MET_cut[i][jj]->Rebin(2);
      H_METPhi_cut[i][jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_cut_%i", jj));
      H_MIP_cut[i][jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_MIP_cut_%i", jj));
      H_dphoMETPhi_cut[i][jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", jj));
      H_nvtx_cut[i][jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", jj));
      H_njet_cut[i][jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", jj));
      H_nvtx_cut[i][jj]->Rebin(10);
      
      if(i==0){
	HA_phoEB_ptcut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", jj))->Clone();
	HA_phoEB_Etacut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Etacut_%i", jj))->Clone();
	HA_phoEB_Phicut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Phicut_%i", jj))->Clone();
	HA_MET_cut[jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", jj))->Clone();
	HA_METPhi_cut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_cut_%i", jj))->Clone();
	HA_MIP_cut[jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_MIP_cut_%i", jj))->Clone();
	HA_dphoMETPhi_cut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", jj))->Clone();
	HA_nvtx_cut[jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", jj))->Clone();
	HA_njet_cut[jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", jj))->Clone();
		
	HA_phoEB_ptcut[jj]->Scale(scale[i]);	
	HA_phoEB_Etacut[jj]->Scale(scale[i]);	
	HA_phoEB_Phicut[jj]->Scale(scale[i]);	
	HA_MET_cut[jj]->Scale(scale[i]);		
	HA_METPhi_cut[jj]->Scale(scale[i]);	
	HA_MIP_cut[jj]->Scale(scale[i]);		
	HA_dphoMETPhi_cut[jj]->Scale(scale[i]);	
	HA_nvtx_cut[jj]->Scale(scale[i]);		
	HA_njet_cut[jj]->Scale(scale[i]);

	HA_phoEB_ptcut[jj]->SetFillStyle(3145);	
	HA_phoEB_Etacut[jj]->SetFillStyle(3145);	
	HA_phoEB_Phicut[jj]->SetFillStyle(3145);	
	HA_MET_cut[jj]->SetFillStyle(3145);		
	HA_METPhi_cut[jj]->SetFillStyle(3145);	
	HA_MIP_cut[jj]->SetFillStyle(3145);		
	HA_dphoMETPhi_cut[jj]->SetFillStyle(3145);	
	HA_nvtx_cut[jj]->SetFillStyle(3145);		
	HA_njet_cut[jj]->SetFillStyle(3145);

	HA_phoEB_ptcut[jj]->SetFillColor(12);	
	HA_phoEB_Etacut[jj]->SetFillColor(12);	
	HA_phoEB_Phicut[jj]->SetFillColor(12);	
	HA_MET_cut[jj]->SetFillColor(12);		
	HA_METPhi_cut[jj]->SetFillColor(12);	
	HA_MIP_cut[jj]->SetFillColor(12);		
	HA_dphoMETPhi_cut[jj]->SetFillColor(12);	
	HA_nvtx_cut[jj]->SetFillColor(12);		
	HA_njet_cut[jj]->SetFillColor(12);
	
	HA_phoEB_ptcut[jj]->SetMarkerSize(0.1);	
	HA_phoEB_Etacut[jj]->SetMarkerSize(0.1);	
	HA_phoEB_Phicut[jj]->SetMarkerSize(0.1);	
	HA_MET_cut[jj]->SetMarkerSize(0.1);		
	HA_METPhi_cut[jj]->SetMarkerSize(0.1);	
	HA_MIP_cut[jj]->SetMarkerSize(0.1);		
	HA_dphoMETPhi_cut[jj]->SetMarkerSize(0.1);	
	HA_nvtx_cut[jj]->SetMarkerSize(0.1);		
	HA_njet_cut[jj]->SetMarkerSize(0.1);
      }

      if(i==nfile-2){
	HSig_phoEB_ptcut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", jj))->Clone();
	HSig_phoEB_Etacut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Etacut_%i", jj))->Clone();
	HSig_phoEB_Phicut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Phicut_%i", jj))->Clone();
	HSig_MET_cut[jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", jj))->Clone();
	HSig_METPhi_cut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_cut_%i", jj))->Clone();
	HSig_MIP_cut[jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_MIP_cut_%i", jj))->Clone();
	HSig_dphoMETPhi_cut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", jj))->Clone();
	HSig_nvtx_cut[jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", jj))->Clone();
	HSig_njet_cut[jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", jj))->Clone();
	//HA_nvtx_cut[jj]->Rebin(4);
	
	HSig_phoEB_ptcut[jj]->Scale(scale[i]);	
	HSig_phoEB_Etacut[jj]->Scale(scale[i]);	
	HSig_phoEB_Phicut[jj]->Scale(scale[i]);	
	HSig_MET_cut[jj]->Scale(scale[i]);		
	HSig_METPhi_cut[jj]->Scale(scale[i]);	
	HSig_MIP_cut[jj]->Scale(scale[i]);		
	HSig_dphoMETPhi_cut[jj]->Scale(scale[i]);	
	HSig_nvtx_cut[jj]->Scale(scale[i]);		
	HSig_njet_cut[jj]->Scale(scale[i]);

	HSig_phoEB_ptcut[jj]->SetLineColor(kRed);	
	HSig_phoEB_Etacut[jj]->SetLineColor(kRed);	
	HSig_phoEB_Phicut[jj]->SetLineColor(kRed);	
	HSig_MET_cut[jj]->SetLineColor(kRed);		
	HSig_METPhi_cut[jj]->SetLineColor(kRed);	
	HSig_MIP_cut[jj]->SetLineColor(kRed);		
	HSig_dphoMETPhi_cut[jj]->SetLineColor(kRed);	
	HSig_nvtx_cut[jj]->SetLineColor(kRed);		
	HSig_njet_cut[jj]->SetLineColor(kRed);

	HSig_phoEB_ptcut[jj]->SetLineWidth(2);	
	HSig_phoEB_Etacut[jj]->SetLineWidth(2);	
	HSig_phoEB_Phicut[jj]->SetLineWidth(2);	
	HSig_MET_cut[jj]->SetLineWidth(2);		
	HSig_METPhi_cut[jj]->SetLineWidth(2);	
	HSig_MIP_cut[jj]->SetLineWidth(2);		
	HSig_dphoMETPhi_cut[jj]->SetLineWidth(2);	
	HSig_nvtx_cut[jj]->SetLineWidth(2);		
	HSig_njet_cut[jj]->SetLineWidth(2);
      }

      H_phoEB_ptcut[i][jj]->Scale(scale[i]);	
      H_phoEB_Etacut[i][jj]->Scale(scale[i]);	
      H_phoEB_Phicut[i][jj]->Scale(scale[i]);	
      H_MET_cut[i][jj]->Scale(scale[i]);		
      H_METPhi_cut[i][jj]->Scale(scale[i]);	
      H_MIP_cut[i][jj]->Scale(scale[i]);		
      H_dphoMETPhi_cut[i][jj]->Scale(scale[i]);	
      H_nvtx_cut[i][jj]->Scale(scale[i]);		
      H_njet_cut[i][jj]->Scale(scale[i]);

      if(i<nfile-1){
	H_phoEB_ptcut[i][jj]->SetFillColor(cnum[i]);	
	H_phoEB_Etacut[i][jj]->SetFillColor(cnum[i]);	
	H_phoEB_Phicut[i][jj]->SetFillColor(cnum[i]);	
	H_MET_cut[i][jj]->SetFillColor(cnum[i]);		
	H_METPhi_cut[i][jj]->SetFillColor(cnum[i]);	
	H_MIP_cut[i][jj]->SetFillColor(cnum[i]);		
	H_dphoMETPhi_cut[i][jj]->SetFillColor(cnum[i]);	
	H_nvtx_cut[i][jj]->SetFillColor(cnum[i]);		
	H_njet_cut[i][jj]->SetFillColor(cnum[i]);
      }

      H_phoEB_ptcut[i][jj]->SetLineColor(1);	
      H_phoEB_Etacut[i][jj]->SetLineColor(1);	
      H_phoEB_Phicut[i][jj]->SetLineColor(1);	
      H_MET_cut[i][jj]->SetLineColor(1);		
      H_METPhi_cut[i][jj]->SetLineColor(1);	
      H_MIP_cut[i][jj]->SetLineColor(1);		
      H_dphoMETPhi_cut[i][jj]->SetLineColor(1);	
      H_nvtx_cut[i][jj]->SetLineColor(1);		
      H_njet_cut[i][jj]->SetLineColor(1);

      H_phoEB_ptcut[i][jj]->SetLineWidth(1);	
      H_phoEB_Etacut[i][jj]->SetLineWidth(1);	
      H_phoEB_Phicut[i][jj]->SetLineWidth(1);	
      H_MET_cut[i][jj]->SetLineWidth(1);		
      H_METPhi_cut[i][jj]->SetLineWidth(1);	
      H_MIP_cut[i][jj]->SetLineWidth(1);		
      H_dphoMETPhi_cut[i][jj]->SetLineWidth(1);	
      H_nvtx_cut[i][jj]->SetLineWidth(1);		
      H_njet_cut[i][jj]->SetLineWidth(1);

      if(i == nfile-1){
	H_phoEB_ptcut[i][jj]->SetLineWidth(2);	
	H_phoEB_Etacut[i][jj]->SetLineWidth(2);	
	H_phoEB_Phicut[i][jj]->SetLineWidth(2);	
	H_MET_cut[i][jj]->SetLineWidth(2);		
	H_METPhi_cut[i][jj]->SetLineWidth(2);	
	H_MIP_cut[i][jj]->SetLineWidth(2);		
	H_dphoMETPhi_cut[i][jj]->SetLineWidth(2);	
	H_nvtx_cut[i][jj]->SetLineWidth(2);		
	H_njet_cut[i][jj]->SetLineWidth(2);

	H_phoEB_ptcut[i][jj]->SetMarkerStyle(8);	
	H_phoEB_Etacut[i][jj]->SetMarkerStyle(8);	
	H_phoEB_Phicut[i][jj]->SetMarkerStyle(8);	
	H_MET_cut[i][jj]->SetMarkerStyle(8);		
	H_METPhi_cut[i][jj]->SetMarkerStyle(8);	
	H_MIP_cut[i][jj]->SetMarkerStyle(8);		
	H_dphoMETPhi_cut[i][jj]->SetMarkerStyle(8);	
	H_nvtx_cut[i][jj]->SetMarkerStyle(8);		
	H_njet_cut[i][jj]->SetMarkerStyle(8);

	H_phoEB_ptcut[i][jj]->SetMarkerSize(0.9);	
	H_phoEB_Etacut[i][jj]->SetMarkerSize(0.9);	
	H_phoEB_Phicut[i][jj]->SetMarkerSize(0.9);	
	H_MET_cut[i][jj]->SetMarkerSize(0.9);		
	H_METPhi_cut[i][jj]->SetMarkerSize(0.9);	
	H_MIP_cut[i][jj]->SetMarkerSize(0.9);		
	H_dphoMETPhi_cut[i][jj]->SetMarkerSize(0.9);	
	H_nvtx_cut[i][jj]->SetMarkerSize(0.9);		
	H_njet_cut[i][jj]->SetMarkerSize(0.9);
      }
      

      H_jetpt_cut[i][jj]		= (TH1F*)fopen->Get(Form("dijet/h_jetpt_cut_%i", jj));
      H_jetEta_cut[i][jj]		= (TH1F*)fopen->Get(Form("dijet/h_jetEta_cut_%i", jj));
      H_jetPhi_cut[i][jj]		= (TH1F*)fopen->Get(Form("dijet/h_jetPhi_cut_%i", jj));
	
      if(i==0){
	HA_jetpt_cut[jj]		= (TH1F*)fopen->Get(Form("dijet/h_jetpt_cut_%i", jj))->Clone();
	HA_jetEta_cut[jj]		= (TH1F*)fopen->Get(Form("dijet/h_jetEta_cut_%i", jj))->Clone();
	HA_jetPhi_cut[jj]		= (TH1F*)fopen->Get(Form("dijet/h_jetPhi_cut_%i", jj))->Clone();
	  

	HA_jetpt_cut[jj]->Scale(scale[i]);		
	HA_jetEta_cut[jj]->Scale(scale[i]);		
	HA_jetPhi_cut[jj]->Scale(scale[i]);		
	  
	HA_jetpt_cut[jj]->SetFillStyle(3145);		
	HA_jetEta_cut[jj]->SetFillStyle(3145);		
	HA_jetPhi_cut[jj]->SetFillStyle(3145);		
	  
	HA_jetpt_cut[jj]->SetFillColor(12);		
	HA_jetEta_cut[jj]->SetFillColor(12);		
	HA_jetPhi_cut[jj]->SetFillColor(12);		

	HA_jetpt_cut[jj]->SetMarkerSize(0.1);		
	HA_jetEta_cut[jj]->SetMarkerSize(0.1);		
	HA_jetPhi_cut[jj]->SetMarkerSize(0.1);		
      }

      if(i==nfile-2){
	HSig_jetpt_cut[jj]		= (TH1F*)fopen->Get(Form("dijet/h_jetpt_cut_%i", jj))->Clone();
	HSig_jetEta_cut[jj]		= (TH1F*)fopen->Get(Form("dijet/h_jetEta_cut_%i", jj))->Clone();
	HSig_jetPhi_cut[jj]		= (TH1F*)fopen->Get(Form("dijet/h_jetPhi_cut_%i", jj))->Clone();
	  
	HSig_jetpt_cut[jj]->Scale(scale[i]);		
	HSig_jetEta_cut[jj]->Scale(scale[i]);		
	HSig_jetPhi_cut[jj]->Scale(scale[i]);		
	  
	HSig_jetpt_cut[jj]->SetLineColor(kRed);		
	HSig_jetEta_cut[jj]->SetLineColor(kRed);		
	HSig_jetPhi_cut[jj]->SetLineColor(kRed);		
	  
	HSig_jetpt_cut[jj]->SetLineWidth(2);		
	HSig_jetEta_cut[jj]->SetLineWidth(2);		
	HSig_jetPhi_cut[jj]->SetLineWidth(2);		
	  	  
      }
	
      H_jetpt_cut[i][jj]->Scale(scale[i]);		
      H_jetEta_cut[i][jj]->Scale(scale[i]);		
      H_jetPhi_cut[i][jj]->Scale(scale[i]);		
	
      if(i<nfile-1){
	H_jetpt_cut[i][jj]->SetFillColor(cnum[i]);		
	H_jetEta_cut[i][jj]->SetFillColor(cnum[i]);		
	H_jetPhi_cut[i][jj]->SetFillColor(cnum[i]);		
      }

      H_jetpt_cut[i][jj]->SetLineColor(1);		
      H_jetEta_cut[i][jj]->SetLineColor(1);		
      H_jetPhi_cut[i][jj]->SetLineColor(1);		
	
      H_jetpt_cut[i][jj]->SetLineWidth(1);		
      H_jetEta_cut[i][jj]->SetLineWidth(1);		
      H_jetPhi_cut[i][jj]->SetLineWidth(1);		
	
      if(i == nfile-1){
	H_jetpt_cut[i][jj]->SetLineWidth(2);		
	H_jetEta_cut[i][jj]->SetLineWidth(2);		
	H_jetPhi_cut[i][jj]->SetLineWidth(2);		
	  
	H_jetpt_cut[i][jj]->SetMarkerStyle(8);		
	H_jetEta_cut[i][jj]->SetMarkerStyle(8);		
	H_jetPhi_cut[i][jj]->SetMarkerStyle(8);		

	H_jetpt_cut[i][jj]->SetMarkerSize(0.9);		
	H_jetEta_cut[i][jj]->SetMarkerSize(0.9);		
	H_jetPhi_cut[i][jj]->SetMarkerSize(0.9);		

      }
        
    }

    H_djetMETPhi_cut[i]	= (TH1F*)fopen->Get("dijet/h_djetMETPhi_cut");
    H_dr_phojet[i]		= (TH1F*)fopen->Get("dijet/h_dr_phojet");
    H_dEta_phojet[i]	= (TH1F*)fopen->Get("dijet/h_dEta_phojet");
    H_dPhi_phojet[i]	= (TH1F*)fopen->Get("dijet/h_dPhi_phojet");

    if(i==0){

      HA_djetMETPhi_cut	= (TH1F*)fopen->Get("dijet/h_djetMETPhi_cut")->Clone();
      HA_dr_phojet		= (TH1F*)fopen->Get("dijet/h_dr_phojet")->Clone();
      HA_dEta_phojet	= (TH1F*)fopen->Get("dijet/h_dEta_phojet")->Clone();
      HA_dPhi_phojet	= (TH1F*)fopen->Get("dijet/h_dPhi_phojet")->Clone();

      HA_djetMETPhi_cut->Scale(scale[i]);	
      HA_dr_phojet->Scale(scale[i]);		
      HA_dEta_phojet->Scale(scale[i]);	
      HA_dPhi_phojet->Scale(scale[i]);

      HA_djetMETPhi_cut->SetFillStyle(3145);	
      HA_dr_phojet->SetFillStyle(3145);		
      HA_dEta_phojet->SetFillStyle(3145);	
      HA_dPhi_phojet->SetFillStyle(3145);

      HA_djetMETPhi_cut->SetFillColor(12);	
      HA_dr_phojet->SetFillColor(12);		
      HA_dEta_phojet->SetFillColor(12);	
      HA_dPhi_phojet->SetFillColor(12);

      HA_djetMETPhi_cut->SetMarkerSize(0.1);	
      HA_dr_phojet->SetMarkerSize(0.1);		
      HA_dEta_phojet->SetMarkerSize(0.1);	
      HA_dPhi_phojet->SetMarkerSize(0.1);

    }

    if(i==nfile-2){

      HSig_djetMETPhi_cut	= (TH1F*)fopen->Get("dijet/h_djetMETPhi_cut")->Clone();
      HSig_dr_phojet		= (TH1F*)fopen->Get("dijet/h_dr_phojet")->Clone();
      HSig_dEta_phojet	= (TH1F*)fopen->Get("dijet/h_dEta_phojet")->Clone();
      HSig_dPhi_phojet	= (TH1F*)fopen->Get("dijet/h_dPhi_phojet")->Clone();

      HSig_djetMETPhi_cut->Scale(scale[i]);	
      HSig_dr_phojet->Scale(scale[i]);		
      HSig_dEta_phojet->Scale(scale[i]);	
      HSig_dPhi_phojet->Scale(scale[i]);

      HSig_djetMETPhi_cut->SetLineColor(kRed);	
      HSig_dr_phojet->SetLineColor(kRed);		
      HSig_dEta_phojet->SetLineColor(kRed);	
      HSig_dPhi_phojet->SetLineColor(kRed);
	  
      HSig_djetMETPhi_cut->SetLineWidth(2);	
      HSig_dr_phojet->SetLineWidth(2);		
      HSig_dEta_phojet->SetLineWidth(2);	
      HSig_dPhi_phojet->SetLineWidth(2);

    }

    if(i == nfile-1){

      H_djetMETPhi_cut[i]->SetLineWidth(2);	
      H_dr_phojet[i]->SetLineWidth(2);		
      H_dEta_phojet[i]->SetLineWidth(2);	
      H_dPhi_phojet[i]->SetLineWidth(2);

      H_djetMETPhi_cut[i]->SetMarkerStyle(8);	
      H_dr_phojet[i]->SetMarkerStyle(8);		
      H_dEta_phojet[i]->SetMarkerStyle(8);	
      H_dPhi_phojet[i]->SetMarkerStyle(8);

      H_djetMETPhi_cut[i]->SetMarkerSize(0.9);	
      H_dr_phojet[i]->SetMarkerSize(0.9);		
      H_dEta_phojet[i]->SetMarkerSize(0.9);	
      H_dPhi_phojet[i]->SetMarkerSize(0.9);

    }

    H_djetMETPhi_cut[i]->Scale(scale[i]);	
    H_dr_phojet[i]->Scale(scale[i]);		
    H_dEta_phojet[i]->Scale(scale[i]);	
    H_dPhi_phojet[i]->Scale(scale[i]);

    if(i<nfile-1){
      H_djetMETPhi_cut[i]->SetFillColor(cnum[i]);	
      H_dr_phojet[i]->SetFillColor(cnum[i]);		
      H_dEta_phojet[i]->SetFillColor(cnum[i]);	
      H_dPhi_phojet[i]->SetFillColor(cnum[i]);
    }

    H_djetMETPhi_cut[i]->SetLineColor(1);	
    H_dr_phojet[i]->SetLineColor(1);		
    H_dEta_phojet[i]->SetLineColor(1);	
    H_dPhi_phojet[i]->SetLineColor(1);

    H_djetMETPhi_cut[i]->SetLineWidth(1);	
    H_dr_phojet[i]->SetLineWidth(1);		
    H_dEta_phojet[i]->SetLineWidth(1);	
    H_dPhi_phojet[i]->SetLineWidth(1);
    
  }

  for(Int_t i=0; i<nfile-1; i++){
    //if(i==10) continue;
    if(i>0){
      
      HA_minJMETdPhi_cut->Add(H_minJMETdPhi_cut[i]);
      HA_dr_jetjet_cut->Add(H_dr_jetjet_cut[i]);
      HA_dEta_jetjet_cut->Add(H_dEta_jetjet_cut[i]);
      HA_dPhi_jetjet_cut->Add(H_dPhi_jetjet_cut[i]);
      HA_dijetMass_cut->Add(H_dijetMass_cut[i]);
      HA_ptoverMET_cut->Add(H_ptoverMET_cut[i]);
      HA_phoCentral_cut->Add(H_phoCentral_cut[i]);
      HA_jetjetZdPhi_cut->Add(H_jetjetZdPhi_cut[i]);
      HA_jetjetphodPhi_cut->Add(H_jetjetphodPhi_cut[i]);

      for(Int_t jj=0; jj<2; jj++){
	HA_phoEB_ptcut[jj]->Add(H_phoEB_ptcut[i][jj]);
	HA_phoEB_Etacut[jj]->Add(H_phoEB_Etacut[i][jj]);
	HA_phoEB_Phicut[jj]->Add(H_phoEB_Phicut[i][jj]);
	HA_MIP_cut[jj]->Add(H_MIP_cut[i][jj]);
	HA_MET_cut[jj]->Add(H_MET_cut[i][jj]);
	HA_METPhi_cut[jj]->Add(H_METPhi_cut[i][jj]);
	HA_dphoMETPhi_cut[jj]->Add(H_dphoMETPhi_cut[i][jj]);
	HA_nvtx_cut[jj]->Add(H_nvtx_cut[i][jj]);
	HA_njet_cut[jj]->Add(H_njet_cut[i][jj]);
	
	HA_jetpt_cut[jj]->Add(H_jetpt_cut[i][jj]);
	HA_jetEta_cut[jj]->Add(H_jetEta_cut[i][jj]);
	HA_jetPhi_cut[jj]->Add(H_jetPhi_cut[i][jj]);
	
      }

      HA_djetMETPhi_cut->Add(H_djetMETPhi_cut[i]);
      HA_dr_phojet->Add(H_dr_phojet[i]);
      HA_dEta_phojet->Add(H_dEta_phojet[i]);
      HA_dPhi_phojet->Add(H_dPhi_phojet[i]);
    }
  }

  THStack *HS_phoEB_ptcut[nhisto];		 
  THStack *HS_phoEB_Etacut[nhisto];		 
  THStack *HS_phoEB_Phicut[nhisto];		 
  THStack *HS_MIP_cut[nhisto];		 
  THStack *HS_MET_cut[nhisto];		 
  THStack *HS_METPhi_cut[nhisto];		 
  THStack *HS_dphoMETPhi_cut[nhisto];	 
  THStack *HS_nvtx_cut[nhisto];		 
  THStack *HS_njet_cut[nhisto];		 

  THStack *HS_jetpt_cut[nhisto];
  THStack *HS_jetEta_cut[nhisto];
  THStack *HS_jetPhi_cut[nhisto];
  THStack *HS_djetMETPhi_cut;
  THStack *HS_dr_phojet;
  THStack *HS_dEta_phojet;
  THStack *HS_dPhi_phojet;

  THStack *HS_minJMETdPhi_cut;
  THStack *HS_dr_jetjet_cut;
  THStack *HS_dEta_jetjet_cut;
  THStack *HS_dPhi_jetjet_cut;
  THStack *HS_dijetMass_cut;
  THStack *HS_ptoverMET_cut;
  THStack *HS_phoCentral_cut;
  THStack *HS_jetjetZdPhi_cut;
  THStack *HS_jetjetphodPhi_cut;
  
  for(Int_t jj=0; jj<2; jj++){
    HS_phoEB_ptcut[jj]		= new THStack(Form("HS_phoEB_ptcut_%i", jj), "HS_phoEB_ptcut");		 
    HS_phoEB_Etacut[jj]		= new THStack(Form("HS_phoEB_Etacut_%i", jj), "HS_phoEB_Etacut");		 
    HS_phoEB_Phicut[jj]		= new THStack(Form("HS_phoEB_Phicut_%i", jj), "HS_phoEB_Phicut");		 
    HS_MIP_cut[jj]		= new THStack(Form("HS_MIP_cut_%i", jj), "HS_MIP_cut");		 
    HS_MET_cut[jj]		= new THStack(Form("HS_MET_cut_%i", jj), "HS_MET_cut");		 
    HS_METPhi_cut[jj]		= new THStack(Form("HS_METPhi_cut_%i", jj), "HS_METPhi_cut");		 
    HS_dphoMETPhi_cut[jj]	= new THStack(Form("HS_dphoMETPhi_cut_%i", jj), "HS_dphoMETPhi_cut");	 
    HS_nvtx_cut[jj]		= new THStack(Form("HS_nvtx_cut_%i", jj), "HS_nvtx_cut");		 
    HS_njet_cut[jj]		= new THStack(Form("HS_njet_cut_%i", jj), "HS_njet_cut");

    
    HS_jetpt_cut[jj]	= new THStack(Form("HS_jetpt_cut_%i", jj), "HS_jetpt_cut");
    HS_jetEta_cut[jj]	= new THStack(Form("HS_jetEta_cut_%i", jj), "HS_jetEta_cut");
    HS_jetPhi_cut[jj]	= new THStack(Form("HS_jetPhi_cut_%i", jj), "HS_jetPhi_cut");
    
  }
  
  HS_djetMETPhi_cut = new THStack("HS_djetMETPhi_cut", "HS_djetMETPhi_cut");
  HS_dr_phojet	= new THStack("HS_dr_phojet", "HS_dr_phojet");
  HS_dEta_phojet	= new THStack("HS_dEta_phojet", "HS_dEta_phojet");
  HS_dPhi_phojet	= new THStack("HS_dPhi_phojet", "HS_dPhi_phojet");

  HS_minJMETdPhi_cut = new THStack("HS_minJMETdPhi_cut", "HS_minJMETdPhi_cut");
  HS_dr_jetjet_cut = new THStack("HS_dr_jetjet_cut", "HS_dr_jetjet_cut");
  HS_dEta_jetjet_cut = new THStack("HS_dEta_jetjet_cut", "HS_dEta_jetjet_cut");
  HS_dPhi_jetjet_cut = new THStack("HS_dPhi_jetjet_cut", "HS_dPhi_jetjet_cut");
  HS_dijetMass_cut = new THStack("HS_dijetMass_cut", "HS_dijetMass_cut");
  HS_ptoverMET_cut = new THStack("HS_ptoverMET_cut", "HS_ptoverMET_cut");
  HS_phoCentral_cut = new THStack("HS_phoCentral_cut", "HS_phoCentral_cut");
  HS_jetjetZdPhi_cut = new THStack("HS_jetjetZdPhi_cut", "HS_jetjetZdPhi_cut");
  HS_jetjetphodPhi_cut = new THStack("HS_jetjetphodPhi_cut", "HS_jetjetphodPhi_cut");

					       
  
  for(Int_t i=0; i<nfile-1; i++){
    //if(i==10) continue;

    HS_minJMETdPhi_cut->Add(H_minJMETdPhi_cut[i]);
    HS_dr_jetjet_cut->Add(H_dr_jetjet_cut[i]);
    HS_dEta_jetjet_cut->Add(H_dEta_jetjet_cut[i]);
    HS_dPhi_jetjet_cut->Add(H_dPhi_jetjet_cut[i]);
    HS_dijetMass_cut->Add(H_dijetMass_cut[i]);
    HS_ptoverMET_cut->Add(H_ptoverMET_cut[i]);
    HS_phoCentral_cut->Add(H_phoCentral_cut[i]);
    HS_jetjetZdPhi_cut->Add(H_jetjetZdPhi_cut[i]);
    HS_jetjetphodPhi_cut->Add(H_jetjetphodPhi_cut[i]);

    for(Int_t jj=0; jj<nhisto; jj++){
      
      HS_phoEB_ptcut[jj]->Add(H_phoEB_ptcut[i][jj]);
      HS_phoEB_Etacut[jj]->Add(H_phoEB_Etacut[i][jj]);
      HS_phoEB_Phicut[jj]->Add(H_phoEB_Phicut[i][jj]);
      HS_MIP_cut[jj]->Add(H_MIP_cut[i][jj]);
      HS_MET_cut[jj]->Add(H_MET_cut[i][jj]);
      HS_METPhi_cut[jj]->Add(H_METPhi_cut[i][jj]);
      HS_dphoMETPhi_cut[jj]->Add(H_dphoMETPhi_cut[i][jj]);
      HS_nvtx_cut[jj]->Add(H_nvtx_cut[i][jj]);
      HS_njet_cut[jj]->Add(H_njet_cut[i][jj]);      
      
      HS_jetpt_cut[jj]->Add(H_jetpt_cut[i][jj]);	
      HS_jetEta_cut[jj]->Add(H_jetEta_cut[i][jj]);	
      HS_jetPhi_cut[jj]->Add(H_jetPhi_cut[i][jj]);	
    }	

    HS_djetMETPhi_cut->Add(H_djetMETPhi_cut[i]);
    HS_dr_phojet->Add(H_dr_phojet[i]);
    HS_dEta_phojet->Add(H_dEta_phojet[i]);
    HS_dPhi_phojet->Add(H_dPhi_phojet[i]);

  }

  //TLegend *lhs = new TLegend(0.6,0.7, 0.92, 0.90);
  TLegend *lhs = new TLegend(0.50, 0.66, 0.90, 0.90);//VBS
  lhs->SetTextSize(0.035);
  lhs->SetFillStyle(0);
  //TString ltext[20] = {"#gamma+jet", "W(#rightarrowl#nu)+jets", "Z(#rightarrow#nu#nu)+jets", "Z(#rightarrowll)+jets", "Monoboson", "Diboson", "Triboson", "Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug", "DATA"};//Znunug QCD
  TString ltext[20] = {"#gamma+jet", "V+jets", "V#gamma+jets", "Monoboson", "Diboson", "Triboson", "tt#gamma/tt", "Z#gamma#rightarrow#nu#nu#gamma(VBS)", "Data"};//Znunug VBS
  //TString ltext[20] = {"#gamma+jet", "V+jets", "Monoboson", "Diboson", "Triboson", "ttJets", "W(#rightarrowl#nu)#gamma", "Z(#rightarrowll)#gamma", "Z(#rightarrow#nu#nu)#gamma", "Z#gamma#rightarrow#nu#nu#gamma(VBS)", "DATA"};//Znunug VBS

  // Data/MC event yield text
  Int_t dataIntegral =  H_phoEB_ptcut[nfile-1][1]->Integral(1, -1);
  Float_t mcIntegral = HA_phoEB_ptcut[1]->Integral(1, -1);
  TLatex *ths = new TLatex(0.51, 0.63, Form("Data/MC = %i/%.2f", dataIntegral, mcIntegral));
  ths->SetNDC();
  ths->SetTextAlign(13);
  //TText *ths  = new TText(0.5, 0.8, Form("Data/MC = %i/%.2f", dataIntegral, mcIntegral));
  ths->SetTextFont(42);
  ths->SetTextSize(0.04);
  
  //HS_dphoMETPhi->Draw("HIST");
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
  pad1->SetLogy(1);
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
  HS_minJMETdPhi_cut->Draw("HIST");
  //H_minJMETdPhi_cut[nfile-1]->Draw("SAME");
  HSig_minJMETdPhi_cut->Draw("HISTSAME");
  HA_minJMETdPhi_cut->Draw("E2 SAME");
  HS_minJMETdPhi_cut->GetYaxis()->SetTitle("Events");
  HS_minJMETdPhi_cut->GetXaxis()->SetTitle("");
  //HS_minJMETdPhi_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_minJMETdPhi_cut->GetXaxis()->SetLabelSize(0);
  HS_minJMETdPhi_cut->SetMaximum(100000);
  HS_minJMETdPhi_cut->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    //if(ifile == nfile-1) lhs->AddEntry(H_minJMETdPhi_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_minJMETdPhi_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_minJMETdPhi_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  //ths->Draw("SAME");
  title = HS_minJMETdPhi_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_minJMETdPhi_cut, H_minJMETdPhi_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#Delta#phi_{min}(j_{1,2}, MET)");
  hratio->Draw("EP");
  tg->Draw("L");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  pad1->cd();
  HS_dr_jetjet_cut->Draw("HIST");
  //H_dr_jetjet_cut[nfile-1]->Draw("SAME");
  HSig_dr_jetjet_cut->Draw("HISTSAME");
  HA_dr_jetjet_cut->Draw("E2 SAME");
  HS_dr_jetjet_cut->GetYaxis()->SetTitle("Events");
  HS_dr_jetjet_cut->GetXaxis()->SetTitle("");
  //HS_dr_jetjet_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_dr_jetjet_cut->GetXaxis()->SetLabelSize(0);
  HS_dr_jetjet_cut->SetMaximum(100000);
  HS_dr_jetjet_cut->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    //if(ifile == nfile-1) lhs->AddEntry(H_dr_jetjet_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dr_jetjet_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dr_jetjet_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  //ths->Draw("SAME");
  title = HS_dr_jetjet_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dr_jetjet_cut, H_dr_jetjet_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#DeltaR(jet , jet)");
  hratio->Draw("EP");
  tg->Draw("L");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  pad1->cd();
  HS_dEta_jetjet_cut->Draw("HIST");
  //H_dEta_jetjet_cut[nfile-1]->Draw("SAME");
  HSig_dEta_jetjet_cut->Draw("HISTSAME");
  HA_dEta_jetjet_cut->Draw("E2 SAME");
  HS_dEta_jetjet_cut->GetYaxis()->SetTitle("Events");
  HS_dEta_jetjet_cut->GetXaxis()->SetTitle("");
  //HS_dEta_jetjet_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_dEta_jetjet_cut->GetXaxis()->SetLabelSize(0);
  HS_dEta_jetjet_cut->SetMaximum(80000);
  HS_dEta_jetjet_cut->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    //if(ifile == nfile-1) lhs->AddEntry(H_dEta_jetjet_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dEta_jetjet_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dEta_jetjet_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  //ths->Draw("SAME");
  title = HS_dEta_jetjet_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dEta_jetjet_cut, H_dEta_jetjet_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#Delta#eta(jet , jet)");
  hratio->Draw("EP");
  tg->Draw("L");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_dPhi_jetjet_cut->Draw("HIST");
  //H_dPhi_jetjet_cut[nfile-1]->Draw("SAME");
  HSig_dPhi_jetjet_cut->Draw("HISTSAME");
  HA_dPhi_jetjet_cut->Draw("E2 SAME");
  HS_dPhi_jetjet_cut->GetYaxis()->SetTitle("Events");
  HS_dPhi_jetjet_cut->GetXaxis()->SetTitle("");
  //HS_dPhi_jetjet_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_dPhi_jetjet_cut->GetXaxis()->SetLabelSize(0);
  HS_dPhi_jetjet_cut->SetMaximum(80000);
  HS_dPhi_jetjet_cut->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    //if(ifile == nfile-1) lhs->AddEntry(H_dPhi_jetjet_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dPhi_jetjet_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dPhi_jetjet_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  //ths->Draw("SAME");
  title = HS_dPhi_jetjet_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dPhi_jetjet_cut, H_dPhi_jetjet_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#Delta#phi(jet , jet)");
  hratio->Draw("EP");
  tg->Draw("L");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  pad1->cd();
  HS_dijetMass_cut->Draw("HIST");
  //H_dijetMass_cut[nfile-1]->Draw("SAME");
  HSig_dijetMass_cut->Draw("HISTSAME");
  HA_dijetMass_cut->Draw("E2 SAME");
  HS_dijetMass_cut->GetYaxis()->SetTitle("Events");
  HS_dijetMass_cut->GetXaxis()->SetTitle("");
  //HS_dijetMass_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_dijetMass_cut->GetXaxis()->SetLabelSize(0);
  HS_dijetMass_cut->GetXaxis()->SetRangeUser(200, 2000);
  HS_dijetMass_cut->SetMaximum(100000);
  HS_dijetMass_cut->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    //if(ifile == nfile-1) lhs->AddEntry(H_dijetMass_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dijetMass_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dijetMass_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  //ths->Draw("SAME");
  title = HS_dijetMass_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dijetMass_cut, H_dijetMass_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("Mass^{jj}[GeV]");
  hratio->GetXaxis()->SetRangeUser(200, 2000);
  hratio->Draw("EP");
  tg->Draw("L");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //linear scale
  pad1->cd();
  pad1->SetLogy(0);
  HS_dijetMass_cut->Draw("HIST");
  //H_dijetMass_cut[nfile-1]->Draw("SAME");
  HSig_dijetMass_cut->Draw("HISTSAME");
  HA_dijetMass_cut->Draw("E2 SAME");
  HS_dijetMass_cut->GetYaxis()->SetTitle("Events");
  HS_dijetMass_cut->GetXaxis()->SetTitle("");
  //HS_dijetMass_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_dijetMass_cut->GetXaxis()->SetLabelSize(0);
  HS_dijetMass_cut->GetXaxis()->SetRangeUser(200, 2000);
  HS_dijetMass_cut->SetMaximum(200);
  HS_dijetMass_cut->SetMinimum(0);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    //if(ifile == nfile-1) lhs->AddEntry(H_dijetMass_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dijetMass_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dijetMass_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  //ths->Draw("SAME");
  title = HS_dijetMass_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dijetMass_cut, H_dijetMass_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("Mass^{jj}[GeV]");
  hratio->GetXaxis()->SetRangeUser(200, 2000);
  hratio->Draw("EP");
  tg->Draw("L");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s_linear.pdf", saveto, title));
  pad1->SetLogy(1);

  
  pad1->cd();
  HS_ptoverMET_cut->Draw("HIST");
  //H_ptoverMET_cut[nfile-1]->Draw("SAME");
  HSig_ptoverMET_cut->Draw("HISTSAME");
  HA_ptoverMET_cut->Draw("E2 SAME");
  HS_ptoverMET_cut->GetYaxis()->SetTitle("Events");
  HS_ptoverMET_cut->GetXaxis()->SetTitle("");
  //HS_ptoverMET_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_ptoverMET_cut->GetXaxis()->SetLabelSize(0);
  HS_ptoverMET_cut->GetXaxis()->SetRangeUser(0, 3);
  //if(jj==1){HS_ptoverMET_cut->SetMaximum(10000); HS_ptoverMET_cut->SetMinimum(0.1);}
  HS_ptoverMET_cut->SetMaximum(10000);
  HS_ptoverMET_cut->SetMinimum(0.1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    //if(ifile == nfile-1) lhs->AddEntry(H_ptoverMET_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_ptoverMET_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_ptoverMET_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  //ths->Draw("SAME");
  title = HS_ptoverMET_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_ptoverMET_cut, H_ptoverMET_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetRangeUser(0, 3);
  hratio->GetXaxis()->SetTitle("p_{T}^{#gamma}/E_{T}^{miss}");
  hratio->Draw("EP");
  tg->Draw("L");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    
  //linear scale ptoverMET
  pad1->cd();
  pad1->SetLogy(0);
  HS_ptoverMET_cut->Draw("HIST");
  //H_ptoverMET_cut[nfile-1]->Draw("SAME");
  HSig_ptoverMET_cut->Draw("HISTSAME");
  HA_ptoverMET_cut->Draw("E2 SAME");
  HS_ptoverMET_cut->GetYaxis()->SetTitle("Events");
  HS_ptoverMET_cut->GetXaxis()->SetTitle("");
  //HS_ptoverMET_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_ptoverMET_cut->GetXaxis()->SetLabelSize(0);
  HS_ptoverMET_cut->GetXaxis()->SetRangeUser(0, 3);
  //if(jj==1){HS_ptoverMET_cut->SetMaximum(120); HS_ptoverMET_cut->SetMinimum(0);}
  HS_ptoverMET_cut->SetMaximum(120);
  HS_ptoverMET_cut->SetMinimum(0);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    //if(ifile == nfile-1) lhs->AddEntry(H_ptoverMET_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_ptoverMET_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_ptoverMET_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  //ths->Draw("SAME");
  title = HS_ptoverMET_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_ptoverMET_cut, H_ptoverMET_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetRangeUser(0, 3);
  hratio->GetXaxis()->SetTitle("p_{T}^{#gamma}/E_{T}^{miss}");
  hratio->Draw("EP");
  tg->Draw("L");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s_linear.pdf", saveto, title));
  pad1->SetLogy(1);

  
  pad1->cd();
  HS_phoCentral_cut->Draw("HIST");
  //H_phoCentral_cut[nfile-1]->Draw("SAME");
  HSig_phoCentral_cut->Draw("HISTSAME");
  HA_phoCentral_cut->Draw("E2 SAME");
  HS_phoCentral_cut->GetYaxis()->SetTitle("Events");
  HS_phoCentral_cut->GetXaxis()->SetTitle("");
  //HS_phoCentral_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_phoCentral_cut->GetXaxis()->SetLabelSize(0);
  HS_phoCentral_cut->SetMaximum(10000);
  HS_phoCentral_cut->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    //if(ifile == nfile-1) lhs->AddEntry(H_phoCentral_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_phoCentral_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_phoCentral_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  //ths->Draw("SAME");
  title = HS_phoCentral_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_phoCentral_cut, H_phoCentral_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("C_{#gamma}");
  hratio->Draw("EP");
  tg->Draw("L");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_jetjetZdPhi_cut->Draw("HIST");
  //H_jetjetZdPhi_cut[nfile-1]->Draw("SAME");
  HSig_jetjetZdPhi_cut->Draw("HISTSAME");
  HA_jetjetZdPhi_cut->Draw("E2 SAME");
  HS_jetjetZdPhi_cut->GetYaxis()->SetTitle("Events");
  HS_jetjetZdPhi_cut->GetXaxis()->SetTitle("");
  //HS_jetjetZdPhi_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_jetjetZdPhi_cut->GetXaxis()->SetLabelSize(0);
  HS_jetjetZdPhi_cut->SetMaximum(10000);
  HS_jetjetZdPhi_cut->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    //if(ifile == nfile-1) lhs->AddEntry(H_jetjetZdPhi_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_jetjetZdPhi_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_jetjetZdPhi_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  //ths->Draw("SAME");
  title = HS_jetjetZdPhi_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_jetjetZdPhi_cut, H_jetjetZdPhi_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#Delta#phi(Z, jj)");
  hratio->Draw("EP");
  tg->Draw("L");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_jetjetphodPhi_cut->Draw("HIST");
  //H_jetjetphodPhi_cut[nfile-1]->Draw("SAME");
  HSig_jetjetphodPhi_cut->Draw("HISTSAME");
  HA_jetjetphodPhi_cut->Draw("E2 SAME");
  HS_jetjetphodPhi_cut->GetYaxis()->SetTitle("Events");
  HS_jetjetphodPhi_cut->GetXaxis()->SetTitle("");
  //HS_jetjetphodPhi_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_jetjetphodPhi_cut->GetXaxis()->SetLabelSize(0);
  HS_jetjetphodPhi_cut->SetMaximum(10000);
  HS_jetjetphodPhi_cut->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    //if(ifile == nfile-1) lhs->AddEntry(H_jetjetphodPhi_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_jetjetphodPhi_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_jetjetphodPhi_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  //ths->Draw("SAME");
  title = HS_jetjetphodPhi_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_jetjetphodPhi_cut, H_jetjetphodPhi_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#Delta#phi(#gamma, jj)");
  hratio->Draw("EP");
  tg->Draw("L");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  for(Int_t jj=0; jj<nhisto; jj++){
    
    pad1->cd();
    HS_phoEB_ptcut[jj]->Draw("HIST");
    //H_phoEB_ptcut[nfile-1][jj]->Draw("SAME");
    HSig_phoEB_ptcut[jj]->Draw("HISTSAME");
    HA_phoEB_ptcut[jj]->Draw("E2 SAME");
    HS_phoEB_ptcut[jj]->GetYaxis()->SetTitle("Events");
    HS_phoEB_ptcut[jj]->GetXaxis()->SetTitle("");
    //HS_phoEB_ptcut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_ptcut[jj]->GetXaxis()->SetLabelSize(0);
    //HS_phoEB_ptcut[jj]->GetXaxis()->SetRangeUser(210, 1000);
    HS_phoEB_ptcut[jj]->GetXaxis()->SetRangeUser(0, 220); // Et75
    HS_phoEB_ptcut[jj]->SetMaximum(10000);
    HS_phoEB_ptcut[jj]->SetMinimum(1);
    if(jj==1){HS_phoEB_ptcut[jj]->SetMaximum(100000); HS_phoEB_ptcut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_phoEB_ptcut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_phoEB_ptcut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_phoEB_ptcut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_phoEB_ptcut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_ptcut[jj], H_phoEB_ptcut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("P_{T}^{#gamma}[GeV]");
    hratio->GetXaxis()->SetRangeUser(0, 220);
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    
    pad1->cd();
    HS_phoEB_Etacut[jj]->Draw("HIST");
    //H_phoEB_Etacut[nfile-1][jj]->Draw("SAME");
    HSig_phoEB_Etacut[jj]->Draw("HISTSAME");
    HA_phoEB_Etacut[jj]->Draw("E2 SAME");
    HS_phoEB_Etacut[jj]->GetYaxis()->SetTitle("Events");
    HS_phoEB_Etacut[jj]->GetXaxis()->SetTitle("");
    //HS_phoEB_Etacut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_Etacut[jj]->GetXaxis()->SetLabelSize(0);
    HS_phoEB_Etacut[jj]->SetMaximum(100000);
    HS_phoEB_Etacut[jj]->SetMinimum(1);
    if(jj==1){HS_phoEB_Etacut[jj]->SetMaximum(10000); HS_phoEB_Etacut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_phoEB_Etacut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_phoEB_Etacut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_phoEB_Etacut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_phoEB_Etacut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_Etacut[jj], H_phoEB_Etacut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("|#eta^{#gamma}|");
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_phoEB_Phicut[jj]->Draw("HIST");
    //H_phoEB_Phicut[nfile-1][jj]->Draw("SAME");
    HSig_phoEB_Phicut[jj]->Draw("HISTSAME");
    HA_phoEB_Phicut[jj]->Draw("E2 SAME");
    HS_phoEB_Phicut[jj]->GetYaxis()->SetTitle("Events");
    HS_phoEB_Phicut[jj]->GetXaxis()->SetTitle("");
    //HS_phoEB_Phicut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_Phicut[jj]->GetXaxis()->SetLabelSize(0);
    HS_phoEB_Phicut[jj]->SetMaximum(80000);
    HS_phoEB_Phicut[jj]->SetMinimum(1);
    if(jj==1){HS_phoEB_Phicut[jj]->SetMaximum(10000); HS_phoEB_Phicut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_phoEB_Phicut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_phoEB_Phicut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_phoEB_Phicut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_phoEB_Phicut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_Phicut[jj], H_phoEB_Phicut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#phi^{#gamma}");
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    
    pad1->cd();
    HS_MET_cut[jj]->Draw("HIST");
    //H_MET_cut[nfile-1][jj]->Draw("SAME");
    HSig_MET_cut[jj]->Draw("HISTSAME");
    HA_MET_cut[jj]->Draw("E2 SAME");
    HS_MET_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_MET_cut[jj]->GetXaxis()->SetTitle("");
    //HS_MET_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_MET_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_MET_cut[jj]->SetMaximum(10000);
    HS_MET_cut[jj]->SetMinimum(1);
    if(jj==1){HS_MET_cut[jj]->SetMaximum(10000); HS_MET_cut[jj]->SetMinimum(0.01);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_MET_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_MET_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_MET_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_MET_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_MET_cut[jj], H_MET_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("E_{T}^{miss}");
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    
    pad1->cd();
    HS_METPhi_cut[jj]->Draw("HIST");
    //H_METPhi_cut[nfile-1][jj]->Draw("SAME");
    HSig_METPhi_cut[jj]->Draw("HISTSAME");
    HA_METPhi_cut[jj]->Draw("E2 SAME");
    HS_METPhi_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_METPhi_cut[jj]->GetXaxis()->SetTitle("");
    //HS_METPhi_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_METPhi_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_METPhi_cut[jj]->SetMaximum(10000);
    HS_METPhi_cut[jj]->SetMinimum(1);
    if(jj==1){HS_METPhi_cut[jj]->SetMaximum(10000); HS_METPhi_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_METPhi_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_METPhi_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_METPhi_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_METPhi_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_METPhi_cut[jj], H_METPhi_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#phi^{MET}");
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_MIP_cut[jj]->Draw("HIST");
    //H_MIP_cut[nfile-1][jj]->Draw("SAME");
    HSig_MIP_cut[jj]->Draw("HISTSAME");
    HA_MIP_cut[jj]->Draw("E2 SAME");
    HS_MIP_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_MIP_cut[jj]->GetXaxis()->SetTitle("");
    //HS_MIP_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_MIP_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_MIP_cut[jj]->SetMaximum(10000);
    HS_MIP_cut[jj]->SetMinimum(1);
    if(jj==1){HS_MIP_cut[jj]->SetMaximum(10000); HS_MIP_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_MIP_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_MIP_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_MIP_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_MIP_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_MIP_cut[jj], H_MIP_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("MIP[GeV]");
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_dphoMETPhi_cut[jj]->Draw("HIST");
    //H_dphoMETPhi_cut[nfile-1][jj]->Draw("SAME");
    HSig_dphoMETPhi_cut[jj]->Draw("HISTSAME");
    HA_dphoMETPhi_cut[jj]->Draw("E2 SAME");
    HS_dphoMETPhi_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_dphoMETPhi_cut[jj]->GetXaxis()->SetTitle("");
    //HS_dphoMETPhi_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_dphoMETPhi_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_dphoMETPhi_cut[jj]->SetMaximum(10000);
    HS_dphoMETPhi_cut[jj]->SetMinimum(1);
    if(jj==1){HS_dphoMETPhi_cut[jj]->SetMaximum(10000); HS_dphoMETPhi_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_dphoMETPhi_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dphoMETPhi_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dphoMETPhi_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_dphoMETPhi_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dphoMETPhi_cut[jj], H_dphoMETPhi_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#Delta#phi(#gamma , E_{T}^{miss})");
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_nvtx_cut[jj]->Draw("HIST");
    //H_nvtx_cut[nfile-1][jj]->Draw("SAME");
    HSig_nvtx_cut[jj]->Draw("HISTSAME");
    HA_nvtx_cut[jj]->Draw("E2 SAME");
    HS_nvtx_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_nvtx_cut[jj]->GetXaxis()->SetTitle("");
    //HS_nvtx_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_nvtx_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_nvtx_cut[jj]->SetMaximum(10000);
    HS_nvtx_cut[jj]->SetMinimum(1);
    if(jj==1){HS_nvtx_cut[jj]->SetMaximum(10000); HS_nvtx_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_nvtx_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_nvtx_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_nvtx_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_nvtx_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_nvtx_cut[jj], H_nvtx_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("Nvtx");
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_njet_cut[jj]->Draw("HIST");
    //H_njet_cut[nfile-1][jj]->Draw("SAME");
    HSig_njet_cut[jj]->Draw("HISTSAME");
    HA_njet_cut[jj]->Draw("E2 SAME");
    HS_njet_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_njet_cut[jj]->GetXaxis()->SetTitle("");
    //HS_njet_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_njet_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_njet_cut[jj]->SetMaximum(10000);
    HS_njet_cut[jj]->SetMinimum(1);
    if(jj==1){HS_njet_cut[jj]->SetMaximum(10000); HS_njet_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_njet_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_njet_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_njet_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_njet_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_njet_cut[jj], H_njet_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("Njet");
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_djetMETPhi_cut->Draw("HIST");
    //H_djetMETPhi_cut[nfile-1]->Draw("SAME");
    HSig_djetMETPhi_cut->Draw("HISTSAME");
    HA_djetMETPhi_cut->Draw("E2 SAME");
    HS_djetMETPhi_cut->GetYaxis()->SetTitle("Events");
    HS_djetMETPhi_cut->GetXaxis()->SetTitle("");
    //HS_djetMETPhi_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_djetMETPhi_cut->GetXaxis()->SetLabelSize(0);
    HS_djetMETPhi_cut->SetMaximum(100000);
    HS_djetMETPhi_cut->SetMinimum(1);
    if(jj==1){HS_djetMETPhi_cut->SetMaximum(100000); HS_djetMETPhi_cut->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_djetMETPhi_cut[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_djetMETPhi_cut, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_djetMETPhi_cut[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_djetMETPhi_cut->GetName();
    pad2->cd();
    hratio = ratioplot(HS_djetMETPhi_cut, H_djetMETPhi_cut[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle(Form("#Delta#phi(jet%i , E_{T}^{miss})", jj));
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    
    pad1->cd();
    HS_dr_phojet->Draw("HIST");
    //H_dr_phojet[nfile-1]->Draw("SAME");
    HSig_dr_phojet->Draw("HISTSAME");
    HA_dr_phojet->Draw("E2 SAME");
    HS_dr_phojet->GetYaxis()->SetTitle("Events");
    HS_dr_phojet->GetXaxis()->SetTitle("");
    //HS_dr_phojet->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_dr_phojet->GetXaxis()->SetLabelSize(0);
    HS_dr_phojet->SetMaximum(10000);
    HS_dr_phojet->SetMinimum(1);
    if(jj==1){HS_dr_phojet->SetMaximum(10000); HS_dr_phojet->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_dr_phojet[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dr_phojet, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dr_phojet[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_dr_phojet->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dr_phojet, H_dr_phojet[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle(Form("#DeltaR(#gamma , jet%i)", jj));
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_dEta_phojet->Draw("HIST");
    //H_dEta_phojet[nfile-1]->Draw("SAME");
    HSig_dEta_phojet->Draw("HISTSAME");
    HA_dEta_phojet->Draw("E2 SAME");
    HS_dEta_phojet->GetYaxis()->SetTitle("Events");
    HS_dEta_phojet->GetXaxis()->SetTitle("");
    //HS_dEta_phojet->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_dEta_phojet->GetXaxis()->SetLabelSize(0);
    HS_dEta_phojet->SetMaximum(100000);
    HS_dEta_phojet->SetMinimum(1);
    if(jj==1){HS_dEta_phojet->SetMaximum(10000); HS_dEta_phojet->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_dEta_phojet[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dEta_phojet, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dEta_phojet[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_dEta_phojet->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dEta_phojet, H_dEta_phojet[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle(Form("#Delta#eta(#gamma , jet%i)", jj));
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_dPhi_phojet->Draw("HIST");
    //H_dPhi_phojet[nfile-1]->Draw("SAME");
    HSig_dPhi_phojet->Draw("HISTSAME");
    HA_dPhi_phojet->Draw("E2 SAME");
    HS_dPhi_phojet->GetYaxis()->SetTitle("Events");
    HS_dPhi_phojet->GetXaxis()->SetTitle("");
    //HS_dPhi_phojet->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_dPhi_phojet->GetXaxis()->SetLabelSize(0);
    HS_dPhi_phojet->SetMaximum(100000);
    HS_dPhi_phojet->SetMinimum(1);
    if(jj==1){HS_dPhi_phojet->SetMaximum(50000); HS_dPhi_phojet->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_dPhi_phojet[ifile], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dPhi_phojet, ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dPhi_phojet[ifile], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_dPhi_phojet->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dPhi_phojet, H_dPhi_phojet[nfile-1]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle(Form("#Delta#phi(#gamma , jet%i)", jj));
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

      
    pad1->cd();
    HS_jetpt_cut[jj]->Draw("HIST");
    //H_jetpt_cut[nfile-1][jj]->Draw("SAME");
    HSig_jetpt_cut[jj]->Draw("HISTSAME");
    HA_jetpt_cut[jj]->Draw("E2 SAME");
    HS_jetpt_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_jetpt_cut[jj]->GetXaxis()->SetTitle("");
    //HS_jetpt_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_jetpt_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_jetpt_cut[jj]->SetMaximum(100000);
    HS_jetpt_cut[jj]->SetMinimum(1);
    if(jj==1){HS_jetpt_cut[jj]->SetMaximum(10000); HS_jetpt_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_jetpt_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_jetpt_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_jetpt_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_jetpt_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_jetpt_cut[jj], H_jetpt_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("P_{T}^{jet}[GeV]");
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_jetEta_cut[jj]->Draw("HIST");
    //H_jetEta_cut[nfile-1][jj]->Draw("SAME");
    HSig_jetEta_cut[jj]->Draw("HISTSAME");
    HA_jetEta_cut[jj]->Draw("E2 SAME");
    HS_jetEta_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_jetEta_cut[jj]->GetXaxis()->SetTitle("");
    //HS_jetEta_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_jetEta_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_jetEta_cut[jj]->SetMaximum(100000);
    HS_jetEta_cut[jj]->SetMinimum(1);
    if(jj==1){HS_jetEta_cut[jj]->SetMaximum(10000); HS_jetEta_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_jetEta_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_jetEta_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_jetEta_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_jetEta_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_jetEta_cut[jj], H_jetEta_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("|#eta^{jet}|");
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_jetPhi_cut[jj]->Draw("HIST");
    //H_jetPhi_cut[nfile-1][jj]->Draw("SAME");
    HSig_jetPhi_cut[jj]->Draw("HISTSAME");
    HA_jetPhi_cut[jj]->Draw("E2 SAME");
    HS_jetPhi_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_jetPhi_cut[jj]->GetXaxis()->SetTitle("");
    //HS_jetPhi_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_jetPhi_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_jetPhi_cut[jj]->SetMaximum(10000);
    HS_jetPhi_cut[jj]->SetMinimum(1);
    if(jj==1){HS_jetPhi_cut[jj]->SetMaximum(50000); HS_jetPhi_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      //if(ifile == nfile-1) lhs->AddEntry(H_jetPhi_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_jetPhi_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_jetPhi_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    //ths->Draw("SAME");
    title = HS_jetPhi_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_jetPhi_cut[jj], H_jetPhi_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#phi^{jet}");
    hratio->Draw("EP");
    tg->Draw("L");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  }  
}
