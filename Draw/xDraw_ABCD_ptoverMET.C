#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;

#define nhisto 2

void xDraw_ABCD_ptoverMET(Int_t year){

  ofstream ftext;
  TString rootname[10];
  TFile *fopen, *fout;
  
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/GJet/output_merge_GJet.root";
  }
  else if(year==2017){
    rootname[0] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/wk_cms3/judychen/chip02/output_ana/220427/fall17/data/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/wk_cms3/judychen/chip02/output_ana/220427/autumn18/data/output_ggtree.root";
  }

  TCanvas *c1 = new TCanvas("c1");

  //GJet MC
  TH1F *H_ptoverMET_obj;
  Double_t Hyield_ptoverMET_CR = 0;
  Double_t Hyield_ptoverMET_SR = 0;
  Double_t Hyield_ptoverMET_CR_minJMETdPhi_SB = 0;
  Double_t Hyield_ptoverMET_SR_minJMETdPhi_SB = 0;

  //Data
  TH1F *HD_ptoverMET_obj;
  Double_t HDyield_ptoverMET_CR = 0;
  Double_t HDyield_ptoverMET_SR = 0;
  Double_t HDyield_ptoverMET_CR_minJMETdPhi_SB = 0;
  Double_t HDyield_ptoverMET_SR_minJMETdPhi_SB = 0;
  
  Double_t ptoverMET_SR_minJMETdPhi_SB_ratio = 0;
  Double_t ptoverMET_SR_ratio = 0;

  //corr from data shape with MC norm
  TH1F *H_phoEB_ptcut_ptoverMET_CR_corr;
  TH1F *H_jetpt_ptoverMET_CR_corr;
  TH1F *H_MET_ptoverMET_CR_corr;
  TH1F *H_dphoMETPhi_ptoverMET_CR_corr;
  TH1F *H_dijetMass_ptoverMET_CR_corr;
  TH1F *H_dEta_jetjet_ptoverMET_CR_corr;
  TH1F *H_minJMETdPhi_ptoverMET_CR_corr;

  //corr from data shape with MC norm
  TH1F *H_phoEB_ptcut_ptoverMET_SR_corr;
  TH1F *H_jetpt_ptoverMET_SR_corr;
  TH1F *H_MET_ptoverMET_SR_corr;
  TH1F *H_dphoMETPhi_ptoverMET_SR_corr;
  TH1F *H_dijetMass_ptoverMET_SR_corr;
  TH1F *H_dEta_jetjet_ptoverMET_SR_corr;
  TH1F *H_minJMETdPhi_ptoverMET_SR_corr;

  fopen = new TFile(rootname[0]);
  H_ptoverMET_obj = (TH1F*)fopen->Get("ptoverMET_CR/h_phoEB_ptcut_ptoverMET_CR")->Clone();
  Hyield_ptoverMET_CR = H_ptoverMET_obj->Integral(1, -1);
  H_ptoverMET_obj = (TH1F*)fopen->Get("ptoverMET_SR/h_phoEB_ptcut_ptoverMET_SR")->Clone();
  Hyield_ptoverMET_SR = H_ptoverMET_obj->Integral(1, -1);
  H_ptoverMET_obj = (TH1F*)fopen->Get("ptoverMET_CR_minJMETdPhi_SB/h_phoEB_ptcut_ptoverMET_CR_minJMETdPhi_SB")->Clone();
  Hyield_ptoverMET_CR_minJMETdPhi_SB = H_ptoverMET_obj->Integral(1, -1);
  H_ptoverMET_obj = (TH1F*)fopen->Get("ptoverMET_SR_minJMETdPhi_SB/h_phoEB_ptcut_ptoverMET_SR_minJMETdPhi_SB")->Clone();
  Hyield_ptoverMET_SR_minJMETdPhi_SB = H_ptoverMET_obj->Integral(1, -1);
  fopen->Close();

  fopen = new TFile(rootname[1]);
  HD_ptoverMET_obj = (TH1F*)fopen->Get("ptoverMET_CR/h_phoEB_ptcut_ptoverMET_CR")->Clone();
  HDyield_ptoverMET_CR = HD_ptoverMET_obj->Integral(1, -1);
  HD_ptoverMET_obj = (TH1F*)fopen->Get("ptoverMET_SR/h_phoEB_ptcut_ptoverMET_SR")->Clone();
  HDyield_ptoverMET_SR = HD_ptoverMET_obj->Integral(1, -1);
  HD_ptoverMET_obj = (TH1F*)fopen->Get("ptoverMET_CR_minJMETdPhi_SB/h_phoEB_ptcut_ptoverMET_CR_minJMETdPhi_SB")->Clone();
  HDyield_ptoverMET_CR_minJMETdPhi_SB = HD_ptoverMET_obj->Integral(1, -1);
  HD_ptoverMET_obj = (TH1F*)fopen->Get("ptoverMET_SR_minJMETdPhi_SB/h_phoEB_ptcut_ptoverMET_SR_minJMETdPhi_SB")->Clone();
  HDyield_ptoverMET_SR_minJMETdPhi_SB = HD_ptoverMET_obj->Integral(1, -1);
  fopen->Close();

  //fopen = new TFile(rootname[0]);
  //H_ptoverMET_cut = (TH1F*)fopen->Get(Form("SMandVBS/h_ptoverMET_cut_%i", 1))->Clone();
  //H_ptoverMET_CR = (TH1F*)fopen->Get(Form("SMandVBS/h_ptoverMET_cut_%i", 1))->Clone();
  //H_ptoverMET_cut->SetDirectory(0);
  //H_ptoverMET_CR->SetDirectory(0);
  //fopen->Close();
  //
  //fopen = new TFile(rootname[1]);
  //HD_ptoverMET_cut = (TH1F*)fopen->Get(Form("SMandVBS/h_ptoverMET_cut_%i", 1))->Clone();
  //HD_ptoverMET_CR = (TH1F*)fopen->Get(Form("SMandVBS/h_ptoverMET_cut_%i", 1))->Clone();
  //HD_ptoverMET_cut->SetDirectory(0);
  //HD_ptoverMET_CR->SetDirectory(0);
  //fopen->Close();
  
  //for(Int_t i=0; i<12; i++){
  //
  //  H_ptoverMET_CR->SetBinContent(i+1, 0.0);
  //  HD_ptoverMET_CR->SetBinContent(i+1, 0.0);
  //  
  //}
  
  //A_yield = H_ptoverMET_cut->Integral(1, 10);
  //B_yield = HD_ptoverMET_cut->Integral(1, 10);
  //C_yield = H_ptoverMET_cut->Integral(11, 20);
  //D_yield = HD_ptoverMET_cut->Integral(11, 20);
  //
  //AB_ratio = A_yield/B_yield;
  //CD_ratio = C_yield/D_yield;

  printf("ptoverMET_SR minJMETdPhi_SB Data/MC : %3.3f\n", HDyield_ptoverMET_SR_minJMETdPhi_SB/Hyield_ptoverMET_SR_minJMETdPhi_SB);
  printf("ptoverMET_SR Data/MC  : %3.3f\n", HDyield_ptoverMET_SR/Hyield_ptoverMET_SR);
  printf("ptoverMET_CR minJMETdPhi_SB Data/MC : %3.3f\n", HDyield_ptoverMET_CR_minJMETdPhi_SB/Hyield_ptoverMET_CR_minJMETdPhi_SB);
  printf("ptoverMET_CR Data/MC  : %3.3f\n", HDyield_ptoverMET_CR/Hyield_ptoverMET_CR);
  
  fopen = new TFile(rootname[1]);
  H_phoEB_ptcut_ptoverMET_CR_corr = (TH1F*)fopen->Get(Form("ptoverMET_CR_minJMETdPhi_SB/h_phoEB_ptcut_ptoverMET_CR_minJMETdPhi_SB"))->Clone();
  H_jetpt_ptoverMET_CR_corr = (TH1F*)fopen->Get(Form("ptoverMET_CR_minJMETdPhi_SB/h_jetpt_ptoverMET_CR_minJMETdPhi_SB"))->Clone();
  H_MET_ptoverMET_CR_corr = (TH1F*)fopen->Get(Form("ptoverMET_CR_minJMETdPhi_SB/h_MET_ptoverMET_CR_minJMETdPhi_SB"))->Clone();
  H_dphoMETPhi_ptoverMET_CR_corr = (TH1F*)fopen->Get(Form("ptoverMET_CR_minJMETdPhi_SB/h_dphoMETPhi_ptoverMET_CR_minJMETdPhi_SB"))->Clone();
  H_dijetMass_ptoverMET_CR_corr = (TH1F*)fopen->Get(Form("ptoverMET_CR_minJMETdPhi_SB/h_dijetMass_ptoverMET_CR_minJMETdPhi_SB"))->Clone();
  H_dEta_jetjet_ptoverMET_CR_corr = (TH1F*)fopen->Get(Form("ptoverMET_CR_minJMETdPhi_SB/h_dEta_jetjet_ptoverMET_CR_minJMETdPhi_SB"))->Clone();
  H_minJMETdPhi_ptoverMET_CR_corr = (TH1F*)fopen->Get(Form("ptoverMET_CR_minJMETdPhi_SB/h_minJMETdPhi_ptoverMET_CR_minJMETdPhi_SB"))->Clone();

  //printf("phoEB_ptcut Data CR : %3.1f\n", H_phoEB_ptcut_ptoverMET_CR_corr->Integral());
  Double_t CR_ratio = HDyield_ptoverMET_CR/HDyield_ptoverMET_CR_minJMETdPhi_SB;
  H_phoEB_ptcut_ptoverMET_CR_corr->Scale(CR_ratio);
  H_jetpt_ptoverMET_CR_corr->Scale(CR_ratio);
  H_MET_ptoverMET_CR_corr->Scale(CR_ratio);
  H_dphoMETPhi_ptoverMET_CR_corr->Scale(CR_ratio);
  H_dijetMass_ptoverMET_CR_corr->Scale(CR_ratio);
  H_dEta_jetjet_ptoverMET_CR_corr->Scale(CR_ratio);
  H_minJMETdPhi_ptoverMET_CR_corr->Scale(CR_ratio);

  H_phoEB_ptcut_ptoverMET_CR_corr->SetName("h_phoEB_ptcut_ptoverMET_CR");
  H_jetpt_ptoverMET_CR_corr->SetName("h_jetpt_ptoverMET_CR");
  H_MET_ptoverMET_CR_corr->SetName("h_MET_ptoverMET_CR");
  H_dphoMETPhi_ptoverMET_CR_corr->SetName("h_dphoMETPhi_ptoverMET_CR");
  H_dijetMass_ptoverMET_CR_corr->SetName("h_dijetMass_ptoverMET_CR");
  H_dEta_jetjet_ptoverMET_CR_corr->SetName("h_dEta_jetjet_ptoverMET_CR");
  H_minJMETdPhi_ptoverMET_CR_corr->SetName("h_minJMETdPhi_ptoverMET_CR");
  //fopen->Close();

  //fopen = new TFile(rootname[0]);
  H_phoEB_ptcut_ptoverMET_SR_corr = (TH1F*)fopen->Get(Form("ptoverMET_SR_minJMETdPhi_SB/h_phoEB_ptcut_ptoverMET_SR_minJMETdPhi_SB"))->Clone();
  H_jetpt_ptoverMET_SR_corr = (TH1F*)fopen->Get(Form("ptoverMET_SR_minJMETdPhi_SB/h_jetpt_ptoverMET_SR_minJMETdPhi_SB"))->Clone();
  H_MET_ptoverMET_SR_corr = (TH1F*)fopen->Get(Form("ptoverMET_SR_minJMETdPhi_SB/h_MET_ptoverMET_SR_minJMETdPhi_SB"))->Clone();
  H_dphoMETPhi_ptoverMET_SR_corr = (TH1F*)fopen->Get(Form("ptoverMET_SR_minJMETdPhi_SB/h_dphoMETPhi_ptoverMET_SR_minJMETdPhi_SB"))->Clone();
  H_dijetMass_ptoverMET_SR_corr = (TH1F*)fopen->Get(Form("ptoverMET_SR_minJMETdPhi_SB/h_dijetMass_ptoverMET_SR_minJMETdPhi_SB"))->Clone();
  H_dEta_jetjet_ptoverMET_SR_corr = (TH1F*)fopen->Get(Form("ptoverMET_SR_minJMETdPhi_SB/h_dEta_jetjet_ptoverMET_SR_minJMETdPhi_SB"))->Clone();
  H_minJMETdPhi_ptoverMET_SR_corr = (TH1F*)fopen->Get(Form("ptoverMET_SR_minJMETdPhi_SB/h_minJMETdPhi_ptoverMET_SR_minJMETdPhi_SB"))->Clone();

  //printf("phoEB_ptcut Data SR : %3.1f\n", H_phoEB_ptcut_ptoverMET_SR_corr->Integral());
  Double_t SR_ratio = Hyield_ptoverMET_SR*1.25/HDyield_ptoverMET_SR_minJMETdPhi_SB;
  H_phoEB_ptcut_ptoverMET_SR_corr->Scale(SR_ratio);
  H_jetpt_ptoverMET_SR_corr->Scale(SR_ratio);
  H_MET_ptoverMET_SR_corr->Scale(SR_ratio);
  H_dphoMETPhi_ptoverMET_SR_corr->Scale(SR_ratio);
  H_dijetMass_ptoverMET_SR_corr->Scale(SR_ratio);
  H_dEta_jetjet_ptoverMET_SR_corr->Scale(SR_ratio);
  H_minJMETdPhi_ptoverMET_SR_corr->Scale(SR_ratio);

  H_phoEB_ptcut_ptoverMET_SR_corr->SetName("h_phoEB_ptcut_ptoverMET_SR");
  H_jetpt_ptoverMET_SR_corr->SetName("h_jetpt_ptoverMET_SR");
  H_MET_ptoverMET_SR_corr->SetName("h_MET_ptoverMET_SR");
  H_dphoMETPhi_ptoverMET_SR_corr->SetName("h_dphoMETPhi_ptoverMET_SR");
  H_dijetMass_ptoverMET_SR_corr->SetName("h_dijetMass_ptoverMET_SR");
  H_dEta_jetjet_ptoverMET_SR_corr->SetName("h_dEta_jetjet_ptoverMET_SR");
  H_minJMETdPhi_ptoverMET_SR_corr->SetName("h_minJMETdPhi_ptoverMET_SR");

  
  fout = new TFile("GJet_ptoverMET_CR.root", "RECREATE");

  fout->cd();
  fout->mkdir("ptoverMET_CR");
  fout->cd("ptoverMET_CR");
  
  H_phoEB_ptcut_ptoverMET_CR_corr->Write();
  H_jetpt_ptoverMET_CR_corr->Write();
  H_MET_ptoverMET_CR_corr->Write();
  H_dphoMETPhi_ptoverMET_CR_corr->Write();
  H_dijetMass_ptoverMET_CR_corr->Write();
  H_dEta_jetjet_ptoverMET_CR_corr->Write();
  H_minJMETdPhi_ptoverMET_CR_corr->Write();

  fout->mkdir("ptoverMET_SR");
  fout->cd("ptoverMET_SR");
  
  H_phoEB_ptcut_ptoverMET_SR_corr->Write();
  H_jetpt_ptoverMET_SR_corr->Write();
  H_MET_ptoverMET_SR_corr->Write();
  H_dphoMETPhi_ptoverMET_SR_corr->Write();
  H_dijetMass_ptoverMET_SR_corr->Write();
  H_dEta_jetjet_ptoverMET_SR_corr->Write();
  H_minJMETdPhi_ptoverMET_SR_corr->Write();

  fout->Close();

}
