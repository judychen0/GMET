#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
#define nfile 4
#define eqevents 1000000

void xDrawStack(){
  TString rootname[5] = {"/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_GJet_Pt-15To6000_TuneCUETP8M1-Flat_13TeV_pythia8_20M_0000/210505_040352/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_WGToLNuG_01J_5f_PtG_130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0000/210505_082513/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_ZLLGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph_0000/210505_064501/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/210505/summer16_ZGTo2NuG_PtG-130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0000/210505_064756/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]
  Float_t mclumi[5] = {0.002744,311.6,2.786,583.9};//(fb-1) [0,1,2,3,4][GJet,WG,ZG,ZNuNu]
  //Float_t mccross[5] = {}
  Float_t lumi16 = 36.33;

  //color name
  //{"#1C2A60","#7FB5F5","#2F81A3","#FA6D5F","#91A81E","#DCF563", "#1C6040","#EFE2C9"}
  //{""}
  string hexcolor[8] = {"#7FB5F5","#2F81A3","#1C2A60","#FA6D5F","#91A81E","#DCF563", "#1C6040","#EFE2C9"};
  TColor *color[8];
  Int_t cnum[8];
  
  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  TTree *t;
  Int_t entries;
  Float_t outentries = 0.;
  Float_t scale = 0.;

  Int_t binmin, binmax;
  Double_t cutmin = -1.6;
  Double_t cutmax  = 1.6;
  
 
  TH1F *H_MET[nfile];
  TH1F *H_dphoMETPhi[nfile];
  TH1F *H_phoEB_pt_MET[nfile];
  TH1F *H_phoEB_ptoverMET[nfile];
  
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";
    
  for(Int_t i=0; i<nfile; i++){
    
    fopen = new TFile(rootname[i]);
    t =(TTree*)fopen->Get("t");
    
    entries = t->GetEntries();
    cout << "print" << entries << endl;
    outentries = eqevents*(lumi16/mclumi[i]);
    scale = outentries/entries;
    //if(i==4) scale*=100000;
    if(i==0) scale*=0.00001;
    
    H_MET[i] = (TH1F*)fopen->Get("h_MET");
    H_MET[i]->Scale(scale);
    
    H_dphoMETPhi[i] = (TH1F*)fopen->Get("h_dphoMETPhi");
    H_dphoMETPhi[i]->Scale(scale);

    H_phoEB_pt_MET[i] = new TH1F(Form("H_phoEB_pt_MET%i", i), Form("H_phoEB_pt_MET%i", i), 20, 100, 900);
    t->Draw(Form("realphoEt>>H_phoEB_pt_MET%i", i));
    H_phoEB_pt_MET[i]->Scale(scale);
    //H_phoEB_pt_dphoMETPhi[i] = (TH1F*)fopen->Get("h_phoEB_pt_dphoMETPhi");
    //H_phoEB_pt_dphoMETPhi[i]->Scale(scale);
    
    H_phoEB_ptoverMET[i] = (TH1F*)fopen->Get("h_phoEB_ptoverMET");
    H_phoEB_ptoverMET[i]->Scale(scale);
    //fopen->Close();

    
    for(Int_t j=0; j<8; j++){
      color[j] = new TColor();
      Int_t n = hexcolor[j].length();
      char chararray[n+1];
      strcpy(chararray, hexcolor[j].c_str());
      cnum[j] = color[j]->GetColor(chararray);
    } 
    
    H_MET[i]->SetFillColor(cnum[i]);
    H_MET[i]->SetLineColor(cnum[i]);

    H_dphoMETPhi[i]->SetFillColor(cnum[i]);
    H_dphoMETPhi[i]->SetLineColor(cnum[i]);

    //cut small dphi
    /*
    binmin = H_dphoMETPhi[i]->FindBin(cutmin);
    cout << binmin << endl;
    binmax = H_dphoMETPhi[i]->FindBin(cutmax);
    for(Int_t j=binmin; j<binmax+1; j++){
      H_dphoMETPhi[i]->SetBinContent(j, 0);
    }
    */

    H_phoEB_pt_MET[i]->SetFillColor(cnum[i]);
    H_phoEB_pt_MET[i]->SetLineColor(cnum[i]);
    
    H_phoEB_ptoverMET[i]->SetFillColor(cnum[i]);
    H_phoEB_ptoverMET[i]->SetLineColor(cnum[i]);
  }

  THStack *HS_MET = new THStack("HS_MET", "HS_MET");
  THStack *HS_dphoMETPhi = new THStack("HS_dphoMETPhi", "HS_dphoMETPhi");
  THStack *HS_phoEB_pt_MET = new THStack("HS_phoEB_pt_MET", "HS_phoEB_pt_MET");
  THStack *HS_phoEB_ptoverMET = new THStack("HS_phoEB_ptoverMET", "HS_phoEV_ptoverMET");


  //TColor *color = new TColor();
  //Int_t num = color->GetColor("#B63162");
  //H_dphoMETPhi[0]->SetFillColor(num);//maskgreen
  //H_dphoMETPhi[1]->SetFillColor(220);//gooseyellow
  //H_dphoMETPhi[2]->SetFillColor(63);//skyblue
  //H_dphoMETPhi[3]->SetFillColor(63);//skyblue
  //H_dphoMETPhi[4]->SetFillColor(29);//green

  //H_phoEB_ptoverMET[0]->SetFillColor(num);//maskgreen
  //H_phoEB_ptoverMET[1]->SetFillColor(220);//gooseyellow
  //H_phoEB_ptoverMET[2]->SetFillColor(63);//skyblue
  //H_phoEB_ptoverMET[3]->SetFillColor(63);//skyblue
  //H_phoEB_ptoverMET[4]->SetFillColor(29);//green

  
  for(Int_t i=0; i<nfile; i++){
    //if(i==4)continue;
    //H_dphoMETPhi[i]->GetYaxis()->SetRangeUser(100, 10000);
    HS_MET->Add(H_MET[i]);
    H_MET[i]->Draw("HIST");
    title = H_MET[i]->GetName();
    //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));

    HS_phoEB_pt_MET->Add(H_phoEB_pt_MET[i]);
    
    c1->SetLogy();
    c1->Update();
    
    HS_dphoMETPhi->Add(H_dphoMETPhi[i]);
    H_dphoMETPhi[i]->Draw("HIST");
    title = H_dphoMETPhi[i]->GetName();
    //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    
    HS_phoEB_ptoverMET->Add(H_phoEB_ptoverMET[i]);    
    H_phoEB_ptoverMET[i]->Draw("HIST");
    title = H_phoEB_ptoverMET[i]->GetName();
    //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
  }
  
  HS_MET->Draw("HIST");
  title = HS_MET->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  //HS_dphoMETPhi->Draw("HIST");
  HS_dphoMETPhi->SetMinimum(400);
  HS_dphoMETPhi->Draw("HIST");
  title = HS_dphoMETPhi->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  HS_phoEB_ptoverMET->Draw("HIST");
  //HS_phoEB_ptoverMET->GetYaxis()->SetRangeUser(0.01, 100);
  title = HS_phoEB_ptoverMET->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title)); 

  c1->SetLogy(0);
  c1->Update();
  //HS_phoEB_pt_MET->SetMaximum(100000);
  HS_phoEB_pt_MET->Draw("HIST");
  title = HS_phoEB_pt_MET->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  
}
