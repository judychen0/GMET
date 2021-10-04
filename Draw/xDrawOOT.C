#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;
#define nfile 2
#define nhisto 2
#define ncolor 10

void xDrawOOT(Int_t year){
  /*
  TString rootname[20] = {
			  "/home/judy/ntuhep/GMET/output_file/summer16/data/OOT_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/fall17/data/OOT_ggtree.root",
			  "/home/judy/ntuhep/GMET/output_file/autumn18/data/OOT_ggtree.root"
			  
  };
  */
  TString rootname[20];
  if(year==2016){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/summer16/data/OOT_ggtree.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judy/ntuhep/GMET/output_file/fall17/data/OOT_ggtree.root";
  }
  //Int_t iyear = 0;
  //if(year == 2016) iyear = 0;
  //else if(iyear == 2017) iyear = 1;
  //else if(iyear == 2018) iyear = 2;

  string hexcolor[ncolor] = {"#fff176", "#03a9f4","#ff3360", "#67ccc1","#1C2A60", "#DCDB06", "#FA6D5F", "#F0F5E1","#DCF563", "#2F81A3" };
  TColor *color[ncolor];
  Int_t cnum[ncolor];
   
  for(Int_t j=0; j<8; j++){
    color[j] = new TColor();
    Int_t n = hexcolor[j].length();
    char chararray[n+1];
    strcpy(chararray, hexcolor[j].c_str());
    //color[j]->SetAlpha(0.4);
    cnum[j] = color[j]->GetColor(chararray);
    cnum[j] = TColor::GetColorTransparent(cnum[j], 0.6);
  }

  TFile *fopen;
  TCanvas *c1 = new TCanvas("c1");

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  TH1F* H_phoEB_sieie;
  TH1F* H_phoEB_SeedTime;
  TH1F* H_phoEB_MIP;

  TH1F* H_phoEBprompt_sieie;
  TH1F* H_phoEBprompt_SeedTime;
  TH1F* H_phoEBprompt_MIP;

  TH1F* H_ootBH_sieie;
  TH1F* H_ootBH_SeedTime;
  TH1F* H_ootBH_MIP;

  TH1F* H_ootBH210_sieie;
  TH1F* H_ootBH210_SeedTime;
  TH1F* H_ootBH210_MIP;

  TH1F* H_ootSP_sieie;
  TH1F* H_ootSP_SeedTime;
  TH1F* H_ootSP_MIP;

  TH1F* H_ootSP210_sieie;
  TH1F* H_ootSP210_SeedTime;
  TH1F* H_ootSP210_MIP;
  
  TLegend *lh = new TLegend(0.63, 0.80, 0.75, 0.90);
  lh->SetTextSize(0.03);
  lh->SetFillStyle(0);
  TString ltext[10] = {"Beam Halo", "Spike"};

  fopen = new TFile(rootname[0]);
  
  H_phoEB_sieie = (TH1F*)fopen->Get("h_phoEB_sieie");
  H_phoEB_SeedTime = (TH1F*)fopen->Get("h_phoEB_SeedTime");
  H_phoEB_MIP = (TH1F*)fopen->Get("h_phoEB_MIP");

  H_phoEBprompt_sieie = (TH1F*)fopen->Get("h_phoEBprompt_sieie");
  H_phoEBprompt_SeedTime = (TH1F*)fopen->Get("h_phoEBprompt_SeedTime");
  H_phoEBprompt_MIP = (TH1F*)fopen->Get("h_phoEBprompt_MIP");
   
  H_ootBH_sieie = (TH1F*)fopen->Get("h_ootBH_sieie");
  H_ootBH_SeedTime = (TH1F*)fopen->Get("h_ootBH_SeedTime");
  H_ootBH_MIP = (TH1F*)fopen->Get("h_ootBH_MIP");
   
  H_ootBH210_sieie = (TH1F*)fopen->Get("h_ootBH210_sieie");
  H_ootBH210_SeedTime = (TH1F*)fopen->Get("h_ootBH210_SeedTime");
  H_ootBH210_MIP = (TH1F*)fopen->Get("h_ootBH210_MIP");
   
  H_ootSP_sieie = (TH1F*)fopen->Get("h_ootSP_sieie");
  H_ootSP_SeedTime = (TH1F*)fopen->Get("h_ootSP_SeedTime");
  H_ootSP_MIP = (TH1F*)fopen->Get("h_ootSP_MIP");
   
  H_ootSP210_sieie = (TH1F*)fopen->Get("h_ootSP210_sieie");
  H_ootSP210_SeedTime = (TH1F*)fopen->Get("h_ootSP210_SeedTime");
  H_ootSP210_MIP = (TH1F*)fopen->Get("h_ootSP210_MIP");

  H_phoEB_sieie->SetLineColor(kBlack);
  H_phoEB_SeedTime->SetLineColor(kBlack);
  H_phoEB_MIP->SetLineColor(kBlack);
  
  H_phoEBprompt_sieie->SetLineColor(kRed);
  H_phoEBprompt_SeedTime->SetLineColor(kRed);
  H_phoEBprompt_MIP->SetLineColor(kRed);

  H_ootBH_sieie->SetLineColor(kTeal-1);
  H_ootBH_SeedTime->SetLineColor(kTeal-1);
  H_ootBH_MIP->SetLineColor(kTeal-1);
  
  H_ootBH210_sieie->SetLineColor(kTeal-1);
  H_ootBH210_SeedTime->SetLineColor(kTeal-1);
  H_ootBH210_MIP->SetLineColor(kTeal-1);

  H_ootSP_sieie->SetLineColor(kAzure-1);
  H_ootSP_SeedTime->SetLineColor(kAzure-1);
  H_ootSP_MIP->SetLineColor(kAzure-1);

  H_ootSP210_sieie->SetLineColor(kAzure-1);
  H_ootSP210_SeedTime->SetLineColor(kAzure-1);
  H_ootSP210_MIP->SetLineColor(kAzure-1);

  c1->SetLogy();
  H_phoEB_SeedTime->Draw("HIST");
  H_phoEB_SeedTime->GetXaxis()->SetTitle("SeedTime(ns)");
  title = H_phoEB_SeedTime->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  H_phoEBprompt_SeedTime->Draw("HIST");
  H_phoEBprompt_SeedTime->GetXaxis()->SetTitle("SeedTime(ns)");
  title = H_phoEBprompt_SeedTime->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  H_ootBH_SeedTime->Draw("HIST");
  H_ootBH_SeedTime->GetXaxis()->SetTitle("SeedTime(ns)");
  title = H_ootBH_SeedTime->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  H_ootBH210_SeedTime->Draw("HIST");
  H_ootBH210_SeedTime->GetXaxis()->SetTitle("SeedTime(ns)");
  title = H_ootBH210_SeedTime->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  H_ootSP_SeedTime->Draw("HIST");
  H_ootSP_SeedTime->GetXaxis()->SetTitle("SeedTime(ns)");
  title = H_ootSP_SeedTime->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  H_ootSP210_SeedTime->Draw("HIST");
  H_ootSP210_SeedTime->GetXaxis()->SetTitle("SeedTime(ns)");
  title = H_ootSP210_SeedTime->GetName();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));

  H_phoEBprompt_SeedTime->Draw("HIST");
  H_phoEB_SeedTime->Draw("HISTSAME");
  H_ootBH_SeedTime->Draw("HISTSAME");
  H_ootSP_SeedTime->Draw("HISTSAME");
  lh->Clear();
  lh->AddEntry(H_phoEB_SeedTime, "Candidate", "L");
  lh->AddEntry(H_phoEBprompt_SeedTime, "Prompt", "L");
  lh->AddEntry(H_ootBH_SeedTime, "Beam halo", "L");
  lh->AddEntry(H_ootSP_SeedTime, "Spike", "L");
  lh->Draw("SAME");
  c1->SaveAs(Form("%s/H_SeedTimecomp.pdf", saveto));

  H_phoEBprompt_SeedTime->Draw("HIST");
  H_phoEB_SeedTime->Draw("HISTSAME");
  H_ootBH210_SeedTime->Draw("HISTSAME");
  H_ootSP210_SeedTime->Draw("HISTSAME");
  lh->Clear();
  lh->AddEntry(H_phoEB_SeedTime, "Candidate", "L");
  lh->AddEntry(H_phoEBprompt_SeedTime, "Prompt", "L");
  lh->AddEntry(H_ootBH210_SeedTime, "Beam halo", "L");
  lh->AddEntry(H_ootSP210_SeedTime, "Spike", "L");
  lh->Draw("SAME");
  c1->SaveAs(Form("%s/H_SeedTime210comp.pdf", saveto));
}
