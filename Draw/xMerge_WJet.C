#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
//#define nfile 4
#define nWJet 6
#define nhisto 2

void xMerge_WJet(Int_t year){
  TString rootname[10];
  TFile *fopen, *fout;
  //TCanvas *c1 = new TCanvas("c1");
  TTree *t;
  Int_t entries = 1.0;
  Float_t outentries = 0.;
  Float_t scale[nWJet] = {0};

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
  
  TH1F *HSumofGenW[nWJet];
  TH1F *H_Events[nWJet];

  Float_t mcXsec[10] = {336.4, 45.21, 10.89, 5.019, 1.159, 0.008};//WJet
  /*
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/job_summer16_WJetsToLNu_MG_HT200to400/sumGenW.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/job_summer16_WJetsToLNu_MG_HT400to600/sumGenW.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/job_summer16_WJetsToLNu_MG_HT600to800/sumGenW.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/job_summer16_WJetsToLNu_MG_HT800to1200/sumGenW.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/job_summer16_WJetsToLNu_MG_HT1200to2500/sumGenW.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/job_summer16_WJetsToLNu_MG_HT2500toInf/sumGenW.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/job_fall17_WJetsToLNu_MG_HT200to400/sumGenW.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/job_fall17_WJetsToLNu_MG_HT400to600/sumGenW.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/job_fall17_WJetsToLNu_MG_HT600to800/sumGenW.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/job_fall17_WJetsToLNu_MG_HT800to1200/sumGenW.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/job_fall17_WJetsToLNu_MG_HT1200to2500/sumGenW.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/job_fall17_WJetsToLNu_MG_HT2500toInf/sumGenW.root";
  }
  else if(year==2018){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/job_autumn18_WJetsToLNu_MG_HT200to400/sumGenW.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/job_autumn18_WJetsToLNu_MG_HT400to600/sumGenW.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/job_autumn18_WJetsToLNu_MG_HT600to800/sumGenW.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/job_autumn18_WJetsToLNu_MG_HT800to1200/sumGenW.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/job_autumn18_WJetsToLNu_MG_HT1200to2500/sumGenW.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/job_autumn18_WJetsToLNu_MG_HT2500toInf/sumGenW.root";
  }
  */
  
  
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/job_summer16_WJetsToLNu_MG_HT200to400/output_ggtree.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/job_summer16_WJetsToLNu_MG_HT400to600/output_ggtree.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/job_summer16_WJetsToLNu_MG_HT600to800/output_ggtree.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/job_summer16_WJetsToLNu_MG_HT800to1200/output_ggtree.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/job_summer16_WJetsToLNu_MG_HT1200to2500/output_ggtree.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/job_summer16_WJetsToLNu_MG_HT2500toInf/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/job_fall17_WJetsToLNu_MG_HT200to400/output_ggtree.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/job_fall17_WJetsToLNu_MG_HT400to600/output_ggtree.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/job_fall17_WJetsToLNu_MG_HT600to800/output_ggtree.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/job_fall17_WJetsToLNu_MG_HT800to1200/output_ggtree.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/job_fall17_WJetsToLNu_MG_HT1200to2500/output_ggtree.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/job_fall17_WJetsToLNu_MG_HT2500toInf/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/job_autumn18_WJetsToLNu_MG_HT200to400/output_ggtree.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/job_autumn18_WJetsToLNu_MG_HT400to600/output_ggtree.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/job_autumn18_WJetsToLNu_MG_HT600to800/output_ggtree.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/job_autumn18_WJetsToLNu_MG_HT800to1200/output_ggtree.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/job_autumn18_WJetsToLNu_MG_HT1200to2500/output_ggtree.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/job_autumn18_WJetsToLNu_MG_HT2500toInf/output_ggtree.root";
  }

  for(Int_t i=0; i<nWJet; i++){
    fopen = new TFile(rootname[i]);
    H_Events[i] = (TH1F*)fopen->Get("hEvents");
    //HSumofGenW[i] = (TH1F*)fopen->Get("hSumofGenW");
    HSumofGenW[i] = (TH1F*)fopen->Get("hSumGenWeight");
    entries = 1.0;
    //entries = H_Events[i]->GetBinContent(1);
    entries = HSumofGenW[i]->GetBinContent(1);
    outentries = mcXsec[i]*1000*lumi;
    scale[i] = 0;
    scale[i] = fabs(outentries/entries);
    cout << "print " << entries << " " << outentries << " " << scale[i]<< endl;
  }

  TH1F *H_dr_jetjet;
  TH1F *H_dEta_jetjet;
  TH1F *H_dPhi_jetjet;
  TH1F *H_dijetMass;
  
  TH1F *H_phoEB_pt_210[2];
  TH1F *H_phoEB_pt_M[2];
  TH1F *H_phoEB_pt_chworst[2];
  TH1F *H_phoEB_pt_MchWorst[2];
  TH1F *H_phoEB_pt_SeedTime[2];
  TH1F *H_phoEB_pt_leptonveto[2];
  TH1F *H_phoEB_pt_MET[2];
  TH1F *H_phoEB_pt_fixMET[2];
  TH1F *H_phoEB_pt_dphoMETPhi[2];
  TH1F *H_phoEB_pt_djetMETPhi[2];
  TH1F *H_phoEB_pt_jetveto[2];
    
  TH1F *H_phoEB_ptcut[2];
  TH1F *H_phoEB_Etacut[2];
  TH1F *H_phoEB_Phicut[2];
  TH1F *H_SeedTime_cut[2];
  TH1F *H_MIP_Nm1[2];
  TH1F *H_MIP_cut[2];
  TH1F *H_MET_Nm1[2];
  TH1F *H_MET_cut[2];
  TH1F *H_MET_Nm1_djetMETPhim0p5[2];
  TH1F *H_MET_Nm1_METm100[2];
  TH1F *H_METPhi_Nm1[2];
  TH1F *H_METPhi_cut[2];
  TH1F *H_dphoMETPhi_Nm1[2];
  TH1F *H_dphoMETPhi_cut[2];
  TH1F *H_phoEB_ptoverMET_cut[2];
  TH1F *H_njet_Nm1[2];
  TH1F *H_njet_cut[2];
  TH1F *H_nvtx_cut[2];
  TH1F *H_phoIsMatch[2];
  TH1F *H_jetpt_210[2];
  TH1F *H_jetpt_M[2];
  TH1F *H_jetpt_chworst[2];
  TH1F *H_jetpt_SeedTime[2];
  TH1F *H_jetpt_leptonveto[2];
  TH1F *H_jetpt_MET[2];
  TH1F *H_jetpt_dphoMETPhi[2];
  TH1F *H_jetpt_djetMETPhi[2];
  TH1F *H_jetpt_jetveto[2];
  
  TH1F *H_jetpt_cut[2][2];
  TH1F *H_jetEta_cut[2][2];
  TH1F *H_jetPhi_cut[2][2];
  TH1F *H_djetMETPhi_Nm1[2][2];
  TH1F *H_djetMETPhi_cut[2][2];
  TH1F *H_dr_phojet[2][2];
  TH1F *H_dEta_phojet[2][2];
  TH1F *H_dPhi_phojet[2][2];

  
  
  TH1F *HA_dr_jetjet;
  TH1F *HA_dEta_jetjet;
  TH1F *HA_dPhi_jetjet;
  TH1F *HA_dijetMass;
  
  TH1F *HA_phoEB_pt_210[2];
  TH1F *HA_phoEB_pt_M[2];
  TH1F *HA_phoEB_pt_MchWorst[2];
  TH1F *HA_phoEB_pt_chworst[2];
  TH1F *HA_phoEB_pt_SeedTime[2];
  TH1F *HA_phoEB_pt_leptonveto[2];
  TH1F *HA_phoEB_pt_MET[2];
  TH1F *HA_phoEB_pt_fixMET[2];
  TH1F *HA_phoEB_pt_dphoMETPhi[2];
  TH1F *HA_phoEB_pt_djetMETPhi[2];
  TH1F *HA_phoEB_pt_jetveto[2];

  TH1F *HA_phoEB_ptcut[2];
  TH1F *HA_phoEB_Etacut[2];
  TH1F *HA_phoEB_Phicut[2];
  TH1F *HA_SeedTime_cut[2];
  TH1F *HA_MIP_Nm1[2];
  TH1F *HA_MIP_cut[2];
  TH1F *HA_MET_Nm1[2];
  TH1F *HA_MET_cut[2];
  TH1F *HA_MET_Nm1_djetMETPhim0p5[2];
  TH1F *HA_MET_Nm1_METm100[2];
  TH1F *HA_METPhi_Nm1[2];
  TH1F *HA_METPhi_cut[2];
  TH1F *HA_dphoMETPhi_Nm1[2];
  TH1F *HA_dphoMETPhi_cut[2];
  TH1F *HA_phoEB_ptoverMET_cut[2];
  TH1F *HA_njet_Nm1[2];
  TH1F *HA_njet_cut[2];
  TH1F *HA_nvtx_cut[2];
  TH1F *HA_phoIsMatch[2];
  TH1F *HA_jetpt_210[2];
  TH1F *HA_jetpt_M[2];
  TH1F *HA_jetpt_chworst[2];
  TH1F *HA_jetpt_SeedTime[2];
  TH1F *HA_jetpt_leptonveto[2];
  TH1F *HA_jetpt_MET[2];
  TH1F *HA_jetpt_dphoMETPhi[2];
  TH1F *HA_jetpt_djetMETPhi[2];
  TH1F *HA_jetpt_jetveto[2];
  
  TH1F *HA_jetpt_cut[2][2];
  TH1F *HA_jetEta_cut[2][2];
  TH1F *HA_jetPhi_cut[2][2];
  TH1F *HA_djetMETPhi_Nm1[2][2];
  TH1F *HA_djetMETPhi_cut[2][2];
  TH1F *HA_dr_phojet[2][2];
  TH1F *HA_dEta_phojet[2][2];
  TH1F *HA_dPhi_phojet[2][2];

  
 

  for(Int_t i=0; i<nWJet;i++){
    fopen = new TFile(rootname[i]);
    if(i==0){
      HA_dr_jetjet = (TH1F*)fopen->Get("h_dr_jetjet")->Clone();
      HA_dEta_jetjet = (TH1F*)fopen->Get("h_dEta_jetjet")->Clone();
      HA_dPhi_jetjet = (TH1F*)fopen->Get("h_dPhi_jetjet")->Clone();
      HA_dijetMass = (TH1F*)fopen->Get("h_dijetMass")->Clone();
      
      for(Int_t j=0; j<nhisto; j++){
	HA_phoEB_pt_210[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_210_%i", j))->Clone();
	HA_phoEB_pt_M[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_M_%i", j))->Clone();
	HA_phoEB_pt_MchWorst[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_MchWorst_%i", j))->Clone();
	HA_phoEB_pt_chworst[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_chworst_%i", j))->Clone();
	HA_phoEB_pt_SeedTime[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_SeedTime_%i", j))->Clone();
	HA_phoEB_pt_leptonveto[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_leptonveto_%i", j))->Clone();
	HA_phoEB_pt_MET[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_MET_%i", j))->Clone();
	HA_phoEB_pt_fixMET[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_fixMET_%i", j))->Clone();
	HA_phoEB_pt_dphoMETPhi[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_dphoMETPhi_%i", j))->Clone();
	HA_phoEB_pt_djetMETPhi[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_djetMETPhi_%i", j))->Clone();
	HA_phoEB_pt_jetveto[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetveto_%i", j))->Clone();

	HA_phoEB_ptcut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", j))->Clone();
	HA_phoEB_Etacut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Etacut_%i", j))->Clone();
	HA_phoEB_Phicut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Phicut_%i", j))->Clone();
	HA_SeedTime_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_SeedTime_cut_%i", j))->Clone();
	HA_MIP_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MIP_Nm1_%i", j))->Clone();
	HA_MIP_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MIP_cut_%i", j))->Clone();
	HA_MET_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", j))->Clone();
	HA_MET_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", j))->Clone();
	HA_MET_Nm1_djetMETPhim0p5[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_djetMETPhim0p5_%i", j))->Clone();
	HA_MET_Nm1_METm100[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_METm100_%i", j))->Clone();
	HA_METPhi_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_Nm1_%i", j))->Clone();
	HA_METPhi_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_cut_%i", j))->Clone();
	HA_dphoMETPhi_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_Nm1_%i", j))->Clone();
	HA_dphoMETPhi_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", j))->Clone();
	HA_phoEB_ptoverMET_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverMET_cut_%i", j))->Clone();
	HA_njet_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_Nm1_%i", j))->Clone();
	HA_njet_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", j))->Clone();
	HA_nvtx_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", j))->Clone();
	HA_phoIsMatch[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoIsMatch_%i", j))->Clone();
	
	HA_jetpt_210[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_210_%i", j))->Clone();
	HA_jetpt_M[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_M_%i", j))->Clone();
	HA_jetpt_chworst[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_chworst_%i", j))->Clone();
	HA_jetpt_SeedTime[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_SeedTime_%i", j))->Clone();
	HA_jetpt_leptonveto[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_leptonveto_%i", j))->Clone();
	HA_jetpt_MET[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_MET_%i", j))->Clone();
	HA_jetpt_dphoMETPhi[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_dphoMETPhi_%i", j))->Clone();
	HA_jetpt_djetMETPhi[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_djetMETPhi_%i", j))->Clone();
	HA_jetpt_jetveto[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_jetveto_%i", j))->Clone();
	
	for(Int_t jj=0; jj<2; jj++){
	  HA_jetpt_cut[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_cut_%i_jet%i", j, jj))->Clone();
	  HA_jetEta_cut[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetEta_cut_%i_jet%i", j, jj))->Clone();
	  HA_jetPhi_cut[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetPhi_cut_%i_jet%i", j, jj))->Clone();
	  HA_djetMETPhi_Nm1[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_djetMETPhi_Nm1_%i_jet%i", j, jj))->Clone();
	  HA_djetMETPhi_cut[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_djetMETPhi_cut_%i_jet%i", j, jj))->Clone();
	  HA_dr_phojet[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_dr_phojet_%i_jet%i", j, jj))->Clone();
	  HA_dEta_phojet[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_dEta_phojet_%i_jet%i", j, jj))->Clone();
	  HA_dPhi_phojet[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_dPhi_phojet_%i_jet%i", j, jj))->Clone();
	}
      }

      HA_dr_jetjet->Scale(scale[i]);
      HA_dEta_jetjet->Scale(scale[i]);
      HA_dPhi_jetjet->Scale(scale[i]);
      HA_dijetMass->Scale(scale[i]);
      
      for(Int_t j=0; j<nhisto; j++){
	HA_phoEB_pt_210[j]->Scale(scale[i]);
	HA_phoEB_pt_M[j]->Scale(scale[i]);
	HA_phoEB_pt_MchWorst[j]->Scale(scale[i]);
	HA_phoEB_pt_chworst[j]->Scale(scale[i]);
	HA_phoEB_pt_SeedTime[j]->Scale(scale[i]);
	HA_phoEB_pt_leptonveto[j]->Scale(scale[i]);
	HA_phoEB_pt_MET[j]->Scale(scale[i]);
	HA_phoEB_pt_fixMET[j]->Scale(scale[i]);
	HA_phoEB_pt_dphoMETPhi[j]->Scale(scale[i]);
	HA_phoEB_pt_djetMETPhi[j]->Scale(scale[i]);
	HA_phoEB_pt_jetveto[j]->Scale(scale[i]);
	HA_phoEB_ptcut[j]->Scale(scale[i]);
	HA_phoEB_Etacut[j]->Scale(scale[i]);
	HA_phoEB_Phicut[j]->Scale(scale[i]);
	HA_SeedTime_cut[j]->Scale(scale[i]);
	HA_MIP_Nm1[j]->Scale(scale[i]);
	HA_MIP_cut[j]->Scale(scale[i]);
	HA_MET_Nm1[j]->Scale(scale[i]);
	HA_MET_cut[j]->Scale(scale[i]);
	HA_MET_Nm1_djetMETPhim0p5[j]->Scale(scale[i]);
	HA_MET_Nm1_METm100[j]->Scale(scale[i]);
	HA_METPhi_Nm1[j]->Scale(scale[i]);
	HA_METPhi_cut[j]->Scale(scale[i]);
	HA_dphoMETPhi_Nm1[j]->Scale(scale[i]);
	HA_dphoMETPhi_cut[j]->Scale(scale[i]);
	HA_njet_Nm1[j]->Scale(scale[i]);
	HA_njet_cut[j]->Scale(scale[i]);
	HA_phoEB_ptoverMET_cut[j]->Scale(scale[i]);
	HA_nvtx_cut[j]->Scale(scale[i]);
	HA_phoIsMatch[j]->Scale(scale[i]);
	HA_jetpt_210[j]->Scale(scale[i]);
	HA_jetpt_M[j]->Scale(scale[i]);
	HA_jetpt_chworst[j]->Scale(scale[i]);
	HA_jetpt_SeedTime[j]->Scale(scale[i]);
	HA_jetpt_leptonveto[j]->Scale(scale[i]);
	HA_jetpt_MET[j]->Scale(scale[i]);
	HA_jetpt_dphoMETPhi[j]->Scale(scale[i]);
	HA_jetpt_djetMETPhi[j]->Scale(scale[i]);
	HA_jetpt_jetveto[j]->Scale(scale[i]);
	
	for(Int_t jj=0; jj<nhisto; jj++){
	  HA_jetpt_cut[j][jj]->Scale(scale[i]);
	  HA_jetEta_cut[j][jj]->Scale(scale[i]);
	  HA_jetPhi_cut[j][jj]->Scale(scale[i]);
	  HA_djetMETPhi_Nm1[j][jj]->Scale(scale[i]);
	  HA_djetMETPhi_cut[j][jj]->Scale(scale[i]);
	  HA_dr_phojet[j][jj]->Scale(scale[i]);
	  HA_dEta_phojet[j][jj]->Scale(scale[i]);
	  HA_dPhi_phojet[j][jj]->Scale(scale[i]);
	}
      }

      HA_dr_jetjet->SetDirectory(0);
      HA_dEta_jetjet->SetDirectory(0);
      HA_dPhi_jetjet->SetDirectory(0);
      HA_dijetMass->SetDirectory(0);
  
      for(Int_t j=0; j<nhisto; j++){
	HA_phoEB_pt_210[j]->SetDirectory(0);
	HA_phoEB_pt_M[j]->SetDirectory(0);
	HA_phoEB_pt_MchWorst[j]->SetDirectory(0);
	HA_phoEB_pt_chworst[j]->SetDirectory(0);
	HA_phoEB_pt_SeedTime[j]->SetDirectory(0);
	HA_phoEB_pt_leptonveto[j]->SetDirectory(0);
	HA_phoEB_pt_MET[j]->SetDirectory(0);
	HA_phoEB_pt_fixMET[j]->SetDirectory(0);
	HA_phoEB_pt_dphoMETPhi[j]->SetDirectory(0);
	HA_phoEB_pt_djetMETPhi[j]->SetDirectory(0);
	HA_phoEB_pt_jetveto[j]->SetDirectory(0);
	HA_phoEB_ptcut[j]->SetDirectory(0);
	HA_phoEB_Etacut[j]->SetDirectory(0);
	HA_phoEB_Phicut[j]->SetDirectory(0);
	HA_SeedTime_cut[j]->SetDirectory(0);
	HA_MIP_Nm1[j]->SetDirectory(0);
	HA_MIP_cut[j]->SetDirectory(0);
	HA_MET_Nm1[j]->SetDirectory(0);
	HA_MET_cut[j]->SetDirectory(0);
	HA_MET_Nm1_djetMETPhim0p5[j]->SetDirectory(0);
	HA_MET_Nm1_METm100[j]->SetDirectory(0);
	HA_METPhi_Nm1[j]->SetDirectory(0);
	HA_METPhi_cut[j]->SetDirectory(0);
	HA_dphoMETPhi_Nm1[j]->SetDirectory(0);
	HA_dphoMETPhi_cut[j]->SetDirectory(0);
	HA_njet_Nm1[j]->SetDirectory(0);
	HA_njet_cut[j]->SetDirectory(0);
	HA_phoEB_ptoverMET_cut[j]->SetDirectory(0);
	HA_nvtx_cut[j]->SetDirectory(0);
	HA_phoIsMatch[j]->SetDirectory(0);
	HA_jetpt_210[j]->SetDirectory(0);
	HA_jetpt_M[j]->SetDirectory(0);
	HA_jetpt_chworst[j]->SetDirectory(0);
	HA_jetpt_SeedTime[j]->SetDirectory(0);
	HA_jetpt_leptonveto[j]->SetDirectory(0);
	HA_jetpt_MET[j]->SetDirectory(0);
	HA_jetpt_dphoMETPhi[j]->SetDirectory(0);
	HA_jetpt_djetMETPhi[j]->SetDirectory(0);
	HA_jetpt_jetveto[j]->SetDirectory(0);
	
	for(Int_t jj=0; jj<nhisto; jj++){
	  HA_jetpt_cut[j][jj]->SetDirectory(0);
	  HA_jetEta_cut[j][jj]->SetDirectory(0);
	  HA_jetPhi_cut[j][jj]->SetDirectory(0);
	  HA_djetMETPhi_Nm1[j][jj]->SetDirectory(0);
	  HA_djetMETPhi_cut[j][jj]->SetDirectory(0);
	  HA_dr_phojet[j][jj]->SetDirectory(0);
	  HA_dEta_phojet[j][jj]->SetDirectory(0);
	  HA_dPhi_phojet[j][jj]->SetDirectory(0);
	}
      }
    }
      
    H_dr_jetjet = (TH1F*)fopen->Get("h_dr_jetjet")->Clone();
    H_dEta_jetjet = (TH1F*)fopen->Get("h_dEta_jetjet")->Clone();
    H_dPhi_jetjet = (TH1F*)fopen->Get("h_dPhi_jetjet")->Clone();
    H_dijetMass = (TH1F*)fopen->Get("h_dijetMass")->Clone();
    
    for(Int_t j=0; j<nhisto; j++){
      H_phoEB_pt_210[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_210_%i", j))->Clone();
      H_phoEB_pt_M[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_M_%i", j))->Clone();
      H_phoEB_pt_MchWorst[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_MchWorst_%i", j))->Clone();
      H_phoEB_pt_chworst[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_chworst_%i", j))->Clone();
      H_phoEB_pt_SeedTime[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_SeedTime_%i", j))->Clone();
      H_phoEB_pt_leptonveto[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_leptonveto_%i", j))->Clone();
      H_phoEB_pt_MET[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_MET_%i", j))->Clone();
      H_phoEB_pt_fixMET[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_fixMET_%i", j))->Clone();
      H_phoEB_pt_dphoMETPhi[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_dphoMETPhi_%i", j))->Clone();
      H_phoEB_pt_djetMETPhi[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_djetMETPhi_%i", j))->Clone();
      H_phoEB_pt_jetveto[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetveto_%i", j))->Clone();
	
      H_phoEB_ptcut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", j))->Clone();
      H_phoEB_Etacut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Etacut_%i", j))->Clone();
      H_phoEB_Phicut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Phicut_%i", j))->Clone();
      H_SeedTime_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_SeedTime_cut_%i", j))->Clone();
      H_MIP_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MIP_Nm1_%i", j))->Clone();
      H_MIP_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MIP_cut_%i", j))->Clone();
      H_MET_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", j))->Clone();
      H_MET_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", j))->Clone();
      H_MET_Nm1_djetMETPhim0p5[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_djetMETPhim0p5_%i", j))->Clone();
      H_MET_Nm1_METm100[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_METm100_%i", j))->Clone();
      H_METPhi_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_Nm1_%i", j))->Clone();
      H_METPhi_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_cut_%i", j))->Clone();
      H_dphoMETPhi_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_Nm1_%i", j))->Clone();
      H_dphoMETPhi_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", j))->Clone();
      H_phoEB_ptoverMET_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverMET_cut_%i", j))->Clone();
      H_njet_Nm1[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_Nm1_%i", j))->Clone();
      H_njet_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", j))->Clone();
      H_nvtx_cut[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", j))->Clone();
      H_phoIsMatch[j] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoIsMatch_%i", j))->Clone();
      
      H_jetpt_210[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_210_%i", j))->Clone();
      H_jetpt_M[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_M_%i", j))->Clone();
      H_jetpt_chworst[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_chworst_%i", j))->Clone();
      H_jetpt_SeedTime[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_SeedTime_%i", j))->Clone();
      H_jetpt_leptonveto[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_leptonveto_%i", j))->Clone();
      H_jetpt_MET[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_MET_%i", j))->Clone();
      H_jetpt_dphoMETPhi[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_dphoMETPhi_%i", j))->Clone();
      H_jetpt_djetMETPhi[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_djetMETPhi_%i", j))->Clone();
      H_jetpt_jetveto[j] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_jetveto_%i", j))->Clone();
      for(Int_t jj=0; jj<2; jj++){
	H_jetpt_cut[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_cut_%i_jet%i", j, jj))->Clone();
	H_jetEta_cut[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetEta_cut_%i_jet%i", j, jj))->Clone();
	H_jetPhi_cut[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_jetPhi_cut_%i_jet%i", j, jj))->Clone();
	H_djetMETPhi_Nm1[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_djetMETPhi_Nm1_%i_jet%i", j, jj))->Clone();
	H_djetMETPhi_cut[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_djetMETPhi_cut_%i_jet%i", j, jj))->Clone();
	H_dr_phojet[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_dr_phojet_%i_jet%i", j, jj))->Clone();
	H_dEta_phojet[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_dEta_phojet_%i_jet%i", j, jj))->Clone();
	H_dPhi_phojet[j][jj] = (TH1F*)fopen->Get(Form("h_jetpt/h_dPhi_phojet_%i_jet%i", j, jj))->Clone();
      }
    }

    H_dr_jetjet->Scale(scale[i]);
    H_dEta_jetjet->Scale(scale[i]);
    H_dPhi_jetjet->Scale(scale[i]);
    H_dijetMass->Scale(scale[i]);
      
    for(Int_t j=0; j<nhisto; j++){
      H_phoEB_pt_210[j]->Scale(scale[i]);
      H_phoEB_pt_M[j]->Scale(scale[i]);
      H_phoEB_pt_MchWorst[j]->Scale(scale[i]);
      H_phoEB_pt_chworst[j]->Scale(scale[i]);
      H_phoEB_pt_SeedTime[j]->Scale(scale[i]);
      H_phoEB_pt_leptonveto[j]->Scale(scale[i]);
      H_phoEB_pt_MET[j]->Scale(scale[i]);
      H_phoEB_pt_fixMET[j]->Scale(scale[i]);
      H_phoEB_pt_dphoMETPhi[j]->Scale(scale[i]);
      H_phoEB_pt_djetMETPhi[j]->Scale(scale[i]);
      H_phoEB_pt_jetveto[j]->Scale(scale[i]);
	
      H_phoEB_ptcut[j]->Scale(scale[i]);
      H_phoEB_Etacut[j]->Scale(scale[i]);
      H_phoEB_Phicut[j]->Scale(scale[i]);
      H_SeedTime_cut[j]->Scale(scale[i]);
      H_MIP_Nm1[j]->Scale(scale[i]);
      H_MIP_cut[j]->Scale(scale[i]);
      H_MET_Nm1[j]->Scale(scale[i]);
      H_MET_cut[j]->Scale(scale[i]);
      H_MET_Nm1_djetMETPhim0p5[j]->Scale(scale[i]);
      H_MET_Nm1_METm100[j]->Scale(scale[i]);
      H_METPhi_Nm1[j]->Scale(scale[i]);
      H_METPhi_cut[j]->Scale(scale[i]);
      H_dphoMETPhi_Nm1[j]->Scale(scale[i]);
      H_dphoMETPhi_cut[j]->Scale(scale[i]);
      H_njet_Nm1[j]->Scale(scale[i]);
      H_njet_cut[j]->Scale(scale[i]);
      H_phoEB_ptoverMET_cut[j]->Scale(scale[i]);
      H_nvtx_cut[j]->Scale(scale[i]);
      H_phoIsMatch[j]->Scale(scale[i]);
      H_jetpt_210[j]->Scale(scale[i]);
      H_jetpt_M[j]->Scale(scale[i]);
      H_jetpt_chworst[j]->Scale(scale[i]);
      H_jetpt_SeedTime[j]->Scale(scale[i]);
      H_jetpt_leptonveto[j]->Scale(scale[i]);
      H_jetpt_MET[j]->Scale(scale[i]);
      H_jetpt_dphoMETPhi[j]->Scale(scale[i]);
      H_jetpt_djetMETPhi[j]->Scale(scale[i]);
      H_jetpt_jetveto[j]->Scale(scale[i]);
	
      for(Int_t jj=0; jj<nhisto; jj++){
	H_jetpt_cut[j][jj]->Scale(scale[i]);
	H_jetEta_cut[j][jj]->Scale(scale[i]);
	H_jetPhi_cut[j][jj]->Scale(scale[i]);
	H_djetMETPhi_Nm1[j][jj]->Scale(scale[i]);
	H_djetMETPhi_cut[j][jj]->Scale(scale[i]);
	H_dr_phojet[j][jj]->Scale(scale[i]);
	H_dEta_phojet[j][jj]->Scale(scale[i]);
	H_dPhi_phojet[j][jj]->Scale(scale[i]);
      }
    }
      
    HA_dr_jetjet->SetDirectory(0);
    HA_dEta_jetjet->SetDirectory(0);
    HA_dPhi_jetjet->SetDirectory(0);
    HA_dijetMass->SetDirectory(0);
      
    for(Int_t j=0; j<nhisto; j++){
      HA_phoEB_pt_210[j]->SetDirectory(0);
      HA_phoEB_pt_M[j]->SetDirectory(0);
      HA_phoEB_pt_MchWorst[j]->SetDirectory(0);
      HA_phoEB_pt_chworst[j]->SetDirectory(0);
      HA_phoEB_pt_SeedTime[j]->SetDirectory(0);
      HA_phoEB_pt_leptonveto[j]->SetDirectory(0);
      HA_phoEB_pt_MET[j]->SetDirectory(0);
      HA_phoEB_pt_fixMET[j]->SetDirectory(0);
      HA_phoEB_pt_dphoMETPhi[j]->SetDirectory(0);
      HA_phoEB_pt_djetMETPhi[j]->SetDirectory(0);
      HA_phoEB_pt_jetveto[j]->SetDirectory(0);
	
      HA_phoEB_ptcut[j]->SetDirectory(0);
      HA_phoEB_Etacut[j]->SetDirectory(0);
      HA_phoEB_Phicut[j]->SetDirectory(0);
      HA_SeedTime_cut[j]->SetDirectory(0);
      HA_MIP_Nm1[j]->SetDirectory(0);
      HA_MIP_cut[j]->SetDirectory(0);
      HA_MET_Nm1[j]->SetDirectory(0);
      HA_MET_cut[j]->SetDirectory(0);
      HA_MET_Nm1_djetMETPhim0p5[j]->SetDirectory(0);
      HA_MET_Nm1_METm100[j]->SetDirectory(0);
      HA_METPhi_Nm1[j]->SetDirectory(0);
      HA_METPhi_cut[j]->SetDirectory(0);
      HA_dphoMETPhi_Nm1[j]->SetDirectory(0);
      HA_dphoMETPhi_cut[j]->SetDirectory(0);
      HA_njet_Nm1[j]->SetDirectory(0);
      HA_njet_cut[j]->SetDirectory(0);
      HA_phoEB_ptoverMET_cut[j]->SetDirectory(0);
      HA_nvtx_cut[j]->SetDirectory(0);
      HA_phoIsMatch[j]->SetDirectory(0);
      HA_jetpt_210[j]->SetDirectory(0);
      HA_jetpt_M[j]->SetDirectory(0);
      HA_jetpt_chworst[j]->SetDirectory(0);
      HA_jetpt_SeedTime[j]->SetDirectory(0);
      HA_jetpt_leptonveto[j]->SetDirectory(0);
      HA_jetpt_MET[j]->SetDirectory(0);
      HA_jetpt_dphoMETPhi[j]->SetDirectory(0);
      HA_jetpt_djetMETPhi[j]->SetDirectory(0);
      HA_jetpt_jetveto[j]->SetDirectory(0);
	
      for(Int_t jj=0; jj<nhisto; jj++){
	HA_jetpt_cut[j][jj]->SetDirectory(0);
	HA_jetEta_cut[j][jj]->SetDirectory(0);
	HA_jetPhi_cut[j][jj]->SetDirectory(0);
	HA_djetMETPhi_Nm1[j][jj]->SetDirectory(0);
	HA_djetMETPhi_cut[j][jj]->SetDirectory(0);
	HA_dr_phojet[j][jj]->SetDirectory(0);
	HA_dEta_phojet[j][jj]->SetDirectory(0);
	HA_dPhi_phojet[j][jj]->SetDirectory(0);
      }
    }

    if(i>0){
      HA_dr_jetjet->Add(H_dr_jetjet);
      HA_dEta_jetjet->Add(H_dEta_jetjet);
      HA_dPhi_jetjet->Add(H_dPhi_jetjet);
      HA_dijetMass->Add(H_dijetMass);
    
      for(Int_t j=0; j<nhisto; j++){
	HA_phoEB_pt_210[j]->Add(H_phoEB_pt_210[j]);
	HA_phoEB_pt_M[j]->Add(H_phoEB_pt_M[j]);
	HA_phoEB_pt_MchWorst[j]->Add(H_phoEB_pt_MchWorst[j]);
	HA_phoEB_pt_chworst[j]->Add(H_phoEB_pt_chworst[j]);
	HA_phoEB_pt_SeedTime[j]->Add(H_phoEB_pt_SeedTime[j]);
	HA_phoEB_pt_leptonveto[j]->Add(H_phoEB_pt_leptonveto[j]);
	HA_phoEB_pt_MET[j]->Add(H_phoEB_pt_MET[j]);
	HA_phoEB_pt_fixMET[j]->Add(H_phoEB_pt_fixMET[j]);
	HA_phoEB_pt_dphoMETPhi[j]->Add(H_phoEB_pt_dphoMETPhi[j]);
	HA_phoEB_pt_djetMETPhi[j]->Add(H_phoEB_pt_djetMETPhi[j]);
	HA_phoEB_pt_jetveto[j]->Add(H_phoEB_pt_jetveto[j]);
      
	HA_phoEB_ptcut[j]->Add(H_phoEB_ptcut[j]);
	HA_phoEB_Etacut[j]->Add(H_phoEB_Etacut[j]);
	HA_phoEB_Phicut[j]->Add(H_phoEB_Phicut[j]);
	HA_SeedTime_cut[j]->Add(H_SeedTime_cut[j]);
	HA_MIP_Nm1[j]->Add(H_MIP_Nm1[j]);
	HA_MIP_cut[j]->Add(H_MIP_cut[j]);
	HA_MET_Nm1[j]->Add(H_MET_Nm1[j]);
	HA_MET_cut[j]->Add(H_MET_cut[j]);
	HA_MET_Nm1_djetMETPhim0p5[j]->Add(H_MET_Nm1_djetMETPhim0p5[j]);
	HA_MET_Nm1_METm100[j]->Add(H_MET_Nm1_METm100[j]);
	HA_METPhi_Nm1[j]->Add(H_METPhi_Nm1[j]);
	HA_METPhi_cut[j]->Add(H_METPhi_cut[j]);
	HA_dphoMETPhi_Nm1[j]->Add(H_dphoMETPhi_Nm1[j]);
	HA_dphoMETPhi_cut[j]->Add(H_dphoMETPhi_cut[j]);
	HA_njet_Nm1[j]->Add(H_njet_Nm1[j]);
	HA_njet_cut[j]->Add(H_njet_cut[j]);
	HA_phoEB_ptoverMET_cut[j]->Add(H_phoEB_ptoverMET_cut[j]);
	HA_nvtx_cut[j]->Add(H_nvtx_cut[j]);
	HA_phoIsMatch[j]->Add(H_phoIsMatch[j]);
	HA_jetpt_210[j]->Add(H_jetpt_210[j]);
	HA_jetpt_M[j]->Add(H_jetpt_M[j]);
	HA_jetpt_chworst[j]->Add(H_jetpt_chworst[j]);
	HA_jetpt_SeedTime[j]->Add(H_jetpt_SeedTime[j]);
	HA_jetpt_leptonveto[j]->Add(H_jetpt_leptonveto[j]);
	HA_jetpt_MET[j]->Add(H_jetpt_MET[j]);
	HA_jetpt_dphoMETPhi[j]->Add(H_jetpt_dphoMETPhi[j]);
	HA_jetpt_djetMETPhi[j]->Add(H_jetpt_djetMETPhi[j]);
	HA_jetpt_jetveto[j]->Add(H_jetpt_jetveto[j]);
      
	for(Int_t jj=0; jj<nhisto; jj++){
	  HA_jetpt_cut[j][jj]->Add(H_jetpt_cut[j][jj]);
	  HA_jetEta_cut[j][jj]->Add(H_jetEta_cut[j][jj]);
	  HA_jetPhi_cut[j][jj]->Add(H_jetPhi_cut[j][jj]);
	  HA_djetMETPhi_Nm1[j][jj]->Add(H_djetMETPhi_Nm1[j][jj]);
	  HA_djetMETPhi_cut[j][jj]->Add(H_djetMETPhi_cut[j][jj]);
	  HA_dr_phojet[j][jj]->Add(H_dr_phojet[j][jj]);
	  HA_dEta_phojet[j][jj]->Add(H_dEta_phojet[j][jj]);
	  HA_dPhi_phojet[j][jj]->Add(H_dPhi_phojet[j][jj]);
	}
      }
    }
    
  }

  fout = new TFile("output_merge_WJet.root", "RECREATE");

  HA_dr_jetjet->Write();
  HA_dEta_jetjet->Write();
  HA_dPhi_jetjet->Write();
  HA_dijetMass->Write();
  
  fout->mkdir("SMandVBS");
  fout->cd("SMandVBS");
  for(Int_t j=0; j<nhisto; j++){
    HA_phoEB_pt_210[j]->Write();
    HA_phoEB_pt_M[j]->Write();
    HA_phoEB_pt_MchWorst[j]->Write();
    HA_phoEB_pt_chworst[j]->Write();
    HA_phoEB_pt_SeedTime[j]->Write();
    HA_phoEB_pt_leptonveto[j]->Write();
    HA_phoEB_pt_MET[j]->Write();
    HA_phoEB_pt_fixMET[j]->Write();
    HA_phoEB_pt_dphoMETPhi[j]->Write();
    HA_phoEB_pt_djetMETPhi[j]->Write();
    HA_phoEB_pt_jetveto[j]->Write();
    
    HA_phoEB_ptcut[j]->Write();
    HA_phoEB_Etacut[j]->Write();
    HA_phoEB_Phicut[j]->Write();
    HA_SeedTime_cut[j]->Write();
    HA_MIP_Nm1[j]->Write();
    HA_MIP_cut[j]->Write();
    HA_MET_Nm1[j]->Write();
    HA_MET_cut[j]->Write();
    HA_MET_Nm1_djetMETPhim0p5[j]->Write();
    HA_MET_Nm1_METm100[j]->Write();
    HA_METPhi_Nm1[j]->Write();
    HA_METPhi_cut[j]->Write();
    HA_dphoMETPhi_Nm1[j]->Write();
    HA_dphoMETPhi_cut[j]->Write();
    HA_njet_Nm1[j]->Write();
    HA_njet_cut[j]->Write();
    HA_phoEB_ptoverMET_cut[j]->Write();
    HA_nvtx_cut[j]->Write();
    HA_phoIsMatch[j]->Write();
  }

  fout->mkdir("h_jetpt");
  fout->cd("h_jetpt");
  for(Int_t j=0; j<nhisto; j++){
    HA_jetpt_210[j]->Write();
    HA_jetpt_M[j]->Write();
    HA_jetpt_chworst[j]->Write();
    HA_jetpt_SeedTime[j]->Write();
    HA_jetpt_leptonveto[j]->Write();
    HA_jetpt_MET[j]->Write();
    HA_jetpt_dphoMETPhi[j]->Write();
    HA_jetpt_djetMETPhi[j]->Write();
    HA_jetpt_jetveto[j]->Write();
    
    for(Int_t jj=0; jj<nhisto; jj++){
      HA_jetpt_cut[j][jj]->Write();
      HA_jetEta_cut[j][jj]->Write();
      HA_jetPhi_cut[j][jj]->Write();
      HA_djetMETPhi_Nm1[j][jj]->Write();
      HA_djetMETPhi_cut[j][jj]->Write();
      HA_dr_phojet[j][jj]->Write();
      HA_dEta_phojet[j][jj]->Write();
      HA_dPhi_phojet[j][jj]->Write();
    }
  }

}
