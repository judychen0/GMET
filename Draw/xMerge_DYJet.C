#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
//#define nfile 4
#define nDYJet 6
#define nhisto 2

void xMerge_DYJet(Int_t year){
  TString rootname[10];
  TFile *fopen, *fout;

  Int_t entries = 1.0;
  Float_t outentries = 0.;
  Float_t scale[nDYJet] = {0};

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
  
  Float_t mcXsec[10] = {38.83, 5.342, 1.286, 0.5668, 0.1361, 0.003019};//DYJet
  
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/job_summer16_DYJetsToLL_m50_MG_HT200to400/output_ggtree.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/job_summer16_DYJetsToLL_m50_MG_HT400to600/output_ggtree.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/job_summer16_DYJetsToLL_m50_MG_HT600to800/output_ggtree.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/job_summer16_DYJetsToLL_m50_MG_HT800to1200/output_ggtree.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/job_summer16_DYJetsToLL_m50_MG_HT1200to2500/output_ggtree.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/job_summer16_DYJetsToLL_m50_MG_HT2500toInf/output_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/DYJet/job_fall17_DYJetsToLL_m50_MG_HT200to400/output_ggtree.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/DYJet/job_fall17_DYJetsToLL_m50_MG_HT400to600/output_ggtree.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/DYJet/job_fall17_DYJetsToLL_m50_MG_HT600to800/output_ggtree.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/DYJet/job_fall17_DYJetsToLL_m50_MG_HT800to1200/output_ggtree.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/DYJet/job_fall17_DYJetsToLL_m50_MG_HT1200to2500/output_ggtree.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/DYJet/job_fall17_DYJetsToLL_m50_MG_HT2500toInf/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/DYJet/job_autumn18_DYJetsToLL_m50_MG_HT200to400/output_ggtree.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/DYJet/job_autumn18_DYJetsToLL_m50_MG_HT400to600/output_ggtree.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/DYJet/job_autumn18_DYJetsToLL_m50_MG_HT600to800/output_ggtree.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/DYJet/job_autumn18_DYJetsToLL_m50_MG_HT800to1200/output_ggtree.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/DYJet/job_autumn18_DYJetsToLL_m50_MG_HT1200to2500/output_ggtree.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/DYJet/job_autumn18_DYJetsToLL_m50_MG_HT2500toInf/output_ggtree.root";
  }

  for(Int_t i=0; i<nDYJet; i++){
    fopen = new TFile(rootname[i]);
    hEvents = (TH1F*)fopen->Get("hEvents");
    hSumofGenW = (TH1F*)fopen->Get("hSumGenWeight");
    entries = hSumofGenW->GetBinContent(1);
    outentries = mcXsec[1]*1000*lumi;

    scale[i] = fabs(outentries/entries);
    cout << "print " << entries << " " << outentries << " " << scale[i]<< endl;
  }

   TH1F* h_dr_jetjet = new TH1F("h_dr_jetjet", "jet jet dR", 20, 0., 8);
  TH1F* h_dEta_jetjet = new TH1F("h_dEta_jetjet", "jet jet dEta", 20, 0., 8);
  TH1F* h_dPhi_jetjet = new TH1F("h_dPhi_jetjet", "jet jet dPhi", 30, -3.14, 3.14);
  TH1F* h_dijetMass = new TH1F("h_dijetMass", "dijet mass", 80, 0, 2000);

  Double_t ptbin[30] = {22, 30, 36, 50, 75, 90, 120, 170, 175, 180, 185, 190, 210,
			230, 250, 300, 350, 400, 500, 750, 1000, 1500, 2000, 3000, 10000};//22 bins, 2016
  Double_t etabin[10] = {-1.566, -1.4442, -0.8, 0, 0.8, 1.4442, 1.566};//6bins

  //[0, 1][SM, VBS]
  TH1F *h_phoEB_pt_210[2];
  TH1F *h_phoEB_pt_M[2];
  TH1F *h_phoEB_pt_leptonveto[2];
  TH1F *h_phoEB_pt_MET[2];
  TH1F *h_phoEB_pt_dphoMETPhi[2];
  TH1F *h_phoEB_pt_djetMETPhi[2];
  TH1F *h_phoEB_pt_jetveto[2];
  
  TH1F *h_phoEB_ptcut[2];
  TH1F *h_phoEB_Etacut[2];
  TH1F *h_phoEB_Phicut[2];
    
  TH1F *h_phoIsMatch[2];
  
  TH1F *h_MIP_Nm1[2];
  TH1F *h_MIP_cut[2];
  
  TH1F *h_MET_Nm1[2];
  TH1F *h_MET_cut[2];
  TH1F *h_MET_Nm1_djetMETPhim0p5[2];
  TH1F *h_MET_Nm1_djetMETPhi_SB[2][8];
  TH1F *h_MET_test[2][4];
  
  TH1F *h_METPhi_Nm1[2];
  TH1F *h_METPhi_cut[2];

  TH1F *h_dphoMETPhi_Nm1[2];
  TH1F *h_dphoMETPhi_cut[2];
  TH1F *h_dphoMETPhi_test[2][4];
  
  TH1F *h_phoEB_ptoverMET_cut[2];
  
  TH1F *h_nvtx_cut[2];

  TH1F *h_njet_Nm1[2];
  TH1F *h_njet_cut[2];

  TH1F *h_nlep_cut[2];

  TH2F *h2_MET_djetMETPhi[2][4];
  
  for(Int_t j=0; j<2; j++){
    h_phoEB_pt_210[j] = new TH1F(Form("h_phoEB_pt_210_%i", j), "matched phoEB pt pt200 cut", 22, ptbin);
    h_phoEB_pt_M[j] = new TH1F(Form("h_phoEB_pt_M_%i", j), "matched phoEB pt M IDcut", 22, ptbin);
    h_phoEB_pt_leptonveto[j] = new TH1F(Form("h_phoEB_pt_leptonveto_%i", j), "leptonveto cut", 22, ptbin);
    h_phoEB_pt_MET[j] = new TH1F(Form("h_phoEB_pt_MET_%i", j), "matched phoEB pt MET cut", 22, ptbin);
    h_phoEB_pt_dphoMETPhi[j] = new TH1F(Form("h_phoEB_pt_dphoMETPhi_%i", j), "matched phoEB pt dphoMETPhi cut", 22, ptbin);
    h_phoEB_pt_djetMETPhi[j] = new TH1F(Form("h_phoEB_pt_djetMETPhi_%i", j), "matched phoEB pt djetMETPhi cut", 22, ptbin);
    h_phoEB_pt_jetveto[j] = new TH1F(Form("h_phoEB_pt_jetveto_%i", j), Form("h_phoEB_pt_jetveto_%i", j), 22, ptbin);  

    h_phoEB_ptcut[j] = new TH1F(Form("h_phoEB_ptcut_%i", j), "phoEB pt cut all pas varbin", 20, 200, 1000);
    h_phoEB_Etacut[j] = new TH1F(Form("h_phoEB_Etacut_%i", j), "phoEB eta cut all pas varbins", 6, etabin);
    h_phoEB_Phicut[j] = new TH1F(Form("h_phoEB_Phicut_%i", j), "phoEB phi cut all pas varbins", 30, -3.14, 3.14);
    h_MIP_Nm1[j] = new TH1F(Form("h_MIP_Nm1_%i", j), "MIP energy N-1 cut", 50, 0, 10);
    h_MIP_cut[j] = new TH1F(Form("h_MIP_cut_%i", j), "MIP energy N cut", 50, 0, 10);
    h_MET_Nm1[j] = new TH1F(Form("h_MET_Nm1_%i", j), "pf MET N-1 cut", 60, 0, 1200);
    h_MET_cut[j] = new TH1F(Form("h_MET_cut_%i", j), "pf MET N cut", 60, 0, 1200);
    h_MET_Nm1_djetMETPhim0p5[j] = new TH1F(Form("h_MET_Nm1_djetMETPhim0p5_%i", j), "pf MET N-1 cut with djetMETPhi<0.5", 60, 0, 1200);
    for(Int_t ii=0; ii<8; ii++){
      h_MET_Nm1_djetMETPhi_SB[j][ii] = new TH1F(Form("h_MET_Nm1_djetMETPhi_SB0p%i_%i", ii+2, j), Form("pfMET N-1 cut with djetMETPhi<0.%i", ii+2), 60, 0, 1200);
    }
    h_METPhi_Nm1[j] = new TH1F(Form("h_METPhi_Nm1_%i", j), "pf MET N-1 cut", 30, -3.14, 3.14);
    h_METPhi_cut[j] = new TH1F(Form("h_METPhi_cut_%i", j), "pf MET N cut", 30, -3.14, 3.14);
    h_dphoMETPhi_Nm1[j] = new TH1F(Form("h_dphoMETPhi_Nm1_%i", j), "deltaPhi of pho and MET N-1 cut", 30, -3.14, 3.14);
    h_dphoMETPhi_cut[j] = new TH1F(Form("h_dphoMETPhi_cut_%i", j), "deltaPhi of pho and MET N cut", 30, -3.14, 3.14);
    h_phoEB_ptoverMET_cut[j] = new TH1F(Form("h_phoEB_ptoverMET_cut_%i", j), "phoEB pt/MET N cut", 20, 0, 4);
    h_njet_Nm1[j] = new TH1F(Form("h_njet_Nm1_%i", j), "njet N-1 cut", 10, 0, 10);
    h_njet_cut[j] = new TH1F(Form("h_njet_cut_%i", j), "njet N cut", 10, 0, 10);
    h_nvtx_cut[j] = new TH1F(Form("h_nvtx_cut_%i", j), "#vtx N cut", 100, 0, 100);
    h_nlep_cut[j] = new TH1F(Form("h_nlep_cut_%i", j), "nlep N cut", 10, 0, 10);
    h_phoIsMatch[j] = new TH1F(Form("h_phoIsMatch_%i", j), "phoIsMatch N cut", 2, 0, 2);
    for(Int_t ii=0; ii<4; ii++){
      h_MET_test[j][ii] = new TH1F(Form("h_MET_test_cut%i_%i", ii, j), "MET cut at diff val", 22, ptbin);
      h_dphoMETPhi_test[j][ii] = new TH1F(Form("h_dphoMETPhi_test_cut%i_%i", ii, j), "dphoMETPhi cut at diff val", 22, ptbin);
      h2_MET_djetMETPhi[j][ii] = new TH2F(Form("h2_MET_djetMETPhi_MET%i_%i", ii, j), "MET and djetMETPhi relation at diff MET cut", 60, 0, 1200, 30, -3.14, 3.14);
    }
  }

  //[0, 1][SM, VBS][0,1][jet1, jet2]
  TH1F *h_jetpt_210[2];
  TH1F *h_jetpt_M[2];
  TH1F *h_jetpt_leptonveto[2];
  TH1F *h_jetpt_MET[2];
  TH1F *h_jetpt_dphoMETPhi[2];
  TH1F *h_jetpt_djetMETPhi[2];
  //detail jet cut
  TH1F *h_jetpt_jetID[2];
  TH1F *h_jetpt_jetPUID[2];
  TH1F *h_jetpt_jetjetdEta[2];
  TH1F *h_jetpt_dijetMass[2];
  TH1F *h_jetpt_phojetdR[2];
  TH1F *h_jetpt_jetjetdR[2];
  //detail jet cut
  TH1F *h_jetpt_jetveto[2];
  
  TH1F *h_jetpt_cut[2][2];
  TH1F *h_jetEta_cut[2][2];
  TH1F *h_jetPhi_cut[2][2];
  
  TH1F *h_djetMETPhi_Nm1[2][2];
  TH1F *h_djetMETPhi_cut[2][2];

  TH1F *h_mindjetMETPhi_Nm1[2];
  TH1F *h_mindjetMETPhi_cut[2];

  TH1F *h_dr_phojet[2][2];
  TH1F *h_dEta_phojet[2][2];
  TH1F *h_dPhi_phojet[2][2];
  
  for(Int_t ii=0; ii<2; ii++){
    h_jetpt_210[ii] = new TH1F(Form("h_jetpt_210_%i", ii), "jetpt phoEB 200 cut", 25, 30, 1030);
    h_jetpt_M[ii] = new TH1F(Form("h_jetpt_M_%i", ii), "jetpt phoEB M cut", 25, 30, 1030);
    h_jetpt_leptonveto[ii] = new TH1F(Form("h_jetpt_leptonveto_%i", ii), "jetpt phoEB leptonveto", 25, 30, 1030);
    h_jetpt_MET[ii] = new TH1F(Form("h_jetpt_MET_%i", ii), "jetpt phoEB MET cut", 25, 30, 1030);
    h_jetpt_dphoMETPhi[ii] = new TH1F(Form("h_jetpt_dphoMETPhi_%i", ii), "jetpt phoEB dphoMETPhi cut", 25, 30, 1030);
    h_jetpt_djetMETPhi[ii] = new TH1F(Form("h_jetpt_djetMETPhi_%i", ii), "jetpt phoEB djetMETPhi cut", 25, 30, 1030);
    h_jetpt_jetID[ii] = new TH1F(Form("h_jetpt_jetID_%i", ii), "jetpt phoEB jetID cut", 25, 30, 1030);
    h_jetpt_jetPUID[ii] = new TH1F(Form("h_jetpt_jetPUID_%i", ii), "jetpt phoEB jetPUID cut", 25, 30, 1030);
    h_jetpt_jetjetdEta[ii] = new TH1F(Form("h_jetpt_jetjetdEta_%i", ii), "jetpt phoEB jetjetdEta cut", 25, 30, 1030);
    h_jetpt_dijetMass[ii] = new TH1F(Form("h_jetpt_dijetMass_%i", ii), "jetpt phoEB dijetMass", 25, 30, 1030);
    h_jetpt_phojetdR[ii] = new TH1F(Form("h_jetpt_phojetdR_%i", ii), "jetpt phoEB phojetdR cut", 25, 30, 1030);
    h_jetpt_jetjetdR[ii] = new TH1F(Form("h_jetpt_jetjetdR_%i", ii), "jetpt phoEB jetjetdR cut", 25, 30, 1030);
    h_jetpt_jetveto[ii] = new TH1F(Form("h_jetpt_jetveto_%i", ii), "jetpt phoEB jetveto cut", 25, 30, 1030);
    
    h_mindjetMETPhi_Nm1[ii] = new TH1F(Form("h_mindjetMETPhi_Nm1_%i", ii), "mindjetMETPhi N-1 cut", 30, -3.14, 3.14);
    h_mindjetMETPhi_cut[ii] = new TH1F(Form("h_mindjetMETPhi_cut_%i", ii), "mindjetMETPhi cut all", 30, -3.14, 3.14);
    
    for(Int_t jj=0; jj<2; jj++){
      
      h_jetpt_cut[ii][jj] = new TH1F(Form("h_jetpt_cut_%i_jet%i", ii, jj), "jetpt cut all", 25, 30, 1030);
      h_jetEta_cut[ii][jj] = new TH1F(Form("h_jetEta_cut_%i_jet%i", ii, jj), "jetEta cut all", 20, -5, 5);
      h_jetPhi_cut[ii][jj] = new TH1F(Form("h_jetPhi_cut_%i_jet%i", ii, jj), "jetphi cut all", 30, -3.14, 3.14);
      h_jetpt_cut[ii][jj]->Sumw2();
      h_jetEta_cut[ii][jj]->Sumw2();
      h_jetPhi_cut[ii][jj]->Sumw2();

      h_djetMETPhi_Nm1[ii][jj] = new TH1F(Form("h_djetMETPhi_Nm1_%i_jet%i", ii, jj), "deltaPhi of jet and MET N-1 cut", 30, -3.14, 3.14);
      h_djetMETPhi_cut[ii][jj] = new TH1F(Form("h_djetMETPhi_cut_%i_jet%i", ii, jj), "deltaPhi of jet and MET N cut", 30, -3.14, 3.14);
      h_djetMETPhi_Nm1[ii][jj]->Sumw2();
      h_djetMETPhi_cut[ii][jj]->Sumw2();

      h_dr_phojet[ii][jj] = new TH1F(Form("h_dr_phojet_%i_jet%i", ii, jj), "pho jet dr", 20, 0., 8);
      h_dEta_phojet[ii][jj] = new TH1F(Form("h_dEta_phojet_%i_jet%i", ii, jj), "pho jet dEta", 20, 0., 8);
      h_dPhi_phojet[ii][jj] = new TH1F(Form("h_dPhi_phojet_%i_jet%i", ii, jj), "pho jet dPhi", 30, -3.14, 3.14);
      h_dr_phojet[ii][jj]->Sumw2();
      h_dEta_phojet[ii][jj]->Sumw2();
      h_dPhi_phojet[ii][jj]->Sumw2();
    }
  }

  for(Int_t i=0; i<nDYJet; i++){
    fopen = new TFile(rootname[i]);
 
    h_dr_jetjet->Add((TH1F*)fopen->Get("h_dr_jetjet"), scale[i]);
    h_dEta_jetjet->Add((TH1F*)fopen->Get("h_dEta_jetjet"), scale[i]);
    h_dPhi_jetjet->Add((TH1F*)fopen->Get("h_dPhi_jetjet"), scale[i]);
    h_dijetMass->Add((TH1F*)fopen->Get("h_dijetMass"), scale[i]);

    for(Int_t j=0; j<nhisto; j++){
      h_phoEB_pt_210[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_210_%i", j)), scale[i]);
      h_phoEB_pt_M[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_M_%i", j)), scale[i]);
      h_phoEB_pt_leptonveto[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_leptonveto_%i", j)), scale[i]);
      h_phoEB_pt_MET[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_MET_%i", j)), scale[i]);
      h_phoEB_pt_dphoMETPhi[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_dphoMETPhi_%i", j)), scale[i]);
      h_phoEB_pt_djetMETPhi[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_djetMETPhi_%i", j)), scale[i]);
      h_phoEB_pt_jetveto[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetveto_%i", j)), scale[i]);

      h_phoEB_ptcut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", j)), scale[i]);
      h_phoEB_Etacut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Etacut_%i", j)), scale[i]);
      h_phoEB_Phicut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_Phicut_%i", j)), scale[i]);
      h_MIP_Nm1[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MIP_Nm1_%i", j)), scale[i]);
      h_MIP_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MIP_cut_%i", j)), scale[i]);
      h_MET_Nm1[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", j)), scale[i]);
      h_MET_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", j)), scale[i]);
      //h_MET_Nm1_djetMETPhim0p5[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_djetMETPhim0p5_%i", j)), scale[i]);
      h_METPhi_Nm1[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_Nm1_%i", j)), scale[i]);
      h_METPhi_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_cut_%i", j)), scale[i]);
      h_dphoMETPhi_Nm1[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_Nm1_%i", j)), scale[i]);
      h_dphoMETPhi_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", j)), scale[i]);
      h_phoEB_ptoverMET_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverMET_cut_%i", j)), scale[i]);
      h_njet_Nm1[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_njet_Nm1_%i", j)), scale[i]);
      h_njet_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", j)), scale[i]);
      h_nvtx_cut[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", j)), scale[i]);
      h_phoIsMatch[j]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_phoIsMatch_%i", j)), scale[i]);

      for(Int_t ii=0; ii<8; ii++){
	h_MET_Nm1_djetMETPhi_SB[j][ii]->Add((TH1F*)fopen->Get(Form("h_MET_Nm1/h_MET_Nm1_djetMETPhi_SB0p%i_%i", ii+2, j)), scale[i]);
      }

      for(Int_t ii=0; ii<4; ii++){
	h_MET_test[j][ii]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_MET_test_cut%i_%i", ii, j)), scale[i]);
	h_dphoMETPhi_test[j][ii]->Add((TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_test_cut%i_%i", ii, j)), scale[i]);
	h2_MET_djetMETPhi[j][ii]->Add((TH2F*)fopen->Get(Form("SMandVBS/h2_MET_djetMETPhi_MET%i_%i", ii, j)), scale[i]);
      }
      
      h_jetpt_210[j]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_210_%i", j)), scale[i]);
      h_jetpt_M[j]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_M_%i", j)), scale[i]);
      h_jetpt_leptonveto[j]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_leptonveto_%i", j)), scale[i]);
      h_jetpt_MET[j]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_MET_%i", j)), scale[i]);
      h_jetpt_dphoMETPhi[j]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_dphoMETPhi_%i", j)), scale[i]);
      h_jetpt_djetMETPhi[j]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_jetveto_%i", j)), scale[i]);
      h_jetpt_jetveto[j]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_jetveto_%i", j)), scale[i]);

      h_mindjetMETPhi_Nm1[j]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_mindjetMETPhi_Nm1_%i", j)), scale[i]);
      h_mindjetMETPhi_cut[j]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_mindjetMETPhi_cut_%i", j)), scale[i]);

      for(Int_t jj=0; jj<2; jj++){
	h_jetpt_cut[j][jj]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_jetpt_cut_%i_jet%i", j, jj)), scale[i]);
	h_jetEta_cut[j][jj]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_jetEta_cut_%i_jet%i", j, jj)), scale[i]);
	h_jetPhi_cut[j][jj]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_jetPhi_cut_%i_jet%i", j, jj)), scale[i]);
	h_djetMETPhi_Nm1[j][jj]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_djetMETPhi_Nm1_%i_jet%i", j, jj)), scale[i]);
	h_djetMETPhi_cut[j][jj]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_djetMETPhi_cut_%i_jet%i", j, jj)), scale[i]);
	h_dr_phojet[j][jj]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_dr_phojet_%i_jet%i", j, jj)), scale[i]);
	h_dEta_phojet[j][jj]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_dEta_phojet_%i_jet%i", j, jj)), scale[i]);
	h_dPhi_phojet[j][jj]->Add((TH1F*)fopen->Get(Form("h_jetpt/h_dPhi_phojet_%i_jet%i", j, jj)), scale[i]);
      }
    }   
  }

  fout = new TFile("output_merge_DYJet.root", "RECREATE");
  
  h_dr_jetjet->Write();
  h_dEta_jetjet->Write();
  h_dPhi_jetjet->Write();
  h_dijetMass->Write();

  fout->mkdir("SMandVBS");
  fout->cd("SMandVBS");
  for(Int_t i=0; i<2; i++){
    
    h_phoEB_pt_210[i]->Write();
    h_phoEB_pt_M[i]->Write();
    h_phoEB_pt_leptonveto[i]->Write();
    h_phoEB_pt_MET[i]->Write();
    h_phoEB_pt_dphoMETPhi[i]->Write();
    h_phoEB_pt_djetMETPhi[i]->Write();
    h_phoEB_pt_jetveto[i]->Write();
      
    h_phoEB_ptcut[i]->Write();
    h_phoEB_Etacut[i]->Write();
    h_phoEB_Phicut[i]->Write();
      
    h_phoIsMatch[i]->Write();
      
    h_MIP_Nm1[i]->Write();
    h_MIP_cut[i]->Write();
    
    h_MET_Nm1[i]->Write();
    h_MET_cut[i]->Write();
    for(Int_t jj=0; jj<4; jj++){
      h_MET_test[i][jj]->Write();
    }   

    //h_MET_Nm1_djetMETPhim0p5[i]->Write();
      
    h_METPhi_Nm1[i]->Write();
    h_METPhi_cut[i]->Write();
      
    h_dphoMETPhi_Nm1[i]->Write();
    h_dphoMETPhi_cut[i]->Write();
    for(Int_t jj=0; jj<4; jj++){
      h_dphoMETPhi_test[i][jj]->Write();
    }

    h_phoEB_ptoverMET_cut[i]->Write();

    h_njet_Nm1[i]->Write();
    h_njet_cut[i]->Write();

    h_nlep_cut[i]->Write();
    h_nvtx_cut[i]->Write();
    for(Int_t jj=0; jj<4; jj++){
      h2_MET_djetMETPhi[i][jj]->Write();
    }  
  }
  
  fout->mkdir("h_jetpt");
  fout->cd("h_jetpt");
  for(Int_t i=0; i<2; i++){
    h_jetpt_210[i]->Write();
    h_jetpt_M[i]->Write();
    h_jetpt_leptonveto[i]->Write();
    h_jetpt_MET[i]->Write();
    h_jetpt_dphoMETPhi[i]->Write();
    h_jetpt_djetMETPhi[i]->Write();
    h_jetpt_jetID[i]->Write();
    h_jetpt_jetPUID[i]->Write();
    h_jetpt_jetjetdEta[i]->Write();
    h_jetpt_dijetMass[i]->Write();
    h_jetpt_phojetdR[i]->Write();
    h_jetpt_jetjetdR[i]->Write();
    h_jetpt_jetveto[i]->Write();
    
    h_mindjetMETPhi_Nm1[i]->Write();
    h_mindjetMETPhi_cut[i]->Write();
          
    for(Int_t jj=0; jj<2; jj++){
      h_jetpt_cut[i][jj]->Write();
      h_jetEta_cut[i][jj]->Write();
      h_jetPhi_cut[i][jj]->Write();

      h_djetMETPhi_Nm1[i][jj]->Write();
      h_djetMETPhi_cut[i][jj]->Write();
      
      h_dr_phojet[i][jj]->Write();
      h_dEta_phojet[i][jj]->Write();
      h_dPhi_phojet[i][jj]->Write();
    }
  }

  fout->mkdir("h_MET_Nm1");
  fout->cd("h_MET_Nm1");
  for(Int_t i=0; i<2; i++){
    for(Int_t jj=0; jj<8; jj++){
      h_MET_Nm1_djetMETPhi_SB[i][jj]->Write();
    }
  }
  
  fout->Close();
  
}
