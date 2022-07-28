#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TF1.h"
#include <fstream>
#include <iostream>
#include <TMinuit.h>
#include <vector>
#include <TMath.h>
#include "TVirtualFitter.h"
#include "TFile.h"
#include "TLegend.h"
#include "TMarker.h"
#include "TLine.h"
#include "TRandom2.h"
#include "../Drawlumi/CMS_lumi.C"

using namespace std;

#define NPAR 3

TH1F* ratioplot(TH1F* st, TH1F* h1){

  if (!st) {
       Warning("TRatioPlot", "Need only a stack");
       //return;
    }
  
  
  TH1F* tmpHist = (TH1F*)h1->Clone();
  //tmpHist->Reset();
  
  TH1F *ratio = (TH1F*)st->Clone("ratio");
  ratio->SetLineColor(kBlack);
  ratio->SetMinimum(-0.4);
  ratio->SetMaximum(2.4);
  ratio->Sumw2();
  ratio->Divide(tmpHist);
  
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerSize(0.8);
  ratio->Draw("E3 P");
  
  ratio->SetTitle("");
  ratio->GetYaxis()->SetTitle("Data/Fit");
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

//TF1* fchi2;
//TH1F* h1;
//TF1* bsFin;

// Remember to change the values of xh and xl in all_tree.C too!!!
//Double_t xh = 6.5;
//Double_t xl = 4.5;
Double_t xl = -1.;
Double_t xh = 1.;
Double_t bin_size = 0.05;

const double _two_pi = 2.0 * TMath::Pi();
Double_t fit_lo_edge = -1.;
Double_t fit_hi_edge = 1.;


vector<Double_t> CandColl;
vector<Double_t> PromptColl;
vector<Double_t> BHColl;
vector<Double_t> SPColl;

//vector<Double_t> totalColl;

vector<Double_t> info;
vector<Double_t> info_err;

//par[0] = prompt template;
//par[1] = BH template;
//par[2] = SP template;

void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{

  Double_t Lsum=0.;
  Double_t Nevt=0.;
  Double_t fprompt = par[0];
  Double_t fBH = par[1];
  Double_t fSP = par[2];

  for ( int i=0; i<CandColl.size(); i++ ) {
    Nevt += CandColl[i];
    //PDF for signal and background
    Double_t Lprompt = PromptColl[i];
    Double_t LBH = BHColl[i];
    Double_t LSP = SPColl[i];
    for (int data=0; data<CandColl[i]; data++) {	
      //Get Log Likelihood
      Lsum += TMath::Log( (fprompt*Lprompt + fBH*LBH + fSP*LSP) / (fprompt+fBH+fSP) );
    }
  }
  f=2*( -1*Lsum + (fprompt+fBH+fSP)  - Nevt*TMath::Log(fprompt+fBH+fSP) );
  
}


//___________________________________________________________________________
Double_t* xDrawOOT_gMinutFit(int ptbin=30, char EBEE[10]="EB", int fit_data=0, int year=2018)
{

  const char *title;
  //const char *saveto = "/home/judy/ntuhep/GJet/summer16_Zg_EWK/"; 
  const char *saveto = ".";

  TCanvas *c1 = new TCanvas("c1");
  double count=0;
  CandColl.clear();
  PromptColl.clear();
  BHColl.clear();
  SPColl.clear();

  //totalColl.clear();
  //ctauColl.clear();

  // Get template from .root
  TString rootname[20];
  if(year==2016){
    rootname[0] = "/home/jou/output/summer16/OOT_ggtree_jetjetdR.root";
  }
  else if(year==2017){
    rootname[0] = "/home/judychen/Draw/chipoutput/fall17/OOT/OOT_ggtree.root";
  }
  else if(year==2018){
    rootname[0] = "/home/judychen/Draw/chipoutput/autumn18/OOT/OOT_ggtree.root";
  }
  
  TFile* fopen = new TFile(rootname[0]);

  TH1F* hCand = (TH1F*)fopen->Get("h_phoEB_SeedTime");
  TH1F* hPrompt = (TH1F*)fopen->Get("h_phoEBprompt_SeedTime");
  TH1F* hBH = (TH1F*)fopen->Get("h_ootBH210_SeedTime");
  TH1F* hSP = (TH1F*)fopen->Get("h_ootSP210_SeedTime");

  Int_t nbins = hCand->GetNbinsX();
  Int_t ndata = hCand->Integral(1, -1);

  Float_t promptfrac = hPrompt->Integral(1, -1)/ndata;
  Float_t BHfrac = hBH->Integral(1, -1)/ndata;
  Float_t SPfrac = hSP->Integral(1, -1)/ndata;

  Double_t* fitted = new Double_t[6];
  if(ndata==0) {
    printf(" ---  no events in the fit \n");
    fitted[0] = 0.;
    fitted[1] = 0.;
    fitted[2] = 0.;
    fitted[3] = 0.;
    fitted[4] = 0.;
    fitted[5] = 0.;
    return fitted;
  }
  
  printf(" --------- before the fit ------------- \n");
  printf("NCand %6.3f, NPrompt %6.3f, NBH %3.3f, NSP %3.3f \n", hCand->Integral(), hPrompt->Integral(), hBH->Integral(), hSP->Integral());
  
  printf(" -------------------------------------- \n");
  
  hPrompt->Scale(1./hPrompt->Integral(1, -1));
  hBH->Scale(1./hBH->Integral(1, -1));
  hSP->Scale(1./hSP->Integral(1, -1));
  
  for (int ibin=1; ibin<=nbins; ibin++) {
    CandColl.push_back(hCand->GetBinContent(ibin));
    PromptColl.push_back(hPrompt->GetBinContent(ibin));
    BHColl.push_back(hBH->GetBinContent(ibin));
    SPColl.push_back(hSP->GetBinContent(ibin));
    if(CandColl[ibin-1] == 0.0) CandColl[ibin-1] += pow(10, -10);
    if(PromptColl[ibin-1] == 0.0) PromptColl[ibin-1] += pow(10, -10);
    if(BHColl[ibin-1] == 0.0) BHColl[ibin-1] += pow(10, -10);
    if(SPColl[ibin-1] == 0.0) SPColl[ibin-1] += pow(10, -10);

  }
  printf( " -----  Got %d, %d, %d events for fit ----- \n ", CandColl.size(), PromptColl.size(), BHColl.size(), SPColl.size() );  
  if ( CandColl.size() != PromptColl.size() || CandColl.size()!=BHColl.size() || CandColl.size()!=SPColl.size() ) {
    printf(" error ...  inconsistent hit collection size \n");
    return fitted;
  }

  //--------------------------------------------------
  //init parameters for fit
  Double_t vstart[10] = {1., 1., 1.};
  vstart[0] = promptfrac*ndata;
  vstart[1] = BHfrac*ndata;
  vstart[2] = SPfrac*ndata;
  //vstart[0] = sigfrac*ndata;
  //vstart[1] = (1-sigfrac)*ndata;
 
  TMinuit *gMinuit = new TMinuit(NPAR);  
  gMinuit->Command("SET STR 1");
  gMinuit->SetFCN(fcn);
  Double_t arglist[10];
  Int_t ierflg = 0;
  
  arglist[0] = 1;
  gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
  arglist[0] = 1;
  gMinuit->mnexcm("SET PRINT", arglist ,1,ierflg);

  Double_t step[] = { 0.1, 0.1, 0.1,};

  gMinuit->mnparm(0,  "Prompt yield"  , vstart[0],  step[0], 0., ndata*2. , ierflg);
  gMinuit->mnparm(1,  "BH yield"  , vstart[1],  step[1], 0., ndata*0.01 , ierflg);
  gMinuit->mnparm(2,  "SP yield"  , vstart[2],  step[2], 0., ndata*0.01 , ierflg);
  
  printf(" --------------------------------------------------------- \n");
  printf(" Now ready for minimization step \n --------------------------------------------------------- \n");
  
  arglist[0] = 2000; // number of iteration
  arglist[1] = 1.;
  gMinuit->mnexcm("MIGRAD", arglist, 2, ierflg);
  printf (" -------------------------------------------- \n");
  printf("Finished.  ierr = %2.2f \n", ierflg);

  info.clear();
  info_err.clear();

  double para[NPAR+1],errpara[NPAR+1];
  if ( ierflg == 0 ) 
    {
      for(int j=0; j<=NPAR-1;j++) {
        gMinuit->GetParameter(j, para[j],errpara[j]);
        para[NPAR] = CandColl.size();
        info.push_back(para[j]);
        info_err.push_back(errpara[j]);
        printf("Parameter (yeild) %d = %f +- %f\n",j,para[j],errpara[j]);
	
      }
      printf(" fitted yield %1.3f \n", (para[0]+para[1]+para[2])/ndata );

      //info.push_back(sigColl.size());

      //do minos if fit sucessed.
//       printf("         ---------------------------------------------------------\n");
//       printf("          Now call for minos step \n");
//       printf("         ---------------------------------------------------------\n");
      
//       arglist[0] = 200; // number of iteration
//       arglist[1] = 1;
//       gMinuit->mnexcm("MINOS", arglist ,2,ierflg);
//       printf("         --------------------------------------------------------- \n");
//       printf("         Done Minos.  ierr = %d \n", ierflg);
//       Double_t amin;
//       gMinuit->mnprin(1,amin);
    }
  else {
    printf(" *********** Fit failed! ************\n");
    gMinuit->GetParameter(0, para[0],errpara[0]);
    gMinuit->GetParameter(1, para[1],errpara[1]);
    gMinuit->GetParameter(2, para[2],errpara[2]);
    para[0]=0.; errpara[0]=0.;
  }

  
  // Print results
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  gMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  gMinuit->mnprin(1,amin);  
  gMinuit->mnmatu(1);
  printf(" ========= happy ending !? =========================== \n");
  
  printf("FCN =  %3.3f \n", amin);

  double yerr[20];
  for(int i=0;i<20;i++){
    yerr[i] = 0.;
  }

  TH1F* HCand = (TH1F*)hCand->Clone();
  TH1F* HPrompt = (TH1F*)hPrompt->Clone();
  TH1F* HBH = (TH1F*)hBH->Clone();
  TH1F* HSP = (TH1F*)hSP->Clone();
  
  HPrompt->Scale(para[0]);
  HBH->Scale(para[1]);
  HSP->Scale(para[2]);
  
  TH1F *hfit = (TH1F*)HPrompt->Clone();
  hfit->Add(hBH);
  hfit->Add(hSP);

  hfit->SetLineColor(90);
  HPrompt->SetLineColor(kRed);
  HBH->SetLineColor(kTeal-1);
  HSP->SetLineColor(kAzure-1);

  hfit->SetMarkerColor(90);
  HPrompt->SetMarkerColor(kRed);
  HBH->SetMarkerColor(kTeal-1);
  HSP->SetMarkerColor(kAzure-1);

  hfit->SetLineWidth(2);
  HPrompt->SetLineWidth(2);
  HBH->SetLineWidth(2);
  HSP->SetLineWidth(2);

  hfit->SetMarkerSize(0.4);
  HPrompt->SetMarkerSize(0.3);
  HBH->SetMarkerSize(0.3);
  HSP->SetMarkerSize(0.3);

  // plot
  c1->Draw();
  
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
  HCand->Draw("EP");
  HCand->SetLineColor(kBlack);
  HCand->SetLineWidth(2);
  HCand->SetMarkerStyle(8);
  HCand->SetMarkerSize(0.7);
  //cout << "data cand: " << H_phoEB_SeedTime->Integral(1, -1) << endl;
  //cout << "data cand 3ns: " << H_phoEB_SeedTime->Integral(45, 56) << endl;

  HCand->SetName("HF_SeedTime");
  HCand->GetYaxis()->SetTitle("Events");
  HCand->GetXaxis()->SetTitle("");

  hfit->Draw("HIST SAME");
  HPrompt->Draw("EP SAME");
  HBH->Draw("EP SAME");
  HSP->Draw("EP SAME");

  lhs->Clear();
  lhs->AddEntry(hfit, "Fit result", "L");
  lhs->AddEntry(HPrompt, "Prompt", "PL");
  lhs->AddEntry(HBH, "Beam Halo", "PL");
  lhs->AddEntry(HSP, "Spike", "PL");
  lhs->AddEntry(HCand, "DATA", "PL");
  lhs->Draw("SAME");
  title = HCand->GetName();
  pad2->cd();
  hratio = ratioplot(HCand, hfit);
  hratio->SetTitle("");
  hratio->GetXaxis()->SetTitle("SeedTime[ns]");
  hratio->Draw("EP");
  tg->Draw("L SAME");
  CMS_lumi(pad1, period, 0);
  c1->Update();
  c1->RedrawAxis();
  c1->SaveAs(Form("%s/%s.pdf", saveto, title));
  
  printf("Data %7.1f events, fitted %7.1f\n", HCand->Integral(1, -1), hfit->Integral(1, -1));
  printf("Data %7.1f, and fitted (in #pm 3ns) %7.1f events \n", HCand->Integral(45,56), hfit->Integral(45,56));
  printf("Prompt %7.1f #pm %7.1f events \n", para[0], errpara[0]);
  printf("Prompt (in #pm 3ns) %7.1f #pm %7.1f events \n", HPrompt->Integral(45, 56), HPrompt->Integral(45, 56)*errpara[0]/para[0]);
  printf("BH %4.1f #pm %4.1f events \n", para[1], errpara[1]);
  printf("BH (in #pm 3ns) %4.1f #pm %4.1f events \n", HBH->Integral(45, 56), HBH->Integral(45, 56)*errpara[1]/para[1]);
  printf("SP %4.1f #pm %4.1f events \n", para[2], errpara[2]);
  printf("SP (in #pm 3ns) %4.1f #pm %4.1f events \n", HSP->Integral(45, 56), HSP->Integral(45, 56)*errpara[2]/para[2]); 
  
  printf("----- fit results with signal projection   ----------- \n");
  hCand->Chi2Test(hfit,"P");

//   ftemplate->Close();

  fitted[0] = para[0];
  fitted[1] = errpara[0];
  fitted[2] = para[1];
  //if (fit_data==2 ) fitted[2] += hdata->GetBinContent(hdata->GetNbinsX()+1);
  fitted[3] = errpara[1];
  fitted[4] = para[2];
  fitted[5] = errpara[2];
  return fitted;
}

//void pulltest(int ptbin=15, char EBEE[10]="EB", float input=0.5){
//
//
//  TH1F *h1 = new TH1F("h1","",100,-10., 10.);
//  TH1F *h2 = new TH1F("h2","",3000, 0., 3000);
//
//  h1->SetNdivisions(505,"XY");
//  h2->SetNdivisions(505,"XY");
//
//  int nexp=1000;
//  Double_t Nevt=0.;
//
//  for (int i=0; i<nexp; i++) {
//    Ifit(ptbin,EBEE);
//    Nevt=0.;
//    for ( int ii=0; ii<dataColl.size(); ii++ ) {
//      Nevt += dataColl[ii];
//    }
//    printf("fit purity %2.2f +- %2.2f err with %d events. \n", info[0], info_err[0], Nevt);
//    h1->Fill((info[0]/Nevt-input)/(info_err[0]/Nevt));
//    h2->Fill(info[0]);
//  }    
//
//  TCanvas *c2 = new TCanvas("c2","",1000,500);
//  c2->Divide(2,1);
//  c2->cd(1);
//  char txt[100];
//  sprintf(txt, "(purity-input)/error");
//  h1->SetXTitle(txt);
//  h1->Fit("gaus");
//  h1->Draw();
//  c2->cd(2);
//  sprintf(txt, "fitted signal (input %d)", input*Nevt);
//  h2->SetXTitle(txt);
//  h2->Fit("gaus");
//  h2->GetXaxis()->SetRangeUser(0., Nevt*1.2);
//  if ( input >0.8 )  h2->GetXaxis()->SetRangeUser(0., Nevt*1.4);
//  h2->Draw();  
//  sprintf(txt, "plots/extmLfit_pull_%s_pt%d.pdf", EBEE, ptbin);
//  c2->SaveAs(txt);
//
//  
//}
