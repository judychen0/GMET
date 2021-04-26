#include <iostream>
#include <fstream>
#include "TH1F.h"
using namespace std;

void xDrawIso(){
  ofstream ftext;
    
  TString rootname = "./output_ggtree.root";
  TFile *fopen = new TFile(rootname);
  TCanvas *c1 = new TCanvas("c1");
  TTree *t =(TTree*)fopen->Get("t");

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = "./graph";

  //sieie
  TH1F *H_sieieFull5x5 = new TH1F("H_sieieFull5x5", "H_sieieFull5x5", 40, 0.001, 0.021); 
  H_sieieFull5x5->GetYaxis()->SetRangeUser(.01, 800000);//60EWK, 800nng
  t->Draw("sieieFull5x5>>H_sieieFull5x5");
  title = H_sieieFull5x5->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_sieieFull5x5_L = new TH1F("H_sieieFull5x5_L", "H_sieieFull5x5_L", 40, 0.001, 0.021);
  //H_sieieFull5x5_L->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("sieieFull5x5_Lwp>>H_sieieFull5x5_L");
  //title = H_sieieFull5x5_L->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_sieieFull5x5_M = new TH1F("H_sieieFull5x5_M", "H_sieieFull5x5_M", 40, 0.001, 0.021);
  //H_sieieFull5x5_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("sieieFull5x5_Mwp>>H_sieieFull5x5_M");
  title = H_sieieFull5x5_M->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_sieieFull5x5_T = new TH1F("H_sieieFull5x5_T", "H_sieieFull5x5_T", 40, 0.001, 0.021);
  //H_sieieFull5x5_T->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("sieieFull5x5_Twp>>H_sieieFull5x5_T");
  //title = H_sieieFull5x5_T->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //sieie multi histo
  H_sieieFull5x5->SetFillStyle(0);
  H_sieieFull5x5->SetLineColor(kBlack);
  H_sieieFull5x5->GetXaxis()->SetTitle("sIEtaIEta");
  H_sieieFull5x5->Draw();
  
  //H_sieieFull5x5_L->SetFillStyle(0);
  //H_sieieFull5x5_L->SetLineColor(kGreen+1);
  //H_sieieFull5x5_L->Draw("SAME");

  H_sieieFull5x5_M->SetFillStyle(0);
  H_sieieFull5x5_M->SetLineColor(kYellow+1);
  H_sieieFull5x5_M->Draw("SAME");

  //H_sieieFull5x5_T->SetFillStyle(0);
  //H_sieieFull5x5_T->SetLineColor(kAzure+1);
  //H_sieieFull5x5_T->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "sieieFull5x5_rel"));

  //TH1F *h_realphoEB_sieie = (TH1F*)fopen->Get("h_realphoEB_sieie");
  //TH1F *h_realphoEB_sieie_L = (TH1F*)fopen->Get("h_realphoEB_sieie_L");
  //TH1F *h_realphoEB_sieie_M = (TH1F*)fopen->Get("h_realphoEB_sieie_M");
  //TH1F *h_realphoEB_sieie_T = (TH1F*)fopen->Get("h_realphoEB_sieie_T");

  Int_t nbins;
  nbins = H_sieieFull5x5->GetNbinsX();
  Double_t sieie_count = H_sieieFull5x5->Integral(1, nbins);
  //Double_t sieie_Lcount = H_sieieFull5x5_L->Integral(1, nbins);
  Double_t sieie_Mcount = H_sieieFull5x5_M->Integral(1, nbins);
  //Double_t sieie_Tcount = H_sieieFull5x5_T->Integral(1, nbins);
  //Double_t sieie_Leff = sieie_Lcount/sieie_count;
  Double_t sieie_Meff = sieie_Mcount/sieie_count;
  //Double_t sieie_Teff = sieie_Tcount/sieie_count;
  
  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  //ftext << "sieie Leff : " << sieie_Leff << endl;
  ftext << "sieie_Meff : " << sieie_Meff << endl;
  //ftext << "sieie_Teff : " << sieie_Teff << endl;
  ftext.close();

  //sieip
  TH1F *H_sieipFull5x5 = new TH1F("H_sieipFull5x5", "H_sieipFull5x5", 40, 0.001, 0.021); 
  H_sieipFull5x5->GetYaxis()->SetRangeUser(.01, 800000);//60EWK, 800nng
  t->Draw("sieipFull5x5>>H_sieipFull5x5");
  title = H_sieipFull5x5->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_sieipFull5x5_L = new TH1F("H_sieipFull5x5_L", "H_sieipFull5x5_L", 40, 0.001, 0.021);
  //H_sieipFull5x5_L->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("sieipFull5x5_Lwp>>H_sieipFull5x5_L");
  //title = H_sieipFull5x5_L->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_sieipFull5x5_M = new TH1F("H_sieipFull5x5_M", "H_sieipFull5x5_M", 40, 0.001, 0.021);
  //H_sieipFull5x5_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("sieipFull5x5_Mwp>>H_sieipFull5x5_M");
  title = H_sieipFull5x5_M->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_sieipFull5x5_T = new TH1F("H_sieipFull5x5_T", "H_sieipFull5x5_T", 40, 0.001, 0.021);
  //H_sieipFull5x5_T->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("sieipFull5x5_Twp>>H_sieipFull5x5_T");
  //title = H_sieipFull5x5_T->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //sieip multi histo
  H_sieipFull5x5->SetFillStyle(0);
  H_sieipFull5x5->SetLineColor(kBlack);
  H_sieipFull5x5->GetXaxis()->SetTitle("sIEtaIEta");
  H_sieipFull5x5->Draw();
  
  //H_sieipFull5x5_L->SetFillStyle(0);
  //H_sieipFull5x5_L->SetLineColor(kGreen+1);
  //H_sieipFull5x5_L->Draw("SAME");

  H_sieipFull5x5_M->SetFillStyle(0);
  H_sieipFull5x5_M->SetLineColor(kYellow+1);
  H_sieipFull5x5_M->Draw("SAME");

  //H_sieipFull5x5_T->SetFillStyle(0);
  //H_sieipFull5x5_T->SetLineColor(kAzure+1);
  //H_sieipFull5x5_T->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "sieipFull5x5_rel"));

  
  nbins = H_sieipFull5x5->GetNbinsX();
  Double_t sieip_count = H_sieipFull5x5->Integral(1, nbins);
  //Double_t sieip_Lcount = H_sieipFull5x5_L->Integral(1, nbins);
  Double_t sieip_Mcount = H_sieipFull5x5_M->Integral(1, nbins);
  //Double_t sieip_Tcount = H_sieipFull5x5_T->Integral(1, nbins);
  //Double_t sieip_Leff = sieip_Lcount/sieip_count;
  Double_t sieip_Meff = sieip_Mcount/sieip_count;
  //Double_t sieip_Teff = sieip_Tcount/sieip_count;
  
  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  //ftext << "sieip Leff : " << sieip_Leff << endl;
  ftext << "sieip_Meff : " << sieip_Meff << endl;
  //ftext << "sieip_Teff : " << sieip_Teff << endl;
  ftext.close();

  //sipip
  TH1F *H_sipipFull5x5 = new TH1F("H_sipipFull5x5", "H_sipipFull5x5", 40, 0.001, 0.021); 
  H_sipipFull5x5->GetYaxis()->SetRangeUser(.01, 800000);//60EWK, 800nng
  t->Draw("sipipFull5x5>>H_sipipFull5x5");
  title = H_sipipFull5x5->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_sipipFull5x5_L = new TH1F("H_sipipFull5x5_L", "H_sipipFull5x5_L", 40, 0.001, 0.021);
  //H_sipipFull5x5_L->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("sipipFull5x5_Lwp>>H_sipipFull5x5_L");
  //title = H_sipipFull5x5_L->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_sipipFull5x5_M = new TH1F("H_sipipFull5x5_M", "H_sipipFull5x5_M", 40, 0.001, 0.021);
  //H_sipipFull5x5_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("sipipFull5x5_Mwp>>H_sipipFull5x5_M");
  title = H_sipipFull5x5_M->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_sipipFull5x5_T = new TH1F("H_sipipFull5x5_T", "H_sipipFull5x5_T", 40, 0.001, 0.021);
  //H_sipipFull5x5_T->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("sipipFull5x5_Twp>>H_sipipFull5x5_T");
  //title = H_sipipFull5x5_T->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //sipip multi histo
  H_sipipFull5x5->SetFillStyle(0);
  H_sipipFull5x5->SetLineColor(kBlack);
  H_sipipFull5x5->GetXaxis()->SetTitle("sIEtaIEta");
  H_sipipFull5x5->Draw();
  
  //H_sipipFull5x5_L->SetFillStyle(0);
  //H_sipipFull5x5_L->SetLineColor(kGreen+1);
  //H_sipipFull5x5_L->Draw("SAME");

  H_sipipFull5x5_M->SetFillStyle(0);
  H_sipipFull5x5_M->SetLineColor(kYellow+1);
  H_sipipFull5x5_M->Draw("SAME");

  //H_sipipFull5x5_T->SetFillStyle(0);
  //H_sipipFull5x5_T->SetLineColor(kAzure+1);
  //H_sipipFull5x5_T->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "sipipFull5x5_rel"));

  
  nbins = H_sipipFull5x5->GetNbinsX();
  Double_t sipip_count = H_sipipFull5x5->Integral(1, nbins);
  //Double_t sipip_Lcount = H_sipipFull5x5_L->Integral(1, nbins);
  Double_t sipip_Mcount = H_sipipFull5x5_M->Integral(1, nbins);
  //Double_t sipip_Tcount = H_sipipFull5x5_T->Integral(1, nbins);
  //Double_t sipip_Leff = sipip_Lcount/sipip_count;
  Double_t sipip_Meff = sipip_Mcount/sipip_count;
  //Double_t sipip_Teff = sipip_Tcount/sipip_count;
  
  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  //ftext << "sipip Leff : " << sipip_Leff << endl;
  ftext << "sipip_Meff : " << sipip_Meff << endl;
  //ftext << "sipip_Teff : " << sipip_Teff << endl;
  ftext.close();

  //draw TH1F logY histograms
  c1->SetLogy();
  c1->Update();

  //r9
  TH1F *H_r9Full5x5 = new TH1F("H_r9Full5x5", "H_r9Full5x5", 15, 0., 1.5);
  //H_r9Full5x5->GetYaxis()->SetRangeUser(.01, 10000000);
  H_r9Full5x5->GetXaxis()->SetTitle("r9");
  t->Draw("r9Full5x5>>H_r9Full5x5");
  title = H_r9Full5x5->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  TH1F *H_r9Full5x5_M = new TH1F("H_r9Full5x5_M", "H_r9Full5x5_M", 15, 0., 1.5);
  //H_r9Full5x5_M->GetYaxis()->SetRangeUser(.01, 10000000);
  H_r9Full5x5_M->GetXaxis()->SetTitle("r9");
  t->Draw("r9Full5x5_Mwp>>H_r9Full5x5_M");
  title = H_r9Full5x5_M->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  H_r9Full5x5->SetFillStyle(0);
  H_r9Full5x5->SetLineColor(kBlack);
  H_r9Full5x5->GetXaxis()->SetTitle("r9Full5x5");
  H_r9Full5x5->Draw();
  
  //H_r9Full5x5_L->SetFillStyle(0);
  //H_r9Full5x5_L->SetLineColor(kGreen+1);
  //H_r9Full5x5_L->Draw("SAME");

  H_r9Full5x5_M->SetFillStyle(0);
  H_r9Full5x5_M->SetLineColor(kYellow+1);
  H_r9Full5x5_M->Draw("SAME");

  //H_r9Full5x5_T->SetFillStyle(0);
  //H_r9Full5x5_T->SetLineColor(kAzure+1);
  //H_r9Full5x5_T->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "r9Full5x5_rel"));

  nbins = H_r9Full5x5->GetNbinsX();
  Double_t r9Full5x5_count = H_r9Full5x5->Integral(1, nbins);
  //Double_t r9Full5x5_Lcount = H_r9Full5x5_L->Integral(1, nbins);
  Double_t r9Full5x5_Mcount = H_r9Full5x5_M->Integral(1, nbins);
  //Double_t r9Full5x5_Tcount = H_r9Full5x5_T->Integral(1, nbins);
  //Double_t r9Full5x5_Leff = r9Full5x5_Lcount/r9Full5x5_count;
  Double_t r9Full5x5_Meff = r9Full5x5_Mcount/r9Full5x5_count;
  //Double_t r9Full5x5_Teff = r9Full5x5_Tcount/r9Full5x5_count;

  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  //ftext << "r9Full5x5 Leff : " << r9Full5x5_Leff << endl;
  ftext << "r9Full5x5_Meff : " << r9Full5x5_Meff << endl;
  //ftext << "r9Full5x5_Teff : " << r9Full5x5_Teff << endl;
  ftext.close();

  //HoverE
  TH1F *H_HoverE = new TH1F("H_HoverE", "H_HoverE", 10, 0., 1);
  H_HoverE->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("HoverE>>H_HoverE");
  title = H_HoverE->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_HoverE_L = new TH1F("H_HoverE_L", "H_HoverE_L", 10, 0., 1);
  //H_HoverE_L->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("HoverE_Lwp>>H_HoverE_L");
  //title = H_HoverE_L->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_HoverE_M = new TH1F("H_HoverE_M", "H_HoverE_M", 10, 0., 1);
  H_HoverE_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("HoverE_Mwp>>H_HoverE_M");
  title = H_HoverE_M->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_HoverE_T = new TH1F("H_HoverE_T", "H_HoverE_T", 10, 0., 1);
  //H_HoverE_T->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("HoverE_Twp>>H_HoverE_T");
  //title = H_HoverE_T->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  H_HoverE->SetFillStyle(0);
  H_HoverE->SetLineColor(kBlack);
  H_HoverE->GetXaxis()->SetTitle("HoverE");
  H_HoverE->Draw();
  
  //H_HoverE_L->SetFillStyle(0);
  //H_HoverE_L->SetLineColor(kGreen+1);
  //H_HoverE_L->Draw("SAME");

  H_HoverE_M->SetFillStyle(0);
  H_HoverE_M->SetLineColor(kYellow+1);
  H_HoverE_M->Draw("SAME");

  //H_HoverE_T->SetFillStyle(0);
  //H_HoverE_T->SetLineColor(kAzure+1);
  //H_HoverE_T->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "HoverE_rel"));

  nbins = H_HoverE->GetNbinsX();
  Double_t HoverE_count = H_HoverE->Integral(1, nbins);
  //Double_t HoverE_Lcount = H_HoverE_L->Integral(1, nbins);
  Double_t HoverE_Mcount = H_HoverE_M->Integral(1, nbins);
  //Double_t HoverE_Tcount = H_HoverE_T->Integral(1, nbins);
  //Double_t HoverE_Leff = HoverE_Lcount/HoverE_count;
  Double_t HoverE_Meff = HoverE_Mcount/HoverE_count;
  //Double_t HoverE_Teff = HoverE_Tcount/HoverE_count;

  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  //ftext << "HoverE Leff : " << HoverE_Leff << endl;
  ftext << "HoverE_Meff : " << HoverE_Meff << endl;
  //ftext << "HoverE_Teff : " << HoverE_Teff << endl;
  ftext.close();

  //chIso
  TH1F *H_chIso = new TH1F("H_chIso", "H_chIso", 25, 0., 10);
  H_chIso->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("chIso_rc>>H_chIso");
  title = H_chIso->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_chIso_L = new TH1F("H_chIso_L", "H_chIso_L", 25, 0., 10);
  //H_chIso_L->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("chIso_Lwp>>H_chIso_L");
  //title = H_chIso_L->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_chIso_M = new TH1F("H_chIso_M", "H_chIso_M", 25, 0., 10);
  H_chIso_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("chIso_Mwp>>H_chIso_M");
  title = H_chIso_M->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_chIso_T = new TH1F("H_chIso_T", "H_chIso_T", 25, 0., 10);
  //H_chIso_T->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("chIso_Twp>>H_chIso_T");
  //title = H_chIso_T->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  H_chIso->SetFillStyle(0);
  H_chIso->SetLineColor(kBlack);
  H_chIso->GetXaxis()->SetTitle("chIso");
  H_chIso->Draw();
  
  //H_chIso_L->SetFillStyle(0);
  //H_chIso_L->SetLineColor(kGreen+1);
  //H_chIso_L->Draw("SAME");

  H_chIso_M->SetFillStyle(0);
  H_chIso_M->SetLineColor(kYellow+1);
  H_chIso_M->Draw("SAME");

  //H_chIso_T->SetFillStyle(0);
  //H_chIso_T->SetLineColor(kAzure+1);
  //H_chIso_T->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "chIso_rel"));

  nbins = H_chIso->GetNbinsX();
  Double_t chIso_count = H_chIso->Integral(1, nbins);
  //Double_t chIso_Lcount = H_chIso_L->Integral(1, nbins);
  Double_t chIso_Mcount = H_chIso_M->Integral(1, nbins);
  //Double_t chIso_Tcount = H_chIso_T->Integral(1, nbins);
  //Double_t chIso_Leff = chIso_Lcount/chIso_count;
  Double_t chIso_Meff = chIso_Mcount/chIso_count;
  //Double_t chIso_Teff = chIso_Tcount/chIso_count;

  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  //ftext << "chIso Leff : " << chIso_Leff << endl;
  ftext << "chIso_Meff : " << chIso_Meff << endl;
  //ftext << "chIso_Teff : " << chIso_Teff << endl;
  ftext.close();
  
  //phoIso
  TH1F *H_phoIso = new TH1F("H_phoIso", "H_phoIso", 25, 0., 10);
  H_phoIso->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("phoIso_rc>>H_phoIso");
  title = H_phoIso->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_phoIso_L = new TH1F("H_phoIso_L", "H_phoIso_L", 25, 0., 10);
  //H_phoIso_L->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("phoIso_Lwp>>H_phoIso_L");
  //title = H_phoIso_L->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_phoIso_M = new TH1F("H_phoIso_M", "H_phoIso_M", 25, 0., 10);
  H_phoIso_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("phoIso_Mwp>>H_phoIso_M");
  title = H_phoIso_M->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_phoIso_T = new TH1F("H_phoIso_T", "H_phoIso_T", 25, 0., 10);
  //H_phoIso_T->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("phoIso_Twp>>H_phoIso_T");
  //title = H_phoIso_T->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  H_phoIso->SetFillStyle(0);
  H_phoIso->SetLineColor(kBlack);
  H_phoIso->GetXaxis()->SetTitle("phoIso");
  H_phoIso->Draw();
  
  //H_phoIso_L->SetFillStyle(0);
  //H_phoIso_L->SetLineColor(kGreen+1);
  //H_phoIso_L->Draw("SAME");

  H_phoIso_M->SetFillStyle(0);
  H_phoIso_M->SetLineColor(kYellow+1);
  H_phoIso_M->Draw("SAME");

  //H_phoIso_T->SetFillStyle(0);
  //H_phoIso_T->SetLineColor(kAzure+1);
  //H_phoIso_T->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "phoIso_rel"));

  
  nbins = H_phoIso->GetNbinsX();
  Double_t phoIso_count = H_phoIso->Integral(1, nbins);
  //Double_t phoIso_Lcount = H_phoIso_L->Integral(1, nbins);
  Double_t phoIso_Mcount = H_phoIso_M->Integral(1, nbins);
  //Double_t phoIso_Tcount = H_phoIso_T->Integral(1, nbins);
  //Double_t phoIso_Leff = phoIso_Lcount/phoIso_count;
  Double_t phoIso_Meff = phoIso_Mcount/phoIso_count;
  //Double_t phoIso_Teff = phoIso_Tcount/phoIso_count;

  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  //ftext << "phoIso Leff : " << phoIso_Leff << endl;
  ftext << "phoIso_Meff : " << phoIso_Meff << endl;
  //ftext << "phoIso_Teff : " << phoIso_Teff << endl;
  ftext.close();
  
  //nhIso
  TH1F *H_nhIso = new TH1F("H_nhIso", "H_nhIso", 25, 0., 10);
  H_nhIso->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("nhIso_rc>>H_nhIso");
  title = H_nhIso->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_nhIso_L = new TH1F("H_nhIso_L", "H_nhIso_L", 25, 0., 10);
  //H_nhIso_L->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("nhIso_Lwp>>H_nhIso_L");
  //title = H_nhIso_L->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_nhIso_M = new TH1F("H_nhIso_M", "H_nhIso_M", 25, 0., 10);
  H_nhIso_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("nhIso_Mwp>>H_nhIso_M");
  title = H_nhIso_M->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_nhIso_T = new TH1F("H_nhIso_T", "H_nhIso_T", 25, 0., 10);
  //H_nhIso_T->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("nhIso_Twp>>H_nhIso_T");
  //title = H_nhIso_T->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  H_nhIso->SetFillStyle(0);
  H_nhIso->SetLineColor(kBlack);
  H_nhIso->GetXaxis()->SetTitle("nhIso");
  H_nhIso->Draw();
  
  //H_nhIso_L->SetFillStyle(0);
  //H_nhIso_L->SetLineColor(kGreen+1);
  //H_nhIso_L->Draw("SAME");

  H_nhIso_M->SetFillStyle(0);
  H_nhIso_M->SetLineColor(kYellow+1);
  H_nhIso_M->Draw("SAME");

  //H_nhIso_T->SetFillStyle(0);
  //H_nhIso_T->SetLineColor(kAzure+1);
  //H_nhIso_T->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "nhIso_rel"));

  nbins = H_nhIso->GetNbinsX();
  Double_t nhIso_count = H_nhIso->Integral(1, nbins);
  //Double_t nhIso_Lcount = H_nhIso_L->Integral(1, nbins);
  Double_t nhIso_Mcount = H_nhIso_M->Integral(1, nbins);
  //Double_t nhIso_Tcount = H_nhIso_T->Integral(1, nbins);
  //Double_t nhIso_Leff = nhIso_Lcount/nhIso_count;
  Double_t nhIso_Meff = nhIso_Mcount/nhIso_count;
  //Double_t nhIso_Teff = nhIso_Tcount/nhIso_count;

  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  //ftext << "nhIso Leff : " << nhIso_Leff << endl;
  ftext << "nhIso_Meff : " << nhIso_Meff << endl;
  //ftext << "nhIso_Teff : " << nhIso_Teff << endl;
  ftext.close();

  //chWorst
  TH1F *H_chWorst = new TH1F("H_chWorst", "H_chWorst", 25, 0., 10);
  H_chWorst->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("chWorst_rc>>H_chWorst");
  title = H_chWorst->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_chWorst_L = new TH1F("H_chWorst_L", "H_chWorst_L", 25, 0., 10);
  //H_chWorst_L->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("chWorst_Lwp>>H_chWorst_L");
  //title = H_chWorst_L->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_chWorst_M = new TH1F("H_chWorst_M", "H_chWorst_M", 25, 0., 10);
  H_chWorst_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("chWorst_Mwp>>H_chWorst_M");
  title = H_chWorst_M->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_chWorst_T = new TH1F("H_chWorst_T", "H_chWorst_T", 25, 0., 10);
  //H_chWorst_T->GetYaxis()->SetRangeUser(.01, 10000000);
  //t->Draw("chWorst_Twp>>H_chWorst_T");
  //title = H_chWorst_T->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  H_chWorst->SetFillStyle(0);
  H_chWorst->SetLineColor(kBlack);
  H_chWorst->GetXaxis()->SetTitle("chWorst");
  H_chWorst->Draw();
  
  //H_chWorst_L->SetFillStyle(0);
  //H_chWorst_L->SetLineColor(kGreen+1);
  //H_chWorst_L->Draw("SAME");

  H_chWorst_M->SetFillStyle(0);
  H_chWorst_M->SetLineColor(kYellow+1);
  H_chWorst_M->Draw("SAME");

  //H_chWorst_T->SetFillStyle(0);
  //H_chWorst_T->SetLineColor(kAzure+1);
  //H_chWorst_T->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "chWorst_rel"));

  nbins = H_chWorst->GetNbinsX();
  Double_t chWorst_count = H_chWorst->Integral(1, nbins);
  //Double_t chWorst_Lcount = H_chWorst_L->Integral(1, nbins);
  Double_t chWorst_Mcount = H_chWorst_M->Integral(1, nbins);
  //Double_t chWorst_Tcount = H_chWorst_T->Integral(1, nbins);
  //Double_t chWorst_Leff = chWorst_Lcount/chWorst_count;
  Double_t chWorst_Meff = chWorst_Mcount/chWorst_count;
  //Double_t chWorst_Teff = chWorst_Tcount/chWorst_count;

  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  //ftext << "chWorst Leff : " << chWorst_Leff << endl;
  ftext << "chWorst_Meff : " << chWorst_Meff << endl;
  //ftext << "chWorst_Teff : " << chWorst_Teff << endl;
  ftext.close();

  
  
}
