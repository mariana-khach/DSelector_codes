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


void plots_new_helicity()
{
  //Output .root file
  TFile *fileOUT=new TFile("Tree_file.root","Recreate");
 
  //Root file with kin tree to be analyzed
  TChain *treeMC=new TChain("kin");
  treeMC->Add("MY_root_file.root");



TTree *newtree = treeMC->CloneTree(0);

    
    float E_FinalStateMC[3],Px_FinalStateMC[3],Py_FinalStateMC[3],Pz_FinalStateMC[3],Px_BeamMC,Py_BeamMC,Pz_BeamMC,E_BeamMC;

    
    treeMC->SetBranchAddress("E_FinalState",E_FinalStateMC);
    treeMC->SetBranchAddress("Px_FinalState",Px_FinalStateMC);
    treeMC->SetBranchAddress("Py_FinalState",Py_FinalStateMC);
    treeMC->SetBranchAddress("Pz_FinalState",Pz_FinalStateMC);
    treeMC->SetBranchAddress("Px_Beam",&Px_BeamMC);  
    treeMC->SetBranchAddress("Py_Beam",&Py_BeamMC);
    treeMC->SetBranchAddress("Pz_Beam",&Pz_BeamMC);
    treeMC->SetBranchAddress("E_Beam",&E_BeamMC);
 
    
    TH1F *h_CosThetaMC=new TH1F("h_CosThetaMC","cos#theta",50,-1,1);
    TH1F* h_EtaprimePiMassMC=new TH1F("h_EtaprimePiMassMC",";M(#eta#pi^{-})",12,1.1,2.0);
    TH1F* h_EtaprimeMass=new TH1F("h_EtaprimeMass",";M(#eta^{'})",100,0.4,1.5);
    TH2F* h_CosTheta_vs_EtaprimePiMass=new TH2F("h_CosTheta_vs_EtaprimePiMass",";M(#eta#pi); coos#theta",100,0.6,3.0,100,-1,1);
    TH1F* h_t=new TH1F("h_t",";t;",200,0,5);
    TH1F* h_phi=new TH1F("h_phi",";#phi;",180,-180,180);
    TH1F* h_Phi=new TH1F("h_Phi",";#Phi;",180,-180,180);
    TH2F* h_t_vs_EtaprimePiMass=new TH2F("h_t_vs_EtaprimePiMass",";M(#eta#pi); t",200,0,5,100,0,3);
    TH1F* h_beamE=new TH1F("h_beamE",";t;",400,0,12);
    TH2F* h_CosTheta_vs_t=new TH2F("h_CosTheta_vs_t",";t [(GeV/c)^{2}]; cos#theta",100,0.,3.0,100,-1,1);
    TH2F* h_Phi_vs_EtaprimePiMass=new TH2F("h_Phi_vs_EtaprimePiMass",";M(#eta#pi); #Phi",100,0.6,3.0,180,-180,180);
    TH2F* h_phi_vs_EtaprimePiMass=new TH2F("h_phi_vs_EtaprimePiMass",";M(#eta#pi); #phi",100,0.6,3.0,180,-180,180);   
    TH2F* h_Phi_vs_phi=new TH2F("h_Phi_vs_phi",";#phi; #Phi",180,-180,180,180,-180,180);
    TH2F* h_CosTheta_vs_phi=new TH2F("h_CosTheta_vs_phi",";#phi [Deg.]; cos#theta",180,-180,180,100,-1,1);    

    Long64_t NEntriesMC=treeMC->GetEntries();
    cout<<"There are "<<NEntriesMC<<" entries."<<endl;
    //NEntries=2500000;
    
    for (Long64_t i_entry=0; i_entry<NEntriesMC; i_entry++) {
        treeMC->GetEntry(i_entry);
        if (i_entry%500000==0)                            //outputs every 500,000 events processed
            cout<<i_entry<<" events processed"<<endl;
        
          
        TLorentzVector TargetP4MC;
        TargetP4MC.SetPxPyPzE(0,0,0,0.938272);

        


	//////////////////////////////////////////////////// Zlm Helicity Frame//////////////////////////////////////////

	TLorentzVector beam,recoil,EtaprimeP4MC,Pi0P4MC;   

	recoil.SetPxPyPzE(Px_FinalStateMC[0],Py_FinalStateMC[0],Pz_FinalStateMC[0],E_FinalStateMC[0]);
        EtaprimeP4MC.SetPxPyPzE(Px_FinalStateMC[1],Py_FinalStateMC[1],Pz_FinalStateMC[1],E_FinalStateMC[1]);
        Pi0P4MC.SetPxPyPzE(Px_FinalStateMC[2],Py_FinalStateMC[2],Pz_FinalStateMC[2],E_FinalStateMC[2]);
	beam.SetPxPyPzE(Px_BeamMC,Py_BeamMC,Pz_BeamMC,E_BeamMC);

	 TLorentzVector resonance = EtaprimeP4MC + Pi0P4MC;

	 double t_beam_etaprimepi=-(beam-resonance).M2();
	 double EtaprimeMass= EtaprimeP4MC.M();
	 double EtaprimePiMass=resonance.M();

	 TLorentzRotation resRestBoost( -resonance.BoostVector() );

	 TLorentzVector beam_res   = resRestBoost * beam;
	 TLorentzVector recoil_res = resRestBoost * recoil;
	 TLorentzVector EtaprimeP4MC_res = resRestBoost * EtaprimeP4MC;


	 // Helicity frame 
	 
	 TVector3 z = -1. * recoil_res.Vect().Unit();
	 TVector3 y = (beam.Vect().Unit().Cross(-recoil.Vect().Unit())).Unit();
	 //TVector3 y = beam_res.Vect().Cross(z).Unit(); //TwoPSHelicity yields the same
	                                             

	 // or GJ frame?                                                                
 /*                                                                       
	  TVector3 z = beam_res.Vect().Unit();                        
	   TVector3 y = (beam.Vect().Unit().Cross(-recoil.Vect().Unit())).Unit();
	  // TVector3 y = recoil_res.Vect().Cross(z).Unit();//TwoPSAngles yields the same
	  */
	 TVector3 x = y.Cross(z);

	 TVector3 angles( (EtaprimeP4MC_res.Vect()).Dot(x),
			  (EtaprimeP4MC_res.Vect()).Dot(y),
			  (EtaprimeP4MC_res.Vect()).Dot(z) );

	 double cosTheta = angles.CosTheta();
	 double phi = angles.Phi()*180./TMath::Pi();

	 ////////////////////////////////////////////////////////////////////////////////////////////////////

	 double polAngle=0;   //polarization angle, angle between beam polarization plane and the horizontal in lab frame                     
	 TVector3 eps(cos(polAngle*TMath::DegToRad()), sin(polAngle*TMath::DegToRad()), 0.0); // beam polarization vector                 
	 double Phi = atan2(y.Dot(eps), beam.Vect().Unit().Dot(eps.Cross(y)));

	 //h_CosTheta_vs_EtaprimePiMass->Rebin2D();



	h_beamE->Fill(E_BeamMC);
      	h_EtaprimePiMassMC->Fill(EtaprimePiMass);
        h_CosThetaMC->Fill(cosTheta);
        h_EtaprimeMass->Fill(EtaprimeMass);
	h_t->Fill(t_beam_etaprimepi);
	h_phi->Fill(phi);
	h_Phi->Fill(Phi*180./TMath::Pi());
	//cout<<EtaMass<<"   pimass=  "<<Pi0P4MC.M()<<endl;
        h_CosTheta_vs_EtaprimePiMass->Fill(EtaprimePiMass,cosTheta);
	h_t_vs_EtaprimePiMass->Fill(EtaprimePiMass,t_beam_etaprimepi);
	h_CosTheta_vs_t->Fill(t_beam_etaprimepi,cosTheta);    
	h_CosTheta_vs_phi->Fill(phi,cosTheta);
	h_Phi_vs_EtaprimePiMass->Fill(EtaprimePiMass,Phi*180./TMath::Pi());
	h_phi_vs_EtaprimePiMass->Fill(EtaprimePiMass,phi);
	h_Phi_vs_phi->Fill(phi,Phi*180./TMath::Pi());
}
    
    TStyle* dStyle = new TStyle("JLab_Style", "JLab_Style");
    dStyle->Reset("Modern");
    //Size & Margins
    dStyle->SetCanvasDefH(800);
    dStyle->SetCanvasDefW(1200);
    
    //Basic object fill colors
    dStyle->SetCanvasBorderMode(0);
    dStyle->SetLegendFillColor(0);
    dStyle->SetCanvasColor(0);
    
    //Stat box
    dStyle->SetOptFit(112);
    dStyle->SetOptStat(10); //entries only //mean/rms should be replaced with a fit
    
    //Titles and Labels
    dStyle->SetLabelSize(0.07, "xyz");
    dStyle->SetTitleSize(0.075, "xyz");
    dStyle->SetTitleOffset(1.1, "x");
    dStyle->SetTitleOffset(0.8, "y");
    
    //Margins (correlated with title/label size)
    dStyle->SetPadBottomMargin(0.15);
    
    //Default Histogram Style Settings
    //dStyle->SetHistFillColor(kTeal);
    dStyle->SetHistFillStyle(1001); //solid fill (0 for hollow) //see http://root.cern.ch/root/html/TAttFill.html
    dStyle->SetHistLineColor(kBlack);
    dStyle->SetHistMinimumZero(kTRUE);
    
    //Default Function Style Settings
    dStyle->SetFuncColor(kBlack);
    dStyle->SetFuncStyle(1); //see http://root.cern.ch/root/html/TAttLine.html
    dStyle->SetFuncWidth(3);
    
    //Default Line Style Settings
    dStyle->SetLineColor(kBlack);
    dStyle->SetLineStyle(1); //see http://root.cern.ch/root/html/TAttLine.html
    dStyle->SetLineWidth(1); //is border of TLegend
    
    //Default Graph Style Settings
    dStyle->SetMarkerColor(kBlack);
    dStyle->SetMarkerSize(1); //see http://root.cern.ch/root/html/TAttMarker.html
    dStyle->SetMarkerStyle(22); //see http://root.cern.ch/root/html/TAttMarker.html
    
    //Grid
    dStyle->SetPadGridX(kTRUE);
    dStyle->SetPadGridY(kTRUE);
    
    //Histogram/Graph Title Size
    dStyle->SetTitleSize(0.075, "t"); //"t": could be anything that is not "x" "y" or "z"
    
    //Palette
    dStyle->SetNumberContours(200);
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
    dStyle->SetPalette(kBird); //57
#else
    dStyle->SetPalette(55); //rainbow
#endif
    dStyle->cd();
    
    TLatex ytitle;
    TLatex maintitle;
    TLatex xtitle;
    
   
      gStyle->SetOptStat(0000);

    /*TCanvas *c0b=new TCanvas("c0b","");
    c0b->cd(1);
  
    h_EtaprimeMass->GetXaxis()->SetNdivisions(504);
    h_EtaprimeMass->GetYaxis()->SetNdivisions(504);
    h_EtaprimeMass->GetYaxis()->SetLabelSize(0.05);
    h_EtaprimeMass->GetXaxis()->SetLabelSize(0.05);
    h_EtaprimeMass->GetYaxis()->SetTitle("");
    h_EtaprimeMass->GetXaxis()->SetTitle("");
    h_EtaprimeMass->Draw();
    
       c0b->SaveAs("Plots/EtaprimeMass.pdf");
    */

    /* TCanvas* c1=new TCanvas("c1","");
    c1->cd(1);
    h_CosTheta_vs_EtaPiMass->Draw("colz");
    c1->SaveAs("Plots/CosTheta_vs_EtaPiMass.pdf");
    */
    TCanvas* c1b=new TCanvas("c1bb","");
    c1b->Divide(2,1);
    c1b->cd(1);
    h_EtaprimePiMassMC->Draw("");
    c1b->cd(2);
    h_CosThetaMC->Draw();
    c1b->SaveAs("Plots/EtaprimePi_Dists.pdf");
    
    TCanvas* c2=new TCanvas("c2","");
    //c1b->Divide(2,1);
    c2->cd(1);
    c2->SetLeftMargin(0.05);
    h_EtaprimePiMassMC->SetMarkerSize(1.5);
    h_EtaprimePiMassMC->SetMarkerStyle(20);
    h_EtaprimePiMassMC->SetMarkerColor(kBlack);
    h_EtaprimePiMassMC->SetLineColor(kBlack);
    h_EtaprimePiMassMC->GetXaxis()->SetNdivisions(504);
    h_EtaprimePiMassMC->GetYaxis()->SetNdivisions(504);
    h_EtaprimePiMassMC->GetYaxis()->SetLabelSize(0.05);
    h_EtaprimePiMassMC->GetXaxis()->SetLabelSize(0.05);
    //h_EtaPiMassMC->TGaxis::SetMaxDigits(3);
    h_EtaprimePiMassMC->GetXaxis()->SetTitle("");

    h_EtaprimePiMassMC->Draw("pe");
    
    xtitle.SetTextAlign(21); //align center x
    xtitle.SetTextSize(0.075);//percent of canvas size
    xtitle.DrawLatexNDC(0.5, 0.05, "M(#eta^{'}#pi^{0}) [GeV/c^{2}]");
    
    ytitle.SetTextAlign(21); //align center x
    ytitle.SetTextSize(0.05);//percent of canvas size
    ytitle.SetTextAngle(90);
    c2->SetLeftMargin(0.13);
    ytitle.DrawLatexNDC(0.05, 0.5, "Counts/24 MeV");

    c2->SaveAs("Plots/EtaprimePi_Mass.pdf");
    
    TCanvas* c2b=new TCanvas("c2b","");
    //c1b->Divide(2,1);
    c2b->cd(1);
    h_CosTheta_vs_EtaprimePiMass->GetXaxis()->SetNdivisions(504);
    h_CosTheta_vs_EtaprimePiMass->GetYaxis()->SetNdivisions(504);
    h_CosTheta_vs_EtaprimePiMass->GetYaxis()->SetTitle("");
    h_CosTheta_vs_EtaprimePiMass->GetXaxis()->SetTitle("");

    h_CosTheta_vs_EtaprimePiMass->Rebin2D();
    h_CosTheta_vs_EtaprimePiMass->Draw("colz");
    
    xtitle.SetTextAlign(21); //align center x
    xtitle.SetTextSize(0.075);//percent of canvas size
    xtitle.DrawLatexNDC(0.5, 0.05, "M(#eta^{'}#pi^{0}) [GeV/c^{2}]");
    
    ytitle.SetTextAlign(21); //align center x
    ytitle.SetTextSize(0.075);//percent of canvas size
    ytitle.SetTextAngle(90);
    ytitle.DrawLatexNDC(0.05, 0.5, "cos#theta");

    c2b->SaveAs("Plots/CosTheta_vs_Mass.pdf");



    TCanvas* c3=new TCanvas("c3","");
    
    h_t_vs_EtaprimePiMass->SetAxisRange(0,3.5,"X");
    h_t_vs_EtaprimePiMass->SetAxisRange(0,1,"Y");

    h_t_vs_EtaprimePiMass->SetTitle(";M(#eta^{'}#pi^{0}) [GeV/c];t [(GeV/c)^{2}]");
    h_t_vs_EtaprimePiMass->Draw("colz");

    c3->SaveAs("Plots/t_vs_Mass.pdf");



    TCanvas* c4=new TCanvas("c4","");
     h_CosTheta_vs_t->SetTitle(";t [(GeV/c)^2];cos#theta [Deg.]");
     h_CosTheta_vs_t->Draw("colz");
    c4->SaveAs("Plots/costheta_vs_t.pdf");


    TCanvas* c5=new TCanvas("c5","");
    h_beamE->SetAxisRange(8,9.1,"X");
    h_beamE->SetTitle(";E_{beam} [GeV];");
    h_beamE->Draw();
    c5->SaveAs("Plots/Ebeam.pdf");

 TCanvas* c6=new TCanvas("c6","");
 h_EtaprimeMass->Draw();    
c6->SaveAs("Plots/M_etaprime.pdf");



// h_EtaprimePiMassMC->Write();
//h_CosThetaMC->Write();
// h_CosTheta_vs_EtaprimePiMass->Write()
//newtree->AutoSave();


 cout<<" Number of events "<<treeMC->GetEntries()<<endl;

 cout<<" Number of signal events "<<h_EtaprimePiMassMC->Integral()<<endl;

}
