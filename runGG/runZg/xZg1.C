#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
#include "TH1F.h"
#include "TH2F.h"

#include "smallntuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "PhotonSelection.h"

Float_t WEIGHT = 1.0;

Double_t deltaPhi(Double_t phi1, Double_t phi2) {
  Double_t dPhi = phi1 - phi2;
  if (dPhi > TMath::Pi()) dPhi -= 2.*TMath::Pi();
  if (dPhi < -TMath::Pi()) dPhi += 2.*TMath::Pi();
  return dPhi;
}

Double_t deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2) {
  Double_t dEta, dPhi ;
  dEta = eta1 - eta2;
  dPhi = deltaPhi(phi1, phi2);
  return sqrt(dEta*dEta+dPhi*dPhi);
}

Double_t deltaEta(Double_t eta1, Double_t eta2){
  Double_t dEta;
  dEta = std::abs(eta1 - eta2);
  return dEta;
}

Int_t SetBit(Int_t nbit, Int_t bit){
  return (bit | (1<<nbit));
}

Int_t ResetBit(Int_t nbit, Int_t bit){
  return bit & ~(1<<nbit);
}

void xZg1(char* pathes, Int_t sig){
  //***********************Initialization***********************//
  
  //Get processed events
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("hEvents")->Clone();
  hEvents->SetDirectory(0);
  TH1F *hSumofGenW = (TH1F*)fopen->Get("hSumGenWeight")->Clone();
  hSumofGenW->SetDirectory(0);
  fopen->Close();
  
  //access EventTree with TreeReader class
  TreeReader data(pathes);

  //create an output .root file
  TFile *fout_;
  char foutname[50];
  string treestr = pathes;
  Int_t istr = treestr.find("ggtree");
  string treename = treestr.substr(istr);
  sprintf(foutname, "output_%s", treename.c_str());
  fout_ = new TFile(foutname,"RECREATE");

  Double_t ptbin[20] = {200, 210, 250, 300, 350, 400, 450, 500, 550, 600, 700, 800, 1000, 2000, 5000};//14 bins, 2016
  Double_t etabin[20] = {-1.4442, -0.6, 0, 0.6, 1.4442};//4bins
  Double_t METbin[20] = {0, 80, 100, 150, 200, 250, 300, 400, 500, 650, 800, 1200};//11bins
  Double_t phibin[20] = {-TMath::Pi(), -2.5, -2, -1.4, -0.8, 0.0, 0.8, 1.4, 2, 2.5, TMath::Pi()};//10bins
  Double_t dphibin[20] = {-TMath::Pi(), -2.7, -2.3, -1.9, -1.5, -1.2, -0.5, 0.0, 0.5, 1.2, 1.5, 1.9, 2.3, 2.7, TMath::Pi()};//14bins
  
  //[0, 1][SM, VBS]
  TH1F *h_phoEB_pt_210[2];
  TH1F *h_phoEB_pt_M[2];
  TH1F *h_phoEB_pt_leptonveto[2];
  TH1F *h_phoEB_pt_MET[2];
  TH1F *h_phoEB_pt_dphoMETPhi[2];
  TH1F *h_phoEB_pt_jetjetdEta[2];
  TH1F *h_phoEB_pt_jetjetdPhi[2];
  TH1F *h_phoEB_pt_phojetdR[2];
  TH1F *h_phoEB_pt_phoCentral[2];
  TH1F *h_phoEB_pt_mindJMETPhi[2];
  TH1F *h_phoEB_pt_dijetMass[2];
  TH1F *h_phoEB_pt_jetveto[2];
  
  TH1F *h_phoEB_ptcut[2];
  TH1F *h_phoEB_Etacut[2];
  TH1F *h_phoEB_Phicut[2];
    
  TH1F *h_phoIsMatch[2];
    
  TH1F *h_MIP_cut[2];
  
  TH1F *h_MET_Nm1[2];
  TH1F *h_MET_cut[2];
  
  TH1F *h_METPhi_Nm1[2];
  TH1F *h_METPhi_cut[2];

  TH1F *h_dphoMETPhi_Nm1[2];
  TH1F *h_dphoMETPhi_cut[2];

  TH1F *h_ptoverMET_cut[2];
  TH1F *h_ptoverMET_Nm1[2];
  
  TH1F *h_nvtx_cut[2];

  TH1F *h_njet_Nm1[2];
  TH1F *h_njet_cut[2];

  TH1F *h_nlep_cut[2];

  for(Int_t j=0; j<2; j++){
    h_phoEB_pt_210[j] = new TH1F(Form("h_phoEB_pt_210_%i", j), "matched phoEB pt pt200 cut", 14, ptbin);
    h_phoEB_pt_M[j] = new TH1F(Form("h_phoEB_pt_M_%i", j), "matched phoEB pt M IDcut", 14, ptbin);
    h_phoEB_pt_leptonveto[j] = new TH1F(Form("h_phoEB_pt_leptonveto_%i", j), "leptonveto cut", 14, ptbin);
    h_phoEB_pt_MET[j] = new TH1F(Form("h_phoEB_pt_MET_%i", j), "matched phoEB pt MET cut", 14, ptbin);
    h_phoEB_pt_dphoMETPhi[j] = new TH1F(Form("h_phoEB_pt_dphoMETPhi_%i", j), "matched phoEB pt dphoMETPhi cut", 14, ptbin);
    h_phoEB_pt_jetjetdEta[j] = new TH1F(Form("h_phoEB_pt_jetjetdEta_%i", j), "jetjet dEta cut", 14, ptbin);
    h_phoEB_pt_jetjetdPhi[j] = new TH1F(Form("h_phoEB_pt_jetjetdPhi_%i", j), "jetjet dPhi cut", 14, ptbin);
    h_phoEB_pt_phojetdR[j] = new TH1F(Form("h_phoEB_pt_phojetdR_%i", j), "phojet dR cut", 14, ptbin);
    h_phoEB_pt_phoCentral[j] = new TH1F(Form("h_phoEB_pt_phoCentral_%i", j), "phoCentral cut", 14, ptbin);
    h_phoEB_pt_mindJMETPhi[j] = new TH1F(Form("h_phoEB_pt_mindJMETPhi_%i", j), "mindJMETPhi cut", 14, ptbin);
    h_phoEB_pt_dijetMass[j] = new TH1F(Form("h_phoEB_pt_dijetMass_%i", j), "dijetMass cut", 14, ptbin);
    h_phoEB_pt_jetveto[j] = new TH1F(Form("h_phoEB_pt_jetveto_%i", j), Form("h_phoEB_pt_jetveto_%i", j), 14, ptbin);  
    h_phoEB_pt_210[j]->Sumw2();
    h_phoEB_pt_M[j]->Sumw2();
    h_phoEB_pt_leptonveto[j]->Sumw2();
    h_phoEB_pt_MET[j]->Sumw2();
    h_phoEB_pt_dphoMETPhi[j]->Sumw2();
    h_phoEB_pt_jetjetdEta[j]->Sumw2();
    h_phoEB_pt_jetjetdPhi[j]->Sumw2();
    h_phoEB_pt_phojetdR[j]->Sumw2();
    h_phoEB_pt_phoCentral[j]->Sumw2();
    h_phoEB_pt_mindJMETPhi[j]->Sumw2();
    h_phoEB_pt_dijetMass[j]->Sumw2();
    h_phoEB_pt_jetveto[j]->Sumw2();
    
    h_phoEB_ptcut[j] = new TH1F(Form("h_phoEB_ptcut_%i", j), "phoEB pt cut all pas varbin", 14, ptbin);
    h_phoEB_Etacut[j] = new TH1F(Form("h_phoEB_Etacut_%i", j), "phoEB eta cut all pas varbins", 4, etabin);
    h_phoEB_Phicut[j] = new TH1F(Form("h_phoEB_Phicut_%i", j), "phoEB phi cut all pas varbins", 10, phibin);
    h_phoEB_ptcut[j]->Sumw2();
    h_phoEB_Etacut[j]->Sumw2();
    h_phoEB_Phicut[j]->Sumw2();
        
    h_MIP_cut[j] = new TH1F(Form("h_MIP_cut_%i", j), "MIP energy N cut", 50, 0, 10);
    h_MIP_cut[j]->Sumw2();
    
    h_MET_Nm1[j] = new TH1F(Form("h_MET_Nm1_%i", j), "pf MET N-1 cut", 11, METbin);
    h_MET_cut[j] = new TH1F(Form("h_MET_cut_%i", j), "pf MET N cut", 11, METbin);
    h_MET_Nm1[j]->Sumw2();
    h_MET_cut[j]->Sumw2();
    
    
    h_METPhi_Nm1[j] = new TH1F(Form("h_METPhi_Nm1_%i", j), "pf MET N-1 cut", 10, phibin);
    h_METPhi_cut[j] = new TH1F(Form("h_METPhi_cut_%i", j), "pf MET N cut", 10, phibin);
    h_METPhi_Nm1[j]->Sumw2();
    h_METPhi_cut[j]->Sumw2();
  
    h_dphoMETPhi_Nm1[j] = new TH1F(Form("h_dphoMETPhi_Nm1_%i", j), "deltaPhi of pho and MET N-1 cut", 14, dphibin);
    h_dphoMETPhi_cut[j] = new TH1F(Form("h_dphoMETPhi_cut_%i", j), "deltaPhi of pho and MET N cut", 14, dphibin);
    h_dphoMETPhi_Nm1[j]->Sumw2();
    h_dphoMETPhi_cut[j]->Sumw2();
    
    h_ptoverMET_cut[j] = new TH1F(Form("h_ptoverMET_cut_%i", j), "phoEB pt/MET N cut", 60, 0, 12);
    h_ptoverMET_Nm1[j] = new TH1F(Form("h_ptoverMET_Nm1_%i", j), "phoEB pt/MET N-1 cut", 60, 0, 12);
    h_ptoverMET_cut[j]->Sumw2();
    h_ptoverMET_Nm1[j]->Sumw2();
   
    h_nvtx_cut[j] = new TH1F(Form("h_nvtx_cut_%i", j), "#vtx N cut", 500, 0, 100);
    h_nvtx_cut[j]->Sumw2();

    h_njet_Nm1[j] = new TH1F(Form("h_njet_Nm1_%i", j), "njet N-1 cut", 10, 0, 10);
    h_njet_cut[j] = new TH1F(Form("h_njet_cut_%i", j), "njet N cut", 10, 0, 10);
    h_njet_Nm1[j]->Sumw2();
    h_njet_cut[j]->Sumw2();

    h_nlep_cut[j] = new TH1F(Form("h_nlep_cut_%i", j), "nlep N cut", 10, 0, 10);
    h_nlep_cut[j]->Sumw2();
    
    h_phoIsMatch[j] = new TH1F(Form("h_phoIsMatch_%i", j), "phoIsMatch N cut", 2, 0, 2);
    h_phoIsMatch[j]->Sumw2();
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
  //
  //    h_jetpt_cut[ii][jj]->Sumw2();
  //    h_jetEta_cut[ii][jj]->Sumw2();
  //    h_jetPhi_cut[ii][jj]->Sumw2();
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
    
    h_jetpt_cut[ii]->Sumw2();
    h_jetEta_cut[ii]->Sumw2();
    h_jetPhi_cut[ii]->Sumw2();
  
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
  //  h_djetMETPhi_Nm1[ii]->Sumw2();
  //  h_djetMETPhi_cut[ii]->Sumw2();
  //
  //  h_dr_phojet[ii] = new TH1F(Form("h_dr_phojet_jet%i", ii), "pho jet dr", 20, 0., 8);
  //  h_dEta_phojet[ii] = new TH1F(Form("h_dEta_phojet_jet%i", ii), "pho jet dEta", 32, 0., 8);
  //  h_dPhi_phojet[ii] = new TH1F(Form("h_dPhi_phojet_jet%i", ii), "pho jet dPhi", 14, dphibin);
  //  h_dr_phojet[ii]->Sumw2();
  //  h_dEta_phojet[ii]->Sumw2();
  //  h_dPhi_phojet[ii]->Sumw2();       
  //}

  TH1F *h_djetMETPhi_Nm1;
  TH1F *h_djetMETPhi_cut;

  TH1F *h_dr_phojet;
  TH1F *h_dEta_phojet;
  TH1F *h_dPhi_phojet;

  h_djetMETPhi_Nm1 = new TH1F("h_djetMETPhi_Nm1", "djetMETPhi mindJMETPhi N-1 cut", 14, dphibin);
  h_djetMETPhi_cut = new TH1F("h_djetMETPhi_cut", "djetMETPhi mindJMETPhi cut all", 14, dphibin);
  h_djetMETPhi_Nm1->Sumw2();
  h_djetMETPhi_cut->Sumw2();
  h_dr_phojet = new TH1F("h_dr_phojet", "pho jet dr", 20, 0., 8);
  h_dEta_phojet = new TH1F("h_dEta_phojet", "pho jet dEta", 32, 0., 8);
  h_dPhi_phojet = new TH1F("h_dPhi_phojet", "pho jet dPhi", 14, dphibin);
  h_dr_phojet->Sumw2();
  h_dEta_phojet->Sumw2();
  h_dPhi_phojet->Sumw2();       

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
  h_dr_jetjet_cut->Sumw2();
  h_minJMETdPhi_Nm1->Sumw2();
  h_minJMETdPhi_cut->Sumw2();
  h_dEta_jetjet_Nm1->Sumw2();
  h_dEta_jetjet_cut->Sumw2();
  h_dPhi_jetjet_cut->Sumw2();
  h_dPhi_jetjet_Nm1->Sumw2();
  h_dijetMass_Nm1->Sumw2();
  h_dijetMass_cut->Sumw2();
  h_phoCentral_Nm1->Sumw2();
  h_phoCentral_cut->Sumw2();
 
  TH1F *h_jetjetZdPhi_cut;
  TH1F *h_jetjetphodPhi_cut;
  TH1F *h_jet3Central_cut;

  h_jetjetZdPhi_cut = new TH1F("h_jetjetZdPhi_cut", "jetjet Z dPhi N cut", 14, dphibin);
  h_jetjetphodPhi_cut = new TH1F("h_jetjetphodPhi_cut", "jetjet pho dPhi N cut", 14, dphibin);
  h_jet3Central_cut = new TH1F("h_jet3Central_cut", "jet3Central N cut", 10, 0, 1);
  h_jetjetZdPhi_cut->Sumw2();
  h_jetjetphodPhi_cut->Sumw2();
  h_jet3Central_cut->Sumw2();

  //minJMETdPhi SB
  TH1F *h_MET_Nm1_minJMETdPhi_SB;
  TH1F *h_phoEB_ptcut_minJMETdPhi_SB;
  TH1F *h_jetpt_minJMETdPhi_SB;
  TH1F *h_dijetMass_minJMETdPhi_SB;
  
  h_MET_Nm1_minJMETdPhi_SB = new TH1F("h_MET_Nm1_minJMETdPhi_SB", "MET in minJMETdPhi SB region", 20, 0, 2000);
  h_phoEB_ptcut_minJMETdPhi_SB = new TH1F("h_phoEB_ptcut_minJmetdPhi_SB", "pho pT in minJMETdPhi SB region", 14, ptbin);
  h_jetpt_minJMETdPhi_SB = new TH1F("h_jetpt_minJMETdPhi_SB", "jet pT in minJMETdPhi SB region", 8, jetptbin);
  h_dijetMass_minJMETdPhi_SB = new TH1F("h_dijetMass_minJMETdPhi_SB", "dijetMass in minJMETdPhi SB region", 8, massbin);
  h_MET_Nm1_minJMETdPhi_SB->Sumw2();
  h_phoEB_ptcut_minJMETdPhi_SB->Sumw2();
  h_jetpt_minJMETdPhi_SB->Sumw2();
  h_dijetMass_minJMETdPhi_SB->Sumw2();

  //minJMETdPhi SR
  TH1F *h_MET_Nm1_minJMETdPhi_SR;
  TH1F *h_phoEB_ptcut_minJMETdPhi_SR;
  TH1F *h_jetpt_minJMETdPhi_SR;
  TH1F *h_dijetMass_minJMETdPhi_SR;

  h_MET_Nm1_minJMETdPhi_SR = new TH1F("h_MET_Nm1_minJMETdPhi_SR", "MET in minJMETdPhi SR region", 20, 0, 2000);
  h_phoEB_ptcut_minJMETdPhi_SR = new TH1F("h_phoEB_ptcut_minJmetdPhi_SR", "pho pT in minJMETdPhi SR region", 14, ptbin);
  h_jetpt_minJMETdPhi_SR = new TH1F("h_jetpt_minJMETdPhi_SR", "jet pT in minJMETdPhi SR region", 8, jetptbin);
  h_dijetMass_minJMETdPhi_SR = new TH1F("h_dijetMass_minJMETdPhi_SR", "dijetMass in minJMETdPhi SR region", 8, massbin);
  h_MET_Nm1_minJMETdPhi_SR->Sumw2();
  h_phoEB_ptcut_minJMETdPhi_SR->Sumw2();
  h_jetpt_minJMETdPhi_SR->Sumw2();
  h_dijetMass_minJMETdPhi_SR->Sumw2();

  // ptoverMET SR (ptoverMET < 2.4 && minJMETdPhi SB)
  TH1F *h_phoEB_ptcut_ptoverMET_CR_minJMETdPhi_SB;
  TH1F *h_jetpt_ptoverMET_CR_minJMETdPhi_SB;
  TH1F *h_MET_ptoverMET_CR_minJMETdPhi_SB;
  TH1F *h_dphoMETPhi_ptoverMET_CR_minJMETdPhi_SB;
  TH1F *h_dijetMass_ptoverMET_CR_minJMETdPhi_SB;
  TH1F *h_dEta_jetjet_ptoverMET_CR_minJMETdPhi_SB;
  TH1F *h_minJMETdPhi_ptoverMET_CR_minJMETdPhi_SB;

  h_phoEB_ptcut_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_phoEB_ptcut_ptoverMET_CR_minJMETdPhi_SB", "", 14, ptbin);
  h_jetpt_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_jetpt_ptoverMET_CR_minJMETdPhi_SB", "", 8, jetptbin);
  h_MET_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_MET_ptoverMET_CR_minJMETdPhi_SB", "", 11, METbin);
  h_dphoMETPhi_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_dphoMETPhi_ptoverMET_CR_minJMETdPhi_SB", "", 14, dphibin);
  h_minJMETdPhi_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_minJMETdPhi_ptoverMET_CR_minJMETdPhi_SB", "", 14, dphibin);
  h_dijetMass_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_dijetMass_ptoverMET_CR_minJMETdPhi_SB", "", 8, massbin);
  h_dEta_jetjet_ptoverMET_CR_minJMETdPhi_SB = new TH1F("h_dEta_jetjet_ptoverMET_CR_minJMETdPhi_SB", "", 32, 0., 8);
  h_phoEB_ptcut_ptoverMET_CR_minJMETdPhi_SB->Sumw2();
  h_jetpt_ptoverMET_CR_minJMETdPhi_SB->Sumw2();
  h_MET_ptoverMET_CR_minJMETdPhi_SB->Sumw2();
  h_dphoMETPhi_ptoverMET_CR_minJMETdPhi_SB->Sumw2();
  h_dijetMass_ptoverMET_CR_minJMETdPhi_SB->Sumw2();
  h_dEta_jetjet_ptoverMET_CR_minJMETdPhi_SB->Sumw2();
  h_minJMETdPhi_ptoverMET_CR_minJMETdPhi_SB->Sumw2();

  // ptoverMET CR (ptoverMET > 2.4 && minJMETdPhi SB)
  TH1F *h_phoEB_ptcut_ptoverMET_CR;
  TH1F *h_jetpt_ptoverMET_CR;
  TH1F *h_MET_ptoverMET_CR;
  TH1F *h_dphoMETPhi_ptoverMET_CR;
  TH1F *h_dijetMass_ptoverMET_CR;
  TH1F *h_dEta_jetjet_ptoverMET_CR;
  TH1F *h_minJMETdPhi_ptoverMET_CR;

  h_phoEB_ptcut_ptoverMET_CR = new TH1F("h_phoEB_ptcut_ptoverMET_CR", "", 14, ptbin);
  h_jetpt_ptoverMET_CR = new TH1F("h_jetpt_ptoverMET_CR", "", 8, jetptbin);
  h_MET_ptoverMET_CR = new TH1F("h_MET_ptoverMET_CR", "", 11, METbin);
  h_dphoMETPhi_ptoverMET_CR = new TH1F("h_dphoMETPhi_ptoverMET_CR", "", 14, dphibin);
  h_minJMETdPhi_ptoverMET_CR = new TH1F("h_minJMETdPhi_ptoverMET_CR", "", 14, dphibin);
  h_dijetMass_ptoverMET_CR = new TH1F("h_dijetMass_ptoverMET_CR", "", 8, massbin);
  h_dEta_jetjet_ptoverMET_CR = new TH1F("h_dEta_jetjet_ptoverMET_CR", "", 32, 0., 8);
  h_phoEB_ptcut_ptoverMET_CR->Sumw2();
  h_jetpt_ptoverMET_CR->Sumw2();
  h_MET_ptoverMET_CR->Sumw2();
  h_dphoMETPhi_ptoverMET_CR->Sumw2();
  h_dijetMass_ptoverMET_CR->Sumw2();
  h_dEta_jetjet_ptoverMET_CR->Sumw2();
  h_minJMETdPhi_ptoverMET_CR->Sumw2();

  // ptoverMET SR (ptoverMET < 2.2)
  TH1F *h_phoEB_ptcut_ptoverMET_SR_minJMETdPhi_SB;
  TH1F *h_jetpt_ptoverMET_SR_minJMETdPhi_SB;
  TH1F *h_MET_ptoverMET_SR_minJMETdPhi_SB;
  TH1F *h_dphoMETPhi_ptoverMET_SR_minJMETdPhi_SB;
  TH1F *h_dijetMass_ptoverMET_SR_minJMETdPhi_SB;
  TH1F *h_dEta_jetjet_ptoverMET_SR_minJMETdPhi_SB;
  TH1F *h_minJMETdPhi_ptoverMET_SR_minJMETdPhi_SB;

  h_phoEB_ptcut_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_phoEB_ptcut_ptoverMET_SR_minJMETdPhi_SB", "", 14, ptbin);
  h_jetpt_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_jetpt_ptoverMET_SR_minJMETdPhi_SB", "", 8, jetptbin);
  h_MET_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_MET_ptoverMET_SR_minJMETdPhi_SB", "", 11, METbin);
  h_dphoMETPhi_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_dphoMETPhi_ptoverMET_SR_minJMETdPhi_SB", "", 14, dphibin);
  h_minJMETdPhi_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_minJMETdPhi_ptoverMET_SR_minJMETdPhi_SB", "", 14, dphibin);
  h_dijetMass_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_dijetMass_ptoverMET_SR_minJMETdPhi_SB", "", 8, massbin);
  h_dEta_jetjet_ptoverMET_SR_minJMETdPhi_SB = new TH1F("h_dEta_jetjet_ptoverMET_SR_minJMETdPhi_SB", "", 32, 0., 8);
  h_phoEB_ptcut_ptoverMET_SR_minJMETdPhi_SB->Sumw2();
  h_jetpt_ptoverMET_SR_minJMETdPhi_SB->Sumw2();
  h_MET_ptoverMET_SR_minJMETdPhi_SB->Sumw2();
  h_dphoMETPhi_ptoverMET_SR_minJMETdPhi_SB->Sumw2();
  h_dijetMass_ptoverMET_SR_minJMETdPhi_SB->Sumw2();
  h_dEta_jetjet_ptoverMET_SR_minJMETdPhi_SB->Sumw2();
  h_minJMETdPhi_ptoverMET_SR_minJMETdPhi_SB->Sumw2();

  // ptoverMET SR (ptoverMET < 2.2)
  TH1F *h_phoEB_ptcut_ptoverMET_SR;
  TH1F *h_jetpt_ptoverMET_SR;
  TH1F *h_MET_ptoverMET_SR;
  TH1F *h_dphoMETPhi_ptoverMET_SR;
  TH1F *h_dijetMass_ptoverMET_SR;
  TH1F *h_dEta_jetjet_ptoverMET_SR;
  TH1F *h_minJMETdPhi_ptoverMET_SR;

  h_phoEB_ptcut_ptoverMET_SR = new TH1F("h_phoEB_ptcut_ptoverMET_SR", "", 14, ptbin);
  h_jetpt_ptoverMET_SR = new TH1F("h_jetpt_ptoverMET_SR", "", 8, jetptbin);
  h_MET_ptoverMET_SR = new TH1F("h_MET_ptoverMET_SR", "", 11, METbin);
  h_dphoMETPhi_ptoverMET_SR = new TH1F("h_dphoMETPhi_ptoverMET_SR", "", 14, dphibin);
  h_minJMETdPhi_ptoverMET_SR = new TH1F("h_minJMETdPhi_ptoverMET_SR", "", 14, dphibin);
  h_dijetMass_ptoverMET_SR = new TH1F("h_dijetMass_ptoverMET_SR", "", 8, massbin);
  h_dEta_jetjet_ptoverMET_SR = new TH1F("h_dEta_jetjet_ptoverMET_SR", "", 32, 0., 8);
  h_phoEB_ptcut_ptoverMET_SR->Sumw2();
  h_jetpt_ptoverMET_SR->Sumw2();
  h_MET_ptoverMET_SR->Sumw2();
  h_dphoMETPhi_ptoverMET_SR->Sumw2();
  h_dijetMass_ptoverMET_SR->Sumw2();
  h_dEta_jetjet_ptoverMET_SR->Sumw2();
  h_minJMETdPhi_ptoverMET_SR->Sumw2();

  TH2F *h2_nvtx_METX = new TH2F("h2_nvtx_METX", "h2_nvtx_METX", 100, 0, 100, 100, -500, 500);
  TH2F *h2_nvtx_METY = new TH2F("h2_nvtx_METY", "h2_nvtx_METY", 100, 0, 100, 100, -500, 500);
  
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
  h2_minJMETdPhi_ptoverMET = new TH2F("h2_minJMETdPhi_ptoverMET", "minJMETdPhi and ptoverMET relation", 14, dphibin, 10, 0, 5);
  h2_MET_minJMETdPhi->Sumw2();
  h2_MET_dijetMass->Sumw2();
  h2_MET_ptoverMET->Sumw2();
  h2_dijetMass_minJMETdPhi->Sumw2();
  h2_dijetMass_ptoverMET->Sumw2();
  h2_minJMETdPhi_ptoverMET->Sumw2();

  //input variable
  Bool_t	isData		      = 0;
  Int_t		nGoodVtx	      = 0;
  Bool_t	isPVGood	      = 0;
  Float_t	EventWeight	      = 0;
  Float_t	rho		      = 0;
  Int_t*        phoIsMatch	      = 0;
  Int_t         nPho		      = 0;
  Float_t*	phoE		      = 0; 
  Float_t*	phoEt		      = 0;
  Float_t*	phoEta		      = 0;
  Float_t*	phoSCEta	      = 0;
  Float_t*	phoPhi		      = 0;
  //Float_t*      phoPFChIso	      = 0;
  //Float_t*      phoPFChWorstIso	      = 0;
  //Float_t*      phoPFChIso_rc	      = 0;
  //Float_t*      phoPFChWorstIso_newEA = 0;
  Float_t*	phoSeedTime	      = 0;
  Float_t*	phoMIP		      = 0;
  Float_t       corrMET		      = 0;
  Float_t       corrMETPhi	      = 0;
  Int_t		nJet		      = 0;
  Int_t		npfjet		      = 0;
  Int_t         nSMjet		      = 0;
  Int_t         nVBSjet		      = 0;
  Float_t*	jetPt		      = 0;
  Float_t*	jetEta		      = 0;
  Float_t*	jetPhi		      = 0;
  Float_t*	jetEn		      = 0;
  Int_t*	nonPUjetid	      = 0;
  Double_t	dijetMass	      = 0;
  Int_t*        jetID		      = 0;
  Int_t*        jetPUIDMVA	      = 0;
  Float_t*      jetPUID		      = 0;
  //Float_t*	jetCHF		      = 0;				
  //Float_t*	jetNHF		      = 0;				
  //Float_t*	jetNEF		      = 0;				
  //Int_t*	jetNCH		      = 0;				
  //Int_t*	jetNNP		      = 0;
  Int_t		nLep		      = 0;
  Int_t*	cutflowSMID	      = 0; 
  Int_t*	cutflowVBSID	      = 0;
  Float_t*	phoMETdPhi	      = 0;
  Float_t phoCentral = 0;
  Float_t       minJMETdPhi	      = 0;
  Float_t*	JMETdPhi	      = 0;
  Float_t	jetjetdEta	      = 0;
  Float_t	jetjetdPhi	      = 0;
  Float_t	jetjetdR	      = 0;
  Float_t*	phojetdEta	      = 0;
  Float_t*	phojetdPhi	      = 0;
  Float_t*	phojetdR	      = 0;
  Float_t jetjetZdPhi = 0;
  Float_t jetjetphodPhi = 0;
  Float_t jet3Central = 0;
  
  
  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);
    //PVGood	= data.GetBool(		"isPVGood"		);
    //(isPVGood == 0) continue;
    
    isPVGood				 =	data.GetBool(		"isPVGood"		);
    nGoodVtx				 =	data.GetInt(		"nGoodVtx"		);
    EventWeight				 =	data.GetFloat(	"EventWeight"		);
    rho					 =	data.GetFloat(		"rho"			);
    phoIsMatch				 =	data.GetPtrInt(		"phoIsMatch"		);
    ///reco pho
    nPho				 =	data.GetInt(		"nPho"			);
    phoE				 =	data.GetPtrFloat(	"phoE"			);
    phoEt				 =	data.GetPtrFloat(	"phoEt"			);
    phoEta				 =	data.GetPtrFloat(	"phoEta"		);
    phoSCEta				 =	data.GetPtrFloat(	"phoSCEta"		);
    phoPhi				 =	data.GetPtrFloat(	"phoPhi"		);
    //phoPFChIso				 =	data.GetPtrFloat(	"phoPFChIso"		);
    //phoPFChWorstIso			 =	data.GetPtrFloat(	"phoPFChWorstIso"	);
    //phoPFChIso_rc			 =	data.GetPtrFloat(	"phoPFChIso_rc"		);
    //phoPFChWorstIso_newEA		 =	data.GetPtrFloat(	"phoPFChWorstIso_newEA"	);
    phoSeedTime				 =	data.GetPtrFloat(	"phoSeedTime"		);
    phoMIP				 =	data.GetPtrFloat(	"phoMIPTotEnergy"	);
    ///MET
    corrMET				 =	data.GetFloat(		"corrMET"		);
    corrMETPhi				 =	data.GetFloat(		"corrMETPhi"		);
    ///jet
    nJet				 =	data.GetInt(		"nJet"			);
    npfjet				 =	data.GetInt(		"npfjet"		);
    nSMjet				 =	data.GetInt(            "nSMjet"                );
    nVBSjet				 =	data.GetInt(            "nVBSjet"		);
    jetPt				 =	data.GetPtrFloat(	"jetPtSmear"		);
    jetEta				 =	data.GetPtrFloat(	"jetEta"		);
    jetPhi				 =	data.GetPtrFloat(	"jetPhi"		);
    jetEn				 =	data.GetPtrFloat(	"jetEn"			);
    nonPUjetid				 =	data.GetPtrInt(		"nonPUjetid"		);
    dijetMass				 =	data.GetDouble(		"dijetMass"		);
    if(sig == 1) jetID			 =	data.GetPtrInt(		"jetULTID"		);
    else if(sig == 0) jetID	         =      data.GetPtrInt(		"jetTID"		);
    nLep	=	data.GetInt(					"nLep"			);
    phoMETdPhi	=	data.GetPtrFloat(				"phoMETdPhi"		);
    phoCentral = data.GetFloat("phoCentral");
    minJMETdPhi	=	data.GetFloat(					"minJMETdPhi"           );
    JMETdPhi	=	data.GetPtrFloat(				"JMETdPhi"		);
    jetjetdR	=	data.GetFloat(					"jetjetdR"		);
    jetjetdEta	=	data.GetFloat(					"jetjetdEta"		);
    jetjetdPhi	=	data.GetFloat(					"jetjetdPhi"		);
    phojetdR	=	data.GetPtrFloat(				"phojetdR"		);
    phojetdEta	=	data.GetPtrFloat(				"phojetdEta"		);
    phojetdPhi	=	data.GetPtrFloat(				"phojetdPhi"		);
    jetjetZdPhi = data.GetFloat("jetjetZdPhi");
    jetjetphodPhi = data.GetFloat("jetjetphodPhi");
    jet3Central = data.GetFloat("jet3Central");
    //cout << "get data" << en
    
    //MET XY corr
    Float_t METX = corrMET*cos(corrMETPhi);
    h2_nvtx_METX->Fill(nGoodVtx, METX);
    Float_t METY = corrMET*sin(corrMETPhi);
    h2_nvtx_METY->Fill(nGoodVtx, METY);
    //cout << "get MET corr" << endl;
    
    //cutflow ID
    cutflowSMID = data.GetPtrInt("cutflowSMID");
    cutflowVBSID = data.GetPtrInt("cutflowVBSID");
    //cout << "get cutflow corr" << endl;
    
    //Int_t leadingpho = 1;
    Int_t fillpho[2];
    for(Int_t ipho=0; ipho<nPho; ipho++){
      Int_t cutflow[2];
      cutflow[0] = cutflowSMID[ipho];
      cutflow[1] = cutflowVBSID[ipho];
      WEIGHT = EventWeight;

      for(Int_t icat=0; icat<2; icat++){

	fillpho[icat] = 0;
	if(pho_sel(cutflow[icat], 16) == 1) fillpho[icat] += 1;
	if(fillpho[icat]>1) continue;
	if(icat == 0) npfjet = nSMjet;
	else if(icat == 1) npfjet = nVBSjet;
 
	if(pho_sel(cutflow[icat],3) == 1) h_phoEB_pt_210[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat],4) == 1) h_phoEB_pt_M[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat],6) == 1) h_phoEB_pt_leptonveto[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat],7) == 1) h_phoEB_pt_MET[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat],8) == 1) h_phoEB_pt_dphoMETPhi[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat],9) == 1) h_phoEB_pt_jetveto[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat],11) == 1) h_phoEB_pt_jetjetdEta[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat],12) == 1) h_phoEB_pt_jetjetdPhi[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat],13) == 1) h_phoEB_pt_phojetdR[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat],14) == 1) h_phoEB_pt_phoCentral[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat],15) == 1) h_phoEB_pt_mindJMETPhi[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat],16) == 1) h_phoEB_pt_dijetMass[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat],16) == 1) {

	  //results
	  h_phoEB_ptcut[icat]->Fill(phoEt[ipho], WEIGHT);
	  h_phoEB_Etacut[icat]->Fill(phoEta[ipho], WEIGHT);
	  h_phoEB_Phicut[icat]->Fill(phoPhi[ipho], WEIGHT);
	  h_MIP_cut[icat]->Fill(phoMIP[ipho], WEIGHT);
	  h_MET_cut[icat]->Fill(corrMET, WEIGHT);
	  h_METPhi_cut[icat]->Fill(corrMETPhi, WEIGHT);
	  h_dphoMETPhi_cut[icat]->Fill(phoMETdPhi[ipho], WEIGHT);
	  h_ptoverMET_cut[icat]->Fill(phoEt[ipho]/corrMET, WEIGHT);
	  h_nvtx_cut[icat]->Fill(nGoodVtx, WEIGHT);
	  h_njet_cut[icat]->Fill(npfjet, WEIGHT);

	  for(Int_t jj=0; jj<npfjet; jj++){
	    if(jj>1) continue;
	    Int_t ijet = nonPUjetid[jj];
	    h_jetpt_cut[icat]->Fill(jetPt[ijet], WEIGHT);
	    h_jetEta_cut[icat]->Fill(jetEta[ijet], WEIGHT);
	    h_jetPhi_cut[icat]->Fill(jetPhi[ijet], WEIGHT);
	  }

	  //VBS results
	  if(icat == 1){
	    for(Int_t jj=0; jj<npfjet; jj++){
	      Int_t ijet = nonPUjetid[jj];
	      if(jj>1) continue;
	      h_djetMETPhi_cut->Fill(JMETdPhi[ijet], WEIGHT);
	      h_dr_phojet->Fill(phojetdR[ijet], WEIGHT);
	      h_dEta_phojet->Fill(phojetdEta[ijet], WEIGHT);
	      h_dPhi_phojet->Fill(phojetdPhi[ijet], WEIGHT);
	      if(h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) >= 12 && h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) <= 20) h_jetpt_ptoverMET_CR->Fill(jetPt[ijet], WEIGHT);
	      else if(h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) >= 1 && h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) <= 11) h_jetpt_ptoverMET_SR->Fill(jetPt[ijet], WEIGHT);
	      //if(h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) >= 1 && h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) <= 11) h_jetpt_ptoverMET_SR->Fill(jetPt[ijet], WEIGHT);
	    }
	    h_minJMETdPhi_cut->Fill(minJMETdPhi, WEIGHT);
	    h_dr_jetjet_cut->Fill(jetjetdR, WEIGHT);
	    h_dEta_jetjet_cut->Fill(jetjetdEta, WEIGHT);
	    h_dPhi_jetjet_cut->Fill(jetjetdPhi, WEIGHT);
	    h_dijetMass_cut->Fill(dijetMass, WEIGHT);
	    h_phoCentral_cut->Fill(phoCentral, WEIGHT);

	    h_jetjetZdPhi_cut->Fill(jetjetZdPhi, WEIGHT);
	    h_jetjetphodPhi_cut->Fill(jetjetphodPhi, WEIGHT);
	    h_jet3Central_cut->Fill(jet3Central, WEIGHT);

	    //if((phoEt[ipho]/corrMET) >= 2 && (phoEt[ipho]/corrMET) <= 4){//ptoverMET CR
	    if(h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) >= 12 && h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) <= 20){//ptoverMET CR
	      //ptoverMET_CR_count++;
	      h_phoEB_ptcut_ptoverMET_CR->Fill(phoEt[ipho], WEIGHT);
	      h_MET_ptoverMET_CR->Fill(corrMET, WEIGHT);
	      h_dphoMETPhi_ptoverMET_CR->Fill(phoMETdPhi[ipho], WEIGHT);
	      h_dijetMass_ptoverMET_CR->Fill(dijetMass, WEIGHT);
	      h_dEta_jetjet_ptoverMET_CR->Fill(jetjetdEta, WEIGHT);
	      h_minJMETdPhi_ptoverMET_CR->Fill(minJMETdPhi, WEIGHT);
	    }
	    //else if((phoEt[ipho]/corrMET) < 2){//ptoverMET SR
	    else if(h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) >= 1 && h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) <= 11){//ptoverMET SR
	      //if(h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) >= 1 && h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) <= 11){//ptoverMET SR
	      h_phoEB_ptcut_ptoverMET_SR->Fill(phoEt[ipho], WEIGHT);
	      h_MET_ptoverMET_SR->Fill(corrMET, WEIGHT);
	      h_dphoMETPhi_ptoverMET_SR->Fill(phoMETdPhi[ipho], WEIGHT);
	      h_dijetMass_ptoverMET_SR->Fill(dijetMass, WEIGHT);
	      h_dEta_jetjet_ptoverMET_SR->Fill(jetjetdEta, WEIGHT);
	      h_minJMETdPhi_ptoverMET_SR->Fill(minJMETdPhi, WEIGHT);
	    }
	    	    
	  }
	}
	
	//Nm1 plots
	if(Nm1_sel(cutflow[icat],7) == 1){
	  h_MET_Nm1[icat]->Fill(corrMET, WEIGHT);
	  h_METPhi_Nm1[icat]->Fill(corrMETPhi, WEIGHT);
	}
	if(Nm1_sel(cutflow[icat],8) == 1) h_dphoMETPhi_Nm1[icat]->Fill(phoMETdPhi[ipho], WEIGHT);
	if(Nm1_sel(cutflow[icat],9) == 1) h_njet_Nm1[icat]->Fill(npfjet, WEIGHT);
	if(Nm1_sel(cutflow[icat],14) == 1) h_ptoverMET_Nm1[icat]->Fill(phoEt[ipho]/corrMET, WEIGHT);
	
	//VBS Nm1 plots
    	if(icat == 1){

	  if(Nm1_sel(cutflow[icat],15) == 1){
	    for(Int_t jj=0; jj<npfjet; jj++){
	      if(jj>1) continue;
	      Int_t ijet = nonPUjetid[jj];
	      h_djetMETPhi_Nm1->Fill(JMETdPhi[ijet], WEIGHT);
	    }
	    h_minJMETdPhi_Nm1->Fill(minJMETdPhi, WEIGHT);
	  }
	  if(Nm2_sel(cutflow[icat],10,11) == 1) h_dEta_jetjet_Nm1->Fill(jetjetdEta, WEIGHT);
	  if(Nm1_sel(cutflow[icat],12) == 1) h_dPhi_jetjet_Nm1->Fill(jetjetdPhi, WEIGHT);
	  if(Nm1_sel(cutflow[icat],16) == 1) h_dijetMass_Nm1->Fill(dijetMass, WEIGHT);
	  if(Nm1_sel(cutflow[icat],14) == 1) h_phoCentral_Nm1->Fill(phoCentral, WEIGHT);
	  
	  if(Nm2_sel(cutflow[icat], 7, 15) == 1 && minJMETdPhi<0.5) h_MET_Nm1_minJMETdPhi_SB->Fill(corrMET, WEIGHT);
	  if(Nm1_sel(cutflow[icat], 7) == 1) h_MET_Nm1_minJMETdPhi_SR->Fill(corrMET, WEIGHT);

	  //2d plots
	  if(Nm2_sel(cutflow[icat],7,15) == 1) h2_MET_minJMETdPhi->Fill(corrMET, minJMETdPhi, WEIGHT);
	  if(Nm2_sel(cutflow[icat],7,16) == 1) h2_MET_dijetMass->Fill(corrMET, dijetMass, WEIGHT);
	  if(Nm2_sel(cutflow[icat],7,14) == 1) h2_MET_ptoverMET->Fill(corrMET, phoEt[ipho]/corrMET, WEIGHT);
	  if(Nm2_sel(cutflow[icat],15,16) == 1) h2_dijetMass_minJMETdPhi->Fill(dijetMass, minJMETdPhi, WEIGHT);
	  if(Nm2_sel(cutflow[icat],14,16) == 1) h2_dijetMass_ptoverMET->Fill(dijetMass, phoEt[ipho]/corrMET, WEIGHT);
	  if(Nm2_sel(cutflow[icat],14,15) == 1) h2_minJMETdPhi_ptoverMET->Fill(minJMETdPhi, phoEt[ipho]/corrMET, WEIGHT);

	  if(Nm1_sel(cutflow[icat], 15) == 1 && minJMETdPhi<0.5){

	    for(Int_t jj=0; jj<npfjet; jj++){
	      Int_t ijet = nonPUjetid[jj];
	      if(jj>1) continue;
	      if(h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) >= 12 && h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) <= 20) h_jetpt_ptoverMET_CR->Fill(jetPt[ijet], WEIGHT);
	    }

	    if(h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) >= 12 && h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) <= 20){//ptoverMET CR
	      //ptoverMET_CR_count++;
	      h_phoEB_ptcut_ptoverMET_CR_minJMETdPhi_SB->Fill(phoEt[ipho], WEIGHT);
	      h_MET_ptoverMET_CR_minJMETdPhi_SB->Fill(corrMET, WEIGHT);
	      h_dphoMETPhi_ptoverMET_CR_minJMETdPhi_SB->Fill(phoMETdPhi[ipho], WEIGHT);
	      h_dijetMass_ptoverMET_CR_minJMETdPhi_SB->Fill(dijetMass, WEIGHT);
	      h_dEta_jetjet_ptoverMET_CR_minJMETdPhi_SB->Fill(jetjetdEta, WEIGHT);
	      h_minJMETdPhi_ptoverMET_CR_minJMETdPhi_SB->Fill(minJMETdPhi, WEIGHT);
	    }
	    
	    if(h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) >= 1 && h_ptoverMET_cut[icat]->GetXaxis()->FindBin(phoEt[ipho]/corrMET) <= 11){//ptoverMET SR
	      h_phoEB_ptcut_ptoverMET_SR_minJMETdPhi_SB->Fill(phoEt[ipho], WEIGHT);
	      h_MET_ptoverMET_SR_minJMETdPhi_SB->Fill(corrMET, WEIGHT);
	      h_dphoMETPhi_ptoverMET_SR_minJMETdPhi_SB->Fill(phoMETdPhi[ipho], WEIGHT);
	      h_dijetMass_ptoverMET_SR_minJMETdPhi_SB->Fill(dijetMass, WEIGHT);
	      h_dEta_jetjet_ptoverMET_SR_minJMETdPhi_SB->Fill(jetjetdEta, WEIGHT);
	      h_minJMETdPhi_ptoverMET_SR_minJMETdPhi_SB->Fill(minJMETdPhi, WEIGHT);
	    }
	  }
	  
	}
	
      }

      
      ///
    }
    //cout << "get cutflow plot"  << endl;
    
    
  }
  //****************END LOOP**********************//

  //****************Terminate*********************//
  fout_->cd();
  //outtree_->Write();

  hEvents->Write();
  hSumofGenW->Write();
  
  h_MET_Nm1_minJMETdPhi_SB->Write();
  h_MET_Nm1_minJMETdPhi_SR->Write();

  fout_->mkdir("SMandVBS");
  fout_->cd("SMandVBS");
  for(Int_t j=0; j<2; j++){
    h_phoEB_pt_210[j]->Write();
    h_phoEB_pt_M[j]->Write();
    h_phoEB_pt_leptonveto[j]->Write();
    h_phoEB_pt_MET[j]->Write();
    h_phoEB_pt_dphoMETPhi[j]->Write();
    h_phoEB_pt_jetjetdEta[j]->Write();
    h_phoEB_pt_jetjetdPhi[j]->Write();
    h_phoEB_pt_phojetdR[j]->Write();
    h_phoEB_pt_phoCentral[j]->Write();
    h_phoEB_pt_mindJMETPhi[j]->Write();
    h_phoEB_pt_dijetMass[j]->Write();
    h_phoEB_pt_jetveto[j]->Write();

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

    h_ptoverMET_Nm1[j]->Write();
    h_ptoverMET_cut[j]->Write();

    h_nvtx_cut[j]->Write();
    
    h_njet_Nm1[j]->Write();
    h_njet_cut[j]->Write();
    
  }

  fout_->mkdir("dijet");
  fout_->cd("dijet");
  
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

  fout_->mkdir("ptoverMET_CR_minJMETdPhi_SB");
  fout_->cd("ptoverMET_CR_minJMETdPhi_SB");

  h_phoEB_ptcut_ptoverMET_CR_minJMETdPhi_SB->Write();
  h_jetpt_ptoverMET_CR_minJMETdPhi_SB->Write();
  h_MET_ptoverMET_CR_minJMETdPhi_SB->Write();
  h_dphoMETPhi_ptoverMET_CR_minJMETdPhi_SB->Write();
  h_dijetMass_ptoverMET_CR_minJMETdPhi_SB->Write();
  h_dEta_jetjet_ptoverMET_CR_minJMETdPhi_SB->Write();
  h_minJMETdPhi_ptoverMET_CR_minJMETdPhi_SB->Write();

  
  fout_->mkdir("ptoverMET_CR");
  fout_->cd("ptoverMET_CR");

  h_phoEB_ptcut_ptoverMET_CR->Write();
  h_jetpt_ptoverMET_CR->Write();
  h_MET_ptoverMET_CR->Write();
  h_dphoMETPhi_ptoverMET_CR->Write();
  h_dijetMass_ptoverMET_CR->Write();
  h_dEta_jetjet_ptoverMET_CR->Write();
  h_minJMETdPhi_ptoverMET_CR->Write();

  fout_->mkdir("ptoverMET_SR_minJMETdPhi_SB");
  fout_->cd("ptoverMET_SR_minJMETdPhi_SB");

  h_phoEB_ptcut_ptoverMET_SR_minJMETdPhi_SB->Write();
  h_jetpt_ptoverMET_SR_minJMETdPhi_SB->Write();
  h_MET_ptoverMET_SR_minJMETdPhi_SB->Write();
  h_dphoMETPhi_ptoverMET_SR_minJMETdPhi_SB->Write();
  h_dijetMass_ptoverMET_SR_minJMETdPhi_SB->Write();
  h_dEta_jetjet_ptoverMET_SR_minJMETdPhi_SB->Write();
  h_minJMETdPhi_ptoverMET_SR_minJMETdPhi_SB->Write();

  fout_->mkdir("ptoverMET_SR");
  fout_->cd("ptoverMET_SR");

  h_phoEB_ptcut_ptoverMET_SR->Write();
  h_jetpt_ptoverMET_SR->Write();
  h_MET_ptoverMET_SR->Write();
  h_dphoMETPhi_ptoverMET_SR->Write();
  h_dijetMass_ptoverMET_SR->Write();
  h_dEta_jetjet_ptoverMET_SR->Write();
  h_minJMETdPhi_ptoverMET_SR->Write();

  fout_->mkdir("h2_plots");
  fout_->cd("h2_plots");

  h2_MET_minJMETdPhi->Write();
  h2_MET_dijetMass->Write();
  h2_MET_ptoverMET->Write();
  h2_dijetMass_minJMETdPhi->Write();
  h2_dijetMass_ptoverMET->Write();
  h2_minJMETdPhi_ptoverMET->Write();
  
  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());
}
