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

void xZg1(char* pathes){
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

  //chworst pileup indept. study profile
  Float_t EAbin[10] = {0.0, 1.0, 1.479, 2.0, 2.2, 2.3, 2.4, 3};//8 bins
  TH2F *h2_chIso_rho[7];
  TH2F *h2_chworst_rho[7];
  for(Int_t i=0; i<7; i++){
    h2_chIso_rho[i] = new TH2F(Form("h2_chIso_rho_eta%i", i), Form("h2_chIso_rho_eta%i", i), 60, 0, 30, 30, 0, 3);
    h2_chworst_rho[i] = new TH2F(Form("h2_chworst_rho_eta%i",i), Form("h2_chworst_rho_eta%i", i), 60, 0, 30, 30, 0, 3);
    h2_chIso_rho[i]->Sumw2();
    h2_chworst_rho[i]->Sumw2();
  }

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
  TH1F *h_MET_test[2][4];//[80, 100, 140, 170]
  TH1F *h_MET_Nm1_djetMETPhi_SB[2][8];
  TH1F *h_MET_Nm1_djetMETPhim0p5[2];
  
  TH1F *h_METPhi_Nm1[2];
  TH1F *h_METPhi_cut[2];

  TH1F *h_dphoMETPhi_Nm1[2];
  TH1F *h_dphoMETPhi_cut[2];
  TH1F *h_dphoMETPhi_test[2][4];//[0.5, 0.8, 1.0, 1.2]
    
  TH1F *h_phoEB_ptoverMET_cut[2];
  
  TH1F *h_nvtx_cut[2];

  TH1F *h_njet_Nm1[2];
  TH1F *h_njet_cut[2];

  TH1F *h_nlep_cut[2];
  
  for(Int_t j=0; j<2; j++){
    h_phoEB_pt_210[j] = new TH1F(Form("h_phoEB_pt_210_%i", j), "matched phoEB pt pt200 cut", 22, ptbin);
    h_phoEB_pt_M[j] = new TH1F(Form("h_phoEB_pt_M_%i", j), "matched phoEB pt M IDcut", 22, ptbin);
    h_phoEB_pt_leptonveto[j] = new TH1F(Form("h_phoEB_pt_leptonveto_%i", j), "leptonveto cut", 22, ptbin);
    h_phoEB_pt_MET[j] = new TH1F(Form("h_phoEB_pt_MET_%i", j), "matched phoEB pt MET cut", 22, ptbin);
    h_phoEB_pt_dphoMETPhi[j] = new TH1F(Form("h_phoEB_pt_dphoMETPhi_%i", j), "matched phoEB pt dphoMETPhi cut", 22, ptbin);
    h_phoEB_pt_djetMETPhi[j] = new TH1F(Form("h_phoEB_pt_djetMETPhi_%i", j), "matched phoEB pt djetMETPhi cut", 22, ptbin);
    h_phoEB_pt_jetveto[j] = new TH1F(Form("h_phoEB_pt_jetveto_%i", j), Form("h_phoEB_pt_jetveto_%i", j), 22, ptbin);  
    h_phoEB_pt_210[j]->Sumw2();
    h_phoEB_pt_M[j]->Sumw2();
    h_phoEB_pt_leptonveto[j]->Sumw2();
    h_phoEB_pt_MET[j]->Sumw2();
    h_phoEB_pt_dphoMETPhi[j]->Sumw2();
    h_phoEB_pt_djetMETPhi[j]->Sumw2();
    
    h_phoEB_ptcut[j] = new TH1F(Form("h_phoEB_ptcut_%i", j), "phoEB pt cut all pas varbin", 20, 200, 1000);
    h_phoEB_Etacut[j] = new TH1F(Form("h_phoEB_Etacut_%i", j), "phoEB eta cut all pas varbins", 6, etabin);
    h_phoEB_Phicut[j] = new TH1F(Form("h_phoEB_Phicut_%i", j), "phoEB phi cut all pas varbins", 30, -3.14, 3.14);
    h_phoEB_ptcut[j]->Sumw2();
    h_phoEB_Etacut[j]->Sumw2();
    h_phoEB_Phicut[j]->Sumw2();
        
    h_MIP_Nm1[j] = new TH1F(Form("h_MIP_Nm1_%i", j), "MIP energy N-1 cut", 50, 0, 10);
    h_MIP_cut[j] = new TH1F(Form("h_MIP_cut_%i", j), "MIP energy N cut", 50, 0, 10);
    h_MIP_Nm1[j]->Sumw2();
    h_MIP_cut[j]->Sumw2();
    
    h_MET_Nm1[j] = new TH1F(Form("h_MET_Nm1_%i", j), "pf MET N-1 cut", 60, 0, 1200);
    h_MET_cut[j] = new TH1F(Form("h_MET_cut_%i", j), "pf MET N cut", 60, 0, 1200);
    h_MET_Nm1[j]->Sumw2();
    h_MET_cut[j]->Sumw2();

    
    for(Int_t ii=0; ii<4; ii++){
      h_MET_test[j][ii] = new TH1F(Form("h_MET_test_cut%i_%i", ii, j), "MET cut at diff val", 22, ptbin);
      h_MET_test[j][ii]->Sumw2();
    }

    h_MET_Nm1_djetMETPhim0p5[j] = new TH1F(Form("h_MET_Nm1_djetMETPhim0p5_%i", j), "pf MET N-1 cut with djetMETPhi<0.5", 60, 0, 1200);
    h_MET_Nm1_djetMETPhim0p5[j]->Sumw2();
    
    for(Int_t ii=0; ii<8; ii++){
      h_MET_Nm1_djetMETPhi_SB[j][ii] = new TH1F(Form("h_MET_Nm1_djetMETPhi_SB0p%i_%i", ii+2, j), Form("pfMET N-1 cut with djetMETPhi<0.%i", ii+2), 60, 0, 1200);
      h_MET_Nm1_djetMETPhi_SB[j][ii]->Sumw2();
    }
    
    h_METPhi_Nm1[j] = new TH1F(Form("h_METPhi_Nm1_%i", j), "pf MET N-1 cut", 30, -3.14, 3.14);
    h_METPhi_cut[j] = new TH1F(Form("h_METPhi_cut_%i", j), "pf MET N cut", 30, -3.14, 3.14);
    h_METPhi_Nm1[j]->Sumw2();
    h_METPhi_cut[j]->Sumw2();
  
    h_dphoMETPhi_Nm1[j] = new TH1F(Form("h_dphoMETPhi_Nm1_%i", j), "deltaPhi of pho and MET N-1 cut", 30, -3.14, 3.14);
    h_dphoMETPhi_cut[j] = new TH1F(Form("h_dphoMETPhi_cut_%i", j), "deltaPhi of pho and MET N cut", 30, -3.14, 3.14);
    h_dphoMETPhi_Nm1[j]->Sumw2();
    h_dphoMETPhi_cut[j]->Sumw2();

    
    for(Int_t ii=0; ii<4; ii++){
      h_dphoMETPhi_test[j][ii] = new TH1F(Form("h_dphoMETPhi_test_cut%i_%i", ii, j), "dphoMETPhi cut at diff val", 22, ptbin);
      h_dphoMETPhi_test[j][ii]->Sumw2();
    }
    
    h_phoEB_ptoverMET_cut[j] = new TH1F(Form("h_phoEB_ptoverMET_cut_%i", j), "phoEB pt/MET N cut", 20, 0, 4);
    h_phoEB_ptoverMET_cut[j]->Sumw2();
   
    h_nvtx_cut[j] = new TH1F(Form("h_nvtx_cut_%i", j), "#vtx N cut", 100, 0, 100);
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
    h_jetpt_210[ii]->Sumw2();
    h_jetpt_M[ii]->Sumw2();
    h_jetpt_leptonveto[ii]->Sumw2();
    h_jetpt_MET[ii]->Sumw2();
    h_jetpt_dphoMETPhi[ii]->Sumw2();
    h_jetpt_djetMETPhi[ii]->Sumw2();
    h_jetpt_jetID[ii]->Sumw2();
    h_jetpt_jetPUID[ii]->Sumw2();
    h_jetpt_jetjetdEta[ii]->Sumw2();
    h_jetpt_dijetMass[ii]->Sumw2();
    h_jetpt_phojetdR[ii]->Sumw2();
    h_jetpt_jetjetdR[ii]->Sumw2();
    h_jetpt_jetveto[ii]->Sumw2();

    h_mindjetMETPhi_Nm1[ii] = new TH1F(Form("h_mindjetMETPhi_Nm1_%i", ii), "mindjetMETPhi N-1 cut", 30, -3.14, 3.14);
    h_mindjetMETPhi_cut[ii] = new TH1F(Form("h_mindjetMETPhi_cut_%i", ii), "mindjetMETPhi cut all", 30, -3.14, 3.14);
    h_mindjetMETPhi_Nm1[ii]->Sumw2();
    h_mindjetMETPhi_cut[ii]->Sumw2();
    
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

  TH1F *h_dr_jetjet;
  TH1F *h_dEta_jetjet;
  TH1F *h_dPhi_jetjet;
  TH1F *h_dijetMass;
  
  h_dr_jetjet = new TH1F("h_dr_jetjet", "jet jet dR", 20, 0., 8);
  h_dEta_jetjet = new TH1F("h_dEta_jetjet", "jet jet dEta", 20, 0., 8);
  h_dPhi_jetjet = new TH1F("h_dPhi_jetjet", "jet jet dPhi", 30, -3.14, 3.14);
  h_dijetMass = new TH1F("h_dijetMass", "dijet mass", 80, 0., 2000);
  h_dr_jetjet->Sumw2();
  h_dEta_jetjet->Sumw2();
  h_dPhi_jetjet->Sumw2();
  h_dijetMass->Sumw2();

  TH2F* h2_nvtx_METX = new TH2F("h2_nvtx_METX", "h2_nvtx_METX", 100, 0, 100, 100, -500, 500);
  TH2F* h2_nvtx_METY = new TH2F("h2_nvtx_METY", "h2_nvtx_METY", 100, 0, 100, 100, -500, 500);

  TH2F* h2_MET_djetMETPhi[2][4];//MET[80, 100, 140, 170]
  for(Int_t j=0; j<2; j++){
    for(Int_t ii=0; ii<4; ii++){
      h2_MET_djetMETPhi[j][ii] = new TH2F(Form("h2_MET_djetMETPhi_MET%i_%i", ii, j), "MET and djetMETPhi relation at diff MET cut", 60, 0, 1200, 30, -3.14, 3.14);
      h2_MET_djetMETPhi[j][ii]->Sumw2();
    }
  }

  //input variable
  Bool_t	isData		= 0;
  Int_t		nGoodVtx	= 0;
  Bool_t	isPVGood        = 0;
  Float_t*	EventWeight	= 0;
  Float_t	rho		= 0;
  Int_t*        phoIsMatch      = 0;
  Int_t         nPho            = 0;
  Float_t*	phoE		= 0; 
  Float_t*	phoEt		= 0;
  Float_t*	phoEta	        = 0;
  Float_t*	phoSCEta	= 0;
  Float_t*	phoPhi	       	= 0;
  Float_t*      phoPFChIso      = 0;
  Float_t*      phoPFChWorstIso = 0;
  Float_t*      phoPFChIso_rc      = 0;
  Float_t*      phoPFChWorstIso_newEA = 0;
  Float_t*	phoSeedTime	= 0;
  Float_t*	phoMIP       	= 0;
  Float_t       pfMET		= 0;
  Float_t       pfMETPhi	= 0;
  Int_t		nJet		= 0;
  Int_t		npfjet		= 0;
  Int_t         nSMjet          = 0;
  Int_t         nVBSjet         = 0;
  
  Float_t*	jetPt		= 0;
  Float_t*	jetEta		= 0;
  Float_t*	jetPhi		= 0;
  Float_t*	jetEn		= 0;
  Int_t*	nonPUjetid	= 0;
  Double_t	dijetMass	= 0;
  Int_t*        jetTID          = 0;
  Int_t*        jetPUIDMVA      = 0;
  Int_t		nLep		= 0;
  Int_t*	cutflowSMID	= 0; 
  Int_t*	cutflowVBSID	= 0;
  Int_t*        cutflowSMfixMETID = 0;
  Int_t*        cutflowVBSfixMETID = 0;
  Float_t*	phoMETdPhi	= 0;
  Float_t       minJMETdPhi = 0;
  Float_t*	JMETdPhi	= 0;
  Float_t		jetjetdEta   = 0;
  Float_t		jetjetdPhi   = 0;
  Float_t		jetjetdR     = 0;
  Float_t*	phojetdEta     = 0;
  Float_t*	phojetdPhi      = 0;
  Float_t*	phojetdR     = 0;

  //output variable
  Int_t			nGoodVtx_     = 0;
  Float_t		EventWeight_  = 0;
  Float_t		rho_	     = 0;
  Float_t	phoEt_210	     = 0;
  Float_t	phoEt_M	     = 0;
  Float_t	phoEt_chworst	     = 0;
  Float_t	phoEt_SeedTime	     = 0;
  Float_t	phoEt_leptonveto	     = 0;
  Float_t       phoEt_MET             = 0;
  Float_t       phoEt_fixMET          = 0;
  Float_t	phoEt_dphoMETPhi	     = 0;
  Float_t	phoEt_djetMETPhi	     = 0;
  Float_t	phoEt_jetveto	     = 0;
  Float_t	phoEt_HLT	     = 0;
  Float_t	phoEta_	     = 0;
  Float_t	phoSCEta_     = 0;
  Float_t	phoPhi_	     = 0;
  Float_t	phoSeedTime_  = 0;
  Float_t	phoMIP_       = 0;
  Float_t		pfMET_	     = 0;
  Float_t		pfMETPhi_     = 0;
  Float_t               phoMETdPhi_   = 0;
  Int_t			nJet_	     = 0;
  Int_t			npfjet_	     = 0;
  vector<Float_t>	jetPt_;
  vector<Float_t>	jetEta_;
  vector<Float_t>	jetPhi_	;
  vector<Float_t>	jetEn_;
  vector<Int_t>		nonPUjetid_;
  Double_t		dijetMass_    = 0;
  Int_t			nLep_	     = 0;
  vector<Int_t>		cutflowSMID_;
  vector<Int_t>		cutflowVBSID_;
  vector<Float_t>	JMETdPhi_     ;
  Float_t		jetjetdEta_   = 0;
  Float_t		jetjetdPhi_   = 0;
  Float_t		jetjetdR_     = 0;
  vector<Float_t>	phojetdEta_    ;
  vector<Float_t>	phojetdPhi_     ;
  vector<Float_t>	phojetdR_     ;

  TTree *outtree_;
  outtree_ = new TTree("t", "raw tree");

  outtree_->Branch("nGoodVtx"		,&nGoodVtx_, "nGoodVtx/I"	);
  outtree_->Branch("EventWeight"	,&EventWeight_, "EventWeight/F"	);
  outtree_->Branch("rho"		,&rho_, "rho/F"			);
  outtree_->Branch("phoEt_210"		,&phoEt_210, "phoEt_210/F"			);
  outtree_->Branch("phoEt_M"		,&phoEt_M, "phoEt_M/F"			);
  outtree_->Branch("phoEt_chworst"	,&phoEt_chworst, "phoEt_chworst/F"			);
  outtree_->Branch("phoEt_SeedTime"	,&phoEt_SeedTime, "phoEt_SeedTime/F"		);
  outtree_->Branch("phoEt_leptonveto"	,&phoEt_leptonveto, "phoEt_leptonveto/F"		);
  outtree_->Branch("phoEt_MET"		,&phoEt_MET, "phoEt_MET/F"			);
  outtree_->Branch("phoEt_fixMET"		,&phoEt_fixMET, "phoEt_fixMET/F"			);
  outtree_->Branch("phoEt_dphoMETPhi"	,&phoEt_dphoMETPhi, "phoEt_dphoMETPhi/F"		);
  outtree_->Branch("phoEt_djetMETPhi"	,&phoEt_djetMETPhi, "phoEt_djetMETPhi/F"		);
  outtree_->Branch("phoEt_jetveto"	,&phoEt_jetveto, "phoEt_jetveto/F"			);
  outtree_->Branch("phoEt_HLT"		,&phoEt_HLT, "phoEt_HLT/F"			);
  outtree_->Branch("phoEta"		,&phoEta_, "phoEta/F"			);
  outtree_->Branch("phoSCEta"		,&phoSCEta_, "phoSCEta/F"			);
  outtree_->Branch("phoPhi"		,&phoPhi_, "phoPhi/F"			);
  outtree_->Branch("phoSeedTime"	,&phoSeedTime_, "phoSeedTime/F"			);
  outtree_->Branch("phoMIP"		,&phoMIP_, "phoMIP/F"			);
  outtree_->Branch("pfMET"		,&pfMET_, "pfMET/F"		);
  outtree_->Branch("pfMETPhi"		,&pfMETPhi_, "pfMETPhi/F"	);
  outtree_->Branch("phoMETdPhi"		,&phoMETdPhi_, "phoMETdPhi/F"	);
  outtree_->Branch("npfjet"		,&npfjet, "npfjet/I"		);
  outtree_->Branch("jetPt"		,&jetPt_			);
  outtree_->Branch("jetEta"		,&jetEta_			);
  outtree_->Branch("jetPhi"		,&jetPhi_			);
  outtree_->Branch("jetEn"		,&jetEn_			);
  outtree_->Branch("dijetMass"		,&dijetMass_, "dijetMass/D"	);
  outtree_->Branch("nLep"		,&nLep_, "nLep/I"		);
  outtree_->Branch("JMETdPhi"		,&JMETdPhi_			);
  outtree_->Branch("jetjetdEta"		,&jetjetdEta_, "jetjetdEta/F"	);
  outtree_->Branch("jetjetdPhi"		,&jetjetdPhi_, "jetjetdPhi/F"	);
  outtree_->Branch("jetjetdR"		,&jetjetdR_, "jetjetdR/F"	);
  outtree_->Branch("phojetdEta"		,&phojetdEta_			);
  outtree_->Branch("phojetdPhi"		,&phojetdPhi_			);
  outtree_->Branch("phojetdR"		,&phojetdR_			);
    
  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);
    //PVGood	= data.GetBool(		"isPVGood"		);
    //(isPVGood == 0) continue;
    
    //init
    nGoodVtx_		= 0;
    EventWeight_	= 0;
    rho_		= 0;
    phoEt_210		= 0;
    phoEt_M		= 0;
    phoEt_chworst	= 0;
    phoEt_SeedTime	= 0;
    phoEt_leptonveto	= 0;
    phoEt_MET		= 0;
    phoEt_dphoMETPhi	= 0;
    phoEt_djetMETPhi	= 0;
    phoEt_jetveto	= 0;
    phoEt_HLT		= 0;
    phoEta_		= 0;
    phoSCEta_		= 0;
    phoPhi_		= 0;
    phoSeedTime_	= 0;
    phoMIP_		= 0;
    pfMET_		= 0;
    pfMETPhi_		= 0;
    phoMETdPhi_		= 0;
    nJet_		= 0;
    npfjet_		= 0;
    dijetMass_  = 0;
    nLep_	    = 0;
    jetjetdEta_ = 0;
    jetjetdPhi_ = 0;
    jetjetdR_   = 0;
    nonPUjetid_.clear();
    jetPt_.clear();
    jetEn_.clear();
    jetEta_.clear();
    jetPhi_.clear();
    cutflowSMID_.clear();
    cutflowVBSID_.clear();
    JMETdPhi_.clear();
    phojetdR_.clear();
    phojetdEta_.clear();
    phojetdPhi_.clear();
    
    
    isPVGood    = data.GetBool("isPVGood");
    //if(isPVGood == 0) continue;
    
    nGoodVtx	= data.GetInt(		"nGoodVtx"		);
    EventWeight	= data.GetPtrFloat(	"EventWeight"		);
    rho		= data.GetFloat(	"rho"			);
    phoIsMatch = data.GetPtrInt("phoIsMatch");
    //reco pho
    nPho        = data.GetInt("nPho");
    phoE	= data.GetPtrFloat(	"phoE"			);
    phoEt	= data.GetPtrFloat(	"phoEt"			);
    phoEta	= data.GetPtrFloat(	"phoEta"		);
    phoSCEta	= data.GetPtrFloat(	"phoSCEta"		);
    phoPhi	= data.GetPtrFloat(	"phoPhi"		);
    phoPFChIso  = data.GetPtrFloat("phoPFChIso");
    phoPFChWorstIso = data.GetPtrFloat("phoPFChWorstIso");
    phoPFChIso_rc  = data.GetPtrFloat("phoPFChIso_rc");
    phoPFChWorstIso_newEA = data.GetPtrFloat("phoPFChWorstIso_newEA");
    phoSeedTime	= data.GetPtrFloat(	"phoSeedTime"		);
    phoMIP	= data.GetPtrFloat(	"phoMIPTotEnergy"	);
    pfMET	= data.GetFloat(	"corrMET"			);
    pfMETPhi	= data.GetFloat(	"corrMETPhi"		);

    //jet veto selection
    nJet = data.GetInt(			"nJet"			);
    npfjet = data.GetInt(		"npfjet"		);
    nSMjet = data.GetInt(               "nSMjet"                );
    nVBSjet = data.GetInt(               "nVBSjet"              );
    jetPt      = data.GetPtrFloat(	"jetPtSmear"		);
    jetEta     = data.GetPtrFloat(	"jetEta"		);
    jetPhi     = data.GetPtrFloat(	"jetPhi"		);
    jetEn      = data.GetPtrFloat(	"jetEn"			);
    nonPUjetid = data.GetPtrInt(	"nonPUjetid"		);
    dijetMass = data.GetDouble(		"dijetMass"		);
    jetTID = data.GetPtrInt("jetTID");
    jetPUIDMVA = data.GetPtrInt("jetPUIDMVA");
    
    nLep = data.GetInt(			"nLep"			);

    phoMETdPhi = data.GetPtrFloat(	"phoMETdPhi"		);
    minJMETdPhi = data.GetFloat(        "minJMETdPhi"           );
    JMETdPhi = data.GetPtrFloat(	"JMETdPhi"		);
    jetjetdR = data.GetFloat(		"jetjetdR"		);
    jetjetdEta = data.GetFloat(		"jetjetdEta"		);
    jetjetdPhi = data.GetFloat(		"jetjetdPhi"		);
    phojetdR = data.GetPtrFloat(	"phojetdR"		);
    phojetdEta = data.GetPtrFloat(	"phojetdEta"		);
    phojetdPhi = data.GetPtrFloat(	"phojetdPhi"		);
    //cout << "get data" << endl;
    
   
    //MET XY corr
    Float_t METX = pfMET*cos(pfMETPhi);
    h2_nvtx_METX->Fill(nGoodVtx, METX);
    Float_t METY = pfMET*sin(pfMETPhi);
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
      WEIGHT = EventWeight[ipho];

      /*
      if(pho_sel(cutflow[0], 3) == 1){
	for(Int_t j=0; j<7; j++){
	  if(fabs(phoSCEta[ipho]) >= EAbin[j] && fabs(phoSCEta[ipho]) < EAbin[j+1]){
	    h2_chIso_rho[j]->Fill(rho, phoPFChIso[ipho]);
	    h2_chworst_rho[j]->Fill(rho, phoPFChWorstIso[ipho]);
	  }
	}
      }
      */
      float METcut[4] = {80.0, 100.0, 140.0, 170.0};
      float dphoMETPhicut[4] = {0.5, 0.8, 1.0, 1.2};
      
      for(Int_t icat=0; icat<2; icat++){
	fillpho[icat] = 0;
	if(pho_sel(cutflow[icat], 11) == 1) fillpho[icat] += 1;
	if(fillpho[icat]>1) continue;
	if(icat == 0) npfjet = nSMjet;
	else if(icat == 1) npfjet = nVBSjet;
	
	if(pho_sel(cutflow[icat], 3)	== 1) h_phoEB_pt_210[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat], 4)	== 1) h_phoEB_pt_M[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat], 7)	== 1) {
	  h_phoEB_pt_leptonveto[icat]->Fill(phoEt[ipho], WEIGHT);
	  for(Int_t jj=0; jj<4; jj++){
	    if(pfMET > METcut[jj]) h_MET_test[icat][jj]->Fill(phoEt[ipho], WEIGHT);
	  }
	}
	if(pho_sel(cutflow[icat], 8)	== 1) {
	  h_phoEB_pt_MET[icat]->Fill(phoEt[ipho], WEIGHT);
	  for(Int_t jj=0; jj<4; jj++){
	    if(phoMETdPhi[ipho] > dphoMETPhicut[jj]) h_dphoMETPhi_test[icat][jj]->Fill(phoEt[ipho], WEIGHT);
	  }
	}
	if(pho_sel(cutflow[icat], 9)	== 1) h_phoEB_pt_dphoMETPhi[icat]->Fill(phoEt[ipho], WEIGHT);
	  
	if(pho_sel(cutflow[icat], 10)	== 1) h_phoEB_pt_djetMETPhi[icat]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[icat], 11)	== 1) h_phoEB_pt_jetveto[icat]->Fill(phoEt[ipho], WEIGHT);
		
	for(Int_t jj=0; jj<npfjet; jj++){
	  if(icat == 1 && jj >=2) continue;
	  Int_t ijet = nonPUjetid[jj];
	  if(pho_sel(cutflow[icat], 3)	== 1) h_jetpt_210[icat]->Fill(jetPt[ijet], WEIGHT);
	  if(pho_sel(cutflow[icat], 4)	== 1) h_jetpt_M[icat]->Fill(jetPt[ijet], WEIGHT);
	  if(pho_sel(cutflow[icat], 7)	== 1) h_jetpt_leptonveto[icat]->Fill(jetPt[ijet], WEIGHT);
	  if(pho_sel(cutflow[icat], 8)	== 1) h_jetpt_MET[icat]->Fill(jetPt[ijet], WEIGHT);
	  if(pho_sel(cutflow[icat], 9)	== 1) h_jetpt_dphoMETPhi[icat]->Fill(jetPt[ijet], WEIGHT);
	  if(pho_sel(cutflow[icat], 10) == 1) h_jetpt_djetMETPhi[icat]->Fill(jetPt[ijet], WEIGHT);
	  
	  if(pho_sel(cutflow[icat], 10) == 1 && icat == 1){
	    if(jetTID[ijet] == 1) h_jetpt_jetID[icat]->Fill(jetPt[ijet], WEIGHT);
	    if((jetTID[ijet] == 1) && (jetPUIDMVA[ijet] == 1)) h_jetpt_jetPUID[icat]->Fill(jetPt[ijet], WEIGHT);
	    if((jetTID[ijet] == 1) && (jetPUIDMVA[ijet] == 1) && (jetjetdEta > 2.5)) h_jetpt_jetjetdEta[icat]->Fill(jetPt[ijet], WEIGHT);
	    if((jetTID[ijet] == 1) && (jetPUIDMVA[ijet] == 1) && (jetjetdEta > 2.5) && (dijetMass>250)) h_jetpt_dijetMass[ijet]->Fill(jetPt[ijet], WEIGHT);
	    if((jetTID[ijet] == 1) && (jetPUIDMVA[ijet] == 1) && (jetjetdEta > 2.5) && (dijetMass>250) && (phojetdR[0] > 0.4) && (phojetdR[1] > 0.4)) h_jetpt_phojetdR[icat]->Fill(jetPt[ijet], WEIGHT);
	    if((jetTID[ijet] == 1) && (jetPUIDMVA[ijet] == 1) && (jetjetdEta > 2.5) && (dijetMass>250) && (phojetdR[0] > 0.4) && (phojetdR[1] > 0.4) && (jetjetdR > 0.5)) h_jetpt_jetjetdR[icat]->Fill(jetPt[ijet], WEIGHT);
	  }
	  
	  if(pho_sel(cutflow[icat], 11) == 1) h_jetpt_jetveto[icat]->Fill(jetPt[ijet], WEIGHT);
	}

	//result
	if(pho_sel(cutflow[icat], 11) == 1){
	  h_phoEB_ptcut[icat]->Fill(phoEt[ipho], WEIGHT);
	  h_phoEB_Etacut[icat]->Fill(phoEta[ipho], WEIGHT);
	  h_phoEB_Phicut[icat]->Fill(phoPhi[ipho], WEIGHT);
	  h_phoEB_ptoverMET_cut[icat]->Fill(phoEt[ipho]/pfMET, WEIGHT);
	  h_MET_cut[icat]->Fill(pfMET, WEIGHT);
	  h_METPhi_cut[icat]->Fill(pfMETPhi, WEIGHT);
	  h_MIP_cut[icat]->Fill(phoMIP[ipho], WEIGHT);
	  h_dphoMETPhi_cut[icat]->Fill(phoMETdPhi[ipho], WEIGHT);
	  h_nvtx_cut[icat]->Fill(nGoodVtx, WEIGHT);
	  h_njet_cut[icat]->Fill(npfjet, WEIGHT);
	  h_nlep_cut[icat]->Fill(nLep, WEIGHT);
	  h_phoIsMatch[icat]->Fill(phoIsMatch[ipho], WEIGHT);
	  h_mindjetMETPhi_cut[icat]->Fill(minJMETdPhi, WEIGHT);
	}
	for(Int_t jj=0; jj<npfjet; jj++){
	  if(icat == 1 && jj >=2) continue;
	  Int_t ijet = nonPUjetid[jj];
	  if(pho_sel(cutflow[icat], 11) == 1){
	    h_jetpt_cut[icat][jj]->Fill(jetPt[ijet], WEIGHT);
	    h_jetEta_cut[icat][jj]->Fill(jetEta[ijet], WEIGHT);
	    h_jetPhi_cut[icat][jj]->Fill(jetPhi[ijet], WEIGHT);
	    h_djetMETPhi_cut[icat][jj]->Fill(JMETdPhi[jj], WEIGHT);
	    h_dr_phojet[icat][jj]->Fill(phojetdR[jj], WEIGHT);
	    h_dEta_phojet[icat][jj]->Fill(fabs(phojetdEta[jj]), WEIGHT);
	    h_dPhi_phojet[icat][jj]->Fill(phojetdPhi[jj], WEIGHT);
	  }
	}

	if(icat == 1){
	  if(pho_sel(cutflow[icat], 11) == 1){
	    h_dr_jetjet->Fill(jetjetdR, WEIGHT);
	    h_dEta_jetjet->Fill(fabs(jetjetdEta), WEIGHT);
	    h_dPhi_jetjet->Fill(jetjetdPhi, WEIGHT);
	    h_dijetMass->Fill(dijetMass, WEIGHT);
	  }
	}
	
	//Nm1
	if(Nm1_sel(cutflow[icat], 7) == 1){
	  h_mindjetMETPhi_Nm1[icat]->Fill(minJMETdPhi, WEIGHT);
	}
	
	if(Nm1_sel(cutflow[icat], 8) == 1) {
	  h_MET_Nm1[icat]->Fill(pfMET, WEIGHT);
	  h_METPhi_Nm1[icat]->Fill(pfMETPhi, WEIGHT);
	  for(Int_t jj=0; jj<4; jj++){
	    if(pfMET > METcut[jj]) {
	      h2_MET_djetMETPhi[icat][jj]->Fill(pfMET, minJMETdPhi, WEIGHT);
	    }
	  }
	}
	
	if(Nm1_sel(cutflow[icat], 9) == 1){
	  h_dphoMETPhi_Nm1[icat]->Fill(phoMETdPhi[ipho], WEIGHT);
	  
	}	
	for(Int_t jj=0; jj<npfjet; jj++){
	  Int_t ijet = nonPUjetid[jj];
	  if(Nm1_sel(cutflow[icat], 10) == 1 && jj<2) h_djetMETPhi_Nm1[icat][jj]->Fill(JMETdPhi[jj], WEIGHT);
	}
	if(Nm1_sel(cutflow[icat], 11) == 1) h_njet_Nm1[icat]->Fill(npfjet, WEIGHT);

	//Nm2
	float minJMETdPhicut[4] = {0.2, 0.3, 0.4, 0.5};
	if(pho_sel(cutflow[icat], 7) == 1 && (cutflow[icat]>>9&1) == 1 && (cutflow[icat]>>11&1) == 1 && (cutflow[icat]>>10&1) == 0){
	  for(Int_t jj=0; jj<4; jj++){
	    if(minJMETdPhi < minJMETdPhicut[jj]) h_MET_Nm1_djetMETPhi_SB[icat][jj]->Fill(pfMET, WEIGHT);
	    if(minJMETdPhi>0.1 && minJMETdPhi > minJMETdPhicut[jj]) h_MET_Nm1_djetMETPhi_SB[icat][jj+4]->Fill(pfMET, WEIGHT);
	  }
	}
	//if(pho_sel(cutflow[icat], 7) == 1 && (cutflow[icat]>>9&1) == 1 && (cutflow[icat]>>11&1) == 1 && (cutflow[icat]>>10&1) == 0) 
		
      }     
    }    //cout << "get cutflow plot"  << endl;
    
    //fill tree VBS raw #events
    nGoodVtx_= nGoodVtx;
    Int_t fill = 0;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      Int_t cutflow;
      cutflow = cutflowVBSID[ipho];
      
      if(pho_sel(cutflow, 11) == 1) fill += 1;
      if(fill>1) continue;
      
      if(pho_sel(cutflow, 3)	== 1)  phoEt_210 = phoEt[ipho];
      if(pho_sel(cutflow, 4)	== 1)  phoEt_M = phoEt[ipho];
      if(pho_sel(cutflow, 5)	== 1)  phoEt_chworst = phoEt[ipho];
      if(pho_sel(cutflow, 6)	== 1)  phoEt_SeedTime = phoEt[ipho];
      if(pho_sel(cutflow, 7)	== 1)  phoEt_leptonveto = phoEt[ipho];
      if(pho_sel(cutflow, 8)	== 1)  phoEt_MET = phoEt[ipho];
      if(pho_sel(cutflow, 9)	== 1)  phoEt_dphoMETPhi = phoEt[ipho];
      if(pho_sel(cutflow, 10)	== 1)  phoEt_djetMETPhi = phoEt[ipho];
      if(pho_sel(cutflow, 11)	== 1)  phoEt_jetveto = phoEt[ipho];
      
      if(pho_sel(cutflow, 11)	== 1){
	EventWeight_ = EventWeight[ipho];
	phoEta_ = phoEta[ipho];
	phoSCEta_ = phoSCEta[ipho];
	phoPhi_ = phoPhi[ipho];
	phoSeedTime_ = phoSeedTime[ipho];
	phoMIP_ = phoMIP[ipho];
	pfMET_	     = pfMET;
	pfMETPhi_     = pfMETPhi;
	phoMETdPhi_   = phoMETdPhi[ipho];
	npfjet_	     = npfjet;
	jetjetdEta_   = fabs(jetjetdEta);
	jetjetdPhi_   = jetjetdPhi;
	jetjetdR_     = jetjetdR;
	for(Int_t jj=0; jj<npfjet; jj++){
	  if(jj >=2) continue;
	  Int_t ijet = nonPUjetid[jj];
	  jetPt_.push_back(jetPt[ijet]);
	  jetEta_.push_back(jetEta[ijet]);
	  jetPhi_.push_back(jetPhi[ijet]);
	  jetEn_.push_back(jetEn[ijet]);
	  JMETdPhi_.push_back(JMETdPhi[jj]);
	  phojetdEta_.push_back(fabs(phojetdEta[jj]));
	  phojetdPhi_.push_back(phojetdPhi[jj] );
	  phojetdR_.push_back( phojetdR[jj]) ;
	}
      }
    }
    //cout << "get VBS fill" << endl;
    outtree_->Fill();
    //cout << "get tree fill" << endl;
  }
  //****************END LOOP**********************//

  //****************Terminate*********************//
  fout_->cd();
  outtree_->Write();

  hEvents->Write();
  hSumofGenW->Write();
  
  h_dr_jetjet->Write();
  h_dEta_jetjet->Write();
  h_dPhi_jetjet->Write();
  h_dijetMass->Write();

  h2_nvtx_METX->Write();
  h2_nvtx_METY->Write();
  
  fout_->mkdir("SMandVBS");
  fout_->cd("SMandVBS");
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
  
  fout_->mkdir("h_jetpt");
  fout_->cd("h_jetpt");
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

  fout_->mkdir("h_MET_Nm1");
  fout_->cd("h_MET_Nm1");
  for(Int_t i=0; i<2; i++){
    for(Int_t jj=0; jj<8; jj++){
      h_MET_Nm1_djetMETPhi_SB[i][jj]->Write();
      
    }
  }
    

  /*
  fout_->mkdir("h2_chIso_rho");
  fout_->cd("h2_chIso_rho");
  for(Int_t i=0; i<7; i++){
    h2_chIso_rho[i]->Write();
  }
  
  fout_->mkdir("h2_chworst_rho");
  fout_->cd("h2_chworst_rho");
  for(Int_t i=0; i<7; i++){
    h2_chworst_rho[i]->Write();
  }
  */
  
  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());
}
