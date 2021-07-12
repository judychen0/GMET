#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
//#define nfile 4
#define nGJet 4
#define nhisto 2

void xMerge_GJet(){
  TString rootname[10] = {
			  
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_GJets_MG_HT100to200/210708_031658/output_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_GJets_MG_HT200to400/210708_053232/output_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_GJets_MG_HT400to600/210708_104709/output_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_GJets_MG_HT600toInf/210708_122530/output_ggtree.root"
			 
  };//8 gjet

  //Float_t mcXsec[10] = {9319, 9155, 2323, 2314, 278.5, 271.8, 93.85, 94.7};
  //Float_t mcXsec[10] = {9155, 2314, 271.8, 94.7};
  Float_t mcXsec[10] = {4984, 1125, 128.1, 41.69};
  
  Float_t Xsec = 1125;
  Float_t lumi16 = 36.33;
  
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
  
  TFile *fopen, *fout;
  TCanvas *c1 = new TCanvas("c1");
  TTree *t;
  Int_t entries;
  Float_t outentries = 0.;
  Float_t scale = 0.;

  for(Int_t i=0; i<nGJet;i++){
    fopen = new TFile(rootname[i]);
    HA_Events = (TH1F*)fopen->Get("hEvents");
    entries = HA_Events->GetBinContent(1);
    outentries = mcXsec[i]*1000*lumi16;
    scale = Xsec/mcXsec[i];
    //scale = outentries/entries;
    if(i==0){
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
      }

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
      }

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
      }     
    }

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
    }

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
    }
      
  }

  fout = new TFile("output_merge.root", "RECREATE");
  
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
  }
}
