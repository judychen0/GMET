#include <TMath.h>
#include <TMVA/Reader.h>
#include <TTree.h>
#include <vector>
using namespace std;

void eleIDcut(Int_t iWP, TreeReader &data,  vector<int>& passed){
  //iWP[L/M/T][1/2/3]
  passed.clear();
  //load from data
  Int_t    nEle = data.GetInt("nEle");
  Float_t* elePt = data.GetPtrFloat("elePt");
  Float_t* eleEta = data.GetPtrFloat("eleEta");
  Short_t* eleID         = data.GetPtrShort("eleIDbit");

  //Int_t nPho = realpho.size();
  for(Int_t iele=0; iele < nEle; iele++){
    //if((phoID[ipho]>>iWP&1) == 0) continue;
    //passed.push_back(ipho);
    Int_t pass =0;
    //if()
    if((eleID[iele]>>iWP&1) == 1) pass =1;
    else pass =0;
    passed.push_back(pass);
  }
}
