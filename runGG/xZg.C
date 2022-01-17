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

void xZg(char* pathes, Int_t dataset){
  //Get processed events
  TH1F *hEvents, *hSumofGenW;
  TFile *fopen = new TFile(pathes, "READ");
  if(dataset==0){
    hSumofGenW = (TH1F*)fopen->Get("hSumGenWeight")->Clone();
    hSumofGenW->SetDirectory(0);
  }
  hEvents = (TH1F*)fopen->Get("hEvents")->Clone();
  hEvents->SetDirectory(0);
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

  //input variable
  Bool_t	isData		      = 0;
  Int_t		nGoodVtx	      = 0;
  Float_t*	EventWeight	      = 0;
  Float_t	rho		      = 0;
  Int_t*        phoIsMatch	      = 0;
  Int_t         nPho		      = 0;
  Float_t*	phoE		      = 0; 
  Float_t*	phoEt		      = 0;
  Float_t*	phoEta		      = 0;
  Float_t*	phoSCEta	      = 0;
  Float_t*	phoPhi		      = 0;
  Float_t*      phoPFChIso	      = 0;
  Float_t*      phoPFChWorstIso	      = 0;
  Float_t*      phoPFChIso_rc	      = 0;
  Float_t*      phoPFChWorstIso_newEA = 0;
  Float_t*	phoSeedTime	      = 0;
  Float_t*	phoMIP		      = 0;
  Int_t		metFilters	      = 0;
  Float_t       pfMET		      = 0;
  Float_t       pfMETPhi	      = 0;
  Int_t         isVBS                 = 0;
  Int_t		npfjet		      = 0;
  Float_t*	jetPt		      = 0;
  Float_t*	jetEta		      = 0;
  Float_t*	jetPhi		      = 0;
  Float_t*	jetEn		      = 0;
  Int_t*	nonPUjetid	      = 0;
  Double_t	dijetMass	      = 0;
  Int_t*        jetTID		      = 0;
  Int_t*        jetLID		      = 0;
  Int_t*        jetPUIDMVA	      = 0;
  Int_t		nLep		      = 0;
  Int_t* cutflowID = 0;
  Float_t*	phoMETdPhi	      = 0;
  Float_t       minJMETdPhi	      = 0;
  Float_t*	JMETdPhi	      = 0;
  Float_t	jetjetdEta	      = 0;
  Float_t	jetjetdPhi	      = 0;
  Float_t	jetjetdR	      = 0;
  Float_t*	phojetdEta	      = 0;
  Float_t*	phojetdPhi	      = 0;
  Float_t*	phojetdR	      = 0;

  //output variable
  Int_t isVBS;
  Float_t EventWeight;
  vector<Float_t> phoEB_pt_210;
  vector<Float_t> phoEB_pt_M;
  vector<Float_t> phoEB_pt_leptonveto;
  vector<Float_t> phoEB_pt_MET;
  vector<Float_t> phoEB_pt_dphoMETPhi;
  vector<Float_t> phoEB_pt_djetMETPhi;
  vector<Float_t> phoEB_pt_jetID;
  vector<Float_t> phoEB_pt_jetjetdEta;
  vector<Float_t> phoEB_pt_dijetMass;
  vector<Float_t> phoEB_pt_phojetdR;
  vector<Float_t> phoEB_pt_jetjetdR;

  vector<Float_t> phoEB_ptcut;
  vector<Float_t> phoEB_Etacut;
  vector<Float_t> phoEB_Phicut;

  vector<Float_t> MIP_cut;
  vector<Float_t> MET_Nm1;
  vector<Float_t> MET_cut;
  vector<Float_t> djetMETPhi_SB;
  vector<Float_t> MET_Nm1_djetMETPhi_SB;
  vector<Float_t> METPhi_Nm1;
  vector<Float_t> METPhi_cut;
  vector<Float_t> dphoMETPhi_Nm1;
  vector<Float_t> dphoMETPhi_cut;
  vector<Float_t> ptoverMET_cut;
  vector<Float_t> nvtx_cut;
  vector<Float_t> njet_Nm1;
  vector<Float_t> njet_cut;
  vector<Float_t> nlep_cut;

  vector<Float_t> jetpt_210;
  vector<Float_t> jetpt_M;
  vector<Float_t> jetpt_leptonveto;
  vector<Float_t> jetpt_MET;
  vector<Float_t> jetpt_dphoMETPhi;
  vector<Float_t> jetpt_djetMETPhi;
  vector<Float_t> jetpt_jetID;
  vector<Float_t> jetpt_jetjetdEta;
  vector<Float_t> jetpt_dijetMass;
  vector<Float_t> jetpt_phojetdR;
  vector<Float_t> jetpt_jetjetdR;
  
  vector<Float_t> jetpt_cut;
  vector<Float_t> jetEta_cut;
  vector<Float_t> jetPhi_cut;
  vector<Float_t> djetMETPhi_Nm1;
  vector<Float_t> djetMETPhi_cut;
  vector<Float_t> phojetdR_cut;
  vector<Float_t> phojetdEta_cut;
  vector<Float_t> phojetdPhi_cut;
  vector<Float_t> jetjetdR_cut;
  vector<Float_t> jetjetdEta_cut;
  vector<Float_t> jetjetdPhi_cut;
  vector<Float_t> jetjetMass_cut;

  TTree *outtree_;
  outtree_ = new TTree("t", "raw tree");
  
  outtree_->Branch("isVBS",                     &isVBS, "isVBS/I");
  outtree_->Branch("EventWeight", &EventWeight, "EventWeight/F");
  outtree_->Branch("phoEB_pt_210",		&phoEB_pt_210);
  outtree_->Branch("phoEB_pt_M",		&phoEB_pt_M);
  outtree_->Branch("phoEB_pt_leptonveto",	&phoEB_pt_leptonveto);
  outtree_->Branch("phoEB_pt_MET",		&phoEB_pt_MET);
  outtree_->Branch("phoEB_pt_dphoMETPhi",	&phoEB_pt_dphoMETPhi);
  outtree_->Branch("phoEB_pt_djetMETPhi",	&phoEB_pt_djetMETPhi);
  outtree_->Branch("phoEB_pt_jetID",		&phoEB_pt_jetID);
  outtree_->Branch("phoEB_pt_jetjetdEta",	&phoEB_pt_jetjetdEta);
  outtree_->Branch("phoEB_pt_dijetMass",	&phoEB_pt_dijetMass);
  outtree_->Branch("phoEB_pt_phojetdR",		&phoEB_pt_phojetdR);
  outtree_->Branch("phoEB_pt_jetjetdR",		&phoEB_pt_jetjetdR);
  
  outtree_->Branch("phoEB_ptcut",		&phoEB_ptcut);
  outtree_->Branch("phoEB_Etacut",		&phoEB_Etacut);
  outtree_->Branch("phoEB_Phicut",		&phoEB_Phicut);

  outtree_->Branch("MIP_cut",			&MIP_cut);
  outtree_->Branch("MET_Nm1",			&MET_Nm1);
  outtree_->Branch("MET_cut",			&MET_cut);
  outtree_->Branch("MET_Nm1_djetMETPhi_SB",	&MET_Nm1_djetMETPhi_SB);
  outtree_->Branch("METPhi_Nm1",		&METPhi_Nm1);
  outtree_->Branch("METPhi_cut",		&METPhi_cut);
  outtree_->Branch("dphoMETPhi_Nm1",		&dphoMETPhi_Nm1);
  outtree_->Branch("dphoMETPhi_cut",		&dphoMETPhi_cut);
  outtree_->Branch("ptoverMET_cut",	&phoEB_ptoverMET_cut);
  outtree_->Branch("nvtx_cut",			&nvtx_cut);
  outtree_->Branch("njet_Nm1",			&njet_Nm1);
  outtree_->Branch("njet_cut",			&njet_cut);
  outtree_->Branch("nlep_cut",			&nlep_cut);

  outtree_->Branch("jetpt_210",			&jetpt_210);
  outtree_->Branch("jetpt_M",			&jetpt_M);
  outtree_->Branch("jetpt_leptonveto",		&jetpt_leptonveto);
  outtree_->Branch("jetpt_MET",			&jetpt_MET);
  outtree_->Branch("jetpt_dphoMETPhi",		&jetpt_dphoMETPhi);
  outtree_->Branch("jetpt_djetMETPhi",		&jetpt_djetMETPhi);
  outtree_->Branch("jetpt_jetID",		&jetpt_jetID);
  outtree_->Branch("jetpt_jetjetdEta",		&jetpt_jetjetdEta);
  outtree_->Branch("jetpt_dijetMass",		&jetpt_dijetMass);
  outtree_->Branch("jetpt_phojetdR",		&jetpt_phojetdR);
  outtree_->Branch("jetpt_jetjetdR",		&jetpt_jetjetdR);
  
  outtree_->Branch("jetpt_cut",			&jetpt_cut);
  outtree_->Branch("jetEta_cut",		&jetEta_cut);
  outtree_->Branch("jetPhi_cut",		&jetPhi_cut);
  outtree_->Branch("djetMETPhi_Nm1",		&djetMETPhi_Nm1);
  outtree_->Branch("djetMETPhi_cut",		&djetMETPhi_cut);
  outtree_->Branch("phojetdR_cut",		&phojetdR_cut);
  outtree_->Branch("phojetdEta_cut",		&phojetdEta_cut);
  outtree_->Branch("phojetdPhi_cut",		&phojetdPhi_cut);
  outtree_->Branch("jetjetdR_cut",		&jetjetdR_cut);
  outtree_->Branch("jetjetdEta_cut",		&jetjetdEta_cut);
  outtree_->Branch("jetjetdPhi_cut",		&jetjetdPhi_cut);
  outtree_->Branch("jetjetMass_cut",		&jetjetMass_cut);

  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);

    //init output vector
    isVBS = 0;
    EventWeight = 0;
    phoEB_pt_210.clear();
    phoEB_pt_M.clear();
    phoEB_pt_leptonveto.clear();
    phoEB_pt_MET.clear();
    phoEB_pt_dphoMETPhi.clear();
    phoEB_pt_djetMETPhi.clear();
    phoEB_pt_jetID.clear();
    phoEB_pt_jetjetdEta.clear();
    phoEB_pt_dijetMass.clear();
    phoEB_pt_phojetdR.clear();
    phoEB_pt_jetjetdR.clear();

    phoEB_ptcut.clear();
    phoEB_Etacut.clear();
    phoEB_Phicut.clear();

    MIP_cut.clear();
    MET_Nm1.clear();
    MET_cut.clear();
    MET_Nm1_djetMETPhi_SB.clear();
    METPhi_Nm1.clear();
    METPhi_cut.clear();
    dphoMETPhi_Nm1.clear();
    dphoMETPhi_cut.clear();
    ptoverMET_cut.clear();
    nvtx_cut.clear();
    njet_Nm1.clear();
    njet_cut.clear();
    nlep_cut.clear();

    jetpt_210.clear();
    jetpt_M.clear();
    jetpt_leptonveto.clear();
    jetpt_MET.clear();
    jetpt_dphoMETPhi.clear();
    jetpt_djetMETPhi.clear();
    jetpt_jetID.clear();
    jetpt_jetjetdEta.clear();
    jetpt_dijetMass.clear();
    jetpt_phojetdR.clear();
    jetpt_jetjetdR.clear();
  
    jetpt_cut.clear();
    jetEta_cut.clear();
    jetPhi_cut.clear();
    djetMETPhi_Nm1.clear();
    djetMETPhi_cut.clear();
    phojetdR_cut.clear();
    phojetdEta_cut.clear();
    phojetdPhi_cut.clear();
    jetjetdR_cut.clear();
    jetjetdEta_cut.clear();
    jetjetdPhi_cut.clear();
    jetjetMass_cut.clear();

    //get data
    Bool_t	isData		      = data.GetBool("isData");
    Int_t	nGoodVtx	      = data.GetInt("nGoodVtx");
    Float_t*	EventWeight	      = data.GetPtrFloat("EventWeight");
    Float_t	rho		      = data.GetFloat("rho");
    Int_t*      phoIsMatch	      = data.GetPtrInt("phoIsMatch");
    Int_t       nPho		      = data.GetInt("nPho");
    Float_t*	phoE		      = data.GetPtrFloat("phoE"); 
    Float_t*	phoEt		      = data.GetPtrFloat("phoEt");
    Float_t*	phoEta		      = data.GetPtrFloat("phoEta");
    Float_t*	phoSCEta	      = data.GetPtrFloat("phoSCEta");
    Float_t*	phoPhi		      = data.GetPtrFloat("phoPhi");
    Float_t*    phoPFChIso	      = data.GetPtrFloat("phoPFChIso");
    Float_t*    phoPFChWorstIso	      = data.GetPtrFloat("phoPFChWorstIso");
    Float_t*    phoPFChIso_rc	      = data.GetPtrFloat("phoPFChIso_rc");
    Float_t*    phoPFChWorstIso_newEA = data.GetPtrFloat("phoPFChWorstIso_newEA");
    Float_t*	phoSeedTime	      = data.GetPtrFloat("phoSeedTime");
    Float_t*	phoMIP		      = data.GetPtrFloat("phoMIP");
    Int_t	metFilters	      = data.GetInt("metFilters");
    Float_t     pfMET		      = data.GetFloat("corrMET");
    Float_t     pfMETPhi	      = data.GetFloat("corrMETPhi");
    Int_t       isVBS = data.GetInt("isVBS");
    Int_t	npfjet		      = data.GetInt("npfjet");
    Float_t*	jetPt		      = data.GetPtrFloat("jetPt");
    Float_t*	jetEta		      = data.GetPtrFloat("jetEta");
    Float_t*	jetPhi		      = data.GetPtrFloat("jetPhi");
    Float_t*	jetEn		      = data.GetPtrFloat("jetEn");
    Int_t*	nonPUjetid	      = data.GetPtrInt("nonPUjetid");
    Double_t	dijetMass	      = data.GetDouble("dijetMass");
    Int_t*      jetTID		      = data.GetPtrInt("jetTID");
    Int_t*      jetLID		      = data.GetPtrInt("jetLID");
    Int_t*      jetPUIDMVA	      = data.GetPtrInt("jetPUIDMVA");
    Int_t	nLep		      = data.GetInt("nLep");
    Int_t* cutflowID = data.GetPtrInt("cutflowID");
    Float_t*	phoMETdPhi	      = data.GetPtrFloat("phoMETdPhi");
    Float_t     minJMETdPhi	      = data.GetFloat("minJMETdPhi");
    Float_t*	JMETdPhi	      = data.Get("JMETdPhi");
    Float_t	jetjetdEta	      = data.GetFloat("jetjetdEta");
    Float_t	jetjetdPhi	      = data.GetFloat("jetjetdPhi");
    Float_t	jetjetdR	      = data.GetFloat("jetjetdR");
    Float_t*	phojetdEta	      = data.GetPtrFloat("phojetdEta");
    Float_t*	phojetdPhi	      = data.GetPtrFloat("phojetdPhi");
    Float_t*	phojetdR	      = data.GetPtrFloat("phojetdR");

    //MET filters
    if(dataset==1 && metFilters>0) continue;

    Int_t fillpho = 1;
    for(Int_t ipho=0; ipho<fillpho; ipho++){
      if(pho_sel(cutflowID[ipho],3)==1) phoEB_pt_210.push_back(phoEt[ipho]);
      if(pho_sel(cutflowID[ipho],4)==1) phoEB_pt_M.push_back(phoEt[ipho]);
      if(pho_sel(cutflowID[ipho],6)==1) phoEB_pt_leptonveto.push_back(phoEt[ipho]);
      if(pho_sel(cutflowID[ipho],7)==1) phoEB_pt_MET.push_back(phoEt[ipho]);
      if(pho_sel(cutflowID[ipho],8)==1) phoEB_pt_dphoMETPhi.push_back(phoEt[ipho]);
      if(pho_sel(cutflowID[ipho],9)==1) phoEB_pt_djetMETPhi.push_back(phoEt[ipho]);
      if(pho_sel(cutflowID[ipho],10)==1) phoEB_pt_jetID.push_back(phoEt[ipho]);
      if(pho_sel(cutflowID[ipho],11)==1) phoEB_pt_jetjetdEta.push_back(phoEt[ipho]);
      if(pho_sel(cutflowID[ipho],12)==1) phoEB_pt_dijetMass.push_back(phoEt[ipho]);
      if(pho_sel(cutflowID[ipho],13)==1) phoEB_pt_phojetdR.push_back(phoEt[ipho]);
      if(pho_sel(cutflowID[ipho],14)==1) phoEB_pt_jetjetdR.push_back(phoEt[ipho]);

      //results
      if(pho_sel(cutflowID[ipho],14)==1){
	phoEB_ptcut.push_back(phoEt[ipho]);
	phoEB_Etacut.push_back(phoEta[ipho]);
	phoEB_Phicut.push_back(phoPhi[ipho]);
	ptoverMET.push_back(phoEt[ipho]/pfMET);
	MET_cut.push_back(pfMET);
	METPhi_cut.push_back(pfMETPhi);
	MIP_cut.push_back(phoMIP[ipho]);
	dphoMETPhi_cut.push_back(phoMETdPhi[ipho]);
	nvtx_cut.push_back(nGoodVtx);
	njet_cut.push_back(npfjet);
	nlep_cut.push_back(nLep);	
	
	for(Int_t jj=0; jj<npfjet; jj++){
	  Int_t ijet = nonPUjetid[jj];
	  if(isVBS==1 && jj>=2) continue;
	
	  jetpt_cut.push_back(jetPt[ijet]);
	  jetEta_cut.push_back(jetEta[ijet]);
	  jetPhi_cut.push_back(jetPhi[ijet]);
	  djetMETPhi_cut.push_back(JMETdPhi[jj]);
	  phojetdR.push_back(phojetdR[jj]);
	  phojetdEta.push_back(phojetdEta[jj]);
	  phojetdPhi.push_back(phojetdPhi[jj]);
	}

	if(isVBS==1){
	  jetjetdR_cut.push_back(jetjetdR);
	  jetjetdEta_cut.push_back(fabs(jetjetdEta));
	  jetjetdPhi_cut.push_back(jetjetdPhi);
	  jetjetMass_cut.push_back(dijetMass);
	}
      }

      //Nm1
      if(Nm1_sel(cutflowID[ipho],7)==1){
	MET_Nm1.push_back(pfMET);
	METPhi_Nm1.push_back(pfMETPhi);
      }
      if(Nm1_sel(cutflowID[ipho],8)==1) dphoMETPhi_Nm1.push_back(phoMETdPhi[ipho]);
      for(Int_t jj=0; jj<npfjet; jj++){
	Int_t ijet = nonPUjetid[jj];
	if(Nm1_sel(cutflowID[ipho],9)==1) djetMETPhi_Nm1.push_back(JMETdPhi[jj]);
      }
      if(Nm1_sel(cutflowID[ipho],10)==1) njet_Nm1.push_back(npfjet);

      //Nm2
      if(Nm2_sel(cutflowID[ipho],7,9) ==1){
	
      }
      
    }
    
    
  }
  //****************END LOOP**********************//

  //****************Terminate*********************//
  fout_->cd();
  outtree_->Write();

  hEvents->Write();

  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());
  
}
