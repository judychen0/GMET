#include <iostream>
using namespace std;
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TVector2.h"

#include "untuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "IsoCorrection.h"
#include "PhotonSelection.h"
#include "JetSelection.h"
#include "METXYshift.h"
#include "puweicalc.h"

Double_t deltaPhi(Double_t phi1, Double_t phi2) {
  Double_t dPhi = phi1 - phi2;
  if (dPhi > TMath::Pi()) dPhi -= 2.*TMath::Pi();
  if (dPhi < -TMath::Pi()) dPhi += 2.*TMath::Pi();
  return dPhi;
}

Double_t Phi_mpi_pi(Double_t phi){
  if(phi > TMath::Pi()) phi -= 2.*TMath::Pi();
  if(phi < -TMath::Pi()) phi += 2.*TMath::Pi();
  return phi;
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
  return (bit & ~(1<<nbit));
}

void xSkim_data(char* pathes, Int_t year){

  //Get processed events
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("ggNtuplizer/hEvents")->Clone();
  hEvents->SetDirectory(0);
  fopen->Close();

  //access EventTree
  TreeReader data(pathes);
 
  //Creat Output skimmed tree
  TFile *fout_;
  char foutname[50];
  string treestr = pathes;
  Int_t istr = treestr.find("ggtree");
  string treename = treestr.substr(istr);
  sprintf(foutname, "mini_%s", treename.c_str());
  fout_ = new TFile(foutname,"RECREATE");

  //input variables
  Int_t         run_;
  Long64_t      event_;
  Bool_t        isData_;
  Int_t         nVtx_;
  Int_t         nGoodVtx_;
  Bool_t        isPVGood_;
  Float_t       rho_;
  Float_t       pfMET_;
  Float_t       pfMETPhi_;
  Int_t         metFilters_;
  Int_t         nPho_;
  Float_t*      phoE_;
  Float_t*      phoEt_;
  Float_t*      phoEta_;
  Float_t*      phoPhi_;
  Float_t*      phoCalibE_;
  Float_t*      phoCalibEt_;
  Float_t*      phoSCEta_;
  Float_t*      phoSCPhi_;
  Int_t*        phohasPixelSeed_;
  Int_t*        phoEleVeto_;
  Float_t*      phoHoverE_;
  Float_t*      phoSigmaIEtaIEtaFull5x5_;
  Float_t*      phoR9Full5x5_;
  Float_t*      phoPFChIso_;
  Float_t*      phoPFPhoIso_;
  Float_t*      phoPFNeuIso_;
  Float_t*      phoPFChWorstIso_;
  Long64_t*     phoFiredSingleTrgs_;
  Float_t*      phoSeedTime_;
  Float_t*      phoSeedEnergy_;
  Float_t*      phoMIPTotEnergy_;
  Short_t*      phoIDbit_;
  Int_t         nOOTPho_;
  Float_t*      ootPhoE_;
  Float_t*      ootPhoEt_;
  Float_t*      ootPhoEta_;
  Float_t*      ootPhoPhi_;
  Float_t*      ootPhoSCEta_;
  Float_t*      ootPhoSCPhi_;
  Float_t*      ootPhoHoverE_;
  Float_t*      ootPhoSigmaIEtaIEtaFull5x5_;
  Float_t*      ootPhoR9Full5x5_;
  Float_t*      ootPhoSeedTime_;
  Float_t*      ootPhoSeedEnergy_;
  Float_t*      ootPhoMIPTotEnergy_;
  Int_t         nEle_;
  Float_t*      elePt_;
  Float_t*      eleD0_;
  Float_t*      eleDz_;
  Float_t*      eleEta_;
  Short_t*      eleIDbit_;
  Int_t         nMu_;
  Float_t*      muPt_;
  Int_t*        muIDbit_;
  Float_t*      muD0_;
  Float_t*      muDz_;
  Int_t         nJet_;
  Float_t*      jetPt_;
  Float_t*      jetEn_;
  Float_t*      jetEta_;
  Float_t*      jetPhi_;
  Float_t*      jetPUID_;
  Int_t*        jetID_;
  Float_t*	jetCEF_;
  Float_t*	jetMUF_;
  Float_t*      jetCHF_;
  Float_t*      jetNHF_;
  Float_t*      jetNEF_;
  Int_t*        jetNCH_;
  Int_t*        jetNNP_;
  
  //output branch variables
  Int_t                 run;
  Long64_t              event;
  Bool_t                isData;
  Int_t                 nVtx;
  Int_t                 nGoodVtx;
  Bool_t                isPVGood;
  Float_t               rho;
  Float_t               EventWeight;
  Float_t               pfMET;
  Float_t               pfMETPhi;
  Float_t		corrMET;
  Float_t		corrMETPhi;
  Int_t                 metFilters;
  Int_t                 failedMET;
  Int_t                 nPho;//HLT pho
  vector<Float_t>       phoE;
  vector<Float_t>       phoEt;
  vector<Float_t>       phoEta;
  vector<Float_t>       phoPhi;
  vector<Float_t>       phoCalibE;
  vector<Float_t>       phoCalibEt;
  vector<Float_t>       phoSCEta;
  vector<Float_t>       phoSCPhi;
  vector<Int_t>         phohasPixelSeed;
  vector<Int_t>         phoEleVeto;
  vector<Float_t>       phoHoverE;
  vector<Float_t>       phoSigmaIEtaIEtaFull5x5;
  vector<Float_t>       phoR9Full5x5;
  vector<Float_t>       phoPFChIso;
  vector<Float_t>       phoPFPhoIso;
  vector<Float_t>       phoPFNeuIso;
  vector<Float_t>       phoPFChWorstIso;
  vector<Float_t>       phoPFChIso_rc;
  vector<Float_t>       phoPFPhoIso_rc;
  vector<Float_t>       phoPFNeuIso_rc;
  vector<Float_t>       phoPFChWorstIso_rc;
  vector<Float_t>       phoPFChWorstIso_newEA;
  vector<Long64_t>      phoFiredSingleTrgs;
  vector<Float_t>       phoSeedTime;
  vector<Float_t>       phoSeedEnergy;
  vector<Float_t>       phoMIPTotEnergy;
  vector<Float_t>       phoMETdPhi;
  vector<Short_t>       phoIDbit;

  Int_t			nOOTPho;
  vector<Float_t>       ootPhoE;
  vector<Float_t>       ootPhoEt;
  vector<Float_t>       ootPhoEta;
  vector<Float_t>       ootPhoPhi;
  vector<Float_t>       ootPhoSCEta;
  vector<Float_t>       ootPhoSCPhi;
  vector<Float_t>       ootPhoHoverE;
  vector<Float_t>       ootPhoSigmaIEtaIEtaFull5x5;
  vector<Float_t>       ootPhoR9Full5x5;
  vector<Float_t>       ootPhoSeedTime;
  vector<Float_t>       ootPhoSeedEnergy;
  vector<Float_t>       ootPhoMIPTotEnergy;
  vector<Int_t>         ootBeamHaloID;
  vector<Int_t>         ootSpikeID;
    
  vector<Int_t>         MphoID;
  vector<Int_t>         cutflowSMID;
  vector<Int_t>         cutflowVBSID;
  vector<Int_t>         cutflowSMfixMETID;
  vector<Int_t>         cutflowVBSfixMETID;
  Int_t                 nEle;
  vector<Float_t>       elePt;
  vector<Float_t>       eleD0;
  vector<Float_t>       eleDz;
  vector<Float_t>       eleEta;
  vector<Short_t>       eleIDbit;
  Int_t                 nMu;
  vector<Float_t>       muPt;
  vector<Int_t>         muIDbit;
  vector<Float_t>       muD0;
  vector<Float_t>       muDz;
  Int_t                 nLep;
  Int_t                 nJet;
  Int_t                 npfjet;
  Int_t                 nSMjet;
  Int_t                 nVBSjet;
  vector<Int_t>         nonPUjetid;
  vector<Float_t>       jetPt;
  vector<Float_t>       jetEn;
  vector<Float_t>       jetEta;
  vector<Float_t>       jetPhi;
  Double_t              dijetMass;
  vector<Int_t>         jetPUIDMVA;
  vector<Float_t>       jetPUID;
  vector<Int_t>         jetID;
  vector<Int_t>         jetTID;
  vector<Float_t>       jetCEF;
  vector<Float_t>       jetMUF;
  vector<Float_t>	jetCHF;
  vector<Float_t>	jetNHF;
  vector<Float_t>	jetNEF;
  vector<Int_t>		jetNCH;
  vector<Int_t>		jetNNP;
  Float_t               minJMETdPhi;
  vector<Float_t>       JMETdPhi;
  Float_t               jetjetdR;
  Float_t               jetjetdEta;
  Float_t               jetjetdPhi;
  vector<Float_t>       phojetdR;
  vector<Float_t>       phojetdEta;
  vector<Float_t>       phojetdPhi;
    
    
  TTree *outtree_;
  outtree_ = new TTree("EventTree", "EventTree");

  outtree_->Branch("run"			,&run           ,"run/I"		);
  outtree_->Branch("event"			,&event         ,"event/L"		);
  outtree_->Branch("isData"			,&isData        ,"isData/O"		);
  outtree_->Branch("nVtx"			,&nVtx          ,"nVtx/I"		);
  outtree_->Branch("nGoodVtx"			,&nGoodVtx      ,"nGoodVtx/I"		);
  outtree_->Branch("isPVGood"			,&isPVGood      ,"isPVGood/O"		);
  outtree_->Branch("rho"			,&rho           ,"rho/F"		);
  outtree_->Branch("EventWeight"		,&EventWeight   ,"EventWeight/F"	);
  outtree_->Branch("pfMET"			,&pfMET         ,"pfMET/F"		);
  outtree_->Branch("pfMETPhi"			,&pfMETPhi      ,"pfMETPhi/F"		);
  outtree_->Branch("corrMET"			,&corrMET       ,"corrMET/F"            );
  outtree_->Branch("corrMETPhi"			,&corrMETPhi    ,"corrMETPhi/F"         );
  outtree_->Branch("metFilters"			,&metFilters    ,"metFilters/I"		);
  outtree_->Branch("failedMET"			,&failedMET     ,"failedMET/I"		);
  outtree_->Branch("nPho"			,&nPho          ,"nPho/I"		);
  outtree_->Branch("phoE"			,&phoE					);
  outtree_->Branch("phoEt"			,&phoEt					);
  outtree_->Branch("phoEta"			,&phoEta				);
  outtree_->Branch("phoPhi"			,&phoPhi				);
  outtree_->Branch("phoCalibE"			,&phoCalibE				);
  outtree_->Branch("phoCalibEt"			,&phoCalibEt				);
  outtree_->Branch("phoSCEta"			,&phoSCEta				);
  outtree_->Branch("phoSCPhi"			,&phoSCPhi				);
  outtree_->Branch("phohasPixelSeed"            ,&phohasPixelSeed			);
  outtree_->Branch("phoEleVeto"                 ,&phoEleVeto				);
  outtree_->Branch("phoHoverE"                  ,&phoHoverE				);
  outtree_->Branch("phoSigmaIEtaIEtaFull5x5"    ,&phoSigmaIEtaIEtaFull5x5		);
  outtree_->Branch("phoR9Full5x5"               ,&phoR9Full5x5				);
  outtree_->Branch("phoPFChIso"                 ,&phoPFChIso				);
  outtree_->Branch("phoPFPhoIso"                ,&phoPFPhoIso				);
  outtree_->Branch("phoPFNeuIso"                ,&phoPFNeuIso				);
  outtree_->Branch("phoPFChWorstIso"            ,&phoPFChWorstIso			);
  outtree_->Branch("phoPFChIso_rc"              ,&phoPFChIso_rc				);
  outtree_->Branch("phoPFPhoIso_rc"             ,&phoPFPhoIso_rc			);
  outtree_->Branch("phoPFNeuIso_rc"             ,&phoPFNeuIso_rc			);
  outtree_->Branch("phoPFChWorstIso_rc"         ,&phoPFChWorstIso_rc			);
  outtree_->Branch("phoPFChWorstIso_newEA"      ,&phoPFChWorstIso_newEA			);
  outtree_->Branch("phoFiredSingleTrgs"         ,&phoFiredSingleTrgs			);
  outtree_->Branch("phoSeedTime"                ,&phoSeedTime				);
  outtree_->Branch("phoSeedEnergy"              ,&phoSeedEnergy				);
  outtree_->Branch("phoMIPTotEnergy"            ,&phoMIPTotEnergy			);
  outtree_->Branch("phoMETdPhi"                 ,&phoMETdPhi				);
  outtree_->Branch("phoIDbit"                   ,&phoIDbit				);
  outtree_->Branch("nOOTPho"			,&nOOTPho       ,"nOOTPho/I"		);
  outtree_->Branch("ootPhoE"                    ,&ootPhoE				);
  outtree_->Branch("ootPhoEt"                   ,&ootPhoEt				);
  outtree_->Branch("ootPhoEta"                  ,&ootPhoEta				);
  outtree_->Branch("ootPhoPhi"                  ,&ootPhoPhi				);
  outtree_->Branch("ootPhoSCEta"                ,&ootPhoSCEta				);
  outtree_->Branch("ootPhoSCPhi"                ,&ootPhoSCPhi				);
  outtree_->Branch("ootPhoHoverE"               ,&ootPhoHoverE				);
  outtree_->Branch("ootPhoSigmaIEtaIEtaFull5x5" ,&ootPhoSigmaIEtaIEtaFull5x5		);
  outtree_->Branch("ootPhoR9Full5x5"            ,&ootPhoR9Full5x5			);
  outtree_->Branch("ootPhoSeedTime"             ,&ootPhoSeedTime			);
  outtree_->Branch("ootPhoSeedEnergy"           ,&ootPhoSeedEnergy			);
  outtree_->Branch("ootPhoMIPTotEnergy"         ,&ootPhoMIPTotEnergy			);
  outtree_->Branch("ootBeamHaloID"		,&ootBeamHaloID				);
  outtree_->Branch("ootSpikeID"			,&ootSpikeID				);
  outtree_->Branch("MphoID"                     ,&MphoID				);
  outtree_->Branch("cutflowSMID"                ,&cutflowSMID				);
  outtree_->Branch("cutflowVBSID"               ,&cutflowVBSID				);
  outtree_->Branch("cutflowSMfixMETID"          ,&cutflowSMfixMETID			);
  outtree_->Branch("cutflowVBSfixMETID"         ,&cutflowVBSfixMETID			);
  outtree_->Branch("nEle"			,&nEle          ,"nEle/I"		);
  outtree_->Branch("elePt"                      ,&elePt					);
  outtree_->Branch("eleD0"                      ,&eleD0					);
  outtree_->Branch("eleDz"                      ,&eleDz					);
  outtree_->Branch("eleEta"                     ,&eleEta				);
  outtree_->Branch("eleIDbit"                   ,&eleIDbit				);
  outtree_->Branch("nMu"			,&nMu           ,"nMu/I"		);
  outtree_->Branch("muPt"                       ,&muPt					);
  outtree_->Branch("muIDbit"                    ,&muIDbit				);
  outtree_->Branch("muD0"                       ,&muD0					);
  outtree_->Branch("muDz"                       ,&muDz					);
  outtree_->Branch("nLep"			,&nLep          ,"nLep/I"		);
  outtree_->Branch("nJet"			,&nJet          ,"nJet/I"		);
  outtree_->Branch("npfjet"			,&npfjet        ,"npfjet/I"		);
  outtree_->Branch("nSMjet"			,&nSMjet        ,"nSMjet/I"		);
  outtree_->Branch("nVBSjet"			,&nVBSjet       ,"nVBSjet/I"		);
  outtree_->Branch("nonPUjetid"                 ,&nonPUjetid				);
  outtree_->Branch("jetPt"                      ,&jetPt					);
  outtree_->Branch("jetEn"                      ,&jetEn					);
  outtree_->Branch("jetEta"                     ,&jetEta				);
  outtree_->Branch("jetPhi"                     ,&jetPhi				);
  outtree_->Branch("dijetMass"			,&dijetMass     ,"dijetMass/D"		);
  outtree_->Branch("jetPUIDMVA"                 ,&jetPUIDMVA				);
  outtree_->Branch("jetPUID"                    ,&jetPUID				);
  outtree_->Branch("jetID"                      ,&jetID					);
  outtree_->Branch("jetTID"                     ,&jetTID				);
  outtree_->Branch("jetCEF"                     ,&jetCEF				);
  outtree_->Branch("jetMUF"                     ,&jetMUF				);
  outtree_->Branch("jetCHF"                     ,&jetCHF				);
  outtree_->Branch("jetNHF"                     ,&jetNHF				);
  outtree_->Branch("jetNEF"                     ,&jetNEF				);
  outtree_->Branch("jetNCH"                     ,&jetNCH				);
  outtree_->Branch("jetNNP"                     ,&jetNNP				);
  outtree_->Branch("minJMETdPhi"		,&minJMETdPhi   ,"minJMETdPhi/F"	);
  outtree_->Branch("JMETdPhi"                   ,&JMETdPhi				);
  outtree_->Branch("jetjetdR"			,&jetjetdR      ,"jetjetdR/F"		);
  outtree_->Branch("jetjetdEta"			,&jetjetdEta    ,"jetjetdEta/F"		);
  outtree_->Branch("jetjetdPhi"                 ,&jetjetdPhi    ,"jetjetdPhi/F"		);
  outtree_->Branch("phojetdR"                   ,&phojetdR				);
  outtree_->Branch("phojetdEta"                 ,&phojetdEta				);
  outtree_->Branch("phojetdPhi"                 ,&phojetdPhi				);

  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    //cout << "get ev " << ev << endl;
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);

    run_                         = data.GetInt(		"run"				);
    event_			 = data.GetLong64(	"event"				);
    isData_			 = data.GetBool(	"isData"			);
    nVtx_			 = data.GetInt(		"nVtx"				);
    nGoodVtx_			 = data.GetInt(		"nGoodVtx"			);
    isPVGood_                    = data.GetBool(	"isPVGood"			);
    rho_			 = data.GetFloat(	"rho"				);
    pfMET_			 = data.GetFloat(	"pfMET"				);
    pfMETPhi_			 = data.GetFloat(	"pfMETPhi"			);
    metFilters_		         = data.GetInt(		"metFilters"			);
    nPho_			 = data.GetInt(		"nPho"				);
    phoE_			 = data.GetPtrFloat(	"phoE"				);
    phoEt_			 = data.GetPtrFloat(	"phoEt"				);
    phoEta_			 = data.GetPtrFloat(	"phoEta"			);
    phoPhi_			 = data.GetPtrFloat(	"phoPhi"			);
    phoCalibE_			 = data.GetPtrFloat(	"phoCalibE"			);
    phoCalibEt_		         = data.GetPtrFloat(	"phoCalibEt"			);
    phoSCEta_			 = data.GetPtrFloat(	"phoSCEta"			);
    phoSCPhi_			 = data.GetPtrFloat(	"phoSCPhi"			);
    phohasPixelSeed_		 = data.GetPtrInt(	"phohasPixelSeed"		);
    phoEleVeto_		         = data.GetPtrInt(	"phoEleVeto"			);
    phoHoverE_			 = data.GetPtrFloat(	"phoHoverE"			);
    phoSigmaIEtaIEtaFull5x5_     = data.GetPtrFloat(	"phoSigmaIEtaIEtaFull5x5"	);
    phoR9Full5x5_		 = data.GetPtrFloat(	"phoR9Full5x5"			);
    phoPFChIso_		         = data.GetPtrFloat(	"phoPFChIso"			);
    phoPFPhoIso_		 = data.GetPtrFloat(	"phoPFPhoIso"			);
    phoPFNeuIso_		 = data.GetPtrFloat(	"phoPFNeuIso"			);
    phoPFChWorstIso_		 = data.GetPtrFloat(	"phoPFChWorstIso"		);
    phoFiredSingleTrgs_	         = data.GetPtrLong64(	"phoFiredSingleTrgs"		);
    phoSeedTime_		 = data.GetPtrFloat(	"phoSeedTime"			);
    phoSeedEnergy_		 = data.GetPtrFloat(	"phoSeedEnergy"			);
    phoMIPTotEnergy_		 = data.GetPtrFloat(	"phoMIPTotEnergy"		);
    phoIDbit_			 = data.GetPtrShort(	"phoIDbit"			);

    nOOTPho_			 = data.GetInt(		"nOOTPho"			);
    ootPhoE_			 = data.GetPtrFloat(	"ootPhoE"			);
    ootPhoEt_			 = data.GetPtrFloat(	"ootPhoEt"			);
    ootPhoEta_			 = data.GetPtrFloat(	"ootPhoEta"			);
    ootPhoPhi_			 = data.GetPtrFloat(	"ootPhoPhi"			);
    ootPhoSCEta_		 = data.GetPtrFloat(	"ootPhoSCEta"			);
    ootPhoSCPhi_		 = data.GetPtrFloat(	"ootPhoSCPhi"			);
    ootPhoHoverE_		 = data.GetPtrFloat(	"ootPhoHoverE"			);
    ootPhoSigmaIEtaIEtaFull5x5_  = data.GetPtrFloat(	"ootPhoSigmaIEtaIEtaFull5x5"	);
    ootPhoR9Full5x5_		 = data.GetPtrFloat(	"ootPhoR9Full5x5"		);
    ootPhoSeedTime_              = data.GetPtrFloat(	"ootPhoSeedTime"		);
    ootPhoSeedEnergy_              = data.GetPtrFloat(	"ootPhoSeedEnergy"		);
    ootPhoMIPTotEnergy_          = data.GetPtrFloat(	"ootPhoMIPTotEnergy"		);
    
    nEle_			 = data.GetInt(		"nEle"				);
    elePt_			 = data.GetPtrFloat(	"elePt"				);
    eleD0_			 = data.GetPtrFloat(	"eleD0"				);
    eleDz_			 = data.GetPtrFloat(	"eleDz"				);
    eleEta_			 = data.GetPtrFloat(	"eleEta"			);
    eleIDbit_			 = data.GetPtrShort(	"eleIDbit"			);
    nMu_			 = data.GetInt(		"nMu"				);
    muPt_ 			 = data.GetPtrFloat(	"muPt"				);
    muIDbit_			 = data.GetPtrInt(	"muIDbit"			);
    muD0_			 = data.GetPtrFloat(	"muD0"				);
    muDz_			 = data.GetPtrFloat(	"muDz"				);
    nJet_			 = data.GetInt(		"nJet"				);
    jetPt_			 = data.GetPtrFloat(	"jetPt"				);
    jetEn_			 = data.GetPtrFloat(	"jetEn"				);
    jetEta_			 = data.GetPtrFloat(	"jetEta"			);
    jetPhi_			 = data.GetPtrFloat(	"jetPhi"			);
    jetPUID_			 = data.GetPtrFloat(	"jetPUID"			);
    jetID_			 = data.GetPtrInt(	"jetID"				);
    jetCEF_			 = data.GetPtrFloat(	"jetCEF"			);
    jetMUF_			 = data.GetPtrFloat(	"jetMUF"			);
    jetCHF_			 = data.GetPtrFloat(	"jetCHF"			);
    jetNHF_			 = data.GetPtrFloat(	"jetNHF"			);
    jetNEF_			 = data.GetPtrFloat(	"jetNEF"			);
    jetNCH_			 = data.GetPtrInt(	"jetNCH"			);
    jetNNP_			 = data.GetPtrInt(	"jetNNP"			);
    
    //cout << "get all variable" << endl;
    //init variable
    run		 = 0;
    event	 = 0;
    isData	 = 0;
    nVtx	 = 0;
    nGoodVtx	 = 0;
    isPVGood	 = 0;
    rho		 = 0;
    EventWeight	 = 1;
    pfMET	 = 0;
    pfMETPhi	 = 0;
    corrMET	 = 0;
    corrMETPhi	 = 0;
    metFilters	 = 0;
    failedMET	 = 0;
    nPho	 = 0;
    phoE.clear();
    phoEt.clear();
    phoEta.clear();
    phoPhi.clear();
    phoCalibE.clear();
    phoCalibEt.clear();
    phoSCEta.clear();
    phoSCPhi.clear();
    phohasPixelSeed.clear();
    phoEleVeto.clear();
    phoHoverE.clear();
    phoSigmaIEtaIEtaFull5x5.clear();
    phoR9Full5x5.clear();
    phoPFChIso.clear();
    phoPFPhoIso.clear();
    phoPFNeuIso.clear();
    phoPFChWorstIso.clear();
    phoPFChIso_rc.clear();
    phoPFPhoIso_rc.clear();
    phoPFNeuIso_rc.clear();
    phoPFChWorstIso_rc.clear();
    phoPFChWorstIso_newEA.clear();
    phoFiredSingleTrgs.clear();
    phoSeedTime.clear();
    phoSeedEnergy.clear();
    phoMIPTotEnergy.clear();
    phoMETdPhi.clear();
    phoIDbit.clear();
    nOOTPho	 = 0;
    ootPhoE.clear();
    ootPhoEt.clear();
    ootPhoEta.clear();
    ootPhoPhi.clear();
    ootPhoSCEta.clear();
    ootPhoSCPhi.clear();
    ootPhoHoverE.clear();
    ootPhoSigmaIEtaIEtaFull5x5.clear();
    ootPhoR9Full5x5.clear();
    ootPhoSeedTime.clear();
    ootPhoSeedEnergy.clear();
    ootPhoMIPTotEnergy.clear();
    ootBeamHaloID.clear();
    ootSpikeID.clear();
    
    MphoID.clear();
    cutflowSMID.clear();
    cutflowVBSID.clear();
    cutflowSMfixMETID.clear();
    cutflowVBSfixMETID.clear();
    nEle	 = 0;
    elePt.clear();
    eleD0.clear();
    eleDz.clear();
    eleEta.clear();
    eleIDbit.clear();
    nMu		 = 0;
    muPt.clear();
    muIDbit.clear();
    muD0.clear();
    muDz.clear();
    nLep	 = 0;
    nJet	 = 0;
    npfjet	 = 0;
    nSMjet	 = 0;
    nVBSjet	 = 0;
    nonPUjetid.clear();
    jetPt.clear();
    jetEn.clear();
    jetEta.clear();
    jetPhi.clear();
    dijetMass	 = 0.0;
    jetPUIDMVA.clear();
    jetPUID.clear();
    jetID.clear();
    jetTID.clear();
    jetCEF.clear();
    jetMUF.clear();
    jetCHF.clear();
    jetNHF.clear();
    jetNEF.clear();
    jetNCH.clear();
    jetNNP.clear();
    minJMETdPhi	 = 0;
    JMETdPhi.clear();
    jetjetdR	 = 0;
    jetjetdEta	 = 0;
    jetjetdPhi	 = 0;
    phojetdR.clear();
    phojetdEta.clear();
    phojetdPhi.clear();


    //rho correction of pho isolation
    vector<vector<Float_t>> Iso_rc; //[ch, pho, nh, chw, chw_newEA]
    Iso_rc.clear();
    for(Int_t iso = 0; iso < 5; iso++){
      vector <Float_t> isolist;
      rhoCorrection(iso, data, isolist);
      Iso_rc.push_back(isolist);
    }

    for(Int_t ipho=0; ipho<nPho_; ipho++){
      if(year == 2016 && (phoFiredSingleTrgs_[ipho]>>7&1) == 0) continue;
      else if(year == 2017 && (phoFiredSingleTrgs_[ipho]>>8&1) == 0) continue;
      else if(year == 2018 && (phoFiredSingleTrgs_[ipho]>>7&1) == 0) continue;
      nPho++;
      phoE.push_back(phoE_[ipho]);
      phoEt.push_back(phoEt_[ipho]);
      phoEta.push_back(phoEta_[ipho]);
      phoPhi.push_back(phoPhi_[ipho]);
      phoCalibE.push_back(phoCalibE_[ipho]);
      phoCalibEt.push_back(phoCalibEt_[ipho]);
      phoSCEta.push_back(phoSCEta_[ipho]);
      phoSCPhi.push_back(phoSCPhi_[ipho]);
      phohasPixelSeed.push_back(phohasPixelSeed_[ipho]);
      phoEleVeto.push_back(phoEleVeto_[ipho]);
      phoHoverE.push_back(phoHoverE_[ipho]);
      phoSigmaIEtaIEtaFull5x5.push_back(phoSigmaIEtaIEtaFull5x5_[ipho]);
      phoR9Full5x5.push_back(phoR9Full5x5_[ipho]);
      phoPFChIso.push_back(phoPFChIso_[ipho]);
      phoPFPhoIso.push_back(phoPFPhoIso_[ipho]);
      phoPFNeuIso.push_back(phoPFNeuIso_[ipho]);
      phoPFChWorstIso.push_back(phoPFChWorstIso_[ipho]);
      phoFiredSingleTrgs.push_back(phoFiredSingleTrgs_[ipho]);
      phoSeedTime.push_back(phoSeedTime_[ipho]);
      phoSeedEnergy.push_back(phoSeedEnergy_[ipho]);
      phoMIPTotEnergy.push_back(phoMIPTotEnergy_[ipho]);
      phoIDbit.push_back(phoIDbit_[ipho]);

      phoPFChIso_rc.push_back(Iso_rc[0][ipho]);
      phoPFPhoIso_rc.push_back(Iso_rc[1][ipho]);
      phoPFNeuIso_rc.push_back(Iso_rc[2][ipho]);
      phoPFChWorstIso_rc.push_back(Iso_rc[3][ipho]);
      phoPFChWorstIso_newEA.push_back(Iso_rc[4][ipho]);
    }
    //cout << "get rho corr" << endl;
    if(nPho_<1) continue;
    
    //lepton veto selection
    Int_t nLele = 0;
    for(Int_t iele=0; iele<nEle_; iele++){
      if((eleIDbit_[iele]>>1&1) == 0 && elePt_[iele] <10) continue;//pass loose
      if(eleD0_[iele] >0.5 && fabs(eleEta_[iele]) <1.4442) continue;
      else if(eleD0_[iele] > 1.0 && fabs(eleEta_[iele]) > 1.5666) continue;
      if(eleDz_[iele] >1.0 && fabs(eleEta_[iele]) <1.4442) continue;
      else if(eleDz_[iele] >2.0 && fabs(eleEta_[iele]) >1.5666) continue;
      nLele++;
    }
    
    Int_t nLmu = 0;
    for(Int_t imu=0; imu<nMu_; imu++){
      if((muIDbit_[imu]>>0&1) == 0 && muPt_[imu] <10) continue;
      if(muD0_[imu] > 2) continue;
      if(muDz_[imu] > 5) continue;
      nLmu++;
    }
    Int_t nlep = 0;
    nLep = nLele + nLmu;
    //cout << "get lepton veto" << endl;
    
    //MET XY corr
    //METXYshift(year, pathes, data, corrMET, corrMETPhi);
    //cout << "get MET corr" << endl;
    vector<Float_t> corrXY; corrXY.clear();
    //METXYshift(year, pathes, data, corrXY);
    METXYshift(year, pathes, data, corrXY);
    corrMET = corrXY[0];
    corrMETPhi = Phi_mpi_pi(corrXY[1]);
    
    //jetID selection TLepVeto
    jet_TIDsel(year, data, jetTID);
    jet_PUIDsel(data, jetPUIDMVA);
    
    //count # jet
    minJMETdPhi = 4;
    for(Int_t i=0; i<nJet_; i++){
      //if(year == 2017 && jetPt_[i]<50 && fabs(jetEta_[i])>2.56 && fabs(jetEta_[i])<3.139) jetPt_[i] = 0;//remove 2017 ecal noise
      if(fabs(deltaPhi(jetPhi_[i], corrMETPhi)) < minJMETdPhi) minJMETdPhi = fabs(deltaPhi(jetPhi_[i], corrMETPhi));
      //if((jetPUIDMVA[i] == 1) && (jetPt_[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1)){// UL data with PU
      if((jetPt_[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1)){// UL data no PU
	nonPUjetid.push_back(i);
	npfjet++;
      }
    }
    //cout << "get # jets" << endl;
    
    //phoID selection
    phoIDcut(1, year, data, MphoID);
    //cout << "get phoIDcut" << endl;
    
    Int_t bit=0; Int_t fixbit=0;
    TLorentzVector  leadjetP4, subjetP4, dijetP4;
    if(npfjet >= 2){
      jetjetdEta = deltaEta(jetEta_[nonPUjetid[0]], jetEta_[nonPUjetid[1]]);
      jetjetdPhi = deltaPhi(jetPhi_[nonPUjetid[0]], jetPhi_[nonPUjetid[1]]);
      jetjetdR = deltaR(jetEta_[nonPUjetid[0]], jetPhi_[nonPUjetid[0]], jetEta_[nonPUjetid[1]], jetPhi_[nonPUjetid[1]]);

      Int_t ilead = nonPUjetid[0];
      leadjetP4.SetPtEtaPhiE(jetPt_[ilead], jetEta_[ilead], jetPhi_[ilead], jetEn_[ilead]);
      Int_t isub = nonPUjetid[1];
      subjetP4.SetPtEtaPhiE(jetPt_[isub], jetEta_[isub], jetPhi_[isub], jetEn_[isub]);
      
      dijetP4 = leadjetP4 + subjetP4;
      dijetMass = dijetP4.M();

      for(Int_t ipho=0; ipho<nPho; ipho++){
	//if(ipho>0) continue;
	phojetdR.push_back(deltaR(phoEta[ipho], phoPhi[ipho], jetEta_[nonPUjetid[0]], jetPhi_[nonPUjetid[0]]));
	phojetdR.push_back(deltaR(phoEta[ipho], phoPhi[ipho], jetEta_[nonPUjetid[1]], jetPhi_[nonPUjetid[1]]));

	phojetdEta.push_back(fabs(deltaEta(phoEta[ipho], jetEta_[nonPUjetid[0]])));
	phojetdEta.push_back(fabs(deltaEta(phoEta[ipho], jetEta_[nonPUjetid[1]])));

	phojetdPhi.push_back(fabs(deltaPhi(phoPhi[ipho], jetPhi_[nonPUjetid[0]])));
	phojetdPhi.push_back(fabs(deltaPhi(phoPhi[ipho], jetPhi_[nonPUjetid[1]])));
      }

      JMETdPhi.push_back(deltaPhi(jetPhi_[nonPUjetid[0]], corrMETPhi));
      JMETdPhi.push_back(deltaPhi(jetPhi_[nonPUjetid[1]], corrMETPhi));
    }
    
    else{
      jetjetdEta = -999;
      jetjetdPhi = -999;
      jetjetdR = -999;
      
      for(Int_t ipho=0; ipho<nPho; ipho++){
	//if(ipho>0) continue;
	phojetdR.push_back(-999);
	phojetdEta.push_back(-999);
	phojetdPhi.push_back(-999);
      }
      
      for(Int_t jj=0; jj<npfjet; jj++){
	Int_t ijet = nonPUjetid[jj];
 	JMETdPhi.push_back(deltaPhi(jetPhi_[ijet], corrMETPhi));
      }
    }
    
    for(Int_t ipho=0; ipho<nPho; ipho++){
      //if(ipho > 0) continue;
      phoMETdPhi.push_back(deltaPhi(phoPhi[ipho], corrMETPhi));
    }

    //cutflow setbit
    //cout << "get Tlorentz" << endl;
    
    for(Int_t ipho=0; ipho<nPho; ipho++){
      //if(ipho > 0) continue;
      bit=0;
      fixbit=0;
									bit = SetBit(0, bit);
      if(phohasPixelSeed[ipho] == 0)					bit = SetBit(1, bit);
      if(fabs(phoSCEta[ipho]) < 1.4442)					bit = SetBit(2, bit);
      if(phoCalibEt[ipho] > 210)					bit = SetBit(3, bit);
      if(MphoID[ipho] == 1)						bit = SetBit(4, bit);
      //if(Iso_rc[4][ipho] < 1.141)					bit = SetBit(5, bit);
      if(fabs(phoSeedTime[ipho])<3)					bit = SetBit(5, bit);
      if(nLep<1)							bit = SetBit(6, bit);
      //if(failedMET>0)							bit = SetBit(8, bit);
      //(corrMET> 140+(0.138*(phoCalibEt[ipho]-500)))			bit = SetBit(8, bit);
      if(corrMET>100)							bit = SetBit(7, bit);
      if(fabs(deltaPhi(phoPhi[ipho], corrMETPhi)) > 0.8)		bit = SetBit(8, bit);
      if(minJMETdPhi   > 0.5)						bit = SetBit(9, bit);
      
      bit = SetBit(10, bit); bit = SetBit(11, bit); bit = SetBit(12, bit); bit = SetBit(13, bit);
      if(npfjet <=1){                                           bit = SetBit(14, bit); nSMjet = npfjet;}
      
      
      //fixbit = bit;
      //fixbit = ResetBit(8, fixbit);
      //if(corrMET>100) fixbit = SetBit(8, fixbit);
      //cutflowSMfixMETID.push_back(fixbit);
      cutflowSMID.push_back(bit);
      //cout << "get SM cut" << endl;

      bit = ResetBit(10, bit); bit = ResetBit(11, bit); bit = ResetBit(12, bit); bit = ResetBit(13, bit); bit = ResetBit(14, bit);
      
      if(phojetdR[0]>0.4 && phojetdR[1]>0.4)	bit = SetBit(10, bit);
      if(jetjetdEta > 2.5)			bit = SetBit(11, bit);
      if(jetjetdR >0.5)				bit = SetBit(12, bit);
      if(dijetMass>250)				bit = SetBit(13, bit);
      if(npfjet >=2){				bit = SetBit(14, bit); nVBSjet = npfjet;}
      
      //      if(npfjet >=2 && jetjetdEta > 2.5 && (dijetMass>250) && (phojetdR[0] > 0.4) && (phojetdR[1] > 0.4) && (jetjetdR > 0.5)){ bit = SetBit(11, bit); nVBSjet = npfjet;}

      
      //fixbit = bit;
      //fixbit = ResetBit(8, fixbit);
      //if(corrMET>80) fixbit = SetBit(8, fixbit);
      //cutflowVBSfixMETID.push_back(fixbit);

      cutflowVBSID.push_back(bit);
      //cout << "get VBS cut" << endl;
    }
    //cout << "get setbit" << endl;
    /*
    Int_t passBH = 0;
    Int_t passSpike = 0;
    for(Int_t ipho =0; ipho<nOOTPho; ipho++){
      passBH = 0; passSpike = 0;
      //oot beam halo
      if(ootPhoEt[ipho] > 210 && (corrMET > 140+(0.138*(phoCalibEt[ipho]-500))) && (ootPhoMIPTotEnergy[ipho] > 4.9) && ((metFilters_>>3&1) == 0)) passBH = 1;
      ootBeamHaloID.push_back(passBH);

      //oot spike
      if(ootPhoEt[ipho] > 210 && (corrMET > 140+(0.138*(phoCalibEt[ipho]-500))) && (ootPhoSigmaIEtaIEtaFull5x5[ipho] < 0.001)) passSpike = 1;
      ootSpikeID.push_back(passSpike);
    }
    */

    //fill tree
    run = run_;
    event = event_;
    isData = isData_;
    nVtx = nVtx_;
    nGoodVtx = nGoodVtx_;
    isPVGood = isPVGood_;
    rho = rho_;
    pfMET = pfMET_;
    pfMETPhi = pfMETPhi_;
    metFilters = metFilters_;

    nOOTPho = nOOTPho_;
    for(Int_t i=0; i<nOOTPho_; i++){
      ootPhoE.push_back(ootPhoE_[i]);
      ootPhoEt.push_back(ootPhoEt_[i]);
      ootPhoEta.push_back(ootPhoEta_[i]);
      ootPhoPhi.push_back(ootPhoPhi_[i]);
      ootPhoSCEta.push_back(ootPhoSCEta_[i]);
      ootPhoSCPhi.push_back(ootPhoSCPhi_[i]);
      ootPhoHoverE.push_back(ootPhoHoverE_[i]);
      ootPhoSigmaIEtaIEtaFull5x5.push_back(ootPhoSigmaIEtaIEtaFull5x5_[i]);
      ootPhoR9Full5x5.push_back(ootPhoR9Full5x5_[i]);
      ootPhoSeedTime.push_back(ootPhoSeedTime_[i]);
      ootPhoSeedEnergy.push_back(ootPhoSeedEnergy_[i]);
      ootPhoMIPTotEnergy.push_back(ootPhoMIPTotEnergy_[i]);
    }

    nEle = nEle_;
    for(Int_t i=0; i<nEle_; i++){
      elePt.push_back(elePt_[i]);
      eleD0.push_back(eleD0_[i]);
      eleDz.push_back(eleDz_[i]);
      eleEta.push_back(eleEta_[i]);
      eleIDbit.push_back(eleIDbit_[i]);
    }
    //cout << "fill ele" << endl;

    nMu = nMu_;
    for(Int_t i=0; i<nMu_; i++){
      muPt.push_back(muPt_[i]);
      muIDbit.push_back(muIDbit_[i]);
      muD0.push_back(muD0_[i]);
      muDz.push_back(muDz_[i]);
    }
    //cout << "fill mu" << endl;    

    nJet = nJet_;
    for(Int_t i=0; i<nJet_; i++){
      jetPt.push_back(jetPt_[i]);
      jetEn.push_back(jetEn_[i]);
      jetEta.push_back(jetEta_[i]);
      jetPhi.push_back(jetPhi_[i]);
      jetPUID.push_back(jetPUID_[i]);
      //tID.push_back(jetID_[i]);
      jetCEF.push_back(jetCEF_[i]);
      jetMUF.push_back(jetMUF_[i]);
      jetCHF.push_back(jetCHF_[i]);
      jetNHF.push_back(jetNHF_[i]);
      jetNEF.push_back(jetNEF_[i]);
      jetNCH.push_back(jetNCH_[i]);
      jetNNP.push_back(jetNNP_[i]);
    }
    //cout << "fill jet" << endl;
    outtree_->Fill();
    //cout << "fill tree " << endl;
  }
  //****************END LOOP**********************//

  //****************Terminate*********************//
  fout_->cd();
  outtree_->Write();

  hEvents->Write();
  
  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());
}
