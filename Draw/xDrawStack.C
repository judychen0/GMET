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
  //Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 0.296};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, Znng]
  Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 0.1817};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, ZnngVBS]
  
  Float_t filter[20] = {1., 1., 1., 1., 1., 1., 1.};
  Float_t kfactor[20] = {1., 1., 1., 1., 1., 1., 1.};
  /*
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/GJet/sumGenW.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/sumGenW.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/ZJet/sumGenW.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/sumGenW.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Wg_aMCatNLO/sumGenW.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Zg_aMCatNLO/sumGenW.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_TT_aMCatNLO/sumGenW.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/sumGenW.root";
    rootname[8] = "/home/judy/ntuhep/GMET/output_file/summer16/data/sumGenW.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/GJet/sumGenW.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/sumGenW.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/ZJet/sumGenW.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/DYJet/sumGenW.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Wg_aMCatNLO/sumGenW.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Zg_aMCatNLO/sumGenW.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_TT_aMCatNLO/sumGenW.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/sumGenW.root";
    rootname[8] = "/home/judy/ntuhep/GMET/output_file/fall17/data/sumGenW.root";
  }
  */
  
  
  if(year==2016){
    rootname[0] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/mc/job_summer16_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/mc/job_summer16_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/mc/job_summer16_TT_aMCatNLO/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    //rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/mc/job_summer16_Zg_EWK/output_ggtree.root";
    rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/summer16/data/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/mc/job_fall17_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/mc/job_fall17_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/mc/job_fall17_TT_aMCatNLO/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    //rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/data/output_ggtree.root";
    rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/fall17/data/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/mc/job_autumn18_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/mc/job_autumn18_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/mc/job_autumn18_TT_aMCatNLO_ext1/output_ggtree.root";
    //rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/mc/job_autumn18_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/mc/job_UL18_Znunug_VBS/output_ggtree.root";
    rootname[11] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/211229/autumn18/data/output_ggtree.root";
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
  
  TH1F *H_phoEB_ptcut[nfile][nhisto];
  TH1F *H_phoEB_Etacut[nfile][nhisto];
  TH1F *H_phoEB_Phicut[nfile][nhisto];
  TH1F *H_phoEB_ptoverMET_cut[nfile][nhisto];
  TH1F *H_MET_cut[nfile][nhisto];
  TH1F *H_METPhi_cut[nfile][nhisto];
  TH1F *H_MIP_cut[nfile][nhisto];
  TH1F *H_dphoMETPhi_cut[nfile][nhisto];
  TH1F *H_nvtx_cut[nfile][nhisto];
  TH1F *H_njet_cut[nfile][nhisto];
  
  TH1F *H_jetpt_cut[nfile][nhisto][njet];
  TH1F *H_jetEta_cut[nfile][nhisto][njet];
  TH1F *H_jetPhi_cut[nfile][nhisto][njet];
  TH1F *H_djetMETPhi_cut[nfile][nhisto][njet];
  TH1F *H_dr_phojet[nfile][nhisto][njet];
  TH1F *H_dEta_phojet[nfile][nhisto][njet];
  TH1F *H_dPhi_phojet[nfile][nhisto][njet];

  TH1F *H_dr_jetjet[nfile];
  TH1F *H_dEta_jetjet[nfile];
  TH1F *H_dPhi_jetjet[nfile];
  TH1F *H_dijetMass_cut[nfile];

  TH1F *H_MET_phojetdR[nfile];
  TH1F *H_MET_jetjetdEta[nfile];
  TH1F *H_MET_jetjetdR[nfile];
  TH1F *H_MET_dijetMass[nfile];
  TH1F *H_MET_jetveto[nfile];
  
  // Add mc histos
  TH1F *HA_phoEB_ptcut[nhisto];		 
  TH1F *HA_phoEB_Etacut[nhisto];		 
  TH1F *HA_phoEB_Phicut[nhisto];		 
  TH1F *HA_phoEB_ptoverMET_cut[nhisto];	 
  TH1F *HA_MET_cut[nhisto];		 
  TH1F *HA_METPhi_cut[nhisto];		 
  TH1F *HA_MIP_cut[nhisto];		 
  TH1F *HA_dphoMETPhi_cut[nhisto];	 
  TH1F *HA_nvtx_cut[nhisto];		 
  TH1F *HA_njet_cut[nhisto];		 

  TH1F *HA_jetpt_cut[nhisto][njet];
  TH1F *HA_jetEta_cut[nhisto][njet];
  TH1F *HA_jetPhi_cut[nhisto][njet];
  TH1F *HA_djetMETPhi_cut[nhisto][njet];
  TH1F *HA_dr_phojet[nhisto][njet];
  TH1F *HA_dEta_phojet[nhisto][njet];
  TH1F *HA_dPhi_phojet[nhisto][njet];
  
  TH1F *HA_dr_jetjet;
  TH1F *HA_dEta_jetjet;
  TH1F *HA_dPhi_jetjet;
  TH1F *HA_dijetMass_cut;

  TH1F *HA_MET_phojetdR;
  TH1F *HA_MET_jetjetdEta;
  TH1F *HA_MET_jetjetdR;
  TH1F *HA_MET_dijetMass;
  TH1F *HA_MET_jetveto;

  // signal highlight
  TH1F *HSig_phoEB_ptcut[nhisto];
  TH1F *HSig_phoEB_Etacut[nhisto];
  TH1F *HSig_phoEB_Phicut[nhisto];
  TH1F *HSig_phoEB_ptoverMET_cut[nhisto];
  TH1F *HSig_MET_cut[nhisto];
  TH1F *HSig_METPhi_cut[nhisto];
  TH1F *HSig_MIP_cut[nhisto];
  TH1F *HSig_dphoMETPhi_cut[nhisto];
  TH1F *HSig_nvtx_cut[nhisto];
  TH1F *HSig_njet_cut[nhisto];
  
  TH1F *HSig_jetpt_cut[nhisto][njet];
  TH1F *HSig_jetEta_cut[nhisto][njet];
  TH1F *HSig_jetPhi_cut[nhisto][njet];
  TH1F *HSig_djetMETPhi_cut[nhisto][njet];
  TH1F *HSig_dr_phojet[nhisto][njet];
  TH1F *HSig_dEta_phojet[nhisto][njet];
  TH1F *HSig_dPhi_phojet[nhisto][njet];

  TH1F *HSig_dr_jetjet;
  TH1F *HSig_dEta_jetjet;
  TH1F *HSig_dPhi_jetjet;
  TH1F *HSig_dijetMass_cut;

  TH1F *HSig_MET_phojetdR;
  TH1F *HSig_MET_jetjetdEta;
  TH1F *HSig_MET_jetjetdR;
  TH1F *HSig_MET_dijetMass;
  TH1F *HSig_MET_jetveto;

  
  const char *title;
  const char *saveto = ".";
  
  for(Int_t i=0; i<nfile; i++){
    //if(i==10) continue;//pass Zvv signal
    fopen = new TFile(rootname[i]);
    if(i==nfile-1) scale[i] = 1.0;
    
    H_dr_jetjet[i]	= (TH1F*)fopen->Get("h_dr_jetjet");
    H_dEta_jetjet[i]	= (TH1F*)fopen->Get("h_dEta_jetjet");
    H_dPhi_jetjet[i]	= (TH1F*)fopen->Get("h_dPhi_jetjet");
    H_dijetMass_cut[i]	= (TH1F*)fopen->Get("h_dijetMass_cut");
    H_dijetMass_cut[i]->Rebin(8);

    H_MET_phojetdR[i] = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_phojetdR");
    H_MET_jetjetdEta[i] = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_jetjetdEta");
    H_MET_jetjetdR[i] = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_jetjetdR");
    H_MET_dijetMass[i] = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_dijetMass");
    H_MET_jetveto[i] = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_jetveto");
    
    if(i==0){
      HA_dr_jetjet = (TH1F*)fopen->Get("h_dr_jetjet")->Clone();
      HA_dEta_jetjet = (TH1F*)fopen->Get("h_dEta_jetjet")->Clone();
      HA_dPhi_jetjet = (TH1F*)fopen->Get("h_dPhi_jetjet")->Clone();
      HA_dijetMass_cut = (TH1F*)fopen->Get("h_dijetMass_cut")->Clone();
      //HA_dijetMass_cut->Rebin(8);

      HA_MET_phojetdR = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_phojetdR")->Clone();
      HA_MET_jetjetdEta = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_jetjetdEta")->Clone();
      HA_MET_jetjetdR = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_jetjetdR")->Clone();
      HA_MET_dijetMass = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_dijetMass")->Clone();
      HA_MET_jetveto = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_jetveto")->Clone();

      ///
      HA_dr_jetjet->Scale(scale[i]);
      HA_dEta_jetjet->Scale(scale[i]);
      HA_dPhi_jetjet->Scale(scale[i]);
      HA_dijetMass_cut->Scale(scale[i]);
      
      HA_MET_phojetdR->Scale(scale[i]);
      HA_MET_jetjetdEta->Scale(scale[i]);
      HA_MET_jetjetdR->Scale(scale[i]);
      HA_MET_dijetMass->Scale(scale[i]);
      HA_MET_jetveto->Scale(scale[i]);

      ///
      HA_dr_jetjet->SetFillStyle(3145);
      HA_dEta_jetjet->SetFillStyle(3145);
      HA_dPhi_jetjet->SetFillStyle(3145);
      HA_dijetMass_cut->SetFillStyle(3145);

      HA_MET_phojetdR->SetFillStyle(3145);
      HA_MET_jetjetdEta->SetFillStyle(3145);
      HA_MET_jetjetdR->SetFillStyle(3145);
      HA_MET_dijetMass->SetFillStyle(3145);
      HA_MET_jetveto->SetFillStyle(3145);

      ///
      HA_dr_jetjet->SetFillColor(12);
      HA_dEta_jetjet->SetFillColor(12);
      HA_dPhi_jetjet->SetFillColor(12);
      HA_dijetMass_cut->SetFillColor(12);

      HA_MET_phojetdR->SetFillColor(12);
      HA_MET_jetjetdEta->SetFillColor(12);
      HA_MET_jetjetdR->SetFillColor(12);
      HA_MET_dijetMass->SetFillColor(12);
      HA_MET_jetveto->SetFillColor(12);

      ///
      HA_dr_jetjet->SetMarkerSize(0.1);	 
      HA_dEta_jetjet->SetMarkerSize(0.1);	 
      HA_dPhi_jetjet->SetMarkerSize(0.1);	 
      HA_dijetMass_cut->SetMarkerSize(0.1);

      HA_MET_phojetdR->SetMarkerSize(0.1);
      HA_MET_jetjetdEta->SetMarkerSize(0.1);
      HA_MET_jetjetdR->SetMarkerSize(0.1);
      HA_MET_dijetMass->SetMarkerSize(0.1);
      HA_MET_jetveto->SetMarkerSize(0.1);

    }

    if(i==nfile-2){
      HSig_dr_jetjet = (TH1F*)fopen->Get("h_dr_jetjet")->Clone();
      HSig_dEta_jetjet = (TH1F*)fopen->Get("h_dEta_jetjet")->Clone();
      HSig_dPhi_jetjet = (TH1F*)fopen->Get("h_dPhi_jetjet")->Clone();
      HSig_dijetMass_cut = (TH1F*)fopen->Get("h_dijetMass_cut")->Clone();
      //HA_dijetMass_cut->Rebin(8);

      HSig_MET_phojetdR = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_phojetdR")->Clone();
      HSig_MET_jetjetdEta = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_jetjetdEta")->Clone();
      HSig_MET_jetjetdR = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_jetjetdR")->Clone();
      HSig_MET_dijetMass = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_dijetMass")->Clone();
      HSig_MET_jetveto = (TH1F*)fopen->Get("h_MET_jetcut/h_MET_jetveto")->Clone();

      ///
      HSig_dr_jetjet->Scale(scale[i]);
      HSig_dEta_jetjet->Scale(scale[i]);
      HSig_dPhi_jetjet->Scale(scale[i]);
      HSig_dijetMass_cut->Scale(scale[i]);

      HSig_MET_phojetdR->Scale(scale[i]);
      HSig_MET_jetjetdEta->Scale(scale[i]);
      HSig_MET_jetjetdR->Scale(scale[i]);
      HSig_MET_dijetMass->Scale(scale[i]);
      HSig_MET_jetveto->Scale(scale[i]);

      ///
      HSig_dr_jetjet->SetLineColor(kRed);	 
      HSig_dEta_jetjet->SetLineColor(kRed);	 
      HSig_dPhi_jetjet->SetLineColor(kRed);	 
      HSig_dijetMass_cut->SetLineColor(kRed);

      HSig_MET_phojetdR->SetLineColor(kRed);
      HSig_MET_jetjetdEta->SetLineColor(kRed);
      HSig_MET_jetjetdR->SetLineColor(kRed);
      HSig_MET_dijetMass->SetLineColor(kRed);
      HSig_MET_jetveto->SetLineColor(kRed);

      ///
      HSig_dr_jetjet->SetLineWidth(2);	 
      HSig_dEta_jetjet->SetLineWidth(2);	 
      HSig_dPhi_jetjet->SetLineWidth(2);	 
      HSig_dijetMass_cut->SetLineWidth(2);

      HSig_MET_phojetdR->SetLineWidth(2);
      HSig_MET_jetjetdEta->SetLineWidth(2);
      HSig_MET_jetjetdR->SetLineWidth(2);
      HSig_MET_dijetMass->SetLineWidth(2);
      HSig_MET_jetveto->SetLineWidth(2);

    }
    
    H_dr_jetjet[i]->Scale(scale[i]);	 
    H_dEta_jetjet[i]->Scale(scale[i]);	 
    H_dPhi_jetjet[i]->Scale(scale[i]);	 
    H_dijetMass_cut[i]->Scale(scale[i]);

    H_MET_phojetdR[i]->Scale(scale[i]);
    H_MET_jetjetdEta[i]->Scale(scale[i]);
    H_MET_jetjetdR[i]->Scale(scale[i]);
    H_MET_dijetMass[i]->Scale(scale[i]);
    H_MET_jetveto[i]->Scale(scale[i]);

    if(i<nfile-1){
      H_dr_jetjet[i]->SetFillColor(cnum[i]);	 
      H_dEta_jetjet[i]->SetFillColor(cnum[i]);	 
      H_dPhi_jetjet[i]->SetFillColor(cnum[i]);	 
      H_dijetMass_cut[i]->SetFillColor(cnum[i]);

      H_MET_phojetdR[i]->SetFillColor(cnum[i]);
      H_MET_jetjetdEta[i]->SetFillColor(cnum[i]);
      H_MET_jetjetdR[i]->SetFillColor(cnum[i]);
      H_MET_dijetMass[i]->SetFillColor(cnum[i]);
      H_MET_jetveto[i]->SetFillColor(cnum[i]);

    }

    ///
    H_dr_jetjet[i]->SetLineColor(1);	 
    H_dEta_jetjet[i]->SetLineColor(1);	 
    H_dPhi_jetjet[i]->SetLineColor(1);	 
    H_dijetMass_cut[i]->SetLineColor(1);

    H_MET_phojetdR[i]->SetLineColor(1);
    H_MET_jetjetdEta[i]->SetLineColor(1);
    H_MET_jetjetdR[i]->SetLineColor(1);
    H_MET_dijetMass[i]->SetLineColor(1);
    H_MET_jetveto[i]->SetLineColor(1);

    ///
    H_dr_jetjet[i]->SetLineWidth(1);	 
    H_dEta_jetjet[i]->SetLineWidth(1);	 
    H_dPhi_jetjet[i]->SetLineWidth(1);	 
    H_dijetMass_cut[i]->SetLineWidth(1);

    H_MET_phojetdR[i]->SetLineWidth(1);
    H_MET_jetjetdEta[i]->SetLineWidth(1);
    H_MET_jetjetdR[i]->SetLineWidth(1);
    H_MET_dijetMass[i]->SetLineWidth(1);
    H_MET_jetveto[i]->SetLineWidth(1);

    
    if(i == nfile-1){
      ///
      H_dr_jetjet[i]->SetLineWidth(2);	 
      H_dEta_jetjet[i]->SetLineWidth(2);	 
      H_dPhi_jetjet[i]->SetLineWidth(2);	 
      H_dijetMass_cut[i]->SetLineWidth(2);

      H_MET_phojetdR[i]->SetLineWidth(2);
      H_MET_jetjetdEta[i]->SetLineWidth(2);
      H_MET_jetjetdR[i]->SetLineWidth(2);
      H_MET_dijetMass[i]->SetLineWidth(2);
      H_MET_jetveto[i]->SetLineWidth(2);

      ///
      H_dr_jetjet[i]->SetMarkerStyle(8);	 
      H_dEta_jetjet[i]->SetMarkerStyle(8);	 
      H_dPhi_jetjet[i]->SetMarkerStyle(8);	 
      H_dijetMass_cut[i]->SetMarkerStyle(8);

      H_MET_phojetdR[i]->SetMarkerStyle(8);
      H_MET_jetjetdEta[i]->SetMarkerStyle(8);
      H_MET_jetjetdR[i]->SetMarkerStyle(8);
      H_MET_dijetMass[i]->SetMarkerStyle(8);
      H_MET_jetveto[i]->SetMarkerStyle(8);

      ///
      H_dr_jetjet[i]->SetMarkerSize(0.9);	 
      H_dEta_jetjet[i]->SetMarkerSize(0.9);	 
      H_dPhi_jetjet[i]->SetMarkerSize(0.9);	 
      H_dijetMass_cut[i]->SetMarkerSize(0.9);

      H_MET_phojetdR[i]->SetMarkerSize(0.9);
      H_MET_jetjetdEta[i]->SetMarkerSize(0.9);
      H_MET_jetjetdR[i]->SetMarkerSize(0.9);
      H_MET_dijetMass[i]->SetMarkerSize(0.9);
      H_MET_jetveto[i]->SetMarkerSize(0.9);

    }
    for(Int_t jj=0; jj<nhisto; jj++){
      H_phoEB_ptcut[i][jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", jj));
      H_phoEB_Etacut[i][jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Etacut_%i", jj));
      H_phoEB_Phicut[i][jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Phicut_%i", jj));
      H_phoEB_ptoverMET_cut[i][jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverMET_cut_%i", jj));
      H_MET_cut[i][jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", jj));
      //H_MET_cut[i][jj]->Rebin(2);
      H_METPhi_cut[i][jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_cut_%i", jj));
      H_MIP_cut[i][jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_MIP_cut_%i", jj));
      H_dphoMETPhi_cut[i][jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", jj));
      H_nvtx_cut[i][jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", jj));
      H_njet_cut[i][jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", jj));
      H_nvtx_cut[i][jj]->Rebin(4);
      
      if(i==0){
	HA_phoEB_ptcut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", jj))->Clone();
	HA_phoEB_Etacut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Etacut_%i", jj))->Clone();
	HA_phoEB_Phicut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Phicut_%i", jj))->Clone();
	HA_phoEB_ptoverMET_cut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverMET_cut_%i", jj))->Clone();
	HA_MET_cut[jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", jj))->Clone();
	HA_METPhi_cut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_cut_%i", jj))->Clone();
	HA_MIP_cut[jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_MIP_cut_%i", jj))->Clone();
	HA_dphoMETPhi_cut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", jj))->Clone();
	HA_nvtx_cut[jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", jj))->Clone();
	HA_njet_cut[jj]		= (TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", jj))->Clone();
	//HA_nvtx_cut[jj]->Rebin(4);
	
	HA_phoEB_ptcut[jj]->Scale(scale[i]);	
	HA_phoEB_Etacut[jj]->Scale(scale[i]);	
	HA_phoEB_Phicut[jj]->Scale(scale[i]);	
	HA_phoEB_ptoverMET_cut[jj]->Scale(scale[i]);	
	HA_MET_cut[jj]->Scale(scale[i]);		
	HA_METPhi_cut[jj]->Scale(scale[i]);	
	HA_MIP_cut[jj]->Scale(scale[i]);		
	HA_dphoMETPhi_cut[jj]->Scale(scale[i]);	
	HA_nvtx_cut[jj]->Scale(scale[i]);		
	HA_njet_cut[jj]->Scale(scale[i]);

	HA_phoEB_ptcut[jj]->SetFillStyle(3145);	
	HA_phoEB_Etacut[jj]->SetFillStyle(3145);	
	HA_phoEB_Phicut[jj]->SetFillStyle(3145);	
	HA_phoEB_ptoverMET_cut[jj]->SetFillStyle(3145);	
	HA_MET_cut[jj]->SetFillStyle(3145);		
	HA_METPhi_cut[jj]->SetFillStyle(3145);	
	HA_MIP_cut[jj]->SetFillStyle(3145);		
	HA_dphoMETPhi_cut[jj]->SetFillStyle(3145);	
	HA_nvtx_cut[jj]->SetFillStyle(3145);		
	HA_njet_cut[jj]->SetFillStyle(3145);

	HA_phoEB_ptcut[jj]->SetFillColor(12);	
	HA_phoEB_Etacut[jj]->SetFillColor(12);	
	HA_phoEB_Phicut[jj]->SetFillColor(12);	
	HA_phoEB_ptoverMET_cut[jj]->SetFillColor(12);	
	HA_MET_cut[jj]->SetFillColor(12);		
	HA_METPhi_cut[jj]->SetFillColor(12);	
	HA_MIP_cut[jj]->SetFillColor(12);		
	HA_dphoMETPhi_cut[jj]->SetFillColor(12);	
	HA_nvtx_cut[jj]->SetFillColor(12);		
	HA_njet_cut[jj]->SetFillColor(12);
	
	HA_phoEB_ptcut[jj]->SetMarkerSize(0.1);	
	HA_phoEB_Etacut[jj]->SetMarkerSize(0.1);	
	HA_phoEB_Phicut[jj]->SetMarkerSize(0.1);	
	HA_phoEB_ptoverMET_cut[jj]->SetMarkerSize(0.1);	
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
	HSig_phoEB_ptoverMET_cut[jj]	= (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverMET_cut_%i", jj))->Clone();
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
	HSig_phoEB_ptoverMET_cut[jj]->Scale(scale[i]);	
	HSig_MET_cut[jj]->Scale(scale[i]);		
	HSig_METPhi_cut[jj]->Scale(scale[i]);	
	HSig_MIP_cut[jj]->Scale(scale[i]);		
	HSig_dphoMETPhi_cut[jj]->Scale(scale[i]);	
	HSig_nvtx_cut[jj]->Scale(scale[i]);		
	HSig_njet_cut[jj]->Scale(scale[i]);

	HSig_phoEB_ptcut[jj]->SetLineColor(kRed);	
	HSig_phoEB_Etacut[jj]->SetLineColor(kRed);	
	HSig_phoEB_Phicut[jj]->SetLineColor(kRed);	
	HSig_phoEB_ptoverMET_cut[jj]->SetLineColor(kRed);	
	HSig_MET_cut[jj]->SetLineColor(kRed);		
	HSig_METPhi_cut[jj]->SetLineColor(kRed);	
	HSig_MIP_cut[jj]->SetLineColor(kRed);		
	HSig_dphoMETPhi_cut[jj]->SetLineColor(kRed);	
	HSig_nvtx_cut[jj]->SetLineColor(kRed);		
	HSig_njet_cut[jj]->SetLineColor(kRed);

	HSig_phoEB_ptcut[jj]->SetLineWidth(2);	
	HSig_phoEB_Etacut[jj]->SetLineWidth(2);	
	HSig_phoEB_Phicut[jj]->SetLineWidth(2);	
	HSig_phoEB_ptoverMET_cut[jj]->SetLineWidth(2);	
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
      H_phoEB_ptoverMET_cut[i][jj]->Scale(scale[i]);	
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
	H_phoEB_ptoverMET_cut[i][jj]->SetFillColor(cnum[i]);	
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
      H_phoEB_ptoverMET_cut[i][jj]->SetLineColor(1);	
      H_MET_cut[i][jj]->SetLineColor(1);		
      H_METPhi_cut[i][jj]->SetLineColor(1);	
      H_MIP_cut[i][jj]->SetLineColor(1);		
      H_dphoMETPhi_cut[i][jj]->SetLineColor(1);	
      H_nvtx_cut[i][jj]->SetLineColor(1);		
      H_njet_cut[i][jj]->SetLineColor(1);

      H_phoEB_ptcut[i][jj]->SetLineWidth(1);	
      H_phoEB_Etacut[i][jj]->SetLineWidth(1);	
      H_phoEB_Phicut[i][jj]->SetLineWidth(1);	
      H_phoEB_ptoverMET_cut[i][jj]->SetLineWidth(1);	
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
	H_phoEB_ptoverMET_cut[i][jj]->SetLineWidth(2);	
	H_MET_cut[i][jj]->SetLineWidth(2);		
	H_METPhi_cut[i][jj]->SetLineWidth(2);	
	H_MIP_cut[i][jj]->SetLineWidth(2);		
	H_dphoMETPhi_cut[i][jj]->SetLineWidth(2);	
	H_nvtx_cut[i][jj]->SetLineWidth(2);		
	H_njet_cut[i][jj]->SetLineWidth(2);

	H_phoEB_ptcut[i][jj]->SetMarkerStyle(8);	
	H_phoEB_Etacut[i][jj]->SetMarkerStyle(8);	
	H_phoEB_Phicut[i][jj]->SetMarkerStyle(8);	
	H_phoEB_ptoverMET_cut[i][jj]->SetMarkerStyle(8);	
	H_MET_cut[i][jj]->SetMarkerStyle(8);		
	H_METPhi_cut[i][jj]->SetMarkerStyle(8);	
	H_MIP_cut[i][jj]->SetMarkerStyle(8);		
	H_dphoMETPhi_cut[i][jj]->SetMarkerStyle(8);	
	H_nvtx_cut[i][jj]->SetMarkerStyle(8);		
	H_njet_cut[i][jj]->SetMarkerStyle(8);

	H_phoEB_ptcut[i][jj]->SetMarkerSize(0.9);	
	H_phoEB_Etacut[i][jj]->SetMarkerSize(0.9);	
	H_phoEB_Phicut[i][jj]->SetMarkerSize(0.9);	
	H_phoEB_ptoverMET_cut[i][jj]->SetMarkerSize(0.9);	
	H_MET_cut[i][jj]->SetMarkerSize(0.9);		
	H_METPhi_cut[i][jj]->SetMarkerSize(0.9);	
	H_MIP_cut[i][jj]->SetMarkerSize(0.9);		
	H_dphoMETPhi_cut[i][jj]->SetMarkerSize(0.9);	
	H_nvtx_cut[i][jj]->SetMarkerSize(0.9);		
	H_njet_cut[i][jj]->SetMarkerSize(0.9);
      }
      
      for(Int_t kk=0; kk<njet; kk++){
	H_jetpt_cut[i][jj][kk]		= (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_cut_%i_jet%i", jj, kk));
	H_jetEta_cut[i][jj][kk]		= (TH1F*)fopen->Get(Form("h_jetpt/h_jetEta_cut_%i_jet%i", jj, kk));
	H_jetPhi_cut[i][jj][kk]		= (TH1F*)fopen->Get(Form("h_jetpt/h_jetPhi_cut_%i_jet%i", jj, kk));
	H_djetMETPhi_cut[i][jj][kk]	= (TH1F*)fopen->Get(Form("h_jetpt/h_djetMETPhi_cut_%i_jet%i", jj, kk));
	H_dr_phojet[i][jj][kk]		= (TH1F*)fopen->Get(Form("h_jetpt/h_dr_phojet_%i_jet%i", jj, kk));
	H_dEta_phojet[i][jj][kk]	= (TH1F*)fopen->Get(Form("h_jetpt/h_dEta_phojet_%i_jet%i", jj, kk));
	H_dPhi_phojet[i][jj][kk]	= (TH1F*)fopen->Get(Form("h_jetpt/h_dPhi_phojet_%i_jet%i", jj, kk));

	if(i==0){
	  HA_jetpt_cut[jj][kk]		= (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_cut_%i_jet%i", jj, kk))->Clone();
	  HA_jetEta_cut[jj][kk]		= (TH1F*)fopen->Get(Form("h_jetpt/h_jetEta_cut_%i_jet%i", jj, kk))->Clone();
	  HA_jetPhi_cut[jj][kk]		= (TH1F*)fopen->Get(Form("h_jetpt/h_jetPhi_cut_%i_jet%i", jj, kk))->Clone();
	  HA_djetMETPhi_cut[jj][kk]	= (TH1F*)fopen->Get(Form("h_jetpt/h_djetMETPhi_cut_%i_jet%i", jj, kk))->Clone();
	  HA_dr_phojet[jj][kk]		= (TH1F*)fopen->Get(Form("h_jetpt/h_dr_phojet_%i_jet%i", jj, kk))->Clone();
	  HA_dEta_phojet[jj][kk]	= (TH1F*)fopen->Get(Form("h_jetpt/h_dEta_phojet_%i_jet%i", jj, kk))->Clone();
	  HA_dPhi_phojet[jj][kk]	= (TH1F*)fopen->Get(Form("h_jetpt/h_dPhi_phojet_%i_jet%i", jj, kk))->Clone();

 	  HA_jetpt_cut[jj][kk]->Scale(scale[i]);		
	  HA_jetEta_cut[jj][kk]->Scale(scale[i]);		
	  HA_jetPhi_cut[jj][kk]->Scale(scale[i]);		
	  HA_djetMETPhi_cut[jj][kk]->Scale(scale[i]);	
	  HA_dr_phojet[jj][kk]->Scale(scale[i]);		
	  HA_dEta_phojet[jj][kk]->Scale(scale[i]);	
	  HA_dPhi_phojet[jj][kk]->Scale(scale[i]);

	  HA_jetpt_cut[jj][kk]->SetFillStyle(3145);		
	  HA_jetEta_cut[jj][kk]->SetFillStyle(3145);		
	  HA_jetPhi_cut[jj][kk]->SetFillStyle(3145);		
	  HA_djetMETPhi_cut[jj][kk]->SetFillStyle(3145);	
	  HA_dr_phojet[jj][kk]->SetFillStyle(3145);		
	  HA_dEta_phojet[jj][kk]->SetFillStyle(3145);	
	  HA_dPhi_phojet[jj][kk]->SetFillStyle(3145);

	  HA_jetpt_cut[jj][kk]->SetFillColor(12);		
	  HA_jetEta_cut[jj][kk]->SetFillColor(12);		
	  HA_jetPhi_cut[jj][kk]->SetFillColor(12);		
	  HA_djetMETPhi_cut[jj][kk]->SetFillColor(12);	
	  HA_dr_phojet[jj][kk]->SetFillColor(12);		
	  HA_dEta_phojet[jj][kk]->SetFillColor(12);	
	  HA_dPhi_phojet[jj][kk]->SetFillColor(12);

	  HA_jetpt_cut[jj][kk]->SetMarkerSize(0.1);		
	  HA_jetEta_cut[jj][kk]->SetMarkerSize(0.1);		
	  HA_jetPhi_cut[jj][kk]->SetMarkerSize(0.1);		
	  HA_djetMETPhi_cut[jj][kk]->SetMarkerSize(0.1);	
	  HA_dr_phojet[jj][kk]->SetMarkerSize(0.1);		
	  HA_dEta_phojet[jj][kk]->SetMarkerSize(0.1);	
	  HA_dPhi_phojet[jj][kk]->SetMarkerSize(0.1);
	}

	if(i==nfile-2){
	  HSig_jetpt_cut[jj][kk]		= (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_cut_%i_jet%i", jj, kk))->Clone();
	  HSig_jetEta_cut[jj][kk]		= (TH1F*)fopen->Get(Form("h_jetpt/h_jetEta_cut_%i_jet%i", jj, kk))->Clone();
	  HSig_jetPhi_cut[jj][kk]		= (TH1F*)fopen->Get(Form("h_jetpt/h_jetPhi_cut_%i_jet%i", jj, kk))->Clone();
	  HSig_djetMETPhi_cut[jj][kk]	= (TH1F*)fopen->Get(Form("h_jetpt/h_djetMETPhi_cut_%i_jet%i", jj, kk))->Clone();
	  HSig_dr_phojet[jj][kk]		= (TH1F*)fopen->Get(Form("h_jetpt/h_dr_phojet_%i_jet%i", jj, kk))->Clone();
	  HSig_dEta_phojet[jj][kk]	= (TH1F*)fopen->Get(Form("h_jetpt/h_dEta_phojet_%i_jet%i", jj, kk))->Clone();
	  HSig_dPhi_phojet[jj][kk]	= (TH1F*)fopen->Get(Form("h_jetpt/h_dPhi_phojet_%i_jet%i", jj, kk))->Clone();

 	  HSig_jetpt_cut[jj][kk]->Scale(scale[i]);		
	  HSig_jetEta_cut[jj][kk]->Scale(scale[i]);		
	  HSig_jetPhi_cut[jj][kk]->Scale(scale[i]);		
	  HSig_djetMETPhi_cut[jj][kk]->Scale(scale[i]);	
	  HSig_dr_phojet[jj][kk]->Scale(scale[i]);		
	  HSig_dEta_phojet[jj][kk]->Scale(scale[i]);	
	  HSig_dPhi_phojet[jj][kk]->Scale(scale[i]);
	  
	  HSig_jetpt_cut[jj][kk]->SetLineColor(kRed);		
	  HSig_jetEta_cut[jj][kk]->SetLineColor(kRed);		
	  HSig_jetPhi_cut[jj][kk]->SetLineColor(kRed);		
	  HSig_djetMETPhi_cut[jj][kk]->SetLineColor(kRed);	
	  HSig_dr_phojet[jj][kk]->SetLineColor(kRed);		
	  HSig_dEta_phojet[jj][kk]->SetLineColor(kRed);	
	  HSig_dPhi_phojet[jj][kk]->SetLineColor(kRed);

	  HSig_jetpt_cut[jj][kk]->SetLineWidth(2);		
	  HSig_jetEta_cut[jj][kk]->SetLineWidth(2);		
	  HSig_jetPhi_cut[jj][kk]->SetLineWidth(2);		
	  HSig_djetMETPhi_cut[jj][kk]->SetLineWidth(2);	
	  HSig_dr_phojet[jj][kk]->SetLineWidth(2);		
	  HSig_dEta_phojet[jj][kk]->SetLineWidth(2);	
	  HSig_dPhi_phojet[jj][kk]->SetLineWidth(2);
	  
	}
	
	H_jetpt_cut[i][jj][kk]->Scale(scale[i]);		
	H_jetEta_cut[i][jj][kk]->Scale(scale[i]);		
	H_jetPhi_cut[i][jj][kk]->Scale(scale[i]);		
	H_djetMETPhi_cut[i][jj][kk]->Scale(scale[i]);	
	H_dr_phojet[i][jj][kk]->Scale(scale[i]);		
	H_dEta_phojet[i][jj][kk]->Scale(scale[i]);	
	H_dPhi_phojet[i][jj][kk]->Scale(scale[i]);

	if(i<nfile-1){
	  H_jetpt_cut[i][jj][kk]->SetFillColor(cnum[i]);		
	  H_jetEta_cut[i][jj][kk]->SetFillColor(cnum[i]);		
	  H_jetPhi_cut[i][jj][kk]->SetFillColor(cnum[i]);		
	  H_djetMETPhi_cut[i][jj][kk]->SetFillColor(cnum[i]);	
	  H_dr_phojet[i][jj][kk]->SetFillColor(cnum[i]);		
	  H_dEta_phojet[i][jj][kk]->SetFillColor(cnum[i]);	
	  H_dPhi_phojet[i][jj][kk]->SetFillColor(cnum[i]);
	}

	H_jetpt_cut[i][jj][kk]->SetLineColor(1);		
	H_jetEta_cut[i][jj][kk]->SetLineColor(1);		
	H_jetPhi_cut[i][jj][kk]->SetLineColor(1);		
	H_djetMETPhi_cut[i][jj][kk]->SetLineColor(1);	
	H_dr_phojet[i][jj][kk]->SetLineColor(1);		
	H_dEta_phojet[i][jj][kk]->SetLineColor(1);	
	H_dPhi_phojet[i][jj][kk]->SetLineColor(1);

	H_jetpt_cut[i][jj][kk]->SetLineWidth(1);		
	H_jetEta_cut[i][jj][kk]->SetLineWidth(1);		
	H_jetPhi_cut[i][jj][kk]->SetLineWidth(1);		
	H_djetMETPhi_cut[i][jj][kk]->SetLineWidth(1);	
	H_dr_phojet[i][jj][kk]->SetLineWidth(1);		
	H_dEta_phojet[i][jj][kk]->SetLineWidth(1);	
	H_dPhi_phojet[i][jj][kk]->SetLineWidth(1);

	if(i == nfile-1){
	  H_jetpt_cut[i][jj][kk]->SetLineWidth(2);		
	  H_jetEta_cut[i][jj][kk]->SetLineWidth(2);		
	  H_jetPhi_cut[i][jj][kk]->SetLineWidth(2);		
	  H_djetMETPhi_cut[i][jj][kk]->SetLineWidth(2);	
	  H_dr_phojet[i][jj][kk]->SetLineWidth(2);		
	  H_dEta_phojet[i][jj][kk]->SetLineWidth(2);	
	  H_dPhi_phojet[i][jj][kk]->SetLineWidth(2);

	  H_jetpt_cut[i][jj][kk]->SetMarkerStyle(8);		
	  H_jetEta_cut[i][jj][kk]->SetMarkerStyle(8);		
	  H_jetPhi_cut[i][jj][kk]->SetMarkerStyle(8);		
	  H_djetMETPhi_cut[i][jj][kk]->SetMarkerStyle(8);	
	  H_dr_phojet[i][jj][kk]->SetMarkerStyle(8);		
	  H_dEta_phojet[i][jj][kk]->SetMarkerStyle(8);	
	  H_dPhi_phojet[i][jj][kk]->SetMarkerStyle(8);

	  H_jetpt_cut[i][jj][kk]->SetMarkerSize(0.9);		
	  H_jetEta_cut[i][jj][kk]->SetMarkerSize(0.9);		
	  H_jetPhi_cut[i][jj][kk]->SetMarkerSize(0.9);		
	  H_djetMETPhi_cut[i][jj][kk]->SetMarkerSize(0.9);	
	  H_dr_phojet[i][jj][kk]->SetMarkerSize(0.9);		
	  H_dEta_phojet[i][jj][kk]->SetMarkerSize(0.9);	
	  H_dPhi_phojet[i][jj][kk]->SetMarkerSize(0.9);
	}
      }
      
    }
  }

  for(Int_t i=0; i<nfile-1; i++){
    //if(i==10) continue;
    if(i>0){
      HA_dr_jetjet->Add(H_dr_jetjet[i]);
      HA_dEta_jetjet->Add(H_dEta_jetjet[i]);
      HA_dPhi_jetjet->Add(H_dPhi_jetjet[i]);
      HA_dijetMass_cut->Add(H_dijetMass_cut[i]);
      HA_MET_phojetdR->Add(H_MET_phojetdR[i]);
      HA_MET_jetjetdEta->Add(H_MET_jetjetdEta[i]);
      HA_MET_jetjetdR->Add(H_MET_jetjetdR[i]);
      HA_MET_dijetMass->Add(H_MET_dijetMass[i]);
      HA_MET_jetveto->Add(H_MET_jetveto[i]);

      for(Int_t jj=0; jj<2; jj++){
	HA_phoEB_ptcut[jj]->Add(H_phoEB_ptcut[i][jj]);
	HA_phoEB_Etacut[jj]->Add(H_phoEB_Etacut[i][jj]);
	HA_phoEB_Phicut[jj]->Add(H_phoEB_Phicut[i][jj]);
	HA_phoEB_ptoverMET_cut[jj]->Add(H_phoEB_ptoverMET_cut[i][jj]);
	HA_MET_cut[jj]->Add(H_MET_cut[i][jj]);
	HA_METPhi_cut[jj]->Add(H_METPhi_cut[i][jj]);
	HA_MIP_cut[jj]->Add(H_MIP_cut[i][jj]);
	HA_dphoMETPhi_cut[jj]->Add(H_dphoMETPhi_cut[i][jj]);
	HA_nvtx_cut[jj]->Add(H_nvtx_cut[i][jj]);
	HA_njet_cut[jj]->Add(H_njet_cut[i][jj]);
	for(Int_t kk=0; kk<njet; kk++){
	  HA_jetpt_cut[jj][kk]->Add(H_jetpt_cut[i][jj][kk]);
	  HA_jetEta_cut[jj][kk]->Add(H_jetEta_cut[i][jj][kk]);
	  HA_jetPhi_cut[jj][kk]->Add(H_jetPhi_cut[i][jj][kk]);
	  HA_djetMETPhi_cut[jj][kk]->Add(H_djetMETPhi_cut[i][jj][kk]);
	  HA_dr_phojet[jj][kk]->Add(H_dr_phojet[i][jj][kk]);
	  HA_dEta_phojet[jj][kk]->Add(H_dEta_phojet[i][jj][kk]);
	  HA_dPhi_phojet[jj][kk]->Add(H_dPhi_phojet[i][jj][kk]);
	}
      }
    }
  }

  THStack *HS_phoEB_ptcut[nhisto];		 
  THStack *HS_phoEB_Etacut[nhisto];		 
  THStack *HS_phoEB_Phicut[nhisto];		 
  THStack *HS_phoEB_ptoverMET_cut[nhisto];	 
  THStack *HS_MET_cut[nhisto];		 
  THStack *HS_METPhi_cut[nhisto];		 
  THStack *HS_MIP_cut[nhisto];		 
  THStack *HS_dphoMETPhi_cut[nhisto];	 
  THStack *HS_nvtx_cut[nhisto];		 
  THStack *HS_njet_cut[nhisto];		 

  THStack *HS_jetpt_cut[nhisto][njet];
  THStack *HS_jetEta_cut[nhisto][njet];
  THStack *HS_jetPhi_cut[nhisto][njet];
  THStack *HS_djetMETPhi_cut[nhisto][njet];
  THStack *HS_dr_phojet[nhisto][njet];
  THStack *HS_dEta_phojet[nhisto][njet];
  THStack *HS_dPhi_phojet[nhisto][njet];
  
  THStack *HS_dr_jetjet;
  THStack *HS_dEta_jetjet;
  THStack *HS_dPhi_jetjet;
  THStack *HS_dijetMass_cut;

  THStack *HS_MET_phojetdR;
  THStack *HS_MET_jetjetdEta;
  THStack *HS_MET_jetjetdR;
  THStack *HS_MET_dijetMass;
  THStack *HS_MET_jetveto;
  
  HS_dr_jetjet		= new THStack("HS_dr_jetjet", "HS_dr_jetjet");
  HS_dEta_jetjet	= new THStack("HS_dEta_jetjet", "HS_dEta_jetjet");
  HS_dPhi_jetjet	= new THStack("HS_dPhi_jetjet", "HS_dPhi_jetjet");
  HS_dijetMass_cut		= new THStack("HS_dijetMass_cut", "HS_dijetMass_cut");

  HS_MET_phojetdR = new THStack("HS_MET_phojetdR", "HS_MET_phojetdR");
  HS_MET_jetjetdEta = new THStack("HS_MET_jetjetdEta", "HS_MET_jetjetdEta");
  HS_MET_jetjetdR = new THStack("HS_MET_jetjetdR", "HS_MET_jetjetdR");
  HS_MET_dijetMass = new THStack("HS_MET_dijetMass", "HS_MET_dijetMass");
  HS_MET_jetveto = new THStack("HS_MET_jetveto", "HS_MET_jetveto");

  
  for(Int_t jj=0; jj<2; jj++){
    HS_phoEB_ptcut[jj]		= new THStack(Form("HS_phoEB_ptcut_%i", jj), "HS_phoEB_ptcut");		 
    HS_phoEB_Etacut[jj]		= new THStack(Form("HS_phoEB_Etacut_%i", jj), "HS_phoEB_Etacut");		 
    HS_phoEB_Phicut[jj]		= new THStack(Form("HS_phoEB_Phicut_%i", jj), "HS_phoEB_Phicut");		 
    HS_phoEB_ptoverMET_cut[jj]	= new THStack(Form("HS_phoEB_ptoverMET_cut_%i", jj), "HS_phoEB_ptoverMET_cut");	 
    HS_MET_cut[jj]		= new THStack(Form("HS_MET_cut_%i", jj), "HS_MET_cut");		 
    HS_METPhi_cut[jj]		= new THStack(Form("HS_METPhi_cut_%i", jj), "HS_METPhi_cut");		 
    HS_MIP_cut[jj]		= new THStack(Form("HS_MIP_cut_%i", jj), "HS_MIP_cut");		 
    HS_dphoMETPhi_cut[jj]	= new THStack(Form("HS_dphoMETPhi_cut_%i", jj), "HS_dphoMETPhi_cut");	 
    HS_nvtx_cut[jj]		= new THStack(Form("HS_nvtx_cut_%i", jj), "HS_nvtx_cut");		 
    HS_njet_cut[jj]		= new THStack(Form("HS_njet_cut_%i", jj), "HS_njet_cut");
    
    for(Int_t kk=0; kk<njet; kk++){
      HS_jetpt_cut[jj][kk]	= new THStack(Form("HS_jetpt_cut_%i_jet%i", jj, kk), "HS_jetpt_cut");
      HS_jetEta_cut[jj][kk]	= new THStack(Form("HS_jetEta_cut_%i_jet%i", jj, kk), "HS_jetEta_cut");
      HS_jetPhi_cut[jj][kk]	= new THStack(Form("HS_jetPhi_cut_%i_jet%i", jj, kk), "HS_jetPhi_cut");
      HS_djetMETPhi_cut[jj][kk] = new THStack(Form("HS_djetMETPhi_cut_%i_jet%i", jj, kk), "HS_djetMETPhi_cut");
      HS_dr_phojet[jj][kk]	= new THStack(Form("HS_dr_phojet_%i_jet%i", jj, kk), "HS_dr_phojet");
      HS_dEta_phojet[jj][kk]	= new THStack(Form("HS_dEta_phojet_%i_jet%i", jj, kk), "HS_dEta_phojet");
      HS_dPhi_phojet[jj][kk]	= new THStack(Form("HS_dPhi_phojet_%i_jet%i", jj, kk), "HS_dPhi_phojet");
    }
  }
					       
  
  for(Int_t i=0; i<nfile-1; i++){
    //if(i==10) continue;
    HS_dr_jetjet->Add(H_dr_jetjet[i]);	
    HS_dEta_jetjet->Add(H_dEta_jetjet[i]);	
    HS_dPhi_jetjet->Add(H_dPhi_jetjet[i]);	
    HS_dijetMass_cut->Add(H_dijetMass_cut[i]);

    HS_MET_phojetdR->Add(H_MET_phojetdR[i]);
    HS_MET_jetjetdEta->Add(H_MET_jetjetdEta[i]);
    HS_MET_jetjetdR->Add(H_MET_jetjetdR[i]);
    HS_MET_dijetMass->Add(H_MET_dijetMass[i]);
    HS_MET_jetveto->Add(H_MET_jetveto[i]);

    for(Int_t jj=0; jj<nhisto; jj++){
      HS_phoEB_ptcut[jj]->Add(H_phoEB_ptcut[i][jj]);	
      HS_phoEB_Etacut[jj]->Add(H_phoEB_Etacut[i][jj]);	
      HS_phoEB_Phicut[jj]->Add(H_phoEB_Phicut[i][jj]);	
      HS_phoEB_ptoverMET_cut[jj]->Add(H_phoEB_ptoverMET_cut[i][jj]);	
      HS_MET_cut[jj]->Add(H_MET_cut[i][jj]);		
      HS_METPhi_cut[jj]->Add(H_METPhi_cut[i][jj]);		
      HS_MIP_cut[jj]->Add(H_MIP_cut[i][jj]);		
      HS_nvtx_cut[jj]->Add(H_nvtx_cut[i][jj]);		
      HS_njet_cut[jj]->Add(H_njet_cut[i][jj]);

      
      Double_t ufbin = H_dphoMETPhi_cut[i][jj]->GetBinContent(0) + H_dphoMETPhi_cut[i][jj]->GetBinContent(1);
      Double_t ofbin = H_dphoMETPhi_cut[i][jj]->GetBinContent(31) + H_dphoMETPhi_cut[i][jj]->GetBinContent(30);
      H_dphoMETPhi_cut[i][jj]->SetBinContent(1,ufbin);
      H_dphoMETPhi_cut[i][jj]->SetBinContent(30,ofbin);
      c1->SetLogy();
      c1->Update();
      HS_dphoMETPhi_cut[jj]->Add(H_dphoMETPhi_cut[i][jj]);
      c1->SetLogy(0);
      c1->Update();
      
      for(Int_t kk=0; kk<nhisto; kk++){
	HS_jetpt_cut[jj][kk]->Add(H_jetpt_cut[i][jj][kk]);	
	HS_jetEta_cut[jj][kk]->Add(H_jetEta_cut[i][jj][kk]);	
	HS_jetPhi_cut[jj][kk]->Add(H_jetPhi_cut[i][jj][kk]);	
	
	HS_dr_phojet[jj][kk]->Add(H_dr_phojet[i][jj][kk]);	
	HS_dEta_phojet[jj][kk]->Add(H_dEta_phojet[i][jj][kk]);	
	HS_dPhi_phojet[jj][kk]->Add(H_dPhi_phojet[i][jj][kk]);
	Double_t ufbin = H_djetMETPhi_cut[i][jj][kk]->GetBinContent(0) + H_djetMETPhi_cut[i][jj][kk]->GetBinContent(1);
	Double_t ofbin = H_djetMETPhi_cut[i][jj][kk]->GetBinContent(31) + H_djetMETPhi_cut[i][jj][kk]->GetBinContent(30);
	H_djetMETPhi_cut[i][jj][kk]->SetBinContent(1, ufbin);
	H_djetMETPhi_cut[i][jj][kk]->SetBinContent(30, ofbin);
	c1->SetLogy();
	c1->Update();
	HS_djetMETPhi_cut[jj][kk]->Add(H_djetMETPhi_cut[i][jj][kk]);
	c1->SetLogy(0);
	c1->Update();
      }
      
           
    }
  }
  

  //TLegend *lhs = new TLegend(0.6,0.7, 0.92, 0.90);
  TLegend *lhs = new TLegend(0.52, 0.66, 0.92, 0.90);//VBS
  lhs->SetTextSize(0.035);
  lhs->SetFillStyle(0);
  //TString ltext[20] = {"#gamma+jet", "W(#rightarrowl#nu)+jets", "Z(#rightarrow#nu#nu)+jets", "Z(#rightarrowll)+jets", "Monoboson", "Diboson", "Triboson", "Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug", "DATA"};//Znunug QCD
  TString ltext[20] = {"#gamma+jet", "W(#rightarrowl#nu)+jets", "Z(#rightarrow#nu#nu)+jets", "Z(#rightarrowll)+jets", "Monoboson", "Diboson", "Triboson", "Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug(VBS)", "DATA"};//Znunug VBS
  
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

  pad1->cd();
  HS_dr_jetjet->Draw("HIST");
  H_dr_jetjet[nfile-1]->Draw("SAME");
  HSig_dr_jetjet->Draw("HISTSAME");
  HA_dr_jetjet->Draw("E2 SAME");
  HS_dr_jetjet->GetYaxis()->SetTitle("Events");
  HS_dr_jetjet->GetXaxis()->SetTitle("");
  //HS_dr_jetjet->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_dr_jetjet->GetXaxis()->SetLabelSize(0);
  HS_dr_jetjet->SetMaximum(10000);
  HS_dr_jetjet->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_dr_jetjet[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dr_jetjet, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dr_jetjet[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_dr_jetjet->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dr_jetjet, H_dr_jetjet[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#DeltaR(jet , jet)");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  pad1->cd();
  HS_dEta_jetjet->Draw("HIST");
  H_dEta_jetjet[nfile-1]->Draw("SAME");
  HSig_dEta_jetjet->Draw("HISTSAME");
  HA_dEta_jetjet->Draw("E2 SAME");
  HS_dEta_jetjet->GetYaxis()->SetTitle("Events");
  HS_dEta_jetjet->GetXaxis()->SetTitle("");
  //HS_dEta_jetjet->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_dEta_jetjet->GetXaxis()->SetLabelSize(0);
  HS_dEta_jetjet->SetMaximum(8000);
  HS_dEta_jetjet->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_dEta_jetjet[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dEta_jetjet, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dEta_jetjet[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_dEta_jetjet->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dEta_jetjet, H_dEta_jetjet[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#Delta#eta(jet , jet)");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_dPhi_jetjet->Draw("HIST");
  H_dPhi_jetjet[nfile-1]->Draw("SAME");
  HSig_dPhi_jetjet->Draw("HISTSAME");
  HA_dPhi_jetjet->Draw("E2 SAME");
  HS_dPhi_jetjet->GetYaxis()->SetTitle("Events");
  HS_dPhi_jetjet->GetXaxis()->SetTitle("");
  //HS_dPhi_jetjet->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_dPhi_jetjet->GetXaxis()->SetLabelSize(0);
  HS_dPhi_jetjet->SetMaximum(8000);
  HS_dPhi_jetjet->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_dPhi_jetjet[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dPhi_jetjet, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dPhi_jetjet[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_dPhi_jetjet->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dPhi_jetjet, H_dPhi_jetjet[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("#Delta#phi(jet , jet)");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  
  pad1->cd();
  HS_dijetMass_cut->Draw("HIST");
  H_dijetMass_cut[nfile-1]->Draw("SAME");
  HSig_dijetMass_cut->Draw("HISTSAME");
  HA_dijetMass_cut->Draw("E2 SAME");
  HS_dijetMass_cut->GetYaxis()->SetTitle("Events");
  HS_dijetMass_cut->GetXaxis()->SetTitle("");
  //HS_dijetMass_cut->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_dijetMass_cut->GetXaxis()->SetLabelSize(0);
  HS_dijetMass_cut->SetMaximum(1000);
  HS_dijetMass_cut->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_dijetMass_cut[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_dijetMass_cut, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_dijetMass_cut[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_dijetMass_cut->GetName();
  pad2->cd();
  hratio = ratioplot(HS_dijetMass_cut, H_dijetMass_cut[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("Mass^{jj}[GeV]");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_MET_phojetdR->Draw("HIST");
  H_MET_phojetdR[nfile-1]->Draw("SAME");
  HSig_MET_phojetdR->Draw("HISTSAME");
  HA_MET_phojetdR->Draw("E2 SAME");
  HS_MET_phojetdR->GetYaxis()->SetTitle("Events");
  HS_MET_phojetdR->GetXaxis()->SetTitle("");
  //HS_MET_phojetdR->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_MET_phojetdR->GetXaxis()->SetLabelSize(0);
  HS_MET_phojetdR->SetMaximum(10000);
  HS_MET_phojetdR->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_MET_phojetdR[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_MET_phojetdR, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_MET_phojetdR[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_MET_phojetdR->GetName();
  pad2->cd();
  hratio = ratioplot(HS_MET_phojetdR, H_MET_phojetdR[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("MET");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_MET_jetjetdEta->Draw("HIST");
  H_MET_jetjetdEta[nfile-1]->Draw("SAME");
  HSig_MET_jetjetdEta->Draw("HISTSAME");
  HA_MET_jetjetdEta->Draw("E2 SAME");
  HS_MET_jetjetdEta->GetYaxis()->SetTitle("Events");
  HS_MET_jetjetdEta->GetXaxis()->SetTitle("");
  //HS_MET_jetjetdEta->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_MET_jetjetdEta->GetXaxis()->SetLabelSize(0);
  HS_MET_jetjetdEta->SetMaximum(10000);
  HS_MET_jetjetdEta->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_MET_jetjetdEta[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_MET_jetjetdEta, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_MET_jetjetdEta[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_MET_jetjetdEta->GetName();
  pad2->cd();
  hratio = ratioplot(HS_MET_jetjetdEta, H_MET_jetjetdEta[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("MET");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_MET_jetjetdR->Draw("HIST");
  H_MET_jetjetdR[nfile-1]->Draw("SAME");
  HSig_MET_jetjetdR->Draw("HISTSAME");
  HA_MET_jetjetdR->Draw("E2 SAME");
  HS_MET_jetjetdR->GetYaxis()->SetTitle("Events");
  HS_MET_jetjetdR->GetXaxis()->SetTitle("");
  //HS_MET_jetjetdR->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_MET_jetjetdR->GetXaxis()->SetLabelSize(0);
  HS_MET_jetjetdR->SetMaximum(10000);
  HS_MET_jetjetdR->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_MET_jetjetdR[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_MET_jetjetdR, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_MET_jetjetdR[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_MET_jetjetdR->GetName();
  pad2->cd();
  hratio = ratioplot(HS_MET_jetjetdR, H_MET_jetjetdR[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("MET");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_MET_dijetMass->Draw("HIST");
  H_MET_dijetMass[nfile-1]->Draw("SAME");
  HSig_MET_dijetMass->Draw("HISTSAME");
  HA_MET_dijetMass->Draw("E2 SAME");
  HS_MET_dijetMass->GetYaxis()->SetTitle("Events");
  HS_MET_dijetMass->GetXaxis()->SetTitle("");
  //HS_MET_dijetMass->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_MET_dijetMass->GetXaxis()->SetLabelSize(0);
  HS_MET_dijetMass->SetMaximum(10000);
  HS_MET_dijetMass->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_MET_dijetMass[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_MET_dijetMass, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_MET_dijetMass[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_MET_dijetMass->GetName();
  pad2->cd();
  hratio = ratioplot(HS_MET_dijetMass, H_MET_dijetMass[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("MET");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  pad1->cd();
  HS_MET_jetveto->Draw("HIST");
  H_MET_jetveto[nfile-1]->Draw("SAME");
  HSig_MET_jetveto->Draw("HISTSAME");
  HA_MET_jetveto->Draw("E2 SAME");
  HS_MET_jetveto->GetYaxis()->SetTitle("Events");
  HS_MET_jetveto->GetXaxis()->SetTitle("");
  //HS_MET_jetveto->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
  HS_MET_jetveto->GetXaxis()->SetLabelSize(0);
  HS_MET_jetveto->SetMaximum(10000);
  HS_MET_jetveto->SetMinimum(1);
  lhs->Clear();
  lhs->SetNColumns(2);
  for(Int_t ifile=0; ifile<nfile; ifile++){
    //if(ifile==10) continue;
    if(ifile == nfile-1) lhs->AddEntry(H_MET_jetveto[ifile], ltext[ifile], "PL");
    if(ifile == nfile-2) lhs->AddEntry(HSig_MET_jetveto, ltext[ifile], "L");
    if(ifile < nfile-1) lhs->AddEntry(H_MET_jetveto[ifile], ltext[ifile], "F");
  }
  lhs->Draw("SAME");
  title = HS_MET_jetveto->GetName();
  pad2->cd();
  hratio = ratioplot(HS_MET_jetveto, H_MET_jetveto[nfile-1]);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("MET");
  hratio->Draw("EP");
  tg->Draw("LSAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  for(Int_t jj=0; jj<nhisto; jj++){
    
    pad1->cd();
    HS_phoEB_ptcut[jj]->Draw("HIST");
    H_phoEB_ptcut[nfile-1][jj]->Draw("SAME");
    HSig_phoEB_ptcut[jj]->Draw("HISTSAME");
    HA_phoEB_ptcut[jj]->Draw("E2 SAME");
    HS_phoEB_ptcut[jj]->GetYaxis()->SetTitle("Events");
    HS_phoEB_ptcut[jj]->GetXaxis()->SetTitle("");
    //HS_phoEB_ptcut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_ptcut[jj]->GetXaxis()->SetLabelSize(0);
    HS_phoEB_ptcut[jj]->SetMaximum(10000);
    HS_phoEB_ptcut[jj]->SetMinimum(1);
    if(jj==1){HS_phoEB_ptcut[jj]->SetMaximum(1000); HS_phoEB_ptcut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      if(ifile == nfile-1) lhs->AddEntry(H_phoEB_ptcut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_phoEB_ptcut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_phoEB_ptcut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_phoEB_ptcut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_ptcut[jj], H_phoEB_ptcut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("P_{T}^{#gamma}[GeV]");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    
    pad1->cd();
    HS_phoEB_Etacut[jj]->Draw("HIST");
    H_phoEB_Etacut[nfile-1][jj]->Draw("SAME");
    HSig_phoEB_Etacut[jj]->Draw("HISTSAME");
    HA_phoEB_Etacut[jj]->Draw("E2 SAME");
    HS_phoEB_Etacut[jj]->GetYaxis()->SetTitle("Events");
    HS_phoEB_Etacut[jj]->GetXaxis()->SetTitle("");
    //HS_phoEB_Etacut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_Etacut[jj]->GetXaxis()->SetLabelSize(0);
    HS_phoEB_Etacut[jj]->SetMaximum(100000);
    HS_phoEB_Etacut[jj]->SetMinimum(1);
    if(jj==1){HS_phoEB_Etacut[jj]->SetMaximum(1000); HS_phoEB_Etacut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      if(ifile == nfile-1) lhs->AddEntry(H_phoEB_Etacut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_phoEB_Etacut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_phoEB_Etacut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_phoEB_Etacut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_Etacut[jj], H_phoEB_Etacut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("|#eta^{#gamma}|");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_phoEB_Phicut[jj]->Draw("HIST");
    H_phoEB_Phicut[nfile-1][jj]->Draw("SAME");
    HSig_phoEB_Phicut[jj]->Draw("HISTSAME");
    HA_phoEB_Phicut[jj]->Draw("E2 SAME");
    HS_phoEB_Phicut[jj]->GetYaxis()->SetTitle("Events");
    HS_phoEB_Phicut[jj]->GetXaxis()->SetTitle("");
    //HS_phoEB_Phicut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_Phicut[jj]->GetXaxis()->SetLabelSize(0);
    HS_phoEB_Phicut[jj]->SetMaximum(80000);
    HS_phoEB_Phicut[jj]->SetMinimum(1);
    if(jj==1){HS_phoEB_Phicut[jj]->SetMaximum(1000); HS_phoEB_Phicut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      if(ifile == nfile-1) lhs->AddEntry(H_phoEB_Phicut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_phoEB_Phicut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_phoEB_Phicut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_phoEB_Phicut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_Phicut[jj], H_phoEB_Phicut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#phi^{#gamma}");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_phoEB_ptoverMET_cut[jj]->Draw("HIST");
    H_phoEB_ptoverMET_cut[nfile-1][jj]->Draw("SAME");
    HSig_phoEB_ptoverMET_cut[jj]->Draw("HISTSAME");
    HA_phoEB_ptoverMET_cut[jj]->Draw("E2 SAME");
    HS_phoEB_ptoverMET_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_phoEB_ptoverMET_cut[jj]->GetXaxis()->SetTitle("");
    //HS_phoEB_ptoverMET_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_ptoverMET_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_phoEB_ptoverMET_cut[jj]->SetMaximum(80000);
    HS_phoEB_ptoverMET_cut[jj]->SetMinimum(1);
    if(jj==1){HS_phoEB_ptoverMET_cut[jj]->SetMaximum(1000); HS_phoEB_ptoverMET_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      if(ifile == nfile-1) lhs->AddEntry(H_phoEB_ptoverMET_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_phoEB_ptoverMET_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_phoEB_ptoverMET_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_phoEB_ptoverMET_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_ptoverMET_cut[jj], H_phoEB_ptoverMET_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("p_{T}^{#gamma}/E_{T}^{miss}");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_MET_cut[jj]->Draw("HIST");
    H_MET_cut[nfile-1][jj]->Draw("SAME");
    HSig_MET_cut[jj]->Draw("HISTSAME");
    HA_MET_cut[jj]->Draw("E2 SAME");
    HS_MET_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_MET_cut[jj]->GetXaxis()->SetTitle("");
    //HS_MET_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_MET_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_MET_cut[jj]->SetMaximum(10000);
    HS_MET_cut[jj]->SetMinimum(1);
    if(jj==1){HS_MET_cut[jj]->SetMaximum(1000); HS_MET_cut[jj]->SetMinimum(0.01);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      if(ifile == nfile-1) lhs->AddEntry(H_MET_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_MET_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_MET_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_MET_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_MET_cut[jj], H_MET_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("E_{T}^{miss}");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    
    pad1->cd();
    HS_METPhi_cut[jj]->Draw("HIST");
    H_METPhi_cut[nfile-1][jj]->Draw("SAME");
    HSig_METPhi_cut[jj]->Draw("HISTSAME");
    HA_METPhi_cut[jj]->Draw("E2 SAME");
    HS_METPhi_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_METPhi_cut[jj]->GetXaxis()->SetTitle("");
    //HS_METPhi_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_METPhi_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_METPhi_cut[jj]->SetMaximum(10000);
    HS_METPhi_cut[jj]->SetMinimum(1);
    if(jj==1){HS_METPhi_cut[jj]->SetMaximum(1000); HS_METPhi_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      if(ifile == nfile-1) lhs->AddEntry(H_METPhi_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_METPhi_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_METPhi_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_METPhi_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_METPhi_cut[jj], H_METPhi_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#phi^{MET}");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_MIP_cut[jj]->Draw("HIST");
    H_MIP_cut[nfile-1][jj]->Draw("SAME");
    HSig_MIP_cut[jj]->Draw("HISTSAME");
    HA_MIP_cut[jj]->Draw("E2 SAME");
    HS_MIP_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_MIP_cut[jj]->GetXaxis()->SetTitle("");
    //HS_MIP_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_MIP_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_MIP_cut[jj]->SetMaximum(10000);
    HS_MIP_cut[jj]->SetMinimum(1);
    if(jj==1){HS_MIP_cut[jj]->SetMaximum(1000); HS_MIP_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      if(ifile == nfile-1) lhs->AddEntry(H_MIP_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_MIP_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_MIP_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_MIP_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_MIP_cut[jj], H_MIP_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("MIP[GeV]");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_dphoMETPhi_cut[jj]->Draw("HIST");
    H_dphoMETPhi_cut[nfile-1][jj]->Draw("SAME");
    HSig_dphoMETPhi_cut[jj]->Draw("HISTSAME");
    HA_dphoMETPhi_cut[jj]->Draw("E2 SAME");
    HS_dphoMETPhi_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_dphoMETPhi_cut[jj]->GetXaxis()->SetTitle("");
    //HS_dphoMETPhi_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_dphoMETPhi_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_dphoMETPhi_cut[jj]->SetMaximum(10000);
    HS_dphoMETPhi_cut[jj]->SetMinimum(1);
    if(jj==1){HS_dphoMETPhi_cut[jj]->SetMaximum(1000); HS_dphoMETPhi_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      if(ifile == nfile-1) lhs->AddEntry(H_dphoMETPhi_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dphoMETPhi_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dphoMETPhi_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_dphoMETPhi_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dphoMETPhi_cut[jj], H_dphoMETPhi_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#Delta#phi(#gamma , E_{T}^{miss})");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_nvtx_cut[jj]->Draw("HIST");
    H_nvtx_cut[nfile-1][jj]->Draw("SAME");
    HSig_nvtx_cut[jj]->Draw("HISTSAME");
    HA_nvtx_cut[jj]->Draw("E2 SAME");
    HS_nvtx_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_nvtx_cut[jj]->GetXaxis()->SetTitle("");
    //HS_nvtx_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_nvtx_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_nvtx_cut[jj]->SetMaximum(10000);
    HS_nvtx_cut[jj]->SetMinimum(1);
    if(jj==1){HS_nvtx_cut[jj]->SetMaximum(1000); HS_nvtx_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      if(ifile == nfile-1) lhs->AddEntry(H_nvtx_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_nvtx_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_nvtx_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_nvtx_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_nvtx_cut[jj], H_nvtx_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("Nvtx");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_njet_cut[jj]->Draw("HIST");
    H_njet_cut[nfile-1][jj]->Draw("SAME");
    HSig_njet_cut[jj]->Draw("HISTSAME");
    HA_njet_cut[jj]->Draw("E2 SAME");
    HS_njet_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_njet_cut[jj]->GetXaxis()->SetTitle("");
    //HS_njet_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_njet_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_njet_cut[jj]->SetMaximum(80000);
    HS_njet_cut[jj]->SetMinimum(1);
    if(jj==1){HS_njet_cut[jj]->SetMaximum(1000); HS_njet_cut[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      //if(ifile==10) continue;
      if(ifile == nfile-1) lhs->AddEntry(H_njet_cut[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_njet_cut[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_njet_cut[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_njet_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_njet_cut[jj], H_njet_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("Njet");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    for(Int_t kk=0; kk<nhisto; kk++){

      pad1->cd();
      HS_jetpt_cut[jj][kk]->Draw("HIST");
      H_jetpt_cut[nfile-1][jj][kk]->Draw("SAME");
      HSig_jetpt_cut[jj][kk]->Draw("HISTSAME");
      HA_jetpt_cut[jj][kk]->Draw("E2 SAME");
      HS_jetpt_cut[jj][kk]->GetYaxis()->SetTitle("Events");
      HS_jetpt_cut[jj][kk]->GetXaxis()->SetTitle("");
      //HS_jetpt_cut[jj][kk]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
      HS_jetpt_cut[jj][kk]->GetXaxis()->SetLabelSize(0);
      HS_jetpt_cut[jj][kk]->SetMaximum(10000);
      HS_jetpt_cut[jj][kk]->SetMinimum(1);
      if(jj==1){HS_jetpt_cut[jj][kk]->SetMaximum(1000); HS_jetpt_cut[jj][kk]->SetMinimum(0.1);}
      lhs->Clear();
      lhs->SetNColumns(2);
      for(Int_t ifile=0; ifile<nfile; ifile++){
	//if(ifile==10) continue;
	if(ifile == nfile-1) lhs->AddEntry(H_jetpt_cut[ifile][jj][kk], ltext[ifile], "PL");
	if(ifile == nfile-2) lhs->AddEntry(HSig_jetpt_cut[jj][kk], ltext[ifile], "L");
	if(ifile < nfile-1) lhs->AddEntry(H_jetpt_cut[ifile][jj][kk], ltext[ifile], "F");
      }
      lhs->Draw("SAME");
      title = HS_jetpt_cut[jj][kk]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_jetpt_cut[jj][kk], H_jetpt_cut[nfile-1][jj][kk]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle(Form("P_{T}^{jet%i}[GeV]", kk));
      hratio->Draw("EP");
      tg->Draw("LSAME");
      CMS_lumi(pad1, period, 0);
      c1->Update();
      c1->RedrawAxis();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));

      pad1->cd();
      HS_jetEta_cut[jj][kk]->Draw("HIST");
      H_jetEta_cut[nfile-1][jj][kk]->Draw("SAME");
      HSig_jetEta_cut[jj][kk]->Draw("HISTSAME");
      HA_jetEta_cut[jj][kk]->Draw("E2 SAME");
      HS_jetEta_cut[jj][kk]->GetYaxis()->SetTitle("Events");
      HS_jetEta_cut[jj][kk]->GetXaxis()->SetTitle("");
      //HS_jetEta_cut[jj][kk]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
      HS_jetEta_cut[jj][kk]->GetXaxis()->SetLabelSize(0);
      HS_jetEta_cut[jj][kk]->SetMaximum(10000);
      HS_jetEta_cut[jj][kk]->SetMinimum(1);
      if(jj==1){HS_jetEta_cut[jj][kk]->SetMaximum(1000); HS_jetEta_cut[jj][kk]->SetMinimum(0.1);}
      lhs->Clear();
      lhs->SetNColumns(2);
      for(Int_t ifile=0; ifile<nfile; ifile++){
	//if(ifile==10) continue;
	if(ifile == nfile-1) lhs->AddEntry(H_jetEta_cut[ifile][jj][kk], ltext[ifile], "PL");
	if(ifile == nfile-2) lhs->AddEntry(HSig_jetEta_cut[jj][kk], ltext[ifile], "L");
	if(ifile < nfile-1) lhs->AddEntry(H_jetEta_cut[ifile][jj][kk], ltext[ifile], "F");
      }
      lhs->Draw("SAME");
      title = HS_jetEta_cut[jj][kk]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_jetEta_cut[jj][kk], H_jetEta_cut[nfile-1][jj][kk]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle(Form("|#eta^{jet%i}|", kk));
      hratio->Draw("EP");
      tg->Draw("LSAME");
      CMS_lumi(pad1, period, 0);
      c1->Update();
      c1->RedrawAxis();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));

      pad1->cd();
      HS_jetPhi_cut[jj][kk]->Draw("HIST");
      H_jetPhi_cut[nfile-1][jj][kk]->Draw("SAME");
      HSig_jetPhi_cut[jj][kk]->Draw("HISTSAME");
      HA_jetPhi_cut[jj][kk]->Draw("E2 SAME");
      HS_jetPhi_cut[jj][kk]->GetYaxis()->SetTitle("Events");
      HS_jetPhi_cut[jj][kk]->GetXaxis()->SetTitle("");
      //HS_jetPhi_cut[jj][kk]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
      HS_jetPhi_cut[jj][kk]->GetXaxis()->SetLabelSize(0);
      HS_jetPhi_cut[jj][kk]->SetMaximum(1000);
      HS_jetPhi_cut[jj][kk]->SetMinimum(1);
      if(jj==1){HS_jetPhi_cut[jj][kk]->SetMaximum(1000); HS_jetPhi_cut[jj][kk]->SetMinimum(0.1);}
      lhs->Clear();
      lhs->SetNColumns(2);
      for(Int_t ifile=0; ifile<nfile; ifile++){
	//if(ifile==10) continue;
	if(ifile == nfile-1) lhs->AddEntry(H_jetPhi_cut[ifile][jj][kk], ltext[ifile], "PL");
	if(ifile == nfile-2) lhs->AddEntry(HSig_jetPhi_cut[jj][kk], ltext[ifile], "L");
	if(ifile < nfile-1) lhs->AddEntry(H_jetPhi_cut[ifile][jj][kk], ltext[ifile], "F");
      }
      lhs->Draw("SAME");
      title = HS_jetPhi_cut[jj][kk]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_jetPhi_cut[jj][kk], H_jetPhi_cut[nfile-1][jj][kk]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle(Form("#phi^{jet%i}", kk));
      hratio->Draw("EP");
      tg->Draw("LSAME");
      CMS_lumi(pad1, period, 0);
      c1->Update();
      c1->RedrawAxis();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));
      
      pad1->cd();
      HS_djetMETPhi_cut[jj][kk]->Draw("HIST");
      H_djetMETPhi_cut[nfile-1][jj][kk]->Draw("SAME");
      HSig_djetMETPhi_cut[jj][kk]->Draw("HISTSAME");
      HA_djetMETPhi_cut[jj][kk]->Draw("E2 SAME");
      HS_djetMETPhi_cut[jj][kk]->GetYaxis()->SetTitle("Events");
      HS_djetMETPhi_cut[jj][kk]->GetXaxis()->SetTitle("");
      //HS_djetMETPhi_cut[jj][kk]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
      HS_djetMETPhi_cut[jj][kk]->GetXaxis()->SetLabelSize(0);
      HS_djetMETPhi_cut[jj][kk]->SetMaximum(10000);
      HS_djetMETPhi_cut[jj][kk]->SetMinimum(1);
      if(jj==1){HS_djetMETPhi_cut[jj][kk]->SetMaximum(1000); HS_djetMETPhi_cut[jj][kk]->SetMinimum(0.1);}
      lhs->Clear();
      lhs->SetNColumns(2);
      for(Int_t ifile=0; ifile<nfile; ifile++){
	//if(ifile==10) continue;
	if(ifile == nfile-1) lhs->AddEntry(H_djetMETPhi_cut[ifile][jj][kk], ltext[ifile], "PL");
	if(ifile == nfile-2) lhs->AddEntry(HSig_djetMETPhi_cut[jj][kk], ltext[ifile], "L");
	if(ifile < nfile-1) lhs->AddEntry(H_djetMETPhi_cut[ifile][jj][kk], ltext[ifile], "F");
      }
      lhs->Draw("SAME");
      title = HS_djetMETPhi_cut[jj][kk]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_djetMETPhi_cut[jj][kk], H_djetMETPhi_cut[nfile-1][jj][kk]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle(Form("#Delta#phi(jet%i , E_{T}^{miss})", kk));
      hratio->Draw("EP");
      tg->Draw("LSAME");
      CMS_lumi(pad1, period, 0);
      c1->Update();
      c1->RedrawAxis();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));

      pad1->cd();
      HS_dr_phojet[jj][kk]->Draw("HIST");
      H_dr_phojet[nfile-1][jj][kk]->Draw("SAME");
      HSig_dr_phojet[jj][kk]->Draw("HISTSAME");
      HA_dr_phojet[jj][kk]->Draw("E2 SAME");
      HS_dr_phojet[jj][kk]->GetYaxis()->SetTitle("Events");
      HS_dr_phojet[jj][kk]->GetXaxis()->SetTitle("");
      //HS_dr_phojet[jj][kk]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
      HS_dr_phojet[jj][kk]->GetXaxis()->SetLabelSize(0);
      HS_dr_phojet[jj][kk]->SetMaximum(1000);
      HS_dr_phojet[jj][kk]->SetMinimum(1);
      if(jj==1){HS_dr_phojet[jj][kk]->SetMaximum(1000); HS_dr_phojet[jj][kk]->SetMinimum(0.1);}
      lhs->Clear();
      lhs->SetNColumns(2);
      for(Int_t ifile=0; ifile<nfile; ifile++){
	//if(ifile==10) continue;
	if(ifile == nfile-1) lhs->AddEntry(H_dr_phojet[ifile][jj][kk], ltext[ifile], "PL");
	if(ifile == nfile-2) lhs->AddEntry(HSig_dr_phojet[jj][kk], ltext[ifile], "L");
	if(ifile < nfile-1) lhs->AddEntry(H_dr_phojet[ifile][jj][kk], ltext[ifile], "F");
      }
      lhs->Draw("SAME");
      title = HS_dr_phojet[jj][kk]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_dr_phojet[jj][kk], H_dr_phojet[nfile-1][jj][kk]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle(Form("#DeltaR(#gamma , jet%i)", kk));
      hratio->Draw("EP");
      tg->Draw("LSAME");
      CMS_lumi(pad1, period, 0);
      c1->Update();
      c1->RedrawAxis();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));

      pad1->cd();
      HS_dEta_phojet[jj][kk]->Draw("HIST");
      H_dEta_phojet[nfile-1][jj][kk]->Draw("SAME");
      HSig_dEta_phojet[jj][kk]->Draw("HISTSAME");
      HA_dEta_phojet[jj][kk]->Draw("E2 SAME");
      HS_dEta_phojet[jj][kk]->GetYaxis()->SetTitle("Events");
      HS_dEta_phojet[jj][kk]->GetXaxis()->SetTitle("");
      //HS_dEta_phojet[jj][kk]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
      HS_dEta_phojet[jj][kk]->GetXaxis()->SetLabelSize(0);
      HS_dEta_phojet[jj][kk]->SetMaximum(10000);
      HS_dEta_phojet[jj][kk]->SetMinimum(1);
      if(jj==1){HS_dEta_phojet[jj][kk]->SetMaximum(1000); HS_dEta_phojet[jj][kk]->SetMinimum(0.1);}
      lhs->Clear();
      lhs->SetNColumns(2);
      for(Int_t ifile=0; ifile<nfile; ifile++){
	//if(ifile==10) continue;
	if(ifile == nfile-1) lhs->AddEntry(H_dEta_phojet[ifile][jj][kk], ltext[ifile], "PL");
	if(ifile == nfile-2) lhs->AddEntry(HSig_dEta_phojet[jj][kk], ltext[ifile], "L");
	if(ifile < nfile-1) lhs->AddEntry(H_dEta_phojet[ifile][jj][kk], ltext[ifile], "F");
      }
      lhs->Draw("SAME");
      title = HS_dEta_phojet[jj][kk]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_dEta_phojet[jj][kk], H_dEta_phojet[nfile-1][jj][kk]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle(Form("#Delta#eta(#gamma , jet%i)", kk));
      hratio->Draw("EP");
      tg->Draw("LSAME");
      CMS_lumi(pad1, period, 0);
      c1->Update();
      c1->RedrawAxis();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));

      pad1->cd();
      HS_dPhi_phojet[jj][kk]->Draw("HIST");
      H_dPhi_phojet[nfile-1][jj][kk]->Draw("SAME");
      HSig_dPhi_phojet[jj][kk]->Draw("HISTSAME");
      HA_dPhi_phojet[jj][kk]->Draw("E2 SAME");
      HS_dPhi_phojet[jj][kk]->GetYaxis()->SetTitle("Events");
      HS_dPhi_phojet[jj][kk]->GetXaxis()->SetTitle("");
      //HS_dPhi_phojet[jj][kk]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
      HS_dPhi_phojet[jj][kk]->GetXaxis()->SetLabelSize(0);
      HS_dPhi_phojet[jj][kk]->SetMaximum(10000);
      HS_dPhi_phojet[jj][kk]->SetMinimum(1);
      if(jj==1){HS_dPhi_phojet[jj][kk]->SetMaximum(1000); HS_dPhi_phojet[jj][kk]->SetMinimum(0.1);}
      lhs->Clear();
      lhs->SetNColumns(2);
      for(Int_t ifile=0; ifile<nfile; ifile++){
	//if(ifile==10) continue;
	if(ifile == nfile-1) lhs->AddEntry(H_dPhi_phojet[ifile][jj][kk], ltext[ifile], "PL");
	if(ifile == nfile-2) lhs->AddEntry(HSig_dPhi_phojet[jj][kk], ltext[ifile], "L");
	if(ifile < nfile-1) lhs->AddEntry(H_dPhi_phojet[ifile][jj][kk], ltext[ifile], "F");
      }
      lhs->Draw("SAME");
      title = HS_dPhi_phojet[jj][kk]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_dPhi_phojet[jj][kk], H_dPhi_phojet[nfile-1][jj][kk]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle(Form("#Delta#phi(#gamma , jet%i)", kk));
      hratio->Draw("EP");
      tg->Draw("LSAME");
      CMS_lumi(pad1, period, 0);
      c1->Update();
      c1->RedrawAxis();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));

      
    }
  }  
}
