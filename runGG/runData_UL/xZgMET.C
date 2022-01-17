#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

#include "TH2F.h"
#include "untuplizer.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "PhotonSelection.h"

void xZgMET(char* pathes){
   //access EventTree with TreeReader class
  TreeReader data(pathes);

  //create an output .root file
  TFile *fout_;
  char foutname[50];
  string treestr = pathes;
  Int_t istr = treestr.find("ggtree");
  string treename = treestr.substr(istr);
  sprintf(foutname, "METcorr_%s", treename.c_str());
  fout_ = new TFile(foutname,"RECREATE");

  TH2F* h2_nvtx_METX = new TH2F("h2_nvtx_METX", "h2_nvtx_METX", 100, 0, 100, 100, -500, 500);
  TH2F* h2_nvtx_METY = new TH2F("h2_nvtx_METY", "h2_nvtx_METY", 100, 0, 100, 100, -500, 500);
  
  Int_t		nGoodVtx	= 0;
  Float_t       pfMET		= 0;
  Float_t       pfMETPhi	= 0;

  //***********************Loop***********************//
  for (Long64_t ev = 0; ev < data.GetEntriesFast(); ev++){
    if (ev % 100000 == 0){
      fprintf(stderr, "Processing event %lli of %lli (%.3f \%)\n", ev+1, data.GetEntriesFast(), (ev+1)*100./data.GetEntriesFast());
    }

    //get data from tree
    data.GetEntry(ev);
    
    nGoodVtx	= data.GetInt(		"nGoodVtx"		);
    pfMET	= data.GetFloat(	"pfMET"			);
    pfMETPhi	= data.GetFloat(	"pfMETPhi"		);
    
    //MET XY corr
    Float_t METX = pfMET*cos(pfMETPhi);
    h2_nvtx_METX->Fill(nGoodVtx, METX);
    Float_t METY = pfMET*sin(pfMETPhi);
    h2_nvtx_METY->Fill(nGoodVtx, METY);
    
  }
  //****************END LOOP**********************//

  //****************Terminate*********************//
  fout_->cd();

  h2_nvtx_METX->Write();
  h2_nvtx_METY->Write();

  fout_->Close();
  fprintf(stderr, "%s Processed all events\n", treename.c_str());
}
