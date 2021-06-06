#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
#define nfile 4

void xDrawStack_Nm1(){
  TString rootname[5] = {
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_GJets_sigMC_HT_13TeV-madgraphMLM-pythia8/output_merge.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_WGToLNuG_01J_5f_PtG_130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/210602_180734/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZLLGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph/210602_184359/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph-pythia8/210602_163537/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]

  Float_t gjetXsec[10] = {9319, 9155, 2323, 2314, 278.5, 271.8, 93.85, 94.7};
  Float_t lumi16 = 36.33;
  Float_t Xsec = 2323.0;

  Float_t xsec[10] = {2323, 1.008, 0.148, 0.17};
  Float_t filter[10] = {1., 0.7772, 1., 1.};

  string hexcolor[8] = {"#7FB5F5","#EFE2C9","#1C2A60","#FA6D5F","#1C6040","#DCF563", "#2F81A3","#91A81E"};
  TColor *color[8];
  Int_t cnum[8];
   
  for(Int_t j=0; j<8; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  }

  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  TTree *t;

  Int_t entries;
  Float_t outentries = 0.;
  Float_t scale = 0.;

  TH1F *H_Events[10];
  
  TH1F *H_MET_Nm1[nfile];
  TH1F *H_njet_Nm1[nfile];
  TH1F *H_dphoMETPhi_Nm1[nfile];
  TH1F *H_jetpt_Nm1[nfile];
  TH1F *H_djetMETPhi_Nm1[nfile];

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    t =(TTree*)fopen->Get("t");

    if(i<nfile-1){
      H_Events[i] = (TH1F*)fopen->Get("hEvents");
      entries = H_Events[i]->GetBinContent(1);
      
      outentries = xsec[i]*1000*lumi16*filter[i];
      
      scale = outentries/entries;
      cout << "print " << entries << " " << outentries << " " << scale<< endl;
      //if(i==4) scale*=100000;
    }

    if(i==4) scale = 2.2768;
    
    H_MET_Nm1[i] = (TH1F*)fopen->Get("h_MET_Nm1");
    H_dphoMETPhi_Nm1[i] = (TH1F*)fopen->Get("h_dphoMETPhi_Nm1");
    H_njet_Nm1[i] = (TH1F*)fopen->Get("h_njet_Nm1");
    H_jetpt_Nm1[i] = (TH1F*)fopen->Get("h_jetpt_Nm1");
    H_djetMETPhi_Nm1[i] = (TH1F*)fopen->Get("h_djetMETPhi_Nm1");

    //if(i<4){
      H_MET_Nm1[i]->Scale(scale);
      H_dphoMETPhi_Nm1[i]->Scale(scale);
      H_njet_Nm1[i]->Scale(scale);
      H_jetpt_Nm1[i]->Scale(scale);
      H_djetMETPhi_Nm1[i]->Scale(scale);
      // }

    H_MET_Nm1[i]->SetFillColor(cnum[i]);
    H_dphoMETPhi_Nm1[i]->SetFillColor(cnum[i]);
    H_njet_Nm1[i]->SetFillColor(cnum[i]);
    H_jetpt_Nm1[i]->SetFillColor(cnum[i]);
    H_djetMETPhi_Nm1[i]->SetFillColor(cnum[i]);

    H_MET_Nm1[i]->SetLineColor(cnum[i]);
    H_dphoMETPhi_Nm1[i]->SetLineColor(cnum[i]);
    H_njet_Nm1[i]->SetLineColor(cnum[i]);
    H_jetpt_Nm1[i]->SetLineColor(cnum[i]);
    H_djetMETPhi_Nm1[i]->SetLineColor(cnum[i]);

    if(i==4){
      H_MET_Nm1[i]->SetLineWidth(2);
      H_dphoMETPhi_Nm1[i]->SetLineWidth(2);
      H_njet_Nm1[i]->SetLineWidth(2);
      H_jetpt_Nm1[i]->SetLineWidth(2);
      H_djetMETPhi_Nm1[i]->SetLineWidth(2);

      H_MET_Nm1[i]->SetMarkerStyle(8);
      H_dphoMETPhi_Nm1[i]->SetMarkerStyle(8);
      H_njet_Nm1[i]->SetMarkerStyle(8);
      H_jetpt_Nm1[i]->SetMarkerStyle(8);
      H_djetMETPhi_Nm1[i]->SetMarkerStyle(8);

      H_MET_Nm1[i]->SetMarkerSize(0.8);
      H_dphoMETPhi_Nm1[i]->SetMarkerSize(0.8);
      H_njet_Nm1[i]->SetMarkerSize(0.8);
      H_jetpt_Nm1[i]->SetMarkerSize(0.8);
      H_djetMETPhi_Nm1[i]->SetMarkerSize(0.8);
    }
  }

  THStack *HS_MET_Nm1 = new THStack("HS_MET_Nm1", "HS_MET_Nm1");
  THStack *HS_dphoMETPhi_Nm1 = new THStack("HS_dphoMETPhi_Nm1", "HS_dphoMETPhi_Nm1");
  THStack *HS_njet_Nm1 = new THStack("HS_njet_Nm1", "HS_njet_Nm1");
  THStack *HS_jetpt_Nm1 = new THStack("HS_jetpt_Nm1", "HS_jetpt_Nm1");
  THStack *HS_djetMETPhi_Nm1 = new THStack("HS_djetMETPhi_Nm1", "HS_djetMETPhi_Nm1");

  for(Int_t i=0; i<nfile; i++){
    HS_MET_Nm1->Add(H_MET_Nm1[i]);
    HS_njet_Nm1->Add(H_njet_Nm1[i]);
    HS_jetpt_Nm1->Add(H_jetpt_Nm1[i]);
    
    c1->SetLogy();
    c1->Update();
    
    HS_dphoMETPhi_Nm1->Add(H_dphoMETPhi_Nm1[i]);
    HS_djetMETPhi_Nm1->Add(H_djetMETPhi_Nm1[i]);
    
  }

  TLegend *lhs = new TLegend(0.65,0.72, 0.75, 0.88);
  lhs->SetBorderSize(0);
  lhs->SetFillStyle(0);

  TRatioPlot *rp;
  TH1 *h;

  HS_MET_Nm1->Draw("HIST");
  //H_MET_Nm1[nfile-1]->Draw("AP SAME");
  HS_MET_Nm1->GetXaxis()->SetTitle("MET");
  HS_MET_Nm1->GetYaxis()->SetTitle("Events");
  lhs->Clear();
  lhs->AddEntry(H_MET_Nm1[0], "#gamma+jet", "F");
  lhs->AddEntry(H_MET_Nm1[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_MET_Nm1[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_MET_Nm1[3], "Zg#rightarrow#nu#nug", "F");
  //lhs->AddEntry(H_MET_Nm1[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_MET_Nm1->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  HS_dphoMETPhi_Nm1->Draw("HIST");
  //H_dphoMETPhi_Nm1[nfile-1]->Draw("SAME");
  HS_dphoMETPhi_Nm1->SetMinimum(0.001);
  HS_dphoMETPhi_Nm1->GetXaxis()->SetTitle("#Delta#phi(#gamma,MET)");
  HS_dphoMETPhi_Nm1->GetYaxis()->SetTitle("Events");
  lhs->Clear();
  lhs->AddEntry(H_dphoMETPhi_Nm1[0], "#gamma+jet", "F");
  lhs->AddEntry(H_dphoMETPhi_Nm1[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_dphoMETPhi_Nm1[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_dphoMETPhi_Nm1[3], "Zg#rightarrow#nu#nug", "F");
  //lhs->AddEntry(H_dphoMETPhi_Nm1[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_dphoMETPhi_Nm1->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  HS_njet_Nm1->Draw("HIST");
  //H_njet_Nm1[nfile-1]->Draw("AP SAME");
  HS_njet_Nm1->GetXaxis()->SetTitle("njet");
  HS_njet_Nm1->GetYaxis()->SetTitle("Events");
  lhs->Clear();
  lhs->AddEntry(H_njet_Nm1[0], "#gamma+jet", "F");
  lhs->AddEntry(H_njet_Nm1[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_njet_Nm1[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_njet_Nm1[3], "Zg#rightarrow#nu#nug", "F");
  //lhs->AddEntry(H_njet_Nm1[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_njet_Nm1->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  HS_jetpt_Nm1->Draw("HIST");
  //H_jetpt_Nm1[nfile-1]->Draw("AP SAME");
  HS_jetpt_Nm1->GetXaxis()->SetTitle("P_{T}^{jet}");
  HS_jetpt_Nm1->GetYaxis()->SetTitle("Events");
  lhs->Clear();
  lhs->AddEntry(H_jetpt_Nm1[0], "#gamma+jet", "F");
  lhs->AddEntry(H_jetpt_Nm1[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_jetpt_Nm1[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_jetpt_Nm1[3], "Zg#rightarrow#nu#nug", "F");
  //lhs->AddEntry(H_jetpt_Nm1[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_jetpt_Nm1->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  HS_djetMETPhi_Nm1->Draw("HIST");
  //H_djetMETPhi_Nm1[nfile-1]->Draw("AP SAME");
  HS_djetMETPhi_Nm1->GetXaxis()->SetTitle("#Delta#phi(jet, MET)");
  HS_djetMETPhi_Nm1->GetYaxis()->SetTitle("Events");
  HS_djetMETPhi_Nm1->SetMinimum(0.5);
  lhs->Clear();
  lhs->AddEntry(H_djetMETPhi_Nm1[0], "#gamma+jet", "F");
  lhs->AddEntry(H_djetMETPhi_Nm1[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_djetMETPhi_Nm1[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_djetMETPhi_Nm1[3], "Zg#rightarrow#nu#nug", "F");
  //lhs->AddEntry(H_djetMETPhi_Nm1[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_djetMETPhi_Nm1->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
}
