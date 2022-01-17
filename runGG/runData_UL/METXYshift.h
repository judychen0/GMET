#include "TMath.h"
#include <TMVA/Reader.h>
#include "TTree.h"
#include "TVector2.h"
#include <string.h>
#include <stdio.h>
#include <vector>

using namespace std;

void METXYshift(Int_t year, char* sample, TreeReader &data, vector<float>& corrXY){

  string samplelist16[30] = {
			     "job_SinglePho_Run2016B_UL",
			     "job_SinglePho_Run2016C_UL",
			     "job_SinglePho_Run2016D_UL",
			     "job_SinglePho_Run2016E_UL",
			     "job_SinglePho_Run2016F1_UL",
			     "job_SinglePho_Run2016F2_UL",
			     "job_SinglePho_Run2016G_UL",
			     "job_SinglePho_Run2016H_UL"
  };

  string samplelist17[30] = {
			     "job_SinglePho_Run2017B_UL",
			     "job_SinglePho_Run2017C_UL",
			     "job_SinglePho_Run2017D_UL",
			     "job_SinglePho_Run2017E_UL",
			     "job_SinglePho_Run2017F_UL",
  };

  string samplelist18[30] = {
			     "job_EGamma_Run2018A_UL",
			     "job_EGamma_Run2018B_UL",
			     "job_EGamma_Run2018C_UL",
			     "job_EGamma_Run2018D_UL"
	};

  Float_t CorrX16[2][30] = {
			    {  0.0288,  0.0170, 0.0013, -0.0343, -0.0279,  0.1644,  0.1328,  0.0970},
			    { -0.5333, -0.1331, 0.1694,  0.7134,  0.4479, -0.8640, -0.2230, -0.4257}
  };//data
  Float_t CorrY16[2][30] = {
			    { 0.0651, 0.1079, 0.0926, 0.1316, 0.0980, 0.0356, 0.0409, 0.0761},
			    { 1.4410, 1.2550, 1.6560, 1.3860, 1.9890, 1.2430, 1.3050, 1.3980}
  };//data
  
  Float_t CorrX17[2][30] = {
			    { -0.2577, -0.2113, -0.2423, -0.1815, -0.2802},
			    {  0.2768, -0.4416,  0.1877, -0.4525,  0.6794}
  };//data
  Float_t CorrY17[2][30] = {
			    {  0.3083,  0.2294, 0.2047, 0.1340, 0.2281},
			    { -0.8289, -0.1217, 0.1419, 0.8226, 0.1968}
  };//data

  Float_t CorrX18[2][30] = {
			    { 0.28741,  0.47724,  0.50605,  0.53609},
			    {-1.74211, -3.46691, -1.28811, -1.31469}
  };
  Float_t CorrY18[2][30] = {
			    { 0.02961,  0.16469,  0.07929,  0.07958},
			    { 0.28504, -0.23183, -2.20700, -1.41992}
  };


  //function start//
  Double_t corrMET, corrMETPhi;
  corrMET = 0;
  corrMETPhi = 0;
  corrXY.clear();

  Int_t nVtx = data.GetInt("nGoodVtx");
  Float_t pfMET = data.GetFloat("pfMET");
  Float_t pfMETPhi = data.GetFloat("pfMETPhi");

  Float_t pfMETX = pfMET * cos(pfMETPhi);
  Float_t pfMETY = pfMET * sin(pfMETPhi);
  
  string samplestr = sample;
  Int_t istr = samplestr.find("job");
  Int_t fstr = samplestr.find("/ggtree");
  Int_t lstr = fstr - istr;
  string samplename = samplestr.substr(istr, lstr);
  
  if(year == 2016){
    for(Int_t i=0; i<30; i++){
      if(samplename == samplelist16[i]){
	pfMETX -= CorrX16[0][i]*nVtx + CorrX16[1][i];
	pfMETY -= CorrY16[0][i]*nVtx + CorrY16[1][i];
	break;

      }
    }
    
  }
  else if(year == 2017){
     for(Int_t i=0; i<30; i++){
      if(samplename == samplelist17[i]){
	pfMETX -= CorrX17[0][i]*nVtx + CorrX17[1][i];
	pfMETY -= CorrY17[0][i]*nVtx + CorrY17[1][i];
	break;

      }
    }
  }
  else if(year == 2018){
    for(Int_t i=0; i<30; i++){
      if(samplename == samplelist18[i]){
	pfMETX -= CorrX18[0][i]*nVtx + CorrX18[1][i];
	pfMETY -= CorrY18[0][i]*nVtx + CorrY18[1][i];
      }
    }
  }

  TVector2 *CMET = new TVector2(pfMETX, pfMETY);
  corrMET = CMET->Mod();
  corrMETPhi = CMET->Phi();
  corrXY.push_back(corrMET);
  corrXY.push_back(corrMETPhi);

  
}


