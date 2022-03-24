#include <TMath.h>
#include <TMVA/Reader.h>
#include <TTree.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

/*
Bool_t pho_presel(Int_t iWP, TreeReader &data){
  Int_t    nPho = data.GetInt("nPho");
  Int_t* hasSeed = data.GetPtrInt("phohasPixelSeed");
  Float_t* phoEt = data.GetPtrFloat("phoEt");
  Float_t* phoSCEta = data.GetPtrFloat("phoSCEta");
  Short_t* phoID         = data.GetPtrShort("phoIDbit");

  Int_t pass =0;
  for(Int_t ipho=0; ipho<nPho; ipho++){
    if(hasSeed[ipho] == 1 && phoEt[ipho]>210 && phoSCEta[ipho]<1.4442 && ((phoID[ipho]>>iWP&1) == 1)) pass =1;
    else pass=0;
  }
}
*/

void pho_IDselection(Int_t iWP, Int_t region, TreeReader &data, vector<int>& passed){
  //void pho_IDselection(Int_t iWP, TreeReader &data, vector<int>& passed){  
  //region = 0;
  passed.clear();

  ofstream ftext;
  ftext.open("Errmessage.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  
  //load from data
  Int_t    nPho = data.GetInt("nPho");
  Float_t* phoEt = data.GetPtrFloat("phoEt");
  Float_t* phoEta = data.GetPtrFloat("phoEta");
  Float_t* phoHoverE = data.GetPtrFloat("phoHoverE");
  Float_t* phoSigmaIEtaIEtaFull5x5 = data.GetPtrFloat("phoSigmaIEtaIEtaFull5x5");
  //Float_t* phoPFChIso = data.GetPtrFloat("phoPFChIso");
  Float_t* phoPFChWorstIso_newEA = data.GetPtrFloat("phoPFChWorstIso_newEA");
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
  ftext << "";
  vector<Int_t> pho_MID; pho_MID.clear();
  for(Int_t ipho=0; ipho<nPho; ipho++){

    //if(fabs(phoEt[ipho]) < 15.) continue;
    
    if(fabs(phoEta[ipho] < 1.0)) iEA = 0;
    else if(fabs(phoEta[ipho]) > 1.0 && fabs(phoEta[ipho]) < 1.479) iEA = 1;
    else if(fabs(phoEta[ipho]) > 1.479 && fabs(phoEta[ipho]) < 2.0) iEA = 2;
    else if(fabs(phoEta[ipho]) > 2.0 && fabs(phoEta[ipho]) < 2.2) iEA = 3;
    else if(fabs(phoEta[ipho]) > 2.2 && fabs(phoEta[ipho]) < 2.3) iEA = 4;
    else if(fabs(phoEta[ipho]) > 2.3 && fabs(phoEta[ipho]) < 2.4) iEA = 5;
    else if(fabs(phoEta[ipho]) > 2.4) iEA = 6;

    if( fabs(phoEta[ipho]) > 1.4442) continue;
    //else if(region ==1 && (fabs(phoEta[ipho]) < 1.566 || fabs(phoEta[ipho]) > 2.5)) continue;
    
    if(phoHoverE[ipho] > HoverE_WP[iWP][region]) continue;
    
    Float_t corr = 0.0;
    //chIso cut
    //corr = phoPFChIso[ipho] - rho*EA[0][iEA];
    //if( corr >= chIso_WP[iWP][region]) continue;
    //if(phoPFChIso[ipho] >= chIso_WP[iWP][region]) continue;
    if(phoPFChWorstIso_newEA[ipho] >= chIso_WP[iWP][region]) continue;
    ftext << "";
    
    //nhIso cut
    corr = phoPFNeuIso[ipho] - rho*EA[2][iEA];
    Float_t corrnh[2] = {0.01512, 0.0117};
    Float_t corrnh_2nd[2] = {0.00002259, 0.000023};
    nhIso_WP[iWP][region] += (corrnh[region]*phoEt[ipho] + corrnh_2nd[region]*phoEt[ipho]*phoEt[ipho]);
    if( corr >= nhIso_WP[iWP][region]) continue;
    ftext << "";
    //if(phoPFNeuIso[ipho] >= nhIso_WP[iWP][region]) continue;    )
    
    //phoIso cut
    corr = phoPFPhoIso[ipho] - rho*EA[1][iEA];
    Float_t corrpho[2] = {0.004017, 0.0037};
    phoIso_WP[ipho][region] += (corrpho[region]*phoEt[ipho]);
    if( corr >= phoIso_WP[iWP][region]) continue;
    ftext << "";
    //if(phoPFPhoIso[ipho] >= phoIso_WP[iWP][region]) continue;

    
    if( phoSigmaIEtaIEtaFull5x5[ipho] > sieie_WP[iWP][region]) continue;
    ftext << "";
    pho_MID.push_back(ipho);
  }

  ftext << "";
  for(Int_t ipho=0; ipho<nPho; ipho++){
    Int_t pass =0;
    for(Int_t ii=0; ii<pho_MID.size(); ii++){
      if(ipho == pho_MID[ii]) pass = 1;
    }
    passed.push_back(pass);
  }
  
  ftext.close();
}


void phoIDcut(Int_t iWP, Int_t year, TreeReader &data,  vector<int>& passed){
  //iWP[L/M/T][0/1/2]
  passed.clear();
  //load from data
  Int_t    nPho = data.GetInt("nPho");
  Float_t* phoEt = data.GetPtrFloat("phoEt");
  Float_t* phoEta = data.GetPtrFloat("phoEta");
  Short_t* phoID         = data.GetPtrShort("phoIDbit");
  Long64_t*     phoFiredSingleTrgs = data.GetPtrLong64(	"phoFiredSingleTrgs"		);

  //Int_t nPho = realpho.size();
  for(Int_t ipho=0; ipho < nPho; ipho++){
    if(year == 2016 && (phoFiredSingleTrgs[ipho]>>7&1) == 0) continue;
      else if(year == 2017 && (phoFiredSingleTrgs[ipho]>>8&1) == 0) continue;
      else if(year == 2018 && (phoFiredSingleTrgs[ipho]>>7&1) == 0) continue;
    //if((phoID[ipho]>>iWP&1) == 0) continue;
    //passed.push_back(ipho);
    Int_t pass =0;
    //if()
    if((phoID[ipho]>>iWP&1) == 1) pass =1;
    else pass =0;
    passed.push_back(pass);
  }
}

Int_t pho_sel(Int_t IDbit, Int_t ibit){
  Int_t ncut = 17;
  Int_t testbit=0;
  
  Int_t nfail=0;
  for(Int_t i=0; i<ibit+1; i++){
    if((IDbit>>i&1) == 0) nfail++;
  }
  if(nfail > 0) return 0;
  else return 1;
}

Int_t Nm1_sel(Int_t IDbit, Int_t ibit){
  Int_t ncut = 17;
  Int_t testbit =0;

  for(Int_t i=0; i<ncut; i++){
    testbit |= (1<<i);
  }
  Int_t compare = (IDbit ^ testbit);
  
  Int_t npass=0;
  for(Int_t i=0; i<ncut; i++){
    if(i!=ibit){
      if((IDbit>>i&1) == 1) npass++;
    }
  }
  if(npass == ncut-1) return 1;
  else return 0;
 
}
