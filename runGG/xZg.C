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

  //access EventTree with TreeReader class
  TreeReader data(pathes);
  //create an output .root file
  TFile *fout_;
  fout_ = new TFile("output_ggtree_mc.root","RECREATE");

  //create histograms in output .root file
  TH1F *h_npho = new TH1F("h_npho", "n pho", 10, 0., 10);
  TH1F *h_nmcpho = new TH1F("h_nmcpho", "n mcpho", 10, 0., 10);
  TH1F *h_matchpho = new TH1F("h_matchpho", "n matchpho", 10, 0., 10);
  TH1F *h_dr_pho = new TH1F("h_dr_pho", "dr of photon", 100, 0., 0.2);
  TH1F *h_dpt_pho = new TH1F("h_dpt_pho", "dpt of photon", 100, 0., 1);
  TH2F *h_dptdr_pho = new TH2F("h_dptdr_pho", "dptdr of photon", 100, 0., 1, 100, 0., 2);

  h_dr_pho->Sumw2();
  h_dpt_pho->Sumw2();
  h_dptdr_pho->Sumw2();

  Double_t ptcut[30] = {22, 30, 36, 50, 75, 90, 120, 170, 175, 180, 185, 190, 200,
		     210, 220, 230, 250, 300, 350, 400, 500, 750, 1000, 1500, 2000, 3000, 10000};//24 bins, 2016
  Double_t etabin[10] = {-1.566, -1.4442, -0.8, 0, 0.8, 1.4442, 1.566};//6bins

  TH1F *h_phoEB_pt = new TH1F("h_phoEB_pt", "matched phoEB pt", 24, ptcut);
  TH1F *h_phoEB_pt_HLT = new TH1F("h_phoEB_pt_HLT", "matched phoEB pt HLT trgs", 24, ptcut);
  TH1F *h_phoEB_pt_200 = new TH1F("h_phoEB_pt_200", "matched phoEB pt pt200 cut", 24, ptcut);
  TH1F *h_phoEB_pt_M = new TH1F("h_phoEB_pt_M", "matched phoEB pt M IDcut", 24, ptcut);
  TH1F *h_phoEB_pt_chworst = new TH1F("h_phoEB_pt_chworst", "matched phoEB pt chworst cut", 24, ptcut);
  TH1F *h_phoEB_pt_HoverE = new TH1F("h_phoEB_pt_HoverE", "matched phoEB pt HoverEc cut", 24, ptcut);
  TH1F *h_phoEB_pt_r9 = new TH1F("h_phoEB_pt_r9", "matched phoEB pt r9 cut", 24, ptcut);
  TH1F *h_phoEB_pt_SeedTime = new TH1F("h_phoEB_pt_SeedTime", "matched phoEB pt SeedTime cut", 24, ptcut);
  TH1F *h_phoEB_pt_MET = new TH1F("h_phoEB_pt_MET", "matched phoEB pt MET cut", 24, ptcut);
  TH1F *h_phoEB_pt_dphoMETPhi = new TH1F("h_phoEB_pt_dphoMETPhi", "matched phoEB pt dphoMETPhi cut", 24, ptcut);
  //TH1F *h_phoEB_pt_MIPEn2 = new TH1F("h_phoEB_pt_MIPEn2", "matched phoEB pt MIP energy cut", 24, ptcut);

  TH1F *h_phoEB_ptoverMET = new TH1F("h_phoEB_ptoverMET", "phoEB pt/MET", 20, 0, 4);
  TH1F *h_phoEB_eta = new TH1F("h_phoEB_eta", "matched phoEB eta varbins", 6, etabin);
  TH1F *h_phoEB_eta_M = new TH1F("h_phoEB_eta_M", "matched phoEB eta with M-cut varbins", 6, etabin);

  h_phoEB_pt->Sumw2();
  h_phoEB_pt_200->Sumw2();
  h_phoEB_pt_M->Sumw2();
  h_phoEB_pt_chworst->Sumw2();
  h_phoEB_pt_HoverE->Sumw2();
  h_phoEB_pt_r9->Sumw2();
  h_phoEB_pt_SeedTime->Sumw2();
  //h_phoEB_pt_MIPEn2->Sumw2();
  h_phoEB_eta->Sumw2();
  h_phoEB_eta_M->Sumw2();
  h_phoEB_pt_dphoMETPhi->Sumw2();
  h_phoEB_pt_MET->Sumw2();
  
  TH1F *h_nphotrgs = new TH1F("h_nphotrgs", "n HLT pho", 10, 0, 10);
  //TH1F *h_MIPEn2 = new TH1F("h_MIPEn2","squared MIPEn", 100, 0, 10);
  TH1F *h_MET = new TH1F("h_MET", "pf MET cut dphoMETPhi", 40, 0, 800);
  //TH1F *h_METPhi = new TH1F("h_METPhi", "pf MET Phi", 8, -4, 4);
  TH1F *h_dphoMETPhi = new TH1F("h_dphoMETPhi", "deltaPhi of matched pho and MET", 80, -4, 4);

  //define branch variables
  Bool_t   isData, isPVGood;
  Int_t    run;
  Long64_t event;
  Int_t nVtx;
  Float_t npho_, nmcpho_, matchpho_;
  Long64_t HLT, HLTIsPrescaled;
  //Float_t MIPE;
  Float_t  pfMET =0;
  Float_t  pfMETPhi =0;
  Float_t genMET =0;
  Float_t genMETPhi =0;

  Float_t SeedTime_, SeedEnergy_, MIPTotEnergy_;
  Float_t mcphoEt, mcphoEta, mcphoPhi, realphoEt, realphoEta, realphoPhi;
  Float_t HoverE, sieie, chIso, phoIso, nhIso, chWorst, eleVeto, rho;
  Float_t chIso_, phoIso_, nhIso_, chWorst_;
  Float_t sieieFull5x5, sieipFull5x5, sipipFull5x5, r9Full5x5;
  Float_t HoverE_sig, chIso_sig, phoIso_sig, nhIso_sig, chWorst_sig;
  Float_t sieieFull5x5_sig, sieipFull5x5_sig, sipipFull5x5_sig, r9Full5x5_sig;
  
  TTree *outtree_;
  outtree_ = new TTree("t", "mini tree");
  
  outtree_->Branch("run", &run, "run/I");
  outtree_->Branch("event", &event, "event/L");
  outtree_->Branch("isData", &isData, "isData/O");
  outtree_->Branch("HLT",         &HLT,        "HLT/L");
  outtree_->Branch("HLTIsPrescaled", &HLTIsPrescaled,        "HLTIsPrescaled/L");
  
  outtree_->Branch("mcphoEt",     &mcphoEt, "mcphoEt/F");
  outtree_->Branch("mcphoEta",    &mcphoEta, "mcphoEta/F");
  outtree_->Branch("mcphoPhi",    &mcphoPhi, "mcphoPhi/F");
  outtree_->Branch("realphoEt",   &realphoEt, "realphoEt/F");
  outtree_->Branch("realphoEta",  &realphoEta, "realphoEta/F");
  outtree_->Branch("realphoPhi",  &realphoPhi, "realphoPhi/F");
  
  outtree_->Branch("eleVeto",     &eleVeto,      "eleVeto/I");
  outtree_->Branch("HoverE",      &HoverE,       "HoverE/F");
  outtree_->Branch("chIso",       &chIso,        "chIso/F");
  outtree_->Branch("phoIso",      &phoIso,       "phoIso/F");
  outtree_->Branch("nhIso",       &nhIso,        "nhIso/F");
  outtree_->Branch("chWorst",     &chWorst,      "chWorst/F");
  outtree_->Branch("chIso_rc",    &chIso_,       "chIso_rc/F");
  outtree_->Branch("phoIso_rc",   &phoIso_,      "phoIso_rc/F");
  outtree_->Branch("nhIso_rc",    &nhIso_,       "nhIso_rc/F");
  outtree_->Branch("chWorst_rc",  &chWorst_,     "chWorst_rc/F");
  
  outtree_->Branch("sieieFull5x5",        &sieieFull5x5,        "sieieFull5x5/F");
  outtree_->Branch("sieipFull5x5",        &sieipFull5x5,        "sieipFull5x5/F");
  outtree_->Branch("sipipFull5x5",        &sipipFull5x5,        "sipipFull5x5/F");
  outtree_->Branch("r9Full5x5",           &r9Full5x5,           "r9Full5x5/F");

  outtree_->Branch("HoverE_sig",       &HoverE_sig, "HoverE_sig/F");
  outtree_->Branch("r9Full5x5_sig",    &r9Full5x5_sig, "r9Full5x5_sig/F");
  outtree_->Branch("sieieFull5x5_sig", &sieieFull5x5_sig, "sieieFull5x5_sig/F");
  outtree_->Branch("sieipFull5x5_sig", &sieipFull5x5_sig, "sieipFull5x5_sig/F");
  outtree_->Branch("sipipFull5x5_sig", &sipipFull5x5_sig, "sipipFull5x5_sig/F");
  outtree_->Branch("chIso_sig",        &chIso_sig, "chIso_sig/F");
  outtree_->Branch("phoIso_sig",       &phoIso_sig, "phoIso_sig/F");
  outtree_->Branch("nhIso_sig",        &nhIso_sig, "nhIso_sig/F");
  outtree_->Branch("chWorst_sig",      &chWorst_sig, "chWorst_sig/F");
  
  //outtree_->Branch("MIPTotEnergy", &MIPTotEnergy_, "MIPTotEnergy/F");

  outtree_->Branch("genMET", &genMET, "genMET/F");
  outtree_->Branch("genMETPhi", &genMETPhi, "genMETPhi/F");
  outtree_->Branch("pfMET", &pfMET, "pfMET/F");
  outtree_->Branch("pfMETPhi", &pfMETPhi, "pfMETPhi/F");

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
    
    //reco
    Float_t* phoE = 0; 
    Float_t* phoEt = 0;
    Float_t* phoEta = 0;
    Float_t* phoPhi = 0;
    Float_t* phoR9  = 0;
    Float_t* phoHoverE = 0; 
    Float_t* phoPFChIso          = 0; 
    Float_t* phoPFNeuIso         = 0; 
    Float_t* phoPFPhoIso         = 0; 
    Int_t*   phoEleVeto          = 0; 
    Float_t* phoSigmaIEtaIEtaFull5x5  = 0;
    Float_t* phoSigmaIEtaIPhiFull5x5  = 0;
    Float_t* phoSigmaIPhiIPhiFull5x5  = 0;
    Float_t* phoR9Full5x5           = 0;
    Float_t* phoPFChWorst   = 0; 
    Float_t  rho                    = 0; 
    Short_t* phoID         = 0;

    Long64_t* phoFiredTrgs = 0;
    Float_t* phoMIPTotEnergy= 0;

    phoE = data.GetPtrFloat("phoE");
    phoEt = data.GetPtrFloat("phoEt");
    phoEta = data.GetPtrFloat("phoEta");
    phoPhi = data.GetPtrFloat("phoPhi");
    phoR9  = data.GetPtrFloat("phoR9");
    phoHoverE = data.GetPtrFloat("phoHoverE");
    phoPFChIso          = data.GetPtrFloat("phoPFChIso");
    phoPFNeuIso         = data.GetPtrFloat("phoPFNeuIso");
    phoPFPhoIso         = data.GetPtrFloat("phoPFPhoIso");
    phoEleVeto          = data.GetPtrInt("phoEleVeto");
    phoSigmaIEtaIEtaFull5x5  = data.GetPtrFloat("phoSigmaIEtaIEtaFull5x5");
    phoSigmaIEtaIPhiFull5x5  = data.GetPtrFloat("phoSigmaIEtaIPhiFull5x5");
    phoSigmaIPhiIPhiFull5x5  = data.GetPtrFloat("phoSigmaIPhiIPhiFull5x5");
    phoR9Full5x5           = data.GetPtrFloat("phoR9Full5x5");
    phoPFChWorst             = data.GetPtrFloat("phoPFChWorstIso");
    rho                    = data.GetFloat("rho");
    phoID         = data.GetPtrShort("phoIDbit");
    phoFiredTrgs = data.GetPtrLong64("phoFiredSingleTrgs");
    
    //phoMIPTotEnergy = data.GetPtrFloat("phoMIPTotEnergy");
      
    pfMET     = data.GetFloat("pfMET");
    pfMETPhi  = data.GetFloat("pfMETPhi");

    
    //rho correction
    vector<vector<Float_t>> Iso_raw; //[ch, pho, nh, chw]
    vector<vector<Float_t>> Iso_rc; //[ch, pho, nh, chw]
    Iso_raw.clear();
    Iso_rc.clear();
    for(Int_t iso = 0; iso < 4; iso++){
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
      if(fabs(mcMomPID[k]) >= 25) continue;
      if(fabs(mcMomPID[k]) > 6 && fabs(mcMomPID[k]) < 22) continue;
      if((mcStatus[k]>>1&1) == 0) continue;
      if(mcPID[k] == 22){
	mc_phoid.push_back(k);
	nMCpho++;
      }
    }
    h_nmcpho->Fill(nMCpho);

    //gen-matching
    vector <Int_t> matchpho_list; matchpho_list.clear();
    vector <Int_t> match;
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
	  //printf("MC phomatched !");
	  matchpho_list.push_back(ipho);
	  nmatchpho++;
	  break;
	}
      }
      match.push_back(isMatched);
    }
    h_matchpho->Fill(nmatchpho);

    vector<Int_t> cutIDpho_list; cutIDpho_list.clear();//Medium Id
    phoIDcut(1, data, cutIDpho_list);

    Int_t nphotrgs =0;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(isPVGood==0) continue;
      if(match[ipho] ==1){
	//if(fabs(phoEta[ipho]) < 1.4442 && (phoFiredTrgs[ipho]>>6&1) == 1){
	//nphotrgs++;
	if(fabs(phoEta[ipho]) < 1.4442){
	  h_phoEB_pt->Fill(phoEt[ipho]);
	  h_phoEB_eta->Fill(phoEta[ipho]);
	}
	Int_t pho_presel = pho_preselection(data, ipho, kTRUE);
	if(pho_presel ==1){
	  h_phoEB_pt_HLT->Fill(phoEt[ipho]);
	  if(phoEt[ipho]<200.) continue;
	  h_phoEB_pt_200->Fill(phoEt[ipho]);
	  h_dphoMETPhi->Fill(deltaPhi(phoPhi[ipho], pfMETPhi));

	  r9Full5x5    =  phoR9Full5x5[ipho];
	  HoverE       =  phoHoverE[ipho];
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
	    
	  if(cutIDpho_list[ipho] != 1) continue;
	  h_phoEB_pt_M->Fill(phoEt[ipho]);
	  h_phoEB_eta_M->Fill(phoEta[ipho]);

	  //if(chWorst_ < 1.5) h_phoEB_pt_chworst->Fill(phoEt[ipho]);
	  //if(chWorst_ < 1.5 && HoverE < 0.05) h_phoEB_pt_HoverE->Fill(phoEt[ipho]);
	  //if(chWorst_ < 1.5 && HoverE < 0.05 && r9Full5x5 < 1){
	  if(HoverE < 0.05) h_phoEB_pt_HoverE->Fill(phoEt[ipho]);
	  if(HoverE < 0.05 && r9Full5x5 < 1){
	    h_phoEB_pt_r9->Fill(phoEt[ipho]);
	    if(pfMET >140) h_phoEB_pt_MET->Fill(phoEt[ipho]);
	    if(fabs(deltaPhi(phoPhi[ipho], pfMETPhi)) > 1.6) {
	      h_phoEB_pt_dphoMETPhi->Fill(phoEt[ipho]);
	      h_MET->Fill(pfMET);
	    }
	    
	    //Float_t MIPE = fabs(phoMIPTotEnergy[ipho]*phoMIPTotEnergy[ipho]);
	    //h_MIPEn2->Fill(MIPE);
	    if(fabs(deltaPhi(phoPhi[ipho], pfMETPhi)) > 1.6 && (pfMET > 140)){
	      //h_phoEB_pt_MIPEn2->Fill(phoEt[ipho]);
	      h_phoEB_ptoverMET->Fill(phoEt[ipho]/pfMET);
	      mcphoEt = mcPt[ipho];
	      mcphoEta = mcEta[ipho];
	      mcphoPhi = mcPhi[ipho];
	      realphoEt = phoEt[ipho];
	      realphoEta = phoEta[ipho];
	      realphoPhi = phoPhi[ipho];
      
	      r9Full5x5_sig = phoR9Full5x5[ipho];
	      HoverE_sig = phoHoverE[ipho];
	      sieieFull5x5_sig = phoSigmaIEtaIEtaFull5x5[ipho];
	      sieipFull5x5_sig = phoSigmaIEtaIPhiFull5x5[ipho];
	      sipipFull5x5_sig = phoSigmaIPhiIPhiFull5x5[ipho];
	      chIso_sig = Iso_rc[0][ipho];
	      phoIso_sig = Iso_rc[1][ipho];
	      nhIso_sig = Iso_rc[2][ipho];
	      chWorst_sig = Iso_rc[3][ipho];
	    }
	  }
	}
	  
      }//matched pho
    }
    h_nphotrgs->Fill(nphotrgs);
    outtree_->Fill();
  }//event loop
  //****************END LOOP**********************//

  //****************Terminate*********************//
  fout_->cd();
  outtree_->Write();

  h_npho->Write();
  h_nmcpho->Write();
  h_matchpho->Write(); 
  h_dr_pho->Write();
  h_dpt_pho->Write();
  h_dptdr_pho->Write();

  h_phoEB_pt->Write();
  h_phoEB_pt_HLT->Write();
  h_phoEB_pt_200->Write();
  h_phoEB_pt_M->Write();
  h_phoEB_pt_chworst->Write();
  h_phoEB_pt_HoverE->Write();
  h_phoEB_pt_r9->Write();
  h_phoEB_pt_dphoMETPhi->Write();
  h_phoEB_pt_MET->Write();
  //h_phoEB_pt_MIPEn2->Write();
  h_phoEB_ptoverMET->Write();
  
  h_phoEB_eta->Write();
  h_phoEB_eta_M->Write();
  
  h_MET->Write();
  //h_METPhi->Write();
  h_dphoMETPhi->Write();
  h_nphotrgs->Write();
  //h_MIPEn2->Write();

  fout_->Close();
  fprintf(stderr, "Processed all events\n");
}
