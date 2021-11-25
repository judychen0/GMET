#include <iostream>
#include <vector>
#include <math.h>
using namespace std;
#include "TH1F.h"
#include "TH2F.h"
#include "TVector2.h"

#include "smallntuplizer.h"
//#include "untuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "PhotonSelection.h"

#define ncat 2

void xZgOOT(char* pathes){
  //Get processed events
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("hEvents")->Clone();
  hEvents->SetDirectory(0);
  fopen->Close();

  //access EventTree with TreeReader class
  TreeReader data(pathes);
  //cout << "get tree" << endl;
  //create an output .root file
  TFile *fout_;
  char foutname[50];
  string treestr = pathes;
  Int_t istr = treestr.find("ggtree");
  string treename = treestr.substr(istr);
  sprintf(foutname, "OOT_%s", treename.c_str());
  fout_ = new TFile(foutname,"RECREATE");

  Int_t nINCPho = 0;
  vector<Float_t> incPhoEt;
  vector<Float_t> incPhoEta;
  vector<Float_t> incPhoPhi;
  vector<Float_t> incPhoSieie;
  vector<Float_t> incPhoHoverE;
  vector<Float_t> incPhoR9;
  vector<Float_t> incPhoMIP;
  vector<Float_t> incPhoSeedTime;
  vector<Float_t> incPhoSeedEn;
  Int_t npOOTPho = 0;
  vector<Float_t> pOOTPhoEt;
  vector<Float_t> pOOTPhoEta;
  vector<Float_t> pOOTPhoPhi;
  vector<Float_t> pOOTPhoSieie;
  vector<Float_t> pOOTPhoHoverE;
  vector<Float_t> pOOTPhoR9;
  vector<Float_t> pOOTPhoMIP;
  vector<Float_t> pOOTPhoSeedTime;
  vector<Float_t> pOOTPhoSeedEn;
  Float_t pfMET_;
  Int_t metfilters_;
  
  TTree *outtree_;
  outtree_ = new TTree("EventTree", "EventTree");

  outtree_->Branch("nINCPho", &nINCPho, "nINCPho/I");
  outtree_->Branch("incPhoEt", &incPhoEt);
  outtree_->Branch("incPhoEta", &incPhoEta);
  outtree_->Branch("incPhoPhi", &incPhoPhi);
  outtree_->Branch("incPhoSieie", &incPhoSieie);
  outtree_->Branch("incPhoHoverE", &incPhoHoverE);
  outtree_->Branch("incPhoR9", &incPhoR9);
  outtree_->Branch("incPhoMIP", &incPhoMIP);
  outtree_->Branch("incPhoSeedTime", &incPhoSeedTime);
  outtree_->Branch("incPhoSeedEn", &incPhoSeedEn);
  outtree_->Branch("npOOTPho", &npOOTPho, "npOOTPho/I");
  outtree_->Branch("pOOTPhoEt", &pOOTPhoEt);
  outtree_->Branch("pOOTPhoEta", &pOOTPhoEta);
  outtree_->Branch("pOOTPhoPhi", &pOOTPhoPhi);
  outtree_->Branch("pOOTPhoSieie", &pOOTPhoSieie);
  outtree_->Branch("pOOTPhoHoverE", &pOOTPhoHoverE);
  outtree_->Branch("pOOTPhoR9", &pOOTPhoR9);
  outtree_->Branch("pOOTPhoMIP", &pOOTPhoMIP);
  outtree_->Branch("pOOTPhoSeedTime", &pOOTPhoSeedTime);
  outtree_->Branch("pOOTPhoSeedEn", &pOOTPhoSeedEn);
  outtree_->Branch("pfMET", &pfMET_, "pfMET/F");
  outtree_->Branch("metfilters", &metfilters_, "metfilters/I");

  
  
  TH1F* h_phoEB_sieie;
  TH1F* h_phoEB_HoverE;
  TH1F* h_phoEB_SeedTime;
  TH1F* h_phoEB_phoEt;
  TH1F* h_phoEB_phoEta;
  TH1F* h_phoEB_phoPhi;
  TH1F* h_phoEB_MIP;
  TH2F* h2_phoEB_sieie_MIP;

  h_phoEB_sieie = new TH1F("h_phoEB_sieie", "h_phoEB_sieie", 150, 0, 0.03);
  h_phoEB_HoverE = new TH1F("h_phoEB_HoverE", "h_phoEB_HoverE", 30, 0, 0.3);
  h_phoEB_SeedTime = new TH1F("h_phoEB_SeedTime", "h_phoEB_SeedTime", 100, -25, 25);
  h_phoEB_MIP = new TH1F("h_phoEB_MIP", "h_phoEB_MIP", 10, 0, 10);
  h2_phoEB_sieie_MIP = new TH2F("h2_phoEB_sieie_MIP", "h2_phoEB_sieie_MIP", 150, 0, 0.03, 10, 0, 10);
  h_phoEB_sieie->Sumw2();
  h_phoEB_HoverE->Sumw2();
  h_phoEB_SeedTime->Sumw2();
  h_phoEB_MIP->Sumw2();
  h2_phoEB_sieie_MIP->Sumw2();

  TH1F* h_phoEBprompt_sieie;
  TH1F* h_phoEBprompt_HoverE;
  TH1F* h_phoEBprompt_SeedTime;
  TH1F* h_phoEBprompt_phoEt;
  TH1F* h_phoEBprompt_phoEta;
  TH1F* h_phoEBprompt_phoPhi;
  TH1F* h_phoEBprompt_MIP;
  TH1F* h_phoEBprompt_SeedEn;
  TH2F* h2_phoEBprompt_sieie_MIP;
  TH2F* h2_phoEBprompt_SCEta_SCPhi;

  h_phoEBprompt_sieie = new TH1F("h_phoEBprompt_sieie", "h_phoEBprompt_sieie", 150, 0, 0.03);
  h_phoEBprompt_HoverE = new TH1F("h_phoEBprompt_HoverE", "h_phoEBprompt_HoverE", 30, 0, 0.3);
  h_phoEBprompt_SeedTime = new TH1F("h_phoEBprompt_SeedTime", "h_phoEBprompt_SeedTime", 100, -25, 25);
  h_phoEBprompt_MIP = new TH1F("h_phoEBprompt_MIP", "h_phoEBprompt_MIP", 10, 0, 10);
  h_phoEBprompt_SeedEn = new TH1F("h_phoEBprompt_SeedEn", "h_phoEBprompt_SeedEn", 100, 0, 1000);
  h2_phoEBprompt_sieie_MIP = new TH2F("h2_phoEBprompt_sieie_MIP", "h2_phoEBprompt_sieie_MIP", 150, 0, 0.03, 10, 0, 10);
  //h2_phoEBprompt_SCEta_SCPhi = new TH2F("h2_phoEBprompt_SCEta_SCPhi", "h2_phoEBprompt_SCEta_SCPhi", )
  h_phoEBprompt_sieie->Sumw2();
  h_phoEBprompt_HoverE->Sumw2();
  h_phoEBprompt_SeedTime->Sumw2();
  h_phoEBprompt_MIP->Sumw2();
  h_phoEBprompt_SeedEn->Sumw2();
  h2_phoEBprompt_sieie_MIP->Sumw2();
  
  TH1F* h_ootBH_sieie;
  TH1F* h_ootBH_HoverE;
  TH1F* h_ootBH_SeedTime;
  TH1F* h_ootBH_phoEt;
  TH1F* h_ootBH_phoEta;
  TH1F* h_ootBH_phoPhi;
  TH1F* h_ootBH_MIP;
  TH2F* h2_ootBH_sieie_MIP;

  h_ootBH_sieie = new TH1F("h_ootBH_sieie", "h_ootBH_sieie", 150, 0, 0.03);
  h_ootBH_HoverE = new TH1F("h_ootBH_HoverE", "h_ootBH_HoverE", 30, 0, 0.3);
  h_ootBH_SeedTime = new TH1F("h_ootBH_SeedTime", "h_ootBH_SeedTime", 100, -25, 25);
  h_ootBH_MIP = new TH1F("h_ootBH_MIP", "h_ootBH_MIP", 10, 0, 10);
  h2_ootBH_sieie_MIP = new TH2F("h2_ootBH_sieie_MIP", "h2_ootBH_sieie_MIP", 150, 0, 0.03, 10, 0, 10);
  h_ootBH_sieie->Sumw2();
  h_ootBH_HoverE->Sumw2();
  h_ootBH_SeedTime->Sumw2();
  h_ootBH_MIP->Sumw2();
  h2_ootBH_sieie_MIP->Sumw2();

  TH1F* h_ootBH210_sieie;
  TH1F* h_ootBH210_HoverE;
  TH1F* h_ootBH210_SeedTime;
  TH1F* h_ootBH210_phoEt;
  TH1F* h_ootBH210_phoEta;
  TH1F* h_ootBH210_phoPhi;
  TH1F* h_ootBH210_MIP;
  TH2F* h2_ootBH210_sieie_MIP;

  h_ootBH210_sieie = new TH1F("h_ootBH210_sieie", "h_ootBH210_sieie", 150, 0, 0.03);
  h_ootBH210_HoverE = new TH1F("h_ootBH210_HoverE", "h_ootBH210_HoverE", 30, 0, 0.3);
  h_ootBH210_SeedTime = new TH1F("h_ootBH210_SeedTime", "h_ootBH210_SeedTime", 100, -25, 25);
  h_ootBH210_MIP = new TH1F("h_ootBH210_MIP", "h_ootBH210_MIP", 10, 0, 10);
  h2_ootBH210_sieie_MIP = new TH2F("h2_ootBH210_sieie_MIP", "h2_ootBH210_sieie_MIP", 150, 0, 0.03, 10, 0, 10);
  h_ootBH210_sieie->Sumw2();
  h_ootBH210_HoverE->Sumw2();
  h_ootBH210_SeedTime->Sumw2();
  h_ootBH210_MIP->Sumw2();
  h2_ootBH210_sieie_MIP->Sumw2();

  TH1F* h_ootSP_sieie;
  TH1F* h_ootSP_HoverE;
  TH1F* h_ootSP_SeedTime;
  TH1F* h_ootSP_phoEt;
  TH1F* h_ootSP_phoEta;
  TH1F* h_ootSP_phoPhi;
  TH1F* h_ootSP_MIP;
  TH2F* h2_ootSP_sieie_MIP;

  h_ootSP_sieie = new TH1F("h_ootSP_sieie", "h_ootSP_sieie", 150, 0, 0.03);
  h_ootSP_HoverE = new TH1F("h_ootSP_HoverE", "h_ootSP_HoverE", 30, 0, 0.3);
  h_ootSP_SeedTime = new TH1F("h_ootSP_SeedTime", "h_ootSP_SeedTime", 100, -25, 25);
  h_ootSP_MIP = new TH1F("h_ootSP_MIP", "h_ootSP_MIP", 10, 0, 10);
  h2_ootSP_sieie_MIP = new TH2F("h2_ootSP_sieie_MIP", "h2_ootSP_sieie_MIP", 150, 0, 0.03, 10, 0, 10);
  h_ootSP_sieie->Sumw2();
  h_ootSP_HoverE->Sumw2();
  h_ootSP_SeedTime->Sumw2();
  h_ootSP_MIP->Sumw2();
  h2_ootSP_sieie_MIP->Sumw2();

  TH1F* h_ootSP210_sieie;
  TH1F* h_ootSP210_HoverE;
  TH1F* h_ootSP210_SeedTime;
  TH1F* h_ootSP210_phoEt;
  TH1F* h_ootSP210_phoEta;
  TH1F* h_ootSP210_phoPhi;
  TH1F* h_ootSP210_MIP;
  TH1F* h_ootSP210_SeedEn;
  TH2F* h2_ootSP210_sieie_MIP;

  h_ootSP210_sieie = new TH1F("h_ootSP210_sieie", "h_ootSP210_sieie", 150, 0, 0.03);
  h_ootSP210_HoverE = new TH1F("h_ootSP210_HoverE", "h_ootSP210_HoverE", 30, 0, 0.3);
  h_ootSP210_SeedTime = new TH1F("h_ootSP210_SeedTime", "h_ootSP210_SeedTime", 100, -25, 25);
  h_ootSP210_MIP = new TH1F("h_ootSP210_MIP", "h_ootSP210_MIP", 10, 0, 10);
  h_ootSP210_SeedEn = new TH1F("h_ootSP210_SeedEn", "h_ootSP210_SeedEn", 100, 0, 1000);
  h2_ootSP210_sieie_MIP = new TH2F("h2_ootSP210_sieie_MIP", "h2_ootSP210_sieie_MIP", 150, 0, 0.03, 10, 0, 10);
  h_ootSP210_sieie->Sumw2();
  h_ootSP210_HoverE->Sumw2();
  h_ootSP210_SeedTime->Sumw2();
  h_ootSP210_MIP->Sumw2();
  h_ootSP210_SeedEn->Sumw2();
  h2_ootSP210_sieie_MIP->Sumw2();
 
  //input variable
  Int_t         nPho            = 0;
  Float_t*	phoE		= 0; 
  Float_t*	phoCalibEt		= 0;
  Float_t*	phoEta	        = 0;
  Float_t*	phoSCEta	= 0;
  Float_t*	phoPhi	       	= 0;
  Float_t*         phoHoverE = 0;
  Float_t*         phoSigmaIEtaIEtaFull5x5 = 0;
  Float_t*         phoR9Full5x5 = 0;
  Float_t*	phoSeedTime	= 0;
  Float_t*      phoSeedEnergy = 0;
  Float_t*	phoMIPTotEnergy       	= 0;
  Int_t*      phohasPixelSeed = 0;
    
  Int_t            nOOTPho = 0;
  Float_t*         ootPhoE = 0;
  Float_t*         ootPhoEt = 0;
  Float_t*         ootPhoEta = 0;
  Float_t*         ootPhoPhi = 0;
  Float_t*         ootPhoSCEta = 0;
  Float_t*         ootPhoSCPhi = 0;
  Float_t*         ootPhoHoverE = 0;
  Float_t*         ootPhoSigmaIEtaIEtaFull5x5 = 0;
  Float_t*         ootPhoR9Full5x5 = 0;
  Float_t*         ootPhoSeedTime = 0;
  Float_t*         ootPhoSeedEnergy = 0;
  Float_t*         ootPhoMIPTotEnergy = 0;
  //Int_t*        ootBeamHaloID = 0;
  //Int_t*        ootSpikeID   = 0;
  Float_t       pfMET           = 0;
  Float_t       pfMETPhi	= 0;
  Int_t         metFilters = 0;
  Float_t* phoMETdPhi =0;
  Float_t minJMETdPhi =0;
  Int_t nLep = 0;
  Int_t npfjet = 0;
  Float_t jetjetdR = 0;
  Float_t jetjetdEta = 0;
  Float_t* phojetdR = 0;
  Double_t dijetMass = 0;

  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);

    nINCPho = 0;
    incPhoEt.clear();
    incPhoEta.clear();
    incPhoPhi.clear();
    incPhoSieie.clear();
    incPhoHoverE.clear();
    incPhoR9.clear();
    incPhoMIP.clear();
    incPhoSeedTime.clear();
    incPhoSeedEn.clear();
    npOOTPho = 0;
    pOOTPhoEt.clear();
    pOOTPhoEta.clear();
    pOOTPhoPhi.clear();
    pOOTPhoSieie.clear();
    pOOTPhoHoverE.clear();
    pOOTPhoR9.clear();
    pOOTPhoMIP.clear();
    pOOTPhoSeedTime.clear();
    pOOTPhoSeedEn.clear();
  
    //reco pho
    nPho			= data.GetInt(		"nPho"				);
    phoCalibEt			= data.GetPtrFloat(	"phoCalibEt"			);
    phoEta			= data.GetPtrFloat(	"phoEta"			);
    phoPhi			= data.GetPtrFloat(	"phoPhi"			);
    phoSCEta			= data.GetPtrFloat(	"phoSCEta"			);
    //phoSCPhi			= data.GetPtrFloat(	"phoSCPhi"			);
    phoHoverE			= data.GetPtrFloat(     "phoHoverE"			);
    phoR9Full5x5		= data.GetPtrFloat(	"phoR9Full5x5"			);
    phoSigmaIEtaIEtaFull5x5     = data.GetPtrFloat(	"phoSigmaIEtaIEtaFull5x5"	);
    phoSeedTime			= data.GetPtrFloat(	"phoSeedTime"			);
    //phoSeedEnergy			= data.GetPtrFloat(	"phoSeedEnergy"			);
    phoMIPTotEnergy		= data.GetPtrFloat(	"phoMIPTotEnergy"		);
    phohasPixelSeed = data.GetPtrInt("phohasPixelSeed");
    
    //if(metFilters>0) continue;

    //oot pho
    nOOTPho			= data.GetInt(		"nOOTPho"			);
    ootPhoEt			= data.GetPtrFloat(	"ootPhoEt"			);
    ootPhoEta			= data.GetPtrFloat(	"ootPhoEta"			);
    ootPhoPhi			= data.GetPtrFloat(	"ootPhoPhi"			);
    ootPhoSCEta			= data.GetPtrFloat(	"ootPhoSCEta"			);
    //ootPhoSCPhi			= data.GetPtrFloat(	"ootPhoSCPhi"			);
    ootPhoHoverE		= data.GetPtrFloat(	"ootPhoHoverE"			);
    ootPhoR9Full5x5		= data.GetPtrFloat(	"ootPhoR9Full5x5"		);
    ootPhoSigmaIEtaIEtaFull5x5  = data.GetPtrFloat(	"ootPhoSigmaIEtaIEtaFull5x5"	);
    ootPhoSeedTime		= data.GetPtrFloat(	"ootPhoSeedTime"		);
    //ootPhoSeedEnergy		= data.GetPtrFloat(	"ootPhoSeedEnergy"		);
    ootPhoMIPTotEnergy		= data.GetPtrFloat(	"ootPhoMIPTotEnergy"		);

    pfMET			= data.GetFloat(	"pfMET"			);
    pfMETPhi = data.GetFloat("pfMETPhi");
    metFilters			= data.GetInt(		"metFilters"			);
    //phoMETdPhi = data.GetPtrFloat(	"phoMETdPhi"		);
    minJMETdPhi = data.GetFloat(        "minJMETdPhi"           );
    nLep = data.GetInt(			"nLep"			);
    npfjet = data.GetInt(		"npfjet"		);
    jetjetdR = data.GetFloat(		"jetjetdR"		);
    jetjetdEta = data.GetFloat(		"jetjetdEta"		);
    phojetdR = data.GetPtrFloat(	"phojetdR"		);
    dijetMass = data.GetDouble(		"dijetMass"		);

    if(nOOTPho==0){
      for(Int_t ipho=0; ipho<nPho; ipho++){
	nINCPho++;

	incPhoEt.push_back(phoCalibEt[ipho]);
	incPhoEta.push_back(phoSCEta[ipho]);
	incPhoPhi.push_back(phoPhi[ipho]);
	incPhoSieie.push_back(phoSigmaIEtaIEtaFull5x5[ipho]);
	incPhoHoverE.push_back(phoHoverE[ipho]);
	incPhoR9.push_back(phoR9Full5x5[ipho]);
	incPhoMIP.push_back(phoMIPTotEnergy[ipho]);
	incPhoSeedTime.push_back(phoSeedTime[ipho]);
	//incPhoSeedEn.push_back(phoSeedEnergy[ipho]);
      }
    }
    else if(nOOTPho>0){
      TVector2 METv2, phov2, ootphov2, sumphov2, sumootphov2;
      TVector2 corrMETv2;
      METv2.SetMagPhi(pfMET, pfMETPhi);
      
      for(Int_t ipho=0; ipho<nPho; ipho++){
	phov2.SetMagPhi(phoCalibEt[ipho], phoPhi[ipho]);
	sumphov2 += phov2;
      }
      for(Int_t opho=0; opho<nOOTPho; opho++){
	ootphov2.SetMagPhi(ootPhoEt[opho], ootPhoPhi[opho]);
	sumootphov2 += ootphov2;
	
	nINCPho++;
		
	incPhoEt.push_back(ootPhoEt[opho]);
	incPhoEta.push_back(ootPhoSCEta[opho]);
	incPhoPhi.push_back(ootPhoPhi[opho]);
	incPhoSieie.push_back(ootPhoSigmaIEtaIEtaFull5x5[opho]);
	incPhoHoverE.push_back(ootPhoHoverE[opho]);
	incPhoR9.push_back(ootPhoR9Full5x5[opho]);
	incPhoMIP.push_back(ootPhoMIPTotEnergy[opho]);
	incPhoSeedTime.push_back(ootPhoSeedTime[opho]);
	//incPhoSeedEn.push_back(ootPhoSeedEnergy[opho]);

	npOOTPho++;
	pOOTPhoEt.push_back(ootPhoEt[opho]);
	pOOTPhoEta.push_back(ootPhoSCEta[opho]);
	pOOTPhoPhi.push_back(ootPhoPhi[opho]);
	pOOTPhoSieie.push_back(ootPhoSigmaIEtaIEtaFull5x5[opho]);
	pOOTPhoHoverE.push_back(ootPhoHoverE[opho]);
	pOOTPhoR9.push_back(ootPhoR9Full5x5[opho]);
	pOOTPhoMIP.push_back(ootPhoMIPTotEnergy[opho]);
	pOOTPhoSeedTime.push_back(ootPhoSeedTime[opho]);
	
      }
      corrMETv2 = METv2 + sumphov2 - sumootphov2;
      pfMET = corrMETv2.Mod();
    }
    
    Int_t fill;
    //phoEB candidate
    fill=0;
    for(Int_t ipho=0; ipho<nINCPho; ipho++){
      //if(abs(incPhoEta[ipho])<1.4442 && incPhoR9[ipho]>0.8  && incPhoEt[ipho]>210 && incPhoHoverE[ipho]<0.2 && pfMET>100){
      if(abs(incPhoEta[ipho])<1.4442 && incPhoR9[ipho]>0.8  && incPhoEt[ipho]>210 && incPhoHoverE[ipho]<0.2 && pfMET>100 && npfjet>=2){
	if(incPhoMIP[ipho]<4.9 && incPhoSieie[ipho]>0.001 && incPhoSieie[ipho]<0.01015 && metFilters==0){
	  h_phoEB_sieie->Fill(incPhoSieie[ipho]);
	  h_phoEB_SeedTime->Fill(incPhoSeedTime[ipho]);
	  h_phoEB_MIP->Fill(incPhoMIP[ipho]);
	}
      }
    }
    
    //prompt
    fill = 0;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      //if(abs(phoSCEta[ipho])<1.4442 && phoR9Full5x5[ipho]>0.8 && phoCalibEt[ipho]>210 && phoHoverE[ipho]<0.2 && phohasPixelSeed[ipho]==1 && pfMET>100){
      if(abs(phoSCEta[ipho])<1.4442 && phoR9Full5x5[ipho]>0.8 && phoCalibEt[ipho]>210 && phoHoverE[ipho]<0.2 && phohasPixelSeed[ipho]==1 && pfMET>100 && npfjet>=2){
	if(phoMIPTotEnergy[ipho]<4.9 && phoSigmaIEtaIEtaFull5x5[ipho]<0.01015 && metFilters==0){
	  h_phoEBprompt_sieie->Fill(phoSigmaIEtaIEtaFull5x5[ipho]);
	  h_phoEBprompt_SeedTime->Fill(phoSeedTime[ipho]);
	  h_phoEBprompt_MIP->Fill(phoMIPTotEnergy[ipho]);
	  //h_phoEBprompt_SeedEn->Fill(phoSeedEnergy[ipho]);
	  fill+=1;
	}
      }
      if(fill>0) break;
    }
    
    
    //Beam halo bkg
    fill = 0;
    for(Int_t ipho=0; ipho<nINCPho; ipho++){
      //if(abs(incPhoEta[ipho])<1.4442 && incPhoR9[ipho]>0.8  && incPhoEt[ipho]>210 && incPhoHoverE[ipho]<0.2 && pfMET>100){
      if(abs(incPhoEta[ipho])<1.4442 && incPhoR9[ipho]>0.8  && incPhoEt[ipho]>210 && incPhoHoverE[ipho]<0.2 && pfMET>100 && npfjet>=2){
	if(incPhoMIP[ipho]>4.9 && incPhoSieie[ipho]>0.001 && metFilters==8 && incPhoSieie[ipho] <0.02){
	  h_ootBH210_sieie->Fill(incPhoSieie[ipho]);
	  h_ootBH210_SeedTime->Fill(incPhoSeedTime[ipho]);
	  h_ootBH210_MIP->Fill(incPhoMIP[ipho]);
	}
      }
    }
    
    
    //spike bkg
    fill = 0;
    for(Int_t ipho=0; ipho<nINCPho; ipho++){
      //if(abs(incPhoEta[ipho])<1.4442 && incPhoR9[ipho]>0.8  && incPhoEt[ipho]>210 && incPhoHoverE[ipho]<0.2 && pfMET>100){
      if(abs(incPhoEta[ipho])<1.4442 && incPhoR9[ipho]>0.8  && incPhoEt[ipho]>210 && incPhoHoverE[ipho]<0.2 && pfMET>100 && npfjet>=2){
	if(incPhoMIP[ipho]<4.9  && incPhoSieie[ipho]<0.0105 && metFilters==0 && incPhoSieie[ipho]<0.001){
	  h_ootSP210_sieie->Fill(incPhoSieie[ipho]);
	  h_ootSP210_SeedTime->Fill(incPhoSeedTime[ipho]);
	  h_ootSP210_MIP->Fill(incPhoMIP[ipho]);
	  //h_ootSP210_SeedEn->Fill(incPhoSeedEn[ipho]);
	  //if(incPhoSieie[ipho]<0.001){
	  //  h_ootSP_sieie->Fill(incPhoSieie[ipho]);
	  //  h_ootSP_SeedTime->Fill(incPhoSeedTime[ipho]);
	  //  h_ootSP_MIP->Fill(incPhoMIP[ipho]);
	  //}
	}
      }
    }
    
    outtree_->Fill();
    
  }
  //****************END LOOP**********************//

  //****************Terminate*********************//
  fout_->cd();

  outtree_->Write();
  hEvents->Write();

  
  h_phoEB_sieie->Write();
  h_phoEB_SeedTime->Write();
  h_phoEB_MIP->Write();

  h_phoEBprompt_sieie->Write();
  h_phoEBprompt_SeedTime->Write();
  h_phoEBprompt_MIP->Write();
  h_phoEBprompt_SeedEn->Write();

  h_ootBH_sieie->Write();
  h_ootBH_SeedTime->Write();
  h_ootBH_MIP->Write();

  h_ootBH210_sieie->Write();
  h_ootBH210_SeedTime->Write();
  h_ootBH210_MIP->Write();

  h_ootSP_sieie->Write();
  h_ootSP_SeedTime->Write();
  h_ootSP_MIP->Write();

  h_ootSP210_sieie->Write();
  h_ootSP210_SeedTime->Write();
  h_ootSP210_MIP->Write();
  h_ootSP210_SeedEn->Write();
  
  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());
  
}
