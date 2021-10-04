#include <iostream>
using namespace std;

#include "untuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "IsoCorrection.h"
#include "PhotonSelection.h"
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
  Int_t            run_;
  Long64_t         event_;
  Bool_t           isData_;
  Int_t            nVtx_;
  Float_t          rho_;
  Float_t          pfMET_;
  Float_t          pfMETPhi_;
  //Int_t            metFilters_;
  Int_t            nPho_;
  Float_t*         phoE_;
  Float_t*         phoEt_;
  Float_t*         phoEta_;
  Float_t*         phoPhi_;
  Float_t*         phoCalibE_;
  Float_t*         phoCalibEt_;
  Float_t*         phoSCEta_;
  Int_t*           phohasPixelSeed_;
  Int_t*           phoEleVeto_;
  Float_t*         phoR9_;
  Float_t*         phoHoverE_;
  Float_t*         phoSigmaIEtaIEtaFull5x5_;
  Float_t*         phoR9Full5x5_;
  Float_t*         phoPFChIso_;
  Float_t*         phoPFPhoIso_;
  Float_t*         phoPFNeuIso_;
  Float_t*         phoPFChWorstIso_;
  Long64_t*        phoFiredSingleTrgs_;
  Float_t*         phoSeedTime_;
  Float_t*         phoMIPTotEnergy_;
  Short_t*         phoIDbit_;
  Int_t            nEle_;
  Float_t*         elePt_;
  Float_t*         eleD0_;
  Float_t*         eleDz_;
  Float_t*         eleEta_;
  Short_t*         eleIDbit_;
  Int_t            nMu_;
  Float_t*         muPt_;
  Int_t*           muIDbit_;
  Float_t*         muD0_;
  Float_t*         muDz_;
  Int_t            nJet_;
  Float_t*         jetPt_;
  Float_t*         jetEn_;
  Float_t*         jetEta_;
  Float_t*         jetPhi_;
  Int_t*           jetPUFullID_;
  Int_t*           jetID_;
  
  //output branch variables
  Int_t                   run;
  Long64_t                event;
  Bool_t                  isData;
  Int_t                   nVtx;
  Float_t                 rho;
  Float_t                 EventWeight;
  Float_t                 pfMET;
  Float_t                 pfMETPhi;
  Int_t                   metFilters;
  Int_t                   failedMET;
  Int_t                   nPho;
  vector<Float_t>         phoE;
  vector<Float_t>         phoEt;
  vector<Float_t>         phoEta;
  vector<Float_t>         phoPhi;
  vector<Float_t>         phoCalibE;
  vector<Float_t>         phoCalibEt;
  vector<Float_t>         phoSCEta;
  vector<Int_t>           phohasPixelSeed;
  vector<Int_t>           phoEleVeto;
  vector<Float_t>         phoR9;
  vector<Float_t>         phoHoverE;
  vector<Float_t>         phoSigmaIEtaIEtaFull5x5;
  vector<Float_t>         phoR9Full5x5;
  vector<Float_t>         phoPFChIso;
  vector<Float_t>         phoPFPhoIso;
  vector<Float_t>         phoPFNeuIso;
  vector<Float_t>         phoPFChWorstIso;
  vector<Float_t>         phoPFChIso_rc;
  vector<Float_t>         phoPFPhoIso_rc;
  vector<Float_t>         phoPFNeuIso_rc;
  vector<Float_t>         phoPFChWorstIso_rc;
  vector<Float_t>         phoPFChWorstIso_newEA;
  vector<Long64_t>        phoFiredSingleTrgs;
  vector<Float_t>         phoSeedTime;
  vector<Float_t>         phoMIPTotEnergy;
  vector<Float_t>         phoMETdPhi;
  vector<Short_t>         phoIDbit;
  vector<Int_t>           MphoID;
  vector<Int_t>           cutflowSMID;
  vector<Int_t>           cutflowVBSID;
  Int_t                   nEle;
  vector<Float_t>         elePt;
  vector<Float_t>         eleD0;
  vector<Float_t>         eleDz;
  vector<Float_t>         eleEta;
  vector<Short_t>         eleIDbit;
  Int_t                   nMu;
  vector<Float_t>         muPt;
  vector<Int_t>           muIDbit;
  vector<Float_t>         muD0;
  vector<Float_t>         muDz;
  Int_t                   nLep;
  Int_t                   nJet;
  Int_t                   nSMjet;
  Int_t                   nVBSjet;
  vector<Int_t>           nonPUjetid;
  vector<Float_t>         jetPt;
  vector<Float_t>         jetEn;
  vector<Float_t>         jetEta;
  vector<Float_t>         jetPhi;
  Double_t                dijetMass;
  vector<Int_t>           jetPUFullID;
  vector<Int_t>           jetID;
  Float_t                 minJMETdPhi;
  vector<Float_t>         JMETdPhi;
  Float_t                 jetjetdEta;
  Float_t                 jetjetdR;
  vector<Float_t>         phojetdR;
  
    
  TTree *outtree_;
  outtree_ = new TTree("EventTree", "EventTree");

  outtree_->Branch("run"                       ,&run                                      ,"run/I"                        );
  outtree_->Branch("event"                     ,&event                                    ,"event/L"                      );
  outtree_->Branch("isData"                    ,&isData                                   ,"isData/O"                     );
  outtree_->Branch("nVtx"                      ,&nVtx                                     ,"nVtx/I"                       );
  outtree_->Branch("rho"                       ,&rho                                      ,"rho/F"                        );
  outtree_->Branch("EventWeight"               ,&EventWeight                              ,"EventWeight/F"                );
  outtree_->Branch("pfMET"                     ,&pfMET                                    ,"pfMET/F"                      );
  outtree_->Branch("pfMETPhi"                  ,&pfMETPhi                                 ,"pfMETPhi/F"                   );
  outtree_->Branch("metFilters"                ,&metFilters                               ,"metFilters/I"                 );
  outtree_->Branch("failedMET"                 ,&failedMET                                ,"failedMET/I"                  );
  outtree_->Branch("nPho"                      ,&nPho                                     ,"nPho/I"                       );
  outtree_->Branch("phoE"                                                                 ,&phoE                          );
  outtree_->Branch("phoEt"                                                                ,&phoEt                         );
  outtree_->Branch("phoEta"                                                               ,&phoEta                        );
  outtree_->Branch("phoPhi"                                                               ,&phoPhi                        );
  outtree_->Branch("phoCalibE"                                                            ,&phoCalibE                     );
  outtree_->Branch("phoCalibEt"                                                           ,&phoCalibEt                    );
  outtree_->Branch("phoSCEta"                                                             ,&phoSCEta                      );
  outtree_->Branch("phohasPixelSeed"                                                      ,&phohasPixelSeed               );
  outtree_->Branch("phoEleVeto"                                                           ,&phoEleVeto                    );
  outtree_->Branch("phoR9"                                                                ,&phoR9                         );
  outtree_->Branch("phoHoverE"                                                            ,&phoHoverE                     );
  outtree_->Branch("phoSigmaIEtaIEtaFull5x5"                                              ,&phoSigmaIEtaIEtaFull5x5       );
  outtree_->Branch("phoR9Full5x5"                                                         ,&phoR9Full5x5                  );
  outtree_->Branch("phoPFChIso"                                                           ,&phoPFChIso                    );
  outtree_->Branch("phoPFPhoIso"                                                          ,&phoPFPhoIso                   );
  outtree_->Branch("phoPFNeuIso"                                                          ,&phoPFNeuIso                   );
  outtree_->Branch("phoPFChWorstIso"                                                      ,&phoPFChWorstIso               );
  outtree_->Branch("phoPFChIso_rc"                                                        ,&phoPFChIso_rc                 );
  outtree_->Branch("phoPFPhoIso_rc"                                                       ,&phoPFPhoIso_rc                );
  outtree_->Branch("phoPFNeuIso_rc"                                                       ,&phoPFNeuIso_rc                );
  outtree_->Branch("phoPFChWorstIso_rc"                                                   ,&phoPFChWorstIso_rc            );
  outtree_->Branch("phoPFChWorstIso_newEA"                                                ,&phoPFChWorstIso_newEA         );
  outtree_->Branch("phoFiredSingleTrgs"                                                   ,&phoFiredSingleTrgs            );
  outtree_->Branch("phoSeedTime"                                                          ,&phoSeedTime                   );
  outtree_->Branch("phoMIPTotEnergy"                                                      ,&phoMIPTotEnergy               );
  outtree_->Branch("phoMETdPhi"                                                           ,&phoMETdPhi                    );
  outtree_->Branch("phoIDbit"                                                             ,&phoIDbit                      );
  outtree_->Branch("MphoID"                                                               ,&MphoID                        );
  outtree_->Branch("cutflowSMID"                                                          ,&cutflowSMID                   );
  outtree_->Branch("cutflowVBSID"                                                         ,&cutflowVBSID                  );
  outtree_->Branch("nEle"                      ,&nEle                                     ,"nEle/I"                       );
  outtree_->Branch("elePt"                                                                ,&elePt                         );
  outtree_->Branch("eleD0"                                                                ,&eleD0                         );
  outtree_->Branch("eleDz"                                                                ,&eleDz                         );
  outtree_->Branch("eleEta"                                                               ,&eleEta                        );
  outtree_->Branch("eleIDbit"                                                             ,&eleIDbit                      );
  outtree_->Branch("nMu"                       ,&nMu                                      ,"nMu/I"                        );
  outtree_->Branch("muPt"                                                                 ,&muPt                          );
  outtree_->Branch("muIDbit"                                                              ,&muIDbit                       );
  outtree_->Branch("muD0"                                                                 ,&muD0                          );
  outtree_->Branch("muDz"                                                                 ,&muDz                          );
  outtree_->Branch("nLep"                      ,&nLep                                     ,"nLep/I"                       );
  outtree_->Branch("nJet"                      ,&nJet                                     ,"nJet/I"                       );
  outtree_->Branch("nSMjet"                    ,&nSMjet                                   ,"nSMjet/I"                     );
  outtree_->Branch("nVBSjet"                   ,&nVBSjet                                  ,"nVBSjet/I"                    );
  outtree_->Branch("nonPUjetid"                                                           ,&nonPUjetid                    );
  outtree_->Branch("jetPt"                                                                ,&jetPt                         );
  outtree_->Branch("jetEn"                                                                ,&jetEn                         );
  outtree_->Branch("jetEta"                                                               ,&jetEta                        );
  outtree_->Branch("jetPhi"                                                               ,&jetPhi                        );
  outtree_->Branch("dijetMass"                 ,&dijetMass                                ,"dijetMass/D"                  );
  outtree_->Branch("jetPUFullID"                                                          ,&jetPUFullID                   );
  outtree_->Branch("jetID"                                                                ,&jetID                         );
  outtree_->Branch("minJMETdPhi"               ,&minJMETdPhi                              ,"minJMETdPhi/F"                );
  outtree_->Branch("JMETdPhi"                                                             ,&JMETdPhi                      );
  outtree_->Branch("jetjetdEta"                ,&jetjetdEta                               ,"jetjetdEta/F"                 );
  outtree_->Branch("jetjetdR"                  ,&jetjetdR                                 ,"jetjetdR/F"                   );
  outtree_->Branch("phojetdR"                                                             ,&phojetdR                      );

  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    //cout << "get ev " << ev << endl;
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);

    run_                          = data.GetInt("run");
    event_			  = data.GetLong64("event");
    isData_			  = data.GetBool("isData");
    nVtx_			  = data.GetInt("nVtx");
    rho_			  = data.GetFloat("rho");
    pfMET_			  = data.GetFloat("pfMET");
    pfMETPhi_			  = data.GetFloat("pfMETPhi");
    //metFilters_		          = data.GetInt("metFilters");
    nPho_			  = data.GetInt("nPho");
    phoE_			  = data.GetPtrFloat("phoE");
    phoEt_			  = data.GetPtrFloat("phoEt");
    phoEta_			  = data.GetPtrFloat("phoEta");
    phoPhi_			  = data.GetPtrFloat("phoPhi");
    phoCalibE_			  = data.GetPtrFloat("phoCalibE");
    phoCalibEt_		          = data.GetPtrFloat("phoCalibEt");
    phoSCEta_			  = data.GetPtrFloat("phoSCEta");
    phohasPixelSeed_		  = data.GetPtrInt("phohasPixelSeed");
    phoEleVeto_		          = data.GetPtrInt("phoEleVeto");
    phoR9_			  = data.GetPtrFloat("phoR9");
    phoHoverE_			  = data.GetPtrFloat("phoHoverE");
    phoSigmaIEtaIEtaFull5x5_      = data.GetPtrFloat("phoSigmaIEtaIEtaFull5x5");
    phoR9Full5x5_		  = data.GetPtrFloat("phoR9Full5x5");
    phoPFChIso_		          = data.GetPtrFloat("phoPFChIso");
    phoPFPhoIso_		  = data.GetPtrFloat("phoPFPhoIso");
    phoPFNeuIso_		  = data.GetPtrFloat("phoPFNeuIso");
    phoPFChWorstIso_		  = data.GetPtrFloat("phoPFChWorstIso");
    phoFiredSingleTrgs_	          = data.GetPtrLong64("phoFiredSingleTrgs");
    phoSeedTime_		  = data.GetPtrFloat("phoSeedTime");
    phoMIPTotEnergy_		  = data.GetPtrFloat("phoMIPTotEnergy");
    phoIDbit_			  = data.GetPtrShort("phoIDbit");
    nEle_			  = data.GetInt("nEle");
    elePt_			  = data.GetPtrFloat("elePt");
    eleD0_			  = data.GetPtrFloat("eleD0");
    eleDz_			  = data.GetPtrFloat("eleDz");
    eleEta_			  = data.GetPtrFloat("eleEta");
    eleIDbit_			  = data.GetPtrShort("eleIDbit");
    nMu_			  = data.GetInt("nMu");
    muPt_ 			  = data.GetPtrFloat("muPt");
    muIDbit_			  = data.GetPtrInt("muIDbit");
    muD0_			  = data.GetPtrFloat("muD0");
    muDz_			  = data.GetPtrFloat("muDz");
    nJet_			  = data.GetInt("nJet");
    jetPt_			  = data.GetPtrFloat("jetPt");
    jetEn_			  = data.GetPtrFloat("jetEn");
    jetEta_			  = data.GetPtrFloat("jetEta");
    jetPhi_			  = data.GetPtrFloat("jetPhi");
    jetPUFullID_		  = data.GetPtrInt("jetPUFullID");
    jetID_			  = data.GetPtrInt("jetID");
    //cout << "get all variable" << endl;
    //init variable

    run = 0;
    event = 0;
    isData = 0;
    nVtx = 0;
    rho = 0;
    EventWeight = 1;
    pfMET = 0;
    pfMETPhi = 0;
    metFilters = 0;
    failedMET = 0;
    nPho = 0;
    phoE.clear();
    phoEt.clear();
    phoEta.clear();
    phoPhi.clear();
    phoCalibE.clear();
    phoCalibEt.clear();
    phoSCEta.clear();
    phohasPixelSeed.clear();
    phoEleVeto.clear();
    phoR9.clear();
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
    cutflowSMID.clear();
    cutflowVBSID.clear();
    nEle = 0;
    elePt.clear();
    eleD0.clear();
    eleDz.clear();
    eleEta.clear();
    eleIDbit.clear();
    nMu = 0;
    muPt.clear();
    muIDbit.clear();
    muD0.clear();
    muDz.clear();
    nLep = 0;
    nJet = 0;
    nSMjet = 0;
    nVBSjet = 0;
    nonPUjetid.clear();
    jetPt.clear();
    jetEn.clear();
    jetEta.clear();
    jetPhi.clear();
    dijetMass = 0.0;
    jetPUFullID.clear();
    jetID.clear();
    minJMETdPhi = 0;
    JMETdPhi.clear();
    jetjetdEta = 0;
    jetjetdR = 0;
    phojetdR.clear();

    
    /*
    //MET filters
    Int_t fail=0;
    for(Int_t ifilt=0; ifilt<8; ifilt++){
      if((metFilters_>>(ifilt+1)&1) == 0) fail++;
    }
    if(fail>0) failedMET = 1;
    */
    
    //rho correction of pho isolation
    vector<vector<Float_t>> Iso_rc; //[ch, pho, nh, chw, chw_newEA]
    Iso_rc.clear();
    for(Int_t iso = 0; iso < 5; iso++){
      vector <Float_t> isolist;
      rhoCorrection(iso, data, isolist);
      Iso_rc.push_back(isolist);
    }
    for(Int_t ipho=0; ipho<nPho_; ipho++){
      phoPFChIso_rc.push_back(Iso_rc[0][ipho]);
      phoPFPhoIso_rc.push_back(Iso_rc[1][ipho]);
      phoPFNeuIso_rc.push_back(Iso_rc[2][ipho]);
      phoPFChWorstIso_rc.push_back(Iso_rc[3][ipho]);
      phoPFChWorstIso_newEA.push_back(Iso_rc[4][ipho]);
    }
    //cout << "get rho corr" << endl;
   
    //lepton veto selection
    Int_t nLele = 0;
    for(Int_t iele=0; iele<nEle_; iele++){
      if((eleIDbit_[iele]>>1&1) == 0 && elePt_[iele] >10) continue;//pass loose
      if(eleD0_[iele] >0.5 && fabs(eleEta_[iele]) <1.4442) continue;
      else if(eleD0_[iele] > 1.0 && fabs(eleEta_[iele]) > 1.5666) continue;
      if(eleDz_[iele] >1.0 && fabs(eleEta_[iele]) <1.4442) continue;
      else if(eleDz_[iele] >2.0 && fabs(eleEta_[iele]) >1.5666) continue;
      nLele++;
    }
    
    Int_t nLmu = 0;
    for(Int_t imu=0; imu<nMu_; imu++){
      if((muIDbit_[imu]>>0&1) == 0 && muPt_[imu] >10) continue;
      if(muD0_[imu] > 2) continue;
      if(muDz_[imu] > 5) continue;
      nLmu++;
    }
    Int_t nlep = 0;
    nLep = nLele + nLmu;
    //cout << "get lepton veto" << endl;
    
    //count # jet
    Int_t npfjet = 0;
    minJMETdPhi = 4;
    for(Int_t i=0; i<nJet_; i++){
      if(year == 2017 && jetPt_[i]<50 && fabs(jetEta_[i])>2.56 && fabs(jetEta_[i])<3.139) jetPt_[i] = 0;//remove 2017 ecal noise
      if(fabs(deltaPhi(jetPhi_[i], pfMETPhi_)) < minJMETdPhi) minJMETdPhi = fabs(deltaPhi(jetPhi_[i], pfMETPhi_));
      if((jetPUFullID_[i] == 6) && (jetPt_[i] > 30) && fabs(jetEta_[i]) < 4.7 && (jetID_[i] == 6)){
	nonPUjetid.push_back(i);
	npfjet++;
      }
    }
    //cout << "get # jets" << endl;
    //phoID selection
    phoIDcut(1, data, MphoID);
    //cout << "get phoIDcut" << endl;
    
    Int_t bit=0;
    TLorentzVector  leadjetP4, subjetP4, dijetP4;
    if(npfjet >= 2){
      jetjetdEta = deltaEta(jetEta_[nonPUjetid[0]], jetEta_[nonPUjetid[1]]);
      jetjetdR = deltaR(jetEta_[nonPUjetid[0]], jetPhi_[nonPUjetid[0]], jetEta_[nonPUjetid[1]], jetPhi_[nonPUjetid[1]]);
      Int_t ilead = nonPUjetid[0];
      leadjetP4.SetPtEtaPhiE(jetPt_[ilead], jetEta_[ilead], jetPhi_[ilead], jetEn_[ilead]);
      Int_t isub = nonPUjetid[1];
      subjetP4.SetPtEtaPhiE(jetPt_[isub], jetEta_[isub], jetPhi_[isub], jetEn_[isub]);
      
      dijetP4 = leadjetP4 + subjetP4;
      dijetMass = dijetP4.M();

      for(Int_t ipho=0; ipho<nPho_; ipho++){
	if(ipho>0) continue;
	phojetdR.push_back(deltaR(phoEta_[ipho], phoPhi_[ipho], jetEta_[nonPUjetid[0]], jetPhi_[nonPUjetid[0]]));
	phojetdR.push_back(deltaR(phoEta_[ipho], phoPhi_[ipho], jetEta_[nonPUjetid[1]], jetPhi_[nonPUjetid[1]]));
      }

      JMETdPhi.push_back(deltaPhi(jetPhi_[nonPUjetid[0]], pfMETPhi_));
      JMETdPhi.push_back(deltaPhi(jetPhi_[nonPUjetid[1]], pfMETPhi_));
    }
    
    else{
      jetjetdEta = -999;
      jetjetdR = -999;
      
      for(Int_t ipho=0; ipho<nPho_; ipho++){
	if(ipho>0) continue;
	phojetdR.push_back(-999);
      }
      
      for(Int_t jj=0; jj<npfjet; jj++){
	Int_t ijet = nonPUjetid[jj];
	JMETdPhi.push_back(deltaPhi(jetPhi_[ijet], pfMETPhi));
      }
    }
    
    for(Int_t ipho=0; ipho<nPho_; ipho++){
      if(ipho > 0) continue;
      phoMETdPhi.push_back(deltaPhi(phoPhi_[ipho], pfMETPhi_));
    }

    //cutflow setbit
    //cout << "get Tlorentz" << endl;
    for(Int_t ipho=0; ipho<nPho_; ipho++){
      if(ipho > 0) continue;
      bit=0;
                                                                                 bit = SetBit(0, bit);
      if(phohasPixelSeed_[ipho] == 0)                                            bit = SetBit(1, bit);
      if(fabs(phoSCEta_[ipho]) < 1.4442)                                         bit = SetBit(2, bit);
      if(phoEt_[ipho] > 210)                                                     bit = SetBit(3, bit);
      if(MphoID[ipho] == 1)                                                      bit = SetBit(4, bit);
      if(Iso_rc[4][ipho] < 1.3)                                                  bit = SetBit(5, bit);
      if(phoSeedTime_[ipho]<3)                                                   bit = SetBit(6, bit);
      if(nLep<1)                                                                 bit = SetBit(7, bit);
      //if(failedMET>0)                                                            bit = SetBit(8, bit);
      if(pfMET_> 140+(0.138*(phoEt_[ipho]-500)))                                 bit = SetBit(8, bit);
      if(fabs(deltaPhi(phoPhi_[ipho], pfMETPhi_)) > 1.2)                         bit = SetBit(9,bit);
      if(minJMETdPhi   > 0.5)                                                    bit = SetBit(10, bit);
      if(npfjet <=1){                                                            bit = SetBit(11, bit); nSMjet = npfjet;}
      if(year == 2016 && (phoFiredSingleTrgs_[ipho]>>7&1) == 1)                  bit = SetBit(12, bit);//singlePho HLT175
      if(year > 2016 && (phoFiredSingleTrgs_[ipho]>>8&1) == 1)                   bit = SetBit(12, bit);//singlePho HLT200
      cutflowSMID.push_back(bit);
      //cout << "get SM cut" << endl;
      bit = ResetBit(11, bit);
      if(npfjet >=2 && jetjetdEta > 2.5 && dijetMass > 500 && (phojetdR[0] > 0.4) && (phojetdR[1] > 0.4) && (jetjetdR > 0.5)){ bit = SetBit(11, bit); nVBSjet = npfjet;}
      cutflowVBSID.push_back(bit);
      //cout << "get VBS cut" << endl;
    }
    //cout << "get setbit" << endl;
    //fill tree
    run = run_;
    event = event_;
    isData = isData_;
    nVtx = nVtx_;
    rho = rho_;
    pfMET = pfMET_;
    pfMETPhi = pfMETPhi_;
    //metFilters = metFilters_;
    nPho = nPho_;
    for(Int_t i=0; i<nPho_; i++){
      phoE.push_back(phoE_[i]);
      phoEt.push_back(phoEt_[i]);
      phoEta.push_back(phoEta_[i]);
      phoPhi.push_back(phoPhi_[i]);
      phoCalibE.push_back(phoCalibE_[i]);
      phoCalibEt.push_back(phoCalibEt_[i]);
      phoSCEta.push_back(phoSCEta_[i]);
      phohasPixelSeed.push_back(phohasPixelSeed_[i]);
      phoEleVeto.push_back(phoEleVeto_[i]);
      phoR9.push_back(phoR9_[i]);
      phoHoverE.push_back(phoHoverE_[i]);
      phoSigmaIEtaIEtaFull5x5.push_back(phoSigmaIEtaIEtaFull5x5_[i]);
      phoR9Full5x5.push_back(phoR9Full5x5_[i]);
      phoPFChIso.push_back(phoPFChIso_[i]);
      phoPFPhoIso.push_back(phoPFPhoIso_[i]);
      phoPFNeuIso.push_back(phoPFNeuIso_[i]);
      phoPFChWorstIso.push_back(phoPFChWorstIso_[i]);
      phoFiredSingleTrgs.push_back(phoFiredSingleTrgs_[i]);
      phoSeedTime.push_back(phoSeedTime_[i]);
      phoMIPTotEnergy.push_back(phoMIPTotEnergy_[i]);
      phoIDbit.push_back(phoIDbit_[i]);
    }
    //cout << "fill photon" << endl;

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
      jetPUFullID.push_back(jetPUFullID_[i]);
      jetID.push_back(jetID_[i]);
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
