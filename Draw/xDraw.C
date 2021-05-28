#include <iostream>
#include <fstream>
#include "TH1F.h"
using namespace std;

void xDraw(){
  //TFile *fopen = new TFile("output_ggtree_mc.root");
  ofstream ftext;

  TString rootname = "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph-pythia8_0000/210527_064105/output_ggtree.root";
  TFile *fopen = new TFile(rootname);
  TCanvas *c1 = new TCanvas("c1");
  TTree *t =(TTree*)fopen->Get("t");
  //Int_t Events = t->GetEntries();

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = "."; 
  //draw TH1F non-logY histograms
  TH1F *H_npho = (TH1F*)fopen->Get("h_npho");
  H_npho->GetXaxis()->SetTitle("photon number");
  H_npho->GetYaxis()->SetTitle("Events");
  H_npho->Draw();
  title = H_npho->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_nmcpho = (TH1F*)fopen->Get("h_nmcpho");
  H_nmcpho->Draw();
  title = H_nmcpho->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  TH1F *H_matchpho = (TH1F*)fopen->Get("h_nmatchpho");
  H_matchpho->Draw("HIST");
  title = H_matchpho->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_nrealphoEB = (TH1F*)fopen->Get("h_nrealphoEB");
  //H_nrealphoEB->Draw();
  //title = H_nrealphoEB->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  //TH1F *H_nrealphoEB_M = (TH1F*)fopen->Get("h_nrealphoEB_M");
  //H_nrealphoEB_M->Draw();
  //title = H_nrealphoEB_M->GetName();
  //c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  Int_t nbins;
  nbins = H_nmcpho->GetNbinsX();
  Double_t mcpho_count = H_nmcpho->Integral(2, nbins);
  nbins = H_matchpho->GetNbinsX();
  Double_t matchpho_count = H_matchpho->Integral(2, nbins);
  Double_t genmatch_eff = (matchpho_count/mcpho_count);
  
  cout << "gen-matched eff : " << genmatch_eff << endl;
  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  ftext <<  "gen-matched eff : " << genmatch_eff << endl;
  ftext.close();

  //nbins = H_nrealphoEB->GetNbinsX();
  //Double_t nrealphoEB_count = H_nrealphoEB->Integral(2, nbins);
  //nbins = H_nrealphoEB_M->GetNbinsX();
  //Double_t nrealphoEBM_count = H_nrealphoEB_M->Integral(2, nbins);
  //Double_t MIDcut_eff = (nrealphoEBM_count/nrealphoEB_count);

  //cout << "M-ID cut eff : " << MIDcut_eff << endl;
  //ftext.open("eff.txt", ios::out | ios::app);
  //if(!ftext){
  //cerr << "Can't open file !" << endl;
  //exit(1);
  //}
  //ftext <<  "M-ID cut eff : " << MIDcut_eff << endl;
  //ftext.close();
  
  /*  
  Double_t ptcut[30] = {22, 30, 36, 50, 75, 90, 120, 170, 175, 180, 185, 190, 200,
		     210, 220, 230, 250, 300, 350, 400, 500, 750, 1000, 1500, 2000, 3000, 10000};//24 bins, 2016

  
  TH1F *H_mcphoEta = new TH1F("H_mcphoEta", "H_mcphoEta", 60, -3, 3);
  t->Draw("mcphoEta>>H_mcphoEta");
  title = H_mcphoEta->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  TH1F *H_mcphoPhi = new TH1F("H_mcphoPhi", "H_mcphoPhi", 8, -4, 4);
  t->Draw("mcphoPhi>>H_mcphoPhi");
  title = H_mcphoPhi->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_realphoEta = new TH1F("H_realphoEta", "H_realphoEta", 60, -3, 3);
  t->Draw("realphoEta>>H_realphoEta");
  title = H_realphoEta->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  TH1F *H_realphoPhi = new TH1F("H_realphoPhi", "H_realphoPhi", 8, -4, 4);
  t->Draw("realphoPhi>>H_realphoPhi");
  title = H_realphoPhi->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  TH1F *H_dphoMETPhi = (TH1F*)fopen->Get("h_dphoMETPhi");
  H_dphoMETPhi->Draw();
  title = H_dphoMETPhi->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  c1->SetLogy();
  c1->Update();

  TH1F *H_mcphoEt = new TH1F("H_mcphoEt", "H_mcphoEt", 20, 0., 800);
  t->Draw("mcphoEt>>H_mcphoEt");
  title = H_mcphoEt->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_realphoEt = new TH1F("H_realphoEt", "H_realphoEt", 20, 0., 1100);
  t->Draw("realphoEt>>H_realphoEt");
  title = H_realphoEt->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  TH1F *H_MET = (TH1F*)fopen->Get("h_MET");
  H_MET->Draw();
  title = H_MET->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

 
  TH1F *H_dr_pho = (TH1F*)fopen->Get("h_dr_pho");
  H_dr_pho->Draw();
  title = H_dr_pho->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  TH1F *H_dpt_pho = (TH1F*)fopen->Get("h_dpt_pho");
  H_dpt_pho->Draw();
  title = H_dpt_pho->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_dptdr_pho = (TH1F*)fopen->Get("h_dptdr_pho");
  c1->SetLogz(); c1->Update();
  H_dptdr_pho->GetXaxis()->SetTitle("dr");
  H_dptdr_pho->GetYaxis()->SetTitle("dpt");
  H_dptdr_pho->Draw("colz");
  title = H_dptdr_pho->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  c1->SetLogz(0);
  c1->SetLogy(0);
  c1->Update();
  
  
  
  TH1F *H_jetCEF = new TH1F("H_jetCEF", "H_jetCEF", 40, 0, 1);
  H_jetCEF->GetYaxis()->SetRangeUser(.01, 1000000);
  t->Draw("jetCEF>>H_jetCEF");
  title = H_jetCEF->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  TH1F *H_jetNEF = new TH1F("H_jetNEF", "H_jetNEF", 40, 0, 1);
  H_jetNEF->GetYaxis()->SetRangeUser(.01, 1000000);
  t->Draw("jetNEF>>H_jetCE");
  title = H_jetNEF->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  TH1F *H_jetCHF = new TH1F("H_jetCHF", "H_jetCHF", 40, 0, 1);
  H_jetCHF->GetYaxis()->SetRangeUser(.01, 1000000);
  t->Draw("jetCHF>>H_jetCHF");
  title = H_jetCHF->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  TH1F *H_jetNHF = new TH1F("H_jetNHF", "H_jetNHF", 40, 0, 1);
  H_jetNHF->GetYaxis()->SetRangeUser(.01, 1000000);
  t->Draw("jetNHF>>H_jetNHF");
  title = H_jetNHF->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_jetNCH = new TH1F("H_jetNCH", "H_jetNCH", 100, 0, 100);
  H_jetNCH->GetYaxis()->SetRangeUser(.01, 1000000);
  t->Draw("jetNCH>>H_jetNCH");
  title = H_jetNCH->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  TH1F *H_jetNNP = new TH1F("H_jetNNP", "H_jetNNP", 100, 0, 100);
  H_jetNNP->GetYaxis()->SetRangeUser(.01, 1000000);
  t->Draw("jetNNP>>H_jetNNP");
  title = H_jetNNP->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  */
  c1->SetLogy(0);
  //c1->SetLogz();
  c1->Update();
}
