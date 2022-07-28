#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;

#define nfile 5
#define nCat 2
#define nhisto 22
#define ncolor 10

TH1F* ratioplot(THStack* st, TH1F* h1){

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
  ratio->SetMaximum(50.4);
  ratio->Sumw2();
  ratio->Divide(tmpHist);
  
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerSize(0.8);
  ratio->Draw("EP");
  
  ratio->SetTitle("");
  ratio->GetYaxis()->SetTitle("Sig/Bkg");
  ratio->GetYaxis()->SetTitleFont(42);
  ratio->GetYaxis()->SetTitleSize(0.1);
  ratio->GetYaxis()->SetTitleOffset(0.4);
  ratio->GetYaxis()->SetLabelFont(42);
  ratio->GetYaxis()->SetLabelOffset(0.008);
  ratio->GetYaxis()->SetLabelSize(0.1);
  ratio->GetYaxis()->SetTickSize(0.032);
  
  ratio->GetXaxis()->SetTitleFont(42);
  ratio->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetTitleOffset(1.1);
  ratio->GetXaxis()->SetLabelFont(42);
  ratio->GetXaxis()->SetLabelOffset(0.03);
  ratio->GetXaxis()->SetLabelSize(0.1);
  ratio->GetXaxis()->SetTickSize(0.1);
  ratio->GetXaxis()->ChangeLabel(-1, -1, 0, -1, -1, -1, "");

  return ratio;
}

void xDrawChSoverB(Int_t year){
  TString rootname[10];
  TFile *fopen;

  Float_t entries = 1.0;
  Float_t outentries = 0.;
  Float_t scale[nfile]  = {0};
  
  TH1F *H_Events[nfile];
  TH1F *HSumofGenW[nfile];

  Float_t lumi16 = 36.33;
  Float_t lumi17 = 41.48;
  Float_t lumi18 = 59.83;
  Float_t lumi = 1.0;
  if(year==2016){
    lumi = lumi16;
  }
  else if(year==2017){
    lumi = lumi17;
  }
  else if(year==2018){
    lumi = lumi18;
  }
  Float_t xsec[10] = {1, 192.3, 123.3, 750.5, 0.296};//[GJet, WJet, ZJet, Wg, Zg, TTJet, Znng]

  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/GJet/sumGenW.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Wg_aMCatNLO/sumGenW.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Zg_aMCatNLO/sumGenW.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_TT_aMCatNLO/sumGenW.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/sumGenW.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/GJet/sumGenW.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Wg_aMCatNLO/sumGenW.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Zg_aMCatNLO/sumGenW.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_TT_aMCatNLO/sumGenW.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/sumGenW.root";
  }

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
  
    H_Events[i] = (TH1F*)fopen->Get("ggNtuplizer/hEvents");
    //HSumofGenW[i] = (TH1F*)fopen->Get("hSumofGenW");
    HSumofGenW[i] = (TH1F*)fopen->Get("ggNtuplizer/hSumGenWeight");
    entries = 1.0;
    //entries = H_Events[i]->GetBinContent(1);
    entries = HSumofGenW[i]->GetBinContent(1);
    outentries = xsec[i]*1000*lumi;
    scale[i] = 1;
    scale[i] = fabs(outentries/entries);
    cout << "print " << entries << " " << outentries << " " << scale[i]<< endl;
    
    //if(i==nfile-1 || i<3) scale = 1.0;
    if(i<1) scale[i] = 1.0;
  }

  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/GJet/output_merge_GJetchIso.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Wg_aMCatNLO/output_ChIso_ggtree.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Zg_aMCatNLO/output_ChIso_ggtree.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_TT_aMCatNLO/output_ChIso_ggtree.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/job_summer16_Znunug_pt130/output_ChIso_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/GJet/output_merge_GJetchIso.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Wg_aMCatNLO/output_ChIso_ggtree.root";
    rootname[2] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Zg_aMCatNLO/output_ChIso_ggtree.root";
    rootname[3] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_TT_aMCatNLO/output_ChIso_ggtree.root";
    rootname[4] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/job_fall17_Znunug_pt130/output_ChIso_ggtree.root";
  }

  //string hexcolor[ncolor] = {"#fff176", "#ff9e00", "#00FFFF", "#FFCCCC", "#67ccc1", "#0066ff", "#4caf50" ,"#03a9f4", "#ff3360"};
  string hexcolor[ncolor] = {"#fff176", "#03a9f4", "#ff3360", "#ff9e00", "#67ccc1", "#0066ff", "#4caf50" ,"#03a9f4", "#ff3360"};
  TColor *color[ncolor];
  Int_t cnum[ncolor];
   
  for(Int_t j=0; j<ncolor; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  } 

  TCanvas *c1 = new TCanvas("c1");
  //c1->Divide(1, 2);
  TTree *t;

  TH1F *H_phoEB_chIsocut[nfile][nCat][nhisto];
  TH1F *H_phoEB_chworstcut[nfile][nCat][nhisto];
  
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    if(i==0) scale[i] = 1.0;

    for(Int_t jj=0; jj<nCat; jj++){
      for(Int_t kk=0; kk<nhisto; kk++){
	H_phoEB_chIsocut[i][jj][kk] = (TH1F*)fopen->Get(Form("chIso/h_chIso_ptcut_%i_%ibin", jj, kk));
	H_phoEB_chworstcut[i][jj][kk] = (TH1F*)fopen->Get(Form("chworst/h_chworst_ptcut_%i_%ibin", jj, kk));

	H_phoEB_chIsocut[i][jj][kk]->Scale(scale[i]);
	H_phoEB_chworstcut[i][jj][kk]->Scale(scale[i]);

	H_phoEB_chIsocut[i][jj][kk]->SetLineColor(1);
	H_phoEB_chworstcut[i][jj][kk]->SetLineColor(1);
	
	if(i<nfile-1){
	  H_phoEB_chIsocut[i][jj][kk]->SetFillColor(cnum[i]);
	  H_phoEB_chworstcut[i][jj][kk]->SetFillColor(cnum[i]);
	}

	if(i==nfile-1){
	  H_phoEB_chIsocut[i][jj][kk]->SetLineColor(cnum[i]);
	  H_phoEB_chworstcut[i][jj][kk]->SetLineColor(cnum[i]);

	  H_phoEB_chIsocut[i][jj][kk]->SetLineWidth(2);
	  H_phoEB_chworstcut[i][jj][kk]->SetLineWidth(2);
	}
	
      }
    }//histos
    
  }

  THStack *HS_phoEB_chIsocut[nCat][nhisto];
  THStack *HS_phoEB_chworstcut[nCat][nhisto];

  for(Int_t jj=0; jj<nCat; jj++){
    for(Int_t kk=0; kk<nhisto; kk++){
      HS_phoEB_chIsocut[jj][kk] = new THStack(Form("HS_phoEB_chIsocut_%i_%ibin", jj, kk), "HS_phoEB_chIsocut");
      HS_phoEB_chworstcut[jj][kk] = new THStack(Form("HS_phoEB_chworst_%i_%ibin", jj, kk), "HS_phoEB_chworstcut");
    }
  }

  for(Int_t i=0; i<nfile-1; i++){
    for(Int_t jj=0; jj<nCat; jj++){
      for(Int_t kk=0; kk<nhisto; kk++){
	HS_phoEB_chIsocut[jj][kk]->Add(H_phoEB_chIsocut[i][jj][kk]);
	HS_phoEB_chworstcut[jj][kk]->Add(H_phoEB_chworstcut[i][jj][kk]);
      }
    }
  }

  TLegend *lhs = new TLegend(0.63,0.63, 0.75, 0.90);
  lhs->SetTextSize(0.04);
  lhs->SetFillStyle(0);
  TString ltext[10] = {"#gamma+jet", "Wg#rightarrowl#nug", "Zg#rightarrowllg", "ttJets", "Zg#rightarrow#nu#nug"};

  TH1F *hratio;
  Double_t x[3] = {-2000, 0.0, 2000};
  Double_t y[3] = {1, 1, 1};
  TGraph *tg = new TGraph(3, x, y);
  tg->SetLineColor(kRed);
  tg->SetLineWidth(1);
  tg->SetLineStyle(8);

  TPad *pad1 = new TPad("pad1", "", 0., 0.3, 1., 1.);
  //pad1->SetTopMargin(0.03);
  pad1->SetBottomMargin(0);
  pad1->SetLogy(1);
  pad1->Draw();
  
  TPad *pad2 = new TPad("pad2", "", 0., 0.02, 1., 0.3);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.3);
  pad2->Draw();

  for(Int_t jj=0; jj<nCat; jj++){
    for(Int_t kk=0; kk<nhisto; kk++){
      
      pad1->cd();
      HS_phoEB_chIsocut[jj][kk]->Draw("HIST");
      H_phoEB_chIsocut[nfile-1][jj][kk]->Draw("HISTSAME");
      HS_phoEB_chIsocut[jj][kk]->GetYaxis()->SetTitle("Events");
      HS_phoEB_chIsocut[jj][kk]->GetXaxis()->SetTitle("");
      HS_phoEB_chIsocut[jj][kk]->GetXaxis()->SetLabelSize(0);
      HS_phoEB_chIsocut[jj][kk]->SetMaximum(8000);
      HS_phoEB_chIsocut[jj][kk]->SetMinimum(0.1);
      lhs->Clear();
      for(Int_t ifile=0; ifile<nfile; ifile++){
	if(ifile < nfile-1) lhs->AddEntry(H_phoEB_chIsocut[ifile][jj][kk], ltext[ifile], "F");
	if(ifile == nfile-1) lhs->AddEntry(H_phoEB_chIsocut[ifile][jj][kk], ltext[ifile], "L");
      }
      lhs->Draw("SAME");
      title = HS_phoEB_chIsocut[jj][kk]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_phoEB_chIsocut[jj][kk], H_phoEB_chIsocut[nfile-1][jj][kk]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle("P_{T}^{#gamma}[GeV]");
      hratio->Draw("EP");
      tg->Draw("LSAME");
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));
      
      pad1->cd();
      HS_phoEB_chworstcut[jj][kk]->Draw("HIST");
      H_phoEB_chworstcut[nfile-1][jj][kk]->Draw("HISTSAME");
      HS_phoEB_chworstcut[jj][kk]->GetYaxis()->SetTitle("Events");
      HS_phoEB_chworstcut[jj][kk]->GetXaxis()->SetTitle("");
      HS_phoEB_chworstcut[jj][kk]->GetXaxis()->SetLabelSize(0);
      HS_phoEB_chworstcut[jj][kk]->SetMaximum(1000);
      HS_phoEB_chworstcut[jj][kk]->SetMinimum(0.1);
      lhs->Clear();
      for(Int_t ifile=0; ifile<nfile; ifile++){
	if(ifile < nfile-1) lhs->AddEntry(H_phoEB_chworstcut[ifile][jj][kk], ltext[ifile], "F");
	if(ifile == nfile-1) lhs->AddEntry(H_phoEB_chworstcut[ifile][jj][kk], ltext[ifile], "L");
      }
      lhs->Draw("SAME");
      title = HS_phoEB_chworstcut[jj][kk]->GetName();
      pad2->cd();
      hratio = ratioplot(HS_phoEB_chworstcut[jj][kk], H_phoEB_chworstcut[nfile-1][jj][kk]);
      hratio->SetTitle("");
      hratio->GetXaxis()->SetTitle("P_{T}^{#gamma}[GeV]");
      hratio->Draw("EP");
      tg->Draw("LSAME");
      c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    }
  }
  
}
