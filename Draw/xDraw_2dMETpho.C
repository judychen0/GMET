#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TColor.h"

using namespace std;

#define nfile 4
#define ncolor 10

void xDraw_2dMETpho(){
  /*
  TString rootname[5] = {
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/output_merge.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_Wg_aMCatNLO/210706_145108/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_Zg_aMCatNLO/210706_154418/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_Znunug_pt130/210706_144715/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]
  */
  TString rootname[5] = {
			 "output_merge.root",
			 "summer16_job_summer16_Wg_aMCatNLO/210706_145108/output_ggtree.root",
			 "summer16_job_summer16_Zg_aMCatNLO/210706_154418/output_ggtree.root",
			 "summer16_job_summer16_Znunug_pt130/210706_144715/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]
  

  string hexcolor[ncolor] = {"#fff176", "#03a9f4","#ff3360", "#67ccc1","#1C2A60", "#DCDB06", "#FA6D5F", "#F0F5E1","#DCF563", "#2F81A3" };
  TColor *color[ncolor];
  Int_t cnum[ncolor];
   
  for(Int_t j=0; j<8; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    //color[j]->SetAlpha(0.4);
    cnum[j] = color[j]->GetColor(chararray);
    cnum[j] = TColor::GetColorTransparent(cnum[j], 0.4);
  }

  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");

  TH2F* H2_MET_phoEt[nfile];
  
  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i], "READ");
    H2_MET_phoEt[i] = (TH2F*)fopen->Get("h2_MET_phoEt");
    //H2_MET_phoEt[i]->SetDirectory(0);
    //fopen->Close();
  }

  Int_t xbin = 50;
  Int_t ybin = 80;
  for(Int_t i=0; i<nfile; i++){
    for(Int_t ii=1; ii<51; ii++){
      for(Int_t jj=1; jj<81; jj++){
	if(H2_MET_phoEt[i]->GetBinContent(ii, jj) > 0) H2_MET_phoEt[i]->SetBinContent(ii, jj, i+0.5);
      }
    }
  }

  //TGraph *tg = new TGraph();
  TF1 *f_ptoverMET = new TF1("f_ptoverMET", "1.014/((1/x) - 0.00475)",120, 800);
  TF1 *f_MET = new TF1("f_MET", "500+((x-140)/0.138)", 10, 800);
  f_ptoverMET->SetLineColor(kAzure-1);
  f_MET->SetLineColor(209);
  f_ptoverMET->SetLineWidth(2);
  f_MET->SetLineWidth(2);

  
  Int_t colors[4];
  for(Int_t i=0; i<nfile; i++){
    colors[i] = cnum[i];
  }
  gStyle->SetPalette(4, colors);
  /*
  for(Int_t i=0; i<nfile; i++){
    H2_MET_phoEt[i]->Draw();
    H2_MET_phoEt[i]->SetLineColor(1);
    H2_MET_phoEt[i]->SetLineWidth(1);
  }
  */
  H2_MET_phoEt[3]->Draw("BOX");
  H2_MET_phoEt[3]->GetXaxis()->SetTitle("MET");
  H2_MET_phoEt[3]->GetYaxis()->SetTitle("phoEt");

  for(Int_t i=0; i<3; i++){
    H2_MET_phoEt[3]->Draw("BOX");
    H2_MET_phoEt[3]->GetXaxis()->SetTitle("MET");
    H2_MET_phoEt[3]->GetYaxis()->SetTitle("phoEt");
    H2_MET_phoEt[i]->Draw("colz SAME");
    f_ptoverMET->Draw("AC SAME");
    f_MET->Draw("AC SAME");
    c1->SaveAs(Form("H2_MET_phoEt_mh_%i.pdf", i));
  }
  
  //H2_MET_phoEt[1]->Draw("colz SAME");
  //H2_MET_phoEt[0]->Draw("colz SAME");
  
  

  
  
}
