#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
#include "./Drawlumi/CMS_lumi.C"

using namespace std;
#define nfile 5
#define nhisto 2
#define nGJet 8
#define ncolor 10

#define eqevents 1000000

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
  ratio->SetMaximum(3.4);
  ratio->Sumw2();
  ratio->Divide(tmpHist);
  
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerSize(0.8);
  ratio->Draw("EP");
  
  ratio->SetTitle("");
  ratio->GetYaxis()->SetTitle("Data/MC");
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

void xDrawStack(Int_t year){
 TString rootname[5] = {
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/output_merge.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_Wg_aMCatNLO/210707_180617/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_Zg_aMCatNLO/210708_013510/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/mc/summer16_job_summer16_Znunug_pt130/210707_175756/output_ggtree.root",
			 "/home/judy/ntuhep/GMET/output_file/summer16/data/output_ggtree.root"
  };//[0,1,2,3][GJet,WG, ZG,ZNuNu]

  //Float_t mclumi[5] = {0.31,311.6,6750,989.7};//(fb-1) [0,1,2,3,4][GJet,WG,ZG,ZNuNu]
  //Float_t mclumi[5] = {311.6,6750,989.7};//(fb-1) [1,2,3,4][WG,ZG,ZNuNu]
  Float_t gjetXsec[10] = {9319, 9155, 2323, 2314, 278.5, 271.8, 93.85, 94.7};
  Float_t lumi16 = 36.33;
  Float_t Xsec = 2323.0;

  Float_t xsec[10] = {1125, 192.3, 123.3, 0.19};
  Float_t filter[10] = {1., 1., 1., 1.};
  Float_t kfactor[10] = {1., 1., 1., 1.557};
  //color name
  //{"#1C2A60","#7FB5F5","#2F81A3","#FA6D5F","#91A81E","#DCF563", "#1C6040","#EFE2C9"}
  //{""}
  //string hexcolor[ncolor] = {"#7FB5F5","#EFE2C9","#1C2A60","#96DC9E","#1C6040", "#FA6D5F","#DCF563", "#2F81A3","#91A81E"};
  string hexcolor[ncolor] = {"#fff176", "#03a9f4","#ff3360", "#67ccc1","#1C2A60", "#DCDB06", "#FA6D5F", "#F0F5E1","#DCF563", "#2F81A3" };
  TColor *color[ncolor];
  Int_t cnum[ncolor];
   
  for(Int_t j=0; j<8; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    cnum[j] = color[j]->GetColor(chararray);
  } 

  
  
  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");
  //c1->Divide(1, 2);
  TTree *t;
  
  Int_t entries;
  Float_t outentries = 0.;
  Float_t scale = 0.;

  TH1F *H_Events[10];

  TH1F *H_MET_cut[nfile][nhisto];
  TH1F *H_dphoMETPhi_cut[nfile][nhisto];
  TH1F *H_njet_cut[nfile][nhisto];
  TH1F *H_jetpt_cut[nfile][nhisto];
  TH1F *H_djetMETPhi_cut[nfile][nhisto];
  TH1F *H_nvtx_cut[nfile][nhisto];

  TH1F *H_phoEB_ptcut[nfile][nhisto];
  TH1F *H_phoEB_ptoverMET[nfile][nhisto];
  TH1F *H_phoEB_ptoverjetpt[nfile][nhisto];
  
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";
  
  for(Int_t i=0; i<nfile; i++){
    fopen = new TFile(rootname[i]);
    for(Int_t jj=0; jj<nhisto; jj++){
      if(i<nfile-1){
	H_Events[i] = (TH1F*)fopen->Get("hEvents");
	entries = H_Events[i]->GetBinContent(1);
	outentries = xsec[i]*kfactor[i]*1000*lumi16*filter[i];
	scale = outentries/entries;
	cout << "print " << entries << " " << outentries << " " << scale<< endl;
	//if(i==4) scale*=100000;
      }
      if(i==4) scale = 1.0;
      
      H_MET_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", jj));
      H_dphoMETPhi_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_dphoMETPhi_cut_%i", jj));
      H_njet_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_njet_cut_%i", jj));
      H_jetpt_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_jetpt_cut_%i", jj));
      H_djetMETPhi_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_djetMETPhi_cut_%i", jj));
      H_nvtx_cut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_nvtx_cut_%i", jj));
    
      H_phoEB_ptcut[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptcut_%i", jj));
      H_phoEB_ptoverMET[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverMET_cut_%i", jj));
      H_phoEB_ptoverjetpt[i][jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_phoEB_ptoverjetpt_%i", jj));
    
      //if(i< 4){
      H_MET_cut[i][jj]->Scale(scale);
      H_dphoMETPhi_cut[i][jj]->Scale(scale);
      H_njet_cut[i][jj]->Scale(scale);
      H_jetpt_cut[i][jj]->Scale(scale);
      H_djetMETPhi_cut[i][jj]->Scale(scale);
      H_nvtx_cut[i][jj]->Scale(scale);
      
      H_phoEB_ptcut[i][jj]->Scale(scale);
      H_phoEB_ptoverMET[i][jj]->Scale(scale);
      H_phoEB_ptoverjetpt[i][jj]->Scale(scale);
      //}
    
      //fopen->Close();
      if(i<4){
	H_MET_cut[i][jj]->SetFillColor(cnum[i]);
	H_dphoMETPhi_cut[i][jj]->SetFillColor(cnum[i]);
	H_njet_cut[i][jj]->SetFillColor(cnum[i]);
	H_jetpt_cut[i][jj]->SetFillColor(cnum[i]);
	H_djetMETPhi_cut[i][jj]->SetFillColor(cnum[i]);
	H_nvtx_cut[i][jj]->SetFillColor(cnum[i]);
    
	H_phoEB_ptcut[i][jj]->SetFillColor(cnum[i]);
	H_phoEB_ptoverMET[i][jj]->SetFillColor(cnum[i]);
	H_phoEB_ptoverjetpt[i][jj]->SetFillColor(cnum[i]);
      }
      /*
	H_MET_cut[i][jj]->SetLineColor(cnum[i]);
	H_dphoMETPhi_cut[i][jj]->SetLineColor(cnum[i]);
	H_njet_cut[i][jj]->SetLineColor(cnum[i]);
	H_jetpt_cut[i][jj]->SetLineColor(cnum[i]);
	H_djetMETPhi_cut[i][jj]->SetLineColor(cnum[i]);
	H_nvtx_cut[i][jj]->SetLineColor(cnum[i]);
    
	H_phoEB_ptMid[i][jj]->SetLineColor(cnum[i]);
	H_phoEB_ptcut[i][jj]->SetLineColor(cnum[i]);
	H_phoEB_ptoverMET[i][jj]->SetLineColor(cnum[i]);
	H_phoEB_ptoverjetpt[i][jj]->SetLineColor(cnum[i]);
      */
      if(i==4){
	H_MET_cut[i][jj]->SetLineWidth(2);
	H_dphoMETPhi_cut[i][jj]->SetLineWidth(2);
	H_njet_cut[i][jj]->SetLineWidth(2);
	H_jetpt_cut[i][jj]->SetLineWidth(2);
	H_djetMETPhi_cut[i][jj]->SetLineWidth(2);
	H_nvtx_cut[i][jj]->SetLineWidth(2);
	H_phoEB_ptcut[i][jj]->SetLineWidth(2);
	H_phoEB_ptoverMET[i][jj]->SetLineWidth(2);
	H_phoEB_ptoverjetpt[i][jj]->SetLineWidth(2);

	H_MET_cut[i][jj]->SetMarkerStyle(8);
	H_dphoMETPhi_cut[i][jj]->SetMarkerStyle(8);
	H_njet_cut[i][jj]->SetMarkerStyle(8);
	H_jetpt_cut[i][jj]->SetMarkerStyle(8);
	H_djetMETPhi_cut[i][jj]->SetMarkerStyle(8);
	H_nvtx_cut[i][jj]->SetMarkerStyle(8);
	H_phoEB_ptcut[i][jj]->SetMarkerStyle(8);
	H_phoEB_ptoverMET[i][jj]->SetMarkerStyle(8);
	H_phoEB_ptoverjetpt[i][jj]->SetMarkerStyle(8);
      
	H_MET_cut[i][jj]->SetMarkerSize(0.9);
	H_dphoMETPhi_cut[i][jj]->SetMarkerSize(0.9);
	H_njet_cut[i][jj]->SetMarkerSize(0.9);
	H_jetpt_cut[i][jj]->SetMarkerSize(0.9);
	H_djetMETPhi_cut[i][jj]->SetMarkerSize(0.9);
	H_nvtx_cut[i][jj]->SetMarkerSize(0.9);
	H_phoEB_ptcut[i][jj]->SetMarkerSize(0.9);
	H_phoEB_ptoverMET[i][jj]->SetMarkerSize(0.9);
	H_phoEB_ptoverjetpt[i][jj]->SetMarkerSize(0.9);

	H_MET_cut[i][jj]->SetLineColor(kBlack);
	H_dphoMETPhi_cut[i][jj]->SetLineColor(kBlack);
	H_njet_cut[i][jj]->SetLineColor(kBlack);
	H_jetpt_cut[i][jj]->SetLineColor(kBlack);
	H_djetMETPhi_cut[i][jj]->SetLineColor(kBlack);
	H_nvtx_cut[i][jj]->SetLineColor(kBlack);
	H_phoEB_ptcut[i][jj]->SetLineColor(kBlack);
	H_phoEB_ptoverMET[i][jj]->SetLineColor(kBlack);
	H_phoEB_ptoverjetpt[i][jj]->SetLineColor(kBlack);
      
      }
    }
  }

  THStack *HS_MET_cut[2];
  THStack *HS_dphoMETPhi_cut[2];
  THStack *HS_njet_cut[2];
  THStack *HS_jetpt_cut[2];
  THStack *HS_djetMETPhi_cut[2];
  THStack *HS_nvtx_cut[2];

  THStack *HS_phoEB_ptcut[2];
  THStack *HS_phoEB_ptoverMET[2];
  THStack *HS_phoEB_ptoverjetpt[2];
    
  for(Int_t jj=0; jj<2; jj++){					       
    HS_MET_cut[jj] = new THStack(Form("HS_MET_cut_%i", jj), "HS_MET_cut");
    HS_dphoMETPhi_cut[jj] = new THStack(Form("HS_dphoMETPhi_cut_%i", jj), "HS_dphoMETPhi_cut");
    HS_njet_cut[jj] = new THStack(Form("HS_njet_cut_%i", jj), "HS_njet_cut");
    HS_jetpt_cut[jj] = new THStack(Form("HS_jetpt_cut_%i", jj), "HS_jetpt_cut");
    HS_djetMETPhi_cut[jj] = new THStack(Form("HS_djetMETPhi_cut_%i", jj), "HS_djetMETPhi_cut");
    HS_nvtx_cut[jj] = new THStack(Form("HS_nvtx_cut_%i", jj), "HS_nvtx_cut");

    HS_phoEB_ptcut[jj] = new THStack(Form("HS_phoEB_ptcut_%i", jj), "HS_phoEB_ptcut");
    HS_phoEB_ptoverMET[jj] = new THStack(Form("HS_phoEB_ptoverMET_cut_%i", jj), "HS_phoEB_ptoverMET");
    HS_phoEB_ptoverjetpt[jj] = new THStack(Form("HS_phoEB_ptoverjetpt_%i", jj), "HS_phoEB_ptoverjetpt");
  }
					       
  
  for(Int_t i=0; i<nfile-1; i++){
    //if(i==4)continue;
    for(Int_t jj=0; jj<2; jj++){
      HS_MET_cut[jj]->Add(H_MET_cut[i][jj]);
      HS_njet_cut[jj]->Add(H_njet_cut[i][jj]);
      HS_jetpt_cut[jj]->Add(H_jetpt_cut[i][jj]);
      HS_phoEB_ptcut[jj]->Add(H_phoEB_ptcut[i][jj]);
      HS_nvtx_cut[jj]->Add(H_nvtx_cut[i][jj]);
    
      c1->SetLogy();
      c1->Update();
    
      HS_dphoMETPhi_cut[jj]->Add(H_dphoMETPhi_cut[i][jj]);
      HS_djetMETPhi_cut[jj]->Add(H_djetMETPhi_cut[i][jj]);
      HS_phoEB_ptoverMET[jj]->Add(H_phoEB_ptoverMET[i][jj]);    
      HS_phoEB_ptoverjetpt[jj]->Add(H_phoEB_ptoverjetpt[i][jj]);
    }
  }

  TLegend *lhs = new TLegend(0.65,0.72, 0.75, 0.88);
  lhs->SetTextSize(0.04);
  lhs->SetFillStyle(0);
  
  //HS_dphoMETPhi->Draw("HIST");
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

  writeExtraText = true;
  extraText  = "Preliminary";
  
  for(Int_t jj=0; jj<2; jj++){
    pad1->cd();
    HS_MET_cut[jj]->Draw("HIST");
    H_MET_cut[nfile-1][jj]->Draw("SAME");
    HS_MET_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_MET_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_MET_cut[jj]->GetXaxis()->SetTitle("");
    HS_MET_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_MET_cut[jj]->SetMaximum(1000);
    HS_MET_cut[jj]->SetMinimum(1);
    if(jj==1){HS_MET_cut[jj]->SetMaximum(1); HS_MET_cut[jj]->SetMinimum(0.001);}
    lhs->Clear();
    lhs->AddEntry(H_MET_cut[0][jj], "#gamma+jet", "F");
    lhs->AddEntry(H_MET_cut[1][jj], "Wg#rightarrowl#nug", "F");
    lhs->AddEntry(H_MET_cut[2][jj], "Zg#rightarrowllg", "F");
    lhs->AddEntry(H_MET_cut[3][jj], "Zg#rightarrow#nu#nug", "F");
    lhs->AddEntry(H_MET_cut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_MET_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_MET_cut[jj], H_MET_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("MET");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, year, 0);
    c1->Update();
    c1->RedrawAxis();
    //c1->GetFrame()->Draw();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();  
    HS_dphoMETPhi_cut[jj]->Draw("HIST");
    H_dphoMETPhi_cut[nfile-1][jj]->Draw("SAME");
    HS_dphoMETPhi_cut[jj]->SetMinimum(1);
    HS_dphoMETPhi_cut[jj]->SetMaximum(500);
    if(jj==1){HS_dphoMETPhi_cut[jj]->SetMaximum(1); HS_dphoMETPhi_cut[jj]->SetMinimum(0.001);}
    HS_dphoMETPhi_cut[jj]->GetXaxis()->SetTitle("");
    HS_dphoMETPhi_cut[jj]->GetYaxis()->SetTitle("Events");
    //HS_dphoMETPhi_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    lhs->Clear();
    lhs->AddEntry(H_dphoMETPhi_cut[0][jj], "#gamma+jet", "F");
    lhs->AddEntry(H_dphoMETPhi_cut[1][jj], "Wg#rightarrowl#nug", "F");
    lhs->AddEntry(H_dphoMETPhi_cut[2][jj], "Zg#rightarrowllg", "F");
    lhs->AddEntry(H_dphoMETPhi_cut[3][jj], "Zg#rightarrow#nu#nug", "F");
    lhs->AddEntry(H_dphoMETPhi_cut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_dphoMETPhi_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_dphoMETPhi_cut[jj], H_dphoMETPhi_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#Delta#phi(#gamma,MET)");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, year, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_njet_cut[jj]->Draw("HIST");
    H_njet_cut[nfile-1][jj]->Draw("SAME");
    HS_njet_cut[jj]->GetXaxis()->SetTitle("");
    HS_njet_cut[jj]->GetYaxis()->SetTitle("Events");
    //HS_njet_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_njet_cut[jj]->SetMaximum(5000);
    HS_njet_cut[jj]->SetMinimum(10);
    if(jj==1){HS_njet_cut[jj]->SetMaximum(10); HS_njet_cut[jj]->SetMinimum(0.01);}
    lhs->Clear();
    lhs->AddEntry(H_njet_cut[0][jj], "#gamma+jet", "F");
    lhs->AddEntry(H_njet_cut[1][jj], "Wg#rightarrowl#nug", "F");
    lhs->AddEntry(H_njet_cut[2][jj], "Zg#rightarrowllg", "F");
    lhs->AddEntry(H_njet_cut[3][jj], "Zg#rightarrow#nu#nug", "F");
    lhs->AddEntry(H_njet_cut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_njet_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_njet_cut[jj], H_njet_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("njet");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, year, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_jetpt_cut[jj]->Draw("HIST");
    H_jetpt_cut[nfile-1][jj]->Draw("SAME");
    HS_jetpt_cut[jj]->GetXaxis()->SetTitle("");
    HS_jetpt_cut[jj]->GetYaxis()->SetTitle("Events");
    HS_jetpt_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_jetpt_cut[jj]->SetMaximum(500);
    HS_jetpt_cut[jj]->SetMinimum(0.1);
    lhs->Clear();
    lhs->AddEntry(H_jetpt_cut[0][jj], "#gamma+jet", "F");
    lhs->AddEntry(H_jetpt_cut[1][jj], "Wg#rightarrowl#nug", "F");
    lhs->AddEntry(H_jetpt_cut[2][jj], "Zg#rightarrowllg", "F");
    lhs->AddEntry(H_jetpt_cut[3][jj], "Zg#rightarrow#nu#nug", "F");
    lhs->AddEntry(H_jetpt_cut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_jetpt_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_jetpt_cut[jj], H_jetpt_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("P_{T}^{jet}");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, year, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_djetMETPhi_cut[jj]->Draw("HIST");
    H_djetMETPhi_cut[nfile-1][jj]->Draw("SAME");
    HS_djetMETPhi_cut[jj]->GetXaxis()->SetTitle("");
    HS_djetMETPhi_cut[jj]->GetYaxis()->SetTitle("Events");
    // HS_djetMETPhi_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_djetMETPhi_cut[jj]->SetMaximum(1000);
    HS_djetMETPhi_cut[jj]->SetMinimum(0.5);
    lhs->Clear();
    lhs->AddEntry(H_djetMETPhi_cut[0][jj], "#gamma+jet", "F");
    lhs->AddEntry(H_djetMETPhi_cut[1][jj], "Wg#rightarrowl#nug", "F");
    lhs->AddEntry(H_djetMETPhi_cut[2][jj], "Zg#rightarrowllg", "F");
    lhs->AddEntry(H_djetMETPhi_cut[3][jj], "Zg#rightarrow#nu#nug", "F");
    lhs->AddEntry(H_djetMETPhi_cut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_djetMETPhi_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_djetMETPhi_cut[jj], H_djetMETPhi_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("#Delta#phi(jet, MET)");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, year, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
    //HS_dphoMETPhi->Draw("HIST");

    pad1->cd();
    HS_nvtx_cut[jj]->Draw("HIST");
    H_nvtx_cut[nfile-1][jj]->Draw("SAME");
    HS_nvtx_cut[jj]->GetYaxis()->SetTitle("Events");
    //HS_nvtx_cut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_nvtx_cut[jj]->GetXaxis()->SetTitle("");
    HS_nvtx_cut[jj]->GetXaxis()->SetLabelSize(0);
    HS_nvtx_cut[jj]->SetMaximum(1000);
    HS_nvtx_cut[jj]->SetMinimum(1);
    if(jj==1){HS_nvtx_cut[jj]->SetMaximum(1); HS_nvtx_cut[jj]->SetMinimum(0.001);}
    lhs->Clear();
    lhs->AddEntry(H_nvtx_cut[0][jj], "#gamma+jet", "F");
    lhs->AddEntry(H_nvtx_cut[1][jj], "Wg#rightarrowl#nug", "F");
    lhs->AddEntry(H_nvtx_cut[2][jj], "Zg#rightarrowllg", "F");
    lhs->AddEntry(H_nvtx_cut[3][jj], "Zg#rightarrow#nu#nug", "F");
    lhs->AddEntry(H_nvtx_cut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_nvtx_cut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_nvtx_cut[jj], H_nvtx_cut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("nVtx");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, year, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_phoEB_ptoverjetpt[jj]->Draw("HIST");
    H_phoEB_ptoverjetpt[nfile-1][jj]->Draw("APE SAME");
    HS_phoEB_ptoverjetpt[jj]->GetXaxis()->SetTitle("");
    HS_phoEB_ptoverjetpt[jj]->GetYaxis()->SetTitle("Events");
    HS_phoEB_ptoverjetpt[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_ptoverjetpt[jj]->SetMaximum(1000);
    HS_phoEB_ptoverjetpt[jj]->SetMinimum(0.01);
    lhs->Clear();
    lhs->AddEntry(H_phoEB_ptoverjetpt[0][jj], "#gamma+jet", "F");
    lhs->AddEntry(H_phoEB_ptoverjetpt[1][jj], "Wg#rightarrowl#nug", "F");
    lhs->AddEntry(H_phoEB_ptoverjetpt[2][jj], "Zg#rightarrowllg", "F");
    lhs->AddEntry(H_phoEB_ptoverjetpt[3][jj], "Zg#rightarrow#nu#nug", "F");
    lhs->AddEntry(H_phoEB_ptoverjetpt[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    //HS_phoEB_ptoverjetpt[jj]->GetYaxis()->SetRangeUser(0.01, 100);
    title = HS_phoEB_ptoverjetpt[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_ptoverjetpt[jj], H_phoEB_ptoverjetpt[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("P_{T}^{#gamma}/P_{T}^{jet}");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, year, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title)); 

  
    pad1->cd();
    HS_phoEB_ptcut[jj]->Draw("HIST");
    H_phoEB_ptcut[nfile-1][jj]->Draw("AP SAME");
    HS_phoEB_ptcut[jj]->GetXaxis()->SetTitle("");
    HS_phoEB_ptcut[jj]->GetYaxis()->SetTitle("Events");
    //HS_phoEB_ptcut[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_ptcut[jj]->GetXaxis()->SetRangeUser(100, 900);
    HS_phoEB_ptcut[jj]->SetMinimum(0.1);
    if(jj==1){HS_phoEB_ptcut[jj]->SetMinimum(0.001);}
    lhs->Clear();
    lhs->AddEntry(H_phoEB_ptcut[0][jj], "#gamma+jet", "F");
    lhs->AddEntry(H_phoEB_ptcut[1][jj], "Wg#rightarrowl#nug", "F");
    lhs->AddEntry(H_phoEB_ptcut[2][jj], "Zg#rightarrowllg", "F");
    lhs->AddEntry(H_phoEB_ptcut[3][jj], "Zg#rightarrow#nu#nug", "F");
    lhs->AddEntry(H_phoEB_ptcut[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    title = HS_phoEB_ptcut[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_ptcut[jj], H_phoEB_ptcut[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("P_{T}^{#gamma}");
    hratio->Draw("EP");
    hratio->GetXaxis()->SetRangeUser(100, 900);
    tg->Draw("LSAME");
    CMS_lumi(pad1, year, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));

    pad1->cd();
    HS_phoEB_ptoverMET[jj]->Draw("HIST");
    H_phoEB_ptoverMET[nfile-1][jj]->Draw("APE SAME");
    HS_phoEB_ptoverMET[jj]->GetXaxis()->SetTitle("");
    HS_phoEB_ptoverMET[jj]->GetYaxis()->SetTitle("Events");
    HS_phoEB_ptoverMET[jj]->GetYaxis()->ChangeLabel(1, -1, 0, -1, -1, -1, "");
    HS_phoEB_ptoverMET[jj]->SetMaximum(10000);
    HS_phoEB_ptoverMET[jj]->SetMinimum(0.1);
    if(jj==1){HS_phoEB_ptoverMET[jj]->SetMaximum(10); HS_phoEB_ptoverMET[jj]->SetMinimum(0.001);}
    lhs->Clear();
    lhs->AddEntry(H_phoEB_ptoverMET[0][jj], "#gamma+jet", "F");
    lhs->AddEntry(H_phoEB_ptoverMET[1][jj], "Wg#rightarrowl#nug", "F");
    lhs->AddEntry(H_phoEB_ptoverMET[2][jj], "Zg#rightarrowllg", "F");
    lhs->AddEntry(H_phoEB_ptoverMET[3][jj], "Zg#rightarrow#nu#nug", "F");
    lhs->AddEntry(H_phoEB_ptoverMET[4][jj], "DATA", "PL");
    lhs->Draw("SAME");
    //HS_phoEB_ptoverMET[jj]->GetYaxis()->SetRangeUser(0.01, 100);
    title = HS_phoEB_ptoverMET[jj]->GetName();
    pad2->cd();
    hratio = ratioplot(HS_phoEB_ptoverMET[jj], H_phoEB_ptoverMET[nfile-1][jj]);
    hratio->SetTitle("");
    hratio->GetXaxis()->SetTitle("P_{T}^{#gamma}/MET");
    hratio->Draw("EP");
    tg->Draw("LSAME");
    CMS_lumi(pad1, year, 0);
    c1->Update();
    c1->RedrawAxis();
    c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  }  
}
