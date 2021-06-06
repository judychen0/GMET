#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;
#define nfile 5
#define nGJet 8

#define eqevents 1000000

TH1* coolHSC(THStack* st)
 {
    if (!st) {
       Warning("TRatioPlot", "Need only a stack");
       //return;
    }
  
    TList *stackHists = st->GetHists();
  
    if (stackHists->GetSize() == 0) {
       Warning("TRatioPlot", "Stack does not have histograms");
       //return;
    }
  
    TH1* tmpHist = (TH1*)stackHists->At(0)->Clone();
    tmpHist->Reset();
  
    for (int i=0;i<stackHists->GetSize();++i) {
       tmpHist->Add((TH1*)stackHists->At(i));
    }
    return tmpHist;
 }

TH1F* coolratioplot(THStack* st, TH1F* h1){

  if (!st) {
       Warning("TRatioPlot", "Need only a stack");
       //return;
    }
  
    TList *stackHists = st->GetHists();
  
    if (stackHists->GetSize() == 0) {
       Warning("TRatioPlot", "Stack does not have histograms");
       //return;
    }
  
    TH1F* tmpHist = (TH1F*)stackHists->At(0)->Clone();
    tmpHist->Reset();
  
    for (int i=0;i<stackHists->GetSize();++i) {
       tmpHist->Add((TH1*)stackHists->At(i));
    }

    
  TH1F *ratio = (TH1F*)h1->Clone("ratio");
  ratio->SetLineColor(kBlack);
  ratio->SetMinimum(-0.4);
  ratio->SetMaximum(2.4);
  ratio->Sumw2();
  ratio->Divide(tmpHist);
  
  ratio->SetMarkerStyle(8);
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerSize(0.8);
  ratio->Draw("EP");
  ratio->SetTitle("");
  
  ratio->GetYaxis()->SetTitle("Data/MC");
  ratio->GetYaxis()->SetTitleSize(0.06);
  ratio->GetYaxis()->SetTitleFont(42);
  ratio->GetYaxis()->SetTitleColor(1);
  ratio->GetYaxis()->SetNdivisions(510);
  ratio->GetYaxis()->SetAxisColor(1);
  ratio->GetYaxis()->SetStripDecimals(kTRUE);
  ratio->GetYaxis()->SetTickLength(0.03);
  ratio->GetYaxis()->SetLabelColor(1);
  ratio->GetYaxis()->SetLabelFont(42);
  ratio->GetYaxis()->SetLabelOffset(0.007);
  ratio->GetYaxis()->SetLabelSize(0.05);

  ratio->GetXaxis()->
  
  
  return ratio;
}

void xDrawStack(){
  TString rootname[5] = {
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_GJets_sigMC_HT_13TeV-madgraphMLM-pythia8/output_merge.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_WGToLNuG_01J_5f_PtG_130_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/210602_180734/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZLLGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph/210602_184359/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph-pythia8/210602_163537/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]

  //Float_t mclumi[5] = {0.31,311.6,6750,989.7};//(fb-1) [0,1,2,3,4][GJet,WG,ZG,ZNuNu]
  //Float_t mclumi[5] = {311.6,6750,989.7};//(fb-1) [1,2,3,4][WG,ZG,ZNuNu]
  Float_t gjetXsec[10] = {9319, 9155, 2323, 2314, 278.5, 271.8, 93.85, 94.7};
  Float_t lumi16 = 36.33;
  Float_t Xsec = 2323.0;

  Float_t xsec[10] = {2323, 1.008, 0.148, 0.17};
  Float_t filter[10] = {1., 0.7772, 1., 1.};
  Float_t kfactor[10] = {1., 1., 1.067, 1.617};
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

  TH1F *H_MET_cut[nfile];
  TH1F *H_dphoMETPhi_cut[nfile];
  TH1F *H_njet_cut[nfile];
  TH1F *H_jetpt_cut[nfile];
  TH1F *H_djetMETPhi_cut[nfile];

  TH1F *H_phoEB_ptMid[nfile];
  TH1F *H_phoEB_ptcut[nfile];
  TH1F *H_phoEB_ptoverMET[nfile];
  TH1F *H_phoEB_ptoverjetpt[nfile];
  
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";
  
  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    
    if(i<nfile-1){
      H_Events[i] = (TH1F*)fopen->Get("hEvents");
      entries = H_Events[i]->GetBinContent(1);
      outentries = xsec[i]*kfactor[i]*1000*lumi16*filter[i];
      scale = outentries/entries;
      cout << "print " << entries << " " << outentries << " " << scale<< endl;
      //if(i==4) scale*=100000;
    }
    if(i==4) scale = 1.9055;
    
    H_MET_cut[i] = (TH1F*)fopen->Get("h_MET_cut");
    H_dphoMETPhi_cut[i] = (TH1F*)fopen->Get("h_dphoMETPhi_cut");
    H_njet_cut[i] = (TH1F*)fopen->Get("h_njet_cut");
    H_jetpt_cut[i] = (TH1F*)fopen->Get("h_jetpt_cut");
    H_djetMETPhi_cut[i] = (TH1F*)fopen->Get("h_djetMETPhi_cut");

    H_phoEB_ptMid[i] = (TH1F*)fopen->Get("h_phoEB_ptMid");
    H_phoEB_ptcut[i] = (TH1F*)fopen->Get("h_phoEB_ptcut");
    H_phoEB_ptoverMET[i] = (TH1F*)fopen->Get("h_phoEB_ptoverMET");
    H_phoEB_ptoverjetpt[i] = (TH1F*)fopen->Get("h_phoEB_ptoverjetpt");
    
    //if(i< 4){
      H_MET_cut[i]->Scale(scale);
      H_dphoMETPhi_cut[i]->Scale(scale);
      H_njet_cut[i]->Scale(scale);
      H_jetpt_cut[i]->Scale(scale);
      H_djetMETPhi_cut[i]->Scale(scale);

      H_phoEB_ptMid[i]->Scale(scale);
      H_phoEB_ptcut[i]->Scale(scale);
      //H_phoEB_pt_dphoMETPhi[i]->Scale(scale);
      H_phoEB_ptoverMET[i]->Scale(scale);
      H_phoEB_ptoverjetpt[i]->Scale(scale);
      //}
    
    //fopen->Close();
    
    H_MET_cut[i]->SetFillColor(cnum[i]);
    H_dphoMETPhi_cut[i]->SetFillColor(cnum[i]);
    H_njet_cut[i]->SetFillColor(cnum[i]);
    H_jetpt_cut[i]->SetFillColor(cnum[i]);
    H_djetMETPhi_cut[i]->SetFillColor(cnum[i]);
    H_phoEB_ptMid[i]->SetFillColor(cnum[i]);
    H_phoEB_ptcut[i]->SetFillColor(cnum[i]);
    H_phoEB_ptoverMET[i]->SetFillColor(cnum[i]);
    H_phoEB_ptoverjetpt[i]->SetFillColor(cnum[i]);

            
    H_MET_cut[i]->SetLineColor(cnum[i]);
    H_dphoMETPhi_cut[i]->SetLineColor(cnum[i]);
    H_njet_cut[i]->SetLineColor(cnum[i]);
    H_jetpt_cut[i]->SetLineColor(cnum[i]);
    H_djetMETPhi_cut[i]->SetLineColor(cnum[i]);
    H_phoEB_ptMid[i]->SetLineColor(cnum[i]);
    H_phoEB_ptcut[i]->SetLineColor(cnum[i]);
    H_phoEB_ptoverMET[i]->SetLineColor(cnum[i]);
    H_phoEB_ptoverjetpt[i]->SetLineColor(cnum[i]);
    if(i==4){
      H_MET_cut[i]->SetLineWidth(2);
      H_dphoMETPhi_cut[i]->SetLineWidth(2);
      H_njet_cut[i]->SetLineWidth(2);
      H_jetpt_cut[i]->SetLineWidth(2);
      H_djetMETPhi_cut[i]->SetLineWidth(2);
      H_phoEB_ptMid[i]->SetLineWidth(2);
      H_phoEB_ptcut[i]->SetLineWidth(2);
      H_phoEB_ptoverMET[i]->SetLineWidth(2);
      H_phoEB_ptoverjetpt[i]->SetLineWidth(2);

      H_MET_cut[i]->SetMarkerStyle(8);
      H_dphoMETPhi_cut[i]->SetMarkerStyle(8);
      H_njet_cut[i]->SetMarkerStyle(8);
      H_jetpt_cut[i]->SetMarkerStyle(8);
      H_djetMETPhi_cut[i]->SetMarkerStyle(8);
      H_phoEB_ptMid[i]->SetMarkerStyle(8);
      H_phoEB_ptcut[i]->SetMarkerStyle(8);
      H_phoEB_ptoverMET[i]->SetMarkerStyle(8);
      H_phoEB_ptoverjetpt[i]->SetMarkerStyle(8);
      
      H_MET_cut[i]->SetMarkerSize(0.8);
      H_dphoMETPhi_cut[i]->SetMarkerSize(0.8);
      H_njet_cut[i]->SetMarkerSize(0.8);
      H_jetpt_cut[i]->SetMarkerSize(0.8);
      H_djetMETPhi_cut[i]->SetMarkerSize(0.8);
      H_phoEB_ptMid[i]->SetMarkerSize(0.8);
      H_phoEB_ptcut[i]->SetMarkerSize(0.8);
      H_phoEB_ptoverMET[i]->SetMarkerSize(0.8);
      H_phoEB_ptoverjetpt[i]->SetMarkerSize(0.8);
      
    }
  }
 
  THStack *HS_MET_cut = new THStack("HS_MET_cut", "HS_MET_cut");
  THStack *HS_dphoMETPhi_cut = new THStack("HS_dphoMETPhi_cut", "HS_dphoMETPhi_cut");
  THStack *HS_njet_cut = new THStack("HS_njet_cut", "HS_njet_cut");
  THStack *HS_jetpt_cut = new THStack("HS_jetpt_cut", "HS_jetpt_cut");
  THStack *HS_djetMETPhi_cut = new THStack("HS_djetMETPhi_cut", "HS_djetMETPhi_cut");

  THStack *HS_phoEB_ptMid = new THStack("HS_phoEB_ptMid", "HS_phoEB_ptMid");
  THStack *HS_phoEB_ptcut = new THStack("HS_phoEB_ptcut", "HS_phoEB_ptcut");
  THStack *HS_phoEB_ptoverMET = new THStack("HS_phoEB_ptoverMET", "HS_phoEB_ptoverMET");
  THStack *HS_phoEB_ptoverjetpt = new THStack("HS_phoEB_ptoverjetpt", "HS_phoEB_ptoverjetpt");

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
    
    HS_MET_cut->Add(H_MET_cut[i]);
    HS_njet_cut->Add(H_njet_cut[i]);
    HS_jetpt_cut->Add(H_jetpt_cut[i]);
    HS_phoEB_ptMid->Add(H_phoEB_ptMid[i]);
    HS_phoEB_ptcut->Add(H_phoEB_ptcut[i]);
    
    c1->SetLogy();
    c1->Update();
    
    HS_dphoMETPhi_cut->Add(H_dphoMETPhi_cut[i]);
    HS_djetMETPhi_cut->Add(H_djetMETPhi_cut[i]);
    HS_phoEB_ptoverMET->Add(H_phoEB_ptoverMET[i]);    
    HS_phoEB_ptoverjetpt->Add(H_phoEB_ptoverjetpt[i]);
  }

  TLegend *lhs = new TLegend(0.65,0.72, 0.75, 0.88);
  lhs->SetBorderSize(0);
  lhs->SetFillStyle(0);
  
  //HS_dphoMETPhi->Draw("HIST");
  TRatioPlot *rp;
  Int_t n = 0;//n points of ratio plot
  Double_t ax[100], ay[100];
  TH1F *hratio;
  /*
  TPad *pad1 = new TPad("pad1", "", 0., 0.3, 1., 1.0);
  pad1->SetBottomMargin(0);
  pad1->Draw();
  c1->cd();
  TPad *pad2 = new TPad("pad2", "", 0., 0.05, 1., 0.3);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.2);
  pad2->Draw();
  
  c1->cd();
  pad1->cd();
  HS_MET_cut->Draw("HIST");
  H_MET_cut[nfile-1]->Draw("SAME");
  HS_MET_cut->GetYaxis()->SetTitle("Events");
  HS_MET_cut->GetXaxis()->SetTitle("MET");
  HS_MET_cut->SetMaximum(100);
  lhs->Clear();
  lhs->AddEntry(H_MET_cut[0], "#gamma+jet", "F");
  lhs->AddEntry(H_MET_cut[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_MET_cut[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_MET_cut[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_MET_cut[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_MET_cut->GetName();
  c1->cd();
  pad2->cd();
  hratio = ratioplot(HS_MET_cut, H_MET_cut[nfile-1]);
  hratio->Draw("EP");
  hratio->SetTitle("");
  hratio->GetYaxis()->SetTitle("Data/MC");
  hratio->GetYaxis()->SetNdivisions(510);
  hratio->GetYaxis()->SetTitleSize(0.06);
  hratio->GetYaxis()->SetTitleFont(42);
  hratio->GetYaxis()->SetTitleColor(1);
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  */

  
  HS_MET_cut->Draw("HIST");
  H_MET_cut[nfile-1]->Draw("SAME");
  HS_MET_cut->GetYaxis()->SetTitle("Events");
  HS_MET_cut->GetXaxis()->SetTitle("MET");
  HS_MET_cut->SetMaximum(100);
  //h = coolHSC(HS_MET_cut);
  rp = new TRatioPlot(HS_MET_cut, H_MET_cut[nfile-1]);
  rp->Draw("EP");
  /*
  n = rp->GetLowerRefGraph()->GetN();
  cout << "npoints " << n << endl; 
  for(Int_t i=0; i<n; i++){
    rp->GetLowerRefGraph()->GetPoint(i, ax[i], ay[i]);
    ay[i]= 1/ay[i];
    rp->GetLowerRefGraph()->SetPoint(i, ax[i], ay[i]);
  }
  */
  rp->GetLowerRefGraph()->SetMarkerStyle(8);
  rp->GetLowerRefGraph()->SetMarkerColor(kBlack);
  rp->GetLowerRefGraph()->SetMarkerSize(0.8);
  rp->GetLowerRefYaxis()->SetTitle("MC/Data");
  rp->GetLowerRefYaxis()->SetRangeUser(-0.1, 2.5);
  rp->SetSeparationMargin(0);
  lhs->Clear();
  lhs->AddEntry(H_MET_cut[0], "#gamma+jet", "F");
  lhs->AddEntry(H_MET_cut[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_MET_cut[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_MET_cut[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_MET_cut[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_MET_cut->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  HS_dphoMETPhi_cut->Draw("HIST");
  H_dphoMETPhi_cut[nfile-1]->Draw("SAME");
  HS_dphoMETPhi_cut->SetMinimum(0.001);
  HS_dphoMETPhi_cut->GetXaxis()->SetTitle("#Delta#phi(#gamma,MET)");
  HS_dphoMETPhi_cut->GetYaxis()->SetTitle("Events");
  rp = new TRatioPlot(HS_dphoMETPhi_cut, H_dphoMETPhi_cut[nfile-1]);
  rp->Draw("EP");
  /*
  n = rp->GetLowerRefGraph()->GetN();
  cout << "npoints " << n << endl; 
  for(Int_t i=0; i<n; i++){
    rp->GetLowerRefGraph()->GetPoint(i, ax[i], ay[i]);
    ay[i]= 1/ay[i];
    rp->GetLowerRefGraph()->SetPoint(i, ax[i], ay[i]);
  }
  */
  rp->GetLowerRefGraph()->SetMarkerStyle(8);
  rp->GetLowerRefGraph()->SetMarkerColor(kBlack);
  rp->GetLowerRefGraph()->SetMarkerSize(0.8);
  rp->GetLowerRefYaxis()->SetTitle("MC/Data");
  rp->GetLowerRefYaxis()->SetRangeUser(-0.1, 2.5);
  rp->SetSeparationMargin(0);
  lhs->Clear();
  lhs->AddEntry(H_dphoMETPhi_cut[0], "#gamma+jet", "F");
  lhs->AddEntry(H_dphoMETPhi_cut[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_dphoMETPhi_cut[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_dphoMETPhi_cut[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_dphoMETPhi_cut[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_dphoMETPhi_cut->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  HS_njet_cut->Draw("HIST");
  H_njet_cut[nfile-1]->Draw("SAME");
  HS_njet_cut->GetXaxis()->SetTitle("njet");
  HS_njet_cut->GetYaxis()->SetTitle("Events");
  HS_njet_cut->SetMaximum(1000);
  rp = new TRatioPlot(HS_njet_cut, H_njet_cut[nfile-1]);
  rp->Draw("EP");
  /*
  n = rp->GetLowerRefGraph()->GetN();
  cout << "npoints " << n << endl; 
  for(Int_t i=0; i<n; i++){
    rp->GetLowerRefGraph()->GetPoint(i, ax[i], ay[i]);
    ay[i]= 1/ay[i];
    rp->GetLowerRefGraph()->SetPoint(i, ax[i], ay[i]);
  }
  */
  rp->GetLowerRefGraph()->SetMarkerStyle(8);
  rp->GetLowerRefGraph()->SetMarkerColor(kBlack);
  rp->GetLowerRefGraph()->SetMarkerSize(0.8);
  rp->GetLowerRefYaxis()->SetTitle("MC/Data");
  rp->GetLowerRefYaxis()->SetRangeUser(-0.1, 2.5);
  rp->SetSeparationMargin(0);
  lhs->Clear();
  lhs->AddEntry(H_njet_cut[0], "#gamma+jet", "F");
  lhs->AddEntry(H_njet_cut[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_njet_cut[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_njet_cut[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_njet_cut[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_njet_cut->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  HS_jetpt_cut->Draw("HIST");
  H_jetpt_cut[nfile-1]->Draw("SAME");
  HS_jetpt_cut->GetXaxis()->SetTitle("P_{T}^{jet}");
  HS_jetpt_cut->GetYaxis()->SetTitle("Events");
  HS_jetpt_cut->SetMaximum(1000);
  rp = new TRatioPlot(HS_jetpt_cut, H_jetpt_cut[nfile-1]);
  rp->Draw("EP");
  /*
  n = rp->GetLowerRefGraph()->GetN();
  cout << "npoints " << n << endl; 
  for(Int_t i=0; i<n; i++){
    rp->GetLowerRefGraph()->GetPoint(i, ax[i], ay[i]);
    ay[i]= 1/ay[i];
    rp->GetLowerRefGraph()->SetPoint(i, ax[i], ay[i]);
  }
  */
  rp->GetLowerRefGraph()->SetMarkerStyle(8);
  rp->GetLowerRefGraph()->SetMarkerColor(kBlack);
  rp->GetLowerRefGraph()->SetMarkerSize(0.8);
  rp->GetLowerRefYaxis()->SetTitle("MC/Data");
  rp->GetLowerRefYaxis()->SetRangeUser(-0.1, 2.5);
  rp->SetSeparationMargin(0);
  lhs->Clear();
  lhs->AddEntry(H_jetpt_cut[0], "#gamma+jet", "F");
  lhs->AddEntry(H_jetpt_cut[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_jetpt_cut[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_jetpt_cut[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_jetpt_cut[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_jetpt_cut->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  HS_djetMETPhi_cut->Draw("HIST");
  H_djetMETPhi_cut[nfile-1]->Draw("SAME");
  HS_djetMETPhi_cut->GetXaxis()->SetTitle("#Delta#phi(jet, MET)");
  HS_djetMETPhi_cut->GetYaxis()->SetTitle("Events");
  HS_djetMETPhi_cut->SetMaximum(100);
  HS_djetMETPhi_cut->SetMinimum(0.5);
  rp = new TRatioPlot(HS_djetMETPhi_cut, H_djetMETPhi_cut[nfile-1]);
  rp->Draw("EP");
  /*
  n = rp->GetLowerRefGraph()->GetN();
  cout << "npoints " << n << endl; 
  for(Int_t i=0; i<n; i++){
    rp->GetLowerRefGraph()->GetPoint(i, ax[i], ay[i]);
    ay[i]= 1/ay[i];
    rp->GetLowerRefGraph()->SetPoint(i, ax[i], ay[i]);
  }
  */
  rp->GetLowerRefGraph()->SetMarkerStyle(8);
  rp->GetLowerRefGraph()->SetMarkerColor(kBlack);
  rp->GetLowerRefGraph()->SetMarkerSize(0.8);
  rp->GetLowerRefYaxis()->SetTitle("MC/Data");
  rp->GetLowerRefYaxis()->SetRangeUser(-0.1, 2.5);
  rp->SetSeparationMargin(0);
  lhs->Clear();
  lhs->AddEntry(H_djetMETPhi_cut[0], "#gamma+jet", "F");
  lhs->AddEntry(H_djetMETPhi_cut[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_djetMETPhi_cut[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_djetMETPhi_cut[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_djetMETPhi_cut[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_djetMETPhi_cut->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  //HS_dphoMETPhi->Draw("HIST");

  HS_phoEB_ptoverMET->Draw("HIST");
  H_phoEB_ptoverMET[nfile-1]->Draw("APE SAME");
  HS_phoEB_ptoverMET->GetXaxis()->SetTitle("P_{T}^{#gamma}/MET");
  HS_phoEB_ptoverMET->GetYaxis()->SetTitle("Events");
  HS_phoEB_ptoverMET->SetMaximum(1000);
  rp = new TRatioPlot(HS_phoEB_ptoverMET, H_phoEB_ptoverMET[nfile-1]);
  rp->Draw("EP");
  /*
  n = rp->GetLowerRefGraph()->GetN();
  cout << "npoints " << n << endl; 
  for(Int_t i=0; i<n; i++){
    rp->GetLowerRefGraph()->GetPoint(i, ax[i], ay[i]);
    ay[i]= 1/ay[i];
    rp->GetLowerRefGraph()->SetPoint(i, ax[i], ay[i]);
  }
  */
  rp->GetLowerRefGraph()->SetMarkerStyle(8);
  rp->GetLowerRefGraph()->SetMarkerColor(kBlack);
  rp->GetLowerRefGraph()->SetMarkerSize(0.8);
  rp->GetLowerRefYaxis()->SetTitle("MC/Data");
  rp->GetLowerRefYaxis()->SetRangeUser(-0.1, 2.5);
  rp->SetSeparationMargin(0);
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

  HS_phoEB_ptoverjetpt->Draw("HIST");
  H_phoEB_ptoverjetpt[nfile-1]->Draw("APE SAME");
  HS_phoEB_ptoverjetpt->GetXaxis()->SetTitle("P_{T}^{#gamma}/P_{T}^{jet}");
  HS_phoEB_ptoverjetpt->GetYaxis()->SetTitle("Events");
  HS_phoEB_ptoverjetpt->SetMaximum(1000);
  rp = new TRatioPlot(HS_phoEB_ptoverjetpt, H_phoEB_ptoverjetpt[nfile-1]);
  rp->Draw("EP");
  /*
  n = rp->GetLowerRefGraph()->GetN();
  cout << "npoints " << n << endl; 
  for(Int_t i=0; i<n; i++){
    rp->GetLowerRefGraph()->GetPoint(i, ax[i], ay[i]);
    ay[i]= 1/ay[i];
    rp->GetLowerRefGraph()->SetPoint(i, ax[i], ay[i]);
  }
  */
  rp->GetLowerRefGraph()->SetMarkerStyle(8);
  rp->GetLowerRefGraph()->SetMarkerColor(kBlack);
  rp->GetLowerRefGraph()->SetMarkerSize(0.8);
  rp->GetLowerRefYaxis()->SetTitle("MC/Data");
  rp->GetLowerRefYaxis()->SetRangeUser(-0.1, 2.5);
  rp->SetSeparationMargin(0);
  lhs->Clear();
  lhs->AddEntry(H_phoEB_ptoverjetpt[0], "#gamma+jet", "F");
  lhs->AddEntry(H_phoEB_ptoverjetpt[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_phoEB_ptoverjetpt[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_phoEB_ptoverjetpt[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_phoEB_ptoverjetpt[4], "DATA", "PL");
  lhs->Draw("SAME");
  //HS_phoEB_ptoverjetpt->GetYaxis()->SetRangeUser(0.01, 100);
  title = HS_phoEB_ptoverjetpt->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title)); 

  //HS_phoEB_ptMid->SetMaximum(100000);
  HS_phoEB_ptMid->Draw("HIST");
  H_phoEB_ptMid[nfile-1]->Draw("AP SAME");
  HS_phoEB_ptMid->GetXaxis()->SetTitle("P_{T}^{#gamma}");
  HS_phoEB_ptMid->GetYaxis()->SetTitle("Events");
  HS_phoEB_ptMid->GetXaxis()->SetRangeUser(100, 900);
  rp = new TRatioPlot(HS_phoEB_ptMid, H_phoEB_ptMid[nfile-1]);
  rp->Draw("EP");
  /*
  n = rp->GetLowerRefGraph()->GetN();
  cout << "npoints " << n << endl; 
  for(Int_t i=0; i<n; i++){
    rp->GetLowerRefGraph()->GetPoint(i, ax[i], ay[i]);
    ay[i]= 1/ay[i];
    rp->GetLowerRefGraph()->SetPoint(i, ax[i], ay[i]);
  }
  */
  rp->GetLowerRefGraph()->SetMarkerStyle(8);
  rp->GetLowerRefGraph()->SetMarkerColor(kBlack);
  rp->GetLowerRefGraph()->SetMarkerSize(0.8);
  rp->GetLowerRefYaxis()->SetTitle("MC/Data");
  rp->GetLowerRefYaxis()->SetRangeUser(-0.1, 2.5);
  rp->SetSeparationMargin(0);
  lhs->Clear();
  lhs->AddEntry(H_phoEB_ptMid[0], "#gamma+jet", "F");
  lhs->AddEntry(H_phoEB_ptMid[1], "Wg#rightarrowl#nug", "F");
  lhs->AddEntry(H_phoEB_ptMid[2], "Zg#rightarrowllg", "F");
  lhs->AddEntry(H_phoEB_ptMid[3], "Zg#rightarrow#nu#nug", "F");
  lhs->AddEntry(H_phoEB_ptMid[4], "DATA", "PL");
  lhs->Draw("SAME");
  title = HS_phoEB_ptMid->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  HS_phoEB_ptcut->Draw("HIST");
  H_phoEB_ptcut[nfile-1]->Draw("AP SAME");
  HS_phoEB_ptcut->GetXaxis()->SetTitle("P_{T}^{#gamma}");
  HS_phoEB_ptcut->GetYaxis()->SetTitle("Events");
  HS_phoEB_ptcut->GetXaxis()->SetRangeUser(100, 900);
  HS_phoEB_ptcut->SetMinimum(0.0001);
  rp = new TRatioPlot(HS_phoEB_ptcut, H_phoEB_ptcut[nfile-1]);
  rp->Draw("EP");
  /*
  n = rp->GetLowerRefGraph()->GetN();
  cout << "npoints " << n << endl; 
  for(Int_t i=0; i<n; i++){
    rp->GetLowerRefGraph()->GetPoint(i, ax[i], ay[i]);
    ay[i]= 1/ay[i];
    rp->GetLowerRefGraph()->SetPoint(i, ax[i], ay[i]);
  }
  */
  rp->GetLowerRefGraph()->SetMarkerStyle(8);
  rp->GetLowerRefGraph()->SetMarkerColor(kBlack);
  rp->GetLowerRefGraph()->SetMarkerSize(0.8);
  rp->GetLowerRefYaxis()->SetTitle("MC/Data");
  rp->GetLowerRefYaxis()->SetRangeUser(-0.1, 2.5);
  rp->SetSeparationMargin(0);
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
