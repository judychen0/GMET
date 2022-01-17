#include <TMVA/Reader.h>
#include <vector>
using namespace std;

Float_t max(Float_t a, Float_t b){
  if(a < b) return b;
  else return a;
}

void rhoCorrection(Int_t iso, TreeReader &data, vector<Float_t>& rcIso){
  rcIso.clear();
  Float_t* phoEta = data.GetPtrFloat("phoEta");
  Int_t    nPho = data.GetInt("nPho");
  Float_t* phoEt = data.GetPtrFloat("phoEt");
  Float_t* phoPFChIso = data.GetPtrFloat("phoPFChIso");
  Float_t* phoPFPhoIso = data.GetPtrFloat("phoPFPhoIso");
  Float_t* phoPFNeuIso = data.GetPtrFloat("phoPFNeuIso");
  Float_t* phoPFChWorstIso = data.GetPtrFloat("phoPFChWorstIso");
  Float_t  rho                    = data.GetFloat("rho");

  Int_t iEA = 0;
  
  //[type ch=0, pho=1, nh=2][eta section : iEA]
  
  Float_t EA[3][7] = {{0.0112, 0.0108, 0.0106, 0.01002, 0.0098, 0.0089, 0.000087},
		      {0.0668, 0.1054, 0.0786, 0.0223, 0.0078, 0.0028, 0.0137},
		      {0.1113, 0.0953, 0.0619, 0.0837, 0.1070, 0.1212, 0.1446}
  };//Fall17

  //Float_t EAworst[7] = {0.0728, 0.0709, 0.0106, 0.01002, 0.0098, 0.0089, 0.000087};
  Float_t EAworst[7];
  EAworst[0] = fmax(0.0004*rho+0.0179, 0.0);
  EAworst[1] = fmax(0.0008*rho+0.0106, 0.0);
  /*
  Float_t EA[3][7] = {{ 0.036 , 0.0377 , 0.0306 , 0.0283 , 0.0254 , 0.0217 , 0.0167 },
		      { 0.121 , 0.1107 , 0.0699 , 0.1056 , 0.1457 , 0.1719 , 0.1998 },
		      { 0.0597 , 0.0807 , 0.0629 , 0.0197 , 0.0184 , 0.0284 , 0.0591 }
  };//spring16 
  */

  for(Int_t ipho=0; ipho<nPho; ipho++){
    
    if(fabs(phoEta[ipho] < 1.0)) iEA = 0;
    else if(fabs(phoEta[ipho]) > 1.0 && fabs(phoEta[ipho]) < 1.479) iEA = 1;
    else if(fabs(phoEta[ipho]) > 1.479 && fabs(phoEta[ipho]) < 2.0) iEA = 2;
    else if(fabs(phoEta[ipho]) > 2.0 && fabs(phoEta[ipho]) < 2.2) iEA = 3;
    else if(fabs(phoEta[ipho]) > 2.2 && fabs(phoEta[ipho]) < 2.3) iEA = 4;
    else if(fabs(phoEta[ipho]) > 2.3 && fabs(phoEta[ipho]) < 2.4) iEA = 5;
    else if(fabs(phoEta[ipho]) > 2.4) iEA = 6;

    
    if(iso == 0) rcIso.push_back(fmax(phoPFChIso[ipho] - rho*EA[iso][iEA], 0.0));
    else if(iso == 1) rcIso.push_back(fmax(phoPFPhoIso[ipho] - rho*EA[iso][iEA], 0.0));
    else if(iso == 2) rcIso.push_back(fmax(phoPFNeuIso[ipho] - rho*EA[iso][iEA], 0.0));
    else if(iso == 3) rcIso.push_back(fmax(phoPFChWorstIso[ipho] - rho*EA[0][iEA], 0.0));
    else if(iso == 4) rcIso.push_back(fmax(phoPFChWorstIso[ipho] - rho*EAworst[iEA], 0.0));
  }
}


void rawIsolation(Int_t iso, TreeReader &data, vector<Float_t>& rcIso){
  rcIso.clear();
  
  Float_t* phoPFChIso = data.GetPtrFloat("phoPFChIso");
  Float_t* phoPFPhoIso = data.GetPtrFloat("phoPFPhoIso");
  Float_t* phoPFNeuIso = data.GetPtrFloat("phoPFNeuIso");
  Float_t* phoPFChWorstIso = data.GetPtrFloat("phoPFChWorstIso");
  Int_t    nPho = data.GetInt("nPho");
 
  for(Int_t ipho=0; ipho<nPho; ipho++){
    if(iso == 0) rcIso.push_back(phoPFChIso[ipho]);
    else if(iso == 1) rcIso.push_back(phoPFPhoIso[ipho]);
    else if(iso == 2) rcIso.push_back(phoPFNeuIso[ipho]);
    else if(iso == 3) rcIso.push_back(phoPFChWorstIso[ipho]);
    else if(iso == 4) rcIso.push_back(0);
  }
  
  
}
