#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "./Drawlumi/CMS_lumi.C"
using namespace std;
//#define nfile 4
#define nfile 12
#define nhisto 2
#define ncolor 10

void xDrawCutflow_nVtx(Int_t year){
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
    rootname[10] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/output_ggtree.root";
    rootname[11] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_UL17_Znunug_VBS/output_ggtree.root";
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

  string hexcolor[ncolor] = {"#b23b8c", "#ffea00", "#03a9f4", "#ff9e00", "#4caf50", "#ff3360", "#0066ff", "#F0F5E1","#DCF563", "#2F81A3" };
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

  TH1F *H_phoEB_nvtx_210[nfile][nhisto];
  TH1F *H_phoEB_nvtx_M[nfile][nhisto];
  TH1F *H_phoEB_nvtx_leptonveto[nfile][nhisto];
  TH1F *H_phoEB_nvtx_MET[nfile][nhisto];
  TH1F *H_phoEB_nvtx_dphoMETPhi[nfile][nhisto];
  TH1F *H_phoEB_nvtx_djetMETPhi[nfile][nhisto];
  TH1F *H_phoEB_nvtx_jetveto[nfile][nhisto];
  
  TGraphAsymmErrors *tgAE_phoEB_nvtx_210[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_nvtx_M[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_nvtx_leptonveto[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_nvtx_MET[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_nvtx_dphoMETPhi[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_nvtx_djetMETPhi[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_nvtx_jetveto[nfile][nhisto];

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

  Float_t integral[20][nfile][nhisto];

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    for(Int_t jj=0; jj<nhisto; jj++){

      H_phoEB_nvtx_210[i][jj] = (TH1F*)fopen->Get(Form("nVtx/h_phoEB_nvtx_210_%i", jj));
      H_phoEB_nvtx_M[i][jj] = (TH1F*)fopen->Get(Form("nVtx/h_phoEB_nvtx_M_%i", jj));
      H_phoEB_nvtx_leptonveto[i][jj] = (TH1F*)fopen->Get(Form("nVtx/h_phoEB_nvtx_leptonveto_%i", jj));
      H_phoEB_nvtx_MET[i][jj] = (TH1F*)fopen->Get(Form("nVtx/h_phoEB_nvtx_MET_%i", jj));
      H_phoEB_nvtx_dphoMETPhi[i][jj] = (TH1F*)fopen->Get(Form("nVtx/h_phoEB_nvtx_dphoMETPhi_%i", jj));
      H_phoEB_nvtx_djetMETPhi[i][jj] = (TH1F*)fopen->Get(Form("nVtx/h_phoEB_nvtx_djetMETPhi_%i", jj));
      H_phoEB_nvtx_jetveto[i][jj] = (TH1F*)fopen->Get(Form("nVtx/h_phoEB_nvtx_jetveto_%i", jj));

      tgAE_phoEB_nvtx_210[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_nvtx_M[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_nvtx_leptonveto[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_nvtx_MET[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_nvtx_dphoMETPhi[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_nvtx_djetMETPhi[i][jj] = new TGraphAsymmErrors();
      tgAE_phoEB_nvtx_jetveto[i][jj] = new TGraphAsymmErrors();

      H_phoEB_nvtx_210[i][jj]->ClearUnderflowAndOverflow();
      H_phoEB_nvtx_M[i][jj]->ClearUnderflowAndOverflow();
      H_phoEB_nvtx_MET[i][jj]->ClearUnderflowAndOverflow();
      H_phoEB_nvtx_dphoMETPhi[i][jj]->ClearUnderflowAndOverflow();
      H_phoEB_nvtx_djetMETPhi[i][jj]->ClearUnderflowAndOverflow();
      H_phoEB_nvtx_jetveto[i][jj]->ClearUnderflowAndOverflow();
      
      integral[0][i][jj] = H_phoEB_nvtx_210[i][jj]->Integral(1, -1);
      integral[1][i][jj] = H_phoEB_nvtx_M[i][jj]->Integral(1, -1);
      integral[2][i][jj] = H_phoEB_nvtx_leptonveto[i][jj]->Integral(1, -1);
      integral[3][i][jj] = H_phoEB_nvtx_MET[i][jj]->Integral(1, -1);
      integral[4][i][jj] = H_phoEB_nvtx_dphoMETPhi[i][jj]->Integral(1, -1);
      integral[5][i][jj] = H_phoEB_nvtx_djetMETPhi[i][jj]->Integral(1, -1);
      integral[6][i][jj] = H_phoEB_nvtx_jetveto[i][jj]->Integral(1, -1);

      H_phoEB_nvtx_210[i][jj]->Rebin(4);
      H_phoEB_nvtx_M[i][jj]->Rebin(4);
      H_phoEB_nvtx_MET[i][jj]->Rebin(4);
      H_phoEB_nvtx_dphoMETPhi[i][jj]->Rebin(4);
      H_phoEB_nvtx_djetMETPhi[i][jj]->Rebin(4);
      H_phoEB_nvtx_jetveto[i][jj]->Rebin(4);

      tgAE_phoEB_nvtx_210[i][jj]->Divide(H_phoEB_nvtx_210[i][jj], H_phoEB_nvtx_210[i][jj]);
      tgAE_phoEB_nvtx_M[i][jj]->Divide(H_phoEB_nvtx_M[i][jj], H_phoEB_nvtx_210[i][jj]);
      tgAE_phoEB_nvtx_leptonveto[i][jj]->Divide(H_phoEB_nvtx_leptonveto[i][jj], H_phoEB_nvtx_210[i][jj]);
      tgAE_phoEB_nvtx_MET[i][jj]->Divide(H_phoEB_nvtx_MET[i][jj], H_phoEB_nvtx_210[i][jj]);
      tgAE_phoEB_nvtx_dphoMETPhi[i][jj]->Divide(H_phoEB_nvtx_dphoMETPhi[i][jj], H_phoEB_nvtx_210[i][jj]);
      tgAE_phoEB_nvtx_djetMETPhi[i][jj]->Divide(H_phoEB_nvtx_djetMETPhi[i][jj], H_phoEB_nvtx_210[i][jj]);
      tgAE_phoEB_nvtx_jetveto[i][jj]->Divide(H_phoEB_nvtx_jetveto[i][jj], H_phoEB_nvtx_210[i][jj]);
      
      tgAE_phoEB_nvtx_210[i][jj]->SetNameTitle(Form("tgAE_phoEB_nvtxeff_210_%i_%i", i, jj), "pho cut pt210");
      tgAE_phoEB_nvtx_M[i][jj]->SetNameTitle(Form("tgAE_phoEB_nvtxeff_M_%i_%i", i, jj), "pho cut M ID");
      tgAE_phoEB_nvtx_leptonveto[i][jj]->SetNameTitle(Form("tgAE_phoEB_nvtxeff_leptonveto_%i_%i", i, jj), "pho cut leptonveto");
      tgAE_phoEB_nvtx_MET[i][jj]->SetNameTitle(Form("tgAE_phoEB_nvtxeff_MET_%i_%i", i, jj), "pho cut MET");
      tgAE_phoEB_nvtx_dphoMETPhi[i][jj]->SetNameTitle(Form("tgAE_phoEB_nvtxeff_dphoMETPhi_%i_%i", i, jj), "pho cut dphoMETPhi");
      tgAE_phoEB_nvtx_djetMETPhi[i][jj]->SetNameTitle(Form("tgAE_phoEB_nvtxeff_djetMETPhi_%i_%i", i, jj), "pho cut djetMETPhi");
      tgAE_phoEB_nvtx_jetveto[i][jj]->SetNameTitle(Form("tgAE_phoEB_nvtxeff_jetveto_%i_%i", i, jj), "pho jetveto");
      
      tgAE_phoEB_nvtx_210[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_nvtx_M[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_nvtx_leptonveto[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_nvtx_MET[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_nvtx_dphoMETPhi[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_nvtx_djetMETPhi[i][jj]->SetMarkerStyle(8);
      tgAE_phoEB_nvtx_jetveto[i][jj]->SetMarkerStyle(8);
      
      tgAE_phoEB_nvtx_M[i][jj]->SetMarkerColor(cnum[0]);
      tgAE_phoEB_nvtx_leptonveto[i][jj]->SetMarkerColor(cnum[1]);
      tgAE_phoEB_nvtx_MET[i][jj]->SetMarkerColor(cnum[3]);
      tgAE_phoEB_nvtx_dphoMETPhi[i][jj]->SetMarkerColor(cnum[4]);
      tgAE_phoEB_nvtx_djetMETPhi[i][jj]->SetMarkerColor(cnum[5]);
      tgAE_phoEB_nvtx_jetveto[i][jj]->SetMarkerColor(cnum[6]);

      tgAE_phoEB_nvtx_210[i][jj]->Draw("APL");
      tgAE_phoEB_nvtx_210[i][jj]->GetXaxis()->SetRangeUser(0, 100); 
      title = tgAE_phoEB_nvtx_210[i][jj]->GetName();
      //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    
      tgAE_phoEB_nvtx_M[i][jj]->Draw("APL");
      tgAE_phoEB_nvtx_M[i][jj]->GetXaxis()->SetRangeUser(0, 100);
      title = tgAE_phoEB_nvtx_M[i][jj]->GetName();
      //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));

      tgAE_phoEB_nvtx_leptonveto[i][jj]->Draw("APL");
      tgAE_phoEB_nvtx_leptonveto[i][jj]->GetXaxis()->SetRangeUser(0, 100);
      title = tgAE_phoEB_nvtx_leptonveto[i][jj]->GetName();
      //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
      
      tgAE_phoEB_nvtx_MET[i][jj]->Draw("APL");
      tgAE_phoEB_nvtx_MET[i][jj]->GetXaxis()->SetRangeUser(0, 100);
      title = tgAE_phoEB_nvtx_MET[i][jj]->GetName();
      //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));

      tgAE_phoEB_nvtx_dphoMETPhi[i][jj]->Draw("APL");
      tgAE_phoEB_nvtx_dphoMETPhi[i][jj]->GetXaxis()->SetRangeUser(0, 100);
      title = tgAE_phoEB_nvtx_dphoMETPhi[i][jj]->GetName(); 
      //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
      
      tgAE_phoEB_nvtx_djetMETPhi[i][jj]->Draw("APL");
      tgAE_phoEB_nvtx_djetMETPhi[i][jj]->GetXaxis()->SetRangeUser(0, 100);
      title = tgAE_phoEB_nvtx_djetMETPhi[i][jj]->GetName();
      //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
      
      tgAE_phoEB_nvtx_jetveto[i][jj]->Draw("APL");
      tgAE_phoEB_nvtx_jetveto[i][jj]->GetXaxis()->SetRangeUser(0, 100);
      title = tgAE_phoEB_nvtx_jetveto[i][jj]->GetName();
      //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    }
  }

  TMultiGraph *mg[nfile][nhisto];
  TLegend *lmg = new TLegend(0.18,0.60, 0.28, 0.88);
  lmg->SetTextSize(0.03);

  TF1 *fitresult = new TF1("fitresult", "pol0", 0, 100);
  Double_t yinter[15][nfile][nhisto], err[15][nfile][nhisto];


  writeExtraText = true;
  Int_t period;
  if(year==2016){
    period = 1;
  }
  else if(year==2017){
    period = 2;
  }
  else if(year==2018){
    period = 3;
  }

  for(Int_t i=0; i<nfile; i++){
    for(Int_t jj=0; jj<nhisto; jj++){
      mg[i][jj] = new TMultiGraph();

      mg[i][jj]->Add(tgAE_phoEB_nvtx_M[i][jj]);
      mg[i][jj]->Add(tgAE_phoEB_nvtx_leptonveto[i][jj]);
      mg[i][jj]->Add(tgAE_phoEB_nvtx_MET[i][jj]);
      mg[i][jj]->Add(tgAE_phoEB_nvtx_dphoMETPhi[i][jj]);
      mg[i][jj]->Add(tgAE_phoEB_nvtx_djetMETPhi[i][jj]);
      mg[i][jj]->Add(tgAE_phoEB_nvtx_jetveto[i][jj]);

      mg[i][jj]->GetXaxis()->SetTitle("Nvtx");
      mg[i][jj]->GetYaxis()->SetTitle("Efficiency");
      mg[i][jj]->GetXaxis()->SetLimits(-1, 100);
      mg[i][jj]->SetMaximum(1.05);
      mg[i][jj]->SetMinimum(0);
      //if(i==0) mg[i][jj]->SetMaximum(0.055);
      mg[i][jj]->SetTitle(Form("cutflow_nvtx_%i_%i", i, jj));
      mg[i][jj]->Draw("APE");

      lmg->Clear();
      lmg->AddEntry(tgAE_phoEB_nvtx_M[i][jj], "Medium ID", "PE");
      lmg->AddEntry(tgAE_phoEB_nvtx_leptonveto[i][jj], "leptonveto", "PE");
      if(jj==0)lmg->AddEntry(tgAE_phoEB_nvtx_MET[i][jj], "MET > 100GeV", "PE");
      if(jj==1)lmg->AddEntry(tgAE_phoEB_nvtx_MET[i][jj], "MET > 80GeV", "PE");
      lmg->AddEntry(tgAE_phoEB_nvtx_dphoMETPhi[i][jj], "#Delta#phi(#gamma, MET) > 1.2", "PE");
      lmg->AddEntry(tgAE_phoEB_nvtx_djetMETPhi[i][jj], "#Delta#phi(jet, MET) > 0.5", "PE");
      if(jj==0)lmg->AddEntry(tgAE_phoEB_nvtx_jetveto[i][jj], "jet SM", "PE");
      else if(jj==1)lmg->AddEntry(tgAE_phoEB_nvtx_jetveto[i][jj], "jet VBS", "PE");
      lmg->Draw("SAME");
      title = mg[i][jj]->GetTitle();
      CMS_lumi(c1, period, 0);
      c1->Update();
      c1->RedrawAxis();
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));

      tgAE_phoEB_nvtx_M[i][jj]->Fit(fitresult,"", "", 20, 80);
      yinter[0][i][jj] = fitresult->GetParameter(0);
      err[0][i][jj] = fitresult->GetParError(0);
    
      tgAE_phoEB_nvtx_leptonveto[i][jj]->Fit(fitresult, "", "", 20, 80);
      yinter[1][i][jj] = fitresult->GetParameter(0);
      err[1][i][jj] = fitresult->GetParError(0);
            
      tgAE_phoEB_nvtx_MET[i][jj]->Fit(fitresult,"", "", 20, 80);
      yinter[2][i][jj] = fitresult->GetParameter(0);
      err[2][i][jj] = fitresult->GetParError(0);

      tgAE_phoEB_nvtx_dphoMETPhi[i][jj]->Fit(fitresult,"", "", 20, 80);
      yinter[3][i][jj] = fitresult->GetParameter(0);
      err[3][i][jj] = fitresult->GetParError(0);
        
      tgAE_phoEB_nvtx_djetMETPhi[i][jj]->Fit(fitresult, "", "", 20, 80);
      yinter[4][i][jj] = fitresult->GetParameter(0);
      err[4][i][jj] = fitresult->GetParError(0);
    
      tgAE_phoEB_nvtx_jetveto[i][jj]->Fit(fitresult, "", "", 20, 80);
      yinter[5][i][jj] = fitresult->GetParameter(0);
      err[5][i][jj] = fitresult->GetParError(0);
   
    }
  }

  Float_t lumiscale[20] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.155635, 0.393159, 0.54324, 0.0081035};//[GJet, WJet, ZJet, DYJet, monoboson, diboson, triboson, Wg, Zg, ttJET, ZnngVBS]
  
   ftext.open("eff_nvtx.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  for(Int_t i=0; i<nfile; i++){
    ftext << "file " << i << endl;
    for(Int_t jj=0; jj<nhisto; jj++){
      for(Int_t j=0; j<6; jj++){
	ftext << j << ") nvtx cut eff " << (integral[j+1][i][jj]*lumiscale[i])/(integral[0][i][jj]*lumiscale[i]) << " error " << err[j][i][jj] << endl;
      }
    }
  }

  ftext.close();
  
}
