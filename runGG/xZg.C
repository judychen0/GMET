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

void xZg(char* pathes){

  //***********************Initialization***********************//

  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("ggNtuplizer/hEvents")->Clone();
  hEvents->SetDirectory(0);
  fopen->Close();
  //access EventTree with TreeReader class
  TreeReader data(pathes);
  //create an output .root file
  TFile *fout_;
  fout_ = new TFile("output_ggtree_mc.root","RECREATE");

  //create histograms in output .root file
  TH1F *h_npho = new TH1F("h_npho", "n pho", 10, 0., 10);
  TH1F *h_nmcpho = new TH1F("h_nmcpho", "n mcpho", 10, 0., 10);
  TH1F *h_nmatchpho = new TH1F("h_nmatchpho", "n matchpho", 10, 0., 10);
  TH1F *h_dr_pho = new TH1F("h_dr_pho", "dr of photon", 100, 0., 0.2);
  TH1F *h_dpt_pho = new TH1F("h_dpt_pho", "dpt of photon", 100, 0., 1);
  TH2F *h_dptdr_pho = new TH2F("h_dptdr_pho", "dptdr of photon", 100, 0., 1, 100, 0., 2);

  TH1F *h_nele = new TH1F("h_nele", "n ele", 10, 0., 10); 
  TH1F *h_nmatchele = new TH1F("h_nmatchele", "n matchele", 10, 0., 10);
  TH2F *h_dptdr_ele = new TH2F("h_dptdr_ele", "dptdr of ele", 100, 0., 1, 100, 0., 2);
  TH1F *h_nmu = new TH1F("h_nmu", "n mu", 10, 0., 10);
  TH1F *h_nmatchmu = new TH1F("h_nmatchmu", "n matchmu", 10, 0., 10);
  TH2F *h_dptdr_mu = new TH2F("h_dptdr_mu", "dptdr of mu", 100, 0., 1, 100, 0., 2);
  
  h_dr_pho->Sumw2();
  h_dpt_pho->Sumw2();
  h_dptdr_pho->Sumw2();
  h_dptdr_ele->Sumw2();
  h_dptdr_mu->Sumw2();

  TH1F *h_dr_epho = new TH1F("h_dr_epho", "dr of e&pho", 1000, 0., 1);
  TH1F *h_dr_mpho = new TH1F("h_dr_mpho", "dr of m&pho", 1000, 0., 1);
  TH1F *h_nclose_e = new TH1F("h_nclose_e", "close e2pho", 10, 0., 10);
  TH1F *h_nclose_m = new TH1F("h_nclose_m", "close m2pho", 10, 0., 10);

  h_dr_epho->Sumw2();
  h_dr_mpho->Sumw2();

  Float_t EAbin[10] = {0.0, 1.0, 1.479, 2.0, 2.2, 2.3, 2.4, 3};//8 bins
  TH2F *h_chIso_rho[7];
  TH2F *h_chworst_rho[7];
  for(Int_t i=0; i<7; i++){
    h_chIso_rho[i] = new TH2F(Form("h_chIso_rho_eta%i", i), Form("h_chIso_rho_eta%i", i), 60, 0, 30, 30, 0, 3);
    h_chworst_rho[i] = new TH2F(Form("h_chworst_rho_eta%i",i), Form("h_chworst_rho_eta%i", i), 60, 0, 30, 30, 0, 3);
  }

  Double_t ptbin[30] = {22, 30, 36, 50, 75, 90, 120, 170, 175, 180, 185, 190, 200,
		     210, 220, 230, 250, 300, 350, 400, 500, 750, 1000, 1500, 2000, 3000, 10000};//24 bins, 2016
  Double_t etabin[10] = {-1.566, -1.4442, -0.8, 0, 0.8, 1.4442, 1.566};//6bins

  TH1F *h_phoEB_pt = new TH1F("h_phoEB_pt", "matched phoEB pt", 24, ptbin);
  TH1F *h_phoEB_M = new TH1F("h_phoEB_M", "phoEB M IDcut", 24, ptbin);
  TH1F *h_phoEB_pt_HLT = new TH1F("h_phoEB_pt_HLT", "matched phoEB pt HLT trgs", 24, ptbin);
  
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

  TH1F *h_phoEB_pt_200 = new TH1F("h_phoEB_pt_200", "matched phoEB pt pt200 cut", 24, ptbin);
  TH1F *h_phoEB_pt_M = new TH1F("h_phoEB_pt_M", "matched phoEB pt M IDcut", 24, ptbin);
  //TH1F *h_phoEB_pt_chworst = new TH1F("h_phoEB_pt_chworst", "matched phoEB pt chworst cut", 24, ptbin);
  //TH1F *h_phoEB_pt_HoverE = new TH1F("h_phoEB_pt_HoverE", "matched phoEB pt HoverEc cut", 24, ptbin);
  TH1F *h_phoEB_pt_r9 = new TH1F("h_phoEB_pt_r9", "matched phoEB pt r9 cut", 24, ptbin);
  //TH1F *h_phoEB_pt_SeedTime = new TH1F("h_phoEB_pt_SeedTime", "matched phoEB pt SeedTime cut", 24, ptbin);
  TH1F *h_phoEB_pt_MET = new TH1F("h_phoEB_pt_MET", "matched phoEB pt MET cut", 24, ptbin);
  TH1F *h_phoEB_pt_SeedTime = new TH1F("h_phoEB_pt_SeedTime", "matched phoEB pt SeedTime cut", 24, ptbin);
  TH1F *h_phoEB_pt_dphoMETPhi = new TH1F("h_phoEB_pt_dphoMETPhi", "matched phoEB pt dphoMETPhi cut", 24, ptbin);
  TH1F *h_phoEB_pt_MIPEn2 = new TH1F("h_phoEB_pt_MIPEn2", "matched phoEB pt MIP energy cut", 24, ptbin);

  TH1F *h_phoEB_ptoverMET = new TH1F("h_phoEB_ptoverMET", "phoEB pt/MET", 20, 0, 4);
  TH1F *h_phoEB_eta = new TH1F("h_phoEB_eta", "matched phoEB eta varbins", 6, etabin);
  TH1F *h_phoEB_eta_M = new TH1F("h_phoEB_eta_M", "matched phoEB eta with M-cut varbins", 6, etabin);

  h_phoEB_pt->Sumw2();
  h_phoEB_M->Sumw2();
  h_phoEB_pt_200->Sumw2();
  h_phoEB_pt_M->Sumw2();
  //h_phoEB_pt_chworst->Sumw2();
  //h_phoEB_pt_HoverE->Sumw2();
  h_phoEB_pt_r9->Sumw2();
  h_phoEB_pt_SeedTime->Sumw2();
  //h_phoEB_pt_MIPEn2->Sumw2();
  h_phoEB_eta->Sumw2();
  h_phoEB_eta_M->Sumw2();
  h_phoEB_pt_dphoMETPhi->Sumw2();
  h_phoEB_pt_MET->Sumw2();
  
  //TH1F *h_nphotrgs = new TH1F("h_nphotrgs", "n HLT pho", 10, 0, 10);
  TH1F *h_MIPEn2 = new TH1F("h_MIPEn2","squared MIPEn", 100, 0, 10);
  TH1F *h_MET_200 = new TH1F("h_MET_200", "pf MET cut pt200", 60, 0, 1200);
  TH1F *h_MET_Nm1 = new TH1F("h_MET_Nm1", "pf MET N-1 cut", 60, 0, 1200);
  TH1F *h_MET_cut = new TH1F("h_MET_cut", "pf MET N cut", 60, 0, 1200);
  //TH1F *h_METPhi = new TH1F("h_METPhi", "pf MET Phi", 8, -4, 4);
  TH1F *h_dphoMETPhi_200 = new TH1F("h_dphoMETPhi_200", "deltaPhi of matched pho and MET", 80, -4, 4);
  TH1F *h_dphoMETPhi_Nm1 = new TH1F("h_dphoMETPhi_Nm1", "deltaPhi of pho and MET N-1 cut", 80, -4, 4);
  TH1F *h_dphoMETPhi_cut = new TH1F("h_dphoMETPhi_cut", "deltaPhi of pho and MET N cut", 80, -4, 4);

  TH1F *h_phoEB_pt200 = new TH1F("h_phoEB_pt200", "phoEB pt pt200 cut pas varbin", 20, 200, 1000);
  TH1F *h_phoEB_ptMid = new TH1F("h_phoEB_ptMid", "phoEB pt M ID cut pas varbin", 20, 200, 1000);
  TH1F *h_phoEB_ptcut = new TH1F("h_phoEB_ptcut", "phoEB pt cut all pas varbin", 20, 200, 1000);

  h_MIPEn2->Sumw2();
  h_MET_200->Sumw2();
  h_MET_Nm1->Sumw2();
  h_MET_cut->Sumw2();
  h_dphoMETPhi_200->Sumw2();
  h_dphoMETPhi_Nm1->Sumw2();
  h_dphoMETPhi_cut->Sumw2();
  h_phoEB_pt200->Sumw2();
  h_phoEB_ptMid->Sumw2();
  h_phoEB_ptcut->Sumw2();
  
  //define branch variables
  Bool_t   isData, isPVGood;
  Float_t puwei_ = 1.;
  Int_t    run;
  Long64_t event;
  Int_t nVtx, nPU;
  Float_t npho_, nmcpho_, matchpho_;
  Int_t isMatched;
  Long64_t HLT, HLTIsPrescaled;
  //Float_t MIPE;
  Float_t  pfMET =0;
  Float_t  pfMETPhi =0;
  Float_t  pfMET_cut =0;
  Float_t  pfMETPhi_cut =0;
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
  outtree_->Branch("HLT",         &HLT,        "HLT/L");
  outtree_->Branch("HLTIsPrescaled", &HLTIsPrescaled,        "HLTIsPrescaled/L");
  outtree_->Branch("puwei",       &puwei_, "puwei/F");
  
  outtree_->Branch("mcphoEt",     &mcphoEt, "mcphoEt/F");
  outtree_->Branch("mcphoEta",    &mcphoEta, "mcphoEta/F");
  outtree_->Branch("mcphoPhi",    &mcphoPhi, "mcphoPhi/F");
  outtree_->Branch("realphoEt",   &realphoEt, "realphoEt/F");
  outtree_->Branch("realphoEta",  &realphoEta, "realphoEta/F");
  outtree_->Branch("realphoPhi",  &realphoPhi, "realphoPhi/F");
  
  outtree_->Branch("eleVeto",     &eleVeto,      "eleVeto/I");
  outtree_->Branch("hasSeed",     &hasSeed,      "hasSeed/I");
  
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
  puCalc.Init("external/puweights/102X/autum18/PU_histo_13TeV_2018_GoldenJSON_69200nb.root");
  Int_t nphohasPixelSeed=0;
  Int_t nmatch=0;
  Int_t nlep=0;
  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    data.GetEntry(ev);
    run     = data.GetInt("run");
    event   = data.GetLong64("event"); 
    isData = data.GetBool("isData");
    isPVGood = data.GetBool("isPVGood");
    nVtx = data.GetInt("nVtx");

    Int_t nPho     = data.GetInt("nPho");
    Int_t nJet     = data.GetInt("nJet");
    h_npho->Fill(nPho);
    
    Int_t    nPUInfo =0;
    Int_t*   puBX=0; 
    Float_t* puTrue=0;
    nPUInfo = data.GetInt("nPUInfo");
    puBX    = data.GetPtrInt("puBX");
    puTrue  = data.GetPtrFloat("puTrue");
    for (Int_t i=0; i<nPUInfo; ++i) {
      if (puBX[i] == 0) nPU = puTrue[i];
    }      
    puwei_ = 1.;
    puwei_ = (Float_t)puCalc.GetWeight(run, puTrue[1]);
    
    
    //reco
    Float_t* phoE = 0; 
    Float_t* phoEt = 0;
    Float_t* phoEta = 0;
    Float_t* phoSCEta = 0;
    Float_t* phoPhi = 0;
    Float_t* phoR9  = 0;
    Float_t* phoHoverE = 0; 
    Float_t* phoPFChIso          = 0; 
    Float_t* phoPFNeuIso         = 0; 
    Float_t* phoPFPhoIso         = 0;
    Int_t*   phohasPixelSeed     = 0;
    Int_t*   phoEleVeto          = 0; 
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
    phoR9  = data.GetPtrFloat("phoR9");
    phoHoverE = data.GetPtrFloat("phoHoverE");
    phoPFChIso          = data.GetPtrFloat("phoPFChIso");
    phoPFNeuIso         = data.GetPtrFloat("phoPFNeuIso");
    phoPFPhoIso         = data.GetPtrFloat("phoPFPhoIso");
    phohasPixelSeed     = data.GetPtrInt("phohasPixelSeed");
    phoEleVeto          = data.GetPtrInt("phoEleVeto");
    phoSigmaIEtaIEtaFull5x5  = data.GetPtrFloat("phoSigmaIEtaIEtaFull5x5");
    phoSigmaIEtaIPhiFull5x5  = data.GetPtrFloat("phoSigmaIEtaIPhiFull5x5");
    phoSigmaIPhiIPhiFull5x5  = data.GetPtrFloat("phoSigmaIPhiIPhiFull5x5");
    phoR9Full5x5           = data.GetPtrFloat("phoR9Full5x5");
    phoPFChWorst             = data.GetPtrFloat("phoPFChWorstIso");
    rho                    = data.GetFloat("rho");
    phoID         = data.GetPtrShort("phoIDbit");
    phoFiredTrgs = data.GetPtrLong64("phoFiredSingleTrgs");

    //phoSeedTime     = data.GetPtrFloat("phoSeedTime");
    //phoSeedEnergy   = data.GetPtrFloat("phoSeedEnergy");
    //phoMIPTotEnergy = data.GetPtrFloat("phoMIPTotEnergy");
      
    pfMET     = data.GetFloat("pfMET");
    pfMETPhi  = data.GetFloat("pfMETPhi");

    
    //rho correction
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

      //get mc photon id
    vector<Int_t> mc_phoid; mc_phoid.clear();
    Int_t nMCpho =0;
    for(Int_t k=0; k < nMC; k++){
      if(fabs(mcMomPID[k]) > 6) continue;
      //if(fabs(mcMomPID[k]) > 6 && fabs(mcMomPID[k]) < 22) continue;
      if((mcStatus[k]>>1&1) == 0) continue;//isPromptFinalState passed 
      if(mcPID[k] == 22){
	mc_phoid.push_back(k);
	//cout << "isPrompt" << (mcStatus[k]>>1&1) << endl;;
	nMCpho++;
      }
    }
    h_nmcpho->Fill(nMCpho);

    //gen-matching
    vector <Int_t> matchpho_list; matchpho_list.clear();
    vector <Int_t> match; match.clear();
    vector <Int_t> match_status; match_status.clear();
    Int_t nmatchpho=0;
    Int_t isMatched;
    for(Int_t ipho=0; ipho < nPho; ipho++){
      //if(phoEt[ipho] < 165.) continue;
      isMatched = 0;
      for(Int_t nn=0; nn < nMCpho; nn++){
	Int_t k = mc_phoid[nn];
	Float_t dr = deltaR(phoEta[ipho], phoPhi[ipho], mcEta[k], mcPhi[k]);
	Float_t dpt = fabs((phoEt[ipho] - mcPt[k])/mcPt[k]);
	h_dptdr_pho->Fill(dr, dpt);
	if(dr < 0.1) h_dpt_pho->Fill(dpt);
	if(dpt < 0.2) h_dr_pho->Fill(dr);
	if(dr < 0.1 && dpt < 0.2){
	  isMatched = 1;
	  matchpho_list.push_back(ipho);
	  
	  //match_status.push_back(st);
	  nmatchpho++;
	  break;
	}
      }
      if(nmatchpho > 1) break;
      match.push_back(isMatched);
    }
    h_nmatchpho->Fill(nmatchpho);

    Int_t nEle = data.GetInt("nEle");
    h_nele->Fill(nEle);
    Float_t *elePt = data.GetPtrFloat("elePt");
    Float_t *eleEta = data.GetPtrFloat("eleEta");
    Float_t *elePhi = data.GetPtrFloat("elePhi");
    
    vector <Int_t> mc_eleid; mc_eleid.clear();
    Int_t nMCele =0;
    for(Int_t k=0; k<nMC; k++){
      //if((mcStatus[k]>>2&1) == 0) continue;
      //if(fabs(mcMomPID[k]) > 24 || fabs(mcMomPID[k]) < 23) continue;
      if(fabs(mcPID[k]) == 11 && (fabs(mcMomPID[k]) == 24 || fabs(mcMomPID[k]) == 23)){
	mc_eleid.push_back(k);
	nMCele++;
      }
    }
      
    vector <Int_t> matchele_list; matchele_list.clear();
    vector <Int_t> matcheleMC_list; matcheleMC_list.clear();
    vector <Int_t> match_ele;
    Int_t nLep_e=0;
    Int_t deZee =0;
    Int_t deWen =0;
    for(Int_t iele=0; iele < nEle; iele++){
      isMatched = 0;
      if(nMCele ==0 ) break;
      for(Int_t nn=0; nn < nMCele; nn++){
	Int_t k = mc_eleid[nn];
	Float_t dr = deltaR(eleEta[iele], elePhi[iele], mcEta[k], mcPhi[k]);
	Float_t dpt = fabs((elePt[iele] - mcPt[k])/mcPt[k]);
	h_dptdr_ele->Fill(dr, dpt);
	if(dr < 0.3){
	  isMatched = 1;
	  //printf("MC phomatched !");
	  matchele_list.push_back(iele);
	  
	  if(mcMomPID[k] ==23) deZee+=1;
	  else if(fabs(mcMomPID[k]) ==24) deWen+=1;
	  else{
	    deZee-=1;
	    deWen-=1;
	  }
	  
	  matcheleMC_list.push_back(k);
	  nLep_e++;
	  break;
	}
      }
    }
    h_nmatchele->Fill(nLep_e);

    Int_t nMu = data.GetInt("nMu");
    h_nmu->Fill(nMu);
    Float_t *muPt = data.GetPtrFloat("muPt");
    Float_t *muEta = data.GetPtrFloat("muEta");
    Float_t *muPhi = data.GetPtrFloat("muPhi");

    vector <Int_t> mc_muid; mc_muid.clear();
    Int_t nMCmu =0;
    for(Int_t k=0; k<nMC; k++){
      //if((mcStatus[k]>>2&1) == 0) continue;
      //if(fabs(mcMomPID[k]) > 24 || fabs(mcMomPID[k]) < 23) continue;
      if(fabs(mcPID[k]) == 13 && (fabs(mcMomPID[k]) == 24 || fabs(mcMomPID[k]) == 23)){
	mc_muid.push_back(k);
	nMCmu++;
      }
    }
    //cout << nMCmu << " " << nMCele << endl;
    vector <Int_t> matchmu_list; matchmu_list.clear();
    vector <Int_t> matchmuMC_list; matchmuMC_list.clear();
    vector <Int_t> match_mu;
    Int_t nLep_m =0;
    Int_t deZmm =0;
    Int_t deWmn =0;
    for(Int_t imu=0; imu < nMu; imu++){
      isMatched =0;
      if(nMCmu ==0) break;
      for(Int_t nn=0; nn < nMCmu; nn++){
	Int_t k = mc_muid[nn];
	Float_t dr = deltaR(muEta[imu], muPhi[imu], mcEta[k], mcPhi[k]);
	Float_t dpt = fabs((muPt[imu] - mcPt[k])/mcPt[k]);
	h_dptdr_mu->Fill(dr, dpt);
	if(dr < 0.3){
	  isMatched = 1;
	  //printf("MC phomatched !");
	  matchmu_list.push_back(imu);
	  
	  if(mcMomPID[k] ==23) deZmm+=1;
	  else if(fabs(mcMomPID[k]) ==24) deWmn+=1;
	  else{
	    deZmm-=1;
	    deWmn-=1;
	  }
	  
	  matchmuMC_list.push_back(k);
	  nLep_m++;
	  break;
	}
      }
    }
    h_nmatchmu->Fill(nLep_m);
  
    vector <Int_t> Zee_list; Zee_list.clear();
    vector <Int_t> Zmm_list; Zmm_list.clear();
    vector <Int_t> Wen_list; Wen_list.clear();
    vector <Int_t> Wmn_list; Wmn_list.clear();
    Int_t nclose_e =0;

    Int_t elist = matchele_list.size();
    //cout << "elist " << elist << endl; 
    for(Int_t ii=0; ii<elist; ii++){
      //if(deZee < 2 && deWen !=1) break;
      
      Int_t iele = matchele_list[ii];
      if(matchpho_list.size() == 0) break;
      Int_t ipho = matchpho_list[0];
      Float_t dr_epho = deltaR(eleEta[iele], elePhi[iele], phoEta[ipho], phoPhi[ipho]);
      //cout << "dr_epho" << dr_epho << endl;
      h_dr_epho->Fill(dr_epho);
      if(dr_epho < 0.4 && (deZee == 2 || deWen == 1)) nclose_e++;
    }
    /*
    if(deZee == 2 && nLep_e >=2){
      
      
    }
    if(deWen == 1 && nLep_e == 1){
      for(Int_t ii=0; ii<matchele_list.size(); ii++){
	Int_t iele = matchele_list[ii];
	Int_t ipho = matchpho_list[0];
	Float_t dr_epho = deltaR(eleEta[iele], elePhi[iele], phoEta[ipho], phoPhi[ipho]);
	h_dr_epho->Fill(dr_epho);
	if(dr_epho < 0.3) nclose++;
      }
    }
    */
    h_nclose_e->Fill(nclose_e);

    Int_t nclose_m = 0;
    Int_t mlist = matchmu_list.size();
    
    for(Int_t ii=0; ii<mlist; ii++){
      //if(deZmm < 2 && deWmn != 1) break;
      if(mlist == 0) break;
      //cout << "mlist " << mlist << endl; 
      
      Int_t imu = matchmu_list[ii];
      if(matchpho_list.size() == 0) break;
      Int_t ipho = matchpho_list[0];
      Float_t dr_mpho = deltaR(muEta[imu], muPhi[imu], phoEta[ipho], phoPhi[ipho]);
      //cout << "dr_mpho" << dr_mpho << endl;
      h_dr_mpho->Fill(dr_mpho);
      if(dr_mpho < 0.4 && (deZmm == 2 || deWmn ==1)) nclose_m++;
    }
    /*
    if(deZmm == 2 && nLep_m >=2){
      
      
    }
    if(deWmn == 1 && nLep_m == 1){
      
      for(Int_t ii=0; ii<matchmu_list.size(); ii++){
	Int_t imu = matchmu_list[ii];
	Int_t ipho = matchpho_list[0];
	Float_t dr_mpho = deltaR(muEta[imu], muPhi[imu], phoEta[ipho], phoPhi[ipho]);
	h_dr_mpho->Fill(dr_mpho);
	if(dr_mpho < 0.3) nclose++;
      }
    }
    */
    h_nclose_m->Fill(nclose_m);

    
    Float_t *eleSCEta = data.GetPtrFloat("eleSCEta");
    Int_t *eleConvVeto = data.GetPtrInt("eleConvVeto");
    Long64_t *eleFired2Trgs = data.GetPtrLong64("eleFiredDoubleTrgs");

    vector<Int_t> cutIDele_list; cutIDele_list.clear();//Loose Id
    eleIDcut(1, data, cutIDele_list);
    vector<Int_t> ele_pass;
    ele_pass.clear();
    Int_t npasse = 0;
    Int_t elepass = 0;
    for(Int_t iele=0; iele < nEle; iele++){
      //if(fabs(eleSCEta[iele]) > 2.5) continue;
      //if(fabs(eleEta[iele]) > 1.4442 && fabs(eleEta[iele]) < 1.566) continue;
      //if(eleConvVeto == 0) continue;
      //ele_pt.push_back(iele);
      if(cutIDele_list[iele] != 1) continue;
      if(elePt[iele] < 10) continue;
      npasse++;
    }
    if(npasse > 0) elepass = 1;
    
    vector<Int_t> cutIDmu_list; cutIDmu_list.clear();//Loose Id
    muIDcut(0, data, cutIDmu_list);

    Int_t npassm = 0;
    Int_t mupass = 0;
    for(Int_t imu=0; imu < nMu; imu++){
      if(cutIDmu_list[imu] != 1) continue;
      if(muPt[imu] < 10) continue;
      npassm++;
    }
    if(npassm > 0) mupass = 1;

    vector<Int_t> cutIDpho_list; cutIDpho_list.clear();//Medium Id
    phoIDcut(1, data, cutIDpho_list);
    vector<Int_t> phoEB_pt, phoEB_pt_cut;
    phoEB_pt.clear();
    phoEB_pt_cut.clear();
    //cout<< "Im here!" << endl; 
    Int_t nfill =0;
  
    //if(nmatchpho == 0 || nmatchpho > 1) continue;//match to single pho
    for(Int_t ipho=0; ipho<nPho; ipho++){
      //if(isPVGood==0) continue;
      
      if(match[ipho] == 0) continue;
      nmatch++;
      nfill++;
      if(nfill > 1) continue;
      //if(npasse == 1 || npassm == 1) continue;
      nlep++;
      //cout << match[ipho] << endl;
      //if(phohasPixelSeed[ipho] == 1) continue;
      nphohasPixelSeed++;
      
      if(fabs(phoSCEta[ipho]) > 1.4442) continue;
      h_phoEB_pt->Fill(phoEt[ipho], puwei_);
      h_phoEB_eta->Fill(phoSCEta[ipho], puwei_);
      
      for(Int_t j=0; j<7; j++){
	if(fabs(phoSCEta[ipho]) >= EAbin[j] && fabs(phoSCEta[ipho]) < EAbin[j+1]){
	  h_chIso_rho[j]->Fill(rho, Iso_raw[0][ipho], puwei_);
	  h_chworst_rho[j]->Fill(rho, Iso_raw[3][ipho], puwei_);
	}
      }

      for(Int_t j=0; j<22; j++){
	if(Iso_rc[0][ipho] < chcut[j]) h_phoEB_pt_chIsocut[j]->Fill(phoEt[ipho], puwei_);//chIso
	if(Iso_rc[3][ipho] < chcut[j]) h_phoEB_pt_chworstcut[j]->Fill(phoEt[ipho], puwei_);//chWorst
	if(Iso_rc[4][ipho] < chcut[j]) h_phoEB_pt_chworstcut_newEA[j]->Fill(phoEt[ipho], puwei_);//chWorst_newEA
      }

      phoEB_pt.push_back(ipho);
      if(cutIDpho_list[ipho] == 1){
	h_phoEB_M->Fill(phoEt[ipho], puwei_);
	phoEB_pt_cut.push_back(ipho);
      }

      if(phoEt[ipho] < 200) continue;
      h_phoEB_pt_200->Fill(phoEt[ipho], puwei_);
      h_phoEB_pt200->Fill(phoEt[ipho], puwei_);
      h_dphoMETPhi_200->Fill(deltaPhi(phoPhi[ipho], pfMETPhi), puwei_);
      h_MET_200->Fill(pfMET);

      if(cutIDpho_list[ipho] != 1) continue;
      h_phoEB_pt_M->Fill(phoEt[ipho], puwei_);
      h_phoEB_ptMid->Fill(phoEt[ipho], puwei_);
      h_phoEB_eta_M->Fill(phoSCEta[ipho], puwei_);
      if(fabs(deltaPhi(phoPhi[ipho], pfMETPhi)) >= 1.2) h_MET_Nm1->Fill(pfMET, puwei_);

      if(pfMET < 80) continue;
      h_phoEB_pt_MET->Fill(phoEt[ipho], puwei_);
      h_phoEB_ptoverMET->Fill(phoEt[ipho]/pfMET);
      h_dphoMETPhi_Nm1->Fill(deltaPhi(phoPhi[ipho], pfMETPhi), puwei_);
      h_MET_cut->Fill(pfMET);

      if(fabs(deltaPhi(phoPhi[ipho], pfMETPhi)) < 1.2) continue;
      h_phoEB_pt_dphoMETPhi->Fill(phoEt[ipho], puwei_);
      h_phoEB_ptcut->Fill(phoEt[ipho], puwei_);
      h_dphoMETPhi_cut->Fill(deltaPhi(phoPhi[ipho], pfMETPhi), puwei_);
      

      if((phoFiredTrgs[ipho]>>6&1) == 0) continue;
      h_phoEB_pt_HLT->Fill(phoEt[ipho], puwei_);
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
    
    //for(Int_t ipho=0; ipho<nPho; ipho++){
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
	mcphoEt = mcPt[ipho];
	mcphoEta = mcEta[ipho];
	mcphoPhi = mcPhi[ipho];
	realphoEt = phoEt[ipho];
	realphoEta = phoEta[ipho];
	realphoPhi = phoPhi[ipho];

	//hasSeed = phohasPixelSeed[ipho];
      
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
    
    //h_nphotrgs->Fill(nphotrgs);
    outtree_->Fill();
  }//event loop
  //****************END LOOP**********************//

  cout <<nphohasPixelSeed << " " << nlep << " " << nmatch << " " << float(nphohasPixelSeed)/float(nmatch) << " " <<float(nlep)/float(nmatch) << endl;
    
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

  h_nele->Write();
  h_nmatchele->Write();
  h_nclose_e->Write();
  h_dptdr_ele->Write();
  h_dr_epho->Write();

  h_nmu->Write();
  h_nmatchmu->Write();
  h_nclose_m->Write();
  h_dptdr_mu->Write();
  h_dr_mpho->Write();
  
  h_phoEB_pt->Write();
  h_phoEB_M->Write();
  h_phoEB_pt_HLT->Write();
  h_phoEB_pt_200->Write();
  h_phoEB_pt_M->Write();
  //h_phoEB_pt_chworst->Write();
  //h_phoEB_pt_HoverE->Write();
  //h_phoEB_pt_r9->Write();
  h_phoEB_pt_dphoMETPhi->Write();
  h_phoEB_pt_dphoMETPhi->Write();
  h_phoEB_pt_MET->Write();
  //h_phoEB_pt_SeedTime->Write();
  //h_phoEB_pt_MIPEn2->Write();
  h_phoEB_ptoverMET->Write();
  
  h_phoEB_eta->Write();
  h_phoEB_eta_M->Write();
  
  h_MET_200->Write();
  h_MET_Nm1->Write();
  h_MET_cut->Write();
  //h_METPhi->Write();
  h_dphoMETPhi_200->Write();
  h_dphoMETPhi_Nm1->Write();
  h_dphoMETPhi_cut->Write();

  h_phoEB_pt200->Write();
  h_phoEB_ptMid->Write();
  h_phoEB_ptcut->Write();
  //h_nphotrgs->Write();
  //h_MIPEn2->Write();
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
