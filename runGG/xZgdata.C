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

void xZgdata(char* pathes){

  //***********************Initialization***********************//
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("ggNtuplizer/hEvents")->Clone();
  hEvents->SetDirectory(0);
  fopen->Close();

  //access EventTree with TreeReader class
  TreeReader data(pathes);

  //create an output .root file
  TFile *fout_;
  fout_ = new TFile("output_ggtree_data.root","RECREATE");

  //create histograms in output .root file
  TH1F *h_npho = new TH1F("h_npho", "n pho", 10, 0., 10);
  TH1F *h_nmcpho = new TH1F("h_nmcpho", "n mcpho", 10, 0., 10);
  TH1F *h_nmatchpho = new TH1F("h_nmatchpho", "n matchpho", 10, 0., 10);
  TH1F *h_dr_pho = new TH1F("h_dr_pho", "dr of photon", 100, 0., 0.2);
  TH1F *h_dpt_pho = new TH1F("h_dpt_pho", "dpt of photon", 100, 0., 1);
  TH2F *h_dptdr_pho = new TH2F("h_dptdr_pho", "dptdr of photon", 100, 0., 1, 100, 0., 2);

  h_dr_pho->Sumw2();
  h_dpt_pho->Sumw2();
  h_dptdr_pho->Sumw2();
  
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
		     210, 220, 230, 250, 300, 350, 400, 500, 750, 1000, 1500, 2000, 3000, 10000};//24 bins, 2016
  Double_t etabin[10] = {-1.566, -1.4442, -0.8, 0, 0.8, 1.4442, 1.566};//6bins

  Float_t chbin[30] = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0, 10, 12, 15};//24 bins
  TH1F *h_phoEB_pt_chIsocut[22];
  TH1F *h_phoEB_pt_chworstcut[22];
  TH1F *h_phoEB_pt_chworstcut_newEA[22];
  for(Int_t j=0; j<22; j++){
    h_phoEB_pt_chIsocut[j] = new TH1F(Form("h_phoEB_pt_chIsocut_%i", j), Form("h_phoEB_pt_chIsocut_%i", j), 24, ptbin);
    h_phoEB_pt_chworstcut[j] = new TH1F(Form("h_phoEB_pt_chworstcut_%i", j), Form("h_phoEB_pt_chworstcut_%i", j), 24, ptbin);
    h_phoEB_pt_chworstcut_newEA[j] = new TH1F(Form("h_phoEB_pt_chworstcut_newEA_%i", j), Form("h_phoEB_pt_chworstcut_newEA_%i", j), 24, ptbin);

    h_phoEB_pt_chIsocut[j]->Sumw2();
    h_phoEB_pt_chworstcut[j]->Sumw2();
    h_phoEB_pt_chworstcut_newEA[j]->Sumw2();
  }

  TH1F *h_phoEB_pt = new TH1F("h_phoEB_pt", "matched phoEB pt", 24, ptbin);
  TH1F *h_phoEB_pt_200 = new TH1F("h_phoEB_pt_200", "matched phoEB pt pt200 cut", 24, ptbin);
  TH1F *h_phoEB_pt_M = new TH1F("h_phoEB_pt_M", "matched phoEB pt M IDcut", 24, ptbin);
  TH1F *h_phoEB_pt_chworst = new TH1F("h_phoEB_pt_chworst", "matched phoEB pt chworst cut", 24, ptbin);
  TH1F *h_phoEB_pt_SeedTime = new TH1F("h_phoEB_pt_SeedTime", "matched phoEB pt SeedTime cut", 24, ptbin);
  TH1F *h_phoEB_pt_r9 = new TH1F("h_phoEB_pt_r9", "matched phoEB pt r9 cut", 24, ptbin);
  TH1F *h_phoEB_pt_phoptoverMET = new TH1F("h_phoEB_pt_phoptoverMET", "matched phoEB pt phoptoverMET cut", 24, ptbin);
  TH1F *h_phoEB_pt_MET = new TH1F("h_phoEB_pt_MET", "matched phoEB pt MET cut", 24, ptbin);
  TH1F *h_phoEB_pt_dphoMETPhi = new TH1F("h_phoEB_pt_dphoMETPhi", "matched phoEB pt dphoMETPhi cut", 24, ptbin);
  TH1F *h_phoEB_pt_jetveto = new TH1F("h_phoEB_pt_jetveto", "matched phoEB pt jetveto", 24, ptbin);
  TH1F *h_phoEB_pt_djetMETPhi = new TH1F("h_phoEB_pt_djetMETPhi", "matched phoEB pt djetMETPhi cut", 24, ptbin);
  TH1F *h_phoEB_pt_HLT = new TH1F("h_phoEB_pt_HLT", "matched phoEB pt HLT trgs", 24, ptbin);

  h_phoEB_pt->Sumw2();
  h_phoEB_pt_200->Sumw2();
  h_phoEB_pt_M->Sumw2();
  h_phoEB_pt_chworst->Sumw2();
  h_phoEB_pt_SeedTime->Sumw2();
  h_phoEB_pt_r9->Sumw2();
  h_phoEB_pt_phoptoverMET->Sumw2();
  h_phoEB_pt_MET->Sumw2();
  h_phoEB_pt_dphoMETPhi->Sumw2();
  h_phoEB_pt_HLT->Sumw2();

  TH1F *h_phoEB_ptoverMET = new TH1F("h_phoEB_ptoverMET", "phoEB pt/MET", 20, 0, 4);  
  TH1F *h_phoEB_eta = new TH1F("h_phoEB_eta", "matched phoEB eta varbins", 6, etabin);
  TH1F *h_phoEB_eta_M = new TH1F("h_phoEB_eta_M", "matched phoEB eta with M-cut varbins", 6, etabin);
  
  h_phoEB_ptoverMET->Sumw2();
  h_phoEB_eta->Sumw2();
  h_phoEB_eta_M->Sumw2();

  TH1F *h_SeedTime_Nm1 = new TH1F("h_SeedTime_Nm1", "SeedTime N-1 cut", 200, -10, 10);
  TH1F *h_SeedTime_cut = new TH1F("h_SeedTime_cut", "SeedTime N cut", 200, -10, 10);

  h_SeedTime_Nm1->Sumw2();
  h_SeedTime_cut->Sumw2();
  
  TH1F *h_MET_Nm1 = new TH1F("h_MET_Nm1", "pf MET N-1 cut", 60, 0, 1200);
  TH1F *h_MET_cut = new TH1F("h_MET_cut", "pf MET N cut", 60, 0, 1200);

  h_MET_Nm1->Sumw2();
  h_MET_cut->Sumw2();

  TH1F *h_dphoMETPhi_Nm1 = new TH1F("h_dphoMETPhi_Nm1", "deltaPhi of pho and MET N-1 cut", 80, -4, 4);
  TH1F *h_dphoMETPhi_cut = new TH1F("h_dphoMETPhi_cut", "deltaPhi of pho and MET N cut", 80, -4, 4);

  h_dphoMETPhi_Nm1->Sumw2();
  h_dphoMETPhi_cut->Sumw2();

  TH1F *h_phoEB_ptcut = new TH1F("h_phoEB_ptcut", "phoEB pt cut all pas varbin", 20, 200, 1000);
  TH1F *h_dr_phojet = new TH1F("h_dr_phojet", "selected pho jet dr", 100, 0., 1.);
  
  h_phoEB_ptcut->Sumw2();
  h_dr_phojet->Sumw2();

  TH1F *h_njet = new TH1F("h_njet", "njet", 10, 0, 10);
  TH1F *h_njet_Nm1 = new TH1F("h_njet_Nm1", "njet N-1 cut", 10, 0, 10);
  TH1F *h_njet_cut = new TH1F("h_njet_cut", "njet N cut", 10, 0, 10);
  
  TH1F *h_jetpt_Nm1 = new TH1F("h_jetpt_Nm1", "jet pt N-1 veto", 25, 30, 1030);
  TH1F *h_jetpt_cut = new TH1F("h_jetpt_cut", "jet pt N cut", 25, 30, 1030);

  h_jetpt_Nm1->Sumw2();
  h_jetpt_cut->Sumw2();

  TH1F *h_djetMETPhi_Nm1 = new TH1F("h_djetMETPhi_Nm1", "deltaPhi of jet and MET N-1 cut", 80, -4, 4);
  TH1F *h_djetMETPhi_cut = new TH1F("h_djetMETPhi_cut", "deltaPhi of jet and MET N cut", 80, -4, 4);

  h_djetMETPhi_Nm1->Sumw2();
  h_djetMETPhi_cut->Sumw2();

  TH1F *h_phoEB_ptoverjetpt = new TH1F("h_phoEB_ptoverjetpt", "phoEB pt/jet pt", 20, 0, 4);
  h_phoEB_ptoverjetpt->Sumw2();

  TH1F *h_nvtx_Nm1 = new TH1F("h_nvtx_Nm1", "#vtx N-jetveto", 60, 0, 60);
  TH1F *h_nvtx_cut = new TH1F("h_nvtx_cut", "#vtx N cut", 60, 0, 60);

  h_nvtx_Nm1->Sumw2();
  h_nvtx_cut->Sumw2();

  TH2F *h2_rho_phoEB_pt_M = new TH2F("h2_rho_phoEB_pt_M", "h2_rho_phoEB_pt_M", 60, 0, 30, 24, ptbin);
  TH2F *h2_rho_phoEB_pt_chworst = new TH2F("h2_rho_phoEB_pt_chworst", "h2_rho_phoEB_pt_chworst", 60, 0, 30, 24, ptbin);
  TH2F *h2_rho_phoEB_pt_chworst_newEA = new TH2F("h2_rho_phoEB_pt_chworst_newEA", "h2_rho_phoEB_pt_chworst_newEA", 60, 0, 30, 24, ptbin);
  
  h2_rho_phoEB_pt_M->Sumw2();
  h2_rho_phoEB_pt_chworst->Sumw2();
  h2_rho_phoEB_pt_chworst_newEA->Sumw2();

  //tree variables
  Bool_t isData;
  Float_t puwei_ = 1.;
  Int_t    run;
  Long64_t event;
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

  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);
    run     = data.GetInt("run");
    event   = data.GetLong64("event"); 
    isData = data.GetBool("isData");
    nVtx = data.GetInt("nVtx");

    Int_t nPho     = data.GetInt("nPho");
    Int_t nJet     = data.GetInt("nJet");
    h_npho->Fill(nPho);
    h_njet->Fill(nJet);
    
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

    //jet veto selection
    Float_t *jetPt = data.GetPtrFloat("jetPt");
    Float_t *jetEta = data.GetPtrFloat("jetEta");
    Float_t *jetPhi = data.GetPtrFloat("jetPhi");
    Int_t *jetPUFullID = data.GetPtrInt("jetPUFullID");
    
    Int_t njetVeto =0;
    Int_t npfjet =0;
    Int_t jetpass =0;
    Float_t mindjetMETPhi =4.;
    vector<Int_t> jetveto_list; jetveto_list.clear();
    for(Int_t i=0; i<nJet; i++){
      if(fabs(deltaPhi(jetPhi[i], pfMETPhi)) < mindjetMETPhi) mindjetMETPhi = fabs(deltaPhi(jetPhi[i], pfMETPhi));
      if(jetPUFullID[i] > 0) continue;
      jetveto_list.push_back(i);
      npfjet++;
      if(jetPt[i] < 30) continue;
      njetVeto++;
    }
    if(njetVeto > 1) jetpass = 1;

    //phoID selection
    vector<Int_t> cutIDpho_list; cutIDpho_list.clear();//Medium Id
    phoIDcut(1, data, cutIDpho_list);
    vector<Int_t> phoEB_pt, phoEB_pt_cut;
    phoEB_pt.clear();
    phoEB_pt_cut.clear();

    Int_t nfill =0;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      //if(match[ipho] == 0) continue;
      nfill++;
      if(nfill > 1) continue;
      if(phoEleVeto[ipho] == 0) continue;
      //if(npasse == 1 || npassm == 1) continue;

      //barrel region 
      if(fabs(phoSCEta[ipho]) > 1.4442) continue;
      h_phoEB_pt->Fill(phoEt[ipho]);
      h_phoEB_eta->Fill(phoSCEta[ipho]);

      for(Int_t j=0; j<7; j++){
	if(fabs(phoSCEta[ipho]) >= EAbin[j] && fabs(phoSCEta[ipho]) < EAbin[j+1]){
	  h_chIso_rho[j]->Fill(rho, Iso_raw[0][ipho]);
	  h_chworst_rho[j]->Fill(rho, Iso_raw[3][ipho]);
	}
      }

      for(Int_t j=0; j<22; j++){
	if(Iso_rc[0][ipho] < chcut[j]) h_phoEB_pt_chIsocut[j]->Fill(phoEt[ipho]);//chIso
	if(Iso_rc[3][ipho] < chcut[j]) h_phoEB_pt_chworstcut[j]->Fill(phoEt[ipho]);//chWorst
	if(Iso_rc[4][ipho] < chcut[j]) h_phoEB_pt_chworstcut_newEA[j]->Fill(phoEt[ipho]);//chWorst_newEA
      }

      phoEB_pt.push_back(ipho);

      
      if(cutIDpho_list[ipho] == 1){
	//h_phoEB_M->Fill(phoEt[ipho]);
	phoEB_pt_cut.push_back(ipho);
      }

      //pT200
      if(phoEt[ipho] < 200) continue;
      h_phoEB_pt_200->Fill(phoEt[ipho]);

      //phoID cut
      if(cutIDpho_list[ipho] != 1) continue;
      h_phoEB_pt_M->Fill(phoEt[ipho]);
      h2_rho_phoEB_pt_M->Fill(rho, phoEt[ipho]);
      if(Iso_rc[3][ipho] > 1.3) h2_rho_phoEB_pt_chworst->Fill(rho, phoEt[ipho]);

      //chworst_newEA cut
      if(Iso_rc[4][ipho] > 1.3) continue;
      //cout << njetVeto << " " << phoSeedTime[ipho] << endl;
      h_phoEB_pt_chworst->Fill(phoEt[ipho]);
      h2_rho_phoEB_pt_chworst_newEA->Fill(rho, phoEt[ipho]);

      //r9 cut
      if(phoR9Full5x5[ipho] > 1) continue;
      h_phoEB_pt_r9->Fill(phoEt[ipho]);

      //pho pT/MET cut
      if((phoEt[ipho]/pfMET) > 2.2) continue;
      h_phoEB_pt_phoptoverMET->Fill(phoEt[ipho]);

      if(fabs(deltaPhi(phoPhi[ipho], pfMETPhi)) >= 1.2 && (jetpass < 1) && mindjetMETPhi > 0.5 && (phoFiredTrgs[ipho]>>6&1) == 0) h_MET_Nm1->Fill(pfMET);
      //if(fabs(deltaPhi(phoPhi[ipho], pfMETPhi)) >= 1.2 && jetpass < 1) h_MET_Nm1->Fill(pfMET);

      //MET cut
      if(pfMET < 140) continue;
      h_phoEB_pt_MET->Fill(phoEt[ipho]);
      
      if((jetpass < 1) && mindjetMETPhi > 0.5 && (phoFiredTrgs[ipho]>>6&1) == 0) h_dphoMETPhi_Nm1->Fill(deltaPhi(phoPhi[ipho], pfMETPhi));
      //if(jetpass < 1) h_dphoMETPhi_Nm1->Fill(deltaPhi(phoPhi[ipho], pfMETPhi));

      //dphoMETPhi cut
      if(fabs(deltaPhi(phoPhi[ipho], pfMETPhi)) < 1.2) continue;
      h_phoEB_pt_dphoMETPhi->Fill(phoEt[ipho]);

      if(mindjetMETPhi > 0.5 && (phoFiredTrgs[ipho]>>6&1) == 0){
	h_njet_Nm1->Fill(njetVeto);
	h_nvtx_Nm1->Fill(nVtx);
      }

      for(Int_t jj =0; jj<npfjet; jj++){
	Int_t ijet = jetveto_list[jj];
	//Int_t ijet = jj;
	if(mindjetMETPhi > 0.5 && (phoFiredTrgs[ipho]>>6&1) == 0){
	  h_jetpt_Nm1->Fill(jetPt[ijet]);
	  h_dr_phojet->Fill(deltaR(phoEta[ipho], phoPhi[ipho], jetEta[ipho], jetPhi[ipho]));
	}
      }

      //jet veto
      if(jetpass == 1) continue;
      h_phoEB_pt_jetveto->Fill(phoEt[ipho]);
      cout << nJet << " " << njetVeto << endl;
      for(Int_t jj =0; jj<npfjet; jj++){	
	Int_t ijet = jetveto_list[jj];
	//Int_t ijet = jj;
	if((phoFiredTrgs[ipho]>>6&1) == 0)h_djetMETPhi_Nm1->Fill(deltaPhi(jetPhi[ijet], pfMETPhi));
      }

      //djetMETPhi cut
      if(mindjetMETPhi < 0.5) continue;
      h_phoEB_pt_djetMETPhi->Fill(phoEt[ipho]);
      

      //HLT
      if((phoFiredTrgs[ipho]>>6&1) == 0) continue;
      h_phoEB_pt_HLT->Fill(phoEt[ipho]);
      h_phoEB_ptoverMET->Fill(phoEt[ipho]/pfMET);
      h_phoEB_ptcut->Fill(phoEt[ipho]);
      h_MET_cut->Fill(pfMET);
      h_SeedTime_cut->Fill(phoSeedTime[ipho]);
      h_dphoMETPhi_cut->Fill(deltaPhi(phoPhi[ipho], pfMETPhi));
      h_njet_cut->Fill(njetVeto);
      h_nvtx_cut->Fill(nVtx);
      for(Int_t jj =0; jj<npfjet; jj++){	
	Int_t ijet = jetveto_list[jj];
	//Int_t ijet = jj;
	cout<< "min djetMETPhi " << mindjetMETPhi << endl;
	h_jetpt_cut->Fill(jetPt[ijet]);
	h_djetMETPhi_cut->Fill(deltaPhi(jetPhi[ijet], pfMETPhi));
	h_phoEB_ptoverjetpt->Fill(jetPt[ijet]/phoEt[ipho]);
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

    //SeedTime_cut = -999;
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

      //SeedTime_ = phoSeedTime[ipho];
      //SeedEnergy_ = phoSeedEnergy[ipho];
      //MIPEnergy_ = phoMIPTotEnergy[ipho];
      
      
      //for(Int_t jj=0; jj<phoEB_pt_cut.size(); jj++){
      if(phoEB_pt_cut.size() < 1) continue;
      if(ipho == phoEB_pt_cut[0]){
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

	//SeedTime_cut = phoSeedTime[ipho];
	//SeedEnergy_cut = phoSeedEnergy[ipho];
	//MIPEnergy_cut  = phoMIPTotEnergy[ipho];
	      
	pfMET_cut = pfMET;
	pfMETPhi_cut = pfMETPhi;
      }
    }
     outtree_->Fill();
  }
  //****************END LOOP**********************//

  //cout <<nphoEleVeto << " " << nlep << " " << nmatch << " " << float(nphoEleVeto)/float(nmatch) << " " <<float(nlep)/float(nmatch) << endl;
    
  //****************Terminate*********************//
  fout_->cd();
  outtree_->Write();

  hEvents->Write();
  h_npho->Write();
  h_nmcpho->Write();
  h_nmatchpho->Write();
  h_dr_pho->Write();
  h_dpt_pho->Write();
  h_dptdr_pho->Write();

  h_njet_Nm1->Write();
  h_njet_cut->Write();
  
  h_phoEB_pt->Write();
  h_phoEB_pt_200->Write();
  h_phoEB_pt_M->Write();
  h_phoEB_pt_chworst->Write();
  h_phoEB_pt_SeedTime->Write();
  h_phoEB_pt_r9->Write();
  h_phoEB_pt_phoptoverMET->Write();
  //h_phoEB_pt_HoverE->Write();
  h_phoEB_pt_MET->Write();
  h_phoEB_pt_dphoMETPhi->Write();
  h_phoEB_pt_jetveto->Write();
  h_phoEB_pt_djetMETPhi->Write();
  //h_phoEB_pt_MIPEn2->Write();
  h_phoEB_pt_HLT->Write();
  
  h_phoEB_ptoverMET->Write();
  h_phoEB_eta->Write();
  h_phoEB_eta_M->Write();

  h_SeedTime_Nm1->Write();
  h_SeedTime_cut->Write();
  
  h_MET_Nm1->Write();
  h_MET_cut->Write();

  h_dphoMETPhi_Nm1->Write();
  h_dphoMETPhi_cut->Write();

  h_phoEB_ptcut->Write();
  h_dr_phojet->Write();

  h_jetpt_Nm1->Write();
  h_jetpt_cut->Write();
  h_phoEB_ptoverjetpt->Write();

  h_djetMETPhi_Nm1->Write();
  h_djetMETPhi_cut->Write();
  
  h_nvtx_Nm1->Write();
  h_nvtx_cut->Write();
  
  h2_rho_phoEB_pt_M->Write();
  h2_rho_phoEB_pt_chworst->Write();
  h2_rho_phoEB_pt_chworst_newEA->Write();

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
