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
#include "ElectronSelection.h"
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
  ///global event
  Int_t         run_;
  Long64_t      event_;
  Bool_t        isData_;
  Int_t         nVtx_;
  Int_t         nGoodVtx_;
  Bool_t        isPVGood_;
  Float_t       rho_;
  ///MET
  Float_t       pfMET_;
  Float_t       pfMETPhi_;
  Float_t puppiMET_;
  Float_t puppiMETPhi_;
  Int_t         metFilters_;
  ///photon
  Int_t         nPho_;
  Float_t*      phoE_;
  Float_t*      phoEt_;
  Float_t*      phoEta_;
  Float_t*      phoPhi_;
  Float_t*      phoCalibE_;
  Float_t*      phoCalibEt_;
  Float_t*      phoSCEta_;
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
  Float_t* phoEtop_;
  Float_t* phoEleft_;
  Float_t* phoEright_;
  Float_t* phoEbottom_;
  
  ///OOT photon
  Int_t         nOOTPho_;
  Float_t*      ootPhoE_;
  Float_t*      ootPhoEt_;
  Float_t*      ootPhoEta_;
  Float_t*      ootPhoPhi_;
  Float_t*      ootPhoSCEta_;
  Float_t*      ootPhoSCPhi_;
  Int_t* ootPhohasPixelSeed_;
  Float_t*      ootPhoHoverE_;
  Float_t*      ootPhoSigmaIEtaIEtaFull5x5_;
  Float_t*      ootPhoR9Full5x5_;
  Float_t*      ootPhoSeedTime_;
  Float_t*      ootPhoSeedEnergy_;
  Float_t*      ootPhoMIPTotEnergy_;
  Float_t* ootPhoEtop_;
  Float_t* ootPhoEleft_;
  Float_t* ootPhoEright_;
  Float_t* ootPhoEbottom_;
  
  ///electron
  Int_t         nEle_;
  Float_t*      elePt_;
  Float_t*      eleEta_;
  Float_t* elePhi_;
  Float_t* eleEn_;
  Float_t*      eleD0_;
  Float_t*      eleDz_;
  Short_t*      eleIDbit_;
  Long64_t* eleFiredSingleTrgs_;
  Float_t*  eleHoverE_;
  Float_t*  eleSigmaIEtaIEtaFull5x5_;
  Float_t*  eleR9Full5x5_;
  Float_t*  elePFChIso_;
  Float_t*  elePFPhoIso_;
  Float_t*  elePFNeuIso_;
  Int_t*    eleConvVeto_;
  Int_t*    eleMissHits_;
  
  ///muon
  Int_t         nMu_;
  Float_t*      muPt_;
  Float_t*      muEta_;
  Int_t*        muIDbit_;
  Float_t*      muD0_;
  Float_t*      muDz_;
  ///jets
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
  ///global event
  Int_t                 run;
  Long64_t              event;
  Bool_t                isData;
  Int_t                 nVtx;
  Int_t                 nGoodVtx;
  Bool_t                isPVGood;
  Float_t               rho;
  Float_t               EventWeight;
  ///MET
  Float_t               pfMET;
  Float_t               pfMETPhi;
  Float_t               puppiMET;
  Float_t               puppiMETPhi;
  Float_t		corrMET;
  Float_t		corrMETPhi;
  Int_t                 metFilters;
  Int_t                 failedMET;
  ///photon
  Int_t                 nPho;//HLT pho
  vector<Float_t>       phoE;
  vector<Float_t>       phoEt;
  vector<Float_t>       phoEta;
  vector<Float_t>       phoPhi;
  vector<Float_t>       phoCalibE;
  vector<Float_t>       phoCalibEt;
  vector<Float_t>       phoSCEta;
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
  vector<Int_t>         MphoID;
  Float_t phoCentral; //photon centrality
  vector<Float_t> phoEtop;
  vector<Float_t> phoEleft;
  vector<Float_t> phoEright;
  vector<Float_t> phoEbottom;
  vector<Float_t> phoE4;
  vector<Float_t> phoE4overE1;
  vector<Float_t> phoEtaWing;
  vector<Float_t> phoEtaWingoverE1;

  ///OOT photon
  Int_t			nOOTPho;
  vector<Float_t>       ootPhoE;
  vector<Float_t>       ootPhoEt;
  vector<Float_t>       ootPhoEta;
  vector<Float_t>       ootPhoPhi;
  vector<Float_t>       ootPhoSCEta;
  vector<Float_t>       ootPhoSCPhi;
  vector<Int_t> ootPhohasPixelSeed;
  vector<Float_t>       ootPhoHoverE;
  vector<Float_t>       ootPhoSigmaIEtaIEtaFull5x5;
  vector<Float_t>       ootPhoR9Full5x5;
  vector<Float_t>       ootPhoSeedTime;
  vector<Float_t>       ootPhoSeedEnergy;
  vector<Float_t>       ootPhoMIPTotEnergy;
  vector<Float_t> ootPhoEtop;
  vector<Float_t> ootPhoEleft;
  vector<Float_t> ootPhoEright;
  vector<Float_t> ootPhoEbottom;
  vector<Float_t> ootPhoE4;
  vector<Float_t> ootPhoE4overE1;
  vector<Float_t> ootPhoEtaWing;
  vector<Float_t> ootPhoEtaWingoverE1;
  
  ///cutflow ID  
  vector<Int_t>         cutflowSMID;
  vector<Int_t>         cutflowVBSID;
  ///electron
  Int_t                 nEle;
  vector<Float_t>       elePt;
  vector<Float_t>       eleEta;
  vector<Float_t> elePhi;
  vector<Float_t> eleEn;
  vector<Float_t>       eleD0;
  vector<Float_t>       eleDz;
  vector<Short_t>       eleIDbit;
  vector<Long64_t> eleFiredSingleTrgs;
  vector<Float_t>  eleHoverE;
  vector<Float_t>  eleSigmaIEtaIEtaFull5x5;
  vector<Float_t>  eleR9Full5x5;
  vector<Float_t>  elePFChIso;
  vector<Float_t>  elePFPhoIso;
  vector<Float_t>  elePFNeuIso;
  vector<Int_t>    eleConvVeto;
  vector<Int_t>    eleMissHits;
  vector<Int_t> MelephoID;
  vector<Int_t> TeleID;
  ///muon
  Int_t                 nMu;
  vector<Float_t>       muPt;
  vector<Float_t>       muEta;
  vector<Int_t>         muIDbit;
  vector<Float_t>       muD0;
  vector<Float_t>       muDz;
  Int_t                 nLep;
  ///jets
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
  Float_t jet3Central; // 3rd leading jet centrality  
  
  ///angular variables
  Float_t               minJMETdPhi;
  vector<Float_t>       JMETdPhi;
  Float_t               jetjetdR;
  Float_t               jetjetdEta;
  Float_t               jetjetdPhi;
  vector<Float_t>       phojetdR;
  vector<Float_t>       phojetdEta;
  vector<Float_t>       phojetdPhi;
  Float_t jetjetZdPhi;
  Float_t jetjetphodPhi;
  Float_t               ZgsysPhi;
  Float_t               jetjetsysPhi;
    
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
  outtree_->Branch("puppiMET"			,&puppiMET         ,"puppiMET/F"		);
  outtree_->Branch("puppiMETPhi"			,&puppiMETPhi      ,"puppiMETPhi/F"		);
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
  outtree_->Branch("phoSeedEnergy"                ,&phoSeedEnergy				);
  outtree_->Branch("phoMIPTotEnergy"            ,&phoMIPTotEnergy			);
  outtree_->Branch("phoMETdPhi"                 ,&phoMETdPhi				);
  outtree_->Branch("phoIDbit"                   ,&phoIDbit				);
  outtree_->Branch("phoEtop" ,&phoEtop);
  outtree_->Branch("phoEleft" ,&phoEleft);
  outtree_->Branch("phoEright" ,&phoEright);
  outtree_->Branch("phoEbottom" ,&phoEbottom);
  outtree_->Branch("phoE4" ,&phoE4);
  outtree_->Branch("phoE4overE1" ,&phoE4overE1);
  outtree_->Branch("phoEtaWing" ,&phoEtaWing);
  outtree_->Branch("phoEtaWingoverE1" ,&phoEtaWingoverE1);
  
  outtree_->Branch("nOOTPho"			,&nOOTPho       ,"nOOTPho/I"		);
  outtree_->Branch("ootPhoE"                    ,&ootPhoE				);
  outtree_->Branch("ootPhoEt"                   ,&ootPhoEt				);
  outtree_->Branch("ootPhoEta"                  ,&ootPhoEta				);
  outtree_->Branch("ootPhoPhi"                  ,&ootPhoPhi				);
  outtree_->Branch("ootPhoSCEta"                ,&ootPhoSCEta				);
  outtree_->Branch("ootPhoSCPhi"                ,&ootPhoSCPhi				);
  outtree_->Branch("ootPhohasPixelSeed"               ,&ootPhohasPixelSeed				);
  outtree_->Branch("ootPhoHoverE"               ,&ootPhoHoverE				);
  outtree_->Branch("ootPhoSigmaIEtaIEtaFull5x5" ,&ootPhoSigmaIEtaIEtaFull5x5		);
  outtree_->Branch("ootPhoR9Full5x5"            ,&ootPhoR9Full5x5			);
  outtree_->Branch("ootPhoSeedTime"             ,&ootPhoSeedTime			);
  outtree_->Branch("ootPhoSeedEnergy"           ,&ootPhoSeedEnergy			);
  outtree_->Branch("ootPhoMIPTotEnergy"         ,&ootPhoMIPTotEnergy			);
  outtree_->Branch("ootPhoEtop" ,&ootPhoEtop);
  outtree_->Branch("ootPhoEleft" ,&ootPhoEleft);
  outtree_->Branch("ootPhoEright" ,&ootPhoEright);
  outtree_->Branch("ootPhoEbottom" ,&ootPhoEbottom);
  outtree_->Branch("ootPhoE4" ,&ootPhoE4);
  outtree_->Branch("ootPhoE4overE1" ,&ootPhoE4overE1);
  outtree_->Branch("ootPhoEtaWing" ,&ootPhoEtaWing);
  outtree_->Branch("ootPhoEtaWingoverE1" ,&ootPhoEtaWingoverE1);
  
  outtree_->Branch("MphoID"                     ,&MphoID				);
  outtree_->Branch("phoCentral",&phoCentral ,"phoCentral/F");
  outtree_->Branch("cutflowSMID"                ,&cutflowSMID				);
  outtree_->Branch("cutflowVBSID"               ,&cutflowVBSID				);
  outtree_->Branch("nEle"			,&nEle          ,"nEle/I"		);
  outtree_->Branch("elePt"                      ,&elePt					);
  outtree_->Branch("eleEta"                     ,&eleEta				);
  outtree_->Branch("elePhi"                     ,&elePhi				);
  outtree_->Branch("eleEn"                     ,&eleEn				);
  outtree_->Branch("eleD0"                      ,&eleD0					);
  outtree_->Branch("eleDz"                      ,&eleDz					);
  
  outtree_->Branch("eleIDbit"                   ,&eleIDbit				);
  outtree_->Branch("eleFiredSingleTrgs" ,&eleFiredSingleTrgs);
  outtree_->Branch("eleHoverE" ,&eleHoverE);
  outtree_->Branch("eleSigmaIEtaIEtaFull5x5",&eleSigmaIEtaIEtaFull5x5);
  outtree_->Branch("eleR9Full5x5",&eleR9Full5x5);
  outtree_->Branch("elePFChIso",&elePFChIso);
  outtree_->Branch("elePFPhoIso",&elePFPhoIso);
  outtree_->Branch("elePFNeuIso",&elePFNeuIso);
  outtree_->Branch("eleConvVeto",&eleConvVeto);
  outtree_->Branch("eleMissHits",&eleMissHits);
  outtree_->Branch("MelephoID",&MelephoID);
  outtree_->Branch("TeleID",&TeleID);
  outtree_->Branch("nMu"			,&nMu           ,"nMu/I"		);
  outtree_->Branch("muPt"                       ,&muPt					);
  outtree_->Branch("muEta" ,&muEta);
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
  outtree_->Branch("jetjetZdPhi"		,&jetjetZdPhi	,"jetjetZdPhi/F"	);
  outtree_->Branch("jetjetphodPhi"		,&jetjetphodPhi	,"jetjetphodPhi/F"	);
  outtree_->Branch("ZgsysPhi" ,&ZgsysPhi ,"ZgsysPhi/F");
  outtree_->Branch("jetjetsysPhi" ,&jetjetsysPhi ,"jetjetsysPhi/F");
  outtree_->Branch("jet3Central",&jet3Central ,"jet3Central/F");
  
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
    puppiMET_			 = data.GetFloat(	"puppiMET"				);
    puppiMETPhi_			 = data.GetFloat(	"puppiMETPhi"			);
    metFilters_		         = data.GetInt(		"metFilters"			);
    nPho_			 = data.GetInt(		"nPho"				);
    phoE_			 = data.GetPtrFloat(	"phoE"				);
    phoEt_			 = data.GetPtrFloat(	"phoEt"				);
    phoEta_			 = data.GetPtrFloat(	"phoEta"			);
    phoPhi_			 = data.GetPtrFloat(	"phoPhi"			);
    phoCalibE_			 = data.GetPtrFloat(	"phoCalibE"			);
    phoCalibEt_		         = data.GetPtrFloat(	"phoCalibEt"			);
    phoSCEta_			 = data.GetPtrFloat(	"phoSCEta"			);
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
    phoEtop_ = data.GetPtrFloat("phoEtop");
    phoEleft_ = data.GetPtrFloat("phoEleft");
    phoEright_ = data.GetPtrFloat("phoEright");
    phoEbottom_ = data.GetPtrFloat("phoEbottom");
    
    nOOTPho_			 = data.GetInt(		"nOOTPho"			);
    ootPhoE_			 = data.GetPtrFloat(	"ootPhoE"			);
    ootPhoEt_			 = data.GetPtrFloat(	"ootPhoEt"			);
    ootPhoEta_			 = data.GetPtrFloat(	"ootPhoEta"			);
    ootPhoPhi_			 = data.GetPtrFloat(	"ootPhoPhi"			);
    ootPhoSCEta_		 = data.GetPtrFloat(	"ootPhoSCEta"			);
    ootPhoSCPhi_		 = data.GetPtrFloat(	"ootPhoSCPhi"			);
    ootPhohasPixelSeed_ = data.GetPtrInt("ootPhohasPixelSeed");
    ootPhoHoverE_		 = data.GetPtrFloat(	"ootPhoHoverE"			);
    ootPhoSigmaIEtaIEtaFull5x5_  = data.GetPtrFloat(	"ootPhoSigmaIEtaIEtaFull5x5"	);
    ootPhoR9Full5x5_		 = data.GetPtrFloat(	"ootPhoR9Full5x5"		);
    ootPhoSeedTime_              = data.GetPtrFloat(	"ootPhoSeedTime"		);
    ootPhoSeedEnergy_              = data.GetPtrFloat(	"ootPhoSeedEnergy"		);
    ootPhoMIPTotEnergy_          = data.GetPtrFloat(	"ootPhoMIPTotEnergy"		);
    ootPhoEtop_ = data.GetPtrFloat("ootPhoEtop");
    ootPhoEleft_ = data.GetPtrFloat("ootPhoEleft");
    ootPhoEright_ = data.GetPtrFloat("ootPhoEright");
    ootPhoEbottom_ = data.GetPtrFloat("ootPhoEbottom");
    
    nEle_			 = data.GetInt(		"nEle"				);
    elePt_			 = data.GetPtrFloat(	"elePt"				);
    eleEta_			 = data.GetPtrFloat(	"eleEta"			);
    //elePhi_ = data.GetPtrFloat("elePhi");
    //eleEn_ = data.GetPtrFloat("eleEn");
    eleD0_			 = data.GetPtrFloat(	"eleD0"				);
    eleDz_			 = data.GetPtrFloat(	"eleDz"				);
    eleIDbit_			 = data.GetPtrShort(	"eleIDbit"			);
    //eleFiredSingleTrgs_ = data.GetPtrLong64("eleFiredSingleTrgs");
    //eleHoverE_ = data.GetPtrFloat("eleHoverE");
    //eleSigmaIEtaIEtaFull5x5_ = data.GetPtrFloat("eleSigmaIEtaIEtaFull5x5");
    //eleR9Full5x5_ = data.GetPtrFloat("eleR9Full5x5");
    //elePFChIso_ = data.GetPtrFloat("elePFChIso");
    //elePFPhoIso_ = data.GetPtrFloat("elePFPhoIso");
    //elePFNeuIso_ = data.GetPtrFloat("elePFNeuIso");
    //eleConvVeto_ = data.GetPtrInt("eleConvVeto");
    //eleMissHits_ = data.GetPtrInt("eleMissHits");
  
    nMu_			 = data.GetInt(		"nMu"				);
    muPt_ 			 = data.GetPtrFloat(	"muPt"				);
    muEta_ = data.GetPtrFloat("muEta");
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
    puppiMET	 = 0;
    puppiMETPhi	 = 0;
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
    MphoID.clear();
    phoCentral = 0;
    phoEtop.clear();
    phoEleft.clear();
    phoEright.clear();
    phoEbottom.clear();
    phoE4.clear();
    phoE4overE1.clear();
    phoEtaWing.clear();
    phoEtaWingoverE1.clear();
    
    nOOTPho	 = 0;
    ootPhoE.clear();
    ootPhoEt.clear();
    ootPhoEta.clear();
    ootPhoPhi.clear();
    ootPhoSCEta.clear();
    ootPhoSCPhi.clear();
    ootPhohasPixelSeed.clear();
    ootPhoHoverE.clear();
    ootPhoSigmaIEtaIEtaFull5x5.clear();
    ootPhoR9Full5x5.clear();
    ootPhoSeedTime.clear();
    ootPhoSeedEnergy.clear();
    ootPhoMIPTotEnergy.clear();
    ootPhoEtop.clear();
    ootPhoEleft.clear();
    ootPhoEright.clear();
    ootPhoEbottom.clear();
    ootPhoE4.clear();
    ootPhoE4overE1.clear();
    ootPhoEtaWing.clear();
    ootPhoEtaWingoverE1.clear();
    
    cutflowSMID.clear();
    cutflowVBSID.clear();
    
    nEle	 = 0;
    elePt.clear();
    eleEta.clear();
    elePhi.clear();
    eleEn.clear();
    eleD0.clear();
    eleDz.clear();
    
    eleIDbit.clear();
    eleFiredSingleTrgs.clear();
    eleHoverE.clear();
    eleSigmaIEtaIEtaFull5x5.clear();
    eleR9Full5x5.clear();
    elePFChIso.clear();
    elePFPhoIso.clear();
    elePFNeuIso.clear();
    eleConvVeto.clear();
    eleMissHits.clear();
    MelephoID.clear();
    TeleID.clear();
    
    nMu		 = 0;
    muPt.clear();
    muEta.clear();
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
    jet3Central = 0;
    minJMETdPhi	 = 0;
    JMETdPhi.clear();
    jetjetdR	 = 0;
    jetjetdEta	 = 0;
    jetjetdPhi	 = 0;
    phojetdR.clear();
    phojetdEta.clear();
    phojetdPhi.clear();
    jetjetZdPhi	= 0;
    jetjetphodPhi	= 0;
    ZgsysPhi = 0;
    jetjetsysPhi = 0;
    
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
      phoEtop.push_back(phoEtop_[ipho]);
      phoEleft.push_back(phoEleft_[ipho]);
      phoEright.push_back(phoEright_[ipho]);
      phoEbottom.push_back(phoEbottom_[ipho]);
      
      phoPFChIso_rc.push_back(Iso_rc[0][ipho]);
      phoPFPhoIso_rc.push_back(Iso_rc[1][ipho]);
      phoPFNeuIso_rc.push_back(Iso_rc[2][ipho]);
      phoPFChWorstIso_rc.push_back(Iso_rc[3][ipho]);
      phoPFChWorstIso_newEA.push_back(Iso_rc[4][ipho]);

      Float_t sumphoE4 = 0;
      sumphoE4 = phoEtop_[ipho]+phoEleft_[ipho]+phoEright_[ipho]+phoEbottom_[ipho];
      phoE4.push_back(sumphoE4);

      Float_t E4overE = 0;
      E4overE = sumphoE4/phoSeedEnergy_[ipho];
      phoE4overE1.push_back(E4overE);
      
      Float_t compWing = 0;
      if(phoEleft_[ipho] > phoEright_[ipho]) compWing = phoEleft_[ipho];
      else compWing = phoEright_[ipho];
      phoEtaWing.push_back(compWing);
      
      Float_t WingoverE = 0;
      WingoverE = compWing/phoSeedEnergy_[ipho];
      phoEtaWingoverE1.push_back(WingoverE);
    }
    //cout << "get rho corr" << endl;
    if(nPho<1) continue;

    //electron t&p ID(pass HLT)
    //epho_IDselection(1, year, 0, data, MelephoID);
    //eleIDcut(2, year, data, TeleID);
    
    //lepton veto selection
    Int_t nLele = 0;
    for(Int_t iele=0; iele<nEle_; iele++){
      
      /// tighter ele selection ///
      if((eleIDbit_[iele]>>1&1) == 1 && elePt_[iele] > 10 && fabs(eleEta_[iele]) < 2.5) {
      	if(fabs(eleEta_[iele]) <1.4442 && eleD0_[iele] < 0.5 && eleDz_[iele] < 1.0) nLele++;
      	else if(fabs(eleEta_[iele]) > 1.5666 && eleD0_[iele] < 1.0 && eleDz_[iele] < 2.0) nLele++;
      }
    }
    
    Int_t nLmu = 0;
    for(Int_t imu=0; imu<nMu_; imu++){

      /// tighter mu selection///
      if((muIDbit_[imu]>>0&1) == 1 && muPt_[imu] > 10 && fabs(muEta_[imu]) < 2.4){
      	if(muD0_[imu] < 2 && muDz_[imu] < 5) nLmu++;
      }
    }
    Int_t nlep = 0;
    nLep = nLele + nLmu;
    //cout << "get lepton veto" << endl;
    
    //MET XY corr
    vector<Float_t> corrXY; corrXY.clear();
    METXYshift(year, pathes, data, corrXY);
    corrMET = corrXY[0];
    corrMETPhi = Phi_mpi_pi(corrXY[1]);
    //cout << "get MET corr" << endl;
    puppiMET = puppiMET_;
    puppiMETPhi = puppiMETPhi_;
    
    //jetID selection TLepVeto
    jet_ULTIDsel(year, data, jetTID);
    jet_ULPUIDsel(year, data, jetPUIDMVA);
    
    //count # jet
    for(Int_t i=0; i<nJet_; i++){
      //if(year == 2017 && jetPt_[i]<50 && fabs(jetEta_[i])>2.56 && fabs(jetEta_[i])<3.139) jetPt_[i] = 0;//remove 2017 ecal noise
      //if(npfjet>=2){
      //	//if((jetPUIDMVA[i] == 1) && (jetPt_[i] < 25) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1)){// UL data with PU
      //	if((jetPUIDMVA[i] == 1) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1)){// UL data with PU and no pT cut
      //	  //if((jetPt_[i] > 25) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1)){// UL data no PU
      //	  nonPUjetid.push_back(i);
      //	  npfjet++;
      //	}
      //}
      //else{
      //	if((jetPUIDMVA[i] == 1) && (jetPt_[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1)){// UL data with PU
      //	  //if((jetPt_[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1)){// UL data no PU
      //	  nonPUjetid.push_back(i);
      //	  npfjet++;
      //	}
      //}
      
      if((jetPUIDMVA[i] == 1) && (jetPt_[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1)){// UL data with PU
	nonPUjetid.push_back(i);
	npfjet++;
      }
    }
    //cout << "get # jets" << endl;
    
    //phoID selection
    phoIDcut(1, year, data, MphoID);
    //cout << "get phoIDcut" << endl;

    //dijet stuff
    Int_t bit=0; Int_t fixbit=0;
    TLorentzVector  leadjetP4, subjetP4, dijetP4;
    
    if(npfjet >= 2){
      Int_t ilead = nonPUjetid[0];
      Int_t isub = nonPUjetid[1];

      //mindjetMETPhi
      if(fabs(deltaPhi(jetPhi_[ilead], puppiMETPhi)) < fabs(deltaPhi(jetPhi_[isub], puppiMETPhi))) minJMETdPhi = deltaPhi(jetPhi_[ilead], puppiMETPhi);
      else if(fabs(deltaPhi(jetPhi_[ilead], puppiMETPhi)) > fabs(deltaPhi(jetPhi_[isub], puppiMETPhi))) minJMETdPhi = deltaPhi(jetPhi_[isub], puppiMETPhi);

      //dijet dEta & dPhi & dR
      jetjetdEta = deltaEta(jetEta_[ilead], jetEta_[isub]);
      jetjetdPhi = deltaPhi(jetPhi_[ilead], jetPhi_[isub]);
      jetjetdR = deltaR(jetEta_[ilead], jetPhi_[ilead], jetEta_[isub], jetPhi_[isub]);

      //dijet Mass
      leadjetP4.SetPtEtaPhiE(jetPt_[ilead], jetEta_[ilead], jetPhi_[ilead], jetEn_[ilead]);
      subjetP4.SetPtEtaPhiE(jetPt_[isub], jetEta_[isub], jetPhi_[isub], jetEn_[isub]);
      dijetP4 = leadjetP4 + subjetP4;
      dijetMass = dijetP4.M();

      //phojet dEta & dPhi & dR
      for(Int_t ipho=0; ipho<nPho; ipho++){
	if(ipho>0) continue;
	phojetdR.push_back(deltaR(phoEta[ipho], phoPhi[ipho], jetEta_[ilead], jetPhi_[ilead]));
	phojetdR.push_back(deltaR(phoEta[ipho], phoPhi[ipho], jetEta_[isub], jetPhi_[isub]));

	phojetdEta.push_back(deltaEta(phoEta[ipho], jetEta_[ilead]));
	phojetdEta.push_back(deltaEta(phoEta[ipho], jetEta_[isub]));

	phojetdPhi.push_back(deltaPhi(phoPhi[ipho], jetPhi_[ilead]));
	phojetdPhi.push_back(deltaPhi(phoPhi[ipho], jetPhi_[isub]));
      }

      //jetMET dPhi
      JMETdPhi.push_back(deltaPhi(jetPhi_[ilead], puppiMETPhi));
      JMETdPhi.push_back(deltaPhi(jetPhi_[isub], puppiMETPhi));
    }
    
    else{
      minJMETdPhi = -999;
      
      jetjetdEta = -999;
      jetjetdPhi = -999;
      jetjetdR = -999;
      
      for(Int_t i=0; i<2; i++){
	//if(ipho>0) continue;
	phojetdR.push_back(-999);
	phojetdEta.push_back(-999);
	phojetdPhi.push_back(-999);
      }
      
      JMETdPhi.push_back(-999);
	
    }
    //cout << "get Tlorentz" << endl;

    //phoMET dPhi
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(ipho > 0) continue;
      phoMETdPhi.push_back(deltaPhi(phoPhi[ipho], puppiMETPhi));
    }

    //photon centrality
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(ipho>0) continue;
      if(npfjet<2) continue;
      Int_t ilead = nonPUjetid[0];
      Int_t isub = nonPUjetid[1];
      
      Float_t j1mj2_Eta = jetEta_[ilead] - jetEta_[isub];
      Float_t j1pj2_Eta = jetEta_[ilead] + jetEta_[isub];
      Float_t x = -4.0 * TMath::Power((phoEta[ipho]-(j1pj2_Eta/2)),2) * TMath::Power(j1mj2_Eta,2);
      phoCentral = TMath::Exp(x);
    }

    //jet3 centrality
    if(npfjet>2){
      Float_t jet3Eta = jetEta_[2];
      Int_t ilead = nonPUjetid[0];
      Int_t isub = nonPUjetid[1];

      Float_t j1mj2_Eta = jetEta_[ilead] - jetEta_[isub];
      Float_t j1pj2_Eta = jetEta_[ilead] + jetEta_[isub];
      Float_t x = -4.0 * TMath::Power((jet3Eta-(j1pj2_Eta/2)),2) * TMath::Power(j1mj2_Eta,2);
      jet3Central = TMath::Exp(x);
    }
    
    //Zg sys centrality
    TVector2 Zv2, phov2, leadjetv2, subjetv2;
    TVector2 Zgsys, jetjetsys;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(ipho>0) continue;
      if(npfjet<2) continue;
      Int_t ilead = nonPUjetid[0];
      Int_t isub = nonPUjetid[1];
      Zv2.SetMagPhi(puppiMET, puppiMETPhi);
      //cout << "create Zv2";
      phov2.SetMagPhi(phoEt[ipho], phoPhi[ipho]);
      //cout << " create phov2";
      leadjetv2.SetMagPhi(jetPt_[ilead], jetPhi_[ilead]);
      subjetv2.SetMagPhi(jetPt_[isub], jetPhi_[isub]);

    
      jetjetsys = leadjetv2+subjetv2;
      jetjetsysPhi = Phi_mpi_pi(jetjetsys.Phi());
      //cout << " " << jetjetsysPhi;
      Zgsys = Zv2+phov2;
      ZgsysPhi = Phi_mpi_pi(Zgsys.Phi());
      //cout << " " << ZgsysPhi;
      jetjetZdPhi = deltaPhi(puppiMETPhi, jetjetsysPhi);
      jetjetphodPhi = deltaPhi(phoPhi[ipho], jetjetsysPhi);
      //cout << " get jetjetZgdPhi" << endl;
    }

    //cutflow setbit
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(ipho > 0) continue;
      bit=0;
											bit = SetBit(0, bit);
      if(phohasPixelSeed[ipho] == 0)							bit = SetBit(1, bit);
      if(fabs(phoSCEta[ipho]) < 1.4442)							bit = SetBit(2, bit);
      if(phoEt[ipho] > 210)							bit = SetBit(3, bit);
      if(MphoID[ipho] == 1)								bit = SetBit(4, bit);
      //if(Iso_rc[4][ipho] < 1.141)							bit = SetBit(5, bit);
      if(fabs(phoSeedTime[ipho])<3)							bit = SetBit(5, bit);
      if(nLep<1)									bit = SetBit(6, bit);
      //(puppiMET> 140+(0.138*(phoCalibEt[ipho]-500)))					bit = SetBit(8, bit);
      if(puppiMET>100)									bit = SetBit(7, bit);
      //if((phoEt[ipho]/puppiMET) < 2.4 ) bit = SetBit(7, bit);
      if(fabs(phoMETdPhi[ipho]) > 0.5)							bit = SetBit(8, bit); 
      if(npfjet == 1 ){

	if(jetPt_[nonPUjetid[0]] >30) {bit = SetBit(9, bit); nSMjet = npfjet;}
	else if(jetPt_[nonPUjetid[0]] <30) {bit = SetBit(9, bit); nSMjet = 0;}
      }
      else if(npfjet == 0){								bit = SetBit(9, bit); nSMjet = npfjet;}
    
      for(Int_t i=0; i<7; i++){								bit = SetBit(i+10, bit); }      
      cutflowSMID.push_back(bit);
      //cout << "get SM cut" << endl;

      for(Int_t i=0; i<9; i++){								bit = ResetBit(i+8, bit); }
      if(npfjet >=2){
	Int_t ilead = nonPUjetid[0];
	Int_t isub = nonPUjetid[1];

	if(fabs(jetjetphodPhi) < 2.7) bit = SetBit(8, bit);
	if(jetPt_[ilead] >50 && jetPt_[isub] >30) {bit = SetBit(9, bit); nVBSjet = npfjet;}
	if((jetEta_[ilead] * jetEta_[isub]) < 0)					bit = SetBit(10, bit);
	if(jetjetdEta > 2.5)								bit = SetBit(11, bit);
	if(fabs(jetjetdPhi) < 2.7)							bit = SetBit(12, bit);
	if(phojetdR[0]>0.4 && phojetdR[1]>0.4)						bit = SetBit(13, bit);
	//if(phoCentral > 0.4)								bit = SetBit(13, bit);
	if((phoEt[ipho]/puppiMET) < 2.4 ) bit = SetBit(14, bit);
	if(fabs(minJMETdPhi)   > 0.5)							bit = SetBit(15, bit);
	if(dijetMass>250)								bit = SetBit(16, bit);
      }
      cutflowVBSID.push_back(bit);
      //cout << "get VBS cut" << endl;
    }
    //cout << "get setbit" << endl;
    
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
      ootPhohasPixelSeed.push_back(ootPhohasPixelSeed_[i]);
      ootPhoHoverE.push_back(ootPhoHoverE_[i]);
      ootPhoSigmaIEtaIEtaFull5x5.push_back(ootPhoSigmaIEtaIEtaFull5x5_[i]);
      ootPhoR9Full5x5.push_back(ootPhoR9Full5x5_[i]);
      ootPhoSeedTime.push_back(ootPhoSeedTime_[i]);
      ootPhoSeedEnergy.push_back(ootPhoSeedEnergy_[i]);
      ootPhoMIPTotEnergy.push_back(ootPhoMIPTotEnergy_[i]);
      ootPhoEtop.push_back(ootPhoEtop_[i]);
      ootPhoEleft.push_back(ootPhoEleft_[i]);
      ootPhoEright.push_back(ootPhoEright_[i]);
      ootPhoEbottom.push_back(ootPhoEbottom_[i]);
      
      Float_t sumphoE4 = 0;
      sumphoE4 = ootPhoEtop_[i]+ootPhoEleft_[i]+ootPhoEright_[i]+ootPhoEbottom_[i];
      ootPhoE4.push_back(sumphoE4);

      Float_t E4overE = 0;
      E4overE = sumphoE4/ootPhoSeedEnergy_[i];
      ootPhoE4overE1.push_back(E4overE);
      
      Float_t compWing = 0;
      if(ootPhoEleft_[i] > ootPhoEright_[i]) compWing = ootPhoEleft_[i];
      else compWing = ootPhoEright_[i];
      ootPhoEtaWing.push_back(compWing);
      
      Float_t WingoverE = 0;
      WingoverE = compWing/phoSeedEnergy_[i];
      ootPhoEtaWingoverE1.push_back(WingoverE);

    }

    nEle = nEle_;
    for(Int_t i=0; i<nEle_; i++){
      //if(year == 2016 && (eleFiredSingleTrgs_[i]>>13&1) == 0) continue;
      //else if(year ==2017 && (eleFiredSingleTrgs_[i]>>13&1) == 0) continue;
      //else if(year == 2018 && (eleFiredSingleTrgs_[i]>>13&1) == 0) continue;
      elePt.push_back(elePt_[i]);
      eleEta.push_back(eleEta_[i]);
      //elePhi.push_back(elePhi_[i]);
      //eleEn.push_back(eleEn_[i])
      eleD0.push_back(eleD0_[i]);
      eleDz.push_back(eleDz_[i]);
      eleIDbit.push_back(eleIDbit_[i]);
      //eleFiredSingleTrgs.push_back(eleFiredSingleTrgs_[i]);
      //eleHoverE.push_back(eleHoverE_[i]);
      //eleSigmaIEtaIEtaFull5x5.push_back(eleSigmaIEtaIEtaFull5x5_[i]);
      //eleR9Full5x5.push_back(eleR9Full5x5_[i]);
      //elePFChIso.push_back(elePFChIso_[i]);
      //elePFPhoIso.push_back(elePFPhoIso_[i]);
      //elePFNeuIso.push_back(elePFNeuIso_[i]);
      //eleConvVeto.push_back(eleConvVeto_[i]);
      //eleMissHits.push_back(eleMissHits_[i]);
    }
    //cout << "fill ele" << endl;

    nMu = nMu_;
    for(Int_t i=0; i<nMu_; i++){
      muPt.push_back(muPt_[i]);
      muEta.push_back(muEta_[i]);
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
