#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "./Drawlumi/CMS_lumi.C"
using namespace std;
#define nfile 12
#define nhisto 2
#define njet 2
#define ncolor 20

TH1F* ratioplot(THStack* st, TH1F* h1){

  if (!st) {
       Warning("TRatioPlot", "Need only a stack");
       //return;
    }
  
  TList *stackHists = st->GetHists();
  
  if (stackHists->GetSize() == 0) {
    Warning("TRatioPlot", "Stack does not have histograms");
    //return;
  }
  
  TH1F* tmpHist = (TH1F*)stackHists->At(0)->Clone();
  tmpHist->Reset();
  
  for (int i=0;i<stackHists->GetSize();++i) {
    tmpHist->Add((TH1*)stackHists->At(i));
  }
  
    
  TH1F *ratio = (TH1F*)h1->Clone("ratio");
  ratio->SetLineColor(kBlack);
  ratio->SetMinimum(-0.4);
  ratio->SetMaximum(3.4);
  ratio->Sumw2();
  ratio->Divide(tmpHist);
  
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerSize(0.8);
  ratio->Draw("EP");
  
  ratio->SetTitle("");
  ratio->GetYaxis()->SetTitle("Data/MC");
  ratio->GetYaxis()->SetTitleFont(42);
  ratio->GetYaxis()->SetTitleSize(0.1);
  ratio->GetYaxis()->SetTitleOffset(0.4);
  ratio->GetYaxis()->SetLabelFont(42);
  ratio->GetYaxis()->SetLabelOffset(0.008);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetYaxis()->SetTickSize(0.032);
  
  ratio->GetXaxis()->SetTitleFont(42);
  ratio->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetTitleOffset(1.1);
  ratio->GetXaxis()->SetLabelFont(42);
  ratio->GetXaxis()->SetLabelOffset(0.03);
  ratio->GetXaxis()->SetLabelSize(0.1);
  ratio->GetXaxis()->SetTickSize(0.1);
  ratio->GetXaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");

  return ratio;
}

void xDrawStack_Nm1(Int_t year){
  TString rootname[20];
  TFile *fopen;
  

  Float_t entries = 1.0;
  Float_t outentries = 0.;
  Float_t scale[nfile] = {0};

  TH1F *H_Events[nfile];
  TH1F *HSumofGenW[nfile];

  Float_t lumi16 = 36.33;
  Float_t lumi17 = 41.48;
  Float_t lumi = 1.0;
  if(year==2016){
    lumi = lumi16;
  }
  else if(year==2017){
    lumi = lumi17;
  }
 //Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 0.296};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, Znng]
  Float_t xsec[20] = {1, 1, 1, 1, 1, 1, 1, 192.3, 123.3, 750.5, 0.018};//[GJet, WJet, ZJet, DYJet, monoB, diB, triB, Wg, Zg, TTJet, ZnngVBS]
  Float_t filter[20] = {1., 1., 1., 1., 1., 1., 1.};
  Float_t kfactor[20] = {1., 1., 1., 1., 1., 1., 1.};
  /*
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/GJet/sumGenW.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/sumGenW.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/ZJet/sumGenW.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/sumGenW.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Wg_aMCatNLO/sumGenW.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Zg_aMCatNLO/sumGenW.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_TT_aMCatNLO/sumGenW.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/sumGenW.root";
    rootname[8] = "/home/judy/ntuhep/GMET/output_file/summer16/data/sumGenW.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/GJet/sumGenW.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/sumGenW.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/ZJet/sumGenW.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/DYJet/sumGenW.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Wg_aMCatNLO/sumGenW.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Zg_aMCatNLO/sumGenW.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_TT_aMCatNLO/sumGenW.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/sumGenW.root";
    rootname[8] = "/home/judy/ntuhep/GMET/output_file/fall17/data/sumGenW.root";
  }
  */
  
  
  
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Wg_aMCatNLO/output_ggtree.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Zg_aMCatNLO/output_ggtree.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_TT_aMCatNLO/output_ggtree.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/output_ggtree.root";
    rootname[8] = "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root";
  }
 else if(year==2017){
   rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_TT_aMCatNLO/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/output_ggtree.root";
    rootname[10] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_UL17_Znunug_VBS/output_ggtree.root";
    rootname[11] = "/home/judy/ntuhep/GMET/output_file/fall17/data/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/monoboson/output_merge_monoBoson.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/diboson/output_merge_diBoson.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/triboson/output_merge_triBoson.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/job_autumn18_Wg_aMCatNLO/output_ggtree.root";
    rootname[8] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/job_autumn18_Zg_aMCatNLO/output_ggtree.root";
    rootname[9] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/job_autumn18_TT_aMCatNLO/output_ggtree.root";
    rootname[10] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/job_autumn18_Znunug_pt130/output_ggtree.root";
    //rootname[10] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/job_UL18_Znunug_VBS/output_ggtree.root";
    rootname[11] = "/home/judy/ntuhep/GMET/output_file/autumn18/data/output_ggtree.root";
  }
  
  for(Int_t i=0; i<nfile-1; i++){
    fopen = new TFile(rootname[i]);
     if((i<nfile-1) && i>=7){
      H_Events[i] = (TH1F*)fopen->Get("hEvents");
      //HSumofGenW[i] = (TH1F*)fopen->Get("hSumofGenW");
      HSumofGenW[i] = (TH1F*)fopen->Get("hSumGenWeight");
      entries = 1.0;
      //entries = H_Events[i]->GetBinContent(1);
      entries = HSumofGenW[i]->GetBinContent(1);
      outentries = xsec[i]*1000*lumi;
      scale[i] = 0;
      scale[i] = fabs(outentries/entries);

      cout << "print " << entries << " " << outentries << " " << scale[i]<< endl;
     }
    //if(i==nfile-1 || i<3) scale = 1.0;
    if(i<7) scale[i] = 1.0;
  }

  //string hexcolor[8] = {"#7FB5F5","#EFE2C9","#1C2A60","#FA6D5F","#1C6040","#DCF563", "#2F81A3","#91A81E"};
  //string hexcolor[ncolor] = {"#fff176", "#03a9f4", "#ff3360", "#ff9e00", "#00FFFF", "#eeff41", "#67ccc1", "#0066ff", "#4caf50"};
 string hexcolor[ncolor] = {"#fff176", "#03a9f4", "#ff3360", "#3d5afe", "#b23b8c", "#96ff5a", "#a983d3", "#ff9e00", "#00FFFF", "#FFCCCC", "#67ccc1"};
  //string hexcolor[ncolor] = {"#fff176", "#03a9f4", "#ff3360", "#c144ae", "#3d5afe", "#ff9e00", "#00FFFF", "#FFCCCC", "#67ccc1"};//QCD
  //string hexcolor[ncolor] = {"#67ccc1", "#03a9f4", "#00e5ff", "#ffea00", "#1C2A60", "#DCDB06", "#FA6D5F", "#F0F5E1","#DCF563", "#2F81A3" };
  TColor *color[ncolor];
  Int_t cnum[ncolor];
   
  for(Int_t j=0; j<ncolor; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  } 

  TCanvas *c1 = new TCanvas("c1");
  TTree *t;

  TH1F *H_SeedTime_Nm1[nfile][nhisto];
  TH1F *H_MET_Nm1[nfile][nhisto];
  TH1F *H_METPhi_Nm1[nfile][nhisto];
  TH1F *H_dphoMETPhi_Nm1[nfile][nhisto];
  TH1F *H_njet_Nm1[nfile][nhisto];
  TH1F *H_djetMETPhi_Nm1[nfile][nhisto][njet];

  TH1F *HA_SeedTime_Nm1[nhisto];
  TH1F *HA_MET_Nm1[nhisto];
  TH1F *HA_METPhi_Nm1[nhisto];
  TH1F *HA_dphoMETPhi_Nm1[nhisto];
  TH1F *HA_njet_Nm1[nhisto];
  TH1F *HA_djetMETPhi_Nm1[nhisto][njet];

  TH1F *HSig_SeedTime_Nm1[nhisto];
  TH1F *HSig_MET_Nm1[nhisto];
  TH1F *HSig_METPhi_Nm1[nhisto];
  TH1F *HSig_dphoMETPhi_Nm1[nhisto];
  TH1F *HSig_njet_Nm1[nhisto];
  TH1F *HSig_djetMETPhi_Nm1[nhisto][njet];

  
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    if(i==nfile-1) scale[i] = 1.0;
    for(Int_t jj=0; jj<nhisto; jj++){
      
      //H_SeedTime_Nm1[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_SeedTime_Nm1_%i", jj));
      H_MET_Nm1[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", jj));
      H_METPhi_Nm1[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_Nm1_%i", jj));
      H_dphoMETPhi_Nm1[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_Nm1_%i", jj));
      H_njet_Nm1[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_Nm1_%i", jj));

      if(i==0){
	HA_MET_Nm1[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", jj))->Clone();
	HA_METPhi_Nm1[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_Nm1_%i", jj))->Clone();
	HA_dphoMETPhi_Nm1[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_Nm1_%i", jj))->Clone();
	HA_njet_Nm1[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_Nm1_%i", jj))->Clone();
	
	HA_MET_Nm1[jj]->Scale(scale[i]);
	HA_METPhi_Nm1[jj]->Scale(scale[i]);
	HA_dphoMETPhi_Nm1[jj]->Scale(scale[i]);
	HA_njet_Nm1[jj]->Scale(scale[i]);

	HA_MET_Nm1[jj]->SetFillStyle(3145);
	HA_METPhi_Nm1[jj]->SetFillStyle(3145);
	HA_dphoMETPhi_Nm1[jj]->SetFillStyle(3145);
	HA_njet_Nm1[jj]->SetFillStyle(3145);

	HA_MET_Nm1[jj]->SetFillColor(12);
	HA_METPhi_Nm1[jj]->SetFillColor(12);
	HA_dphoMETPhi_Nm1[jj]->SetFillColor(12);
	HA_njet_Nm1[jj]->SetFillColor(12);

	HA_MET_Nm1[jj]->SetMarkerSize(0.1);
	HA_METPhi_Nm1[jj]->SetMarkerSize(0.1);
	HA_dphoMETPhi_Nm1[jj]->SetMarkerSize(0.1);
	HA_njet_Nm1[jj]->SetMarkerSize(0.1);

      }

      if(i==nfile-2){
	HSig_MET_Nm1[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", jj))->Clone();
	HSig_METPhi_Nm1[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_METPhi_Nm1_%i", jj))->Clone();
	HSig_dphoMETPhi_Nm1[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_Nm1_%i", jj))->Clone();
	HSig_njet_Nm1[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_Nm1_%i", jj))->Clone();
	
	HSig_MET_Nm1[jj]->Scale(scale[i]);
	HSig_METPhi_Nm1[jj]->Scale(scale[i]);
	HSig_dphoMETPhi_Nm1[jj]->Scale(scale[i]);
	HSig_njet_Nm1[jj]->Scale(scale[i]);

	HSig_MET_Nm1[jj]->SetLineColor(kRed);
	HSig_METPhi_Nm1[jj]->SetLineColor(kRed);
	HSig_dphoMETPhi_Nm1[jj]->SetLineColor(kRed);
	HSig_njet_Nm1[jj]->SetLineColor(kRed);

	HSig_MET_Nm1[jj]->SetLineWidth(2);
	HSig_METPhi_Nm1[jj]->SetLineWidth(2);
	HSig_dphoMETPhi_Nm1[jj]->SetLineWidth(2);
	HSig_njet_Nm1[jj]->SetLineWidth(2);

      }
      
      //H_SeedTime_Nm1[i][jj]->Scale(scale[i]);
      H_MET_Nm1[i][jj]->Scale(scale[i]);
      H_METPhi_Nm1[i][jj]->Scale(scale[i]);
      H_dphoMETPhi_Nm1[i][jj]->Scale(scale[i]);
      H_njet_Nm1[i][jj]->Scale(scale[i]);
      

      if(i<nfile-1){
	//H_SeedTime_Nm1[i][jj]->SetFillColor(cnum[i]);
	H_MET_Nm1[i][jj]->SetFillColor(cnum[i]);
	H_METPhi_Nm1[i][jj]->SetFillColor(cnum[i]);
	H_dphoMETPhi_Nm1[i][jj]->SetFillColor(cnum[i]);
	H_njet_Nm1[i][jj]->SetFillColor(cnum[i]);
	
      }
      
      //H_SeedTime_Nm1[i][jj]->SetLineColor(1);
      H_MET_Nm1[i][jj]->SetLineColor(1);
      H_METPhi_Nm1[i][jj]->SetLineColor(1);
      H_dphoMETPhi_Nm1[i][jj]->SetLineColor(1);
      H_njet_Nm1[i][jj]->SetLineColor(1);

      H_MET_Nm1[i][jj]->SetLineWidth(1);
      H_METPhi_Nm1[i][jj]->SetLineWidth(1);
      H_dphoMETPhi_Nm1[i][jj]->SetLineWidth(1);
      H_njet_Nm1[i][jj]->SetLineWidth(1);
      
      
      if(i==nfile-1){
	//H_SeedTime_Nm1[i][jj]->SetLineWidth(2);
	H_MET_Nm1[i][jj]->SetLineWidth(2);
	H_METPhi_Nm1[i][jj]->SetLineWidth(2);
	H_dphoMETPhi_Nm1[i][jj]->SetLineWidth(2);
	H_njet_Nm1[i][jj]->SetLineWidth(2);
	

	//H_SeedTime_Nm1[i][jj]->SetMarkerStyle(8);
	H_MET_Nm1[i][jj]->SetMarkerStyle(8);
	H_METPhi_Nm1[i][jj]->SetMarkerStyle(8);
	H_dphoMETPhi_Nm1[i][jj]->SetMarkerStyle(8);
	H_njet_Nm1[i][jj]->SetMarkerStyle(8);
	

	//H_SeedTime_Nm1[i][jj]->SetMarkerSize(0.8);
	H_MET_Nm1[i][jj]->SetMarkerSize(0.8);
	H_METPhi_Nm1[i][jj]->SetMarkerSize(0.8);
	H_dphoMETPhi_Nm1[i][jj]->SetMarkerSize(0.8);
	H_njet_Nm1[i][jj]->SetMarkerSize(0.8);
	
      }

      for(Int_t kk=0; kk<njet; kk++){
	H_djetMETPhi_Nm1[i][jj][kk] = (TH1F*)fopen->Get(Form("h_jetpt/h_djetMETPhi_Nm1_%i_jet%i", jj, kk));
	if(i==0){
	  HA_djetMETPhi_Nm1[jj][kk] = (TH1F*)fopen->Get(Form("h_jetpt/h_djetMETPhi_Nm1_%i_jet%i", jj, kk))->Clone();
	  HA_djetMETPhi_Nm1[jj][kk]->Scale(scale[i]);
	  HA_djetMETPhi_Nm1[jj][kk]->SetFillStyle(3145);
	  HA_djetMETPhi_Nm1[jj][kk]->SetFillColor(12);
	  HA_djetMETPhi_Nm1[jj][kk]->SetMarkerSize(0.1);
	}

	if(i==nfile-2){
	  HSig_djetMETPhi_Nm1[jj][kk] = (TH1F*)fopen->Get(Form("h_jetpt/h_djetMETPhi_Nm1_%i_jet%i", jj, kk))->Clone();
	  HSig_djetMETPhi_Nm1[jj][kk]->Scale(scale[i]);
	  HSig_djetMETPhi_Nm1[jj][kk]->SetLineColor(kRed);
	  HSig_djetMETPhi_Nm1[jj][kk]->SetLineWidth(2);
	}
	
	H_djetMETPhi_Nm1[i][jj][kk]->Scale(scale[i]);
	if(i<nfile-1){
	  H_djetMETPhi_Nm1[i][jj][kk]->SetFillColor(cnum[i]);
	}
	
	H_djetMETPhi_Nm1[i][jj][kk]->SetLineColor(1);
	H_djetMETPhi_Nm1[i][jj][kk]->SetLineWidth(1);
	if(i==nfile-1){
	  H_djetMETPhi_Nm1[i][jj][kk]->SetLineWidth(2);
	  H_djetMETPhi_Nm1[i][jj][kk]->SetMarkerStyle(8);
	  H_djetMETPhi_Nm1[i][jj][kk]->SetMarkerSize(0.8);
	}
      }
      
    }
  }

  for(Int_t i=0; i<nfile-1; i++){
    if(i>0){
      for(Int_t jj=0; jj<2; jj++){
	HA_MET_Nm1[jj]->Add(H_MET_Nm1[i][jj]);
	HA_METPhi_Nm1[jj]->Add(H_METPhi_Nm1[i][jj]);
	HA_dphoMETPhi_Nm1[jj]->Add(H_dphoMETPhi_Nm1[i][jj]);
	HA_njet_Nm1[jj]->Add(H_njet_Nm1[i][jj]);
	for(Int_t kk=0; kk<njet; kk++){
	  HA_djetMETPhi_Nm1[jj][kk]->Add(H_djetMETPhi_Nm1[i][jj][kk]);
	}
      }
    }
  }

  THStack *HS_SeedTime_Nm1[nhisto];
  THStack *HS_MET_Nm1[nhisto];
  THStack *HS_METPhi_Nm1[nhisto];
  THStack *HS_dphoMETPhi_Nm1[nhisto];
  THStack *HS_njet_Nm1[nhisto];
  THStack *HS_djetMETPhi_Nm1[nhisto][njet];
  
  for(Int_t jj=0; jj<nhisto; jj++){
    //HS_SeedTime_Nm1[jj] = new THStack(Form("HS_SeedTime_Nm1_%i", jj), "HS_SeedTime_Nm1");
    HS_MET_Nm1[jj] = new THStack(Form("HS_MET_Nm1_%i", jj), "HS_MET_Nm1");
    HS_METPhi_Nm1[jj] = new THStack(Form("HS_METPhi_Nm1_%i", jj), "HS_METPhi_Nm1");
    HS_dphoMETPhi_Nm1[jj] = new THStack(Form("HS_dphoMETPhi_Nm1_%i", jj), "HS_dphoMETPhi_Nm1");
    HS_njet_Nm1[jj] = new THStack(Form("HS_njet_Nm1_%i", jj), "HS_njet_Nm1");
    for(Int_t kk=0; kk<njet; kk++){
      HS_djetMETPhi_Nm1[jj][kk] = new THStack(Form("HS_djetMETPhi_Nm1_%i_jet%i", jj, kk), "HS_djetMETPhi_Nm1");
    }
  }
  
  for(Int_t i=0; i<nfile-1; i++){
    for(Int_t jj=0; jj<nhisto; jj++){
      //HS_SeedTime_Nm1[jj]->Add(H_SeedTime_Nm1[i][jj]);
      HS_MET_Nm1[jj]->Add(H_MET_Nm1[i][jj]);
      HS_METPhi_Nm1[jj]->Add(H_METPhi_Nm1[i][jj]);
      HS_njet_Nm1[jj]->Add(H_njet_Nm1[i][jj]);
      
      c1->SetLogy();
      c1->Update();
    
      HS_dphoMETPhi_Nm1[jj]->Add(H_dphoMETPhi_Nm1[i][jj]);
      for(Int_t kk=0; kk<njet; kk++){
	HS_djetMETPhi_Nm1[jj][kk]->Add(H_djetMETPhi_Nm1[i][jj][kk]);
      }
    }    
  }

  //TLegend *lhs = new TLegend(0.6,0.7, 0.92, 0.90);
  TLegend *lhs = new TLegend(0.52, 0.66, 0.92, 0.90);//VBS
  lhs->SetTextSize(0.035);
  lhs->SetFillStyle(0);
  //TString ltext[20] = {"#gamma+jet", "W(#rightarrowl#nu)+jets", "Z(#rightarrow#nu#nu)+jets", "Z(#rightarrowll)+jets", "Monoboson", "Diboson", "Triboson", "Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug", "DATA"};//Znunug QCD
  TString ltext[20] = {"#gamma+jet", "W(#rightarrowl#nu)+jets", "Z(#rightarrow#nu#nu)+jets", "Z(#rightarrowll)+jets", "Monoboson", "Diboson", "Triboson", "Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug(VBS)", "DATA"};//Znunug VBS

  TH1F *hratio;
  Double_t x[3] = {-2000, 0.0, 2000};
  Double_t y[3] = {1, 1, 1};
  TGraph *tg = new TGraph(3, x, y);
  tg->SetLineColor(kRed);
  tg->SetLineWidth(1);
  tg->SetLineStyle(8);
  
  TPad *pad1 = new TPad("pad1", "", 0., 0.3, 1., 1.0);
  pad1->SetBottomMargin(0);
  pad1->SetLogy(1);
  pad1->Draw();
  
  TPad *pad2 = new TPad("pad2", "", 0., 0.02, 1., 0.3);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.3);
  pad2->Draw();
  
  writeExtraText = true;
  Int_t period = 0;
  if(year==2016){
    period = 1;
  }
  else if(year==2017){
    period = 2;
  }
  else if(year==2018){
    period = 3;
  }
  
  for(Int_t jj=0; jj<2; jj++){
    /*
    pad1->cd();
    HS_SeedTime_Nm1[jj]->Draw("HIST");
    H_SeedTime_Nm1[nfile-1][jj]->Draw("SAME");
    HS_SeedTime_Nm1[jj]->GetYaxis()->SetTitle("Events");
    HS_SeedTime_Nm1[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_SeedTime_Nm1[jj]->GetXaxis()->SetTitle("");
    HS_SeedTime_Nm1[jj]->GetXaxis()->SetLabelSize(0);
    HS_SeedTime_Nm1[jj]->SetMaximum(10000000);
    HS_SeedTime_Nm1[jj]->SetMinimum(1);
    if(jj==1){HS_SeedTime_Nm1[jj]->SetMaximum(100); HS_SeedTime_Nm1[jj]->SetMinimum(0.001);}
    lhs->Clear();
    for(Int_t ifile=0; ifile<nfile; ifile++){
      if(ifile < nfile-1) lhs->AddEntry(H_SeedTime_Nm1[ifile][jj], ltext[ifile], "F");
      if(ifile == nfile-1) lhs->AddEntry(H_SeedTime_Nm1[ifile][jj], ltext[ifile], "PL");
    }
    lhs->Draw("SAME");
    title = HS_SeedTime_Nm1[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_SeedTime_Nm1[jj], H_SeedTime_Nm1[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("SeedTime");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    */
    pad1->cd();
    HS_MET_Nm1[jj]->Draw("HIST");
    H_MET_Nm1[nfile-1][jj]->Draw("SAME");
    HSig_MET_Nm1[jj]->Draw("HISTSAME");
    HA_MET_Nm1[jj]->Draw("E2 SAME");
    HS_MET_Nm1[jj]->GetYaxis()->SetTitle("Events");
    HS_MET_Nm1[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_MET_Nm1[jj]->GetXaxis()->SetTitle("");
    HS_MET_Nm1[jj]->GetXaxis()->SetLabelSize(0);
    HS_MET_Nm1[jj]->SetMaximum(100000);
    HS_MET_Nm1[jj]->SetMinimum(1);
    if(jj==1){HS_MET_Nm1[jj]->SetMaximum(10000); HS_MET_Nm1[jj]->SetMinimum(0.01);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      
      if(ifile == nfile-1) lhs->AddEntry(H_MET_Nm1[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_MET_Nm1[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_MET_Nm1[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_MET_Nm1[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_MET_Nm1[jj], H_MET_Nm1[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("E_{T}^{miss}");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_METPhi_Nm1[jj]->Draw("HIST");
    H_METPhi_Nm1[nfile-1][jj]->Draw("SAME");
    HSig_METPhi_Nm1[jj]->Draw("HISTSAME");
    HA_METPhi_Nm1[jj]->Draw("E2 SAME");
    HS_METPhi_Nm1[jj]->GetYaxis()->SetTitle("Events");
    HS_METPhi_Nm1[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_METPhi_Nm1[jj]->GetXaxis()->SetTitle("");
    HS_METPhi_Nm1[jj]->GetXaxis()->SetLabelSize(0);
    HS_METPhi_Nm1[jj]->SetMaximum(1000000);
    HS_METPhi_Nm1[jj]->SetMinimum(100);
    if(jj==1){HS_METPhi_Nm1[jj]->SetMaximum(10000); HS_METPhi_Nm1[jj]->SetMinimum(1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      
      if(ifile == nfile-1) lhs->AddEntry(H_METPhi_Nm1[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_METPhi_Nm1[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_METPhi_Nm1[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_METPhi_Nm1[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_METPhi_Nm1[jj], H_METPhi_Nm1[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#phi^{MET}");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_dphoMETPhi_Nm1[jj]->Draw("HIST");
    H_dphoMETPhi_Nm1[nfile-1][jj]->Draw("SAME");
    HSig_dphoMETPhi_Nm1[jj]->Draw("HISTSAME");
    HA_dphoMETPhi_Nm1[jj]->Draw("E2 SAME");
    HS_dphoMETPhi_Nm1[jj]->GetYaxis()->SetTitle("Events");
    HS_dphoMETPhi_Nm1[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_dphoMETPhi_Nm1[jj]->GetXaxis()->SetTitle("");
    HS_dphoMETPhi_Nm1[jj]->GetXaxis()->SetLabelSize(0);
    HS_dphoMETPhi_Nm1[jj]->SetMaximum(100000);
    HS_dphoMETPhi_Nm1[jj]->SetMinimum(0.1);
    if(jj==1){HS_dphoMETPhi_Nm1[jj]->SetMaximum(10000); HS_dphoMETPhi_Nm1[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      
      if(ifile == nfile-1) lhs->AddEntry(H_dphoMETPhi_Nm1[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_dphoMETPhi_Nm1[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_dphoMETPhi_Nm1[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_dphoMETPhi_Nm1[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dphoMETPhi_Nm1[jj], H_dphoMETPhi_Nm1[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#Delta#phi(#gamma , E_{T}^{miss})");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_njet_Nm1[jj]->Draw("HIST");
    H_njet_Nm1[nfile-1][jj]->Draw("SAME");
    HSig_njet_Nm1[jj]->Draw("HISTSAME");
    HA_njet_Nm1[jj]->Draw("E2 SAME");
    HS_njet_Nm1[jj]->GetXaxis()->SetTitle("");
    HS_njet_Nm1[jj]->GetYaxis()->SetTitle("Events");
    //HS_njet_Nm1[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_njet_Nm1[jj]->SetMaximum(10000);
    //HS_njet_Nm1[jj]->SetMinimum(10);//2016
    HS_njet_Nm1[jj]->SetMinimum(0.1);//2017
    if(jj==1){HS_njet_Nm1[jj]->SetMaximum(100000); HS_njet_Nm1[jj]->SetMinimum(0.1);}
    lhs->Clear();
    lhs->SetNColumns(2);
    for(Int_t ifile=0; ifile<nfile; ifile++){
      
      if(ifile == nfile-1) lhs->AddEntry(H_njet_Nm1[ifile][jj], ltext[ifile], "PL");
      if(ifile == nfile-2) lhs->AddEntry(HSig_njet_Nm1[jj], ltext[ifile], "L");
      if(ifile < nfile-1) lhs->AddEntry(H_njet_Nm1[ifile][jj], ltext[ifile], "F");
    }
    lhs->Draw("SAME");
    title = HS_njet_Nm1[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_njet_Nm1[jj], H_njet_Nm1[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("Njet");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, period, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    for(Int_t kk=0; kk<njet; kk++){
      pad1->cd();
      HS_djetMETPhi_Nm1[jj][kk]->Draw("HIST");
      H_djetMETPhi_Nm1[nfile-1][jj][kk]->Draw("SAME");
      HSig_djetMETPhi_Nm1[jj][kk]->Draw("HISTSAME");
      HA_djetMETPhi_Nm1[jj][kk]->Draw("E2 SAME");
      HS_djetMETPhi_Nm1[jj][kk]->GetXaxis()->SetTitle("");
      HS_djetMETPhi_Nm1[jj][kk]->GetYaxis()->SetTitle("Events");
      // HS_djetMETPhi_Nm1[jj][kk]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
      HS_djetMETPhi_Nm1[jj][kk]->SetMaximum(100000);
      HS_djetMETPhi_Nm1[jj][kk]->SetMinimum(1);
      if(jj==1){HS_djetMETPhi_Nm1[jj][kk]->SetMaximum(10000); HS_djetMETPhi_Nm1[jj][kk]->SetMinimum(1);};
      lhs->Clear();
      lhs->SetNColumns(2);
      for(Int_t ifile=0; ifile<nfile; ifile++){
	
	if(ifile == nfile-1) lhs->AddEntry(H_djetMETPhi_Nm1[ifile][jj][kk], ltext[ifile], "PL");
	if(ifile == nfile-2) lhs->AddEntry(HSig_djetMETPhi_Nm1[jj][kk], ltext[ifile], "L");
	if(ifile < nfile-1) lhs->AddEntry(H_djetMETPhi_Nm1[ifile][jj][kk], ltext[ifile], "F");
      }
      lhs->Draw("SAME");
      title = HS_djetMETPhi_Nm1[jj][kk]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_djetMETPhi_Nm1[jj][kk], H_djetMETPhi_Nm1[nfile-1][jj][kk]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle("#Delta#phi(jet, E_{T}^{miss})");
      hratio->Draw("EP");
      tg->Draw("LSAME");
      CMS_lumi(pad1, period, 0);
      c1->Update();
      c1->RedrawAxis();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    }
  }
  
}
