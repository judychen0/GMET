#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
#define nfile 2
#define nhisto 22

void xDrawChCuteff(){
  ofstream ftext;
  TString rootname[5] = {
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph-pythia8/210614_100645/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]

  string hexcolor[8] = {"#7FB5F5","#2F81A3","#1C2A60","#FA6D5F","#91A81E","#DCF563", "#1C6040","#EFE2C9"};
  TColor *color[8];
  Int_t cnum[8];
  for(Int_t j=0; j<8; j++){
      color[j] = new TColor();
      Int_t n = hexcolor[j].length();
      char chararray[n+1];
      strcpy(chararray, hexcolor[j].c_str());
      cnum[j] = color[j]->GetColor(chararray);
    }

  TH1F *H_phoEB_pt[nfile];
  TH1F *H_phoEB_pt_chIsocut[nfile][nhisto];
  TH1F *H_phoEB_pt_chworstcut[nfile][nhisto];
  TH1F *H_phoEB_pt_chworstcut_newEA[nfile][nhisto];

  TGraphAsymmErrors *tgAE_phoEB_pt_chIsocut[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_chworstcut[nfile][nhisto];
  TGraphAsymmErrors *tgAE_phoEB_pt_chworstcut_newEA[nfile][nhisto];
  
  TGraph *tg_phoEB_pt_chIsocut[nfile];
  TGraph *tg_phoEB_pt_chworstcut[nfile];
  TGraph *tg_phoEB_pt_chworstcut_newEA[nfile];
  
  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  TTree *t;

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  Int_t turnpoint = 12;//pt200 cut
  Double_t x[nfile][nhisto], y[nfile][nhisto], ey[nfile][nhisto];
  Float_t chbin[30] = {0.0, 0.1, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.5, 4.0, 5.0, 6.0, 7.0, 8.0, 10, 12, 15};//25 bins
  TH1F *H_phoEB_chIso[nfile];
  TH1F *H_phoEB_chWorst[nfile];
  TH1F *H_phoEB_chWorst_newEA[nfile];
  
  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    t = (TTree*)fopen->Get("t");
    
    H_phoEB_pt[i] = (TH1F*)fopen->Get("h_phoEB_pt");
    H_phoEB_chIso[i] = new TH1F(Form("H_phoEB_chIso_%i", i), Form("H_phoEB_chIso_%i", i), 25, chbin);
    H_phoEB_chWorst[i] = new TH1F(Form("H_phoEB_chWorst_%i", i), Form("H_phoEB_chWorst_%i", i), 25, chbin);
    H_phoEB_chWorst_newEA[i] = new TH1F(Form("H_phoEB_chWorst_newEA_%i", i), Form("H_phoEB_chWorst_newEA_%i", i), 25, chbin);
    
    t->Draw(Form("chIso_rc>>H_phoEB_chIso_%i", i));
    t->Draw(Form("chWorst_rc>>H_phoEB_chWorst_%i", i));
    t->Draw(Form("chWorst_rcnewEA>>H_phoEB_chWorst_newEA_%i", i));
    
    Int_t nbins = 25;
    Double_t chIso_count = H_phoEB_chIso[i]->Integral(1, nbins);
    Double_t chWorst_count = H_phoEB_chWorst[i]->Integral(1, nbins);
    Double_t chWorst_newEA_count = H_phoEB_chWorst_newEA[i]->Integral(1, nbins);
    
    tg_phoEB_pt_chIsocut[i] = new TGraph();
    tg_phoEB_pt_chworstcut[i] = new TGraph();
    tg_phoEB_pt_chworstcut_newEA[i] = new TGraph();
    
    for(Int_t j=0; j<nhisto; j++){
      //if(j==0) continue;
      H_phoEB_pt_chIsocut[i][j] = (TH1F*)fopen->Get(Form("h_phoEB_pt_chIsocut/h_phoEB_pt_chIsocut_%i", j));
      H_phoEB_pt_chworstcut[i][j] = (TH1F*)fopen->Get(Form("h_phoEB_pt_chworstcut/h_phoEB_pt_chworst_%i", j));
      H_phoEB_pt_chworstcut_newEA[i][j] = (TH1F*)fopen->Get(Form("h_phoEB_pt_chworstcut_newEA/h_phoEB_pt_chworstcut_newEA_%i", j));

      //if(j==19) cout<< "run!!" << endl;
      tgAE_phoEB_pt_chIsocut[i][j] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_chworstcut[i][j] = new TGraphAsymmErrors();
      tgAE_phoEB_pt_chworstcut_newEA[i][j] = new TGraphAsymmErrors();

      tgAE_phoEB_pt_chIsocut[i][j]->Divide(H_phoEB_pt_chIsocut[i][j], H_phoEB_pt[i]);
      tgAE_phoEB_pt_chworstcut[i][j]->Divide(H_phoEB_pt_chworstcut[i][j], H_phoEB_pt[i]);
      tgAE_phoEB_pt_chworstcut_newEA[i][j]->Divide(H_phoEB_pt_chworstcut_newEA[i][j], H_phoEB_pt[i]);
      
      tgAE_phoEB_pt_chIsocut[i][j]->SetMarkerStyle(8);
      tgAE_phoEB_pt_chworstcut[i][j]->SetMarkerStyle(8);
      tgAE_phoEB_pt_chworstcut_newEA[i][j]->SetMarkerStyle(8);
      
      //tgAE_phoEB_pt_chIsocut[i][j]->Draw("AP");
      //tgAE_phoEB_pt_chIsocut[i][j]->GetPoint(turnpoint, x[i][j], y[i][j]);
      //ey[i][j] = tgAE_phoEB_pt_chIsocut[i][j]->GetErrorY(turnpoint);
      y[i][j] = (H_phoEB_chIso[i]->Integral(1, j+1))/chIso_count;
      tg_phoEB_pt_chIsocut[i]->SetPoint(j, chbin[j+1], y[i][j]);
      
      //tgAE_phoEB_pt_chworstcut[i][j]->Draw("AP");
      //tgAE_phoEB_pt_chworstcut[i][j]->GetPoint(turnpoint, x[i][j], y[i][j]);
      //ey[i][j] = tgAE_phoEB_pt_chworstcut[i][j]->GetErrorY(turnpoint);
      y[i][j] = (H_phoEB_chWorst[i]->Integral(1, j+1))/chWorst_count;
      tg_phoEB_pt_chworstcut[i]->SetPoint(j, chbin[j+1], y[i][j]);

      y[i][j] = (H_phoEB_chWorst_newEA[i]->Integral(1, j+1))/chWorst_newEA_count;
      tg_phoEB_pt_chworstcut_newEA[i]->SetPoint(j, chbin[j+1], y[i][j]);
    }
    
    tg_phoEB_pt_chIsocut[i]->SetMarkerStyle(8);
    tg_phoEB_pt_chworstcut[i]->SetMarkerStyle(8);
    tg_phoEB_pt_chworstcut_newEA[i]->SetMarkerStyle(8);
    
    tg_phoEB_pt_chIsocut[i]->SetMarkerColor(cnum[0]);
    tg_phoEB_pt_chworstcut[i]->SetMarkerColor(cnum[2]);
    tg_phoEB_pt_chworstcut_newEA[i]->SetMarkerColor(cnum[3]);
    
    tg_phoEB_pt_chIsocut[i]->Draw("AP");
    tg_phoEB_pt_chworstcut[i]->Draw("AP");
    tg_phoEB_pt_chworstcut_newEA[i]->Draw("AP");
  }

  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  for(Int_t i=0; i<nfile; i++){
    ftext << "file " << i << endl;
    for(Int_t j=0; j<nhisto; j++){
      ftext << j << ") cut efficiency: " << y[i][j] << endl;
    }  
  }
  
  ftext.close();
  

  TMultiGraph *mg_phoEB_pt_chIsocut = new TMultiGraph();
  TMultiGraph *mg_phoEB_pt_chworstcut = new TMultiGraph();
  TMultiGraph *mg_phoEB_pt_chworstcut_newEA = new TMultiGraph();
  
  TMultiGraph *mg_phoEB_pt_chcut[nfile];
  TMultiGraph *mg_phoEB_pt_chwcut[nfile];
  TLegend *lmg_phoEB_pt = new TLegend(0.65,0.63, 0.75, 0.75);
  lmg_phoEB_pt->SetTextSize(0.03);
  lmg_phoEB_pt->SetFillStyle(0);
  
  for(Int_t i=0; i<nfile; i++){
    mg_phoEB_pt_chcut[i] = new TMultiGraph();
    mg_phoEB_pt_chcut[i]->Add(tg_phoEB_pt_chIsocut[i]);
    mg_phoEB_pt_chcut[i]->Add(tg_phoEB_pt_chworstcut[i]);
    mg_phoEB_pt_chcut[i]->GetXaxis()->SetTitle("cut value");
    mg_phoEB_pt_chcut[i]->GetYaxis()->SetTitle("Efficiency");
    mg_phoEB_pt_chcut[i]->GetXaxis()->SetLimits(-0.5, 10.5);
    
    mg_phoEB_pt_chcut[i]->SetTitle(Form("mg_phoEB_pt_chcut%i", i));
    mg_phoEB_pt_chcut[i]->Draw("AP");
    mg_phoEB_pt_chcut[i]->SetMaximum(1.05);
    mg_phoEB_pt_chcut[i]->SetMinimum(0.0);
    lmg_phoEB_pt->Clear();
    lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chIsocut[i], "ChIso", "PE");
    lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chworstcut[i], "WorstChIso", "PE");
    lmg_phoEB_pt->Draw("SAME");
    title = mg_phoEB_pt_chcut[i]->GetTitle();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    mg_phoEB_pt_chwcut[i] = new TMultiGraph();
    mg_phoEB_pt_chwcut[i]->Add(tg_phoEB_pt_chIsocut[i]);
    mg_phoEB_pt_chwcut[i]->Add(tg_phoEB_pt_chworstcut[i]);
    mg_phoEB_pt_chwcut[i]->Add(tg_phoEB_pt_chworstcut_newEA[i]);
    mg_phoEB_pt_chwcut[i]->GetXaxis()->SetTitle("chworst cut value");
    mg_phoEB_pt_chwcut[i]->GetYaxis()->SetTitle("Efficiency");
    mg_phoEB_pt_chwcut[i]->GetXaxis()->SetLimits(-0.5, 10.5);
    
    mg_phoEB_pt_chwcut[i]->SetTitle(Form("mg_phoEB_pt_chwcut%i", i));
    mg_phoEB_pt_chwcut[i]->Draw("AP");
    mg_phoEB_pt_chwcut[i]->SetMaximum(1.05);
    mg_phoEB_pt_chwcut[i]->SetMinimum(0.0);
    lmg_phoEB_pt->Clear();
    lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chIsocut[i], "ChIso", "PE");
    lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chworstcut[i], "WorstChIso", "PE");
    lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chworstcut_newEA[i], "WorstChIso newEA", "PE");
    lmg_phoEB_pt->Draw("SAME");
    title = mg_phoEB_pt_chwcut[i]->GetTitle();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title)); 
  }

  for(Int_t i=0; i<nfile; i++){
    tg_phoEB_pt_chIsocut[i]->SetMarkerColor(cnum[i]);
    tg_phoEB_pt_chworstcut[i]->SetMarkerColor(cnum[i]);
    tg_phoEB_pt_chworstcut_newEA[i]->SetMarkerColor(cnum[i]);
    
    mg_phoEB_pt_chIsocut->Add(tg_phoEB_pt_chIsocut[i]);
    mg_phoEB_pt_chworstcut->Add(tg_phoEB_pt_chworstcut[i]);
    mg_phoEB_pt_chworstcut_newEA->Add(tg_phoEB_pt_chworstcut_newEA[i]);
  }

  
  mg_phoEB_pt_chIsocut->GetXaxis()->SetTitle("Charge Iso cut");
  mg_phoEB_pt_chworstcut->GetXaxis()->SetTitle("Worst Chrage Iso cut");
  mg_phoEB_pt_chworstcut_newEA->GetXaxis()->SetTitle("Worst Chrage Iso cut (newEA)");  

  mg_phoEB_pt_chIsocut->GetYaxis()->SetTitle("Efficiency");
  mg_phoEB_pt_chworstcut->GetYaxis()->SetTitle("Efficiency");
  mg_phoEB_pt_chworstcut_newEA->GetYaxis()->SetTitle("Efficiency");

  mg_phoEB_pt_chIsocut->GetXaxis()->SetLimits(-0.5, 10.5);
  mg_phoEB_pt_chworstcut->GetXaxis()->SetLimits(-0.5, 10.5);
  mg_phoEB_pt_chworstcut_newEA->GetXaxis()->SetLimits(-0.5, 10.5);

  mg_phoEB_pt_chIsocut->SetMaximum(1.05);
  mg_phoEB_pt_chworstcut->SetMaximum(1.05);
  mg_phoEB_pt_chworstcut_newEA->SetMaximum(1.05);

  mg_phoEB_pt_chIsocut->SetMinimum(0);
  mg_phoEB_pt_chworstcut->SetMinimum(0);
  mg_phoEB_pt_chworstcut_newEA->SetMinimum(0);

  mg_phoEB_pt_chIsocut->SetTitle("mg_phoEB_pt_chIsocut");
  mg_phoEB_pt_chworstcut->SetTitle("mg_phoEB_pt_chworstcut");
  mg_phoEB_pt_chworstcut_newEA->SetTitle("mg_phoEB_pt_chworstcut_newEA");

  lmg_phoEB_pt = new TLegend(0.65,0.60, 0.75, 0.75);
  lmg_phoEB_pt->SetBorderSize(0);
  lmg_phoEB_pt->SetFillStyle(0);
  
  mg_phoEB_pt_chIsocut->Draw("AP");
  lmg_phoEB_pt->Clear();
  //lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chIsocut[0], "#gamma+jet", "PE");
  lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chIsocut[0], "Wg#rightarrowl#nug", "PE");
  lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chIsocut[1], "Zg#rightarrowllg", "PE");
  lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chIsocut[2], "Zg#rightarrow#nu#nug", "PE");
  lmg_phoEB_pt->Draw("SAME");
  title = mg_phoEB_pt_chIsocut->GetTitle();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  mg_phoEB_pt_chworstcut->Draw("AP");
  lmg_phoEB_pt->Clear();
  //lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chworstcut[0], "#gamma+jet", "PE");
  lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chworstcut[0], "Wg#rightarrowl#nug", "PE");
  lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chworstcut[1], "Zg#rightarrowllg", "PE");
  lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chworstcut[2], "Zg#rightarrow#nu#nug", "PE");
  lmg_phoEB_pt->Draw("SAME");
  title = mg_phoEB_pt_chworstcut->GetTitle();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  mg_phoEB_pt_chworstcut_newEA->Draw("AP");
  lmg_phoEB_pt->Clear();
  //lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chworstcut_newEA[0], "#gamma+jet", "PE");
  lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chworstcut_newEA[0], "Wg#rightarrowl#nug", "PE");
  lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chworstcut_newEA[1], "Zg#rightarrowllg", "PE");
  lmg_phoEB_pt->AddEntry(tg_phoEB_pt_chworstcut_newEA[2], "Zg#rightarrow#nu#nug", "PE");
  lmg_phoEB_pt->Draw("SAME");
  title = mg_phoEB_pt_chworstcut_newEA->GetTitle();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  
}
