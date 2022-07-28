#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
//#define nfile 4
#define nVG 3
#define nhisto 2

void xMerge_VG(Int_t year){
  TString rootname[20];
  TFile *fopen, *fout;

  Int_t entries = 1.0;
  Float_t outentries = 0.;
  Float_t scale[nVG] = {0};

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

  TH1F *hEvents = new TH1F("hEvents", "total processed and skimmed events",2,0,2);
  TH1F *hSumofGenW = new TH1F("hSumGenWeight", "Sum of Gen weights",1,0,1);
  
  //Float_t mcXsec[10] = {74.11, 9.39, 2.394, 1.052, 0.2499, 0.005662};//ZJet
  //Float_t mcXsec[20] = {0.2984, 0.206, 0.8099};//Znunug+jet, Zg+jet, Wg+jet
  Float_t mcXsec[20] = {0.8145, 0.2064, 0.3008};//Wg, Zg, Znunug
  
  if(year==2016){
    rootname[0] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/VJet/job_summer16_ZvvJets_HT200to400/output_ggtree.root";
    rootname[1] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/VJet/job_summer16_ZvvJets_HT400to600/output_ggtree.root";
    rootname[2] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/VJet/job_summer16_ZvvJets_HT600to800/output_ggtree.root";
    rootname[3] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/VJet/job_summer16_ZvvJets_HT800to1200/output_ggtree.root";
    rootname[4] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/VJet/job_summer16_ZvvJets_HT1200to2500/output_ggtree.root";
    rootname[5] = "/data1/GMET/ana/ggNtuples102X/V10_06_00_20/220406/summer16/mc/VJet/job_summer16_ZvvJets_HT2500toInf/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/wk_cms3/judychen/chip02/output_ana/220715/fall17/mc/VG/job_UL17_Wg_pt130/output_ggtree.root";
    rootname[1] = "/wk_cms3/judychen/chip02/output_ana/220715/fall17/mc/VG/job_UL17_Zg_pt130/output_ggtree.root";
    rootname[2] = "/wk_cms3/judychen/chip02/output_ana/220715/fall17/mc/VG/job_UL17_Znunug_pt130/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/VG/job_UL18_Wg_pt130/output_ggtree.root";
    rootname[1] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/VG/job_UL18_Zg_pt130/output_ggtree.root";
    rootname[2] = "/wk_cms3/judychen/chip02/output_ana/220715/autumn18/mc/VG/job_UL18_Znunug_pt130/output_ggtree.root";
  }

 for(Int_t i=0; i<nVG; i++){
    fopen = new TFile(rootname[i]);
    hEvents = (TH1F*)fopen->Get("hEvents");
    hSumofGenW = (TH1F*)fopen->Get("hSumGenWeight");
    entries = hSumofGenW->GetBinContent(1);
    outentries = mcXsec[i]*1000*lumi;

    scale[i] = fabs(outentries/entries);
    cout << "print " << entries << " " << outentries << " " << scale[i]<< endl;
  }

 Double_t ptbin[20] = {130, 150, 180, 210, 250, 300, 350, 400, 450, 500, 550, 600, 700, 800, 1000, 2000, 5000};//16 bins, 2016
 //Double_t ptbin[20] = {0, 20, 35, 50, 100, 150, 200};//6 bins, Et_75
 Double_t etabin[20] = {-1.4442, -0.6, 0, 0.6, 1.4442};//4bins
 Double_t METbin[20] = {0, 45, 90, 135, 180, 250, 300, 400, 500, 650, 800, 1200};//10bins
 Double_t phibin[20] = {-TMath::Pi(), -2.5, -2, -1.4, -0.8, 0.0, 0.8, 1.4, 2, 2.5, TMath::Pi()};//10bins
 Double_t dphibin[20] = {-TMath::Pi(), -2.7, -2.3, -1.9, -1.5, -1.2, -0.5, 0.0, 0.5, 1.2, 1.5, 1.9, 2.3, 2.7, TMath::Pi()};//14bins

 //[0, 1][SM, VBS]
 TH1F *h_phoEB_pt_130[2];
 TH1F *h_phoEB_pt_M[2];
 TH1F *h_phoEB_pt_leptonveto[2];
 TH1F *h_phoEB_pt_MET[2];
 TH1F *h_phoEB_pt_dphoMETPhi[2];
 TH1F *h_phoEB_pt_jetveto[2];
 TH1F *h_phoEB_pt_jetpt[2];
 TH1F *h_phoEB_pt_jetjetdEta[2];
 TH1F *h_phoEB_pt_jetjetdPhi[2];
 TH1F *h_phoEB_pt_phojetdR[2];
 TH1F *h_phoEB_pt_ptoverMET[2];
 TH1F *h_phoEB_pt_mindJMETPhi[2];
 TH1F *h_phoEB_pt_dijetMass[2];
  
 TH1F *h_MET_130[2];
 TH1F *h_MET_M[2];
 TH1F *h_MET_leptonveto[2];
 TH1F *h_MET_MET[2];
 TH1F *h_MET_dphoMETPhi[2];
 TH1F *h_MET_jetveto[2];
 TH1F *h_MET_jetpt[2];
 TH1F *h_MET_jetjetdEta[2];
 TH1F *h_MET_jetjetdPhi[2];
 TH1F *h_MET_phojetdR[2];
 TH1F *h_MET_ptoverMET[2];
 TH1F *h_MET_mindJMETPhi[2];
 TH1F *h_MET_dijetMass[2];

  TH1F *h_phoEB_ptcut[2];
  TH1F *h_phoEB_Etacut[2];
  TH1F *h_phoEB_Phicut[2];
    
  TH1F *h_MIP_cut[2];
  
  TH1F *h_MET_Nm1[2];
  TH1F *h_MET_cut[2];
  
  TH1F *h_METPhi_Nm1[2];
  TH1F *h_METPhi_cut[2];

  TH1F *h_dphoMETPhi_Nm1[2];
  TH1F *h_dphoMETPhi_cut[2];
  
  TH1F *h_nvtx_cut[2];

  TH1F *h_njet_Nm1[2];
  TH1F *h_njet_cut[2];

  for(Int_t j=0; j<2; j++){

    h_phoEB_pt_130[j] = new TH1F(Form("h_phoEB_pt_130_%i", j), "matched phoEB pt pt200 cut", 16, ptbin);
    h_phoEB_pt_M[j] = new TH1F(Form("h_phoEB_pt_M_%i", j), "matched phoEB pt M IDcut", 16, ptbin);
    h_phoEB_pt_leptonveto[j] = new TH1F(Form("h_phoEB_pt_leptonveto_%i", j), "leptonveto cut", 16, ptbin);
    h_phoEB_pt_MET[j] = new TH1F(Form("h_phoEB_pt_MET_%i", j), "matched phoEB pt MET cut", 16, ptbin);
    h_phoEB_pt_dphoMETPhi[j] = new TH1F(Form("h_phoEB_pt_dphoMETPhi_%i", j), "matched phoEB pt dphoMETPhi cut", 16, ptbin);
    h_phoEB_pt_jetveto[j] = new TH1F(Form("h_phoEB_pt_jetveto_%i", j), Form("h_phoEB_pt_jetveto_%i", j), 16, ptbin);
    h_phoEB_pt_jetpt[j] = new TH1F(Form("h_phoEB_pt_jetpt_%i", j), Form("h_phoEB_pt_jetpt_%i", j), 16, ptbin);  
    h_phoEB_pt_jetjetdEta[j] = new TH1F(Form("h_phoEB_pt_jetjetdEta_%i", j), "jetjet dEta cut", 16, ptbin);
    h_phoEB_pt_jetjetdPhi[j] = new TH1F(Form("h_phoEB_pt_jetjetdPhi_%i", j), "jetjet dPhi cut", 16, ptbin);
    h_phoEB_pt_phojetdR[j] = new TH1F(Form("h_phoEB_pt_phojetdR_%i", j), "phojet dR cut", 16, ptbin);
    h_phoEB_pt_ptoverMET[j] = new TH1F(Form("h_phoEB_pt_ptoverMET_%i", j), "phoCentral cut", 16, ptbin);
    h_phoEB_pt_mindJMETPhi[j] = new TH1F(Form("h_phoEB_pt_mindJMETPhi_%i", j), "mindJMETPhi cut", 16, ptbin);
    h_phoEB_pt_dijetMass[j] = new TH1F(Form("h_phoEB_pt_dijetMass_%i", j), "dijetMass cut", 16, ptbin);

    h_MET_130[j] = new TH1F(Form("h_MET_130_%i", j), "matched MET pt200 cut", 10, METbin);
    h_MET_M[j] = new TH1F(Form("h_MET_M_%i", j), "matched MET M IDcut", 10, METbin);
    h_MET_leptonveto[j] = new TH1F(Form("h_MET_leptonveto_%i", j), "leptonveto cut", 10, METbin);
    h_MET_MET[j] = new TH1F(Form("h_MET_MET_%i", j), "matched MET MET cut", 10, METbin);
    h_MET_dphoMETPhi[j] = new TH1F(Form("h_MET_dphoMETPhi_%i", j), "matched MET dphoMETPhi cut", 10, METbin);
    h_MET_jetveto[j] = new TH1F(Form("h_MET_jetveto_%i", j), Form("h_MET_jetveto_%i", j), 10, METbin);
    h_MET_jetpt[j] = new TH1F(Form("h_MET_jetpt_%i", j), Form("h_MET_jetpt_%i", j), 10, METbin);  
    h_MET_jetjetdEta[j] = new TH1F(Form("h_MET_jetjetdEta_%i", j), "jetjet dEta cut", 10, METbin);
    h_MET_jetjetdPhi[j] = new TH1F(Form("h_MET_jetjetdPhi_%i", j), "jetjet dPhi cut", 10, METbin);
    h_MET_phojetdR[j] = new TH1F(Form("h_MET_phojetdR_%i", j), "phojet dR cut", 10, METbin);
    h_MET_ptoverMET[j] = new TH1F(Form("h_MET_ptoverMET_%i", j), "phoCentral cut", 10, METbin);
    h_MET_mindJMETPhi[j] = new TH1F(Form("h_MET_mindJMETPhi_%i", j), "mindJMETPhi cut", 10, METbin);
    h_MET_dijetMass[j] = new TH1F(Form("h_MET_dijetMass_%i", j), "dijetMass cut", 10, METbin);


    h_phoEB_ptcut[j] = new TH1F(Form("h_phoEB_ptcut_%i", j), "phoEB pt cut all pas varbin", 16, ptbin);
    h_phoEB_Etacut[j] = new TH1F(Form("h_phoEB_Etacut_%i", j), "phoEB eta cut all pas varbins", 4, etabin);
    h_phoEB_Phicut[j] = new TH1F(Form("h_phoEB_Phicut_%i", j), "phoEB phi cut all pas varbins", 10, phibin);

    h_MIP_cut[j] = new TH1F(Form("h_MIP_cut_%i", j), "MIP energy N cut", 50, 0, 10);
        
    h_MET_Nm1[j] = new TH1F(Form("h_MET_Nm1_%i", j), "pf MET N-1 cut", 10, METbin);
    h_MET_cut[j] = new TH1F(Form("h_MET_cut_%i", j), "pf MET N cut", 10, METbin);
        
    h_METPhi_Nm1[j] = new TH1F(Form("h_METPhi_Nm1_%i", j), "pf MET N-1 cut", 10, phibin);
    h_METPhi_cut[j] = new TH1F(Form("h_METPhi_cut_%i", j), "pf MET N cut", 10, phibin);
      
    h_dphoMETPhi_Nm1[j] = new TH1F(Form("h_dphoMETPhi_Nm1_%i", j), "deltaPhi of pho and MET N-1 cut", 14, dphibin);
    h_dphoMETPhi_cut[j] = new TH1F(Form("h_dphoMETPhi_cut_%i", j), "deltaPhi of pho and MET N cut", 14, dphibin);
       
    h_nvtx_cut[j] = new TH1F(Form("h_nvtx_cut_%i", j), "#vtx N cut", 500, 0, 100);
    
    h_njet_Nm1[j] = new TH1F(Form("h_njet_Nm1_%i", j), "njet N-1 cut", 10, 0, 10);
    h_njet_cut[j] = new TH1F(Form("h_njet_cut_%i", j), "njet N cut", 10, 0, 10);

  }

  Double_t jetptbin[20] = {0, 30, 50, 100, 200, 300, 500, 700, 1200};//8bins
  Double_t jetetabin[30] = {-5, -4.7, -4, -3.5, -3, -2.5, -2.3, -2, -1.7, -1.4, -0.8, 0.0, 0.8, 1.4, 1.7, 2, 2.3, 2.5, 3, 3.5, 4, 4.7, 5};//22bins
    
  ////[0, 1][SM, VBS][0.1][jet1, jet2]
  //TH1F *h_jetpt_cut[2][2];
  //TH1F *h_jetEta_cut[2][2];
  //TH1F *h_jetPhi_cut[2][2];
  //
  //for(Int_t ii=0; ii<2; ii++){
  //  for(Int_t jj=0; jj<2; jj++){
  //    h_jetpt_cut[ii][jj] = new TH1F(Form("h_jetpt_cut_%i_jet%i", ii, jj), "jetpt N cut", 8, jetptbin);
  //    h_jetEta_cut[ii][jj] = new TH1F(Form("h_jetEta_cut_%i_jet%i", ii, jj), "jetEta N cut", 22, jetetabin);
  //    h_jetPhi_cut[ii][jj] = new TH1F(Form("h_jetPhi_cut_%i_jet%i", ii, jj), "jetPhi N cut", 10, phibin);
  //  }
  //}

  //[0, 1][SM, VBS][0.1]
  TH1F *h_jetpt_cut[2];
  TH1F *h_jetEta_cut[2];
  TH1F *h_jetPhi_cut[2];
  for(Int_t ii=0; ii<2; ii++){
    
    h_jetpt_cut[ii] = new TH1F(Form("h_jetpt_cut_%i", ii), "jetpt N cut", 8, jetptbin);
    h_jetEta_cut[ii] = new TH1F(Form("h_jetEta_cut_%i", ii), "jetEta N cut", 22, jetetabin);
    h_jetPhi_cut[ii] = new TH1F(Form("h_jetPhi_cut_%i", ii), "jetPhi N cut", 10, phibin);
  
  }

  ////[0,1][jet1, jet2]
  //TH1F *h_djetMETPhi_Nm1[2];
  //TH1F *h_djetMETPhi_cut[2];
  //
  //TH1F *h_dr_phojet[2];
  //TH1F *h_dEta_phojet[2];
  //TH1F *h_dPhi_phojet[2];
  //
  //for(Int_t ii=0; ii<2; ii++){
  //  h_djetMETPhi_Nm1[ii] = new TH1F(Form("h_djetMETPhi_Nm1_jet%i", ii), "djetMETPhi mindJMETPhi N-1 cut", 14, dphibin);
  //  h_djetMETPhi_cut[ii] = new TH1F(Form("h_djetMETPhi_cut_jet%i", ii), "djetMETPhi mindJMETPhi cut all", 14, dphibin);
  //  
  //  h_dr_phojet[ii] = new TH1F(Form("h_dr_phojet_jet%i", ii), "pho jet dr", 20, 0., 8);
  //  h_dEta_phojet[ii] = new TH1F(Form("h_dEta_phojet_jet%i", ii), "pho jet dEta", 32, 0., 8);
  //  h_dPhi_phojet[ii] = new TH1F(Form("h_dPhi_phojet_jet%i", ii), "pho jet dPhi", 14, dphibin);
  //}

  TH1F *h_djetMETPhi_Nm1;
  TH1F *h_djetMETPhi_cut;

  TH1F *h_dr_phojet;
  TH1F *h_dEta_phojet;
  TH1F *h_dPhi_phojet;

  h_djetMETPhi_Nm1 = new TH1F("h_djetMETPhi_Nm1", "djetMETPhi mindJMETPhi N-1 cut", 14, dphibin);
  h_djetMETPhi_cut = new TH1F("h_djetMETPhi_cut", "djetMETPhi mindJMETPhi cut all", 14, dphibin);
  h_dr_phojet = new TH1F("h_dr_phojet", "pho jet dr", 16, 0., 8);
  h_dEta_phojet = new TH1F("h_dEta_phojet", "pho jet dEta", 32, 0., 8);
  h_dPhi_phojet = new TH1F("h_dPhi_phojet", "pho jet dPhi", 14, dphibin);

  Double_t massbin[20] = {0, 250, 500, 750, 1000, 1250, 1500, 1750, 2000};//8bins

  TH1F *h_dr_jetjet_cut;
  TH1F *h_minJMETdPhi_Nm1;
  TH1F *h_minJMETdPhi_cut;
  TH1F *h_dEta_jetjet_Nm1;
  TH1F *h_dEta_jetjet_cut;
  TH1F *h_dPhi_jetjet_cut;
  TH1F *h_dPhi_jetjet_Nm1;
  TH1F *h_dijetMass_Nm1;
  TH1F *h_dijetMass_cut;
  TH1F *h_phoCentral_Nm1;
  TH1F *h_phoCentral_cut;

  h_dr_jetjet_cut = new TH1F("h_dr_jetjet_cut", "jet jet dR N cut", 20, 0., 8);
  h_minJMETdPhi_Nm1 = new TH1F("h_minJMETdPhi_Nm1", "minJMETdPhi N-1", 14, dphibin);
  h_minJMETdPhi_cut = new TH1F("h_minJMETdPhi_cut", "minJMETdPhi N cut", 14, dphibin);
  h_dEta_jetjet_Nm1 = new TH1F("h_dEta_jetjet_Nm1", "jet jet dEta N-1", 32, 0., 8);
  h_dEta_jetjet_cut = new TH1F("h_dEta_jetjet_cut", "jet jet dEta N cut", 32, 0., 8);
  h_dPhi_jetjet_Nm1 = new TH1F("h_dPhi_jetjet_Nm1", "jet jet dPhi N-1", 14, dphibin);
  h_dPhi_jetjet_cut = new TH1F("h_dPhi_jetjet_cut", "jet jet dPhi N cut", 14, dphibin);
  h_dijetMass_cut = new TH1F("h_dijetMass_cut", "dijet mass N cut", 8, massbin);
  h_dijetMass_Nm1 = new TH1F("h_dijetMass_Nm1", "dijet mass N-1 cut", 8, massbin);
  h_phoCentral_Nm1 = new TH1F("h_phoCentral_Nm1", "phoCentral N-1", 10, 0, 1);
  h_phoCentral_cut = new TH1F("h_phoCentral_cut", "phoCentral N cut", 10, 0, 1);

  TH1F *h_ptoverMET_Nm1;
  TH1F *h_ptoverMET_cut;

  h_ptoverMET_Nm1 = new TH1F("h_ptoverMET_Nm1", "phoEB pt/MET N-1 cut", 60, 0, 12);
  h_ptoverMET_cut = new TH1F("h_ptoverMET_cut", "phoEB pt/MET N cut", 60, 0, 12);
  
  TH1F *h_jetjetZdPhi_cut;
  TH1F *h_jetjetphodPhi_cut;
  TH1F *h_jet3Central_cut;

  h_jetjetZdPhi_cut = new TH1F("h_jetjetZdPhi_cut", "jetjet Z dPhi N cut", 14, dphibin);
  h_jetjetphodPhi_cut = new TH1F("h_jetjetphodPhi_cut", "jetjet pho dPhi N cut", 14, dphibin);
  h_jet3Central_cut = new TH1F("h_jet3Central_cut", "jet3Central N cut", 10, 0, 1);

  TH1F *h_MET_Nm1_minJMETdPhi_SB;
  
  h_MET_Nm1_minJMETdPhi_SB = new TH1F("h_MET_Nm1_minJMETdPhi_SB", "MET in minJMETdPhi SB region", 10, 100, 1100);

  // ptoverMET SR (ptoverMET < 2.2 && minJMETdPhi SB)
  TH1F *h_phoEB_ptcut_ptoverMET_CR_minJMETdPhi_SB;
  TH1F *h_jetpt_ptoverMET_CR_minJMETdPhi_SB;
  TH1F *h_MET_ptoverMET_CR_minJMETdPhi_SB;
  TH1F *h_dphoMETPhi_ptoverMET_CR_minJMETdPhi_SB;
  TH1F *h_dijetMass_ptoverMET_CR_minJMETdPhi_SB;
  TH1F *h_dEta_jetjet_ptoverMET_CR_minJMETdPhi_SB;
  TH1F *h_minJMETdPhi_ptoverMET_CR_minJMETdPhi_SB;

  h_phoEB_ptcut_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_phoEB_ptcut_ptoverMET_CR_minJMETdPhi_SB", "", 16, ptbin);
  h_jetpt_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_jetpt_ptoverMET_CR_minJMETdPhi_SB", "", 8, jetptbin);
  h_MET_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_MET_ptoverMET_CR_minJMETdPhi_SB", "", 10, METbin);
  h_dphoMETPhi_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_dphoMETPhi_ptoverMET_CR_minJMETdPhi_SB", "", 14, dphibin);
  h_minJMETdPhi_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_minJMETdPhi_ptoverMET_CR_minJMETdPhi_SB", "", 14, dphibin);
  h_dijetMass_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_dijetMass_ptoverMET_CR_minJMETdPhi_SB", "", 8, massbin);
  h_dEta_jetjet_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_dEta_jetjet_ptoverMET_CR_minJMETdPhi_SB", "", 32, 0., 8);
  
  // ptoverMET CR (ptoverMET > 2.0)
  TH1F *h_phoEB_ptcut_ptoverMET_CR;
  TH1F *h_jetpt_ptoverMET_CR;
  TH1F *h_MET_ptoverMET_CR;
  TH1F *h_dphoMETPhi_ptoverMET_CR;
  TH1F *h_dijetMass_ptoverMET_CR;
  TH1F *h_dEta_jetjet_ptoverMET_CR;
  TH1F *h_minJMETdPhi_ptoverMET_CR;

  h_phoEB_ptcut_ptoverMET_CR = new TH1F("h_phoEB_ptcut_ptoverMET_CR", "", 16, ptbin);
  h_jetpt_ptoverMET_CR = new TH1F("h_jetpt_ptoverMET_CR", "", 8, jetptbin);
  h_MET_ptoverMET_CR = new TH1F("h_MET_ptoverMET_CR", "", 10, METbin);
  h_dphoMETPhi_ptoverMET_CR = new TH1F("h_dphoMETPhi_ptoverMET_CR", "", 14, dphibin);
  h_minJMETdPhi_ptoverMET_CR = new TH1F("h_minJMETdPhi_ptoverMET_CR", "", 14, dphibin);
  h_dijetMass_ptoverMET_CR = new TH1F("h_dijetMass_ptoverMET_CR", "", 8, massbin);
  h_dEta_jetjet_ptoverMET_CR = new TH1F("h_dEta_jetjet_ptoverMET_CR", "", 32, 0., 8);

  // ptoverMET SR (ptoverMET < 2.2 && minJMETdPhi SB)
  TH1F *h_phoEB_ptcut_ptoverMET_SR_minJMETdPhi_SB;
  TH1F *h_jetpt_ptoverMET_SR_minJMETdPhi_SB;
  TH1F *h_MET_ptoverMET_SR_minJMETdPhi_SB;
  TH1F *h_dphoMETPhi_ptoverMET_SR_minJMETdPhi_SB;
  TH1F *h_dijetMass_ptoverMET_SR_minJMETdPhi_SB;
  TH1F *h_dEta_jetjet_ptoverMET_SR_minJMETdPhi_SB;
  TH1F *h_minJMETdPhi_ptoverMET_SR_minJMETdPhi_SB;

  h_phoEB_ptcut_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_phoEB_ptcut_ptoverMET_SR_minJMETdPhi_SB", "", 16, ptbin);
  h_jetpt_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_jetpt_ptoverMET_SR_minJMETdPhi_SB", "", 8, jetptbin);
  h_MET_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_MET_ptoverMET_SR_minJMETdPhi_SB", "", 10, METbin);
  h_dphoMETPhi_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_dphoMETPhi_ptoverMET_SR_minJMETdPhi_SB", "", 14, dphibin);
  h_minJMETdPhi_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_minJMETdPhi_ptoverMET_SR_minJMETdPhi_SB", "", 14, dphibin);
  h_dijetMass_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_dijetMass_ptoverMET_SR_minJMETdPhi_SB", "", 8, massbin);
  h_dEta_jetjet_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_dEta_jetjet_ptoverMET_SR_minJMETdPhi_SB", "", 32, 0., 8);

  // ptoverMET SR (ptoverMET < 2.0)
  TH1F *h_phoEB_ptcut_ptoverMET_SR;
  TH1F *h_jetpt_ptoverMET_SR;
  TH1F *h_MET_ptoverMET_SR;
  TH1F *h_dphoMETPhi_ptoverMET_SR;
  TH1F *h_dijetMass_ptoverMET_SR;
  TH1F *h_dEta_jetjet_ptoverMET_SR;
  TH1F *h_minJMETdPhi_ptoverMET_SR;

  h_phoEB_ptcut_ptoverMET_SR = new TH1F("h_phoEB_ptcut_ptoverMET_SR", "", 16, ptbin);
  h_jetpt_ptoverMET_SR = new TH1F("h_jetpt_ptoverMET_SR", "", 8, jetptbin);
  h_MET_ptoverMET_SR = new TH1F("h_MET_ptoverMET_SR", "", 10, METbin);
  h_dphoMETPhi_ptoverMET_SR = new TH1F("h_dphoMETPhi_ptoverMET_SR", "", 14, dphibin);
  h_minJMETdPhi_ptoverMET_SR = new TH1F("h_minJMETdPhi_ptoverMET_SR", "", 14, dphibin);
  h_dijetMass_ptoverMET_SR = new TH1F("h_dijetMass_ptoverMET_SR", "", 8, massbin);
  h_dEta_jetjet_ptoverMET_SR = new TH1F("h_dEta_jetjet_ptoverMET_SR", "", 32, 0., 8);

  TH2F *h2_MET_minJMETdPhi;
  TH2F *h2_MET_dijetMass;
  TH2F *h2_MET_ptoverMET;
  TH2F *h2_dijetMass_minJMETdPhi;
  TH2F *h2_dijetMass_ptoverMET;
  TH2F *h2_minJMETdPhi_ptoverMET;
  
  h2_MET_minJMETdPhi = new TH2F("h2_MET_minJMETdPhi", "MET and minJMETdPhi relation", 60, 0, 1200, 30, -TMath::Pi(), TMath::Pi());
  h2_MET_dijetMass = new TH2F("h2_MET_dijetMass", "MET and dijetMass relation", 60, 0, 1200, 20, 250, 2250);
  h2_MET_ptoverMET = new TH2F("h2_MET_ptoverMET", "MET and ptoverMET relation", 60, 0, 1200, 10, 0, 5);
  h2_dijetMass_minJMETdPhi = new TH2F("h2_dijetMass_minJMETdPhi", "dijetMass and minJMETdPhi relation", 20, 250, 2250, 30, -TMath::Pi(), TMath::Pi());
  h2_dijetMass_ptoverMET = new TH2F("h2_dijetMass_ptoverMET", "dijetaMass and ptoverMET relation", 20, 250, 2250, 10, 0, 5);
  h2_minJMETdPhi_ptoverMET = new TH2F("h2_minJMETdPhi_ptoverMET", "dijetaMass and ptoverMET relation", 14, dphibin, 10, 0, 5);

  //merge plots
  for(Int_t i=0; i<nVG; i++){
    fopen = new TFile(rootname[i]);
    
    h_dr_jetjet_cut->Add((TH1F*)fopen->Get("dijet/h_dr_jetjet_cut"), scale[i]);
    h_minJMETdPhi_Nm1->Add((TH1F*)fopen->Get("dijet/h_minJMETdPhi_Nm1"), scale[i]);
    h_minJMETdPhi_cut->Add((TH1F*)fopen->Get("dijet/h_minJMETdPhi_cut"), scale[i]);
    h_dEta_jetjet_Nm1->Add((TH1F*)fopen->Get("dijet/h_dEta_jetjet_Nm1"), scale[i]);
    h_dEta_jetjet_cut->Add((TH1F*)fopen->Get("dijet/h_dEta_jetjet_cut"), scale[i]);
    h_dPhi_jetjet_Nm1->Add((TH1F*)fopen->Get("dijet/h_dPhi_jetjet_Nm1"), scale[i]);
    h_dPhi_jetjet_cut->Add((TH1F*)fopen->Get("dijet/h_dPhi_jetjet_cut"), scale[i]);
    h_dijetMass_cut->Add((TH1F*)fopen->Get("dijet/h_dijetMass_cut"), scale[i]);
    h_dijetMass_Nm1->Add((TH1F*)fopen->Get("dijet/h_dijetMass_Nm1"), scale[i]);
    h_phoCentral_Nm1->Add((TH1F*)fopen->Get("dijet/h_phoCentral_Nm1"), scale[i]);
    h_phoCentral_cut->Add((TH1F*)fopen->Get("dijet/h_phoCentral_cut"), scale[i]);

    h_ptoverMET_Nm1->Add((TH1F*)fopen->Get("dijet/h_ptoverMET_Nm1"), scale[i]);
    h_ptoverMET_cut->Add((TH1F*)fopen->Get("dijet/h_ptoverMET_cut"), scale[i]);
      
    h_jetjetZdPhi_cut->Add((TH1F*)fopen->Get("dijet/h_jetjetZdPhi_cut"), scale[i]);
    h_jetjetphodPhi_cut->Add((TH1F*)fopen->Get("dijet/h_jetjetphodPhi_cut"), scale[i]);
    h_jet3Central_cut->Add((TH1F*)fopen->Get("dijet/h_jet3Central_cut"), scale[i]);

    h_MET_Nm1_minJMETdPhi_SB->Add((TH1F*)fopen->Get("h_MET_Nm1_minJMETdPhi_SB"), scale[i]);

    h_phoEB_ptcut_ptoverMET_CR->Add((TH1F*)fopen->Get("ptoverMET_CR/h_phoEB_ptcut_ptoverMET_CR"), scale[i]);
    h_jetpt_ptoverMET_CR->Add((TH1F*)fopen->Get("ptoverMET_CR/h_jetpt_ptoverMET_CR"), scale[i]);
    h_MET_ptoverMET_CR->Add((TH1F*)fopen->Get("ptoverMET_CR/h_MET_ptoverMET_CR"), scale[i]);
    h_dphoMETPhi_ptoverMET_CR->Add((TH1F*)fopen->Get("ptoverMET_CR/h_dphoMETPhi_ptoverMET_CR"), scale[i]);
    h_minJMETdPhi_ptoverMET_CR->Add((TH1F*)fopen->Get("ptoverMET_CR/h_minJMETdPhi_ptoverMET_CR"), scale[i]);
    h_dijetMass_ptoverMET_CR->Add((TH1F*)fopen->Get("ptoverMET_CR/h_dijetMass_ptoverMET_CR"), scale[i]);
    h_dEta_jetjet_ptoverMET_CR->Add((TH1F*)fopen->Get("ptoverMET_CR/h_dEta_jetjet_ptoverMET_CR"), scale[i]);

    h_phoEB_ptcut_ptoverMET_SR->Add((TH1F*)fopen->Get("ptoverMET_SR/h_phoEB_ptcut_ptoverMET_SR"), scale[i]);
    h_jetpt_ptoverMET_SR->Add((TH1F*)fopen->Get("ptoverMET_SR/h_jetpt_ptoverMET_SR"), scale[i]);
    h_MET_ptoverMET_SR->Add((TH1F*)fopen->Get("ptoverMET_SR/h_MET_ptoverMET_SR"), scale[i]);
    h_dphoMETPhi_ptoverMET_SR->Add((TH1F*)fopen->Get("ptoverMET_SR/h_dphoMETPhi_ptoverMET_SR"), scale[i]);
    h_minJMETdPhi_ptoverMET_SR->Add((TH1F*)fopen->Get("ptoverMET_SR/h_minJMETdPhi_ptoverMET_SR"), scale[i]);
    h_dijetMass_ptoverMET_SR->Add((TH1F*)fopen->Get("ptoverMET_SR/h_dijetMass_ptoverMET_SR"), scale[i]);
    h_dEta_jetjet_ptoverMET_SR->Add((TH1F*)fopen->Get("ptoverMET_SR/h_dEta_jetjet_ptoverMET_SR"), scale[i]);

    h2_MET_minJMETdPhi->Add((TH2F*)fopen->Get("h2_plots/h2_MET_minJMETdPhi"), scale[i]);
    h2_MET_dijetMass->Add((TH2F*)fopen->Get("h2_plots/h2_MET_dijetMass"), scale[i]);
    h2_MET_ptoverMET->Add((TH2F*)fopen->Get("h2_plots/h2_MET_ptoverMET"), scale[i]);
    h2_dijetMass_minJMETdPhi->Add((TH2F*)fopen->Get("h2_plots/h2_dijetMass_minJMETdPhi"), scale[i]);
    h2_dijetMass_ptoverMET->Add((TH2F*)fopen->Get("h2_plots/h2_dijetMass_ptoverMET"), scale[i]);
    h2_minJMETdPhi_ptoverMET->Add((TH2F*)fopen->Get("h2_plots/h2_minJMETdPhi_ptoverMET"), scale[i]);

    for(Int_t j=0; j<nhisto; j++){
      
      h_phoEB_pt_130[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_130_%i", j)), scale[i]);
      h_phoEB_pt_M[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_M_%i", j)), scale[i]);
      h_phoEB_pt_leptonveto[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_leptonveto_%i", j)), scale[i]);
      h_phoEB_pt_MET[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_MET_%i", j)), scale[i]);
      h_phoEB_pt_dphoMETPhi[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_dphoMETPhi_%i", j)), scale[i]);
      h_phoEB_pt_jetveto[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetveto_%i", j)), scale[i]);
      h_phoEB_pt_jetpt[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetpt_%i", j)), scale[i]);
      h_phoEB_pt_jetjetdEta[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetjetdEta_%i", j)), scale[i]);
      h_phoEB_pt_jetjetdPhi[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetjetdPhi_%i", j)), scale[i]);
      h_phoEB_pt_phojetdR[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_phojetdR_%i", j)), scale[i]);
      h_phoEB_pt_ptoverMET[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_ptoverMET_%i", j)), scale[i]);
      h_phoEB_pt_mindJMETPhi[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_mindJMETPhi_%i", j)), scale[i]);
      h_phoEB_pt_dijetMass[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_dijetMass_%i", j)), scale[i]);

      h_MET_130[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_130_%i", j)), scale[i]);
      h_MET_M[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_M_%i", j)), scale[i]);
      h_MET_leptonveto[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_leptonveto_%i", j)), scale[i]);
      h_MET_MET[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_MET_%i", j)), scale[i]);
      h_MET_dphoMETPhi[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_dphoMETPhi_%i", j)), scale[i]);
      h_MET_jetveto[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_jetveto_%i", j)), scale[i]);
      h_MET_jetpt[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_jetpt_%i", j)), scale[i]);
      h_MET_jetjetdEta[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_jetjetdEta_%i", j)), scale[i]);
      h_MET_jetjetdPhi[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_jetjetdPhi_%i", j)), scale[i]);
      h_MET_phojetdR[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_phojetdR_%i", j)), scale[i]);
      h_MET_ptoverMET[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_ptoverMET_%i", j)), scale[i]);
      h_MET_mindJMETPhi[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_mindJMETPhi_%i", j)), scale[i]);
      h_MET_dijetMass[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_dijetMass_%i", j)), scale[i]);


      h_phoEB_ptcut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", j)), scale[i]);
      h_phoEB_Etacut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Etacut_%i", j)), scale[i]);
      h_phoEB_Phicut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Phicut_%i", j)), scale[i]);

      h_MIP_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MIP_cut_%i", j)), scale[i]);
        
      h_MET_Nm1[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", j)), scale[i]);
      h_MET_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", j)), scale[i]);
        
      h_METPhi_Nm1[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_Nm1_%i", j)), scale[i]);
      h_METPhi_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_cut_%i", j)), scale[i]);
      
      h_dphoMETPhi_Nm1[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_Nm1_%i", j)), scale[i]);
      h_dphoMETPhi_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", j)), scale[i]);
       
      h_nvtx_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", j)), scale[i]);
    
      h_njet_Nm1[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_njet_Nm1_%i", j)), scale[i]);
      h_njet_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", j)), scale[i]);

      h_jetpt_cut[j]->Add((TH1F*)fopen->Get(Form("dijet/h_jetpt_cut_%i", j)), scale[i]);
      h_jetEta_cut[j]->Add((TH1F*)fopen->Get(Form("dijet/h_jetEta_cut_%i", j)), scale[i]);
      h_jetPhi_cut[j]->Add((TH1F*)fopen->Get(Form("dijet/h_jetPhi_cut_%i", j)), scale[i]);

    }

    h_djetMETPhi_Nm1->Add((TH1F*)fopen->Get("dijet/h_djetMETPhi_Nm1"), scale[i]);
    h_djetMETPhi_cut->Add((TH1F*)fopen->Get("dijet/h_djetMETPhi_cut"), scale[i]);
    
    h_dr_phojet->Add((TH1F*)fopen->Get("dijet/h_dr_phojet"), scale[i]);
    h_dEta_phojet->Add((TH1F*)fopen->Get("dijet/h_dEta_phojet"), scale[i]);
    h_dPhi_phojet->Add((TH1F*)fopen->Get("dijet/h_dPhi_phojet"), scale[i]);

  }

  fout = new TFile("output_merge_VG.root", "RECREATE");
  fout->cd();

  h_MET_Nm1_minJMETdPhi_SB->Write();
  
  fout->mkdir("SMandVBS");
  fout->cd("SMandVBS");
  for(Int_t j=0; j<2; j++){
    h_phoEB_pt_130[j]->Write();
    h_phoEB_pt_M[j]->Write();
    h_phoEB_pt_leptonveto[j]->Write();
    h_phoEB_pt_MET[j]->Write();
    h_phoEB_pt_dphoMETPhi[j]->Write();
    h_phoEB_pt_jetveto[j]->Write();
    h_phoEB_pt_jetpt[j]->Write();
    h_phoEB_pt_jetjetdEta[j]->Write();
    h_phoEB_pt_jetjetdPhi[j]->Write();
    h_phoEB_pt_phojetdR[j]->Write();
    h_phoEB_pt_ptoverMET[j]->Write();
    h_phoEB_pt_mindJMETPhi[j]->Write();
    h_phoEB_pt_dijetMass[j]->Write();    

    h_MET_130[j]->Write();
    h_MET_M[j]->Write();
    h_MET_leptonveto[j]->Write();
    h_MET_MET[j]->Write();
    h_MET_dphoMETPhi[j]->Write();
    h_MET_jetveto[j]->Write();
    h_MET_jetpt[j]->Write();
    h_MET_jetjetdEta[j]->Write();
    h_MET_jetjetdPhi[j]->Write();
    h_MET_phojetdR[j]->Write();
    h_MET_ptoverMET[j]->Write();
    h_MET_mindJMETPhi[j]->Write();
    h_MET_dijetMass[j]->Write();    

    h_phoEB_ptcut[j]->Write();
    h_phoEB_Etacut[j]->Write();
    h_phoEB_Phicut[j]->Write();

    h_MIP_cut[j]->Write();
    
    h_MET_Nm1[j]->Write();
    h_MET_cut[j]->Write();
    h_METPhi_Nm1[j]->Write();
    h_METPhi_cut[j]->Write();

    h_dphoMETPhi_Nm1[j]->Write();
    h_dphoMETPhi_cut[j]->Write();

    h_nvtx_cut[j]->Write();
    
    h_njet_Nm1[j]->Write();
    h_njet_cut[j]->Write();
    
  }
  
  fout->mkdir("dijet");
  fout->cd("dijet");
  
  h_dr_jetjet_cut->Write();
  h_minJMETdPhi_Nm1->Write();
  h_minJMETdPhi_cut->Write();
  h_dEta_jetjet_Nm1->Write();
  h_dEta_jetjet_cut->Write();
  h_dPhi_jetjet_cut->Write();
  h_dPhi_jetjet_Nm1->Write();
  h_dijetMass_Nm1->Write();
  h_dijetMass_cut->Write();
  h_phoCentral_Nm1->Write();
  h_phoCentral_cut->Write();
  h_ptoverMET_Nm1->Write();
  h_ptoverMET_cut->Write();
  h_jetjetZdPhi_cut->Write();
  h_jetjetphodPhi_cut->Write();
  h_jet3Central_cut->Write();
  
  for(Int_t ii=0; ii<2; ii++){
    h_jetpt_cut[ii]->Write();
    h_jetEta_cut[ii]->Write();
    h_jetPhi_cut[ii]->Write();
  }
    
  h_djetMETPhi_Nm1->Write();
  h_djetMETPhi_cut->Write();

  h_dr_phojet->Write();
  h_dEta_phojet->Write();
  h_dPhi_phojet->Write();
  
  fout->mkdir("ptoverMET_CR");
  fout->cd("ptoverMET_CR");

  h_phoEB_ptcut_ptoverMET_CR->Write();
  h_jetpt_ptoverMET_CR->Write();
  h_MET_ptoverMET_CR->Write();
  h_dphoMETPhi_ptoverMET_CR->Write();
  h_dijetMass_ptoverMET_CR->Write();
  h_dEta_jetjet_ptoverMET_CR->Write();
  h_minJMETdPhi_ptoverMET_CR->Write();

  fout->mkdir("ptoverMET_SR");
  fout->cd("ptoverMET_SR");

  h_phoEB_ptcut_ptoverMET_SR->Write();
  h_jetpt_ptoverMET_SR->Write();
  h_MET_ptoverMET_SR->Write();
  h_dphoMETPhi_ptoverMET_SR->Write();
  h_dijetMass_ptoverMET_SR->Write();
  h_dEta_jetjet_ptoverMET_SR->Write();
  h_minJMETdPhi_ptoverMET_SR->Write();
  
  // fout->mkdir("h2_plots");
  // fout->cd("h2_plots");
  
  // h2_MET_minJMETdPhi->Write();
  // h2_MET_dijetMass->Write();
  // h2_MET_ptoverMET->Write();
  // h2_dijetMass_minJMETdPhi->Write();
  // h2_dijetMass_ptoverMET->Write();
  // h2_minJMETdPhi_ptoverMET->Write();

  fout->Close();
  
}
