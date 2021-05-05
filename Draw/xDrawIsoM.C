void xDrawIsoM(){
  ofstream ftext;
    
  TString rootname = "./output_ggtree.root";
  TFile *fopen = new TFile(rootname);
  TCanvas *c1 = new TCanvas("c1");
  TTree *t =(TTree*)fopen->Get("t");

  const char *title;
  const char *saveto = "./graph";

  //sieie
  TH1F *H_sieieFull5x5 = new TH1F("H_sieieFull5x5", "H_sieieFull5x5", 40, 0.001, 0.021); 
  H_sieieFull5x5->GetYaxis()->SetRangeUser(.01, 800000);//60EWK, 800nng
  t->Draw("sieieFull5x5>>H_sieieFull5x5");
  title = H_sieieFull5x5->GetName();

  TH1F *H_sieieFull5x5_M = new TH1F("H_sieieFull5x5_M", "H_sieieFull5x5_M", 40, 0.001, 0.021);
  //H_sieieFull5x5_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("sieieFull5x5_sig>>H_sieieFull5x5_M");
  title = H_sieieFull5x5_M->GetName();

  //sieie multi histo
  H_sieieFull5x5->SetFillStyle(0);
  H_sieieFull5x5->SetLineColor(kBlack);
  H_sieieFull5x5->GetXaxis()->SetTitle("sIEtaIEta");
  H_sieieFull5x5->Draw();

  H_sieieFull5x5_M->SetFillStyle(0);
  H_sieieFull5x5_M->SetLineColor(kYellow+1);
  H_sieieFull5x5_M->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "sieieFull5x5_rel"));

  Int_t nbins;
  nbins = H_sieieFull5x5->GetNbinsX();
  Double_t sieie_count = H_sieieFull5x5->Integral(1, nbins);
  Double_t sieie_Mcount = H_sieieFull5x5_M->Integral(1, nbins);
  Double_t sieie_Meff = sieie_Mcount/sieie_count;

  //sieip
  TH1F *H_sieipFull5x5 = new TH1F("H_sieipFull5x5", "H_sieipFull5x5", 40, 0.001, 0.021); 
  H_sieipFull5x5->GetYaxis()->SetRangeUser(.01, 800000);//60EWK, 800nng
  t->Draw("sieipFull5x5>>H_sieipFull5x5");
  title = H_sieipFull5x5->GetName();

  TH1F *H_sieipFull5x5_M = new TH1F("H_sieipFull5x5_M", "H_sieipFull5x5_M", 40, 0.001, 0.021);
  //H_sieipFull5x5_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("sieipFull5x5_sig>>H_sieipFull5x5_M");
  title = H_sieipFull5x5_M->GetName();

  //sieip multi histo
  H_sieipFull5x5->SetFillStyle(0);
  H_sieipFull5x5->SetLineColor(kBlack);
  H_sieipFull5x5->GetXaxis()->SetTitle("sIEtaIEta");
  H_sieipFull5x5->Draw();

  H_sieipFull5x5_M->SetFillStyle(0);
  H_sieipFull5x5_M->SetLineColor(kYellow+1);
  H_sieipFull5x5_M->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "sieipFull5x5_rel"));

  nbins = H_sieipFull5x5->GetNbinsX();
  Double_t sieip_count = H_sieipFull5x5->Integral(1, nbins);
  Double_t sieip_Mcount = H_sieipFull5x5_M->Integral(1, nbins);
  Double_t sieip_Meff = sieip_Mcount/sieip_count;
  
  //sipip
  TH1F *H_sipipFull5x5 = new TH1F("H_sipipFull5x5", "H_sipipFull5x5", 40, 0.001, 0.021); 
  H_sipipFull5x5->GetYaxis()->SetRangeUser(.01, 800000);//60EWK, 800nng
  t->Draw("sipipFull5x5>>H_sipipFull5x5");
  title = H_sipipFull5x5->GetName();

  TH1F *H_sipipFull5x5_M = new TH1F("H_sipipFull5x5_M", "H_sipipFull5x5_M", 40, 0.001, 0.021);
  //H_sipipFull5x5_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("sipipFull5x5_sig>>H_sipipFull5x5_M");
  title = H_sipipFull5x5_M->GetName();

  //sipip multi histo
  H_sipipFull5x5->SetFillStyle(0);
  H_sipipFull5x5->SetLineColor(kBlack);
  H_sipipFull5x5->GetXaxis()->SetTitle("sIEtaIEta");
  H_sipipFull5x5->Draw();

  H_sipipFull5x5_M->SetFillStyle(0);
  H_sipipFull5x5_M->SetLineColor(kYellow+1);
  H_sipipFull5x5_M->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "sipipFull5x5_rel"));

  nbins = H_sipipFull5x5->GetNbinsX();
  Double_t sipip_count = H_sipipFull5x5->Integral(1, nbins);
  Double_t sipip_Mcount = H_sipipFull5x5_M->Integral(1, nbins);
  Double_t sipip_Meff = sipip_Mcount/sipip_count;

  
  //draw TH1F logY histograms
  c1->SetLogy();
  c1->Update();

  //r9
  TH1F *H_r9Full5x5 = new TH1F("H_r9Full5x5", "H_r9Full5x5", 15, 0., 1.5);
  //H_r9Full5x5->GetYaxis()->SetRangeUser(.01, 10000000);
  H_r9Full5x5->GetXaxis()->SetTitle("r9");
  t->Draw("r9Full5x5>>H_r9Full5x5");
  title = H_r9Full5x5->GetName();

  TH1F *H_r9Full5x5_M = new TH1F("H_r9Full5x5_M", "H_r9Full5x5_M", 15, 0., 1.5);
  //H_r9Full5x5_M->GetYaxis()->SetRangeUser(.01, 10000000);
  H_r9Full5x5_M->GetXaxis()->SetTitle("r9");
  t->Draw("r9Full5x5_sig>>H_r9Full5x5_M");
  title = H_r9Full5x5_M->GetName();

  H_r9Full5x5->SetFillStyle(0);
  H_r9Full5x5->SetLineColor(kBlack);
  H_r9Full5x5->GetXaxis()->SetTitle("r9Full5x5");
  H_r9Full5x5->Draw();

  H_r9Full5x5_M->SetFillStyle(0);
  H_r9Full5x5_M->SetLineColor(kYellow+1);
  H_r9Full5x5_M->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "r9Full5x5_rel"));

  nbins = H_r9Full5x5->GetNbinsX();
  Double_t r9Full5x5_count = H_r9Full5x5->Integral(1, nbins);
  Double_t r9Full5x5_Mcount = H_r9Full5x5_M->Integral(1, nbins);
  Double_t r9Full5x5_Meff = r9Full5x5_Mcount/r9Full5x5_count;

  //HoverE
  TH1F *H_HoverE = new TH1F("H_HoverE", "H_HoverE", 10, 0., 1);
  H_HoverE->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("HoverE>>H_HoverE");
  title = H_HoverE->GetName();

  TH1F *H_HoverE_M = new TH1F("H_HoverE_M", "H_HoverE_M", 10, 0., 1);
  H_HoverE_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("HoverE_sig>>H_HoverE_M");
  title = H_HoverE_M->GetName();

  H_HoverE->SetFillStyle(0);
  H_HoverE->SetLineColor(kBlack);
  H_HoverE->GetXaxis()->SetTitle("HoverE");
  H_HoverE->Draw();

  H_HoverE_M->SetFillStyle(0);
  H_HoverE_M->SetLineColor(kYellow+1);
  H_HoverE_M->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "HoverE_rel"));

  nbins = H_HoverE->GetNbinsX();
  Double_t HoverE_count = H_HoverE->Integral(1, nbins);
  Double_t HoverE_Mcount = H_HoverE_M->Integral(1, nbins);
  Double_t HoverE_Meff = HoverE_Mcount/HoverE_count;

  //chIso
  TH1F *H_chIso = new TH1F("H_chIso", "H_chIso", 25, 0., 10);
  H_chIso->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("chIso_rc>>H_chIso");
  title = H_chIso->GetName();
  
  TH1F *H_chIso_M = new TH1F("H_chIso_M", "H_chIso_M", 25, 0., 10);
  H_chIso_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("chIso_sig>>H_chIso_M");
  title = H_chIso_M->GetName();

  H_chIso->SetFillStyle(0);
  H_chIso->SetLineColor(kBlack);
  H_chIso->GetXaxis()->SetTitle("chIso");
  H_chIso->Draw();

  H_chIso_M->SetFillStyle(0);
  H_chIso_M->SetLineColor(kYellow+1);
  H_chIso_M->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "chIso_rel"));

  nbins = H_chIso->GetNbinsX();
  Double_t chIso_count = H_chIso->Integral(1, nbins);
  Double_t chIso_Mcount = H_chIso_M->Integral(1, nbins);
  Double_t chIso_Meff = chIso_Mcount/chIso_count;

  //phoIso
  TH1F *H_phoIso = new TH1F("H_phoIso", "H_phoIso", 25, 0., 10);
  H_phoIso->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("phoIso_rc>>H_phoIso");
  title = H_phoIso->GetName();

  TH1F *H_phoIso_M = new TH1F("H_phoIso_M", "H_phoIso_M", 25, 0., 10);
  H_phoIso_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("phoIso_sig>>H_phoIso_M");
  title = H_phoIso_M->GetName();

  HphoIso->SetFillStyle(0);
  H_phoIso->SetLineColor(kBlack);
  H_phoIso->GetXaxis()->SetTitle("phoIso");
  H_phoIso->Draw();
 
  H_phoIso_M->SetFillStyle(0);
  H_phoIso_M->SetLineColor(kYellow+1);
  H_phoIso_M->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "phoIso_rel"));

  nbins = H_phoIso->GetNbinsX();
  Double_t phoIso_count = H_phoIso->Integral(1, nbins);
  Double_t phoIso_Mcount = H_phoIso_M->Integral(1, nbins);
  Double_t phoIso_Meff = phoIso_Mcount/phoIso_count;

   //nhIso
  TH1F *H_nhIso = new TH1F("H_nhIso", "H_nhIso", 25, 0., 10);
  H_nhIso->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("nhIso_rc>>H_nhIso");
  title = H_nhIso->GetName();

  TH1F *H_nhIso_M = new TH1F("H_nhIso_M", "H_nhIso_M", 25, 0., 10);
  H_nhIso_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("nhIso_sig>>H_nhIso_M");
  title = H_nhIso_M->GetName();
  H_nhIso->SetFillStyle(0);
  H_nhIso->SetLineColor(kBlack);
  H_nhIso->GetXaxis()->SetTitle("nhIso");
  H_nhIso->Draw();
  
  //H_nhIso_L->SetFillStyle(0);
  //H_nhIso_L->SetLineColor(kGreen+1);
  //H_nhIso_L->Draw("SAME");

  H_nhIso_M->SetFillStyle(0);
  H_nhIso_M->SetLineColor(kYellow+1);
  H_nhIso_M->Draw("SAME");

  //H_nhIso_T->SetFillStyle(0);
  //H_nhIso_T->SetLineColor(kAzure+1);
  //H_nhIso_T->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "nhIso_rel"));

  nbins = H_nhIso->GetNbinsX();
  Double_t nhIso_count = H_nhIso->Integral(1, nbins);
  Double_t nhIso_Mcount = H_nhIso_M->Integral(1, nbins);
  Double_t nhIso_Meff = nhIso_Mcount/nhIso_count;
  
  //chWorst
  TH1F *H_chWorst = new TH1F("H_chWorst", "H_chWorst", 25, 0., 10);
  H_chWorst->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("chWorst_rc>>H_chWorst");
  title = H_chWorst->GetName();
  
  TH1F *H_chWorst_M = new TH1F("H_chWorst_M", "H_chWorst_M", 25, 0., 10);
  H_chWorst_M->GetYaxis()->SetRangeUser(.01, 10000000);
  t->Draw("chWorst_sig>>H_chWorst_M");
  title = H_chWorst_M->GetName();

  H_chWorst->SetFillStyle(0);
  H_chWorst->SetLineColor(kBlack);
  H_chWorst->GetXaxis()->SetTitle("chWorst");
  H_chWorst->Draw();

  H_chWorst_M->SetFillStyle(0);
  H_chWorst_M->SetLineColor(kYellow+1);
  H_chWorst_M->Draw("SAME");
  c1->SaveAs(Form("%s/%s.pdf", saveto, "chWorst_rel"));

  nbins = H_chWorst->GetNbinsX();
  Double_t chWorst_count = H_chWorst->Integral(1, nbins);
  Double_t chWorst_Mcount = H_chWorst_M->Integral(1, nbins);
  Double_t chWorst_Meff = chWorst_Mcount/chWorst_count;

  ftext.open("eff.txt", ios::out | ios::app);
  if(!ftext){
    cerr << "Can't open file !" << endl;
    exit(1);
  }
  
  ftext << "sieie_Meff : " << sieie_Meff << endl;
  ftext << "sieip_Meff : " << sieip_Meff << endl;
  ftext << "sipip_Meff : " << sipip_Meff << endl;
  ftext << "r9Full5x5_Meff : " << r9Full5x5_Meff << endl;
  ftext << "HoverE_Meff : " << HoverE_Meff << endl;
  ftext << "chIso_Meff : " << chIso_Meff << endl;
  ftext << "phoIso_Meff : " << phoIso_Meff << endl;
  ftext << "nhIso_Meff : " << nhIso_Meff << endl;
  ftext << "chWorst_Meff : " << chWorst_Meff << endl;
  
  ftext.close();
}
