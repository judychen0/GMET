#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "TH1F.h"
using namespace std;

#define nhisto 2

void xDraw_ABCD_ratio(Int_t year){
  ofstream ftext;
  TString rootname[10];
  TFile *fopen, *fout;
  
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/mc/GJet/output_merge_GJet.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/fall17/data/output_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/autumn18/mc/GJet/output_merge_GJet.root";
    rootname[1] = "/home/judy/ntuhep/GMET/output_file/autumn18/data/output_ggtree.root";
  }

  TCanvas *c1 = new TCanvas("c1");
  //c1->Divide(1, 2);
  TTree *t;

  //TH1F *H_MET_cut[nhisto];
  TH1F *H_MET_Nm1[nhisto];
  TH1F *H_MET_Nm1_djetMETPhim0p5[nhisto];
  TH1F *H_MET_Nm1_djetMETPhi_SB[nhisto];

  TH1F *HD_MET_cut[nhisto];
  TH1F *HD_MET_Nm1[nhisto];
  TH1F *HD_MET_Nm1_djetMETPhim0p5[nhisto];
  TH1F *HD_MET_Nm1_djetMETPhi_SB[nhisto];
  
  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  Double_t A_yield[nhisto] = {0};//MET>100, djetMETPhi>0.5
  Double_t B_yield[nhisto] = {0};//MET>100, djetMETPhi<0.5
  Double_t C_yield[nhisto] = {0};//MET<100, djetMETPhi>0.5
  Double_t D_yield[nhisto] = {0};//MET<100, djetMETPhi<0.5

  TH1F *H_Ayield[nhisto]; TH1F *H_FitA[nhisto];
  TH1F *H_Byield[nhisto]; TH1F *H_FitB[nhisto];
  TH1F *H_Cyield[nhisto]; TH1F *H_FitC[nhisto];
  TH1F *H_Dyield[nhisto]; TH1F *H_FitD[nhisto];

  TH1F *H_ABratio[nhisto];
  TH1F *H_CDratio[nhisto];

  TH1F *H_MET_cut_corr[nhisto];
  TH1F *H_MET_Nm1_corr[nhisto];

  TH1F *HD_MET_cut_corr[nhisto];
  TH1F *HD_MET_Nm1_corr[nhisto];
  
  
  //MC GJet
  fopen = new TFile(rootname[0]);
  for(Int_t jj=0; jj<nhisto; jj++){ 
    H_MET_Nm1[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", jj));
    //H_MET_Nm1_djetMETPhim0p5[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_djetMETPhim0p5_%i", jj));
    H_MET_Nm1[jj]->SetDirectory(0);
    //H_MET_Nm1_djetMETPhim0p5[jj]->SetDirectory(0);
    H_MET_Nm1_djetMETPhi_SB[jj] = (TH1F*)fopen->Get(Form("h_MET_Nm1/h_MET_Nm1_djetMETPhi_SB0p5_%i", jj));
    H_MET_Nm1_djetMETPhi_SB[jj]->SetDirectory(0);
  }
  fopen->Close();
  
  //data
  fopen = new TFile(rootname[1]);
  for(Int_t jj=0; jj<nhisto; jj++){ 
    HD_MET_cut[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_cut_%i", jj));
    HD_MET_cut[jj]->SetDirectory(0);
    
    HD_MET_Nm1[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", jj));
    //HD_MET_Nm1_djetMETPhim0p5[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_djetMETPhim0p5_%i", jj));
    HD_MET_Nm1[jj]->SetDirectory(0);
    //HD_MET_Nm1_djetMETPhim0p5[jj]->SetDirectory(0);

    HD_MET_Nm1_djetMETPhi_SB[jj] = (TH1F*)fopen->Get(Form("h_MET_Nm1/h_MET_Nm1_djetMETPhi_SB0p5_%i", jj));
    HD_MET_Nm1_djetMETPhi_SB[jj]->SetDirectory(0);
    
  }
  fopen->Close();
  
  for(Int_t jj=0; jj<nhisto; jj++){

    A_yield[jj] = HD_MET_Nm1[jj]->Integral(6, -1);
    //B_yield[jj] = HD_MET_Nm1_djetMETPhim0p5[jj]->Integral(5, -1);
    B_yield[jj] = HD_MET_Nm1_djetMETPhi_SB[jj]->Integral(6, -1);
    C_yield[jj] = H_MET_Nm1[jj]->Integral(6, -1);
    //D_yield[jj] = H_MET_Nm1_djetMETPhim0p5[jj]->Integral(1, 4);
    D_yield[jj] = H_MET_Nm1_djetMETPhi_SB[jj]->Integral(6, -1);
    cout << D_yield[jj] << " ";
    
    H_Ayield[jj] = new TH1F(Form("H_Ayield_%i", jj), "H_Ayield", 60, 0, 1200);
    H_Byield[jj] = new TH1F(Form("H_Byield_%i", jj), "H_Byield", 60, 0, 1200);
    H_Cyield[jj] = new TH1F(Form("H_Cyield_%i", jj), "H_Cyield", 60, 0, 1200);
    H_Dyield[jj] = new TH1F(Form("H_Dyield_%i", jj), "H_Dyield", 60, 0, 1200);
    H_Ayield[jj]->Sumw2();
    H_Byield[jj]->Sumw2();
    H_Cyield[jj]->Sumw2();
    H_Dyield[jj]->Sumw2();

    H_FitA[jj] = new TH1F(Form("H_FitA_%i", jj), "H_FitA", 60, 0, 1200);
    H_FitB[jj] = new TH1F(Form("H_FitB_%i", jj), "H_FitB", 60, 0, 1200);
    H_FitC[jj] = new TH1F(Form("H_FitC_%i", jj), "H_FitC", 60, 0, 1200);
    H_FitD[jj] = new TH1F(Form("H_FitD_%i", jj), "H_FitD", 60, 0, 1200);
    H_FitA[jj]->Sumw2();
    H_FitB[jj]->Sumw2();
    H_FitC[jj]->Sumw2();
    H_FitD[jj]->Sumw2();

    H_MET_cut_corr[jj] = new TH1F(Form("H_MET_cut_corr_%i", jj), "MET N cut corr", 60, 0, 1200);
    H_MET_Nm1_corr[jj] = new TH1F(Form("H_MET_Nm1_corr_%i", jj), "MET N-1 cut corr", 60, 0, 1200);
    H_MET_cut_corr[jj]->Sumw2();
    H_MET_Nm1_corr[jj]->Sumw2();

    HD_MET_cut_corr[jj] = new TH1F(Form("HD_MET_cut_corr_%i", jj), "MET N cut corr (data)", 60, 0, 1200);
    HD_MET_Nm1_corr[jj] = new TH1F(Form("HD_MET_Nm1_corr_%i", jj), "MET N-1 cut corr (data)", 60, 0, 1200);
    HD_MET_cut_corr[jj]->Sumw2();
    HD_MET_Nm1_corr[jj]->Sumw2();
    
    //H_Ayield[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", jj));
    //H_Byield[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_djetMETPhim0p5_%i", jj));
    //H_Cyield[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_%i", jj));
    //H_Dyield[jj] = (TH1F*)fopen->Get(Form("SMandVBS/h_MET_Nm1_djetMETPhim0p5_%i", jj));
    
    for(Int_t ibin=1; ibin<61; ibin++){
      if(ibin>=6){
	H_Ayield[jj]->SetBinContent(ibin, HD_MET_Nm1[jj]->GetBinContent(ibin));
	H_Byield[jj]->SetBinContent(ibin, HD_MET_Nm1_djetMETPhi_SB[jj]->GetBinContent(ibin));
      }
    }
    for(Int_t ibin=1; ibin<61; ibin++){
      if(ibin<6){
	H_Cyield[jj]->SetBinContent(ibin, HD_MET_Nm1[jj]->GetBinContent(ibin));
	H_Dyield[jj]->SetBinContent(ibin, HD_MET_Nm1_djetMETPhi_SB[jj]->GetBinContent(ibin));
      }
    }
    
    //H_Ayield[jj]->Rebin(2);
    //H_Byield[jj]->Rebin(2);
    //H_Cyield[jj]->Rebin(2);
    //H_Dyield[jj]->Rebin(2);
    
    cout << "bin content set done" << endl;
    
    c1->SetLogy();
    H_Ayield[jj]->Draw();
    title = "H_Ayield";
    c1->SaveAs(Form("%s/%s_%i.pdf", saveto, title, jj));
    
    H_Byield[jj]->Draw();
    title = "H_Byield";
    c1->SaveAs(Form("%s/%s_%i.pdf", saveto, title, jj));
    
    H_Cyield[jj]->Draw();
    title = "H_Cyield";
    c1->SaveAs(Form("%s/%s_%i.pdf", saveto, title, jj));
    
    H_Dyield[jj]->Draw();
    HD_MET_Nm1[jj]->Draw("SAME");
    title = "H_Dyield";
    c1->SaveAs(Form("%s/%s_%i.pdf", saveto, title, jj));

    c1->SetLogy(0);

    for(Int_t ibin=1; ibin<61; ibin++){
      H_FitC[jj]->SetBinContent(ibin, H_Dyield[jj]->GetBinContent(ibin));
      //H_FitA[jj]->SetBinContent(ibin, H_Byield[jj]->GetBinContent(ibin));
      
    }
    //H_FitC[jj] = (TH1F*)H_Dyield[jj]->Clone();
    H_FitA[jj] = (TH1F*)HD_MET_Nm1_djetMETPhi_SB[jj]->Clone();
    for(Int_t ibin=0; ibin<5; ibin++){
      H_FitA[jj]->SetBinContent(ibin+1,0);
    }

    H_FitC[jj]->Scale(C_yield[jj]/D_yield[jj]);
    //H_FitA[jj]->Scale((A_yield[jj]+C_yield[jj])/(B_yield[jj]+D_yield[jj]));
    cout << H_FitA[jj]->Integral(1, -1) << endl;
    H_FitA[jj]->Scale(A_yield[jj]/B_yield[jj]);
    //H_FitA[jj]->Scale(C_yield[jj]/D_yield[jj]);

    for(Int_t ibin=1; ibin<61; ibin++){
      //if(jj==0 && ibin>5) H_MET_cut_corr[jj]->SetBinContent(ibin, H_FitA[jj]->GetBinContent(ibin));
      //if(jj==1 && ibin>5) H_MET_cut_corr[jj]->SetBinContent(ibin, H_FitA[jj]->GetBinContent(ibin));
      H_MET_cut_corr[jj]->SetBinContent(ibin, H_FitA[jj]->GetBinContent(ibin));
      if(ibin<=6) H_MET_Nm1_corr[jj]->SetBinContent(ibin, H_FitC[jj]->GetBinContent(ibin));
      if(ibin<6) H_MET_Nm1_corr[jj]->SetBinContent(ibin, H_FitA[jj]->GetBinContent(ibin));
      
    }

    TLegend *lhs = new TLegend(0.65,0.78, 0.92, 0.90);
    lhs->SetTextSize(0.035);
    lhs->SetFillStyle(0);
  
    c1->SetLogy();
    H_MET_cut_corr[jj]->Draw("HIST");
    H_MET_cut_corr[jj]->SetMaximum(1000);
    H_MET_cut_corr[jj]->SetLineColor(kRed);
    H_MET_cut_corr[jj]->GetXaxis()->SetTitle("MET(GeV)");
    H_MET_cut_corr[jj]->GetYaxis()->SetTitle("Events");
    for(Int_t ibin=1; ibin<61; ibin++){
      //if(jj==0 && ibin>5) HD_MET_cut_corr[jj]->SetBinContent(ibin, HD_MET_Nm1[jj]->GetBinContent(ibin));
      //if(jj==1 && ibin>5) HD_MET_cut_corr[jj]->SetBinContent(ibin, HD_MET_Nm1[jj]->GetBinContent(ibin));
      //if(jj==0 && ibin>5) HD_MET_cut_corr[jj]->SetBinError(ibin, HD_MET_Nm1[jj]->GetBinError(ibin));
      //if(jj==1 && ibin>5) HD_MET_cut_corr[jj]->SetBinError(ibin, HD_MET_Nm1[jj]->GetBinError(ibin));
      if(ibin>=6){
	HD_MET_cut_corr[jj]->SetBinContent(ibin, HD_MET_Nm1[jj]->GetBinContent(ibin));
	HD_MET_cut_corr[jj]->SetBinContent(ibin, HD_MET_Nm1[jj]->GetBinContent(ibin));
	HD_MET_cut_corr[jj]->SetBinError(ibin, HD_MET_Nm1[jj]->GetBinError(ibin));
	HD_MET_cut_corr[jj]->SetBinError(ibin, HD_MET_Nm1[jj]->GetBinError(ibin));
      }
    }
    HD_MET_cut_corr[jj]->Draw("AP SAME");
    //HD_MET_cut_corr[jj]->GetXaxis()->SetRange(7, -1);
    HD_MET_cut_corr[jj]->SetLineColor(kBlack);
    HD_MET_cut_corr[jj]->SetMarkerStyle(8);
    HD_MET_cut_corr[jj]->SetMarkerColor(kBlack);
    lhs->Clear();
    lhs->AddEntry(H_MET_cut_corr[jj], "#gamma+jet", "L");
    lhs->AddEntry(HD_MET_cut_corr[jj], "Data", "PL");
    lhs->Draw("SAME");
    title = "H_MET_cut_corr";
    c1->SaveAs(Form("%s/%s_%i.pdf", saveto, title, jj));

    H_MET_Nm1_corr[jj]->Draw("HIST");
    H_MET_Nm1_corr[jj]->SetMaximum(1000000);
    H_MET_Nm1_corr[jj]->SetLineColor(kRed);
    H_MET_Nm1_corr[jj]->GetXaxis()->SetTitle("MET(GeV)");
    H_MET_Nm1_corr[jj]->GetYaxis()->SetTitle("Events");
    for(Int_t ibin=1; ibin<61; ibin++){
      HD_MET_Nm1_corr[jj]->SetBinContent(ibin, HD_MET_Nm1[jj]->GetBinContent(ibin));
      HD_MET_Nm1_corr[jj]->SetBinError(ibin, HD_MET_Nm1[jj]->GetBinError(ibin));
    }
    HD_MET_Nm1_corr[jj]->Draw("AP SAME");
    //HD_MET_Nm1_corr[jj]->GetXaxis()->SetRange(7, -1);
    HD_MET_Nm1_corr[jj]->SetLineColor(kBlack);
    HD_MET_Nm1_corr[jj]->SetMarkerStyle(8);
    HD_MET_Nm1_corr[jj]->SetMarkerColor(kBlack);
    lhs->Clear();
    lhs->AddEntry(H_MET_Nm1_corr[jj], "#gamma+jet", "L");
    lhs->AddEntry(HD_MET_Nm1_corr[jj], "Data", "PL");
    lhs->Draw("SAME");
    title = "H_MET_Nm1_corr";
    c1->SaveAs(Form("%s/%s_%i.pdf", saveto, title, jj));
    c1->SetLogy(0);
    
    H_ABratio[jj] = (TH1F*)H_Ayield[jj]->Clone();
    H_CDratio[jj] = (TH1F*)H_Cyield[jj]->Clone();
    H_ABratio[jj]->Divide(H_Byield[jj]);
    H_CDratio[jj]->Divide(H_Dyield[jj]);

    H_ABratio[jj]->Draw();
    //H_ABratio[jj]->GetYaxis()->SetRangeUser(0, 1);
    title = "H_ABratio";
    c1->SaveAs(Form("%s/%s_%i.pdf", saveto, title, jj));

    H_CDratio[jj]->Draw();
    //H_CDratio[jj]->GetYaxis()->SetRangeUser(0, 1);
    title = "H_CDratio";
    c1->SaveAs(Form("%s/%s_%i.pdf", saveto, title, jj));

    H_FitA[jj]->Draw();
  }

  fout = new TFile("GJet_template.root", "RECREATE");
  for(Int_t jj=0; jj<nhisto; jj++){
    H_Ayield[jj]->Write();
    H_Byield[jj]->Write();
    H_Cyield[jj]->Write();
    H_Dyield[jj]->Write();
    
    H_FitA[jj]->Write();
    H_FitC[jj]->Write();

    //H_MET_cut_corr[jj]->Write();
    //H_MET_Nm1_corr[jj]->Write();
  }

  fout->mkdir("SMandVBS");
  fout->cd("SMandVBS");
  for(Int_t jj=0; jj<nhisto; jj++){
    H_MET_cut_corr[jj]->Write();
    H_MET_Nm1_corr[jj]->Write();
  }
  fout->Close();

  
  ftext.open("GJet_ABCDratio.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }

  for(Int_t jj=0; jj<nhisto; jj++){
    ftext << "cutflow " << jj << endl;
    ftext << "A_yield : " << A_yield[jj] << " C_yield : " << C_yield[jj] << " A/C(data) : " << A_yield[jj]/C_yield[jj] << endl;
    ftext << "B_yield : " << B_yield[jj] << " D_yield : " << D_yield[jj] << " B/D(MC) : " << B_yield[jj]/D_yield[jj] << endl;
    ftext << "djetMETPhi ctrl yield : " << B_yield[jj]+D_yield[jj];
    ftext << " djetMETPhi sig yield : " << A_yield[jj]+C_yield[jj];
    ftext << " sig/ctrl : " << (A_yield[jj]+C_yield[jj]) / (B_yield[jj]+D_yield[jj]) << endl;
  }
  ftext.close();
  
  
}
