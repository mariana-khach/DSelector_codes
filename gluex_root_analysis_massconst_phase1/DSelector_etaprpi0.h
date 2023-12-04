#ifndef DSelector_etaprpi0_h
#define DSelector_etaprpi0_h

#include <iostream>

#include "DSelector/DSelector.h"
#include "DSelector/DHistogramActions.h"
#include "DSelector/DCutActions.h"

#include "TH1I.h"
#include "TH2I.h"
#include "TLorentzRotation.h"

class DSelector_etaprpi0 : public DSelector
{
	public:

		DSelector_etaprpi0(TTree* locTree = NULL) : DSelector(locTree){}
		virtual ~DSelector_etaprpi0(){}

		void Init(TTree *tree);
		Bool_t Process(Long64_t entry);

	private:

		void Get_ComboWrappers(void);
		void Finalize(void);

		// BEAM POLARIZATION INFORMATION
		UInt_t dPreviousRunNumber;
		bool dIsPolarizedFlag; //else is AMO
		bool dIsPARAFlag; //else is PERP or AMO
		Int_t dPolarizationAngle;

		//bool for default branches in flat tree
		//bool dSaveDefaultFlatBranches;


		//bool dIsMC;

		// ANALYZE CUT ACTIONS
		// // Automatically makes mass histograms where one cut is missing
		DHistogramAction_AnalyzeCutActions* dAnalyzeCutActions;

		//CREATE REACTION-SPECIFIC PARTICLE ARRAYS

		//Step 0
		DParticleComboStep* dStep0Wrapper;
		DBeamParticle* dComboBeamWrapper;
		DChargedTrackHypothesis* dProtonWrapper;

		//Step 1
		DParticleComboStep* dStep1Wrapper;
		DKinematicData* dDecayingPi0Wrapper;
		DNeutralParticleHypothesis* dPhoton1Wrapper;
		DNeutralParticleHypothesis* dPhoton2Wrapper;

		//Step 2
		DParticleComboStep* dStep2Wrapper;
		DChargedTrackHypothesis* dPiMinusWrapper;
		DChargedTrackHypothesis* dPiPlusWrapper;

		//Step 3
		DParticleComboStep* dStep3Wrapper;
		DKinematicData* dDecayingEtaWrapper;
		DNeutralParticleHypothesis* dPhoton3Wrapper;
		DNeutralParticleHypothesis* dPhoton4Wrapper;

		// DEFINE YOUR HISTOGRAMS HERE
		// EXAMPLES:
		//TH1I* dHist_MissingMassSquared;
		//TH1I* dHist_BeamEnergy;
		TH1F *dHist_showerquality1,*dHist_showerquality2, *dHist_showerquality3, *dHist_showerquality4,	*dHist_MissingMassSquared, *dHist_EtaPrimeMass,*dHist_deltatRF,*dHist_tmand,*dHist_BeamEnergy,*dHist_Chi2, *dHist_Chi2_cut;   


		

		//tree stuff (variables for qfactor analysis) goes here
		/*TFile *fileout; // file for the outputtree
		TTree *qfactortree; //qfactortree

		
		Int_t event_num;
		Int_t run_num;


		Double_t mm2;
		Double_t mm2m;
		Double_t dt;
		Double_t be;

		Double_t mpi0;  
		Double_t meta; 
		Double_t metap; 
		Double_t metappi0; 
		Double_t mpi0m;
		Double_t metam; 
		Double_t metapm; 
		Double_t metappi0m; 

		Double_t mpi013;
		Double_t mpi014;
		Double_t mpi023;
		Double_t mpi024;

		Double_t mpi013m;
		Double_t mpi014m;
		Double_t mpi023m;
		Double_t mpi024m;
		

		Double_t mpippimpi0;
		Double_t mpippimpi0m;

		Double_t mpi0p;
		Double_t mpi0pm;

		Double_t mant;
		Double_t mantm;

		//Double_t photon1_sq;
		//Double_t photon2_sq;
		//Double_t photon3_sq;
		//Double_t photon4_sq;
		Double_t mproeta;

		Double_t cos_t;
		Double_t costheta_X_cm;
		Double_t phi_gj;
		Int_t pol;

        Float_t chisq;
        Float_t ndf;
		Float_t chisq_ndf;
        Float_t kinfit_CL;

		//info for amplitude analysis for Mariana
		Float_t px_pr;
		Float_t px_etapr;
		Float_t px_pi0;

		Float_t py_pr;
		Float_t py_etapr;
		Float_t py_pi0;

		Float_t pz_pr;
		Float_t pz_etapr;
		Float_t pz_pi0;

		Float_t e_pr;
		Float_t e_etapr;
		Float_t e_pi0;

		Float_t px_beam;
		Float_t py_beam;
		Float_t pz_beam;
		Float_t e_beam;

		//for bggen study
		Int_t topology;
		*/
		
		


	ClassDef(DSelector_etaprpi0, 0);
};

void DSelector_etaprpi0::Get_ComboWrappers(void)
{
	//Step 0
	dStep0Wrapper = dComboWrapper->Get_ParticleComboStep(0);
	dComboBeamWrapper = static_cast<DBeamParticle*>(dStep0Wrapper->Get_InitialParticle());
	dProtonWrapper = static_cast<DChargedTrackHypothesis*>(dStep0Wrapper->Get_FinalParticle(2));

	//Step 1
	dStep1Wrapper = dComboWrapper->Get_ParticleComboStep(1);
	dDecayingPi0Wrapper = dStep1Wrapper->Get_InitialParticle();
	dPhoton1Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep1Wrapper->Get_FinalParticle(0));
	dPhoton2Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep1Wrapper->Get_FinalParticle(1));

	//Step 2
	dStep2Wrapper = dComboWrapper->Get_ParticleComboStep(2);
	dPiMinusWrapper = static_cast<DChargedTrackHypothesis*>(dStep2Wrapper->Get_FinalParticle(0));
	dPiPlusWrapper = static_cast<DChargedTrackHypothesis*>(dStep2Wrapper->Get_FinalParticle(1));

	//Step 3
	dStep3Wrapper = dComboWrapper->Get_ParticleComboStep(3);
	dDecayingEtaWrapper = dStep3Wrapper->Get_InitialParticle();
	dPhoton3Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep3Wrapper->Get_FinalParticle(0));
	dPhoton4Wrapper = static_cast<DNeutralParticleHypothesis*>(dStep3Wrapper->Get_FinalParticle(1));
}

#endif // DSelector_etaprpi0_h
