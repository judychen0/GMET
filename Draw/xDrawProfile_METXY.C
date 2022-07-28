#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
#define nfile 8
// bkg [16, 16] fall17, autumn18
// Wjet [7, 7]
// Zjet [8, 7]
// DYjet [10, 8]


void xDrawProfile_METXY(){
  ofstream ftext;
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_GJets_MG_HT200to400/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_GJets_MG_HT400to600/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_GJets_MG_HT600toInf/METcorr_ggtree.root",			  
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_Wg_aMCatNLO/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_Wg_aMCatNLO_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_Zg_aMCatNLO/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_TT_aMCatNLO/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_Znunug_pt130/METcorr_ggtree.root"
  
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_GJets_MG_HT100to200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_GJets_MG_HT200to400/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_GJets_MG_HT400to600/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_GJets_MG_HT600toInf/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_TTGjets/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_TTGjets_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_TT_aMCatNLO/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_WW/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_WZ/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_ZZ/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_WWG/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_WZG/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_EWK_Wminus/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_EWK_Wplus/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_EWK_Zll/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_EWK_Zvv/METcorr_ggtree.root"
			  
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_GJets_MG_HT100to200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_GJets_MG_HT200to400/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_GJets_MG_HT400to600/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_GJets_MG_HT600toInf/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_TTGjets/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_TT_aMCatNLO_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_TT_aMCatNLO_ext2/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_WW/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_WZ/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_ZZ/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_WWG/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_WZG/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_EWK_Wminus/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_EWK_Wplus/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_EWK_Zll/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_EWK_Zvv/METcorr_ggtree.root"
			  
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/summer16/data/job_SinglePho_Run2016B_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/data/job_SinglePho_Run2016C_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/data/job_SinglePho_Run2016D_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/data/job_SinglePho_Run2016E_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/data/job_SinglePho_Run2016F1_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/data/job_SinglePho_Run2016F2_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/data/job_SinglePho_Run2016G_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/data/job_SinglePho_Run2016H_UL/METcorr_ggtree.root"
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/fall17/data/job_SinglePho_Run2017B_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/data/job_SinglePho_Run2017C_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/data/job_SinglePho_Run2017D_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/data/job_SinglePho_Run2017E_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/data/job_SinglePho_Run2017F_UL/METcorr_ggtree.root"
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_SinglePho_Run2017B_MarReminiAOD/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_SinglePho_Run2017C_MarReminiAOD/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_SinglePho_Run2017D_MarReminiAOD/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_SinglePho_Run2017E_MarReminiAOD/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_SinglePho_Run2017F_MarReminiAOD/METcorr_ggtree.root"
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/autumn18/data/job_EGamma_Run2018A_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/data/job_EGamma_Run2018B_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/data/job_EGamma_Run2018C_UL/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/data/job_EGamma_Run2018D_UL/METcorr_ggtree.root"
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT200to400/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT200to400_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT200to400_ext2/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT400to600/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT400to600_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT600to800/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT600to800_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT800to1200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT800to1200_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT1200to2500/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT1200to2500_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT2500toInf/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WJetsToLNu_MG_HT2500toInf_ext1/METcorr_ggtree.root"
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_WJetsToLNu_MG_HT100to200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_WJetsToLNu_MG_HT200to400/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_WJetsToLNu_MG_HT400to600/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_WJetsToLNu_MG_HT600to800/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_WJetsToLNu_MG_HT800to1200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_WJetsToLNu_MG_HT1200to2500/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_WJetsToLNu_MG_HT2500toInf/METcorr_ggtree.root"
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_WJetsToLNu_MG_HT100to200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_WJetsToLNu_MG_HT200to400/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_WJetsToLNu_MG_HT400to600/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_WJetsToLNu_MG_HT600to800/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_WJetsToLNu_MG_HT800to1200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_WJetsToLNu_MG_HT1200to2500/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_WJetsToLNu_MG_HT2500toInf/METcorr_ggtree.root",
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_ZvvJets_HT200to400/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_ZvvJets_HT200to400_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_ZvvJets_HT400to600/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_ZvvJets_HT400to600_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_ZvvJets_HT600to800/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_ZvvJets_HT800to1200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_ZvvJets_HT1200to2500/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_ZvvJets_HT1200to2500_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_ZvvJets_HT2500toInf/METcorr_ggtree.root"
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_ZvvJets_HT100to200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_ZvvJets_HT200to400/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_ZvvJets_HT400to600/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_ZvvJets_HT600to800/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_ZvvJets_HT800to1200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_ZvvJets_HT1200to2500/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_ZvvJets_HT1200to2500_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_ZvvJets_HT2500toInf/METcorr_ggtree.root"
  };
  */
  /*
  TString rootname[20] = {
			   "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_ZvvJets_HT100to200/METcorr_ggtree.root",
			   "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_ZvvJets_HT200to400/METcorr_ggtree.root",
			   "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_ZvvJets_HT400to600/METcorr_ggtree.root",
			   "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_ZvvJets_HT600to800/METcorr_ggtree.root",
			   "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_ZvvJets_HT800to1200/METcorr_ggtree.root",
			   "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_ZvvJets_HT1200to2500/METcorr_ggtree.root",
			   "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_ZvvJets_HT2500toInf/METcorr_ggtree.root"
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_DYJetsToLL_m50_MG_HT200to400/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_DYJetsToLL_m50_MG_HT200to400_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_DYJetsToLL_m50_MG_HT400to600/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_DYJetsToLL_m50_MG_HT400to600_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_DYJetsToLL_m50_MG_HT600to800/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_DYJetsToLL_m50_MG_HT800to1200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_DYJetsToLL_m50_MG_HT1200to2500/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_DYJetsToLL_m50_MG_HT2500toInf/METcorr_ggtree.root"
  };
  */
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_DYJetsToLL_m50_MG_HT100to200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_DYJetsToLL_m50_MG_HT100to200_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_DYJetsToLL_m50_MG_HT200to400/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_DYJetsToLL_m50_MG_HT200to400_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_DYJetsToLL_m50_MG_HT400to600/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_DYJetsToLL_m50_MG_HT400to600_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_DYJetsToLL_m50_MG_HT600to800/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_DYJetsToLL_m50_MG_HT800to1200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_DYJetsToLL_m50_MG_HT1200to2500/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/MET/job_fall17_DYJetsToLL_m50_MG_HT2500toInf/METcorr_ggtree.root"
  };
  */
  
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_DYJetsToLL_m50_MG_HT100to200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_DYJetsToLL_m50_MG_HT200to400/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_DYJetsToLL_m50_MG_HT400to600/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_DYJetsToLL_m50_MG_HT400to600_ext2/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_DYJetsToLL_m50_MG_HT600to800/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_DYJetsToLL_m50_MG_HT800to1200/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_DYJetsToLL_m50_MG_HT1200to2500/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/MET/job_autumn18_DYJetsToLL_m50_MG_HT2500toInf/METcorr_ggtree.root"
  };
  
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_EWK_Wminus/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_EWK_Wminus_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_EWK_Wminus_ext2/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_EWK_Wplus/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_EWK_Wplus_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_EWK_Wplus_ext2/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_EWK_Zll/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_EWK_Zll_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_EWK_Zll_ext2/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_EWK_Zvv/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_EWK_Zvv_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_EWK_Zvv_ext2/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WW/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WW_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WZ/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WZ_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_ZZ/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_ZZ_ext1/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WWG/METcorr_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/summer16/MET/job_summer16_WZG/METcorr_ggtree.root"
  };
  */

  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";
 
  TH2F *H2_nvtx_METX[nfile];
  TH2F *H2_nvtx_METY[nfile];

  TProfile *tp_nvtx_METX[nfile];
  TProfile *tp_nvtx_METY[nfile];

  Double_t slope, err;
  Double_t par[3];
  TF1 *fitresult = new TF1("fitresult", "pol1", 10, 60);
  TLatex *latp = new TLatex();
  
  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    
    H2_nvtx_METX[i] = (TH2F*)fopen->Get("h2_nvtx_METX");
    H2_nvtx_METY[i] = (TH2F*)fopen->Get("h2_nvtx_METY");

    tp_nvtx_METX[i] = H2_nvtx_METX[i]->ProfileX(Form("tp_nvtx_METX_%i", i), 1, -1);
    tp_nvtx_METY[i] = H2_nvtx_METY[i]->ProfileX(Form("tp_nvtx_METY_%i", i), 1, -1);

    tp_nvtx_METX[i]->SetMarkerStyle(8);
    tp_nvtx_METY[i]->SetMarkerStyle(8);

    tp_nvtx_METX[i]->SetMarkerColor(1);
    tp_nvtx_METY[i]->SetMarkerColor(1);

    tp_nvtx_METX[i]->SetTitle(Form("tp_nvtx_METX_%i", i));
    tp_nvtx_METY[i]->SetTitle(Form("tp_nvtx_METY_%i", i));

    tp_nvtx_METX[i]->GetXaxis()->SetTitle("nVtx");
    tp_nvtx_METY[i]->GetXaxis()->SetTitle("nVtx");

    tp_nvtx_METX[i]->GetYaxis()->SetTitle("METx");
    tp_nvtx_METY[i]->GetYaxis()->SetTitle("METy");

    //H2_nvtx_METX[i]->Draw("colz");
    tp_nvtx_METX[i]->Draw();
    tp_nvtx_METX[i]->SetMinimum(-100);
    tp_nvtx_METX[i]->SetMaximum(100);
    tp_nvtx_METX[i]->GetXaxis()->SetRangeUser(0, 100);
    tp_nvtx_METX[i]->Fit(fitresult, "", "", 10, 60);
    

    for(Int_t i=0; i<2; i++){par[i] = fitresult->GetParameter(i);}
    latp->SetTextAlign(12);
    latp->SetTextSize(0.035);
    latp->DrawLatex(10, 30, Form("fit = %.3fnvtx+%.3f", par[0], par[1]));
    title = tp_nvtx_METX[i]->GetTitle();
    //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    ftext.open("METXcorr.txt", ios::out | ios::app);
    if(!ftext){
      cerr << "Can't open file !" << endl;
      exit(1);
    }
    ftext << "file " << i << endl;
    ftext << "fit = " << par[1] << "x nVtx + " << par[0] << endl;
    ftext.close();
    
    //H2_nvtx_METY[i]->Draw("colz");
    tp_nvtx_METY[i]->Draw();
    tp_nvtx_METY[i]->SetMinimum(-100);
    tp_nvtx_METY[i]->SetMaximum(100);
    tp_nvtx_METY[i]->GetXaxis()->SetRangeUser(0, 100);
    tp_nvtx_METY[i]->Fit(fitresult, "", "", 10, 60);
    

    for(Int_t i=0; i<2; i++){par[i] = fitresult->GetParameter(i);}
    latp->SetTextAlign(12);
    latp->SetTextSize(0.035);
    latp->DrawLatex(10, 30, Form("fit = %.3fnvtx+%.3f", par[1], par[0]));
    title = tp_nvtx_METY[i]->GetTitle();
    //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    ftext.open("METYcorr.txt", ios::out | ios::app);
    if(!ftext){
      cerr << "Can't open file !" << endl;
      exit(1);
    }
    ftext << "file " << i << endl;
    ftext << "fit = " << par[1] << "x nVtx + " << par[0] << endl;
    ftext.close();
  }
  
}
