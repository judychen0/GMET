#include <TMath.h>
#include <TMVA/Reader.h>
#include <TTree.h>
#include <vector>
using namespace std;

Int_t pho_preselection(TreeReader &data, Int_t ipho, Bool_t hasSeed=kTRUE){
  Int_t phoID=1;
  Float_t* phoEt = data.GetPtrFloat("phoEt");
  Float_t* phoEta = data.GetPtrFloat("phoEta");
  Int_t*   phohasPixelSeed     = data.GetPtrInt("phohasPixelSeed");
  Int_t*   phoEleVeto          = data.GetPtrInt("phoEleVeto");
  Float_t* phoSigmaIEtaIEta    = data.GetPtrFloat("phoSigmaIEtaIEtaFull5x5");
  Float_t* phoHoverE           = data.GetPtrFloat("phoHoverE");
  Float_t* phoPFPhoIso         = data.GetPtrFloat("phoPFPhoIso");
  Float_t* phoPFChWorstIso   = data.GetPtrFloat("phoPFChWorstIso");
  Long64_t* phoFiredTrgs = data.GetPtrLong64("phoFiredSingleTrgs");

  if(fabs(phoEta[ipho]) > 1.4442) phoID=0;
  if((phoFiredTrgs[ipho]>>6&1) == 0) phoID=0;//102X 2016 HLT_175
  //if(phoPFChWorstIso[ipho] > 15.) phoID=0;
  if(hasSeed && phohasPixelSeed[ipho] == 1) phoID=0;
  //if(phoEt[ipho] < 200) phoID=0;
  return phoID;
}

Int_t pho_chselection(TreeReader &data, Int_t ipho, Bool_t eleVeto=kTRUE){
  Int_t phoID=1;
  Float_t* phoEt = data.GetPtrFloat("phoEt");
  Float_t* phoEta = data.GetPtrFloat("phoEta");
  Short_t* phoIDbit       = data.GetPtrShort("phoIDbit");

  if(phoEt[ipho] <15) phoID = 0;
  if(phoEta[ipho] >1.4442) phoID = 0;
  if((phoIDbit[ipho]>>1&1) == 0) phoID = 0;//pass M ID

  return phoID;
}
void pho_selection(Int_t iWP, Int_t region, TreeReader &data, vector<int>& passed){

  passed.clear();
  //load from data
  Int_t    nPho = data.GetInt("nPho");
  Float_t* phoEt = data.GetPtrFloat("phoEt");
  Float_t* phoEta = data.GetPtrFloat("phoEta");
  Float_t* phoHoverE = data.GetPtrFloat("phoHoverE");
  Float_t* phoSigmaIEtaIEtaFull5x5 = data.GetPtrFloat("phoSigmaIEtaIEtaFull5x5");
  Float_t* phoPFChIso = data.GetPtrFloat("phoPFChIso");
  Float_t* phoPFPhoIso = data.GetPtrFloat("phoPFPhoIso");
  Float_t* phoPFNeuIso = data.GetPtrFloat("phoPFNeuIso");
  Float_t  rho = data.GetFloat("rho");

  //[iWP L/M/T][region EB/EE]fall17
  Float_t chIso_WP[3][2] = {{1.694, 2.089},{1.141, 1.051}, {0.65, 0.517}};
  Float_t phoIso_WP[3][2] = {{2.876, 4.162}, {2.08, 3.867}, {2.044, 3.032}};
  Float_t nhIso_WP[3][2] = {{24.032, 19.722}, {1.189, 2.718}, {0.317, 2.716}};
  Float_t HoverE_WP[3][2] = {{0.04596, 0.059}, {0.02197, 0.0326}, {0.02148, 0.0321}};
  Float_t sieie_WP[3][2] = {{0.0106, 0.0272}, {0.01015, 0.0272}, {0.00996, 0.0271}};

  //Int_t phoEB = 0;
  Int_t iEA = 0;
  
  //[type ch=0, pho=1, nh=2][eta section : iEA]
  
  Float_t EA[3][7] = {{0.0112, 0.0108, 0.0106, 0.01002, 0.0098, 0.0089, 0.000087},
		      {0.0668, 0.1054, 0.0786, 0.0223, 0.0078, 0.0028, 0.0137},
		      {0.1113, 0.0953, 0.0619, 0.0837, 0.1070, 0.1212, 0.1446}
  };//fall17 v2ID
  /*
  Float_t EA[3][7] = {{ 0.036 , 0.0377 , 0.0306 , 0.0283 , 0.0254 , 0.0217 , 0.0167 },
		      { 0.121 , 0.1107 , 0.0699 , 0.1056 , 0.1457 , 0.1719 , 0.1998 },
		      { 0.0597 , 0.0807 , 0.0629 , 0.0197 , 0.0184 , 0.0284 , 0.0591 }
  };//spring16 

  Float_t EA[3][7] = {{ 0.012 , 0.010 , 0.014 , 0.012 , 0.016 , 0.020 , 0.012 },
		      { 0.148 , 0.130 , 0.112 , 0.216 , 0.262 , 0.260 , 0.266 },
		      { 0.030 , 0.057 , 0.039 , 0.015 , 0.024 , 0.039 , 0.072 }
  };//2015
  */
  vector<Int_t> pho_MID; pho_MID.clear();
  for(Int_t ipho=0; ipho<nPho; ipho++){

    if(fabs(phoEt[ipho]) < 15.) continue;
    
    if(fabs(phoEta[ipho] < 1.0)) iEA = 0;
    else if(fabs(phoEta[ipho]) > 1.0 && fabs(phoEta[ipho]) < 1.479) iEA = 1;
    else if(fabs(phoEta[ipho]) > 1.479 && fabs(phoEta[ipho]) < 2.0) iEA = 2;
    else if(fabs(phoEta[ipho]) > 2.0 && fabs(phoEta[ipho]) < 2.2) iEA = 3;
    else if(fabs(phoEta[ipho]) > 2.2 && fabs(phoEta[ipho]) < 2.3) iEA = 4;
    else if(fabs(phoEta[ipho]) > 2.3 && fabs(phoEta[ipho]) < 2.4) iEA = 5;
    else if(fabs(phoEta[ipho]) > 2.4) iEA = 6;

    if( fabs(phoEta[ipho]) > 1.4442) continue;
    //else if(region ==1 && (fabs(phoEta[ipho]) < 1.566 || fabs(phoEta[ipho]) > 2.5)) continue;"/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_WGToLNuG_01J_5f_PtG_130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/210531_110600/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZLLGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph/210531_120250/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph-pythia8/210531_164904/output_ggtree.root",
    
    if(phoHoverE[ipho] > HoverE_WP[iWP][region]) continue;
    
    Float_t corr = 0.0;
    //chIso cut
    corr = phoPFChIso[ipho] - rho*EA[0][iEA];
    if( corr >= chIso_WP[iWP][region]) continue;
    //if(phoPFChIso[ipho] >= chIso_WP[iWP][region]) continue;


    //nhIso cut
    corr = phoPFNeuIso[ipho] - rho*EA[2][iEA];
    Float_t corrnh[2] = {0.01512, 0.0117};
    Float_t corrnh_2nd[2] = {0.00002259, 0.000023};
    nhIso_WP[iWP][region] += (corrnh[region]*phoEt[ipho] + corrnh_2nd[region]*phoEt[ipho]*phoEt[ipho]);
    if( corr >= nhIso_WP[iWP][region]) continue;
    //if(phoPFNeuIso[ipho] >= nhIso_WP[iWP][region]) continue;    

    //phoIso cut
    corr = phoPFPhoIso[ipho] - rho*EA[1][iEA];
    Float_t corrpho[2] = {0.004017, 0.0037};
    phoIso_WP[ipho][region] += (corrpho[region]*phoEt[ipho]);
    if( corr >= phoIso_WP[iWP][region]) continue;
    //if(phoPFPhoIso[ipho] >= phoIso_WP[iWP][region]) continue;

    
    if( phoSigmaIEtaIEtaFull5x5[ipho] > sieie_WP[iWP][region]) continue;
    pho_MID.push_back(ipho);
  }

  for(Int_t ipho=0; ipho<nPho; ipho++){
    Int_t pass =0;
    for(Int_t ii=0; ii<pho_MID.size(); ii++){
      if(ipho == pho_MID[ii]) pass = 1;
    }
    passed.push_back(pass);
  }
}

void phoIDcut(Int_t iWP, TreeReader &data,  vector<int>& passed){
  //iWP[L/M/T][0/1/2]
  passed.clear();
  //load from data
  Int_t    nPho = data.GetInt("nPho");
  Float_t* phoEt = data.GetPtrFloat("phoEt");
  Float_t* phoEta = data.GetPtrFloat("phoEta");
  Short_t* phoID         = data.GetPtrShort("phoIDbit");

  //Int_t nPho = realpho.size();
  for(Int_t ipho=0; ipho < nPho; ipho++){
    //if((phoID[ipho]>>iWP&1) == 0) continue;
    //passed.push_back(ipho);
    Int_t pass =0;
    //if()
    if((phoID[ipho]>>iWP&1) == 1) pass =1;
    else pass =0;
    passed.push_back(pass);
  }
}
