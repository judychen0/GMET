#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
//#define nfile 4
#define nWJet 5
#define nhisto 2

void xMerge_GJet(Int_t year){
  TString rootname[20];
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_GJets_MG_HT200to400/210715_170847/output_ggtree.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_GJets_MG_HT400to600/210715_215123/output_ggtree.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_GJets_MG_HT600to800/210716_020208/output_ggtree.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_GJets_MG_HT800to1200/210716_024207/output_ggtree.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_GJets_MG_HT1200to2500/210716_024207/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_GJets_MG_HT200to400/210714_212148/output_ggtree.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_GJets_MG_HT400to600/210714_230920/output_ggtree.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_GJets_MG_HT600to800/210715_012330/output_ggtree.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_GJets_MG_HT800to1200/210715_023758/output_ggtree.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_GJets_MG_HT1200to2500/210715_023758/output_ggtree.root";
  }
  

  //Float_t mcXsec[10] = {9319, 9155, 2323, 2314, 278.5, 271.8, 93.85, 94.7};
  //Float_t mcXsec[10] = {9155, 2314, 271.8, 94.7};
  Float_t mcXsec[10] = {336.4, 45.21, 10.89, 5.019, 1.159};//WJet
  
  Float_t lumi16 = 36.33;
  Float_t lumi17 = 41.48;
  Float_t lumi = 1.0;
  if(year==2016){
    lumi = lumi16;
  }
  else if(year==2017){
    lumi = lumi17;
  }
  
  TH1F *HGenWeight;
  TH1F *HSumofGenW;
  TH1F *H_Events;
  TH2F *H2_MET_phoEt;
  TH1F *H_njet;
  TH2F *H2_rho_phoEB_pt_M;
  TH2F *H2_rho_phoEB_pt_chworst;
  TH2F *H2_rho_phoEB_pt_chworst_newEA;
  TH1F *H_phoEB_pt[2];
  TH1F *H_phoEB_pt_200[2];
  TH1F *H_phoEB_pt_M[2];
  TH1F *H_phoEB_pt_chworst[2];
  TH1F *H_phoEB_pt_SeedTime[2];
  TH1F *H_phoEB_pt_phoptoverMET[2];
  TH1F *H_phoEB_pt_MET[2];
  TH1F *H_phoEB_pt_dphoMETPhi[2];
  TH1F *H_phoEB_pt_djetMETPhi[2];
  TH1F *H_phoEB_pt_jetveto[2];
  TH1F *H_phoEB_pt_HLT[2];
  TH1F *H_phoEB_ptcut[2];
  TH1F *H_phoEB_eta[2];
  TH1F *H_phoEB_ptoverMET_Nm1[2];
  TH1F *H_phoEB_ptoverMET_cut[2];
  TH1F *H_SeedTime_Nm1[2];
  TH1F *H_SeedTime_cut[2];
  TH1F *H_MET_Nm1[2];
  TH1F *H_MET_cut[2];
  TH1F *H_dphoMETPhi_Nm1[2];
  TH1F *H_dphoMETPhi_cut[2];
  TH1F *H_njet_Nm1[2];
  TH1F *H_njet_cut[2];
  TH1F *H_jetpt_Nm1[2];
  TH1F *H_jetpt_cut[2];
  TH1F *H_djetMETPhi_Nm1[2];
  TH1F *H_djetMETPhi_cut[2];
  TH1F *H_phoEB_ptoverjetpt[2];
  TH1F *H_nvtx_cut[2];
  TH1F *H_jetpt_HLT[2];
  TH1F *H_dr_phojet[2];
  TH1F *H_dEta_phojet[2];
  TH1F *H_dPhi_phojet[2];

  TH1F *HAGenWeight;
  TH1F *HASumofGenW;
  TH1F *HA_Events;
  TH2F *HA2_MET_phoEt;
  TH1F *HA_njet;
  TH2F *HA2_rho_phoEB_pt_M;
  TH2F *HA2_rho_phoEB_pt_chworst;
  TH2F *HA2_rho_phoEB_pt_chworst_newEA;
  TH1F *HA_phoEB_pt[2];
  TH1F *HA_phoEB_pt_200[2];
  TH1F *HA_phoEB_pt_M[2];
  TH1F *HA_phoEB_pt_chworst[2];
  TH1F *HA_phoEB_pt_SeedTime[2];
  TH1F *HA_phoEB_pt_phoptoverMET[2];
  TH1F *HA_phoEB_pt_MET[2];
  TH1F *HA_phoEB_pt_dphoMETPhi[2];
  TH1F *HA_phoEB_pt_djetMETPhi[2];
  TH1F *HA_phoEB_pt_jetveto[2];
  TH1F *HA_phoEB_pt_HLT[2];
  TH1F *HA_phoEB_ptcut[2];
  TH1F *HA_phoEB_eta[2];
  TH1F *HA_phoEB_ptoverMET_Nm1[2];
  TH1F *HA_phoEB_ptoverMET_cut[2];
  TH1F *HA_SeedTime_Nm1[2];
  TH1F *HA_SeedTime_cut[2];
  TH1F *HA_MET_Nm1[2];
  TH1F *HA_MET_cut[2];
  TH1F *HA_dphoMETPhi_Nm1[2];
  TH1F *HA_dphoMETPhi_cut[2];
  TH1F *HA_njet_Nm1[2];
  TH1F *HA_njet_cut[2];
  TH1F *HA_jetpt_Nm1[2];
  TH1F *HA_jetpt_cut[2];
  TH1F *HA_djetMETPhi_Nm1[2];
  TH1F *HA_djetMETPhi_cut[2];
  TH1F *HA_phoEB_ptoverjetpt[2];
  TH1F *HA_nvtx_cut[2];
  TH1F *HA_jetpt_HLT[2];
  TH1F *HA_dr_phojet[2];
  TH1F *HA_dEta_phojet[2];
  TH1F *HA_dPhi_phojet[2];
  
  TFile *fopen, *fout;
  //TCanvas *c1 = new TCanvas("c1");
  TTree *t;
  Int_t entries;
  Float_t outentries = 0.;
  Float_t scale = 0.;

  for(Int_t i=0; i<nGJet;i++){
    fopen = new TFile(rootname[i]);
    HA_Events = (TH1F*)fopen->Get("hEvents");
    entries = HA_Events->GetBinContent(1);
    outentries = mcXsec[i]*1000*lumi;
    //scale = Xsec/mcXsec[i];
    scale = outentries/entries;
    if(i==0){
      HAGenWeight = (TH1F*)fopen->Get("hGenWeight")->Clone();
      HASumofGenW = (TH1F*)fopen->Get("hSumofGenW")->Clone();
      HA_Events = (TH1F*)fopen->Get("hEvents")->Clone();
      HA2_MET_phoEt = (TH2F*)fopen->Get("h2_MET_phoEt")->Clone();
      HA_njet = (TH1F*)fopen->Get("h_njet");
      HA2_rho_phoEB_pt_M = (TH2F*)fopen->Get("h2_rho_phoEB_pt_M")->Clone();
      HA2_rho_phoEB_pt_chworst = (TH2F*)fopen->Get("h2_rho_phoEB_pt_chworst")->Clone();
      HA2_rho_phoEB_pt_chworst_newEA = (TH2F*)fopen->Get("h2_rho_phoEB_pt_chworst_newEA")->Clone();
      for(Int_t j=0; j<nhisto; j++){
	HA_phoEB_pt[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_%i", j))->Clone();
	HA_phoEB_pt_200[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_200_%i", j))->Clone();
	HA_phoEB_pt_M[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_M_%i", j))->Clone();
	HA_phoEB_pt_chworst[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_chworst_%i", j))->Clone();
	HA_phoEB_pt_SeedTime[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_SeedTime_%i", j))->Clone();
	HA_phoEB_pt_phoptoverMET[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_phoptoverMET_%i", j))->Clone();
	HA_phoEB_pt_MET[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_MET_%i", j))->Clone();
	HA_phoEB_pt_dphoMETPhi[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_dphoMETPhi_%i", j))->Clone();
	HA_phoEB_pt_djetMETPhi[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_djetMETPhi_%i", j))->Clone();
	HA_phoEB_pt_jetveto[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetveto_%i", j))->Clone();
	HA_phoEB_pt_HLT[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_HLT_%i", j))->Clone();
	HA_phoEB_ptcut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", j))->Clone();
	HA_phoEB_eta[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_eta_%i", j))->Clone();
	HA_dr_phojet[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_dr_phojet_jet%i", j))->Clone();
	HA_phoEB_ptoverMET_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverMET_Nm1_%i", j))->Clone();
	HA_phoEB_ptoverMET_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverMET_cut_%i", j))->Clone();
	HA_SeedTime_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_SeedTime_Nm1_%i", j))->Clone();
	HA_SeedTime_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_SeedTime_cut_%i", j))->Clone();
	HA_MET_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", j))->Clone();
	HA_MET_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", j))->Clone();
	HA_dphoMETPhi_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_Nm1_%i", j))->Clone();
	HA_dphoMETPhi_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", j))->Clone();
	HA_njet_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_Nm1_%i", j))->Clone();
	HA_njet_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", j))->Clone();
	HA_jetpt_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_jetpt_Nm1_%i", j))->Clone();
	HA_jetpt_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_jetpt_cut_%i", j))->Clone();
	HA_djetMETPhi_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_djetMETPhi_Nm1_%i", j))->Clone();
	HA_djetMETPhi_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_djetMETPhi_cut_%i", j))->Clone();
	HA_phoEB_ptoverjetpt[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverjetpt_%i", j))->Clone();
	HA_nvtx_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", j))->Clone();
	HA_jetpt_HLT[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_HLT_jet%i", j))->Clone();
	HA_dr_phojet[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_dr_phojet_jet%i", j))->Clone();
	HA_dEta_phojet[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_dEta_phojet_jet%i", j))->Clone();
	HA_dPhi_phojet[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_dPhi_phojet_jet%i", j))->Clone();
      }

      
      HAGenWeight->Scale(scale);
      HASumofGenW->Scale(scale);
      HA_Events->Scale(scale);
      HA2_MET_phoEt->Scale(scale);
      HA_njet->Scale(scale);
      HA2_rho_phoEB_pt_M->Scale(scale);
      HA2_rho_phoEB_pt_chworst->Scale(scale);
      HA2_rho_phoEB_pt_chworst_newEA->Scale(scale);
      for(Int_t j=0; j<nhisto; j++){
	HA_phoEB_pt[j]->Scale(scale);
	HA_phoEB_pt_200[j]->Scale(scale);
	HA_phoEB_pt_M[j]->Scale(scale);
	HA_phoEB_pt_chworst[j]->Scale(scale);
	HA_phoEB_pt_SeedTime[j]->Scale(scale);
	HA_phoEB_pt_phoptoverMET[j]->Scale(scale);
	HA_phoEB_pt_MET[j]->Scale(scale);
	HA_phoEB_pt_dphoMETPhi[j]->Scale(scale);
	HA_phoEB_pt_djetMETPhi[j]->Scale(scale);
	HA_phoEB_pt_jetveto[j]->Scale(scale);
	HA_phoEB_pt_HLT[j]->Scale(scale);
	HA_phoEB_ptcut[j]->Scale(scale);
	HA_phoEB_eta[j]->Scale(scale);
	HA_dr_phojet[j]->Scale(scale);
	HA_phoEB_ptoverMET_Nm1[j]->Scale(scale);
	HA_phoEB_ptoverMET_cut[j]->Scale(scale);
	HA_SeedTime_Nm1[j]->Scale(scale);
	HA_SeedTime_cut[j]->Scale(scale);
	HA_MET_Nm1[j]->Scale(scale);
	HA_MET_cut[j]->Scale(scale);
	HA_dphoMETPhi_Nm1[j]->Scale(scale);
	HA_dphoMETPhi_cut[j]->Scale(scale);
	HA_njet_Nm1[j]->Scale(scale);
	HA_njet_cut[j]->Scale(scale);
	HA_jetpt_Nm1[j]->Scale(scale);
	HA_jetpt_cut[j]->Scale(scale);
	HA_djetMETPhi_Nm1[j]->Scale(scale);
	HA_djetMETPhi_cut[j]->Scale(scale);
	HA_phoEB_ptoverjetpt[j]->Scale(scale);
	HA_nvtx_cut[j]->Scale(scale);
	HA_jetpt_HLT[j]->Scale(scale);
	HA_dr_phojet[j]->Scale(scale);
	HA_dEta_phojet[j]->Scale(scale);
	HA_dPhi_phojet[j]->Scale(scale);
      }
      
      
      HAGenWeight->SetDirectory(0);
      HASumofGenW->SetDirectory(0);
      HA_Events->SetDirectory(0);
      HA2_MET_phoEt->SetDirectory(0);
      HA_njet->SetDirectory(0);
      HA2_rho_phoEB_pt_M->SetDirectory(0);
      HA2_rho_phoEB_pt_chworst->SetDirectory(0);
      HA2_rho_phoEB_pt_chworst_newEA->SetDirectory(0);
      for(Int_t j=0; j<nhisto; j++){
	HA_phoEB_pt[j]->SetDirectory(0);
	HA_phoEB_pt_200[j]->SetDirectory(0);
	HA_phoEB_pt_M[j]->SetDirectory(0);
	HA_phoEB_pt_chworst[j]->SetDirectory(0);
	HA_phoEB_pt_SeedTime[j]->SetDirectory(0);
	HA_phoEB_pt_phoptoverMET[j]->SetDirectory(0);
	HA_phoEB_pt_MET[j]->SetDirectory(0);
	HA_phoEB_pt_dphoMETPhi[j]->SetDirectory(0);
	HA_phoEB_pt_djetMETPhi[j]->SetDirectory(0);
	HA_phoEB_pt_jetveto[j]->SetDirectory(0);
	HA_phoEB_pt_HLT[j]->SetDirectory(0);
	HA_phoEB_ptcut[j]->SetDirectory(0);
	HA_phoEB_eta[j]->SetDirectory(0);
	HA_dr_phojet[j]->SetDirectory(0);
	HA_phoEB_ptoverMET_Nm1[j]->SetDirectory(0);
	HA_phoEB_ptoverMET_cut[j]->SetDirectory(0);
	HA_SeedTime_Nm1[j]->SetDirectory(0);
	HA_SeedTime_cut[j]->SetDirectory(0);
	HA_MET_Nm1[j]->SetDirectory(0);
	HA_MET_cut[j]->SetDirectory(0);
	HA_dphoMETPhi_Nm1[j]->SetDirectory(0);
	HA_dphoMETPhi_cut[j]->SetDirectory(0);
	HA_njet_Nm1[j]->SetDirectory(0);
	HA_njet_cut[j]->SetDirectory(0);
	HA_jetpt_Nm1[j]->SetDirectory(0);
	HA_jetpt_cut[j]->SetDirectory(0);
	HA_djetMETPhi_Nm1[j]->SetDirectory(0);
	HA_djetMETPhi_cut[j]->SetDirectory(0);
	HA_phoEB_ptoverjetpt[j]->SetDirectory(0);
	HA_nvtx_cut[j]->SetDirectory(0);
	HA_jetpt_HLT[j]->SetDirectory(0);
	HA_dr_phojet[j]->SetDirectory(0);
	HA_dEta_phojet[j]->SetDirectory(0);
	HA_dPhi_phojet[j]->SetDirectory(0);
      }     
    }

    HGenWeight = (TH1F*)fopen->Get("hGenWeight");
    HSumofGenW = (TH1F*)fopen->Get("hSumofGenW");
    H_Events = (TH1F*)fopen->Get("hEvents");
    H2_MET_phoEt = (TH2F*)fopen->Get("h2_MET_phoEt");
    H_njet = (TH1F*)fopen->Get("h_njet");
    H2_rho_phoEB_pt_M = (TH2F*)fopen->Get("h2_rho_phoEB_pt_M");
    H2_rho_phoEB_pt_chworst = (TH2F*)fopen->Get("h2_rho_phoEB_pt_chworst");
    H2_rho_phoEB_pt_chworst_newEA = (TH2F*)fopen->Get("h2_rho_phoEB_pt_chworst_newEA");
    for(Int_t j=0; j<nhisto; j++){
      H_phoEB_pt[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_%i", j));
      H_phoEB_pt_200[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_200_%i", j));
      H_phoEB_pt_M[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_M_%i", j));
      H_phoEB_pt_chworst[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_chworst_%i", j));
      H_phoEB_pt_SeedTime[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_SeedTime_%i", j));
      H_phoEB_pt_phoptoverMET[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_phoptoverMET_%i", j));
      H_phoEB_pt_MET[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_MET_%i", j));
      H_phoEB_pt_dphoMETPhi[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_dphoMETPhi_%i", j));
      H_phoEB_pt_djetMETPhi[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_djetMETPhi_%i", j));
      H_phoEB_pt_jetveto[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetveto_%i", j));
      H_phoEB_pt_HLT[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_HLT_%i", j));
      H_phoEB_ptcut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", j));
      H_phoEB_eta[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_eta_%i", j));
      H_dr_phojet[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_dr_phojet_jet%i", j));
      H_phoEB_ptoverMET_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverMET_Nm1_%i", j));
      H_phoEB_ptoverMET_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverMET_cut_%i", j));
      H_MET_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", j));
      H_MET_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", j));
      H_dphoMETPhi_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_Nm1_%i", j));
      H_dphoMETPhi_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", j));
      H_njet_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_Nm1_%i", j));
      H_njet_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", j));
      H_jetpt_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_jetpt_Nm1_%i", j));
      H_jetpt_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_jetpt_cut_%i", j));
      H_djetMETPhi_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_djetMETPhi_Nm1_%i", j));
      H_djetMETPhi_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_djetMETPhi_cut_%i", j));
      H_phoEB_ptoverjetpt[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverjetpt_%i", j));
      H_nvtx_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", j));
      H_jetpt_HLT[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_HLT_jet%i", j));
      H_dr_phojet[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_dr_phojet_jet%i", j));
    }

    HGenWeight->Scale(scale);
    HSumofGenW->Scale(scale);
    H_Events->Scale(scale);
    H2_MET_phoEt->Scale(scale);
    H_njet->Scale(scale);
    H2_rho_phoEB_pt_M->Scale(scale);
    H2_rho_phoEB_pt_chworst->Scale(scale);
    H2_rho_phoEB_pt_chworst_newEA->Scale(scale);
    for(Int_t j=0; j<nhisto; j++){
      H_phoEB_pt[j]->Scale(scale);
      H_phoEB_pt_200[j]->Scale(scale);
      H_phoEB_pt_M[j]->Scale(scale);
      H_phoEB_pt_chworst[j]->Scale(scale);
      H_phoEB_pt_SeedTime[j]->Scale(scale);
      H_phoEB_pt_phoptoverMET[j]->Scale(scale);
      H_phoEB_pt_MET[j]->Scale(scale);
      H_phoEB_pt_dphoMETPhi[j]->Scale(scale);
      H_phoEB_pt_djetMETPhi[j]->Scale(scale);
      H_phoEB_pt_jetveto[j]->Scale(scale);
      H_phoEB_pt_HLT[j]->Scale(scale);
      H_phoEB_ptcut[j]->Scale(scale);
      H_phoEB_eta[j]->Scale(scale);
      H_dr_phojet[j]->Scale(scale);
      H_phoEB_ptoverMET_Nm1[j]->Scale(scale);
      H_phoEB_ptoverMET_cut[j]->Scale(scale);
      H_SeedTime_Nm1[j]->Scale(scale);
      H_SeedTime_cut[j]->Scale(scale);
      H_MET_Nm1[j]->Scale(scale);
      H_MET_cut[j]->Scale(scale);
      H_dphoMETPhi_Nm1[j]->Scale(scale);
      H_dphoMETPhi_cut[j]->Scale(scale);
      H_njet_Nm1[j]->Scale(scale);
      H_njet_cut[j]->Scale(scale);
      H_jetpt_Nm1[j]->Scale(scale);
      H_jetpt_cut[j]->Scale(scale);
      H_djetMETPhi_Nm1[j]->Scale(scale);
      H_djetMETPhi_cut[j]->Scale(scale);
      H_phoEB_ptoverjetpt[j]->Scale(scale);
      H_nvtx_cut[j]->Scale(scale);
      H_jetpt_HLT[j]->Scale(scale);
      H_dr_phojet[j]->Scale(scale);
      H_dEta_phojet[j]->Scale(scale);
      H_dPhi_phojet[j]->Scale(scale);
    }
    
    HAGenWeight->Add(HGenWeight);
    HASumofGenW->Add(HSumofGenW);
    HA_Events->Add(H_Events);
    HA2_MET_phoEt->Add(H2_MET_phoEt);
    HA_njet->Add(H_njet);
    HA2_rho_phoEB_pt_M->Add(H2_rho_phoEB_pt_M);
    HA2_rho_phoEB_pt_chworst->Add(H2_rho_phoEB_pt_chworst);
    HA2_rho_phoEB_pt_chworst_newEA->Add(H2_rho_phoEB_pt_chworst_newEA);
    for(Int_t j=0; j<nhisto; j++){
      HA_phoEB_pt[j]->Add(H_phoEB_pt[j]);
      HA_phoEB_pt_200[j]->Add(H_phoEB_pt_200[j]);
      HA_phoEB_pt_M[j]->Add(H_phoEB_pt_M[j]);
      HA_phoEB_pt_chworst[j]->Add(H_phoEB_pt_chworst[j]);
      HA_phoEB_pt_SeedTime[j]->Add(H_phoEB_pt_SeedTime[j]);
      HA_phoEB_pt_phoptoverMET[j]->Add(H_phoEB_pt_phoptoverMET[j]);
      HA_phoEB_pt_MET[j]->Add(H_phoEB_pt_MET[j]);
      HA_phoEB_pt_dphoMETPhi[j]->Add(H_phoEB_pt_dphoMETPhi[j]);
      HA_phoEB_pt_djetMETPhi[j]->Add(H_phoEB_pt_djetMETPhi[j]);
      HA_phoEB_pt_jetveto[j]->Add(H_phoEB_pt_jetveto[j]);
      HA_phoEB_pt_HLT[j]->Add(H_phoEB_pt_HLT[j]);
      HA_phoEB_ptcut[j]->Add(H_phoEB_ptcut[j]);
      HA_phoEB_eta[j]->Add(H_phoEB_eta[j]);
      HA_dr_phojet[j]->Add(H_dr_phojet[j]);
      HA_phoEB_ptoverMET_Nm1[j]->Add(H_phoEB_ptoverMET_Nm1[j]);
      HA_phoEB_ptoverMET_cut[j]->Add(H_phoEB_ptoverMET_cut[j]);
      HA_SeedTime_Nm1[j]->Add(H_SeedTime_Nm1[j]);
      HA_SeedTime_cut[j]->Add(H_SeedTime_cut[j]);
      HA_MET_Nm1[j]->Add(H_MET_Nm1[j]);
      HA_MET_cut[j]->Add(H_MET_cut[j]);
      HA_dphoMETPhi_Nm1[j]->Add(H_dphoMETPhi_Nm1[j]);
      HA_dphoMETPhi_cut[j]->Add(H_dphoMETPhi_cut[j]);
      HA_njet_Nm1[j]->Add(H_njet_Nm1[j]);
      HA_njet_cut[j]->Add(H_njet_cut[j]);
      HA_jetpt_Nm1[j]->Add(H_jetpt_Nm1[j]);
      HA_jetpt_cut[j]->Add(H_jetpt_cut[j]);
      HA_djetMETPhi_Nm1[j]->Add(H_djetMETPhi_Nm1[j]);
      HA_djetMETPhi_cut[j]->Add(H_djetMETPhi_cut[j]);
      HA_phoEB_ptoverjetpt[j]->Add(H_phoEB_ptoverjetpt[j]);
      HA_nvtx_cut[j]->Add(H_nvtx_cut[j]);
      HA_jetpt_HLT[j]->Add(H_jetpt_HLT[j]);
      HA_dr_phojet[j]->Add(H_dr_phojet[j]);
      HA_dEta_phojet[j]->Add(H_dEta_phojet[j]);
      HA_dPhi_phojet[j]->Add(H_dPhi_phojet[j]);
    }
      
  }

  fout = new TFile("output_merge_WJet.root", "RECREATE");

  HAGenWeight->Write();
  HASumofGenW->Write();
  HA_Events->Write();
  HA2_MET_phoEt->Write();
  HA_njet->Write();
  HA2_rho_phoEB_pt_M->Write();
  HA2_rho_phoEB_pt_chworst->Write();
  HA2_rho_phoEB_pt_chworst_newEA->Write();
  fout->mkdir("SMandVBS");
  fout->cd("SMandVBS");
  for(Int_t j=0; j<nhisto; j++){
    HA_phoEB_pt[j]->Write();
    HA_phoEB_pt_200[j]->Write();
    HA_phoEB_pt_M[j]->Write();
    HA_phoEB_pt_chworst[j]->Write();
    HA_phoEB_pt_SeedTime[j]->Write();
    HA_phoEB_pt_phoptoverMET[j]->Write();
    HA_phoEB_pt_MET[j]->Write();
    HA_phoEB_pt_dphoMETPhi[j]->Write();
    HA_phoEB_pt_djetMETPhi[j]->Write();
    HA_phoEB_pt_jetveto[j]->Write();
    HA_phoEB_pt_HLT[j]->Write();
    HA_phoEB_ptcut[j]->Write();
    HA_phoEB_eta[j]->Write();
    HA_dr_phojet[j]->Write();
    HA_phoEB_ptoverMET_Nm1[j]->Write();
    HA_phoEB_ptoverMET_cut[j]->Write();
    HA_SeedTime_Nm1[j]->Write();
    HA_SeedTime_cut[j]->Write();
    HA_MET_Nm1[j]->Write();
    HA_MET_cut[j]->Write();
    HA_dphoMETPhi_Nm1[j]->Write();
    HA_dphoMETPhi_cut[j]->Write();
    HA_njet_Nm1[j]->Write();
    HA_njet_cut[j]->Write();
    HA_jetpt_Nm1[j]->Write();
    HA_jetpt_cut[j]->Write();
    HA_djetMETPhi_Nm1[j]->Write();
    HA_djetMETPhi_cut[j]->Write();
    HA_phoEB_ptoverjetpt[j]->Write();
    HA_nvtx_cut[j]->Write();
  }
  fout->mkdir("jetpt");
  fout->cd("jetpt");
  for(Int_t j=0; j<2; j++){
    HA_jetpt_HLT[j]->Write();
    HA_dr_phojet[j]->Write();
    HA_dEta_phojet[j]->Write();
    HA_dPhi_phojet[j]->Write();
  }
}
