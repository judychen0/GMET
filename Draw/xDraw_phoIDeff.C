#include <iostream>
#include <fstream>
#include "TH1F.h"
using namespace std;

void xDraw_phoIDeff(){
  ofstream ftext;
  
  TString rootname = "./output_ggtree.root";
  TFile *fopen = new TFile(rootname);
  TCanvas *c1 = new TCanvas("c1");
  TTree *t =(TTree*)fopen->Get("t");

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = "./graph";

  TH1F *H_realphoEB_pt_var = (TH1F*)fopen->Get("h_realphoEB_pt_var");
  //TH1F *H_realphoEB_pt_L_var = (TH1F*)fopen->Get("h_realphoEB_pt_L_var");
  TH1F *H_realphoEB_pt_M_var = (TH1F*)fopen->Get("h_realphoEB_pt_M_var");
  //TH1F *H_realphoEB_pt_T_var = (TH1F*)fopen->Get("h_realphoEB_pt_T_var");

  //TGraphAsymmErrors *tgrs_L = new TGraphAsymmErrors();
  //tgrs_L->Divide(H_realphoEB_pt_L_var, H_realphoEB_pt_var);
  //tgrs_L->SetNameTitle("H_phocuteff_L", "pho Loose cut eff");
  //tgrs_L->SetMarkerStyle(8);
  //tgrs_L->SetMarkerColor(8);
  //tgrs_L->SetMarkerSize(8);
  //tgrs_L->Draw("AP");
  //title = tgrs_L->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TGraphAsymmErrors *tgrs_M = new TGraphAsymmErrors();
  tgrs_M->Divide(H_realphoEB_pt_M_var, H_realphoEB_pt_var);
  tgrs_M->SetNameTitle("H_phocuteff_M", "pho Medium cut eff");
  tgrs_M->SetMarkerStyle(8);
  tgrs_M->SetMarkerColor(8);
  //tgrs_M->SetMarkerSize(8);
  tgrs_M->Draw("AP");
  title = tgrs_M->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  //TGraphAsymmErrors *tgrs_T = new TGraphAsymmErrors();
  //tgrs_T->Divide(H_realphoEB_pt_T_var, H_realphoEB_pt_var);
  //tgrs_T->SetNameTitle("H_phocuteff_T", "pho Tight cut eff");
  //tgrs_T->SetMarkerStyle(8);
  //tgrs_T->SetMarkerColor(8);
  //tgrs_T->SetMarkerSize(8);
  //tgrs_T->Draw("AP");
  //title = tgrs_T->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  //get L/M/T cut eff multi-histo

  Int_t turnpoint = 8;//point count from 0; EWK 2; Znunug 8
  Double_t x, y, ex, ey; 
  
  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  //tgrs_L->GetPoint(turnpoint, x, y);
  //ey = tgrs_L->GetErrorY(turnpoint);
  //ftext << "phoID L turn up point: " << x << " efficiency: " << y << " error: " << ey << endl;
  
  tgrs_M->GetPoint(turnpoint, x, y);
  ey = tgrs_M->GetErrorY(turnpoint);
  ftext << "phoID M turn up point: " << x << " efficiency: " << y << " error: " << ey << endl;

  //tgrs_T->GetPoint(turnpoint, x, y);
  //ey = tgrs_T->GetErrorY(turnpoint);
  //ftext << "phoID T turn up point: " << x << " efficiency: " << y << " error: " << ey << endl;
  
  ftext.close();
  
  
  TMultiGraph *mg = new TMultiGraph();
  
  //tgrs_L->SetMarkerColor(kGreen+1);
  //tgrs_L->Draw("AP");
  tgrs_M->SetMarkerColor(kAzure+1);
  //tgrs_M->Draw("AP SAME");
  //tgrs_T->SetMarkerColor(kYellow);
  //tgrs_T->Draw("AP SAME");
  //mg->Add(tgrs_L);
  mg->Add(tgrs_M);
  //mg->Add(tgrs_T);
  mg->GetXaxis()->SetTitle("phoEt#[]{GeV}");
  mg->GetYaxis()->SetTitle("Efficiency");
  mg->GetXaxis()->SetLimits(-1, 500);
  mg->SetMaximum(1.05);
  mg->Draw("AP");

  TLegend *legend=new TLegend(0.15,0.75, 0.25, 0.88);
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  //legend->AddEntry(tgrs_L, "Loose", "P");
  legend->AddEntry(tgrs_M, "Medium", "P");
  //legend->AddEntry(tgrs_T, "Tight", "P");
  legend->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf",saveto, "h_realphoEB_cuteff"));

  TH1F *H_realphoEB_eta_var = (TH1F*)fopen->Get("h_realphoEB_eta_var");
  //TH1F *H_realphoEB_eta_L_var = (TH1F*)fopen->Get("h_realphoEB_eta_L_var");
  TH1F *H_realphoEB_eta_M_var = (TH1F*)fopen->Get("h_realphoEB_eta_M_var");
  //TH1F *H_realphoEB_eta_T_var = (TH1F*)fopen->Get("h_realphoEB_eta_T_var");

  //TGraphAsymmErrors *tgrs_eta_L = new TGraphAsymmErrors();
  //tgrs_eta_L->Divide(H_realphoEB_eta_L_var, H_realphoEB_eta_var);
  //tgrs_eta_L->SetNameTitle("H_phocuteff_eta_L", "pho Loose cut eff");
  //tgrs_eta_L->SetMarkerStyle(8);
  //tgrs_eta_L->SetMarkerColor(8);
  //tgrs_eta_L->SetMarkerSize(8);
  //tgrs_eta_L->Draw("AP");
  //title = tgrs_eta_L->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TGraphAsymmErrors *tgrs_eta_M = new TGraphAsymmErrors();
  tgrs_eta_M->Divide(H_realphoEB_eta_M_var, H_realphoEB_eta_var);
  tgrs_eta_M->SetNameTitle("H_phocuteff_eta_M", "pho Medium cut eff");
  tgrs_eta_M->SetMarkerStyle(8);
  tgrs_eta_M->SetMarkerColor(8);
  //tgrs_eta_M->SetMarkerSize(8);
  tgrs_eta_M->Draw("AP");
  title = tgrs_eta_M->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  //TGraphAsymmErrors *tgrs_eta_T = new TGraphAsymmErrors();
  //tgrs_eta_T->Divide(H_realphoEB_eta_T_var, H_realphoEB_eta_var);
  //tgrs_eta_T->SetNameTitle("H_phocuteff_eta_T", "pho Tight cut eff");
  //tgrs_eta_T->SetMarkerStyle(8);
  //tgrs_eta_T->SetMarkerColor(8);
  //tgrs_eta_T->SetMarkerSize(8);
  //tgrs_eta_T->Draw("AP");
  //title = tgrs_eta_T->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  //get L/M/T cut eff multi-histo
  
  TMultiGraph *mg1 = new TMultiGraph();
  
  //tgrs_eta_L->SetMarkerColor(kGreen+1);
  //tgrs_eta_L->Draw("AP");
  tgrs_eta_M->SetMarkerColor(kAzure+1);
  //tgrs_eta_M->Draw("AP SAME");
  //tgrs_eta_T->SetMarkerColor(kYellow);
  //tgrs_eta_T->Draw("AP SAME");
  //mg1->Add(tgrs_eta_L);
  mg1->Add(tgrs_eta_M);
  //mg1->Add(tgrs_eta_T);
  mg1->GetXaxis()->SetTitle("phoEta#[]{GeV}");
  mg1->GetYaxis()->SetTitle("Efficiency");
  mg1->GetXaxis()->SetLimits(-3, 3);
  mg1->SetMaximum(1.05);
  mg1->SetMinimum(0.);
  mg1->Draw("AP");

  TLegend *legend1=new TLegend(0.15,0.75, 0.25, 0.88);
  legend1->SetBorderSize(0);
  legend1->SetFillColor(0);
  //legend1->AddEntry(tgrs_eta_L, "Loose", "P");
  legend1->AddEntry(tgrs_eta_M, "Medium", "P");
  //legend1->AddEntry(tgrs_eta_T, "Tight", "P");
  legend1->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf",saveto, "h_realphoEB_cuteff_eta"));
}
