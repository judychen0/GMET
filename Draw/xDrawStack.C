#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
#define nfile 5

#define eqevents 1000000

void xDrawStack(){
  TString rootname[5] = {
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_GJets_sigMC_HT_13TeV-madgraphMLM-pythia8/210527_150753/output_merge.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_WGToLNuG_01J_5f_PtG_130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_0000/210527_065337/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZLLGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph_0000/210527_072853/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph-pythia8_0000/210527_064105/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]

  //Float_t mclumi[5] = {0.31,311.6,6750,989.7};//(fb-1) [0,1,2,3,4][GJet,WG,ZG,ZNuNu]
  //Float_t mclumi[5] = {311.6,6750,989.7};//(fb-1) [1,2,3,4][WG,ZG,ZNuNu]
  Float_t gjetXsec[10] = {9319, 9155, 2323, 2314, 278.5, 271.8, 93.85, 94.7};
  Float_t lumi16 = 36.33;
  Float_t Xsec = 2323.0;

  Float_t xsec[10] = {2323, 1.008, 0.148, 0.17};
  Float_t filter[10] = {1., 0.7772, 1., 1.};
  //color name
  //{"#1C2A60","#7FB5F5","#2F81A3","#FA6D5F","#91A81E","#DCF563", "#1C6040","#EFE2C9"}
  //{""}
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

  Int_t binmin, binmax;
  Double_t cutmin = -1.6;
  Double_t cutmax  = 1.6;

  TH1F *H_Events[10];
  TH1F *H_MET_Nm1[nfile];
  TH1F *H_dphoMETPhi_Nm1[nfile];
  TH1F *H_MET_cut[nfile];
  TH1F *H_dphoMETPhi_cut[nfile];
  TH1F *H_phoEB_ptcut[nfile];
  TH1F *H_phoEB_ptoverMET[nfile];
  
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";
  
  for(Int_t i=0; i<nfile; i++){

    fopen = new TFile(rootname[i]);
    t =(TTree*)fopen->Get("t");

    if(i<nfile-1){
      H_Events[i] = (TH1F*)fopen->Get("hEvents");
      entries = H_Events[i]->GetBinContent(1);
      cout << "print" << entries << endl;
      outentries = xsec[i]*1000*lumi16*filter[i];
      scale = outentries/entries;
      //if(i==4) scale*=100000;
    }
    if(i==0) scale*=0.01;
    
    H_MET_Nm1[i] = (TH1F*)fopen->Get("h_MET_Nm1");
    H_dphoMETPhi_Nm1[i] = (TH1F*)fopen->Get("h_dphoMETPhi_Nm1");
    H_MET_cut[i] = (TH1F*)fopen->Get("h_MET_cut");
    H_dphoMETPhi_cut[i] = (TH1F*)fopen->Get("h_dphoMETPhi_cut");
    H_phoEB_ptcut[i] = (TH1F*)fopen->Get("h_phoEB_ptcut");
    H_phoEB_ptoverMET[i] = (TH1F*)fopen->Get("h_phoEB_ptoverMET");

    if(i< nfile-1){
      H_MET_Nm1[i]->Scale(scale);
      H_dphoMETPhi_Nm1[i]->Scale(scale);
      H_MET_cut[i]->Scale(scale);
      H_dphoMETPhi_cut[i]->Scale(scale);
      H_phoEB_ptcut[i]->Scale(scale);
      //H_phoEB_pt_dphoMETPhi[i] = (TH1F*)fopen->Get("h_phoEB_pt_dphoMETPhi");
      //H_phoEB_pt_dphoMETPhi[i]->Scale(scale);
      H_phoEB_ptoverMET[i]->Scale(scale);
    }
    
    //fopen->Close();

    H_MET_Nm1[i]->SetFillColor(cnum[i]);
    H_MET_Nm1[i]->SetLineColor(cnum[i]);

    H_dphoMETPhi_Nm1[i]->SetFillColor(cnum[i]);
    H_dphoMETPhi_Nm1[i]->SetLineColor(cnum[i]);

    H_MET_cut[i]->SetFillColor(cnum[i]);
    H_MET_cut[i]->SetLineColor(cnum[i]);

    H_dphoMETPhi_cut[i]->SetFillColor(cnum[i]);
    H_dphoMETPhi_cut[i]->SetLineColor(cnum[i]);

    H_phoEB_ptcut[i]->SetFillColor(cnum[i]);
    H_phoEB_ptcut[i]->SetLineColor(cnum[i]);
    
    H_phoEB_ptoverMET[i]->SetFillColor(cnum[i]);
    H_phoEB_ptoverMET[i]->SetLineColor(cnum[i]);
  }


  THStack *HS_MET_Nm1 = new THStack("HS_MET_Nm1", "HS_MET_Nm1");
  THStack *HS_dphoMETPhi_Nm1 = new THStack("HS_dphoMETPhi_Nm1", "HS_dphoMETPhi_Nm1");
  THStack *HS_MET_cut = new THStack("HS_MET_cut", "HS_MET_cut");
  THStack *HS_dphoMETPhi_cut = new THStack("HS_dphoMETPhi_cut", "HS_dphoMETPhi_cut");
  THStack *HS_phoEB_ptcut = new THStack("HS_phoEB_ptcut", "HS_phoEB_ptcut");
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

  
  for(Int_t i=0; i<nfile-1; i++){
    //if(i==4)continue;
    //H_dphoMETPhi[i]->GetYaxis()->SetRangeUser(100, 10000);
    HS_MET_Nm1->Add(H_MET_Nm1[i]);
    H_MET_Nm1[i]->Draw("HIST");
    title = H_MET_Nm1[i]->GetName();
    //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));

    HS_MET_cut->Add(H_MET_cut[i]);
    H_MET_cut[i]->Draw("HIST");
    title = H_MET_cut[i]->GetName();
    
    HS_phoEB_ptcut->Add(H_phoEB_ptcut[i]);
    
    c1->SetLogy();
    c1->Update();
    
    HS_dphoMETPhi_Nm1->Add(H_dphoMETPhi_Nm1[i]);
    H_dphoMETPhi_Nm1[i]->Draw("HIST");
    title = H_dphoMETPhi_Nm1[i]->GetName();
    //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
    HS_dphoMETPhi_cut->Add(H_dphoMETPhi_cut[i]);
    H_dphoMETPhi_cut[i]->Draw("HIST");
    title = H_dphoMETPhi_cut[i]->GetName();
    
    HS_phoEB_ptoverMET->Add(H_phoEB_ptoverMET[i]);    
    H_phoEB_ptoverMET[i]->Draw("HIST");
    title = H_phoEB_ptoverMET[i]->GetName();
    //c1->SaveAs(Form("%s/%s%i.pdf", saveto, title, i));
  }

  TLegend *lhs = new TLegend(0.65,0.72, 0.75, 0.88);
  lhs->SetBorderSize(0);
  lhs->SetFillStyle(0);
  
 
  HS_MET_Nm1->Draw("HIST");
  H_MET_Nm1[nfile-1]->Draw("AP SAME");
  HS_MET_Nm1->GetXaxis()->SetTitle("MET");
  HS_MET_Nm1->GetYaxis()->SetTitle("Events");
  lhs->Clear();
  lhs->AddEntry(H_MET_Nm1[0], "#gamma+jet", "F");
  lhs->AddEntry(H_MET_Nm1[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_MET_Nm1[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_MET_Nm1[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_MET_Nm1[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_MET_Nm1->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  //HS_dphoMETPhi->Draw("HIST");
  
  HS_dphoMETPhi_Nm1->Draw("HIST");
  H_dphoMETPhi_Nm1[nfile-1]->Draw("SAME");
  HS_dphoMETPhi_Nm1->SetMinimum(5);
  HS_dphoMETPhi_Nm1->GetXaxis()->SetTitle("#Delta#phi(#gamma,MET)");
  HS_dphoMETPhi_Nm1->GetYaxis()->SetTitle("Events");
  lhs->Clear();
  lhs->AddEntry(H_dphoMETPhi_Nm1[0], "#gamma+jet", "F");
  lhs->AddEntry(H_dphoMETPhi_Nm1[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_dphoMETPhi_Nm1[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_dphoMETPhi_Nm1[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_dphoMETPhi_Nm1[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_dphoMETPhi_Nm1->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  HS_MET_cut->Draw("HIST");
  H_MET_cut[nfile-1]->Draw("SAME");
  HS_MET_cut->GetXaxis()->SetTitle("MET");
  HS_MET_cut->GetYaxis()->SetTitle("Events");
  lhs->Clear();
  lhs->AddEntry(H_MET_cut[0], "#gamma+jet", "F");
  lhs->AddEntry(H_MET_cut[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_MET_cut[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_MET_cut[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_MET_cut[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_MET_cut->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  //HS_dphoMETPhi->Draw("HIST");
  
  HS_dphoMETPhi_cut->Draw("HIST");
  H_dphoMETPhi_cut[nfile-1]->Draw("SAME");
  HS_dphoMETPhi_cut->SetMinimum(5);
  HS_dphoMETPhi_cut->GetXaxis()->SetTitle("#Delta#phi(#gamma,MET)");
  HS_dphoMETPhi_cut->GetYaxis()->SetTitle("Events");
  lhs->Clear();
  lhs->AddEntry(H_dphoMETPhi_cut[0], "#gamma+jet", "F");
  lhs->AddEntry(H_dphoMETPhi_cut[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_dphoMETPhi_cut[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_dphoMETPhi_cut[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_dphoMETPhi_cut[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_dphoMETPhi_cut->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  
  HS_phoEB_ptoverMET->Draw("HIST");
  H_phoEB_ptoverMET[nfile-1]->Draw("APE SAME");
  HS_phoEB_ptoverMET->GetXaxis()->SetTitle("P_{T}^{#gamma}/MET");
  HS_phoEB_ptoverMET->GetYaxis()->SetTitle("Events");
  HS_phoEB_ptoverMET->SetMaximum(1000);
  lhs->Clear();
  lhs->AddEntry(H_phoEB_ptoverMET[0], "#gamma+jet", "F");
  lhs->AddEntry(H_phoEB_ptoverMET[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_phoEB_ptoverMET[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_phoEB_ptoverMET[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_phoEB_ptoverMET[4], "DATA", "PL");
  lhs->Draw("SAME");
  //HS_phoEB_ptoverMET->GetYaxis()->SetRangeUser(0.01, 100);
  title = HS_phoEB_ptoverMET->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title)); 

  c1->SetLogy(0);
  c1->Update();
  //HS_phoEB_ptcut->SetMaximum(100000);
  
  HS_phoEB_ptcut->Draw("HIST");
  H_phoEB_ptcut[nfile-1]->Draw("AP SAME");
  HS_phoEB_ptcut->GetXaxis()->SetTitle("P_{T}^{#gamma}");
  HS_phoEB_ptcut->GetYaxis()->SetTitle("Events");
  HS_phoEB_ptcut->GetXaxis()->SetRangeUser(100, 900);
  lhs->Clear();
  lhs->AddEntry(H_phoEB_ptcut[0], "#gamma+jet", "F");
  lhs->AddEntry(H_phoEB_ptcut[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_phoEB_ptcut[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_phoEB_ptcut[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_phoEB_ptcut[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_phoEB_ptcut->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  
}
