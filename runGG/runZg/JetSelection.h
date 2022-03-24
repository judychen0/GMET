#include <TMath.h>
#include <TMVA/Reader.h>
#include <TTree.h>
#include <vector>
using namespace std;

//UL datasets
void jet_TIDsel(Int_t year, TreeReader &data, vector<int>& passed){

  passed.clear();
  Int_t nJet = data.GetInt("nJet");
  Float_t* jetEta = data.GetPtrFloat("jetEta");
  Float_t* jetCHF = data.GetPtrFloat("jetCHF");
  Float_t* jetCEF = data.GetPtrFloat("jetCEF");
  Int_t*   jetNCH = data.GetPtrInt("jetNCH");
  Float_t* jetNHF = data.GetPtrFloat("jetNHF");
  Float_t* jetNEF = data.GetPtrFloat("jetNEF");
  Int_t*   jetNNP = data.GetPtrInt("jetNNP");
  Float_t* jetMUF = data.GetPtrFloat("jetMUF");

  //[iyear][ieta]
  Float_t jetCHF_cut[2][4] = {{0, -1, -1, -1}, {0, -1, -1, -1}};
  Float_t jetCEF_cut[2][4] = {{0.8, -1, -1, -1}, {0.8, 0.8, -1, -1}};
  Float_t jetNCH_cut[2][4] = {{0, -1, -1, -1}, {0, 0, -1, -1}};
  Float_t jetNHF_cut[2][4] = {{0.9, 0.9, 0.9, 0.2}, {0.9, 0.9, -1, 0.2}};
  Float_t jetNEF_cut[2][4] = {{0.9, 0.99, 0.99, 0.9}, {0.9, 0.99, 0.99, 0.9}};
  Float_t jetNNP_cut[2][4] = {{-1, -1, 1, 10}, {-1, -1, 1, 10}};
  Float_t jetMUF_cut[2][4] = {{0.8, -1, -1, -1}, {0.8, 0.8, -1, -1}};
  
  Int_t iEta = 0;
  vector<Int_t> jetlist; jetlist.clear();
  if(year == 2016){
    Int_t iyear = 0;
    for(Int_t ijet=0; ijet<nJet; ijet++){
      if(fabs(jetEta[ijet]) <= 2.4) iEta = 0;
      else if(fabs(jetEta[ijet]) > 2.4 && fabs(jetEta[ijet]) <= 2.7) iEta = 1;
      else if(fabs(jetEta[ijet]) > 2.7 && fabs(jetEta[ijet]) <= 3.0) iEta = 2;
      else if(fabs(jetEta[ijet]) > 3.0 && fabs(jetEta[ijet]) <= 5.0) iEta = 3;

      if(jetCHF[ijet] < jetCHF_cut[iyear][iEta] && jetCHF_cut[iyear][iEta] > 0) continue;
      //if(jetCEF[ijet] > jetCEF_cut[iyear][iEta] && jetCEF_cut[iyear][iEta] > 0) continue;
      if(jetNCH[ijet] < jetNCH_cut[iyear][iEta] && jetNCH_cut[iyear][iEta] > 0) continue;
      if(iEta == 3){
	if(jetNHF[ijet] < jetNHF_cut[iyear][iEta] && jetNHF_cut[iyear][iEta] > 0) continue;
	if((jetNEF[ijet] < jetNEF_cut[iyear][iEta] && jetNEF[ijet] > 0.0) && jetNEF_cut[iyear][iEta] > 0) continue;
      }
      else{
	if(jetNHF[ijet] > jetNHF_cut[iyear][iEta] && jetNHF_cut[iyear][iEta] > 0) continue;
	if(jetNEF[ijet] > jetNEF_cut[iyear][iEta] && jetNEF_cut[iyear][iEta] > 0) continue;
      }
      //if(jetMUF[ijet] > jetMUF_cut[iyear][iEta] && jetMUF_cut[iyear][iEta] > 0) continue;
      if(jetNNP[ijet] < jetNNP_cut[iyear][iEta] && jetNNP_cut[iyear][iEta] > 0) continue;
      jetlist.push_back(ijet);
    }
  }
  else{
    Int_t iyear = 1;
    for(Int_t ijet=0; ijet<nJet; ijet++){
      if(fabs(jetEta[ijet]) <= 2.6) iEta = 0;
      else if(fabs(jetEta[ijet]) > 2.6 && fabs(jetEta[ijet]) <= 2.7) iEta = 1;
      else if(fabs(jetEta[ijet]) > 2.7 && fabs(jetEta[ijet]) <= 3.0) iEta = 2;
      else if(fabs(jetEta[ijet]) > 3.0 && fabs(jetEta[ijet]) <= 5.0) iEta = 3;

      if(jetCHF[ijet] < jetCHF_cut[iyear][iEta] && jetCHF_cut[iyear][iEta] > 0) continue;
      //if(jetCEF[ijet] > jetCEF_cut[iyear][iEta] && jetCHF_cut[iyear][iEta] > 0) continue;
      if(jetNCH[ijet] < jetNCH_cut[iyear][iEta] && jetNCH_cut[iyear][iEta] > 0) continue;
      if(iEta == 3){
	if(jetNHF[ijet] < jetNHF_cut[iyear][iEta] && jetNHF_cut[iyear][iEta] > 0) continue;
	if((jetNEF[ijet] < jetNEF_cut[iyear][iEta] && jetNEF[ijet] > 0.0) && jetNEF_cut[iyear][iEta] > 0) continue;
      }
      else{
	if(jetNHF[ijet] > jetNHF_cut[iyear][iEta] && jetNHF_cut[iyear][iEta] > 0) continue;
	if(jetNEF[ijet] > jetNEF_cut[iyear][iEta] && jetNEF_cut[iyear][iEta] > 0) continue;
      }
      //if(jetMUF[ijet] > jetMUF_cut[iyear][iEta] && jetMUF_cut[iyear][iEta] > 0) continue;
      if(jetNNP[ijet] < jetNNP_cut[iyear][iEta] && jetNNP_cut[iyear][iEta] > 0) continue;
      jetlist.push_back(ijet);
    }
    
  }
  

  for(Int_t ijet=0; ijet<nJet; ijet++){
    Int_t pass = 0;
    for(Int_t ii=0; ii<jetlist.size(); ii++){
      if(ijet == jetlist[ii]) {
	pass = 1;
	break;
      }
    }
    passed.push_back(pass);
  }
}

//UL datasets
void jet_TPUsel(TreeReader &data, vector<int>& passed){
  passed.clear();
  Int_t nJet = data.GetInt("nJet");
  Float_t* jetPt = data.GetPtrFloat("jetPt");
  Float_t* jetEta = data.GetPtrFloat("jetEta");
  Float_t* jetPUID = data.GetPtrFloat("jetPUID");

  Float_t jetPUID_dis[4][4] = {{0.77, 0.38, -0.31, -0.21},
			       {0.90, 0.60, -0.12, -0.13},
			       {0.96, 0.82, 0.20, 0.09},
			       {0.98, 0.92, 0.47, 0.29}
  };//[ipt][ieta](tight)

  Int_t iPt = 0;Int_t iEta = 0;
  vector<Int_t> jetPUlist; jetPUlist.clear();
  for(Int_t ijet=0; ijet<nJet; ijet++){
    if(jetPt[ijet] >= 10 && jetPt[ijet] < 20) iPt = 0;
    else if(jetPt[ijet] >= 20 && jetPt[ijet] < 30) iPt = 1;
    else if(jetPt[ijet] >= 30 && jetPt[ijet] < 40) iPt = 2;
    else if(jetPt[ijet] >= 40 && jetPt[ijet] < 50) iPt = 3;

    if(fabs(jetEta[ijet]) < 2.5) iEta = 0;
    else if(fabs(jetEta[ijet]) >= 2.5 && fabs(jetEta[ijet]) < 2.75) iEta = 1;
    else if(fabs(jetEta[ijet]) >= 2.7 && fabs(jetEta[ijet]) < 3.0) iEta = 2;
    else if(fabs(jetEta[ijet]) >= 3.0 && fabs(jetEta[ijet]) < 5.0) iEta = 3;

    if(jetPUID[ijet] > jetPUID_dis[iPt][iEta]) continue;
    jetPUlist.push_back(ijet);
  }

  for(Int_t ijet=0; ijet<nJet; ijet++){
    Int_t pass = 0;
    for(Int_t ii=0; ii<jetPUlist.size(); ii++){
      if(ijet == jetPUlist[ii]){
	pass = 1;
	break;
      }
    }
    passed.push_back(pass);
  }
  
}

 
