#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "../Drawlumi/CMS_lumi.C"

using namespace std;

TH1F* ratioplot(TH1F* st, TH1F* h1){

  if (!st) {
       Warning("TRatioPlot", "Need only a stack");
       //return;
    }
  
  
  TH1F* tmpHist = (TH1F*)st->Clone();
  //tmpHist->Reset();
  
  TH1F *ratio = (TH1F*)h1->Clone("ratio");
  ratio->SetLineColor(kBlack);
  ratio->SetMinimum(-0.4);
  ratio->SetMaximum(2.4);
  ratio->Sumw2();
  ratio->Divide(tmpHist);
  
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerSize(0.8);
  ratio->Draw("E3 P");
  
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

void xDraw_OOTFit(Int_t year){
  TString rootname[20];
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/data/OOT_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/data/OOT_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/autumn18/data/OOT_ggtree.root";
  }

  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  TH1F* H_phoEB210_sieie;
  TH1F* H_phoEB210_SeedTime;
  TH1F* H_phoEB210_MIP;

  TH1F* H_phoEBprompt_sieie;
  TH1F* H_phoEBprompt_SeedTime;
  TH1F* H_phoEBprompt_MIP;
  
  TH1F* H_ootBH_sieie;
  TH1F* H_ootBH_SeedTime;
  TH1F* H_ootBH_MIP;

  TH1F* H_ootSP_sieie;
  TH1F* H_ootSP_SeedTime;
  TH1F* H_ootSP_MIP;

  fopen = new TFile(rootname[0]);
  
  H_phoEB210_sieie = (TH1F*)fopen->Get("h_phoEB210_sieie");
  H_phoEB210_SeedTime = (TH1F*)fopen->Get("h_phoEB210_SeedTime");
  H_phoEB210_MIP = (TH1F*)fopen->Get("h_phoEB210_MIP");

  H_phoEBprompt_sieie = (TH1F*)fopen->Get("h_phoEBprompt_sieie");
  H_phoEBprompt_SeedTime = (TH1F*)fopen->Get("h_phoEBprompt_SeedTime");
  H_phoEBprompt_MIP = (TH1F*)fopen->Get("h_phoEBprompt_MIP");
   
  H_ootBH_sieie = (TH1F*)fopen->Get("h_ootBH_sieie");
  H_ootBH_SeedTime = (TH1F*)fopen->Get("h_ootBH_SeedTime");
  H_ootBH_MIP = (TH1F*)fopen->Get("h_ootBH_MIP");
   
  H_ootSP_sieie = (TH1F*)fopen->Get("h_ootSP_sieie");
  H_ootSP_SeedTime = (TH1F*)fopen->Get("h_ootSP_SeedTime");
  H_ootSP_MIP = (TH1F*)fopen->Get("h_ootSP_MIP");


  //SetLineColor
  H_phoEB210_sieie->SetLineColor(kBlack);
  H_phoEB210_SeedTime->SetLineColor(kBlack);
  H_phoEB210_MIP->SetLineColor(kBlack);
  
  H_phoEBprompt_sieie->SetLineColor(kRed);
  H_phoEBprompt_SeedTime->SetLineColor(kRed);
  H_phoEBprompt_MIP->SetLineColor(kRed);

  H_ootBH_sieie->SetLineColor(kTeal-1);
  H_ootBH_SeedTime->SetLineColor(kTeal-1);
  H_ootBH_MIP->SetLineColor(kTeal-1);

  H_ootSP_sieie->SetLineColor(kAzure-1);
  H_ootSP_SeedTime->SetLineColor(kAzure-1);
  H_ootSP_MIP->SetLineColor(kAzure-1);

  //SetLineWidth
  H_phoEB210_sieie->SetLineWidth(2);
  H_phoEB210_SeedTime->SetLineWidth(2);
  H_phoEB210_MIP->SetLineWidth(2);
  
  H_phoEBprompt_sieie->SetLineWidth(2);
  H_phoEBprompt_SeedTime->SetLineWidth(2);
  H_phoEBprompt_MIP->SetLineWidth(2);

  H_ootBH_sieie->SetLineWidth(2);
  H_ootBH_SeedTime->SetLineWidth(2);
  H_ootBH_MIP->SetLineWidth(2);

  H_ootSP_sieie->SetLineWidth(2);
  H_ootSP_SeedTime->SetLineWidth(2);
  H_ootSP_MIP->SetLineWidth(2);
  
  //SetMarkerStyle
  H_phoEB210_sieie->SetMarkerStyle(8);
  H_phoEB210_SeedTime->SetMarkerStyle(8);
  H_phoEB210_MIP->SetMarkerStyle(8);
  
  H_phoEBprompt_sieie->SetMarkerStyle(8);
  H_phoEBprompt_SeedTime->SetMarkerStyle(8);
  H_phoEBprompt_MIP->SetMarkerStyle(8);

  H_ootBH_sieie->SetMarkerStyle(8);
  H_ootBH_SeedTime->SetMarkerStyle(8);
  H_ootBH_MIP->SetMarkerStyle(8);

  H_ootSP_sieie->SetMarkerStyle(8);
  H_ootSP_SeedTime->SetMarkerStyle(8);
  H_ootSP_MIP->SetMarkerStyle(8);

  //SetMarkerSize
  H_phoEB210_sieie->SetMarkerSize(0.3);
  H_phoEB210_SeedTime->SetMarkerSize(0.7);
  H_phoEB210_MIP->SetMarkerSize(0.3);
  
  H_phoEBprompt_sieie->SetMarkerSize(0.3);
  H_phoEBprompt_SeedTime->SetMarkerSize(0.3);
  H_phoEBprompt_MIP->SetMarkerSize(0.3);

  H_ootBH_sieie->SetMarkerSize(0.3);
  H_ootBH_SeedTime->SetMarkerSize(0.3);
  H_ootBH_MIP->SetMarkerSize(0.3);

  H_ootSP_sieie->SetMarkerSize(0.3);
  H_ootSP_SeedTime->SetMarkerSize(0.3);
  H_ootSP_MIP->SetMarkerSize(0.3);


  //Fitter
  TObjArray *OOTtemplate = new TObjArray(3);
  //H_phoEBprompt_SeedTime->Scale(1/H_phoEBprompt_SeedTime->Integral(1, -1));
  //H_ootBH_SeedTime->Scale(1/H_ootBH_SeedTime->Integral());
  //H_ootSP_SeedTime->Scale(1/H_ootSP_SeedTime->Integral());

  Int_t nbins = H_phoEB210_SeedTime->GetNbinsX();
  for(Int_t ibin=1; ibin<=nbins; ibin++){
    if(H_phoEB210_SeedTime->GetBinContent(ibin) == 0) H_phoEB210_SeedTime->SetBinContent(ibin, pow(10, -10));
    if(H_phoEBprompt_SeedTime->GetBinContent(ibin) == 0) H_phoEBprompt_SeedTime->SetBinContent(ibin, pow(10, -10));
    if(H_ootBH_SeedTime->GetBinContent(ibin) == 0) H_ootBH_SeedTime->SetBinContent(ibin, pow(10, -10));
    if(H_ootSP_SeedTime->GetBinContent(ibin) == 0) H_ootSP_SeedTime->SetBinContent(ibin, pow(10, -10));
  }
  
  OOTtemplate->Add(H_phoEBprompt_SeedTime);
  OOTtemplate->Add(H_ootBH_SeedTime);
  OOTtemplate->Add(H_ootSP_SeedTime);

  TFractionFitter *fit = new TFractionFitter(H_phoEB210_SeedTime, OOTtemplate);
  fit->Constrain(0, 0, 10);
  fit->Constrain(1, 0.0008, 0.045);
  fit->Constrain(2, 0.0009, 0.05);
  Int_t status = fit->Fit();
  cout << "Fit status : " << status << endl;

  
  TH1F *HP_fitresult_SeedTime;
  TH1F *HP_phoEBprompt_SeedTime;
  TH1F *HP_ootBH_SeedTime;
  TH1F *HP_ootSP_SeedTime;  

  Double_t par0, par1, par2;
  Double_t err0, err1, err2;

  if(status == 0){
    Int_t NDF = fit->GetNDF();
    cout << "Fit NDF : " << NDF << endl;

    Double_t chi2 = fit->GetChisquare();
    cout << "Fit Chi2 : " << chi2 << endl;

    cout << "chi2/NDF = " << chi2/NDF << endl;

    fit->GetResult(0, par0, err0);
    fit->GetResult(1, par1, err1);
    fit->GetResult(2, par2, err2);
    
    HP_fitresult_SeedTime = (TH1F*)fit->GetPlot();
    Float_t fityield = HP_fitresult_SeedTime->Integral(1, -1);
    cout << "fit total yield : " << fityield << endl;
    //HP_fitresult_SeedTime = new TH1F("HP_fitresult_SeedTime", "", 100, -25, 25);
    //HP_fitresult_SeedTime->Add((TH1F*)fit->GetMCPrediction(0));
    //HP_fitresult_SeedTime->Add((TH1F*)fit->GetMCPrediction(1));
    //HP_fitresult_SeedTime->Add((TH1F*)fit->GetMCPrediction(2));
   
    //HP_phoEBprompt_SeedTime = (TH1F*)fit->GetMCPrediction(0);
    //HP_ootBH_SeedTime = (TH1F*)fit->GetMCPrediction(1);
    //HP_ootSP_SeedTime = (TH1F*)fit->GetMCPrediction(2);
    
    
    HP_phoEBprompt_SeedTime = (TH1F*)H_phoEBprompt_SeedTime->Clone();
    HP_ootBH_SeedTime = (TH1F*)H_ootBH_SeedTime->Clone();
    HP_ootSP_SeedTime = (TH1F*)H_ootSP_SeedTime->Clone();

    HP_phoEBprompt_SeedTime->Scale(fityield*par0/(H_phoEBprompt_SeedTime->Integral(1, -1)));
    HP_ootBH_SeedTime->Scale(fityield*par1/(H_ootBH_SeedTime->Integral(1, -1)));
    HP_ootSP_SeedTime->Scale(fityield*par2/(H_ootSP_SeedTime->Integral(1, -1)));
    
     
    //HP_fitresult_SeedTime->Add(HP_phoEBprompt_SeedTime);
    //HP_fitresult_SeedTime->Add(HP_ootBH_SeedTime);
    //HP_fitresult_SeedTime->Add(HP_ootSP_SeedTime);
    
    cout << "fit prompt yield : " << HP_phoEBprompt_SeedTime->Integral(1, -1) << endl;
    cout << "fit BH yield : " << HP_ootBH_SeedTime->Integral(1, -1) << endl;
    cout << "fit SP yield : " << HP_ootSP_SeedTime->Integral(1, -1) << endl;

    cout << "3ns total yield : " << HP_fitresult_SeedTime->Integral(45, 56) << endl;
    cout << "3ns prompt yield : " << HP_phoEBprompt_SeedTime->Integral(45, 56) << endl;
    cout << "3ns BH yield : " << HP_ootBH_SeedTime->Integral(45, 56) << endl;
    cout << "3ns SP yield : " << HP_ootSP_SeedTime->Integral(45, 56) << endl;
    
  }
  
  //SetLineColor
  HP_fitresult_SeedTime->SetLineColor(90);
  HP_phoEBprompt_SeedTime->SetLineColor(kRed);
  HP_ootBH_SeedTime->SetLineColor(kTeal-1);
  HP_ootSP_SeedTime->SetLineColor(kAzure-1);

  //SetMarkerColor
  HP_fitresult_SeedTime->SetMarkerColor(90);
  HP_phoEBprompt_SeedTime->SetMarkerColor(kRed);
  HP_ootBH_SeedTime->SetMarkerColor(kTeal-1);
  HP_ootSP_SeedTime->SetMarkerColor(kAzure-1);

  //SetLineWidth
  HP_fitresult_SeedTime->SetLineWidth(2);
  HP_phoEBprompt_SeedTime->SetLineWidth(2);
  HP_ootBH_SeedTime->SetLineWidth(2);
  HP_ootSP_SeedTime->SetLineWidth(2);

  //SetMarkerColor
  HP_fitresult_SeedTime->SetMarkerSize(0.4);
  HP_phoEBprompt_SeedTime->SetMarkerSize(0.3);
  HP_ootBH_SeedTime->SetMarkerSize(0.3);
  HP_ootSP_SeedTime->SetMarkerSize(0.3);

  TLegend *lhs = new TLegend(0.65, 0.72, 0.92, 0.88);//VBS
  lhs->SetTextSize(0.035);
  lhs->SetFillStyle(0);

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
  Int_t period;
  if(year==2016){
    period = 1;
  }
  else if(year==2017){
    period = 2;
  }
  else if(year==2018){
    period = 3;
  }

  pad1->cd();
  H_phoEB210_SeedTime->Draw();
  cout << "data cand: " << H_phoEB210_SeedTime->Integral(1, -1) << endl;
  cout << "data cand 3ns: " << H_phoEB210_SeedTime->Integral(45, 56) << endl;
  
  H_phoEB210_SeedTime->SetName("HF_SeedTime");
  H_phoEB210_SeedTime->GetYaxis()->SetTitle("Events");
  H_phoEB210_SeedTime->GetXaxis()->SetTitle("");
  HP_fitresult_SeedTime->Draw("HIST SAME");
  HP_phoEBprompt_SeedTime->Draw("EP SAME");
  HP_ootBH_SeedTime->Draw("EP SAME");
  HP_ootSP_SeedTime->Draw("EP SAME");
  //H_phoEBprompt_SeedTime->Draw("EP SAME");
  //H_ootBH_SeedTime->Draw("EP SAME");
  //H_ootSP_SeedTime->Draw("EP SAME");
  lhs->Clear();
  lhs->AddEntry(HP_fitresult_SeedTime, "Fit result", "L");
  lhs->AddEntry(HP_phoEBprompt_SeedTime, "Prompt", "PL");
  lhs->AddEntry(HP_ootBH_SeedTime, "Beam Halo", "PL");
  lhs->AddEntry(HP_ootSP_SeedTime, "Spike", "PL");
  lhs->AddEntry(H_phoEB210_SeedTime, "DATA", "PL");
  lhs->Draw("SAME");
  title = H_phoEB210_SeedTime->GetName();
  pad2->cd();
  hratio = ratioplot(H_phoEB210_SeedTime, HP_fitresult_SeedTime);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("SeedTime(ns)");
  hratio->Draw("EP");
  tg->Draw("L SAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s_METcut.pdf", saveto, title));
  
  HP_fitresult_SeedTime->Draw("EP");
}
