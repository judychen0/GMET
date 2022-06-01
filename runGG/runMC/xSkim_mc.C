///////////////Things to change for UL MC/////////////////////
// HasPixelSeed read in file
// HasPixelSeed GetBinContent
// JetID and JETPUID

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

void xSkim_mc(char* pathes, char* PUpathes, char* IDpathes, char* PSVpathes, Int_t dcount, Int_t year, Int_t sig){

  //Get processed events
  TFile *fopen = new TFile(pathes, "READ");
  TH1F *hEvents = (TH1F*)fopen->Get("ggNtuplizer/hEvents")->Clone();
  hEvents->SetDirectory(0);
  TH1F *hSumofGenW = (TH1F*)fopen->Get("ggNtuplizer/hSumGenWeight")->Clone();
  hSumofGenW->SetDirectory(0);
  fopen->Close();
  
  //Get phoID SF
  //fopen = new TFile("2016_PhotonsMedium.root", "READ");
  fopen = new TFile(IDpathes, "READ");
  TH2F *h_phoIDSF = (TH2F*)fopen->Get("EGamma_SF2D")->Clone();
  h_phoIDSF->SetDirectory(0);
  fopen->Close();

  //Get EleVeto SF
  //fopen = new TFile("PSV_ScaleFactors_2016.root", "READ");
  fopen = new TFile(PSVpathes, "READ");
  TH2F *h_PSVSF = (TH2F*)fopen->Get("Scaling_Factors_PixelSeed_R9_Inclusive")->Clone();//Legacy MC
  //TH1F *h_PSVSF = (TH1F*)fopen->Get("MediumID/SF_HasPix_MediumID")->Clone();// UL MC
  h_PSVSF->SetDirectory(0);
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
  Int_t            run_;
  Long64_t         event_;
  Bool_t           isData_;
  Int_t            nVtx_;
  Int_t            nGoodVtx_;
  Bool_t           isPVGood_;
  Float_t          rho_;
  Float_t          genWeight_;
  Int_t            nPUInfo_;
  Int_t*           puBX_;
  Float_t*         puTrue_;
  ///gen particles
  Int_t            nMC_;
  Int_t*           mcPID_;
  Float_t*         mcPt_;
  Float_t*         mcMass_;
  Float_t*         mcEta_;
  Float_t*         mcPhi_;
  Float_t*         mcE_;
  Float_t*         mcEt_;
  Int_t*           mcMomPID_;
  Float_t*         mcMomPt_;
  Float_t*         mcMomMass_;
  Float_t*         mcMomEta_;
  Float_t*         mcMomPhi_;
  Short_t*         mcStatusFlag_;
  ///MET
  Float_t          pfMET_;
  Float_t          pfMETPhi_;
  //Int_t            metFilters_;
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
  Float_t*      phoMIPTotEnergy_;
  Short_t*      phoIDbit_;
  ///electron
  Int_t         nEle_;
  Float_t*      elePt_;
  Float_t*      eleD0_;
  Float_t*      eleDz_;
  Float_t*      eleEta_;
  Short_t*      eleIDbit_;
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
  Float_t*      jetCEF_;
  Float_t*      jetMUF_;
  Float_t*      jetCHF_;
  Float_t*      jetNHF_;
  Float_t*      jetNEF_;
  Int_t*        jetNCH_;
  Int_t*        jetNNP_;
  Float_t*      jetP4Smear_;
  
  //output branch variables
  ///global event
  Int_t                 run;
  Long64_t              event;
  Bool_t                isData;
  Int_t                 nVtx;
  Int_t                 nGoodVtx;
  Bool_t                isPVGood;
  Float_t               rho;
  Float_t               genWeight;
  Float_t               genWeightSign;
  Float_t       EventWeight;
  Int_t                 nPUInfo;
  vector<Int_t>         puBX;
  vector<Float_t>       puTrue;
  ///gen particle
  Int_t                 nMC;
  vector<Int_t>         mcPID;
  vector<Float_t>       mcPt;
  vector<Float_t>       mcMass;
  vector<Float_t>       mcEta;
  vector<Float_t>       mcPhi;
  vector<Float_t>       mcE;
  vector<Float_t>       mcEt;
  vector<Int_t>         mcMomPID;
  vector<Float_t>       mcMomPt;
  vector<Float_t>       mcMomMass;
  vector<Float_t>       mcMomEta;
  vector<Float_t>       mcMomPhi;
  vector<Short_t>       mcStatusFlag;
  Int_t                 nMCpho;
  Int_t                 nmatchpho;
  vector<Int_t>         phoIsMatch;
  vector<Int_t>         phoIsHard;
  vector<Float_t>       phodR;
  vector<Float_t>       phodPt;
  ///MET
  Float_t               pfMET;
  Float_t               pfMETPhi;
  Float_t               corrMET;
  Float_t               corrMETPhi;
  Int_t                 metFilters;
  Int_t                 failedMET;
  ///photon
  Int_t                 nPho;
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
  vector<Float_t>       phoMIPTotEnergy;
  vector<Float_t>       phoMETdPhi;
  vector<Short_t>       phoIDbit;
  vector<Int_t>         MphoID;
  Float_t phoCentral; //photon centrality
  ///cutflow ID
  vector<Int_t>         cutflowSMID;
  vector<Int_t>         cutflowVBSID;
  vector<Int_t>         cutflowSMfixMETID;
  vector<Int_t>         cutflowVBSfixMETID;
  ///electron
  Int_t                 nEle;
  vector<Float_t>       elePt;
  vector<Float_t>       eleD0;
  vector<Float_t>       eleDz;
  vector<Float_t>       eleEta;
  vector<Short_t>       eleIDbit;
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
  vector<Int_t>         jetULTID;
  vector<Int_t>         jetTID;
  vector<Float_t>       jetCEF;
  vector<Float_t>       jetMUF;
  vector<Float_t>	jetCHF;
  vector<Float_t>	jetNHF;
  vector<Float_t>	jetNEF;
  vector<Int_t>		jetNCH;
  vector<Int_t>		jetNNP;
  vector<Float_t>       jetP4Smear;
  vector<Float_t>       jetPtSmear;
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
  outtree_->Branch("genWeight"			,&genWeight     ,"genWeight/F"		);
  outtree_->Branch("genWeightSign"		,&genWeightSign ,"genWeightSign/F"	);
  outtree_->Branch("EventWeight"                ,&EventWeight	,"EventWeight/F"			);
  outtree_->Branch("nPUInfo"			,&nPUInfo       ,"nPUInfo/I"		);
  outtree_->Branch("puBX"                       ,&puBX					);
  outtree_->Branch("puTrue"                     ,&puTrue				);
  outtree_->Branch("nMC"			,&nMC           ,"nMC/I"		);
  outtree_->Branch("mcPID"                      ,&mcPID					);
  outtree_->Branch("mcPt"                       ,&mcPt					);
  outtree_->Branch("mcMass"                     ,&mcMass				);
  outtree_->Branch("mcEta"                      ,&mcEta					);
  outtree_->Branch("mcPhi"                      ,&mcPhi					);
  outtree_->Branch("mcE"                        ,&mcE					);
  outtree_->Branch("mcEt"                       ,&mcEt					);
  outtree_->Branch("mcMomPID"                   ,&mcMomPID				);
  outtree_->Branch("mcMomPt"                    ,&mcMomPt				);
  outtree_->Branch("mcMomMass"                  ,&mcMomMass				);
  outtree_->Branch("mcMomEta"                   ,&mcMomEta				);
  outtree_->Branch("mcMomPhi"                   ,&mcMomPhi				);
  outtree_->Branch("mcStatusFlag"               ,&mcStatusFlag				);
  outtree_->Branch("nMCpho"			,&nMCpho        ,"nMCpho/I"		);
  outtree_->Branch("nmcatchpho"			,&nmatchpho     ,"nmatchpho/I"		);
  outtree_->Branch("phoIsMatch"                 ,&phoIsMatch				);
  outtree_->Branch("phoIsHard"                  ,&phoIsHard				);
  outtree_->Branch("phodR"                      ,&phodR					);
  outtree_->Branch("phodPt"                     ,&phodPt				);
  outtree_->Branch("pfMET"			,&pfMET         ,"pfMET/F"		);
  outtree_->Branch("pfMETPhi"			,&pfMETPhi      ,"pfMETPhi/F"		);
  outtree_->Branch("corrMET"                    ,&corrMET       ,"corrMET/F"		);
  outtree_->Branch("corrMETPhi"                 ,&corrMETPhi    ,"corrMETPhi/F"		);
  outtree_->Branch("failedMET"			,&failedMET     ,"failedMET/I"		);
  outtree_->Branch("nPho"			,&nPho          ,"nPho/I"		);
  outtree_->Branch("phoE"                       ,&phoE					);
  outtree_->Branch("phoEt"                      ,&phoEt					);
  outtree_->Branch("phoEta"                     ,&phoEta				);
  outtree_->Branch("phoPhi"                     ,&phoPhi				);
  outtree_->Branch("phoCalibE"                  ,&phoCalibE				);
  outtree_->Branch("phoCalibEt"                 ,&phoCalibEt				);
  outtree_->Branch("phoSCEta"                   ,&phoSCEta				);
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
  outtree_->Branch("phoMIPTotEnergy"            ,&phoMIPTotEnergy			);
  outtree_->Branch("phoMETdPhi"                 ,&phoMETdPhi				);
  outtree_->Branch("phoIDbit"                   ,&phoIDbit				);
  outtree_->Branch("MphoID"                     ,&MphoID				);
  outtree_->Branch("phoCentral",&phoCentral ,"phoCentral/F");
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
  outtree_->Branch("muEta" ,&muEta );
  outtree_->Branch("muIDbit"                    ,&muIDbit				);
  outtree_->Branch("muD0"                       ,&muD0					);
  outtree_->Branch("muDz"                       ,&muDz					);
  outtree_->Branch("nLep"			,&nLep          ,"nLep/I"		);
  outtree_->Branch("nJet"			,&nJet          ,"nJet/I"		);
  outtree_->Branch("npfjet"			,&npfjet        ,"npfjet/I"		);
  outtree_->Branch("nSMjet"			,&nSMjet        ,"nSMjet/I"		);
  outtree_->Branch("nVBSjet"			,&nVBSjet       ,"nVBSjet/I"		);
  outtree_->Branch("nonPUjetid"			,&nonPUjetid				);
  outtree_->Branch("jetPt"			,&jetPt					);
  outtree_->Branch("jetEn"			,&jetEn					);
  outtree_->Branch("jetEta"			,&jetEta				);
  outtree_->Branch("jetPhi"			,&jetPhi				);
  outtree_->Branch("dijetMass"			,&dijetMass     ,"dijetMass/D"		);
  outtree_->Branch("jetPUIDMVA"			,&jetPUIDMVA				);
  outtree_->Branch("jetPUID"			,&jetPUID				);
  outtree_->Branch("jetID"			,&jetID					);
  outtree_->Branch("jetULTID"			,&jetULTID				);
  outtree_->Branch("jetTID"			,&jetTID				);
  outtree_->Branch("jetCEF"			,&jetCEF				);
  outtree_->Branch("jetMUF"			,&jetMUF				);
  outtree_->Branch("jetCHF"			,&jetCHF				);
  outtree_->Branch("jetNHF"			,&jetNHF				);
  outtree_->Branch("jetNEF"			,&jetNEF				);
  outtree_->Branch("jetNCH"			,&jetNCH				);
  outtree_->Branch("jetNNP"			,&jetNNP				);
  outtree_->Branch("jetP4Smear"			,&jetP4Smear				);
  outtree_->Branch("jetPtSmear"                 ,&jetPtSmear				);
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
  outtree_->Branch("ZgsysPhi"			,&ZgsysPhi	,"ZgsysPhi/F"		);
  outtree_->Branch("jetjetsysPhi"		,&jetjetsysPhi	,"jetjetsysPhi/F"	);
  outtree_->Branch("jet3Central",&jet3Central ,"jet3Central/F");

  //create histogram
  //TH1F *hSumofGenW = new TH1F("hSumofGenW", "Sum of Gen Weight", 1, 0, 1);

  // pileup reweighting
  PUWeightCalculator puCalc;
  puCalc.Init(PUpathes);

  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    //cout << "get ev " << ev << endl;
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);

    run_                        = data.GetInt(		"run"				);
    event_			= data.GetLong64(	"event"				);
    isData_			= data.GetBool(		"isData"			);
    nVtx_			= data.GetInt(		"nVtx"				);
    nGoodVtx_			= data.GetInt(		"nGoodVtx"			);
    isPVGood_                   = data.GetBool(		"isPVGood"			);
    rho_			= data.GetFloat(	"rho"				);
    genWeight_			= data.GetFloat(	"genWeight"			);
    nPUInfo_			= data.GetInt(		"nPUInfo"			);
    puBX_			= data.GetPtrInt(	"puBX"				);
    puTrue_			= data.GetPtrFloat(	"puTrue"			);
    nMC_			= data.GetInt(		"nMC"				);
    mcPID_			= data.GetPtrInt(	"mcPID"				);
    mcPt_			= data.GetPtrFloat(	"mcPt"				);
    mcMass_			= data.GetPtrFloat(	"mcMass"			);
    mcEta_			= data.GetPtrFloat(	"mcEta"				);
    mcPhi_			= data.GetPtrFloat(	"mcPhi"				);
    mcE_			= data.GetPtrFloat(	"mcE"				);
    mcEt_			= data.GetPtrFloat(	"mcEt"				);
    mcMomPID_			= data.GetPtrInt(	"mcMomPID"			);
    mcMomPt_			= data.GetPtrFloat(	"mcMomPt"			);
    mcMomMass_			= data.GetPtrFloat(	"mcMomMass"			);
    mcMomEta_			= data.GetPtrFloat(	"mcMomEta"			);
    mcMomPhi_			= data.GetPtrFloat(	"mcMomPhi"			);
    mcStatusFlag_		= data.GetPtrShort(	"mcStatusFlag"			);
    pfMET_			= data.GetFloat(	"pfMET"				);
    pfMETPhi_			= data.GetFloat(	"pfMETPhi"			);
    //metFilters_		= data.GetInt(		"metFilters"			);
    nPho_			= data.GetInt(		"nPho"				);
    phoE_			= data.GetPtrFloat(	"phoE"				);
    phoEt_			= data.GetPtrFloat(	"phoEt"				);
    phoEta_			= data.GetPtrFloat(	"phoEta"			);
    phoPhi_			= data.GetPtrFloat(	"phoPhi"			);
    phoCalibE_			= data.GetPtrFloat(	"phoCalibE"			);
    phoCalibEt_		        = data.GetPtrFloat(	"phoCalibEt"			);
    phoSCEta_			= data.GetPtrFloat(	"phoSCEta"			);
    phohasPixelSeed_		= data.GetPtrInt(	"phohasPixelSeed"		);
    phoEleVeto_		        = data.GetPtrInt(	"phoEleVeto"			);
    phoHoverE_			= data.GetPtrFloat(	"phoHoverE"			);
    phoSigmaIEtaIEtaFull5x5_    = data.GetPtrFloat(	"phoSigmaIEtaIEtaFull5x5"	);
    phoR9Full5x5_		= data.GetPtrFloat(	"phoR9Full5x5"			);
    phoPFChIso_		        = data.GetPtrFloat(	"phoPFChIso"			);
    phoPFPhoIso_		= data.GetPtrFloat(	"phoPFPhoIso"			);
    phoPFNeuIso_		= data.GetPtrFloat(	"phoPFNeuIso"			);
    phoPFChWorstIso_		= data.GetPtrFloat(	"phoPFChWorstIso"		);
    phoFiredSingleTrgs_	        = data.GetPtrLong64(	"phoFiredSingleTrgs"		);
    phoSeedTime_		= data.GetPtrFloat(	"phoSeedTime"			);
    phoMIPTotEnergy_		= data.GetPtrFloat(	"phoMIPTotEnergy"		);
    phoIDbit_			= data.GetPtrShort(	"phoIDbit"			);
    nEle_			= data.GetInt(		"nEle"				);
    elePt_			= data.GetPtrFloat(	"elePt"				);
    eleD0_			= data.GetPtrFloat(	"eleD0"				);
    eleDz_			= data.GetPtrFloat(	"eleDz"				);
    eleEta_			= data.GetPtrFloat(	"eleEta"			);
    eleIDbit_			= data.GetPtrShort(	"eleIDbit"			);
    nMu_			= data.GetInt(		"nMu"				);
    muPt_ 			= data.GetPtrFloat(	"muPt"				);
    muEta_ = data.GetPtrFloat("muEta");
    muIDbit_			= data.GetPtrInt(	"muIDbit"			);
    muD0_			= data.GetPtrFloat(	"muD0"				);
    muDz_			= data.GetPtrFloat(	"muDz"				);
    nJet_			= data.GetInt(		"nJet"				);
    jetPt_			= data.GetPtrFloat(	"jetPt"				);
    jetEn_			= data.GetPtrFloat(	"jetEn"				);
    jetEta_			= data.GetPtrFloat(	"jetEta"			);
    jetPhi_			= data.GetPtrFloat(	"jetPhi"			);
    jetPUID_			= data.GetPtrFloat(	"jetPUID"			);
    //jetID_			= data.GetPtrInt(	"jetID"				);
    jetCEF_			= data.GetPtrFloat(	"jetCEF"			);
    jetMUF_			= data.GetPtrFloat(	"jetMUF"			);
    jetCHF_			= data.GetPtrFloat(	"jetCHF"			);
    jetNHF_			= data.GetPtrFloat(	"jetNHF"			);
    jetNEF_			= data.GetPtrFloat(	"jetNEF"			);
    jetNCH_			= data.GetPtrInt(	"jetNCH"			);
    jetNNP_			= data.GetPtrInt(	"jetNNP"			);
    jetP4Smear_                 = data.GetPtrFloat(	"jetP4Smear"			);
    //cout <<						"get all variable" << endl;
    
    //init variable
    run			= 0;
    event		= 0;
    isData		= 0;
    nVtx		= 0;
    nGoodVtx		= 0;
    isPVGood		= 0;
    rho			= 0;
    genWeight		= 0;
    genWeightSign	= 0;
    EventWeight = 0;
    nPUInfo		= 0;
    puBX.clear();
    puTrue.clear();
    nMC			= 0;
    mcPID.clear();
    mcPt.clear();
    mcMass.clear();
    mcEta.clear();
    mcPhi.clear();
    mcE.clear();
    mcEt.clear();
    mcMomPID.clear();
    mcMomPt.clear();
    mcMomMass.clear();
    mcMomEta.clear();
    mcMomPhi.clear();
    mcStatusFlag.clear();
    nMCpho		= 0;
    nmatchpho		= 0;
    phoIsMatch.clear();
    phoIsHard.clear();
    phodR.clear();
    phodPt.clear();
    
    pfMET		= 0;
    pfMETPhi		= 0;
    corrMET		= 0;
    corrMETPhi		= 0;
    metFilters		= 0;
    failedMET		= 0;
    nPho		= 0;
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
    phoMIPTotEnergy.clear();
    phoMETdPhi.clear();
    phoIDbit.clear();
    MphoID.clear();
    phoCentral = 0;
    
    cutflowSMID.clear();
    cutflowVBSID.clear();
    cutflowSMfixMETID.clear();
    cutflowVBSfixMETID.clear();
    nEle		= 0;
    elePt.clear();
    eleD0.clear();
    eleDz.clear();
    eleEta.clear();
    eleIDbit.clear();
    nMu			= 0;
    muPt.clear();
    muEta.clear();
    muIDbit.clear();
    muD0.clear();
    muDz.clear();
    nLep		= 0;
    nJet		= 0;
    npfjet		= 0;
    nSMjet		= 0;
    nVBSjet		= 0;
    nonPUjetid.clear();
    jetPt.clear();
    jetEn.clear();
    jetEta.clear();
    jetPhi.clear();
    dijetMass		= 0.0;
    jetPUIDMVA.clear();
    jetPUID.clear();
    jetID.clear();
    jetULTID.clear();
    jetTID.clear();
    jetCEF.clear();
    jetMUF.clear();
    jetCHF.clear();
    jetNHF.clear();
    jetNEF.clear();
    jetNCH.clear();
    jetNNP.clear();
    jetP4Smear.clear();
    jetPtSmear.clear();
    jet3Central = 0;
    minJMETdPhi		= 0;
    JMETdPhi.clear();
    jetjetdR		= 0;
    jetjetdEta		= 0;
    jetjetdPhi		= 0;
    phojetdR.clear();
    phojetdEta.clear();
    phojetdPhi.clear();
    jetjetZdPhi	= 0;
    jetjetphodPhi	= 0;
    ZgsysPhi		= 0;
    jetjetsysPhi	= 0;

    
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
	phoMIPTotEnergy.push_back(phoMIPTotEnergy_[ipho]);
	phoIDbit.push_back(phoIDbit_[ipho]);

	phoPFChIso_rc.push_back(Iso_rc[0][ipho]);
	phoPFPhoIso_rc.push_back(Iso_rc[1][ipho]);
	phoPFNeuIso_rc.push_back(Iso_rc[2][ipho]);
	phoPFChWorstIso_rc.push_back(Iso_rc[3][ipho]);
	phoPFChWorstIso_newEA.push_back(Iso_rc[4][ipho]);
    }
    //cout << "get rho corr" << endl;
    if(nPho<1) continue;
    
    //get mc photon id
    vector<Int_t> mc_phoid; mc_phoid.clear();
    for(Int_t k=0; k < nMC_; k++){
      if(fabs(mcMomPID_[k]) > 6 && fabs(mcMomPID_[k]) != 21) continue;
      if((mcStatusFlag_[k]>>1&1) == 0) continue;//isPromptFinalState passed
      if(mcPID_[k] == 22){
	mc_phoid.push_back(k);
	nMCpho++;
      }
    }
    
    //gen-matching
    Int_t isMatched;
    for(Int_t ipho=0; ipho < nPho; ipho++){
      isMatched = 0;
      for(Int_t nn=0; nn < nMCpho; nn++){
	Int_t k = mc_phoid[nn];
	Float_t dr = deltaR(phoEta[ipho], phoPhi[ipho], mcEta_[k], mcPhi_[k]);
	Float_t dpt = fabs((phoEt[ipho] - mcPt_[k])/mcPt_[k]);
	if(dr < 0.2) phodPt.push_back(dpt);
	if(dpt < 0.2) phodR.push_back(dr);
	if(dr < 0.2 && dpt < 0.2){
	  isMatched = 1;
	  nmatchpho++;
	  break;
	}
      }
      //if(nmatchpho > 1) break;
      phoIsMatch.push_back(isMatched);
    }

    //genweight refill sign only
    if(genWeight_ > 0) genWeightSign = 1.0;
    else if(genWeight_ == 0) genWeightSign = 0.0;
    else if(genWeight_ < 0) genWeightSign = -1.0;
    //hSumofGenW->Fill(0.5, genWeightSign);
    //cout<< "get norm genwei" << endl;

    //pileup
    Int_t nPU;
    Float_t puwei_ = 1;
    for (Int_t i=0; i<nPUInfo_; ++i) {
      if (puBX_[i] == 0) nPU = puTrue_[i];
    }      
    puwei_ = 1.;
    puwei_ = (Float_t)puCalc.GetWeight(run_, puTrue_[1]);
    //cout << "get PU wei" << endl;
    
    //cal tot weight
    Double_t phoSF = 1.0;
    Double_t PSVSF = 1.0;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(ipho>0) continue;
      if(isData_ == 0){
	Int_t ptbin = 0;
	Int_t etabin = 0;
	if(phoCalibEt[ipho] >= 200) ptbin = 5;
	//else if(phoCalibEt[ipho] >= 250 && phoCalibEt[ipho] < 300) ptbin = 2;
	//else if(phoCalibEt[ipho] >= 300 && phoCalibEt[ipho] < 350) ptbin = 3;
	//else if(phoCalibEt[ipho] >= 350 && phoCalibEt[ipho] < 450) ptbin = 4;
	//else if(phoCalibEt[ipho] >= 450 && phoCalibEt[ipho] < 1000) ptbin = 5;
	
	if(phoSCEta[ipho] > -1.4442 && phoSCEta[ipho] < -0.8) etabin = 1;
	else if(phoSCEta[ipho] >=-0.8 && phoSCEta[ipho] < 0.0) etabin = 2;
	else if(phoSCEta[ipho] >=0.0 && phoSCEta[ipho] < 0.8) etabin = 3;
	else if(phoSCEta[ipho] >=0.8 && phoSCEta[ipho] < 1.4442) etabin = 4;

	if(ptbin>0 && etabin>0) phoSF = h_phoIDSF->GetBinContent(etabin+3, ptbin);

	PSVSF = h_PSVSF->GetBinContent(1, 1);// Legacy MC
	//PSVSF = h_PSVSF->GetBinContent(1);// UL MC
      }
      Double_t tot_wei = 1;
      tot_wei = phoSF*puwei_*genWeightSign;
      EventWeight = tot_wei;
    }
    //cout << "get tot wei" << endl;
   
    //cout << "get gen match" << endl;
    //lepton veto selection
    Int_t nLele = 0;
    for(Int_t iele=0; iele<nEle_; iele++){
      //if((eleIDbit_[iele]>>1&1) == 1 && elePt_[iele] > 10 && fabs(eleEta_[iele]) < 2.5) nLele++;

      //if(eleD0_[iele] >0.5 && fabs(eleEta_[iele]) <1.4442) continue;
      //else if(eleD0_[iele] > 1.0 && fabs(eleEta_[iele]) > 1.5666) continue;
      //if(eleDz_[iele] >1.0 && fabs(eleEta_[iele]) <1.4442) continue;
      //else if(eleDz_[iele] >2.0 && fabs(eleEta_[iele]) >1.5666) continue;
      //nLele++;


      /// tighter ele selection ///
      if((eleIDbit_[iele]>>1&1) == 1 && elePt_[iele] > 10 && fabs(eleEta_[iele]) < 2.5) {
      	if(fabs(eleEta_[iele]) <1.4442 && eleD0_[iele] < 0.5 && eleDz_[iele] < 1.0) nLele++;
      	else if(fabs(eleEta_[iele]) > 1.5666 && eleD0_[iele] < 1.0 && eleDz_[iele] < 2.0) nLele++;
      }
    }
    
    Int_t nLmu = 0;
    for(Int_t imu=0; imu<nMu_; imu++){
      //if((muIDbit_[imu]>>0&1) == 1 && muPt_[imu] > 10 && fabs(muEta_[imu]) < 2.4) nLmu++;

      //if(muD0_[imu] > 2) continue;
      //if(muDz_[imu] > 5) continue;
      //nLmu++;

      /// tighter mu selection///
      if((muIDbit_[imu]>>0&1) == 1 && muPt_[imu] > 10 && fabs(muEta_[imu]) < 2.4){
      	if(muD0_[imu] < 2 && muDz_[imu] < 5) nLmu++;
      }
    }
    Int_t nlep = 0;
    nLep = nLele + nLmu;
    //cout << "get lepton veto" << endl;
   
    //jet smear
    for(Int_t ijet=0; ijet<nJet_; ijet++){
      //if(year == 2017 && jetPt_[ijet]<50 && fabs(jetEta_[ijet])>2.56 && fabs(jetEta_[ijet])<3.139) jetPt_[ijet] = 0;//remove 2017 ecal noise
      if((jetPt_[ijet]*jetP4Smear_[ijet]) <= 0) jetPtSmear.push_back(0);
      else jetPtSmear.push_back(jetPt_[ijet]*jetP4Smear_[ijet]);
    }
    //cout << "get jet smearing" << endl;

    //propagate JEC to MET
    TVector2 jetv2[nJet_], corrjetv2[nJet_];
    TVector2 sumjet;
    TVector2 sumcorrjet;
    for(Int_t ijet=0; ijet<nJet_; ijet++){
      jetv2[ijet].SetMagPhi(jetPt_[ijet], jetPhi_[ijet]);
      corrjetv2[ijet].SetMagPhi(jetPtSmear[ijet], jetPhi_[ijet]);
      if(jetPtSmear[ijet]>10){
	sumjet += jetv2[ijet];
	sumcorrjet += corrjetv2[ijet];
      }
    }
    TVector2 METv2;
    METv2.SetMagPhi(pfMET_, pfMETPhi_);
    TVector2 corrMETv2;
    corrMETv2 = METv2 + sumjet - sumcorrjet;
    corrMET = corrMETv2.Mod();
    //cout << "get MET corr" << endl;

    //MET XY corr
    vector<Float_t> corrXY; corrXY.clear();
    METXYshift(year, pathes, data, corrMET, corrXY);
    corrMET = corrXY[0];
    corrMETPhi = Phi_mpi_pi(corrXY[1]);
    //cout << "get METPhi corr" << endl;
    
    //jetID selection TLepVeto
    //jet_ULTIDsel(year, data, jetULTID);
    //jet_ULPUIDsel(year, data, jetPUIDMVA);
    jet_TIDsel(year, data, jetTID);
    jet_PUIDsel(data, jetPUIDMVA);
    
    //count # jet
    for(Int_t i=0; i<nJet_; i++){

      //if(npfjet>=2){
      //	//if((jetPUIDMVA[i] == 1) && fabs(jetEta_[i]) < 4.7 && (jetULTID[i] == 1)){// UL MC with PU and no pT cut
      //	if((jetPUIDMVA[i] == 1) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1) ){//for legacy MC with PU and no pT cut
      //	  nonPUjetid.push_back(i);
      //	  npfjet++;
      //	}
      //}
      //else{
      //	//if((jetPUIDMVA[i] == 1) && (jetPtSmear[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetULTID[i] == 1)){// UL MC with PU
      //	if((jetPUIDMVA[i] == 1) && (jetPtSmear[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1) ){//for legacy MC with PU
      //	  //if((jetPt_[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1)){// UL data no PU
      //	  nonPUjetid.push_back(i);
      //	  npfjet++;
      //	}
      //}

      //if((jetPUIDMVA[i] == 1) && (jetPtSmear[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetULTID[i] == 1)){// UL MC with PU
      if((jetPUIDMVA[i] == 1) && (jetPtSmear[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetTID[i] == 1) ){//for legacy MC with PU
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
    
    if(npfjet>= 2){
      Int_t ilead = nonPUjetid[0];
      Int_t isub = nonPUjetid[1];

      //mindjetMETPhi
      if(fabs(deltaPhi(jetPhi_[ilead], corrMETPhi)) < fabs(deltaPhi(jetPhi_[isub], corrMETPhi))) minJMETdPhi = deltaPhi(jetPhi_[ilead], corrMETPhi);
      else if(fabs(deltaPhi(jetPhi_[ilead], corrMETPhi)) > fabs(deltaPhi(jetPhi_[isub], corrMETPhi))) minJMETdPhi = deltaPhi(jetPhi_[isub], corrMETPhi);

      //dijet dEta & dPhi & dR
      jetjetdEta = deltaEta(jetEta_[ilead], jetEta_[isub]);
      jetjetdPhi = deltaPhi(jetPhi_[ilead], jetPhi_[isub]);
      jetjetdR = deltaR(jetEta_[ilead], jetPhi_[ilead], jetEta_[isub], jetPhi_[isub]);

      //dijet Mass
      leadjetP4.SetPtEtaPhiE(jetPtSmear[ilead], jetEta_[ilead], jetPhi_[ilead], jetEn_[ilead]);
      subjetP4.SetPtEtaPhiE(jetPtSmear[isub], jetEta_[isub], jetPhi_[isub], jetEn_[isub]);   
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
      JMETdPhi.push_back(deltaPhi(jetPhi_[ilead], corrMETPhi));
      JMETdPhi.push_back(deltaPhi(jetPhi_[isub], corrMETPhi));
    }
    else{
      minJMETdPhi = -999;
      
      jetjetdEta = -999;
      jetjetdPhi = -999;
      jetjetdR = -999;

      for(Int_t i=0; i<2; i++){
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
      phoMETdPhi.push_back(deltaPhi(phoPhi[ipho], corrMETPhi));
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
      Zv2.SetMagPhi(corrMET, corrMETPhi);
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
      jetjetZdPhi = deltaPhi(corrMETPhi, jetjetsysPhi);
      jetjetphodPhi = deltaPhi(phoPhi[ipho], jetjetsysPhi);
      //cout << " get jetjetZgdPhi" << endl;
    }

    //cutflow setbit
    //cout << "get Tlorentz" << endl;
    for(Int_t ipho=0; ipho<nPho; ipho++){
      if(ipho > 0) continue;
      bit=0;
      if(dcount == 0 && sig == 1 && phoIsMatch[ipho] == 1)				bit = SetBit(0, bit);
      if(dcount == 1 && sig == 0 && phoIsMatch[ipho] == 0)				bit = SetBit(0, bit);
      if(dcount == 0 && sig == 0)							bit = SetBit(0, bit);
      //if(dcount == 0 && sig == 0 && phoIsMatch[ipho] == 1)				bit = SetBit(0, bit);//for gamma+jet
      if(phohasPixelSeed[ipho] == 0)							bit = SetBit(1, bit);
      if(fabs(phoSCEta[ipho]) < 1.4442)							bit = SetBit(2, bit);
      if(phoEt[ipho] > 210)							bit = SetBit(3, bit);
      if(MphoID[ipho] == 1)								bit = SetBit(4, bit);
      //if(Iso_rc[4][ipho] < 1.141)							bit = SetBit(5, bit);
      if(fabs(phoSeedTime[ipho])<3)							bit = SetBit(5, bit);
      if(nLep<1)									bit = SetBit(6, bit);
      //(corrMET> 140+(0.138*(phoCalibEt[ipho]-500)))					bit = SetBit(8, bit);
      if(corrMET>100)									bit = SetBit(7, bit);
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
	if((phoEt[ipho]/corrMET) < 2.4 ) bit = SetBit(14, bit);
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
    genWeight = genWeight_;
    nPUInfo = nPUInfo_;
    for(Int_t i=0; i<nPUInfo_; i++){
      puBX.push_back(puBX_[i]);
      puTrue.push_back(puTrue_[i]);
    }
    //cout<< "fill genweight" << endl;
    nMC = nMC_;
    for(Int_t i=0; i<nMC_; i++){
      mcPID.push_back(mcPID_[i]);
      mcPt.push_back(mcPt_[i]);
      mcMass.push_back(mcMass_[i]);
      mcEta.push_back(mcEta_[i]);
      mcPhi.push_back(mcPhi_[i]);
      mcE.push_back(mcE_[i]);
      mcEt.push_back(mcEt_[i]);
      mcMomPID.push_back(mcMomPID_[i]);
      mcMomPt.push_back(mcMomPt_[i]);
      mcMomMass.push_back(mcMomMass_[i]);
      mcMomEta.push_back(mcMomEta_[i]);
      mcMomPhi.push_back(mcMomPhi_[i]);
      mcStatusFlag.push_back(mcStatusFlag_[i]);
    }
    //cout << "fill mc" << endl;
    pfMET = pfMET_;
    pfMETPhi = pfMETPhi_;
    //metFilters = metFilters_;
    
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
      //jetID.push_back(jetID_[i]);
      jetP4Smear.push_back(jetP4Smear_[i]);
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
  
  hSumofGenW->Write();
  hEvents->Write();
  
  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());
}
