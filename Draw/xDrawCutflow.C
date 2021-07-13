#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "./Drawlumi/CMS_lumi.C"
using namespace std;
//#define nfile 4
#define nfile 4
#define nhisto 2
#define ncolor 10
void xDrawCutflow(Int_t year){
  ofstream ftext;
  /*
  TString rootname[5] = {"/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_GJet_Pt-15To6000_TuneCUETP8M1-Flat_13TeV_pythia8_20M_0000/210505_040352/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_WGToLNuG_01J_5f_PtG_130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0000/210505_082513/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_ZLLGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph_0000/210505_064501/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_ZGTo2NuG_PtG-130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0000/210505_064756/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]
  */
TString rootname[5] = {
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/output_merge.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_Wg_aMCatNLO/210707_180617/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_Zg_aMCatNLO/210708_013510/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_Znunug_pt130/210707_175756/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]
  

//TString rootname[5] = {
//			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph-pythia8/210531_074603/output_ggtree.root"
//};
string hexcolor[ncolor] = {"#9297db", "#ffea00", "#4caf50", "#ff9e00", "#03a9f4", "#ff3360", "#0066ff", "#F0F5E1","#DCF563", "#2F81A3" };
//string hexcolor[ncolor] = {"#7FB5F5", "#EA80FC", "#FF9100", "#FFF176", "#0066FF", "#69F0AE","#FF1744",  "#0066FF"};
  TColor *color[ncolor];
  Int_t cnum[ncolor];
  for(Int_t j=0; j<ncolor; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  }

  TH1F *H_phoEB_pt[nfile][nhisto];
  TH1F *H_phoEB_pt_200[nfile][nhisto];
  TH1F *H_phoEB_pt_M[nfile][nhisto];
  TH1F *H_phoEB_pt_chworst[nfile][nhisto];
  TH1F *H_phoEB_pt_phoptoverMET[nfile][nhisto];
  TH1F *H_phoEB_pt_MET[nfile][nhisto];
  TH1F *H_phoEB_pt_dphoMETPhi[nfile][nhisto];
  TH1F *H_phoEB_pt_djetMETPhi[nfile][nhisto];
  TH1F *H_phoEB_pt_jetveto[nfile][nhisto];
  TH1F *H_phoEB_pt_HLT[nfile][nhisto];

  TGraphAsymmErrors *tgAE_phoEB_pt[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_200[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_M[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_chworst[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_phoptoverMET[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_MET[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_dphoMETPhi[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_djetMETPhi[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_jetveto[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_HLT[nfile][nhisto];

  TGraphAsymmErrors *tgAE_phoEB_pt_Meff[nfile][nhisto];

  TLegend *ltgAE = new TLegend(0.65,0.65, 0.75, 0.75);
  ltgAE->SetBorderSize(0);
  ltgAE->SetFillStyle(0);
  TArrow *ArtgAE = new TArrow(205, 0.001, 205, 0.90, 0.03, "|>");
  ArtgAE->SetAngle(40);
  ArtgAE->SetLineWidth(1);
  
  
  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  TTree *t;
  
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    t = (TTree*)fopen->Get("t");
    for(Int_t jj=0; jj<nhisto; jj++){
      H_phoEB_pt[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_%i", jj));
      H_phoEB_pt_200[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_200_%i", jj));
      H_phoEB_pt_M[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_M_%i", jj));
      H_phoEB_pt_chworst[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_chworst_%i", jj));
      H_phoEB_pt_phoptoverMET[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_phoptoverMET_%i", jj));
      H_phoEB_pt_MET[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_MET_%i", jj));
      H_phoEB_pt_dphoMETPhi[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_dphoMETPhi_%i", jj));
      H_phoEB_pt_jetveto[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_jetveto_%i", jj));
      H_phoEB_pt_djetMETPhi[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_djetMETPhi_%i", jj));
      H_phoEB_pt_HLT[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_pt_HLT_%i", jj));
    
      tgAE_phoEB_pt_200[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_M[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_chworst[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_phoptoverMET[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_MET[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_dphoMETPhi[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_jetveto[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_djetMETPhi[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_HLT[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_Meff[i][jj] = new TGraphAsymmErrors();
      
      tgAE_phoEB_pt_200[i][jj]->Divide(H_phoEB_pt_200[i][jj], H_phoEB_pt_200[i][jj]);
      tgAE_phoEB_pt_M[i][jj]->Divide(H_phoEB_pt_M[i][jj], H_phoEB_pt_200[i][jj]);
      tgAE_phoEB_pt_chworst[i][jj]->Divide(H_phoEB_pt_chworst[i][jj], H_phoEB_pt_200[i][jj]);
      tgAE_phoEB_pt_phoptoverMET[i][jj]->Divide(H_phoEB_pt_phoptoverMET[i][jj], H_phoEB_pt_200[i][jj]);
      tgAE_phoEB_pt_MET[i][jj]->Divide(H_phoEB_pt_MET[i][jj], H_phoEB_pt_200[i][jj]);
      tgAE_phoEB_pt_dphoMETPhi[i][jj]->Divide(H_phoEB_pt_dphoMETPhi[i][jj], H_phoEB_pt_200[i][jj]);
      tgAE_phoEB_pt_jetveto[i][jj]->Divide(H_phoEB_pt_jetveto[i][jj], H_phoEB_pt_200[i][jj]);
      tgAE_phoEB_pt_djetMETPhi[i][jj]->Divide(H_phoEB_pt_djetMETPhi[i][jj], H_phoEB_pt_200[i][jj]);
      tgAE_phoEB_pt_HLT[i][jj]->Divide(H_phoEB_pt_HLT[i][jj], H_phoEB_pt_djetMETPhi[i][jj]);
      tgAE_phoEB_pt_Meff[i][jj]->Divide(H_phoEB_pt_M[i][jj], H_phoEB_pt_200[i][jj]);
    
      tgAE_phoEB_pt_200[i][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_200_%i_%i", i, jj), "pho cut pt200");
      tgAE_phoEB_pt_M[i][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_M_%i_%i", i, jj), "pho cut M ID");
      tgAE_phoEB_pt_chworst[i][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_chworst_%i_%i", i, jj), "pho cut chworst");
      tgAE_phoEB_pt_phoptoverMET[i][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_phoptoverMET_%i_%i", i, jj));
      tgAE_phoEB_pt_MET[i][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_MET_%i_%i", i, jj), "pho cut MET");
      tgAE_phoEB_pt_dphoMETPhi[i][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_dphoMETPhi_%i_%i", i, jj), "pho cut dphoMETPhi");
      tgAE_phoEB_pt_jetveto[i][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_jetveto_%i_%i", i, jj), "pho jetveto");
      tgAE_phoEB_pt_djetMETPhi[i][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_djetMETPhi_%i_%i", i, jj), "pho cut djetMETPhi");
      tgAE_phoEB_pt_HLT[i][jj]->SetNameTitle(Form("tgAE_phoEB_pteff_HLT_%i_%i", i, jj), "pho cut HLT");
      tgAE_phoEB_pt_Meff[i][jj]->SetNameTitle(Form("tgAE_phoEB_pt_Meff_%i_%i", i, jj), "pho M ID bit eff");

    
      tgAE_phoEB_pt_200[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_M[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_chworst[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_phoptoverMET[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_MET[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_dphoMETPhi[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_jetveto[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_djetMETPhi[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_HLT[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_pt_Meff[i][jj]->SetMarkerStyle(8);
    

      //tgAE_phoEB_pt_200[i][jj]->SetMarkerColor(cnum[0]);
      tgAE_phoEB_pt_M[i][jj]->SetMarkerColor(cnum[0]);
      tgAE_phoEB_pt_chworst[i][jj]->SetMarkerColor(cnum[1]);
      tgAE_phoEB_pt_phoptoverMET[i][jj]->SetMarkerColor(cnum[2]);
      tgAE_phoEB_pt_MET[i][jj]->SetMarkerColor(cnum[3]);
      tgAE_phoEB_pt_dphoMETPhi[i][jj]->SetMarkerColor(cnum[4]);
      tgAE_phoEB_pt_jetveto[i][jj]->SetMarkerColor(cnum[5]);
      tgAE_phoEB_pt_djetMETPhi[i][jj]->SetMarkerColor(cnum[6]);
      tgAE_phoEB_pt_HLT[i][jj]->SetMarkerColor(cnum[7]);
      tgAE_phoEB_pt_Meff[i][jj]->SetMarkerColor(cnum[0]);
    
      tgAE_phoEB_pt_HLT[i][jj]->Draw("APL");
      tgAE_phoEB_pt_HLT[i][jj]->GetXaxis()->SetTitle("P_{T}^{#gamma}");
      tgAE_phoEB_pt_HLT[i][jj]->GetYaxis()->SetTitle("Efficiency");
      tgAE_phoEB_pt_HLT[i][jj]->GetXaxis()->SetLimits(0, 500);
      tgAE_phoEB_pt_HLT[i][jj]->SetMaximum(1.05);
      tgAE_phoEB_pt_HLT[i][jj]->SetMinimum(0.0);
      ltgAE->Clear();
      ltgAE->AddEntry(tgAE_phoEB_pt_HLT[i][jj], "HLT_175", "PE");
      ltgAE->Draw("SAME");
      //tgAE_phoEB_pt_HLT[i][jj]->GetPoint(turnpoint, x, y);
      //ArtgAE = new TArrow(x, 0.001, x, y-0.01, 0.03, "|>");
      //ArtgAE->SetAngle(40);
      //ArtgAE->SetLineWidth(1);
      //ArtgAE->Draw();
      title = tgAE_phoEB_pt_HLT[i][jj]->GetName();
      c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    
      tgAE_phoEB_pt_200[i][jj]->Draw("APL"); 
      title = tgAE_phoEB_pt_200[i][jj]->GetName();
      //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    
      tgAE_phoEB_pt_M[i][jj]->Draw("APL");
      title = tgAE_phoEB_pt_M[i][jj]->GetName();
      //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    
      tgAE_phoEB_pt_chworst[i][jj]->Draw("APL");
      title = tgAE_phoEB_pt_chworst[i][jj]->GetName();

      tgAE_phoEB_pt_phoptoverMET[i][jj]->Draw("APL");
      title = tgAE_phoEB_pt_phoptoverMET[i][jj]->GetName();
    
      tgAE_phoEB_pt_MET[i][jj]->Draw("APL");
      title = tgAE_phoEB_pt_MET[i][jj]->GetName();
      //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    
      tgAE_phoEB_pt_dphoMETPhi[i][jj]->Draw("APL");
      title = tgAE_phoEB_pt_dphoMETPhi[i][jj]->GetName(); 
      //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
      tgAE_phoEB_pt_jetveto[i][jj]->Draw("APL");
      title = tgAE_phoEB_pt_jetveto[i][jj]->GetName();

      tgAE_phoEB_pt_djetMETPhi[i][jj]->Draw("APL");
      title = tgAE_phoEB_pt_djetMETPhi[i][jj]->GetName();
    
      tgAE_phoEB_pt_Meff[i][jj]->Draw("APL");
      tgAE_phoEB_pt_Meff[i][jj]->GetXaxis()->SetTitle("P_{T}^{#gamma}");
      tgAE_phoEB_pt_Meff[i][jj]->GetYaxis()->SetTitle("Efficiency");
      tgAE_phoEB_pt_Meff[i][jj]->GetXaxis()->SetLimits(0, 500);
      tgAE_phoEB_pt_Meff[i][jj]->SetMaximum(1.05);
      tgAE_phoEB_pt_Meff[i][jj]->SetMinimum(0.0);
      ltgAE->Clear();
      ltgAE->AddEntry(tgAE_phoEB_pt_Meff[i][jj], "pho M ID", "PE");
      ltgAE->Draw("SAME");
      title = tgAE_phoEB_pt_Meff[i][jj]->GetName();
      c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    }					  
  }

  
  TMultiGraph *mg[nfile][nhisto];
  TLegend *lmg = new TLegend(0.18,0.60, 0.28, 0.88);
  lmg->SetTextSize(0.03);

  TF1 *fitresult = new TF1("fitresult", "pol0", 20, 500);
  Double_t yinter[15][nfile][nhisto], err[15][nfile][nhisto];
  
  writeExtraText = true;
  for(Int_t i=0; i<nfile; i++){
    for(Int_t jj=0; jj<nhisto; jj++){
    mg[i][jj] = new TMultiGraph();
    //mg[i][jj]->Add(tgAE_phoEB_pt_HLT[i][jj]);
   
    //mg[i][jj]->Add(tgAE_phoEB_pt_200[i][jj]);
    mg[i][jj]->Add(tgAE_phoEB_pt_M[i][jj]);
    mg[i][jj]->Add(tgAE_phoEB_pt_chworst[i][jj]);
    //mg[i][jj]->Add(tgAE_phoEB_pt_phoptoverMET[i][jj]);
    mg[i][jj]->Add(tgAE_phoEB_pt_MET[i][jj]);
    mg[i][jj]->Add(tgAE_phoEB_pt_dphoMETPhi[i][jj]);
    mg[i][jj]->Add(tgAE_phoEB_pt_jetveto[i][jj]);
    mg[i][jj]->Add(tgAE_phoEB_pt_djetMETPhi[i][jj]);
    
    mg[i][jj]->GetXaxis()->SetTitle("phoEt#[]{GeV}");
    mg[i][jj]->GetYaxis()->SetTitle("Efficiency");
    mg[i][jj]->GetXaxis()->SetLimits(-1, 500);
    mg[i][jj]->SetMaximum(1.05);
    mg[i][jj]->SetMinimum(0);
    //if(i==0) mg[i][jj]->SetMaximum(0.055);
    mg[i][jj]->SetTitle(Form("cutflow_%i_%i", i, jj));
    mg[i][jj]->Draw("APE");
    
    lmg->Clear();
    //lmg->AddEntry(tgAE_phoEB_pt_HLT[i][jj], "HLT_175", "PE");
    //lmg->AddEntry(tgAE_phoEB_pt_200[i][jj], "pt > 200", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_M[i][jj], "Medium ID", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_chworst[i][jj], "chworst < 1.3", "PE");
    //lmg->AddEntry(tgAE_phoEB_pt_phoptoverMET[i][jj], "P_{T}^{#gamma}/MET < 2.344+0.00475#times(P_{T}^{#gamma}-280)", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_MET[i][jj], "MET > 140+0.138#times(P_{T}^{#gamma}-500) GeV", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_dphoMETPhi[i][jj], "#Delta#phi(#gamma, MET) > 1.2", "PE");
    lmg->AddEntry(tgAE_phoEB_pt_djetMETPhi[i][jj], "#Delta#phi(jet, MET) > 0.5", "PE");
    if(jj==0)lmg->AddEntry(tgAE_phoEB_pt_jetveto[i][jj], "jet veto", "PE");
    else if(jj==1)lmg->AddEntry(tgAE_phoEB_pt_jetveto[i][jj], "jet selection", "PE");
    lmg->Draw("SAME");
    title = mg[i][jj]->GetTitle();
    CMS_lumi(c1, year, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    tgAE_phoEB_pt_200[i][jj]->Fit(fitresult,"", "", 200, 500);
    yinter[0][i][jj] = fitresult->GetParameter(0);
    err[0][i][jj] = fitresult->GetParError(0);
    
    tgAE_phoEB_pt_M[i][jj]->Fit(fitresult,"", "", 200, 500);
    yinter[1][i][jj] = fitresult->GetParameter(0);
    err[1][i][jj] = fitresult->GetParError(0);
    
    tgAE_phoEB_pt_chworst[i][jj]->Fit(fitresult, "", "", 200, 500);
    yinter[2][i][jj] = fitresult->GetParameter(0);
    err[2][i][jj] = fitresult->GetParError(0);

    tgAE_phoEB_pt_phoptoverMET[i][jj]->Fit(fitresult, "", "", 200, 500);
    yinter[4][i][jj] = fitresult->GetParameter(0);
    err[4][i][jj] = fitresult->GetParError(0);
    
    tgAE_phoEB_pt_MET[i][jj]->Fit(fitresult,"", "", 200, 500);
    yinter[5][i][jj] = fitresult->GetParameter(0);
    err[5][i][jj] = fitresult->GetParError(0);
    
    tgAE_phoEB_pt_dphoMETPhi[i][jj]->Fit(fitresult,"", "", 200, 500);
    yinter[6][i][jj] = fitresult->GetParameter(0);
    err[6][i][jj] = fitresult->GetParError(0);
    
    tgAE_phoEB_pt_jetveto[i][jj]->Fit(fitresult, "", "", 200, 500);
    yinter[7][i][jj] = fitresult->GetParameter(0);
    err[7][i][jj] = fitresult->GetParError(0);
    
    tgAE_phoEB_pt_djetMETPhi[i][jj]->Fit(fitresult, "", "", 200, 500);
    yinter[8][i][jj] = fitresult->GetParameter(0);
    err[8][i][jj] = fitresult->GetParError(0);
    
    tgAE_phoEB_pt_HLT[i][jj]->Fit(fitresult,"", "", 200, 500);
    yinter[9][i][jj] = fitresult->GetParameter(0);
    err[9][i][jj] = fitresult->GetParError(0);
    
    tgAE_phoEB_pt_Meff[i][jj]->Fit(fitresult,"", "", 200, 500);
    yinter[10][i][jj] = fitresult->GetParameter(0);
    err[10][i][jj] = fitresult->GetParError(0);
    }
  }

  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  for(Int_t i=0; i<nfile; i++){
    ftext << "file " << i << endl;
    for(Int_t jj=0; jj<nhisto; jj++){
      for(Int_t j=0; j<10; j++){
	ftext << j << ") cut efficiency: " << yinter[j][i][jj] << " error: " << err[j][i][jj] << endl;
      }
    }
  }
  
  ftext.close();
  
  
}

 
