#include <TMath.h>
#include <TMVA/Reader.h>
#include <TTree.h>
#include <vector>
using namespace std;

void muIDcut(Int_t iWP, TreeReader &data,  vector<int>& passed){
  //iWP[L/M/T][0/1/3]
  passed.clear();
  //load from data
  Int_t    nMu = data.GetInt("nMu");
  Float_t* muPt = data.GetPtrFloat("muPt");
  Float_t* muEta = data.GetPtrFloat("muEta");
  Int_t* muID         = data.GetPtrInt("muIDbit");

  //Int_t nPho = realpho.size();
  for(Int_t imu=0; imu < nMu; imu++){
    //if((phoID[ipho]>>iWP&1) == 0) continue;
    //passed.push_back(ipho);
    Int_t pass =0;
    //if()
    if((muID[imu]%1) == 0) pass =1;
    else pass =0;
    passed.push_back(pass);
  }
}
