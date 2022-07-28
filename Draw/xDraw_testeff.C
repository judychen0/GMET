#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "./Drawlumi/CMS_lumi.C"
using namespace std;

#define nfile 11
#define nhisto 2
#define ncolor 11

void xDraw_testeff(Int_t year){
  ofstream ftext;
  TString rootname[20];
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/WJet/output_merge_WJet.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/ZJet/output_merge_ZJet.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/DYJet/output_merge_DYJet.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Wg_aMCatNLO/output_ggtree.root";
    rootname[5] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Zg_aMCatNLO/output_ggtree.root";
    rootname[6] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_TT_aMCatNLO/output_ggtree.root";
    rootname[7] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/output_ggtree.root";
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
    rootname[11] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/job_UL18_Znunug_VBS/output_ggtree.root";
  }

  string hexcolor[ncolor] = {"#fff176", "#03a9f4", "#ff3360", "#3d5afe", "#b23b8c", "#96ff5a", "#a983d3", "#ff9e00", "#00FFFF", "#FFCCCC", "#67ccc1"};
  TColor *color[ncolor];
  Int_t cnum[ncolor];
  for(Int_t j=0; j<ncolor; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  }

  TH1F *H_phoEB_pt_leptonveto[nfile];
  TH1F *H_phoEB_pt_MET[nfile];
  TH1F *H_MET_test[nfile][4];
  TH1F *H_dphoMETPhi_test[nfile][4];

  TGraphAsymmErrors *tgAE_MET_test[nfile][4];
  TGraphAsymmErrors *tgAE_dphoMETPhi_test[nfile][4];

  TLegend *ltgAE = new TLegend(0.65,0.65, 0.75, 0.75);
  ltgAE->SetBorderSize(0);
  ltgAE->SetFillStyle(0);

  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  TTree *t;

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  Float_t integral[2][nfile][4];//[2][MET, dphoMETPhi]; [4][ncut]

  for(Int_t i=0; i<nfile; i++){

    fopen = new TFile(rootname[i]);
    
    H_phoEB_pt_leptonveto[i] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_leptonveto_%i", 1));
    H_phoEB_pt_MET[i] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_MET_%i", 1));
      
    for(Int_t icut=0; icut<4; icut++){
      H_MET_test[i][icut] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_test_cut%i_%i", icut, 1));
      H_dphoMETPhi_test[i][icut] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_test_cut%i_%i", icut, 1));

      tgAE_MET_test[i][icut] = new TGraphAsymmErrors();
      tgAE_dphoMETPhi_test[i][icut] = new TGraphAsymmErrors();

      integral[0][i][icut] = H_MET_test[i][icut]->Integral(1, -1);
      integral[1][i][icut] = H_dphoMETPhi_test[i][icut]->Integral(1, -1);
	
      tgAE_MET_test[i][icut]->Divide(H_MET_test[i][icut], H_phoEB_pt_leptonveto[i]);
      tgAE_dphoMETPhi_test[i][icut]->Divide(H_dphoMETPhi_test[i][icut], H_phoEB_pt_MET[i]);

      tgAE_MET_test[i][icut]->SetNameTitle(Form("tgAE_phoEB_pteff_MET_cut%i_%i", icut, 1), "MET cut test");
      tgAE_dphoMETPhi_test[i][icut]->SetNameTitle(Form("tgAE_phoEB_pteff_dphoMETPhi_cut%i_%i", icut, 1), "dphoMETPhi cut test");

      tgAE_MET_test[i][icut]->SetMarkerStyle(8);
      tgAE_dphoMETPhi_test[i][icut]->SetMarkerStyle(8);

      tgAE_MET_test[i][icut]->SetMarkerColor(cnum[i]);
      tgAE_dphoMETPhi_test[i][icut]->SetMarkerColor(cnum[i]);
	
    }
    
  }

  TF1 *fitresult = new TF1("fitresult", "pol0", 200, 500);
  Double_t yinter[2][nfile][4], err[2][nfile][4];

  for(Int_t i=0; i<nfile; i++){
    
    for(Int_t icut=0; icut<4; icut++){
      tgAE_MET_test[i][icut]->Fit(fitresult, "", "", 210, 500);
      yinter[0][i][icut] = fitresult->GetParameter(0);
      err[0][i][icut] = fitresult->GetParError(0);
	
      tgAE_dphoMETPhi_test[i][icut]->Fit(fitresult, "", "", 210, 500);
      yinter[1][i][icut] = fitresult->GetParameter(0);
      err[1][i][icut] = fitresult->GetParError(0);
    }
    
  }

   Float_t lumiscale[20] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.155635, 0.393159, 0.54324, 0.019488, 0.0081035};//[GJet, WJet, ZJet, DYJet, monoboson, diboson, triboson, Wg, Zg, ttJET, Znng, ZnngVBS]
  
  ftext.open("MET_testeff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  Double_t xMET[4] = {80, 100, 140, 170};
  Double_t yMET[4][nfile] = {0};
  Double_t xdphoMETPhi[4] = {0.5, 0.8, 1.0, 1.2};
  Double_t ydphoMETPhi[4][nfile] = {0};

  
  //if(i==1 || i==2) continue;
    
    
  for(Int_t icut=0; icut<4; icut++){
    ftext << icut << "cut" << endl;
    for(Int_t i=0; i<nfile; i++){
      
      //ftext << j << ") cut efficiency: " << yinter[j][i][jj] << " error: " << err[j][i][jj] << endl;
      //ftext << "1) MET cut efficiency: [80, 100, 140, 170] " << endl;
      //ftext << icut << " cut eff : " << integral[0][i][icut]/H_phoEB_pt_210[i]->Integral(1, -1) << " error: " << err[0][i][icut] << endl;
      ftext << i << " MET cut eff : " << yinter[0][i][icut] << " error: " << err[0][i][icut] << endl;
      //yMET[icut][i] = integral[0][i][icut]/H_phoEB_pt_210[i]->Integral(1, -1);
      yMET[icut][i] = yinter[0][i][icut];

      
    }
    for(Int_t i=0; i<nfile; i++){
     
      //ftext << "2) dphoMETPhi cut efficiency: [0.5, 0.8, 1.0, 1.2] " << endl;
      //ftext << icut << " cut eff : " << integral[1][i][icut]/H_phoEB_pt_210[i]->Integral(1, -1) << " error: " << err[1][i][icut] << endl;
      ftext << i << " dphoMETPhi cut eff : " << yinter[1][i][icut] << " error: " << err[1][i][icut] << endl;
      ydphoMETPhi[icut][i] = yinter[1][i][icut];
    }    
  }
  
  ftext.close();

  TGraph *tg_MET_testeff[nfile];  
  TGraph *tg_dphoMETPhi_testeff[nfile];
  Double_t temp_METy[4] = {0};
  Double_t temp_dphoMETPhiy[4] = {0};

  for(Int_t i=0; i<nfile; i++){
    
    for(Int_t icut=0; icut<4; icut++){
      temp_METy[icut] = yMET[icut][i];
      temp_dphoMETPhiy[icut] = ydphoMETPhi[icut][i];
    }
    tg_MET_testeff[i] = new TGraph(4, xMET, temp_METy);
    tg_dphoMETPhi_testeff[i] = new TGraph(4, xdphoMETPhi, temp_dphoMETPhiy);

    tg_MET_testeff[i]->SetMarkerStyle(8);
    tg_dphoMETPhi_testeff[i]->SetMarkerStyle(8);

    tg_MET_testeff[i]->SetMarkerSize(0.6);
    tg_dphoMETPhi_testeff[i]->SetMarkerSize(0.6);

    tg_MET_testeff[i]->SetMarkerColor(cnum[i]);
    tg_dphoMETPhi_testeff[i]->SetMarkerColor(cnum[i]);

    tg_MET_testeff[i]->SetLineColor(cnum[i]);
    tg_dphoMETPhi_testeff[i]->SetLineColor(cnum[i]);

    tg_MET_testeff[i]->SetLineWidth(2);
    tg_dphoMETPhi_testeff[i]->SetLineWidth(2);
 
  }

  TMultiGraph *mg_MET_testeff;
  TMultiGraph *mg_dphoMETPhi_testeff;
  TLegend *lmg = new TLegend(0.18,0.8, 0.78, 0.92);
  lmg->SetTextSize(0.03);

  TString ltext[20] = {"#gamma+jet", "W(#rightarrowl#nu)+jets", "Z(#rightarrow#nu#nu)+jets", "Z(#rightarrowll)+jets", "Monoboson", "Diboson", "Triboson", "Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug(VBS)"};
  
  
  mg_MET_testeff = new TMultiGraph();
  lmg->Clear();
  lmg->SetNColumns(3);
  for(Int_t i=0; i<nfile; i++){
    mg_MET_testeff->Add(tg_MET_testeff[i], "PL");
    lmg->AddEntry(tg_MET_testeff[i], ltext[i], "PL");
  }
  mg_MET_testeff->GetXaxis()->SetTitle("E_{T}^{miss} cut value");
  mg_MET_testeff->GetYaxis()->SetTitle("Efficiency");
  mg_MET_testeff->GetXaxis()->SetLimits(0, 200);
  mg_MET_testeff->SetMaximum(1.05);
  mg_MET_testeff->SetMinimum(0);
  mg_MET_testeff->SetTitle(Form("MET_testcut_%i", 1));
  mg_MET_testeff->Draw("APE");
  lmg->Draw("SAME");
  title = mg_MET_testeff->GetTitle();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  mg_dphoMETPhi_testeff = new TMultiGraph();
  lmg->Clear();
  lmg->SetNColumns(3);
   for(Int_t i=0; i<nfile; i++){
    mg_dphoMETPhi_testeff->Add(tg_dphoMETPhi_testeff[i], "PL");
    lmg->AddEntry(tg_dphoMETPhi_testeff[i], ltext[i], "PL");
  }
  mg_dphoMETPhi_testeff->GetXaxis()->SetTitle("#Delta#phi(#gamma , E_{T}^{miss}) cut value");
  mg_dphoMETPhi_testeff->GetYaxis()->SetTitle("Efficiency");
  mg_dphoMETPhi_testeff->GetXaxis()->SetLimits(0, 2);
  mg_dphoMETPhi_testeff->SetMaximum(1.05);
  mg_dphoMETPhi_testeff->SetMinimum(0);
  mg_dphoMETPhi_testeff->SetTitle(Form("dphoMETPhi_testcut_%i", 1));
  mg_dphoMETPhi_testeff->Draw("APE");
  lmg->Draw("SAME");
  title = mg_dphoMETPhi_testeff->GetTitle();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  
}
