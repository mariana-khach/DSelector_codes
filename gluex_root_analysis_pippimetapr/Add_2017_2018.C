#include <iostream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TMath.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TLegend.h"
#include "TVector3.h"
#include "TStyle.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLine.h"
#include <TRandom3.h>
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TPad.h"
#include "TStyle.h"
#include <numeric>
#include "TLorentzRotation.h"
#include "TChain.h"


void Add_2017_2018()
{

 cout<<"Here1"<<endl;
  TFile *fileIn2017=new TFile("2017/pippimetapr__B3_M35.root");
  TFile *fileIn2018=new TFile("2018/pippimetapr__B3_M35_2018.root");
  //TFile *fileIn2018=new TFile("pippimetapr__B3_M35_2018.root");

 TFile *fileOUT=new TFile("Tree_file.root","Recreate");

cout<<"Here2"<<endl;
TH1F* h1_EtaPrimePiMinusMass_accid_sideband_2017 = (TH1F*)fileIn2017->Get("EtaPrimePiMinusMass_accid_sideband");
TH2F* h2_dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_accid_sideband_2017 = (TH2F*)fileIn2017->Get("CosThetaEta_GJ_vs_EtaPiMinusMass_accid_sideband");
//Delta++ with all cuts but Delta++ mass cut
TH1F* h1_DeltaPPMass_2017 = (TH1F*)fileIn2017->Get("DeltaPPMass_accid_cut");
//MM^2 with all cuts but MM mass cut
TH1F* h1_MM_2017 = (TH1F*)fileIn2017->Get("MissingMassSquared_cut");
//chi^2 with all cuts 
TH1F* h1_chi2_2017 = (TH1F*)fileIn2017->Get("Chi2_cut");
//Histogram before any cuts
TH1F* h1_Ebeam_2017 = (TH1F*)fileIn2017->Get("BeamEnergy");




cout<<"Here3"<<endl;
TH1F* h1_EtaPrimePiMinusMass_accid_sideband_2018 = (TH1F*)fileIn2018->Get("EtaPrimePiMinusMass_accid_sideband");
TH2F* h2_dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_accid_sideband_2018 = (TH2F*)fileIn2018->Get("CosThetaEta_GJ_vs_EtaPiMinusMass_accid_sideband");
//Delta++ with all cuts but Delta++ mass cut
TH1F* h1_DeltaPPMass_2018 = (TH1F*)fileIn2018->Get("DeltaPPMass_accid_cut");
//MM^2 with all cuts but MM mass cut
TH1F* h1_MM_2018 = (TH1F*)fileIn2018->Get("MissingMassSquared_cut");
//chi^2 with all cuts 
TH1F* h1_chi2_2018 = (TH1F*)fileIn2018->Get("Chi2_cut");
//Histogram before any cuts
TH1F* h1_Ebeam_2018 = (TH1F*)fileIn2018->Get("BeamEnergy");
cout<<"Here4"<<endl;



 TH1F *h1_EtaPrimePiMinusMass_accid_sideband_all=(TH1F*) h1_EtaPrimePiMinusMass_accid_sideband_2017->Clone();
 TH2F *h2_dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_accid_sideband_all=(TH2F*) h2_dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_accid_sideband_2017->Clone();
 TH1F *h1_DeltaPPMass_all=(TH1F*) h1_DeltaPPMass_2017->Clone();
TH1F *h1_MM_all=(TH1F*) h1_MM_2017->Clone();
TH1F *h1_chi2_all=(TH1F*) h1_chi2_2017->Clone();
TH1F *h1_Ebeam_all=(TH1F*) h1_Ebeam_2017->Clone();
cout<<"Here5"<<endl;

 h1_EtaPrimePiMinusMass_accid_sideband_all->Add(h1_EtaPrimePiMinusMass_accid_sideband_2018);
 h2_dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_accid_sideband_all->Add(h2_dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_accid_sideband_2018);
 h1_DeltaPPMass_all->Add(h1_DeltaPPMass_2018);
 h1_MM_all->Add(h1_MM_2018);
 h1_chi2_all->Add(h1_chi2_2018);
 h1_Ebeam_all->Add(h1_Ebeam_2018);


 h1_DeltaPPMass_all->Write();
 h1_EtaPrimePiMinusMass_accid_sideband_all->Write();
   h2_dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_accid_sideband_all->Write();
   h1_MM_all->Write();
   h1_chi2_all->Write();
   h1_Ebeam_all->Write();
}
