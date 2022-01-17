#include <TH1D.h>
#include <TLorentzVector.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TMath.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH2.h>
#include <TH2F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
using namespace std;
#include <iostream>
#include <TProfile.h>

#include "untuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "IsoCorrection.h"
#include "PhotonSelection.h"
#include "ElectronSelection.h"
#include "MuonSelection.h"
#include "puweicalc.h"

Float_t WEIGHT =0;

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
  dEta = eta1 - eta2;
  return dEta;
}

Int_t SetBit(Int_t nbit, Int_t bit){
  return (bit | (1<<nbit));
}

Int_t ResetBit(Int_t nbit, Int_t bit){
  return (bit & ~(1<<nbit));
}

void xZgbkg(char* pathes, char* PUpathes, char* IDpathes, char* CSEVpathes){

  //***********************Initialization***********************//
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("ggNtuplizer/hEvents")->Clone();
  hEvents->SetDirectory(0);

  //Get phoID SF
  fopen = new TFile(IDpathes, "READ");
  TH2F *h_phoIDSF = (TH2F*)fopen->Get("EGamma_SF2D")->Clone();
  h_phoIDSF->SetDirectory(0);
  fopen->Close();

  //Get EleVeto SF
  fopen = new TFile(CSEVpathes, "READ");
  TH2F *h_CSVSF = (TH2F*)fopen->Get("Scaling_Factors_CSEV_R9_Inclusive")->Clone();
  h_CSVSF->SetDirectory(0);
  fopen->Close();

  //access EventTree with TreeReader class
  TreeReader data(pathes);

  //create an output .root file
  TFile *fout_;
  fout_ = new TFile("output_ggtree_mc.root","RECREATE");

  //create histograms in output .root file
  TH1F *hGenWeight = new TH1F("hGenWeight", "mc gen Weight", 2000, -1000, 1000);
  TH1F *hSumofGenW = new TH1F("hSumofGenW", "Sum of Gen Weight", 1, 0, 1);
  TH1F *h_npho = new TH1F("h_npho", "n pho", 10, 0., 10);
  TH1F *h_nmcpho = new TH1F("h_nmcpho", "n mcpho", 10, 0., 10);
  TH1F *h_nmatchpho = new TH1F("h_nmatchpho", "n matchpho", 10, 0., 10);
  TH1F *h_dr_pho = new TH1F("h_dr_pho", "dr of photon", 100, 0., 0.2);
  TH1F *h_dpt_pho = new TH1F("h_dpt_pho", "dpt of photon", 100, 0., 1);
  TH2F *h_dptdr_pho = new TH2F("h_dptdr_pho", "dptdr of photon", 100, 0., 1, 100, 0., 2);
  hSumofGenW->Sumw2();
  h_dr_pho->Sumw2();
  h_dpt_pho->Sumw2();
  h_dptdr_pho->Sumw2();

  TH1F *h_mcStatus = new TH1F("h_mcStatus", "check isPrompt pho is also fromHardProcess", 5, 0, 5);
  TH2F *h2_MET_phoEt = new TH2F("h2_MET_phoEt", "h2_MET_phoEt", 50, 0, 1000, 80, 200, 1000);
  h2_MET_phoEt->Sumw2();
  TH2F *h2_phoEt_ptoverMET = new TH2F("h2_phoEt_ptoverMET", "h2_phoEt_ptoverMET", 80, 200, 1000, 20, 0, 4);
  h2_phoEt_ptoverMET->Sumw2();
  
  Float_t EAbin[10] = {0.0, 1.0, 1.479, 2.0, 2.2, 2.3, 2.4, 3};//8 bins
  TH2F *h_chIso_rho[7];
  TH2F *h_chworst_rho[7];
  for(Int_t i=0; i<7; i++){
    h_chIso_rho[i] = new TH2F(Form("h_chIso_rho_eta%i", i), Form("h_chIso_rho_eta%i", i), 60, 0, 30, 30, 0, 3);
    h_chworst_rho[i] = new TH2F(Form("h_chworst_rho_eta%i",i), Form("h_chworst_rho_eta%i", i), 60, 0, 30, 30, 0, 3);
    h_chIso_rho[i]->Sumw2();
    h_chworst_rho[i]->Sumw2();
  }

  Double_t ptbin[30] = {22, 30, 36, 50, 75, 90, 120, 170, 175, 180, 185, 190, 200,
		      220, 250, 300, 350, 400, 500, 750, 1000, 1500, 2000, 3000, 10000};//22 bins, 2016
  Double_t etabin[10] = {-1.566, -1.4442, -0.8, 0, 0.8, 1.4442, 1.566};//6bins

  Float_t chbin[30] = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0, 10, 12, 15};//24 bins
  TH1F *h_phoEB_pt_chIsocut[22];
  TH1F *h_phoEB_pt_chworstcut[22];
  TH1F *h_phoEB_pt_chworstcut_newEA[22];
  for(Int_t j=0; j<22; j++){
    h_phoEB_pt_chIsocut[j] = new TH1F(Form("h_phoEB_pt_chIsocut_%i", j), Form("h_phoEB_pt_chIsocut_%i", j), 22, ptbin);
    h_phoEB_pt_chworstcut[j] = new TH1F(Form("h_phoEB_pt_chworstcut_%i", j), Form("h_phoEB_pt_chworstcut_%i", j), 22, ptbin);
    h_phoEB_pt_chworstcut_newEA[j] = new TH1F(Form("h_phoEB_pt_chworstcut_newEA_%i", j), Form("h_phoEB_pt_chworstcut_newEA_%i", j), 22, ptbin);

    h_phoEB_pt_chIsocut[j]->Sumw2();
    h_phoEB_pt_chworstcut[j]->Sumw2();
    h_phoEB_pt_chworstcut_newEA[j]->Sumw2();
  }

  Float_t ptoverMETbin[20] = {1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4};
  TH1F *h_phoEB_pt_phoMET_vcut[12];
  for(Int_t j=0; j<12; j++){
    h_phoEB_pt_phoMET_vcut[j] = new TH1F(Form("h_phoEB_pt_phoMET_vcut_%i", j), Form("h_phoEB_pt_phoMET_vcut_%i", j), 22, ptbin);
    h_phoEB_pt_phoMET_vcut[j]->Sumw2();
  }

  Float_t METbin[10] = {90, 100, 110, 120, 130, 140};
  TH1F *h_phoEB_pt_MET_vcut[6];
  for(Int_t j=0; j<6; j++){
    h_phoEB_pt_MET_vcut[j] = new TH1F(Form("h_phoEB_pt_MET_vcut_%i", j), Form("h_phoEB_pt_MET_vcut_%i", j), 22, ptbin);
    h_phoEB_pt_MET_vcut[j]->Sumw2();
  }
  

  TH1F *h_njet = new TH1F("h_njet", "njet", 10, 0, 10);
  //[0, 1][SM, VBS]
  TH1F *h_phoEB_pt[2];
  TH1F *h_phoEB_pt_200[2];
  TH1F *h_phoEB_pt_M[2];
  TH1F *h_phoEB_pt_chworst[2];
  TH1F *h_phoEB_pt_SeedTime[2];
  TH1F *h_phoEB_pt_phoptoverMET[2];
  TH1F *h_phoEB_pt_MET[2];
  TH1F *h_phoEB_pt_dphoMETPhi[2];
  TH1F *h_phoEB_pt_djetMETPhi[2];
  TH1F *h_phoEB_pt_jetveto[2];
  TH1F *h_phoEB_pt_HLT[2];

  TH1F *h_phoEB_ptcut[2];
  TH1F *h_phoEB_etacut[2];
    
  TH1F *h_phoEB_ptoverMET_Nm1[2];
  TH1F *h_phoEB_ptoverMET_cut[2];

  TH1F *h_SeedTime_Nm1[2];
  TH1F *h_SeedTime_cut[2];
    
  TH1F *h_MET_Nm1[2];
  TH1F *h_MET_cut[2];
    
  TH1F *h_dphoMETPhi_Nm1[2];
  TH1F *h_dphoMETPhi_cut[2];
    
  TH1F *h_njet_Nm1[2];
  TH1F *h_njet_cut[2];

  TH1F *h_jetpt_Nm1[2];
  TH1F *h_jetpt_cut[2];

  TH1F *h_djetMETPhi_Nm1[2];
  TH1F *h_djetMETPhi_cut[2];
    
  TH1F *h_phoEB_ptoverjetpt_cut[2];
  TH1F *h_nvtx_cut[2];
  
  for(Int_t j=0; j<2; j++){
    h_phoEB_pt[j] = new TH1F(Form("h_phoEB_pt_%i", j), "matched phoEB pt", 22, ptbin);
    h_phoEB_pt_200[j] = new TH1F(Form("h_phoEB_pt_200_%i", j), "matched phoEB pt pt200 cut", 22, ptbin);
    h_phoEB_pt_M[j] = new TH1F(Form("h_phoEB_pt_M_%i", j), "matched phoEB pt M IDcut", 22, ptbin);
    h_phoEB_pt_chworst[j] = new TH1F(Form("h_phoEB_pt_chworst_%i", j), "matched phoEB pt chworst cut", 22, ptbin);
    h_phoEB_pt_SeedTime[j] = new TH1F(Form("h_phoEB_pt_SeedTime_%i", j), "matched phoEB pt SeedTime cut", 22, ptbin);
    h_phoEB_pt_phoptoverMET[j] = new TH1F(Form("h_phoEB_pt_phoptoverMET_%i", j), "matched phoEB pt phoptoverMET cut", 22, ptbin);
    h_phoEB_pt_MET[j] = new TH1F(Form("h_phoEB_pt_MET_%i", j), "matched phoEB pt MET cut", 22, ptbin);
    h_phoEB_pt_dphoMETPhi[j] = new TH1F(Form("h_phoEB_pt_dphoMETPhi_%i", j), "matched phoEB pt dphoMETPhi cut", 22, ptbin);
    h_phoEB_pt_jetveto[j] = new TH1F(Form(Form("h_phoEB_pt_jetveto_%i", j), j), Form("h_phoEB_pt_jetveto_%i", j), 22, ptbin);  
    h_phoEB_pt_djetMETPhi[j] = new TH1F(Form("h_phoEB_pt_djetMETPhi_%i", j), "matched phoEB pt djetMETPhi cut", 22, ptbin);
    h_phoEB_pt_HLT[j] = new TH1F(Form("h_phoEB_pt_HLT_%i", j), "matched phoEB pt HLT trgs", 22, ptbin);

    h_phoEB_pt[j]->Sumw2();
    h_phoEB_pt_200[j]->Sumw2();
    h_phoEB_pt_M[j]->Sumw2();
    h_phoEB_pt_chworst[j]->Sumw2();
    h_phoEB_pt_SeedTime[j]->Sumw2();
    h_phoEB_pt_phoptoverMET[j]->Sumw2();
    h_phoEB_pt_MET[j]->Sumw2();
    h_phoEB_pt_dphoMETPhi[j]->Sumw2();
    h_phoEB_pt_HLT[j]->Sumw2();
    
    h_phoEB_ptcut[j] = new TH1F(Form("h_phoEB_ptcut_%i", j), "phoEB pt cut all pas varbin", 20, 200, 1000);
    h_phoEB_etacut[j] = new TH1F(Form("h_phoEB_eta_%i", j), "matched phoEB eta varbins", 6, etabin);
    h_phoEB_ptcut[j]->Sumw2();
    h_phoEB_etacut[j]->Sumw2();
    

    h_phoEB_ptoverMET_Nm1[j] = new TH1F(Form("h_phoEB_ptoverMET_Nm1_%i", j), "phoEB pt/MET N-1 cut", 20, 0, 4);
    h_phoEB_ptoverMET_cut[j] = new TH1F(Form("h_phoEB_ptoverMET_cut_%i", j), "phoEB pt/MET N cut", 20, 0, 4);
    h_phoEB_ptoverMET_Nm1[j]->Sumw2();
    h_phoEB_ptoverMET_cut[j]->Sumw2();

    h_SeedTime_Nm1[j] = new TH1F(Form("h_SeedTime_Nm1_%i", j), "SeedTime N-1 cut", 100, -25, 25);
    h_SeedTime_cut[j] = new TH1F(Form("h_SeedTime_cut_%i", j), "SeedTime N cut", 100, -25, 25);
    h_SeedTime_Nm1[j]->Sumw2();
    h_SeedTime_cut[j]->Sumw2();
    
    h_MET_Nm1[j] = new TH1F(Form("h_MET_Nm1_%i", j), "pf MET N-1 cut", 60, 0, 1200);
    h_MET_cut[j] = new TH1F(Form("h_MET_cut_%i", j), "pf MET N cut", 60, 0, 1200);
    h_MET_Nm1[j]->Sumw2();
    h_MET_cut[j]->Sumw2();
  
    h_dphoMETPhi_Nm1[j] = new TH1F(Form("h_dphoMETPhi_Nm1_%i", j), "deltaPhi of pho and MET N-1 cut", 30, -3.14, 3.14);
    h_dphoMETPhi_cut[j] = new TH1F(Form("h_dphoMETPhi_cut_%i", j), "deltaPhi of pho and MET N cut", 30, -3.14, 3.14);
    h_dphoMETPhi_Nm1[j]->Sumw2();
    h_dphoMETPhi_cut[j]->Sumw2();

    h_njet_Nm1[j] = new TH1F(Form("h_njet_Nm1_%i", j), Form("h_njet_Nm1_%i", j), 10, 0, 10);
    h_njet_cut[j] = new TH1F(Form("h_njet_cut_%i", j), Form("h_njet_cut_%i", j), 10, 0, 10);
    h_njet_Nm1[j]->Sumw2();
    h_njet_cut[j]->Sumw2();

    h_jetpt_Nm1[j] = new TH1F(Form("h_jetpt_Nm1_%i",j), Form("h_jetpt_Nm1_%i",j), 25, 30, 1030);
    h_jetpt_cut[j] = new TH1F(Form("h_jetpt_cut_%i", j), Form("h_jetpt_cut_%i", j), 25, 30, 1030);
    h_jetpt_Nm1[j]->Sumw2();
    h_jetpt_cut[j]->Sumw2();

    h_djetMETPhi_Nm1[j] = new TH1F(Form("h_djetMETPhi_Nm1_%i", j), "deltaPhi of jet and MET N-1 cut", 30, -3.14, 3.14);
    h_djetMETPhi_cut[j] = new TH1F(Form("h_djetMETPhi_cut_%i", j), "deltaPhi of jet and MET N cut", 30, -3.14, 3.14);
    h_djetMETPhi_Nm1[j]->Sumw2();
    h_djetMETPhi_cut[j]->Sumw2();

    h_phoEB_ptoverjetpt_cut[j] = new TH1F(Form("h_phoEB_ptoverjetpt_%i", j), "phoEB pt/jet pt", 20, 0, 4);
    h_phoEB_ptoverjetpt_cut[j]->Sumw2();
    h_nvtx_cut[j] = new TH1F(Form("h_nvtx_cut_%i", j), "#vtx N cut", 60, 0, 60);
    h_nvtx_cut[j]->Sumw2();
  }

  TH2F *h2_rho_phoEB_pt_M = new TH2F("h2_rho_phoEB_pt_M", "h2_rho_phoEB_pt_M", 60, 0, 30, 22, ptbin);
  TH2F *h2_rho_phoEB_pt_chworst = new TH2F("h2_rho_phoEB_pt_chworst", "h2_rho_phoEB_pt_chworst", 60, 0, 30, 22, ptbin);
  TH2F *h2_rho_phoEB_pt_chworst_newEA = new TH2F("h2_rho_phoEB_pt_chworst_newEA", "h2_rho_phoEB_pt_chworst_newEA", 60, 0, 30, 22, ptbin);
  
  h2_rho_phoEB_pt_M->Sumw2();
  h2_rho_phoEB_pt_chworst->Sumw2();
  h2_rho_phoEB_pt_chworst_newEA->Sumw2();

  //[0,1][jet1, jet2]
  TH1F *h_jetpt[2];
  TH1F *h_jetpt_200[2];
  TH1F *h_jetpt_M[2];
  TH1F *h_jetpt_chworst[2];
  TH1F *h_jetpt_SeedTime[2];
  TH1F *h_jetpt_phoptoverMET[2];
  TH1F *h_jetpt_MET[2];
  TH1F *h_jetpt_dphoMETPhi[2];
  TH1F *h_jetpt_djetMETPhi[2];
  TH1F *h_jetpt_jetveto[2];
  TH1F *h_jetpt_HLT[2];
  TH1F *h_dr_phojet[2];
  TH1F *h_dEta_phojet[2];
  TH1F *h_dPhi_phojet[2];
  TH1F *h_dr_jetjet;
  TH1F *h_dEta_jetjet;

  for(Int_t j=0; j<2; j++){
    h_jetpt[j] = new TH1F(Form("h_jetpt_jet%i", j), "jetpt phoEB cut", 25, 30, 1030);
    h_jetpt_200[j] = new TH1F(Form("h_jetpt_200_jet%i", j), "jetpt phoEB 200 cut", 25, 30, 1030);
    h_jetpt_M[j] = new TH1F(Form("h_jetpt_M_jet%i", j), "jetpt phoEB M cut", 25, 30, 1030);
    h_jetpt_chworst[j] = new TH1F(Form("h_jetpt_chworst_jet%i", j), "jetpt phoEB chworst cut", 25, 30, 1030);
    h_jetpt_SeedTime[j] = new TH1F(Form("h_jetpt_SeedTime_jet%i", j), "jetpt phoEB SeedTime cut", 25, 30, 1030);
    h_jetpt_phoptoverMET[j] = new TH1F(Form("h_jetpt_phoptoverMET_jet%i", j), "jetpt phoEB phoptoverMET cut", 25, 30, 1030);
    h_jetpt_MET[j] = new TH1F(Form("h_jetpt_MET_jet%i", j), "jetpt phoEB MET cut", 25, 30, 1030);
    h_jetpt_dphoMETPhi[j] = new TH1F(Form("h_jetpt_dphoMETPhi_jet%i", j), "jetpt phoEB dphoMETPhi cut", 25, 30, 1030);
    h_jetpt_djetMETPhi[j] = new TH1F(Form("h_jetpt_djetMETPhi_jet%i", j), "jetpt phoEB djetMETPhi cut", 25, 30, 1030);
    h_jetpt_jetveto[j] = new TH1F(Form("h_jetpt_jetveto_jet%i", j), "jetpt phoEB jetveto cut", 25, 30, 1030);
    h_jetpt_HLT[j] = new TH1F(Form("h_jetpt_HLT_jet%i", j), "jetpt phoEB HLT", 25, 30, 1030);
    h_dr_phojet[j] = new TH1F(Form("h_dr_phojet_jet%i", j), "pho jet dr", 14, 0., 7);
    h_dEta_phojet[j] = new TH1F(Form("h_dEta_phojet_jet%i", j), "pho jet dEta", 14, 0., 7);
    h_dPhi_phojet[j] = new TH1F(Form("h_dPhi_phojet_jet%i", j), "pho jet dPhi", 30, -3.14, 3.14);
    
    h_jetpt[j]->Sumw2();
    h_jetpt_200[j]->Sumw2();
    h_jetpt_M[j]->Sumw2();
    h_jetpt_chworst[j]->Sumw2();
    h_jetpt_SeedTime[j]->Sumw2();
    h_jetpt_phoptoverMET[j]->Sumw2();
    h_jetpt_MET[j]->Sumw2();
    h_jetpt_dphoMETPhi[j]->Sumw2();
    h_jetpt_djetMETPhi[j]->Sumw2();
    h_jetpt_jetveto[j]->Sumw2();
    h_jetpt_HLT[j]->Sumw2();
    h_dr_phojet[j]->Sumw2();
    h_dEta_phojet[j]->Sumw2();
    h_dPhi_phojet[j]->Sumw2();
  }

  h_dr_jetjet = new TH1F("h_dr_jetjet", "jet jet dR", 14, 0, 7);
  h_dEta_jetjet = new TH1F("h_dEta_jetjet", "jet jet dEta", 14, 0, 7);
  h_dr_jetjet->Sumw2();
  h_dEta_jetjet->Sumw2();
  
  //tree variables
  Bool_t isData;
  Float_t puwei_ = 1.;
  Int_t    run, year;
  Long64_t event;
  Float_t genWeight;
  Float_t SumofGenW = 0.0;
  Int_t nVtx, nPU;
  Float_t npho_, nmcpho_, matchpho_;
  Int_t isMatched;

  Float_t pfMET =0;
  Float_t pfMETPhi =0;
  Float_t pfMET_cut =0;
  Float_t pfMETPhi_cut =0;
  Float_t genMET =0;
  Float_t genMETPhi =0;

  Float_t SeedTime_, SeedEnergy_, MIPEnergy_;
  Float_t SeedTime_cut, SeedEnergy_cut, MIPEnergy_cut;
  Float_t mcphoEt, mcphoEta, mcphoPhi, realphoEt, realphoEta, realphoPhi;
  Int_t   eleVeto, hasSeed;
  Float_t HoverE, sieie, chIso, phoIso, nhIso, chWorst, rho_;
  Float_t chIso_, phoIso_, nhIso_, chWorst_, chWorst_newEA;
  Float_t sieieFull5x5, sieipFull5x5, sipipFull5x5, r9Full5x5;
  Float_t HoverE_cut, chIso_cut, phoIso_cut, nhIso_cut, chWorst_cut, chWorst_newEA_cut, rho_cut;
  Float_t sieieFull5x5_cut, sieipFull5x5_cut, sipipFull5x5_cut, r9Full5x5_cut;

  Float_t chcut[30] = {0.1, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0, 10, 12, 15};//22 cut
  
  TTree *outtree_;
  outtree_ = new TTree("t", "mini tree");

  outtree_->Branch("run", &run, "run/I");
  outtree_->Branch("event", &event, "event/L");
  outtree_->Branch("isData", &isData, "isData/O");
  outtree_->Branch("puwei",       &puwei_, "puwei/F");
  outtree_->Branch("genWeight", &genWeight, "genWeight/F");
  
  outtree_->Branch("mcphoEt",     &mcphoEt, "mcphoEt/F");
  outtree_->Branch("mcphoEta",    &mcphoEta, "mcphoEta/F");
  outtree_->Branch("mcphoPhi",    &mcphoPhi, "mcphoPhi/F");
  outtree_->Branch("realphoEt",   &realphoEt, "realphoEt/F");
  outtree_->Branch("realphoEta",  &realphoEta, "realphoEta/F");
  outtree_->Branch("realphoPhi",  &realphoPhi, "realphoPhi/F");

  outtree_->Branch("HoverE",      &HoverE,       "HoverE/F");
  outtree_->Branch("rho",         &rho_,         "rho/F");
  outtree_->Branch("chIso",       &chIso,        "chIso/F");
  outtree_->Branch("phoIso",      &phoIso,       "phoIso/F");
  outtree_->Branch("nhIso",       &nhIso,        "nhIso/F");
  outtree_->Branch("chWorst",     &chWorst,      "chWorst/F");
  
  outtree_->Branch("chIso_rc",    &chIso_,       "chIso_rc/F");
  outtree_->Branch("phoIso_rc",   &phoIso_,      "phoIso_rc/F");
  outtree_->Branch("nhIso_rc",    &nhIso_,       "nhIso_rc/F");
  outtree_->Branch("chWorst_rc",  &chWorst_,     "chWorst_rc/F");
  outtree_->Branch("chWorst_rcnewEA", &chWorst_newEA, "chWorst_rcnewEA/F");
  outtree_->Branch("sieieFull5x5",        &sieieFull5x5,        "sieieFull5x5/F");
  outtree_->Branch("sieipFull5x5",        &sieipFull5x5,        "sieipFull5x5/F");
  outtree_->Branch("sipipFull5x5",        &sipipFull5x5,        "sipipFull5x5/F");
  outtree_->Branch("r9Full5x5",           &r9Full5x5,           "r9Full5x5/F");

  outtree_->Branch("HoverE_cut",       &HoverE_cut, "HoverE_cut/F");
  outtree_->Branch("rho_cut",          &rho_cut,    "rho_cut/F");
  outtree_->Branch("chIso_cut",        &chIso_cut, "chIso_cut/F");
  outtree_->Branch("phoIso_cut",       &phoIso_cut, "phoIso_cut/F");
  outtree_->Branch("nhIso_cut",        &nhIso_cut, "nhIso_cut/F");
  outtree_->Branch("chWorst_cut",      &chWorst_cut, "chWorst_cut/F");
  outtree_->Branch("chWorst_newEA_cut", &chWorst_newEA_cut, "chWorst_newEA_cut/F");
  outtree_->Branch("sieieFull5x5_cut", &sieieFull5x5_cut, "sieieFull5x5_cut/F");
  outtree_->Branch("sieipFull5x5_cut", &sieipFull5x5_cut, "sieipFull5x5_cut/F");
  outtree_->Branch("sipipFull5x5_cut", &sipipFull5x5_cut, "sipipFull5x5_cut/F");
  outtree_->Branch("r9Full5x5_cut",    &r9Full5x5_cut, "r9Full5x5_cut/F");

  outtree_->Branch("SeedTime",     &SeedTime_,     "SeedTime/F");
  outtree_->Branch("SeedEnergy",   &SeedEnergy_,   "SeedEnergy/F");
  outtree_->Branch("MIPEnergy", &MIPEnergy_, "MIPEnergy/F");
  outtree_->Branch("SeedTime_cut",     &SeedTime_cut,     "SeedTime_cut/F");
  outtree_->Branch("SeedEnergy_cut",   &SeedEnergy_cut,   "SeedEnergy_cut/F");
  outtree_->Branch("MIPEnergy_cut", &MIPEnergy_cut, "MIPEnergy_cut/F");

  outtree_->Branch("genMET", &genMET, "genMET/F");
  outtree_->Branch("genMETPhi", &genMETPhi, "genMETPhi/F");
  outtree_->Branch("pfMET", &pfMET, "pfMET/F");
  outtree_->Branch("pfMETPhi", &pfMETPhi, "pfMETPhi/F");
  outtree_->Branch("pfMET_cut", &pfMET_cut, "pfMET_cut/F");
  outtree_->Branch("pfMETPhi_cut", &pfMETPhi_cut, "pfMETPhi_cut/F");

  // pileup reweighting
  PUWeightCalculator puCalc;
  puCalc.Init(PUpathes);

  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);
    run     = data.GetInt("run");
    if(run > 274000 && run < 284000) year = 16;
    event   = data.GetLong64("event"); 
    isData = data.GetBool("isData");
    nVtx = data.GetInt("nVtx");
    
    Int_t nPho     = data.GetInt("nPho");
    Int_t nJet     = data.GetInt("nJet");
    h_npho->Fill(nPho);
    h_njet->Fill(nJet);

    genWeight = 1.0;
    if(isData==0){
      genWeight = data.GetFloat("genWeight");
      hGenWeight->Fill(genWeight);
      hSumofGenW->Fill(0.5, genWeight);
    }
    
    Int_t    nPUInfo =0;
    Int_t*   puBX=0; 
    Float_t* puTrue=0;
    if(isData==0){
      nPUInfo = data.GetInt("nPUInfo");
      puBX    = data.GetPtrInt("puBX");
      puTrue  = data.GetPtrFloat("puTrue");
      for (Int_t i=0; i<nPUInfo; ++i) {
	if (puBX[i] == 0) nPU = puTrue[i];
      }      
      puwei_ = 1.;
      puwei_ = (Float_t)puCalc.GetWeight(run, puTrue[1]);
    }
    Int_t nVtx = data.GetInt("nVtx");

    //reco pho
    Float_t* phoE = 0; 
    Float_t* phoEt = 0;
    Float_t* phoEta = 0;
    Float_t* phoSCEta = 0;
    Float_t* phoPhi = 0;
    Float_t* phoHoverE = 0; 
    Float_t* phoPFChIso          = 0; 
    Float_t* phoPFNeuIso         = 0; 
    Float_t* phoPFPhoIso         = 0;
    Int_t*   phoEleVeto     = 0;
    Float_t* phoSigmaIEtaIEtaFull5x5  = 0;
    Float_t* phoSigmaIEtaIPhiFull5x5  = 0;
    Float_t* phoSigmaIPhiIPhiFull5x5  = 0;
    Float_t* phoR9Full5x5           = 0;
    Float_t* phoPFChWorst   = 0; 
    Float_t  rho           = 0; 
    Short_t* phoID         = 0;
    Long64_t* phoFiredTrgs = 0;
    Float_t* phoSeedTime    =0;
    Float_t* phoSeedEnergy  =0;
    Float_t* phoMIPTotEnergy= 0;

    phoE = data.GetPtrFloat("phoE");
    phoEt = data.GetPtrFloat("phoEt");
    phoEta = data.GetPtrFloat("phoEta");
    phoSCEta = data.GetPtrFloat("phoSCEta");
    phoPhi = data.GetPtrFloat("phoPhi");
    phoHoverE = data.GetPtrFloat("phoHoverE");
    phoPFChIso          = data.GetPtrFloat("phoPFChIso");
    phoPFNeuIso         = data.GetPtrFloat("phoPFNeuIso");
    phoPFPhoIso         = data.GetPtrFloat("phoPFPhoIso");
    phoEleVeto     = data.GetPtrInt("phoEleVeto");
    phoSigmaIEtaIEtaFull5x5  = data.GetPtrFloat("phoSigmaIEtaIEtaFull5x5");
    phoSigmaIEtaIPhiFull5x5  = data.GetPtrFloat("phoSigmaIEtaIPhiFull5x5");
    phoSigmaIPhiIPhiFull5x5  = data.GetPtrFloat("phoSigmaIPhiIPhiFull5x5");
    phoR9Full5x5           = data.GetPtrFloat("phoR9Full5x5");
    phoPFChWorst             = data.GetPtrFloat("phoPFChWorstIso");
    rho                    = data.GetFloat("rho");
    phoID         = data.GetPtrShort("phoIDbit");
    phoFiredTrgs = data.GetPtrLong64("phoFiredSingleTrgs");
    phoSeedTime     = data.GetPtrFloat("phoSeedTime");
    //phoSeedEnergy   = data.GetPtrFloat("phoSeedEnergy");
    //phoMIPTotEnergy = data.GetPtrFloat("phoMIPTotEnergy");
    pfMET     = data.GetFloat("pfMET");
    pfMETPhi  = data.GetFloat("pfMETPhi");
    
    //rho correction of pho isolation
    vector<vector<Float_t>> Iso_raw; //[ch, pho, nh, chw]
    vector<vector<Float_t>> Iso_rc; //[ch, pho, nh, chw, chw_newEA]
    Iso_raw.clear();
    Iso_rc.clear();
    for(Int_t iso = 0; iso < 5; iso++){
      vector <Float_t> isolist;
      rhoCorrection(iso, data, isolist);
      Iso_rc.push_back(isolist);
      vector<Float_t> oisolist;
      rawIsolation(iso, data, oisolist);
      Iso_raw.push_back(oisolist);
    }

    //cal tot weight
    vector<Double_t> wei_list; wei_list.clear();
    Double_t phoSF = 1.0;
    Double_t CSVSF = 1.0;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(isData==0){
	if(phoSCEta[ipho] > -1.479 && phoSCEta[ipho] <- 0.8) phoSF = h_phoIDSF->GetBinContent(4, 5);
	else if(phoSCEta[ipho] >= -0.8 && phoSCEta[ipho] < 0.0) phoSF = h_phoIDSF->GetBinContent(5, 5);
	else if(phoSCEta[ipho] >= 0.0 && phoSCEta[ipho] < 0.8) phoSF = h_phoIDSF->GetBinContent(6, 5);
	else if(phoSCEta[ipho] >= 0.8 && phoSCEta[ipho] < 1.479) phoSF = h_phoIDSF->GetBinContent(7, 5);

	//CSVSF = h_CSVSF->GetBinContent(1);//r9 inclusive
	CSVSF = h_CSVSF->GetBinContent(1, 1);//r9 inclusive[2D 2016]
	//if(phoR9Full5x5[ipho] >= 0.94) CSVSF = h_CSVSF->GetBinContent(2);
	//else if (phoR9Full5x5[ipho] < 0.94) CSVSF = h_CSVSF->GetBinContent(3);
      }
      Double_t tot_wei = phoSF*CSVSF*puwei_*genWeight;
      wei_list.push_back(tot_wei);
    }

    //MC 
      Int_t    nMC   =0;     
      Int_t*   mcPID =0;
      Int_t*   mcMomPID =0;
      Float_t* mcPt      =0;
      Float_t* mcEta     =0;
      Float_t* mcPhi     =0;
      Float_t* mcE       =0;
      Float_t* mcEt      =0;
      Float_t* mcMass    =0;
      Short_t* mcStatus  =0;
      Float_t* mcMomMass =0;
      Float_t* mcMomPt   =0;
      Float_t* mcMomEta  =0;
      Float_t* mcMomPhi  =0;
      Float_t* mcMomE    =0;
      
    if(isData==0){
      nMC       = data.GetInt("nMC");
      mcPID     = data.GetPtrInt("mcPID");
      mcPt      = data.GetPtrFloat("mcPt");
      mcEta     = data.GetPtrFloat("mcEta");
      mcPhi     = data.GetPtrFloat("mcPhi");
      mcE       = data.GetPtrFloat("mcE");
      mcMass    = data.GetPtrFloat("mcMass");
      mcStatus  = data.GetPtrShort("mcStatusFlag");
      mcMomPID  = data.GetPtrInt("mcMomPID");
      mcMomMass = data.GetPtrFloat("mcMomMass");
      mcMomPt   = data.GetPtrFloat("mcMomPt");
      mcMomEta  = data.GetPtrFloat("mcMomEta");
      mcMomPhi  = data.GetPtrFloat("mcMomPhi");
      genMET    = data.GetFloat("genMET");
      genMETPhi = data.GetFloat("genMETPhi");
    }

    /*
    Int_t metFilters;
    //metFilters for data
    if(isData==1){
      metFilters = data.GetInt("metFilters");
      for(Int_t ifilter=0; ifilter<9; ifilter++){
	
      }
    }
    */
    
    //jet veto selection
    Float_t	*jetPt		= data.GetPtrFloat("jetPt");
    Float_t	*jetEta		= data.GetPtrFloat("jetEta");
    Float_t	*jetPhi		= data.GetPtrFloat("jetPhi");
    Float_t	*jetEn		= data.GetPtrFloat("jetEn");
   
    Int_t	*jetPUFullID	= data.GetPtrInt("jetPUFullID");
    Int_t	*jetID		= data.GetPtrInt("jetID");

    Float_t	*jetSmear;
    //jet smearing
    if(isData==0){
      jetSmear = data.GetPtrFloat("jetP4Smear");
      for(Int_t ijet=0; ijet<nJet; ijet++){
	jetPt[ijet]  = jetPt[ijet]*jetSmear[ijet];
	if(jetPt[ijet]<0) jetPt[ijet] = 0;
      }
    }
    
    Int_t njetVeto =0;
    Int_t npfjet =0;
    Int_t nVBSjet =0;
    Int_t jetpass =0;
    Int_t jetpassVBS =0;
    Float_t mindjetMETPhi =4.;
    vector<Int_t> jetveto_list; jetveto_list.clear();
    vector<Int_t> jetVBS_list; jetVBS_list.clear();
    for(Int_t i=0; i<nJet; i++){
      if(fabs(deltaPhi(jetPhi[i], pfMETPhi)) < mindjetMETPhi) mindjetMETPhi = fabs(deltaPhi(jetPhi[i], pfMETPhi));
      if(jetPUFullID[i] == 4 && jetPt[i] > 30){
	jetveto_list.push_back(i);
	npfjet++;
	njetVeto++;
      }

      if((jetPUFullID[i] == 6) && (jetPt[i] > 30) && fabs(jetEta[i]) < 4.7 && (jetID[i] == 6)){
	  jetVBS_list.push_back(i);
	  nVBSjet++;
      }
    }
    if(njetVeto > 1) jetpass = 1;
    if(nVBSjet > 1) jetpassVBS = 1;

    //phoID selection
    vector<Int_t> cutIDpho_list; cutIDpho_list.clear();//Medium Id
    phoIDcut(1, data, cutIDpho_list);

    //set cutflow ID
    vector<Int_t> cutflowSMID; cutflowSMID.clear();
    vector<Int_t> cutflowVBSID; cutflowVBSID.clear();

    Int_t bit=0;
    Double_t djetEta = 0.0;
    if(jetVBS_list.size() > 1){
      //djetEta = deltaEta(jetEta[jetVBS_list[0]], jetEta[jetVBS_list[1]]);
      djetEta = jetEta[jetVBS_list[0]] - jetEta[jetVBS_list[1]];
    }
    else djetEta = 999;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      bit=0;
      //InitBit(bit, 12);
      //if(match[ipho] == 1) bit = SetBit(0, bit);
      bit = SetBit(0, bit);
      if(phoEleVeto[ipho] == 1) bit = SetBit(1, bit);
      if(fabs(phoSCEta[ipho]) < 1.4442) bit = SetBit(2, bit);
      if(phoEt[ipho] > 200) bit = SetBit(3, bit);
      if(cutIDpho_list[ipho] == 1) bit = SetBit(4, bit);
      if(Iso_rc[4][ipho] < 1.3) bit = SetBit(5, bit);
      if(phoSeedTime[ipho]<3) bit = SetBit(6, bit);
      //if(phoEt[ipho]/pfMET<(1.014+0.00475*phoEt[ipho])) bit = SetBit(6, bit);
      if(pfMET > 140+(0.138*(phoEt[ipho]-500))) bit = SetBit(7, bit);
      if(fabs(deltaPhi(phoPhi[ipho], pfMETPhi)) > 1.2) bit = SetBit(8, bit);
      if(mindjetMETPhi > 0.5) bit = SetBit(9, bit);
      if(jetpass < 1) bit = SetBit(10, bit);
      if(year == 16 && (phoFiredTrgs[ipho]>>7&1) == 1) bit = SetBit(11, bit);//singlePho HLT175
      if(year > 16 && (phoFiredTrgs[ipho]>>8&1) == 1) bit = SetBit(11, bit);//singlePho HLT200
      //if((phoFiredTrgs[ipho]>>6&1) == 1) bit = SetBit(14, bit);
      cutflowSMID.push_back(bit);

      
      bit = ResetBit(10, bit);
      Double_t dr_phojet1, dr_phojet2, dr_jetjet;
      if(jetVBS_list.size() > 1){
	dr_phojet1 = deltaR(phoEta[ipho], phoPhi[ipho], jetEta[jetVBS_list[0]], jetPhi[jetVBS_list[0]]);
	dr_phojet2 = deltaR(phoEta[ipho], phoPhi[ipho], jetEta[jetVBS_list[1]], jetPhi[jetVBS_list[1]]);
	dr_jetjet = deltaR(jetEta[jetVBS_list[0]], jetPhi[jetVBS_list[0]], jetEta[jetVBS_list[1]], jetPhi[jetVBS_list[1]]);
      }
      else{dr_phojet1 = -999; dr_phojet2 = -999; dr_jetjet = -999;}
      if(jetpassVBS > 0 && fabs(djetEta) > 2.5 && (dr_phojet1 > 0.4) && (dr_phojet2 > 0.4) && (dr_jetjet > 0.5)) bit = SetBit(10, bit);
      cutflowVBSID.push_back(bit);
    }
    
    vector<Int_t> phoEB_pt, phoEB_pt_cut;
    phoEB_pt.clear();
    phoEB_pt_cut.clear();

    Int_t leadingpho = 1;
    if(nPho<1) continue;
    
    for(Int_t ipho=0; ipho<leadingpho; ipho++){
      WEIGHT = wei_list[ipho];
      Int_t cutflow[2];
      cutflow[0] = cutflowSMID[ipho];
      cutflow[1] = cutflowVBSID[ipho];

      if(pho_sel(cutflow[0], 4) == 1){
	phoEB_pt.push_back(ipho);
	
	for(Int_t j=0; j<7; j++){
	  if(fabs(phoSCEta[ipho]) >= EAbin[j] && fabs(phoSCEta[ipho]) < EAbin[j+1]){
	    h_chIso_rho[j]->Fill(rho, Iso_raw[0][ipho], WEIGHT);
	    h_chworst_rho[j]->Fill(rho, Iso_raw[3][ipho], WEIGHT);
	  }
	}

	for(Int_t j=0; j<22; j++){
	  if(Iso_rc[0][ipho] < chcut[j]) h_phoEB_pt_chIsocut[j]->Fill(phoEt[ipho], WEIGHT);//chIso
	  if(Iso_rc[3][ipho] < chcut[j]) h_phoEB_pt_chworstcut[j]->Fill(phoEt[ipho], WEIGHT);//chWorst
	  if(Iso_rc[4][ipho] < chcut[j]) h_phoEB_pt_chworstcut_newEA[j]->Fill(phoEt[ipho], WEIGHT);//chWorst_newEA
	}
      }

      if(pho_sel(cutflow[0], 5) == 1){
	phoEB_pt_cut.push_back(ipho);
	h2_MET_phoEt->Fill(pfMET, phoEt[ipho], WEIGHT);
	h2_phoEt_ptoverMET->Fill(phoEt[ipho], phoEt[ipho]/pfMET, WEIGHT);
	for(Int_t j=0; j<12; j++){
	  if((phoEt[ipho]/pfMET) < ptoverMETbin[j]) h_phoEB_pt_phoMET_vcut[j]->Fill(phoEt[ipho], WEIGHT);
	}

	for(Int_t j=0; j<6; j++){
	  if(pfMET>METbin[j]) h_phoEB_pt_MET_vcut[j]->Fill(phoEt[ipho], WEIGHT);
	}
      }

      
      
      for(Int_t j=0; j<2; j++){
	//cutflow start
	if(pho_sel(cutflow[j], 2) == 1) h_phoEB_pt[j]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[j], 3) == 1) h_phoEB_pt_200[j]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[j], 4) == 1) h_phoEB_pt_M[j]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[j], 5) == 1) h_phoEB_pt_chworst[j]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[j], 6) == 1) h_phoEB_pt_SeedTime[j]->Fill(phoEt[ipho], WEIGHT);
	//if(pho_sel(cutflow[j], 6) == 1) h_phoEB_pt_phoptoverMET[j]->Fill(phoEt[ipho], WEIGHT);
	//if(pho_sel(cutflow[j], 5) == 1 && (phoEt[ipho]/pfMET) < (0.22+0.008*phoEt[ipho])) h_phoEB_pt_phoptoverMET[j]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[j], 7) == 1) h_phoEB_pt_MET[j]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[j], 8) == 1) h_phoEB_pt_dphoMETPhi[j]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[j], 9) == 1) h_phoEB_pt_djetMETPhi[j]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[j], 10) == 1) h_phoEB_pt_jetveto[j]->Fill(phoEt[ipho], WEIGHT);
	if(pho_sel(cutflow[j], 11) == 1) h_phoEB_pt_HLT[j]->Fill(phoEt[ipho], WEIGHT);

	//result
	if(pho_sel(cutflow[j], 11) == 1){
	  h_phoEB_ptcut[j]->Fill(phoEt[ipho], WEIGHT);
	  h_phoEB_etacut[j]->Fill(phoEt[ipho], WEIGHT);
	  h_phoEB_ptoverMET_cut[j]->Fill(phoEt[ipho]/pfMET, WEIGHT);
	  h_SeedTime_cut[j]->Fill(phoSeedTime[ipho], WEIGHT);
	  h_MET_cut[j]->Fill(pfMET, WEIGHT);
	  h_dphoMETPhi_cut[j]->Fill(deltaPhi(phoPhi[ipho], pfMETPhi), WEIGHT);
	  h_nvtx_cut[j]->Fill(nVtx, WEIGHT);
	}

	if(j==0 && pho_sel(cutflow[j], 11) == 1){
	  h_njet_cut[j]->Fill(npfjet, WEIGHT);
	  for(Int_t jj=0; jj<npfjet; jj++){
	    Int_t ijet = jetveto_list[jj];
	    h_jetpt_cut[j]->Fill(jetPt[ijet], WEIGHT);
	    h_djetMETPhi_cut[j]->Fill(deltaPhi(jetPhi[ijet], pfMETPhi), WEIGHT);
	  }
	}

	if(j==1 && pho_sel(cutflow[j], 11) == 1){
	  h_njet_cut[j]->Fill(nVBSjet, WEIGHT);
	  for(Int_t jj=0; jj<nVBSjet; jj++){
	    Int_t ijet = jetVBS_list[jj];
	    h_jetpt_cut[j]->Fill(jetPt[ijet], WEIGHT);
	    h_djetMETPhi_cut[j]->Fill(deltaPhi(jetPhi[ijet], pfMETPhi), WEIGHT);
	  }
	}
	
	//Nm1 plot
	//if(Nm1_sel(cutflow[j], 6) == 1) h_phoEB_ptoverMET_Nm1[j]->Fill(phoEt[ipho]/pfMET, WEIGHT);
	if(Nm1_sel(cutflow[j], 6) == 1) h_SeedTime_Nm1[j]->Fill(phoSeedTime[ipho], WEIGHT);
	if(Nm1_sel(cutflow[j], 7) == 1) h_MET_Nm1[j]->Fill(pfMET, WEIGHT);
	if(Nm1_sel(cutflow[j], 8) == 1) h_dphoMETPhi_Nm1[j]->Fill(deltaPhi(phoPhi[ipho], pfMETPhi), WEIGHT);

	if(j==0 && Nm1_sel(cutflow[j], 9) == 1){
	  //h_njet_Nm1[j]->Fill(npfjet, WEIGHT);
	  for(Int_t jj=0; jj<npfjet; jj++){
	    Int_t ijet = jetveto_list[jj];
	    //h_jetpt_Nm1[j]->Fill(jetPt[ijet], WEIGHT);
	    h_djetMETPhi_Nm1[j]->Fill(deltaPhi(jetPhi[ijet], pfMETPhi), WEIGHT);
	  }
	}

	if(j==0 && Nm1_sel(cutflow[j], 10) == 1){
	  h_njet_Nm1[j]->Fill(npfjet, WEIGHT);
	  for(Int_t jj=0; jj<npfjet; jj++){
	    Int_t ijet = jetveto_list[jj];
	    h_jetpt_Nm1[j]->Fill(jetPt[ijet], WEIGHT);
	    //h_djetMETPhi_Nm1[j]->Fill(deltaPhi(jetPhi[ijet], pfMETPhi), WEIGHT);
	  }
	}

	if(j==1 && Nm1_sel(cutflow[j], 9) == 1){
	  //h_njet_Nm1[j]->Fill(npfjet, WEIGHT);
	  for(Int_t jj=0; jj<nVBSjet; jj++){
	    Int_t ijet = jetVBS_list[jj];
	    //h_jetpt_Nm1[j]->Fill(jetPt[ijet], WEIGHT);
	    h_djetMETPhi_Nm1[j]->Fill(deltaPhi(jetPhi[ijet], pfMETPhi), WEIGHT);
	  }
	}

	if(j==1 && Nm1_sel(cutflow[j], 10) == 1){
	  h_njet_Nm1[j]->Fill(nVBSjet, WEIGHT);
	  for(Int_t jj=0; jj<nVBSjet; jj++){
	    Int_t ijet = jetVBS_list[jj];
	    h_jetpt_Nm1[j]->Fill(jetPt[ijet], WEIGHT);
	    //h_djetMETPhi_Nm1[j]->Fill(deltaPhi(jetPhi[ijet], pfMETPhi), WEIGHT);
	  }
	}
	
	//jetpt
	if(j==1 && jetVBS_list.size()>1){
	  for(Int_t jj=0; jj<2; jj++){
	    Int_t ijet = jetVBS_list[jj];
	    if(pho_sel(cutflow[j], 11) == 1) {
	      h_jetpt_HLT[jj]->Fill(jetPt[ijet], WEIGHT);
	      h_dEta_phojet[jj]->Fill(fabs(deltaEta(phoSCEta[ipho], jetEta[ijet])), WEIGHT);
	      h_dPhi_phojet[jj]->Fill(deltaPhi(phoPhi[ipho], jetPhi[ijet]), WEIGHT);
	      h_dr_phojet[jj]->Fill(deltaR(phoSCEta[ipho], phoPhi[ipho], jetEta[ijet], jetPhi[ijet]), WEIGHT);
	    }
	  }

	  if(pho_sel(cutflow[j], 11) == 1){
	  h_dr_jetjet->Fill(deltaR(jetEta[jetVBS_list[0]], jetPhi[jetVBS_list[0]], jetEta[jetVBS_list[1]], jetPhi[jetVBS_list[1]]), WEIGHT);
	  h_dEta_jetjet->Fill(fabs(deltaEta(jetEta[jetVBS_list[0]], jetEta[jetVBS_list[1]])), WEIGHT);
	  }
	}
	
      }
    }

    r9Full5x5    =  -999;
    HoverE       =  -999;
    rho_         =  -999;
    sieieFull5x5 =  -999;
    sieipFull5x5 =  -999;
    sipipFull5x5 =  -999;
    chIso    = -999;
    phoIso   = -999;
    nhIso    = -999;
    chWorst  = -999;
    chIso_   = -999;
    phoIso_  = -999;
    nhIso_   = -999;
    chWorst_ = -999;
    chWorst_newEA = -999;

    SeedTime_ = -999;
    SeedEnergy_ = -999;
    MIPEnergy_ = -999;
    
    mcphoEt = -999;
    mcphoEta = -999;
    mcphoPhi = -999;
    realphoEt = -999;
    realphoEta = -999;
    realphoPhi = -999;

    hasSeed = -999;
      
    r9Full5x5_cut = -999;
    HoverE_cut = -999;
    sieieFull5x5_cut = -999;
    sieipFull5x5_cut = -999;
    sipipFull5x5_cut = -999;
    chIso_cut = -999;
    phoIso_cut = -999;
    nhIso_cut = -999;
    chWorst_cut = -999;

    SeedTime_cut = -999;
    //SeedEnergy_cut = -999;
    //MIPEnergy_cut  = -999;
	      
    pfMET_cut = -999;
    pfMETPhi_cut = -999;

    for(Int_t ii=0; ii<phoEB_pt.size(); ii++){
      Int_t ipho = phoEB_pt[ii];
      if(ii>0) continue;
      r9Full5x5    =  phoR9Full5x5[ipho]                                                   ;
      HoverE       =  phoHoverE[ipho];
      rho_         =  rho;
      sieieFull5x5 =  phoSigmaIEtaIEtaFull5x5[ipho];
      sieipFull5x5 =  phoSigmaIEtaIPhiFull5x5[ipho];
      sipipFull5x5 =  phoSigmaIPhiIPhiFull5x5[ipho];
      chIso    = Iso_raw[0][ipho];
      phoIso   = Iso_raw[1][ipho];
      nhIso    = Iso_raw[2][ipho];
      chWorst  = Iso_raw[3][ipho];
      chIso_   = Iso_rc[0][ipho];
      phoIso_  = Iso_rc[1][ipho];
      nhIso_   = Iso_rc[2][ipho];
      chWorst_ = Iso_rc[3][ipho];
      chWorst_newEA = Iso_rc[4][ipho];

      SeedTime_ = phoSeedTime[ipho];
              
      //for(Int_t jj=0; jj<phoEB_pt_cut.size(); jj++){
      if(phoEB_pt_cut.size() < 1) continue;
      if(ipho == phoEB_pt_cut[0]){
	if(isData==0){
	  mcphoEt = mcPt[ipho];
	  mcphoEta = mcEta[ipho];
	  mcphoPhi = mcPhi[ipho];
	}
	realphoEt = phoEt[ipho];
	realphoEta = phoEta[ipho];
	realphoPhi = phoPhi[ipho];

	//hasSeed = phoEleVeto[ipho];
      
	r9Full5x5_cut = phoR9Full5x5[ipho];
	HoverE_cut = phoHoverE[ipho];
	sieieFull5x5_cut = phoSigmaIEtaIEtaFull5x5[ipho];
	sieipFull5x5_cut = phoSigmaIEtaIPhiFull5x5[ipho];
	sipipFull5x5_cut = phoSigmaIPhiIPhiFull5x5[ipho];
	chIso_cut = Iso_rc[0][ipho];
	phoIso_cut = Iso_rc[1][ipho];
	nhIso_cut = Iso_rc[2][ipho];
	chWorst_cut = Iso_rc[3][ipho];
	chWorst_newEA_cut = Iso_rc[4][ipho];

	SeedTime_cut = phoSeedTime[ipho];
	      
	pfMET_cut = pfMET;
	pfMETPhi_cut = pfMETPhi;
      }
    }
    outtree_->Fill();
  }
  
  //****************END LOOP**********************//
      
  //****************Terminate*********************//
  fout_->cd();
  outtree_->Write();

  hEvents->Write();
  hGenWeight->Write();
  hSumofGenW->Write();
  h_npho->Write();
  h_nmcpho->Write();
  h_nmatchpho->Write();
  h_dr_pho->Write();
  h_dpt_pho->Write();
  h_dptdr_pho->Write();
  h_mcStatus->Write();
  
  h_njet->Write();
  h2_MET_phoEt->Write();
  h2_phoEt_ptoverMET->Write();
  
  h2_rho_phoEB_pt_M->Write();
  h2_rho_phoEB_pt_chworst->Write();
  h2_rho_phoEB_pt_chworst_newEA->Write();

  h_dr_jetjet->Write();
  h_dEta_jetjet->Write();

  fout_->mkdir("SMandVBS");
  fout_->cd("SMandVBS");
  for(Int_t i=0; i<2; i++){
    h_phoEB_pt[i]->Write();
    h_phoEB_pt_200[i]->Write();
    h_phoEB_pt_M[i]->Write();
    h_phoEB_pt_chworst[i]->Write();
    h_phoEB_pt_SeedTime[i]->Write();
    h_phoEB_pt_phoptoverMET[i]->Write();
    h_phoEB_pt_MET[i]->Write();
    h_phoEB_pt_dphoMETPhi[i]->Write();
    h_phoEB_pt_djetMETPhi[i]->Write();
    h_phoEB_pt_jetveto[i]->Write();
    h_phoEB_pt_HLT[i]->Write();

    h_phoEB_ptcut[i]->Write();
    h_phoEB_etacut[i]->Write();
    h_dr_phojet[i]->Write();
  
    h_phoEB_ptoverMET_Nm1[i]->Write();
    h_phoEB_ptoverMET_cut[i]->Write();

    h_SeedTime_Nm1[i]->Write();
    h_SeedTime_cut[i]->Write();
    
    h_MET_Nm1[i]->Write();
    h_MET_cut[i]->Write();

    h_dphoMETPhi_Nm1[i]->Write();
    h_dphoMETPhi_cut[i]->Write();

    h_njet_Nm1[i]->Write();
    h_njet_cut[i]->Write();

    h_jetpt_Nm1[i]->Write();
    h_jetpt_cut[i]->Write();
  
    h_djetMETPhi_Nm1[i]->Write();
    h_djetMETPhi_cut[i]->Write();
  
    h_phoEB_ptoverjetpt_cut[i]->Write();
    h_nvtx_cut[i]->Write();
  }

  fout_->mkdir("h_jetpt");
  fout_->cd("h_jetpt");
  for(Int_t i=0; i<2; i++){
    h_jetpt[i]->Write();
    h_jetpt_200[i]->Write();
    h_jetpt_M[i]->Write();
    h_jetpt_chworst[i]->Write();
    h_jetpt_SeedTime[i]->Write();
    h_jetpt_phoptoverMET[i]->Write();
    h_jetpt_MET[i]->Write();
    h_jetpt_dphoMETPhi[i]->Write();
    h_jetpt_djetMETPhi[i]->Write();
    h_jetpt_jetveto[i]->Write();
    h_jetpt_HLT[i]->Write();

    h_dr_phojet[i]->Write();
    h_dEta_phojet[i]->Write();
    h_dPhi_phojet[i]->Write();
  }

  fout_->mkdir("h_phoEB_pt_phoMET_vcut");
  fout_->cd("h_phoEB_pt_phoMET_vcut");
  for(Int_t i=0; i<12; i++){
    h_phoEB_pt_phoMET_vcut[i]->Write();
  }

  fout_->mkdir("h_phoEB_pt_MET_vcut");
  fout_->cd("h_phoEB_pt_MET_vcut");
  for(Int_t i=0; i<6; i++){
    h_phoEB_pt_MET_vcut[i]->Write();
  }

  fout_->mkdir("h_phoEB_pt_chIsocut");
  fout_->cd("h_phoEB_pt_chIsocut");
  for(Int_t i=0; i<22; i++){
    h_phoEB_pt_chIsocut[i]->Write();
  }

  fout_->mkdir("h_phoEB_pt_chworstcut");
  fout_->cd("h_phoEB_pt_chworstcut");
  for(Int_t i=0; i<22; i++){
    h_phoEB_pt_chworstcut[i]->Write();
  }

  fout_->mkdir("h_phoEB_pt_chworstcut_newEA");
  fout_->cd("h_phoEB_pt_chworstcut_newEA");
  for(Int_t i=0; i<22; i++){
    h_phoEB_pt_chworstcut_newEA[i]->Write();
  }

  fout_->mkdir("h_chIso_rho");
  fout_->cd("h_chIso_rho");
  for(Int_t i=0; i<7; i++){
    h_chIso_rho[i]->Write();
  }

  fout_->mkdir("h_chworst_rho");
  fout_->cd("h_chworst_rho");
  for(Int_t i=0; i<7; i++){
    h_chworst_rho[i]->Write();
  }

  fout_->Close();
  fprintf(stderr, "Processed all events\n");
}

