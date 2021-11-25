#include "TMath.h"
#include <TMVA/Reader.h>
#include "TTree.h"
#include <string.h>
#include <stdio.h>
#include <vector>

using namespace std;

void METXYshift(Int_t year, char* sample, TreeReader &data, Float_t pfMET, vector<Float_t>& corrXY){
  
  

  
  string samplelist16[30] = {
			     "job_summer16_GJets_MG_HT200to400",
			     "job_summer16_GJets_MG_HT400to600",
			     "job_summer16_GJets_MG_HT600toInf",
			     "job_summer16_Wg_aMCatNLO",
			     "job_summer16_Wg_aMCatNLO_ext1",
			     "job_summer16_Zg_aMCatNLO",
			     "job_summer16_TT_aMCatNLO",
			     "job_summer16_Znunug_pt130"
  };

  string samplelist17[30] = {
			     "job_fall17_GJets_MG_HT200to400",
			     "job_fall17_GJets_MG_HT400to600",
			     "job_fall17_GJets_MG_HT600toInf",
			     "job_fall17_Wg_aMCatNLO",
			     "job_fall17_Wg_aMCatNLO_ext1",
			     "job_fall17_Zg_aMCatNLO",
			     "job_fall17_TT_aMCatNLO",
			     "job_fall17_Znunug_pt130",
			     "job_UL17_Znunug_VBS"
  };

  string samplelist18[30] = {
			     "job_autumn18_GJets_MG_HT200to400",
			     "job_autumn18_GJets_MG_HT400to600",
			     "job_autumn18_GJets_MG_HT600toInf",
			     "job_autumn18_Wg_aMCatNLO",
			     "job_autumn18_Wg_aMCatNLO_ext1",
			     "job_autumn18_Zg_aMCatNLO",
			     "job_autumn18_TT_aMCatNLO_ext1",
			     "job_autumn18_TT_aMCatNLO_ext2",
			     "job_autumn18_Znunug_pt130",
			     "job_autumn18_Znunug_VBS"
  };

  Float_t CorrX16[2][30] = {
			    {-0.2131, -0.2141, -0.2128, -0.2158, -0.2135, -0.2125, -0.2135},
			    {-0.5195, -0.6019, -0.6671, -0.2198, -0.2539, -0.2701, -0.6740}
  };

  Float_t CorrY16[2][30] = {
			    {-0.04922, -0.05463, -0.06143, -0.05038, -0.05179, -0.04736, -0.06258},
			    { 0.85109,  0.84161,  0.93745,  0.96791,  0.99696,  0.89369,  1.28987}
  };

  Float_t CorrX17[2][30] = {
			    {-0.22035, -0.21838, -0.21876, -0.21568, -0.21571, -0.21853, -0.21742, -0.20652, -0.36614},
			    { 0.28378,  0.10241,  0.04216,  0.50094,  0.48733,  0.57537, -0.00980,  0.26314,  2.62962}
  };

  Float_t CorrY17[2][30] = {
			    { 0.18903,  0.18621,  0.18503,  0.18892,  0.18895,  0.18820,  0.18762,  0.20365,  0.32501},
			    {-0.89001, -0.93155, -0.92281, -0.88428, -0.88063, -0.87455, -0.58438, -1.56954, -2.06242}
  };

  Float_t CorrX18[2][30] = {
			    { 0.36879,  0.36949,  0.37166,  0.36699,  0.36859, 0.36722,  0.36971,  0.37150,  0.36832,  0.12687},
			    {-0.48767, -0.31569, -0.09519, -0.7665, -0.81175, -0.80116, -0.11081, -0.16037, -0.78331,  2.27398}
  };

  Float_t CorrY18[2][30] = {
			    { 0.16663,  0.16437,  0.16390,  0.16533,  0.16530,  0.16814,  0.16227,  0.16211,  0.16781,  0.25281},
			    { 0.05936,  0.19353,  0.42546, -0.04766, -0.04149, -0.14987,  0.51395,  0.51463, -0.18159, -1.20530}
  };

  TString samplelist16[30] = {
			      "job_summer16_WJetsToLNu_MG_HT200to400",
			      "job_summer16_WJetsToLNu_MG_HT200to400_ext1",
			      "job_summer16_WJetsToLNu_MG_HT200to400_ext2",
			      "job_summer16_WJetsToLNu_MG_HT400to600",
			      "job_summer16_WJetsToLNu_MG_HT400to600_ext1",
			      "job_summer16_WJetsToLNu_MG_HT600to800",
			      "job_summer16_WJetsToLNu_MG_HT600to800_ext1",
			      "job_summer16_WJetsToLNu_MG_HT800to1200",
			      "job_summer16_WJetsToLNu_MG_HT800to1200_ext1",
			      "job_summer16_WJetsToLNu_MG_HT1200to2500",
			      "job_summer16_WJetsToLNu_MG_HT1200to2500_ext1",
			      "job_summer16_WJetsToLNu_MG_HT2500toInf",
			      "job_summer16_WJetsToLNu_MG_HT2500toInf_ext1",
			      "job_summer16_ZvvJets_HT200to400",
			      "job_summer16_ZvvJets_HT200to400_ext1",
			      "job_summer16_ZvvJets_HT400to600",
			      "job_summer16_ZvvJets_HT400to600_ext1",
			      "job_summer16_ZvvJets_HT600to800",
			      "job_summer16_ZvvJets_HT800to1200",
			      "job_summer16_ZvvJets_HT1200to2500",
			      "job_summer16_ZvvJets_HT1200to2500_ext1",
			      "job_summer16_ZvvJets_HT2500toInf",
			      "job_summer16_DYJetsToLL_m50_MG_HT200to400",
			      "job_summer16_DYJetsToLL_m50_MG_HT200to400_ext1",
			      "job_summer16_DYJetsToLL_m50_MG_HT400to600",
			      "job_summer16_DYJetsToLL_m50_MG_HT400to600_ext1",
			      "job_summer16_DYJetsToLL_m50_MG_HT600to800",
			      "job_summer16_DYJetsToLL_m50_MG_HT800to1200",
			      "job_summer16_DYJetsToLL_m50_MG_HT1200to2500",
			      "job_summer16_DYJetsToLL_m50_MG_HT2500toInf"
			      
  };

  TString samplelist17[30] = {
			      "job_fall17_WJetsToLNu_MG_HT200to400",
			      "job_fall17_WJetsToLNu_MG_HT400to600",
			      "job_fall17_WJetsToLNu_MG_HT600to800",
			      "job_fall17_WJetsToLNu_MG_HT800to1200",
			      "job_fall17_WJetsToLNu_MG_HT1200to2500",
			      "job_fall17_WJetsToLNu_MG_HT2500toInf",
			      "job_fall17_ZvvJets_HT200to400",
			      "job_fall17_ZvvJets_HT400to600",
			      "job_fall17_ZvvJets_HT600to800",
			      "job_fall17_ZvvJets_HT800to1200",
			      "job_fall17_ZvvJets_HT1200to2500",
			      "job_fall17_ZvvJets_HT1200to2500_ext1",
			      "job_fall17_ZvvJets_HT2500toInf",
			      "job_fall17_DYJetsToLL_m50_MG_HT200to400",
			      "job_fall17_DYJetsToLL_m50_MG_HT200to400_ext1",
			      "job_fall17_DYJetsToLL_m50_MG_HT400to600",
			      "job_fall17_DYJetsToLL_m50_MG_HT400to600_ext1",
			      "job_fall17_DYJetsToLL_m50_MG_HT600to800",
			      "job_fall17_DYJetsToLL_m50_MG_HT800to1200",
			      "job_fall17_DYJetsToLL_m50_MG_HT1200to2500",
			      "job_fall17_DYJetsToLL_m50_MG_HT2500toInf"
			      
  };

  TString samplelist18[30] = {
			      "job_autumn18_WJetsToLNu_MG_HT200to400",
			      "job_autumn18_WJetsToLNu_MG_HT400to600",
			      "job_autumn18_WJetsToLNu_MG_HT600to800",
			      "job_autumn18_WJetsToLNu_MG_HT800to1200",
			      "job_autumn18_WJetsToLNu_MG_HT1200to2500",
			      "job_autumn18_WJetsToLNu_MG_HT2500toInf",
			      "job_autumn18_ZvvJets_HT200to400",
			      "job_autumn18_ZvvJets_HT400to600",
			      "job_autumn18_ZvvJets_HT600to800",
			      "job_autumn18_ZvvJets_HT800to1200",
			      "job_autumn18_ZvvJets_HT1200to2500",
			      "job_autumn18_ZvvJets_HT2500toInf",
			      "job_autumn18_DYJetsToLL_m50_MG_HT200to400",
			      "job_autumn18_DYJetsToLL_m50_MG_HT400to600",
			      "job_autumn18_DYJetsToLL_m50_MG_HT400to600_ext2",
			      "job_autumn18_DYJetsToLL_m50_MG_HT600to800",
			      "job_autumn18_DYJetsToLL_m50_MG_HT800to1200",
			      "job_autumn18_DYJetsToLL_m50_MG_HT1200to2500",
			      "job_autumn18_DYJetsToLL_m50_MG_HT2500toInf"
  };

  Float_t CorrX16[2][30] = {
			    {-0.21271, -0.21386, -0.21358, -0.21148, -0.20734, -0.22549, -0.21212, -0.21749, -0.20913, -0.10865, -0.21224, -0.16319, -0.20827, -0.20811, -0.21022, -0.18376, -0.19880, -0.19531, -0.19143, -0.22921, -0.09826, -0.19301, -0.21992, -0.21616, -0.20774, -0.21303, -0.20909, -0.21646, -0.20241, -0.20115},
			    {-0.51573, -0.46404, -0.50979, -0.66574, -0.72296, -0.33074, -0.58516, -0.31099, -0.57287, -2.26952, -0.46622, -1.45560, -0.14948, -0.54772, -0.51273, -1.02192, -0.82228, -0.76478, -0.71878, -0.01469, -2.55552, -0.20873, -0.37947, -0.47372, -0.76212, -0.58821, -0.75011, -0.58882, -0.86564, -0.62288}
  };

  Float_t CorrY16[2][30] = {
			    {-0.06885, -0.06267, -0.05997, -0.09615, -0.06682, -0.07677, -0.06773, -0.09551, -0.06528, -0.20713, -0.06383, -0.10322, -0.08154, -0.06227, -0.06402, -0.09882, -0.06318, -0.07809, -0.07278,  0.03155,  0.10219, -0.10590, -0.05907, -0.05584, -0.05431, -0.05860, -0.06277, -0.06213, -0.11331, -0.07961},
			    { 1.33509,  1.17590,  1.12126,  1.77070,  1.23210,  1.39409,  1.18934,  1.67309,  1.10184,  4.21818,  1.33636,  2.93004,  2.49937,  1.15056,  1.18257,  1.99900,  1.15775,  1.45740,  1.15911, -0.40798, -1.27409,  3.30277,  1.11927,  1.00604,  0.99153,  1.00563,  1.03870,  1.03518,  2.25358,  2.35503}
  };

  Float_t CorrX17[2][30] = {
			    {-0.21560, -0.21386, -0.20957, -0.20354, -0.20780, -0.19428, -0.21347, -0.21086, -0.20548, -0.19801, -0.18259, -0.08751, -0.30846, -0.21457, -0.21391, -0.21423, -0.21162, -0.21495, -0.20800, -0.22524, -0.21771},
			    { 0.18070,  0.01679, -0.01113, -0.14581,  0.16201,  0.68986,  0.16826,  0.16630,  0.14511,  0.06314,  0.00894, -2.39694,  4.60846,  0.14292,  0.12903,  0.02988, -0.06861, -0.01748, -0.16912,  0.48975,  1.30331}
  };

  Float_t CorrY17[2][30] = {
			    { 0.18818,  0.18124,  0.17786,  0.18086,  0.17904,  0.17540,  0.18789,  0.17701,  0.17057,  0.17828,  0.14559,  0.05382,  0.15667,  0.18384,  0.18341,  0.18661,  0.18627,  0.18710,  0.18085,  0.17532,  0.18909},
			    {-0.76314, -0.67712, -0.74078, -0.80438, -0.57066,  0.08030, -0.71906, -0.52286, -0.67492, -1.00912,  0.20901, -0.72451,  2.99536, -0.72311, -0.68341, -0.84155, -0.86319, -0.89171, -0.68833, -0.39452, -0.10353}
  };

  Float_t CorrX18[2][30] = {
			    { 0.36648,  0.36667,  0.35443,  0.36071,  0.35277,  0.33266,  0.36638,  0.35897,  0.33607,  0.32664,  0.30581,  0.34871,  0.36685,  0.37361,  0.36794,  0.37218,  0.36235,  0.35364,  0.35941},
			    {-0.40699, -0.25962, -0.01259, -0.01990,  0.41121,  1.15719, -0.38633, -0.18954, -0.03644,  0.42700,  1.39125,  0.14531, -0.44964, -0.41735, -0.27052, -0.21156,  0.24651,  0.93657,  1.13419}
  };

  Float_t CorrY18[2][30] = {
			    { 0.16102,  0.15235,  0.15467,  0.14762,  0.15939,  0.13856,  0.16049,  0.15355,  0.14721,  0.14359, -0.03233,  0.24212,  0.16222,  0.15779,  0.15913,  0.15719,  0.16275,  0.14378,  0.17027},
			    { 0.28219,  0.56971,  0.52557,  0.93519,  1.01706,  2.70286,  0.35733,  0.49305,  0.44093,  0.85435,  6.11037, -0.64226,  0.20951,  0.39412,  0.42069,  0.54292,  0.65190,  1.66361,  2.10208}
  };
  
  TString samplelist16[30] = {
			      "job_summer16_DYJetsToLL_m50_MG_HT200to400",
			      "job_summer16_DYJetsToLL_m50_MG_HT200to400_ext1",
			      "job_summer16_DYJetsToLL_m50_MG_HT400to600",
			      "job_summer16_DYJetsToLL_m50_MG_HT400to600_ext1",
			      "job_summer16_DYJetsToLL_m50_MG_HT600to800",
			      "job_summer16_DYJetsToLL_m50_MG_HT800to1200",
			      "job_summer16_DYJetsToLL_m50_MG_HT1200to2500",
			      "job_summer16_DYJetsToLL_m50_MG_HT2500toInf"
  };

  Float_t CorrX16[2][30] = {
			    {-0.21992, -0.21616, -0.20774, -0.21303, -0.20909, -0.21646, -0.20241, -0.20115},
			    {-0.37947, -0.47372, -0.76212, -0.58821, -0.75011, -0.58882, -0.86564, -0.62288}
  };

  Float_t CorrY16[2][30] = {
			    {-0.05907, -0.05584, -0.05431, -0.05860, -0.06277, -0.06213, -0.11331, -0.07961},
			    { 1.11927,  1.00604,  0.99153,  1.00563,  1.03870,  1.03518,  2.25358,  2.35503}
  };
  

  TString samplelist17[30] = {
			      "job_fall17_DYJetsToLL_m50_MG_HT200to400",
			      "job_fall17_DYJetsToLL_m50_MG_HT200to400_ext1",
			      "job_fall17_DYJetsToLL_m50_MG_HT400to600",
			      "job_fall17_DYJetsToLL_m50_MG_HT400to600_ext1",
			      "job_fall17_DYJetsToLL_m50_MG_HT600to800",
			      "job_fall17_DYJetsToLL_m50_MG_HT800to1200",
			      "job_fall17_DYJetsToLL_m50_MG_HT1200to2500",
			      "job_fall17_DYJetsToLL_m50_MG_HT2500toInf"
  };

  Float_t CorrX17[2][30] = {
			    {-0.21457, -0.21391, -0.21423, -0.21162, -0.21495, -0.20800, -0.22524, -0.21771},
			    { 0.14292,  0.12903,  0.02988, -0.06861, -0.01748, -0.16912,  0.48975,  1.30331}
  };

  Float_t CorrY17[2][30] = {
			    { 0.18384,  0.18341,  0.18661,  0.18627,  0.18710,  0.18085,  0.17532,  0.18909},
			    {-0.72311, -0.68341, -0.84155, -0.86319, -0.89171, -0.68833, -0.39452, -0.10353}
  };
  
  TString samplelist18[30] = {
			      "job_autumn18_DYJetsToLL_m50_MG_HT200to400",
			      "job_autumn18_DYJetsToLL_m50_MG_HT400to600",
			      "job_autumn18_DYJetsToLL_m50_MG_HT400to600_ext2",
			      "job_autumn18_DYJetsToLL_m50_MG_HT600to800",
			      "job_autumn18_DYJetsToLL_m50_MG_HT800to1200",
			      "job_autumn18_DYJetsToLL_m50_MG_HT1200to2500",
			      "job_autumn18_DYJetsToLL_m50_MG_HT2500toInf"
  };
  Float_t CorrX18[2][30] = {
			    { 0.36685,  0.37361,  0.36794,  0.37218,  0.36235,  0.35364,  0.35941},
			    {-0.44964, -0.41735, -0.27052, -0.21156,  0.24651,  0.93657,  1.13419}
  };

  Float_t CorrY18[2][30] = {
			    { 0.16222,  0.15779,  0.15913,  0.15719,  0.16275,  0.14378,  0.17027},
			    { 0.20951,  0.39412,  0.42069,  0.54292,  0.65190,  1.66361,  2.10208}
  };

  TString samplelist16[30] = {
			      "job_summer16_QCD_HT200to300",
			      "job_summer16_QCD_HT200to300_ext1",
			      "job_summer16_QCD_HT300to500",
			      "job_summer16_QCD_HT300to500_ext1",
			      "job_summer16_QCD_HT500to700",
			      "job_summer16_QCD_HT500to700_ext1",
			      "job_summer16_QCD_HT700to1000",
			      "job_summer16_QCD_HT700to1000_ext1",
			      "job_summer16_QCD_HT1000to1500",
			      "job_summer16_QCD_HT1000to1500_ext1",
			      "job_summer16_QCD_HT1500to2000",
			      "job_summer16_QCD_HT1500to2000_ext1",
			      "job_summer16_QCD_HT2000toInf",
			      "job_summer16_QCD_HT2000toInf_ext1"
  };

  Float_t CorrX16[2][30] = {
			    {-0.21357, -0.21387, -0.21356, -0.21325, -0.21241, -0.21317, -0.21102, -0.21123, -0.21109, -0.20941, -0.20989, -0.21105, -0.21718, -0.21182},
			    {-0.52414, -0.52371, -0.60891, -0.60433, -0.68271, -0.66717, -0.74111, -0.74819, -0.75791, -0.76129, -0.72998, -0.66464, -0.42689, -0.59372}
  };

  Float_t CorrY16[2][30] = {
			    {-0.04807, -0.04771, -0.04997, -0.04999, -0.05685, -0.05531, -0.05826, -0.06122, -0.06274, -0.06594, -0.06474, -0.06801, -0.07361, -0.06744},
			    { 0.98983,  0.97721,  0.97054,  0.97709,  1.01371,  0.97494,  1.00060,  1.04025,  1.08060,  1.16441,  1.42756,  1.43932,  1.87835,  1.83629}
  };
  

  TString samplelist17[30] = {
			      "job_fall17_QCD_HT200to300",
			      "job_fall17_QCD_HT300to500",
			      "job_fall17_QCD_HT500to700",
			      "job_fall17_QCD_HT700to1000",
			      "job_fall17_QCD_HT1000to1500",
			      "job_fall17_QCD_HT1500to2000",
			      "job_fall17_QCD_HT2000toInf"
  };
  

  Float_t CorrX17[2][30] = {
			    {-0.21508, -0.21604, -0.22037, -0.21676, -0.21624, -0.21910, -0.21233},
			    { 0.14528,  0.06644,  0.06789, -0.08684, -0.10125,  0.14412,  0.36691}
  };

  Float_t CorrY17[2][30] = {
			    { 0.18351,  0.18357,  0.18674,  0.18488,  0.18278,  0.18667,  0.18512},
			    {-0.55505, -0.59078, -0.77351, -0.73941, -0.64101, -0.52293, -0.10436}
  };
  
  TString samplelist18[30] = {
			      "job_autumn18_QCD_HT200to300",
			      "job_autumn18_QCD_HT300to500",
			      "job_autumn18_QCD_HT500to700",
			      "job_autumn18_QCD_HT700to1000",
			      "job_autumn18_QCD_HT1000to1500",
			      "job_autumn18_QCD_HT1500to2000",
			      "job_autumn18_QCD_HT2000toInf"
  };
  Float_t CorrX18[2][30] = {
			    { 0.36815,  0.36942,  0.37001,  0.37047,  0.37208,  0.37281,  0.37088},
			    {-0.43524, -0.31703, -0.11897,  0.08256,  0.28712,  0.57837,  0.80818}
  };

  Float_t CorrY18[2][30] = {
			    { 0.16576,  0.16468,  0.16387,  0.16172,  0.16149,  0.15653,  0.15643},
			    { 0.21492,  0.31638,  0.43768,  0.62398,  0.86722,  1.52230,  2.13962}
  };

  TString samplelist16[30] = {
			      "job_summer16_EWK_Wminus",
			      "job_summer16_EWK_Wminus_ext1",
			      "job_summer16_EWK_Wminus_ext2",
			      "job_summer16_EWK_Wplus",
			      "job_summer16_EWK_Wplus_ext1",
			      "job_summer16_EWK_Wplus_ext2",
			      "job_summer16_EWK_Zll",
			      "job_summer16_EWK_Zll_ext1",
			      "job_summer16_EWK_Zll_ext2",
			      "job_summer16_EWK_Zvv",
			      "job_summer16_EWK_Zvv_ext1",
			      "job_summer16_EWK_Zvv_ext2",
			      "job_summer16_WW",
			      "job_summer16_WW_ext1",
			      "job_summer16_WZ",
			      "job_summer16_WZ_ext1",
			      "job_summer16_ZZ",
			      "job_summer16_ZZ_ext1",
			      "job_summer16_WWG",
			      "job_summer16_WZG"
  };

  Float_t CorrX16[2][30] = {
			    {-0.21744, -0.20221, -0.21364, -0.22548, -0.21517, -0.20994, -0.22347, -0.21078, -0.20487, -0.22389, -0.18355, -0.20954, -0.21745, -0.21592, -0.22011, -0.21227, -0.21595, -0.20976, -0.20705, -0.21712},
			    {-0.43835, -0.59998, -0.34241, -0.02688, -0.28797, -0.47705, -0.15731, -0.36865, -0.47875, -0.28434, -1.01487, -0.46797, -0.29716, -0.32971, -0.31105, -0.35398, -0.37227, -0.45407, -0.56949, -0.25175}
  };

  Float_t CorrY16[2][30] = {
			    {-0.07015, -0.05401, -0.06097, -0.05254, -0.06492, -0.04835, -0.06707, -0.05262, -0.05828, -0.05977, -0.02674, -0.05872, -0.04873, -0.05683, -0.05478, -0.04603, -0.07087, -0.05141, -0.04655, -0.05705},
			    { 1.33980,  1.00653,  1.12129,  1.07263,  1.15678,  0.91949,  1.20548,  0.95012,  1.04654,  1.19218,  0.35964,  1.14722,  1.05639,  1.09394,  1.16075,  0.94682,  1.36527,  1.03329,  0.89357,  1.17579}
  };

  TString samplelist17[30] = {
			      "job_fall17_WW",
			      "job_fall17_WZ",
			      "job_fall17_ZZ",
			      "job_fall17_WWG",
			      "job_fall17_WZG",
			      "job_fall17_EWK_Wminus",
			      "job_fall17_EWK_Wplus",
			      "job_fall17_EWK_Zll",
			      "job_fall17_EWK_Zvv"
  };

  Float_t CorrX17[2][30] = {
			    {-0.21815, -0.2105, -0.21557, -0.21191, -0.21420, -0.21192, -0.21512, -0.21913, -0.21342},
			    { 0.40544,  0.23121,  0.30027,  0.27235,  0.33472,  0.39712, 0.39331,  0.28414}
  };
  
  Float_t CorrY17[2][30] = {
			    { 0.18979,  0.18789,  0.18684,  0.19126,  0.18377,  0.18368,  0.18071,  0.18451,  0.18708},
			    {-0.82838, -0.73884, -0.72245, -0.89117, -0.70094, -0.77114, -0.61283, -0.77836, -0.76144}
  };

  TString samplelist18[30] = {
			      "job_autumn18_WW",
			      "job_autumn18_WZ",
			      "job_autumn18_ZZ",
			      "job_autumn18_WWG",
			      "job_autumn18_WZG",
			      "job_autumn18_EWK_Wminus",
			      "job_autumn18_EWK_Wplus",
			      "job_autumn18_EWK_Zll",
			      "job_autumn18_EWK_Zvv"
  };

  Float_t CorrX18[2][30] = {
			    { 0.36699,  0.37255,  0.36431,  0.36689,  0.37419,  0.36961,  0.36850,  0.35698,  0.36706},
			    {-0.63048, -0.71564, -0.56762, -0.70171, -0.78702, -0.70117, -0.72382, -0.48177, -0.77949}
  };

  Float_t CorrY18[2][30] = {
			    { 0.16294,  0.16312,  0.16245,  0.16391,  0.15007,  0.16356,  0.16154,  0.16362,  0.15834},
			    { 0.12118,  0.15636,  0.19710,  0.05475,  0.47079,  0.06980,  0.24238,  0.07083,  0.25343}
  };
  
  //function start//
  Double_t corrMET, corrMETPhi;
  corrMET = 0;
  corrMETPhi = 0;
  corrXY.clear();

  Int_t nVtx = data.GetInt("nGoodVtx");
  //Float_t pfMET = data.GetFloat("pfMET");//replace by input jet smearing corrected MET!!!
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

  //return corrMETPhi;
}


