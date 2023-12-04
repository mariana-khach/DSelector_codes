#include "DSelector_pippimetapr__B3_M35_2018.h"

void DSelector_pippimetapr__B3_M35_2018::Init(TTree *locTree)
{
	// USERS: IN THIS FUNCTION, ONLY MODIFY SECTIONS WITH A "USER" OR "EXAMPLE" LABEL. LEAVE THE REST ALONE.

	// The Init() function is called when the selector needs to initialize a new tree or chain.
	// Typically here the branch addresses and branch pointers of the tree will be set.
	// Init() will be called many times when running on PROOF (once per file to be processed).

	//USERS: SET OUTPUT FILE NAME //can be overriden by user in PROOF
	dOutputFileName = "pippimetapr__B3_M35_2018.root"; //"" for none
	dOutputTreeFileName = ""; //"" for none
	dFlatTreeFileName = "flat_pippimetapr_MinCuts.root"; //output flat tree (one combo per tree entry), "" for none
	dFlatTreeName = ""; //if blank, default name will be chosen
	//dSaveDefaultFlatBranches = true; // False: don't save default branches, reduce disk footprint.
	//dSaveTLorentzVectorsAsFundamentaFlatTree = false; // Default (or false): save particles as TLorentzVector objects. True: save as four doubles instead.

	//Because this function gets called for each TTree in the TChain, we must be careful:
		//We need to re-initialize the tree interface & branch wrappers, but don't want to recreate histograms
	bool locInitializedPriorFlag = dInitializedFlag; //save whether have been initialized previously
	DSelector::Init(locTree); //This must be called to initialize wrappers for each new TTree
	//gDirectory now points to the output file with name dOutputFileName (if any)
	if(locInitializedPriorFlag)
		return; //have already created histograms, etc. below: exit

	Get_ComboWrappers();
	dPreviousRunNumber = 0;

	/*********************************** EXAMPLE USER INITIALIZATION: ANALYSIS ACTIONS **********************************/

	// EXAMPLE: Create deque for histogramming particle masses:
	// // For histogramming the phi mass in phi -> K+ K-
	// // Be sure to change this and dAnalyzeCutActions to match reaction
	std::deque<Particle_t> MyPhi;
	MyPhi.push_back(KPlus); MyPhi.push_back(KMinus);

	//ANALYSIS ACTIONS: //Executed in order if added to dAnalysisActions
	//false/true below: use measured/kinfit data

	//PID
	dAnalysisActions.push_back(new DHistogramAction_ParticleID(dComboWrapper, false));
	//below: value: +/- N ns, Unknown: All PIDs, SYS_NULL: all timing systems
	//dAnalysisActions.push_back(new DCutAction_PIDDeltaT(dComboWrapper, false, 0.5, KPlus, SYS_BCAL));

	//PIDFOM (for charged tracks)
	//dAnalysisActions.push_back(new DHistogramAction_PIDFOM(dComboWrapper));
	//dAnalysisActions.push_back(new DCutAction_PIDFOM(dComboWrapper, KPlus, 0.1));
	//dAnalysisActions.push_back(new DCutAction_EachPIDFOM(dComboWrapper, 0.1));

	//MASSES
	//dAnalysisActions.push_back(new DHistogramAction_InvariantMass(dComboWrapper, false, Lambda, 1000, 1.0, 1.2, "Lambda"));
	//dAnalysisActions.push_back(new DHistogramAction_MissingMassSquared(dComboWrapper, false, 1000, -0.1, 0.1));

	//KINFIT RESULTS
	dAnalysisActions.push_back(new DHistogramAction_KinFitResults(dComboWrapper));

	//CUT MISSING MASS
	//dAnalysisActions.push_back(new DCutAction_MissingMassSquared(dComboWrapper, false, -0.03, 0.02));

	//CUT ON SHOWER QUALITY
	//dAnalysisActions.push_back(new DCutAction_ShowerQuality(dComboWrapper, SYS_FCAL, 0.5));

	//BEAM ENERGY
	dAnalysisActions.push_back(new DHistogramAction_BeamEnergy(dComboWrapper, false));
	dAnalysisActions.push_back(new DCutAction_BeamEnergy(dComboWrapper, false, 8.2, 8.8));  // Coherent peak for runs in the range 30000-59999

	//KINEMATICS
	dAnalysisActions.push_back(new DHistogramAction_ParticleComboKinematics(dComboWrapper, false));

	//CUT Unused Energy
	//	dAnalysisActions.push_back(new DCutAction_Energy_UnusedShowers(dComboWrapper, 0.1));

	// ANALYZE CUT ACTIONS
	// // Change MyPhi to match reaction
	dAnalyzeCutActions = new DHistogramAction_AnalyzeCutActions( dAnalysisActions, dComboWrapper, false, 0, MyPhi, 1000, 0.9, 2.4, "CutActionEffect" );

	//INITIALIZE ACTIONS
	//If you create any actions that you want to run manually (i.e. don't add to dAnalysisActions), be sure to initialize them here as well
	Initialize_Actions();
	dAnalyzeCutActions->Initialize(); // manual action, must call Initialize()

	/******************************** EXAMPLE USER INITIALIZATION: STAND-ALONE HISTOGRAMS *******************************/

	//EXAMPLE MANUAL HISTOGRAMS:
	dHist_phot1_theta=new TH1F("dHist_phot1_theta",";#theta_{#gamma}",720,0,180);
	dHist_phot2_theta=new TH1F("dHist_phot2_theta",";#theta_{#gamma}",720,0,180);
	dHist_phot1_theta_cut=new TH1F("dHist_phot1_theta_cut",";#theta_{#gamma}",720,0,180);
	dHist_phot2_theta_cut=new TH1F("dHist_phot2_theta_cut",";#theta_{#gamma}",720,0,180);
	dHist_showerquality1=new TH1F("dHist_showerquality1",";FCAL shower quality",100,0,1);
	dHist_showerquality2=new TH1F("dHist_showerquality2",";FCAL shower quality",100,0,1);
	dHist_showerquality1_cut=new TH1F("dHist_showerquality1_cut",";FCAL shower quality",100,0,1);
	dHist_showerquality2_cut=new TH1F("dHist_showerquality2_cut",";FCAL shower quality",100,0,1);
	dHist_MissingMassSquared = new TH1F("MissingMassSquared", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
	dHist_MissingMassSquared_cut = new TH1F("MissingMassSquared_cut", ";Missing Mass Squared (GeV/c^{2})^{2}", 600, -0.06, 0.06);
	dHist_BeamEnergy = new TH1F("BeamEnergy", ";Beam Energy (GeV)", 600, 0.0, 12.0);
	dHist_BeamEnergy_accid = new TH1F("BeamEnergy_accid", ";Beam Energy (GeV)", 600, 0.0, 12.0);
	dHist_BeamEnergy_accid_sideband = new TH1F("BeamEnergy_accid_sideband", ";Beam Energy (GeV)", 600, 0.0, 12.0);
	dHist_BeamEnergy_deltapp = new TH1F("BeamEnergy_deltapp", ";Beam Energy (GeV)", 600, 0.0, 12.0);
	dHist_Chi2=new TH1F("Chi2",";#chi^{2}/NDF",500,0,75);
	dHist_Chi2_cut=new TH1F("Chi2_cut",";#chi^{2}/NDF",500,0,75);
	dHist_Eunusedshowers=new TH1F("dHist_Eunusedshowers",";E_{UnusedShowers}",400,0,1);
	dHist_tmand=new TH1F("dHist_tmand",";-t[GeV/c^{2}]",400,0,10);
	dHist_tmand_cut=new TH1F("dHist_tmand_cut",";-t[GeV/c^{2}]",400,0,10);
	dHist_prot_vert=new TH1F("prot_vert",";p_{vert}^{z}",400,0,200);
	dHist_prot_vert_cut=new TH1F("prot_vert_cut",";p_{vert}^{z}",400,0,200);
	dHist_prot_perpvert=new TH1F("prot_perpvert",";p_{vert}^{#perp}",200,0,2);
	dHist_prot_perpvert_cut=new TH1F("prot_perpvert_cut",";p_{vert}^{#perp}",200,0,2);
	dHist_deltatRF_weight=new TH1F("dHist_deltatRF_weight",";#Delta t_{RF}",400,-20,20);
	dHist_deltatRF=new TH1F("dHist_deltatRF",";#Delta t_{RF}",400,-20,20);
	dHist_EtaMass=new TH1F("EtaMass",";M(#eta)",400,0.2,1.);
	dHist_EtaMass_measured=new TH1F("EtaMass_measured",";M(#eta)",400,0.2,1.);
	dHist_Chi2PostEtaPrime=new TH1F("Chi2PostEtaPrime",";#chi^{2}/NDF",200,0,30);
	dHist_Chi2PostEtaPrime_cut=new TH1F("Chi2PostEtaPrime_cut",";#chi^{2}/NDF",200,0,30);
	dHist_EtaPrimeMass=new TH1F("EtaPrimeMass",";M(#eta')",200,0.8,1.3);
	dHist_EtaPrimeMass_accid_sideband=new TH1F("EtaPrimeMass_accid_sideband",";M(#eta')",400,0.8,1.3);
	dHist_EtaPrimeMass_accid=new TH1F("EtaPrimeMass_accid",";M(#eta')",400,0.8,1.3);
	dHist_DeltaPPMass_accid=new TH1F("DeltaPPMass_accid",";M(#Delta^{++})",200,0.9,3.5);
	dHist_DeltaPPMass_accid_cut=new TH1F("DeltaPPMass_accid_cut",";M(#Delta^{++})",200,0.9,3.5);
	dHist_EtaPrimePiMinusMass_accid=new TH1F("EtaPrimePiMinusMass_accid",";M(#eta'#pi^{-})",75,1,3);
	dHist_EtaPrimePiMinusMass_accid_sideband=new TH1F("EtaPrimePiMinusMass_accid_sideband",";M(#eta'#pi^{-})",75,1,3);
	dHist_EtaPrimePiMinusMass_accid_sideband_tcut=new TH1F("EtaPrimePiMinusMass_accid_sideband_tcut",";M(#eta'#pi^{-})",75,1,3);
	dHist_Bayron_pipl2=new TH1F("Bayron_pipl2",";M(p #pi_{2}^{+})",200,0.9,3.5);
	dHist_Bayron_pimi1=new TH1F("Bayron_pimi1",";M(p #pi_{1}^{-})",200,0.9,3.5);
	dHist_Bayron_pimi2=new TH1F("Bayron_pimi2",";M(p #pi_{2}^{-})",200,0.9,3.5);
	dHist_rho_pim1pip1=new TH1F("rho_pim1pip1",";M(#pi_{1}^{-} #pi_{1}^{+})",200,0.1,1.3);
	dHist_rho_pim1pip2=new TH1F("rho_pim1pip2",";M(#pi_{1}^{-} #pi_{2}^{+})",200,0.1,1.3);
	dHist_rho_pim1pim2=new TH1F("rho_pim1pim2",";M(#pi_{1}^{-} #pi_{2}^{-})",200,0.1,1.3);
	dHist_rho_pip1pim2=new TH1F("rho_pip1pim2",";M(#pi_{1}^{+} #pi_{2}^{-})",200,0.1,1.3);
	dHist_rho_pip1pip2=new TH1F("rho_pip1pip2",";M(#pi_{1}^{+} #pi_{2}^{+})",200,0.1,1.3);
	dHist_rho_pim2pip2=new TH1F("rho_pim2pip2",";M(#pi_{2}^{-} #pi_{2}^{+})",200,0.1,1.3);
	dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_accid=new TH2F("CosThetaEta_GJ_vs_EtaPiMinusMass_accid",";M(#pi^{-}#eta); cos#theta_{#pi}",50,0.9,3.5,50,-1,1);
	dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_accid_sideband=new TH2F("CosThetaEta_GJ_vs_EtaPiMinusMass_accid_sideband",";M(#pi^{-}#eta); cos#theta_{#pi}",50,0.9,3.5,50,-1,1);



	      
	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - MAIN TREE *************************/

	//EXAMPLE MAIN TREE CUSTOM BRANCHES (OUTPUT ROOT FILE NAME MUST FIRST BE GIVEN!!!! (ABOVE: TOP)):
	//The type for the branch must be included in the brackets
	//1st function argument is the name of the branch
	//2nd function argument is the name of the branch that contains the size of the array (for fundamentals only)
	/*
	dTreeInterface->Create_Branch_Fundamental<Int_t>("my_int"); //fundamental = char, int, float, double, etc.
	dTreeInterface->Create_Branch_FundamentalArray<Int_t>("my_int_array", "my_int");
	dTreeInterface->Create_Branch_FundamentalArray<Float_t>("my_combo_array", "NumCombos");
	dTreeInterface->Create_Branch_NoSplitTObject<TLorentzVector>("my_p4");
	dTreeInterface->Create_Branch_ClonesArray<TLorentzVector>("my_p4_array");
	*/

	/************************** EXAMPLE USER INITIALIZATION: CUSTOM OUTPUT BRANCHES - FLAT TREE *************************/
	
 dFlatTreeInterface->Create_Branch_Fundamental<Int_t>("pol");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Mandlestam_t");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("KinFitChiSqPerNDF");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("EtaPrimePiMinusMass");
    
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("tagWeight");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("sidebandWeight");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("EtaPrime_Px");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("EtaPrime_Py");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("EtaPrime_Pz");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("EtaPrime_E");
    
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_Px");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_Py");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_Pz");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Proton_E");
    
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiPlus_Px");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiPlus_Py");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiPlus_Pz");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiPlus_E");
    
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiMinus_Px");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiMinus_Py");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiMinus_Pz");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("PiMinus_E");
    
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Beam_Px");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Beam_Py");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Beam_Pz");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Beam_E");
    
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Target_Px");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Target_Py");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Target_Pz");
    dFlatTreeInterface->Create_Branch_Fundamental<Double_t>("Target_E");
	


	/************************************* ADVANCED EXAMPLE: CHOOSE BRANCHES TO READ ************************************/

	//TO SAVE PROCESSING TIME
		//If you know you don't need all of the branches/data, but just a subset of it, you can speed things up
		//By default, for each event, the data is retrieved for all branches
		//If you know you only need data for some branches, you can skip grabbing data from the branches you don't need
		//Do this by doing something similar to the commented code below

	//dTreeInterface->Clear_GetEntryBranches(); //now get none
	//dTreeInterface->Register_GetEntryBranch("Proton__P4"); //manually set the branches you want

	/************************************** DETERMINE IF ANALYZING SIMULATED DATA *************************************/

	dIsMC = (dTreeInterface->Get_Branch("MCWeight") != NULL);

}

Bool_t DSelector_pippimetapr__B3_M35_2018::Process(Long64_t locEntry)
{
	// The Process() function is called for each entry in the tree. The entry argument
	// specifies which entry in the currently loaded tree is to be processed.
	//
	// This function should contain the "body" of the analysis. It can contain
	// simple or elaborate selection criteria, run algorithms on the data
	// of the event and typically fill histograms.
	//
	// The processing can be stopped by calling Abort().
	// Use fStatus to set the return value of TTree::Process().
	// The return value is currently not used.

	//CALL THIS FIRST
	DSelector::Process(locEntry); //Gets the data from the tree for the entry
	//cout << "RUN " << Get_RunNumber() << ", EVENT " << Get_EventNumber() << endl;
	//TLorentzVector locProductionX4 = Get_X4_Production();

	/******************************************** GET POLARIZATION ORIENTATION ******************************************/

	//Only if the run number changes
	//RCDB environment must be setup in order for this to work! (Will return false otherwise)
	UInt_t locRunNumber = Get_RunNumber();
	if(locRunNumber != dPreviousRunNumber)
	{
		dIsPolarizedFlag = dAnalysisUtilities.Get_IsPolarizedBeam(locRunNumber, dIsPARAFlag);
		dPreviousRunNumber = locRunNumber;
		PolDirection = -1;
		dAnalysisUtilities.Get_PolarizationAngle(locRunNumber,PolDirection);
	}

	/********************************************* SETUP UNIQUENESS TRACKING ********************************************/

	//ANALYSIS ACTIONS: Reset uniqueness tracking for each action
	//For any actions that you are executing manually, be sure to call Reset_NewEvent() on them here
	Reset_Actions_NewEvent();
	dAnalyzeCutActions->Reset_NewEvent(); // manual action, must call Reset_NewEvent()

	//PREVENT-DOUBLE COUNTING WHEN HISTOGRAMMING
		//Sometimes, some content is the exact same between one combo and the next
			//e.g. maybe two combos have different beam particles, but the same data for the final-state
		//When histogramming, you don't want to double-count when this happens: artificially inflates your signal (or background)
		//So, for each quantity you histogram, keep track of what particles you used (for a given combo)
		//Then for each combo, just compare to what you used before, and make sure it's unique

	//EXAMPLE 1: Particle-specific info:
	set<Int_t> locUsedSoFar_BeamEnergy; //Int_t: Unique ID for beam particles. set: easy to use, fast to search

	//EXAMPLE 2: Combo-specific info:
		//In general: Could have multiple particles with the same PID: Use a set of Int_t's
		//In general: Multiple PIDs, so multiple sets: Contain within a map
		//Multiple combos: Contain maps within a set (easier, faster to search)
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_MissingMass;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_EtaPrimePiMinusMass;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_EtaPrimeMass;
	set<map<Particle_t, set<Int_t> > > locUsedSoFar_DeltaMass;

	//INSERT USER ANALYSIS UNIQUENESS TRACKING HERE

	/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

	/*
	Int_t locMyInt = 7;
	dTreeInterface->Fill_Fundamental<Int_t>("my_int", locMyInt);

	TLorentzVector locMyP4(4.0, 3.0, 2.0, 1.0);
	dTreeInterface->Fill_TObject<TLorentzVector>("my_p4", locMyP4);

	for(int loc_i = 0; loc_i < locMyInt; ++loc_i)
		dTreeInterface->Fill_Fundamental<Int_t>("my_int_array", 3*loc_i, loc_i); //2nd argument = value, 3rd = array index
	*/

	/************************************************* LOOP OVER COMBOS *************************************************/

	//Loop over combos
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i)
	{
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);

		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut()) // Is false when tree originally created
			continue; // Combo has been cut previously

		/********************************************** GET PARTICLE INDICES *********************************************/

		//Used for tracking uniqueness when filling histograms, and for determining unused particles

		//Step 0
		Int_t locBeamID = dComboBeamWrapper->Get_BeamID();
		Int_t locPiPlus1TrackID = dPiPlus1Wrapper->Get_TrackID();
		Int_t locPiMinus1TrackID = dPiMinus1Wrapper->Get_TrackID();
		Int_t locProtonTrackID = dProtonWrapper->Get_TrackID();

		//Step 1
		Int_t locPiMinus2TrackID = dPiMinus2Wrapper->Get_TrackID();
		Int_t locPiPlus2TrackID = dPiPlus2Wrapper->Get_TrackID();

		//Step 2
		Int_t locPhoton1NeutralID = dPhoton1Wrapper->Get_NeutralID();
		Int_t locPhoton2NeutralID = dPhoton2Wrapper->Get_NeutralID();

		/*********************************************** GET FOUR-MOMENTUM **********************************************/

		// Get P4's: //is kinfit if kinfit performed, else is measured
		//dTargetP4 is target p4
		//Step 0
		TLorentzVector locBeamP4 = dComboBeamWrapper->Get_P4();
		TLorentzVector locPiPlus1P4 = dPiPlus1Wrapper->Get_P4();
		TLorentzVector locPiMinus1P4 = dPiMinus1Wrapper->Get_P4();
		TLorentzVector locProtonP4 = dProtonWrapper->Get_P4();
		//Step 1
		TLorentzVector locPiMinus2P4 = dPiMinus2Wrapper->Get_P4();
		TLorentzVector locPiPlus2P4 = dPiPlus2Wrapper->Get_P4();
		//Step 2
		TLorentzVector locDecayingEtaP4 = dDecayingEtaWrapper->Get_P4();
		TLorentzVector locPhoton1P4 = dPhoton1Wrapper->Get_P4();
		TLorentzVector locPhoton2P4 = dPhoton2Wrapper->Get_P4();

		// Get Measured P4's:
		//Step 0
		TLorentzVector locBeamP4_Measured = dComboBeamWrapper->Get_P4_Measured();
		TLorentzVector locPiPlus1P4_Measured = dPiPlus1Wrapper->Get_P4_Measured();
		TLorentzVector locPiMinus1P4_Measured = dPiMinus1Wrapper->Get_P4_Measured();
		TLorentzVector locProtonP4_Measured = dProtonWrapper->Get_P4_Measured();
		//Step 1
		TLorentzVector locPiMinus2P4_Measured = dPiMinus2Wrapper->Get_P4_Measured();
		TLorentzVector locPiPlus2P4_Measured = dPiPlus2Wrapper->Get_P4_Measured();
		//Step 2
		TLorentzVector locPhoton1P4_Measured = dPhoton1Wrapper->Get_P4_Measured();
		TLorentzVector locPhoton2P4_Measured = dPhoton2Wrapper->Get_P4_Measured();

		/********************************************* GET COMBO RF TIMING INFO *****************************************/

		TLorentzVector locBeamX4_Measured = dComboBeamWrapper->Get_X4_Measured();
		//Double_t locBunchPeriod = dAnalysisUtilities.Get_BeamBunchPeriod(Get_RunNumber());
		// Double_t locDeltaT_RF = dAnalysisUtilities.Get_DeltaT_RF(Get_RunNumber(), locBeamX4_Measured, dComboWrapper);
		// Int_t locRelBeamBucket = dAnalysisUtilities.Get_RelativeBeamBucket(Get_RunNumber(), locBeamX4_Measured, dComboWrapper); // 0 for in-time events, non-zero integer for out-of-time photons
		// Int_t locNumOutOfTimeBunchesInTree = XXX; //YOU need to specify this number
			//Number of out-of-time beam bunches in tree (on a single side, so that total number out-of-time bunches accepted is 2 times this number for left + right bunches) 

		// Bool_t locSkipNearestOutOfTimeBunch = true; // True: skip events from nearest out-of-time bunch on either side (recommended).
		// Int_t locNumOutOfTimeBunchesToUse = locSkipNearestOutOfTimeBunch ? locNumOutOfTimeBunchesInTree-1:locNumOutOfTimeBunchesInTree; 
		// Double_t locAccidentalScalingFactor = dAnalysisUtilities.Get_AccidentalScalingFactor(Get_RunNumber(), locBeamP4.E(), dIsMC); // Ideal value would be 1, but deviations require added factor, which is different for data and MC.
		// Double_t locAccidentalScalingFactorError = dAnalysisUtilities.Get_AccidentalScalingFactorError(Get_RunNumber(), locBeamP4.E()); // Ideal value would be 1, but deviations observed, need added factor.
		// Double_t locHistAccidWeightFactor = locRelBeamBucket==0 ? 1 : -locAccidentalScalingFactor/(2*locNumOutOfTimeBunchesToUse) ; // Weight by 1 for in-time events, ScalingFactor*(1/NBunches) for out-of-time
		// if(locSkipNearestOutOfTimeBunch && abs(locRelBeamBucket)==1) { // Skip nearest out-of-time bunch: tails of in-time distribution also leak in
		// 	dComboWrapper->Set_IsComboCut(true); 
		// 	continue; 
		// } 

		/********************************************* COMBINE FOUR-MOMENTUM ********************************************/

		// DO YOUR STUFF HERE

		// Combine 4-vectors
		TLorentzVector locMissingP4_Measured = locBeamP4_Measured + dTargetP4;
		locMissingP4_Measured -= locPiPlus1P4_Measured + locPiMinus1P4_Measured + locProtonP4_Measured + locPiMinus2P4_Measured + locPiPlus2P4_Measured + locPhoton1P4_Measured + locPhoton2P4_Measured;

		/******************************************** EXECUTE ANALYSIS ACTIONS *******************************************/
		dHist_BeamEnergy->Fill(locBeamP4.E());
		// Loop through the analysis actions, executing them in order for the active particle combo
		dAnalyzeCutActions->Perform_Action(); // Must be executed before Execute_Actions()
		if(!Execute_Actions()) //if the active combo fails a cut, IsComboCutFlag automatically set
			continue;

	TVector3 locPhoton1P3=locPhoton1P4.Vect();
        TVector3 locPhoton2P3=locPhoton2P4.Vect();
        
        
        double ThetaPhoton1=locPhoton1P3.Theta()*180.0/TMath::Pi();
        double ThetaPhoton2=locPhoton2P3.Theta()*180.0/TMath::Pi();
        
        
        double ThetaMin=10.;
        double ThetaMax=12.5;
        
	dHist_phot1_theta->Fill(ThetaPhoton1);
        if(ThetaPhoton1 < ThetaMax && ThetaPhoton1 > ThetaMin){
            dComboWrapper->Set_IsComboCut(true);
            continue;
        }
	dHist_phot1_theta_cut->Fill(ThetaPhoton1);
        
	dHist_phot2_theta->Fill(ThetaPhoton2);
        if ( ThetaPhoton2 < ThetaMax && ThetaPhoton2 > ThetaMin )
        {
            dComboWrapper->Set_IsComboCut(true);
            continue;
        }
	dHist_phot2_theta_cut->Fill(ThetaPhoton2);


	double Photon1_showerquality = dPhoton1Wrapper->Get_Shower_Quality();
	double Photon2_showerquality = dPhoton2Wrapper->Get_Shower_Quality();
	double lim_showerquality=0.5;

	dHist_showerquality1->Fill(Photon1_showerquality);
	dHist_showerquality2->Fill(Photon2_showerquality);
	if ( Photon1_showerquality< lim_showerquality || Photon2_showerquality < lim_showerquality )
	  {
            dComboWrapper->Set_IsComboCut(true);
            continue;
	  }
	dHist_showerquality1_cut->Fill(Photon1_showerquality);
	dHist_showerquality2_cut->Fill(Photon2_showerquality);


	TLorentzVector locEtaP4=locPhoton1P4+locPhoton2P4;
        TLorentzVector locEtaMeasP4=locPhoton1P4_Measured+locPhoton2P4_Measured;
        
        //TLorentzVector locEtaPrimeP4=locPiMinus2P4+locPiPlus2P4+locEtaP4;
        TLorentzVector locEtaPrimeP4=locPiMinus2P4+locPiPlus2P4+locDecayingEtaP4;
        TLorentzVector locDeltaPPP4=locProtonP4+locPiPlus1P4;
        TLorentzVector locPiMinusProtonP4=locProtonP4+locPiMinus1P4;
        TLorentzVector locPiPlusProtonP4=locProtonP4+locPiPlus1P4;
        TLorentzVector locPiMinus1PiPlus1P4=locPiMinus1P4+locPiPlus1P4;
        TLorentzVector locEtaPrimePiMinusP4=locEtaPrimeP4+locPiMinus1P4;
        
        double locEtaMass=locEtaP4.M();
        double locEtaMassMeas=locEtaMeasP4.M();
        
        double locEtaPrimeMass=locEtaPrimeP4.M();
        double locDeltaPPMass=locDeltaPPP4.M();
        double locPiMinusProtonMass=locPiMinusProtonP4.M();
        double locPiPlusProtonMass=locPiPlusProtonP4.M();
        double locPiMinus1PiPlus1Mass=locPiMinus1PiPlus1P4.M();
        double locEtaPrimePiMinusMass=locEtaPrimePiMinusP4.M();
        
        TLorentzVector Mandlestam_t=locDeltaPPP4-dTargetP4;
        double locMandlestam_t=(-1)*Mandlestam_t.Mag2();
        


	//Looking at other combinations of particles to eliminate bayronic contributions and different meson production chenels

	TLorentzVector locBayron_pipl2=locProtonP4+locPiPlus2P4; //pi+ forming etaprime
	TLorentzVector locBayron_pimi1=locProtonP4+locPiMinus1P4; //pi- from resonance decay
	TLorentzVector locBayron_pimi2=locProtonP4+locPiMinus2P4; //pi- forming etaprime
	TLorentzVector locrho_pim1pip1=locPiMinus1P4+locPiPlus1P4;// pi+ from Delta and spare pi-
	TLorentzVector locrho_pim1pip2=locPiMinus1P4+locPiPlus2P4; //pi+ from etaprime and spare pi-
	TLorentzVector locrho_pim1pim2=locPiMinus1P4+locPiMinus2P4;//pi- from etaprime and spare pi-
	TLorentzVector locrho_pip1pim2=locPiPlus1P4+locPiMinus2P4;//pi- from etaprime and spare pi-
	TLorentzVector locrho_pip1pip2=locPiPlus1P4+locPiPlus2P4;//pi- from etaprime and spare pi-
	TLorentzVector locrho_pim2pip2=locPiMinus2P4+locPiPlus2P4;//rho in etaprime decay


	double locBayron_pipl2_M=locBayron_pipl2.M();
	double locBayron_pimi1_M=locBayron_pimi1.M();
	double locBayron_pimi2_M=locBayron_pimi2.M();
	double locrho_pim1pip1_M=locrho_pim1pip1.M();
	double locrho_pim1pip2_M=locrho_pim1pip2.M();
	double locrho_pim1pim2_M=locrho_pim1pim2.M();
	double locrho_pip1pim2_M=locrho_pip1pim2.M();
	double locrho_pip1pip2_M=locrho_pip1pip2.M();
	double locrho_pim2pip2_M=locrho_pim2pip2.M();

        ///CoM Boost
        TLorentzVector locCoMP4=locBeamP4 + dTargetP4;
        TVector3 boostCoM=-(locCoMP4.Vect())*(1.0/locCoMP4.E());
        //Boost in CoM
        TLorentzVector locBeamP4_CM=locBeamP4;
        TLorentzVector locEtaPrimeP4_CM=locEtaPrimeP4;
        TLorentzVector locPiMinusP4_CM=locPiMinus1P4;
        TLorentzVector locEtaPrimePiMinusP4_CM=locEtaPrimePiMinusP4;
        TLorentzVector locDeltaPPP4_CM=locDeltaPPP4;
        
        locDeltaPPP4_CM.Boost(boostCoM);
        locEtaPrimePiMinusP4_CM.Boost(boostCoM);
        locBeamP4_CM.Boost(boostCoM);
        locEtaPrimeP4_CM.Boost(boostCoM);
        locPiMinusP4_CM.Boost(boostCoM);
        
        //GJ Boost
        TVector3 boostGJ=-(locEtaPrimePiMinusP4_CM.Vect())*(1.0/locEtaPrimePiMinusP4_CM.E());
        
        //Define GJ frame vectors
        TLorentzVector locEtaPrimePiMinusP4_GJ=locEtaPrimePiMinusP4_CM;
        TLorentzVector locEtaPrimeP4_GJ=locEtaPrimeP4_CM;
        TLorentzVector locPiminusP4_GJ=locPiMinusP4_CM;
        TLorentzVector locBeamP4GJ=locBeamP4_CM;
        
        //Boost in GJ
        locEtaPrimePiMinusP4_GJ.Boost(boostGJ);
        locBeamP4GJ.Boost(boostGJ);
        locEtaPrimeP4_GJ.Boost(boostGJ);
        locPiminusP4_GJ.Boost(boostGJ);
        
        TVector3 z_GJ;
        z_GJ.SetXYZ(locBeamP4GJ.X(),locBeamP4GJ.Y(),locBeamP4GJ.Z());//z GJ
        TVector3 z_hat_GJ=z_GJ.Unit();
        TVector3 y_GJ=locBeamP4_CM.Vect().Cross(locEtaPrimePiMinusP4_CM.Vect());
        TVector3 y_hat_GJ=y_GJ.Unit();
        TVector3 x_hat_GJ=y_hat_GJ.Cross(z_hat_GJ);//x hat GJ
        
        TVector3 v(locEtaPrimeP4_GJ.Vect()*x_hat_GJ, locEtaPrimeP4_GJ.Vect()*y_hat_GJ,
                   locEtaPrimeP4_GJ.Vect()*z_hat_GJ);
        double cosTheta = v.CosTheta();
        double theta = v.Theta();
        double phi = v.Phi()*180./TMath::Pi();
        
        double locCosThetaEtaGJ = (locBeamP4GJ.Vect().Unit()).Dot(locEtaPrimeP4_GJ.Vect().Unit());
        double locCosThetaPiMinusGJ = (locBeamP4GJ.Vect().Unit()).Dot(locPiminusP4_GJ.Vect().Unit());


 ////////////////////////////////////////////////////////
        double locRFTime = dComboWrapper->Get_RFTime();
        TLorentzVector locBeamX4 = dComboBeamWrapper->Get_X4();
        
        double locBeamDeltaT = locBeamX4.T() - (locRFTime + (locBeamX4.Z() - dTargetCenter.Z())/29.9792458);
        

	dHist_deltatRF->Fill(locBeamDeltaT);
        double tagWeight=0.;
        if  (fabs(locBeamDeltaT) > 2.004) tagWeight=-1./8.;
        else if(fabs(locBeamDeltaT) < 2.004) tagWeight=1.;
        dHist_deltatRF_weight->Fill(locBeamDeltaT,tagWeight);




	double etaprime_mean=0.958;
	double etaprime_sigma=0.009;
        double xp4sigma=etaprime_mean+4.*etaprime_sigma;
        double xm4sigma=etaprime_mean-4.*etaprime_sigma;
        double xp8sigma=etaprime_mean+8.*etaprime_sigma;
        double xm8sigma=etaprime_mean-8.*etaprime_sigma;
        
        double xp2sigma=etaprime_mean+2.*etaprime_sigma;
        double xm2sigma=etaprime_mean-2.*etaprime_sigma;
        double xp6sigma=etaprime_mean+6.*etaprime_sigma;
        double xm6sigma=etaprime_mean-6.*etaprime_sigma;
        
        double xp3sigma=etaprime_mean+3.*etaprime_sigma;
        double xm3sigma=etaprime_mean-3.*etaprime_sigma;
        double xp9sigma=etaprime_mean+9.*etaprime_sigma;
        double xm9sigma=etaprime_mean-9.*etaprime_sigma;

        
        double sidebandWeight=0;
        if (locEtaPrimeMass>xm3sigma && locEtaPrimeMass<xp3sigma) sidebandWeight=1.;
        else if (locEtaPrimeMass<=xm6sigma && locEtaPrimeMass>xm9sigma) sidebandWeight=-1.;
        else if (locEtaPrimeMass<=xp9sigma && locEtaPrimeMass>=xp6sigma) sidebandWeight=-1.;
        


   TLorentzVector locProtonX4_Measured = dComboBeamWrapper->Get_X4_Measured();
        
        double locProtonX = locProtonX4_Measured.X();
        double locProtonY = locProtonX4_Measured.Y();
        double locProtonZ = locProtonX4_Measured.Z();
        
        double dMinProtonZ=52;
        double dMaxProtonZ=78;
        
	dHist_prot_vert->Fill(locProtonX4_Measured.Z());
        if ( locProtonX4_Measured.Z() < dMinProtonZ || locProtonX4_Measured.Z() > dMaxProtonZ )
        {
            dComboWrapper->Set_IsComboCut(true);
            continue;
        }
        dHist_prot_vert_cut->Fill(locProtonX4_Measured.Z());


	dHist_prot_perpvert->Fill(sqrt(locProtonX*locProtonX+locProtonY*locProtonY));
        if (sqrt(locProtonX*locProtonX+locProtonY*locProtonY) > 1.0){
            dComboWrapper->Set_IsComboCut(true);
            continue;
        }
	dHist_prot_perpvert_cut->Fill(sqrt(locProtonX*locProtonX+locProtonY*locProtonY));
        
	double locConfidenceLevel = dComboWrapper->Get_ConfidenceLevel_KinFit("");
        double locKinFitChiSqPerNDF = dComboWrapper->Get_ChiSq_KinFit("")/dComboWrapper->Get_NDF_KinFit("");
        
        
        
        dHist_Chi2->Fill(locKinFitChiSqPerNDF);
        dHist_Chi2PostEtaPrime->Fill(locKinFitChiSqPerNDF,tagWeight);

        if(locKinFitChiSqPerNDF>15){
            dComboWrapper->Set_IsComboCut(true);
            continue;
        }
	dHist_Chi2PostEtaPrime_cut->Fill(locKinFitChiSqPerNDF,tagWeight);





		//if you manually execute any actions, and it fails a cut, be sure to call:
			//dComboWrapper->Set_IsComboCut(true);

		/**************************************** EXAMPLE: FILL CUSTOM OUTPUT BRANCHES **************************************/

		/*
		TLorentzVector locMyComboP4(8.0, 7.0, 6.0, 5.0);
		//for arrays below: 2nd argument is value, 3rd is array index
		//NOTE: By filling here, AFTER the cuts above, some indices won't be updated (and will be whatever they were from the last event)
			//So, when you draw the branch, be sure to cut on "IsComboCut" to avoid these.
		dTreeInterface->Fill_Fundamental<Float_t>("my_combo_array", -2*loc_i, loc_i);
		dTreeInterface->Fill_TObject<TLorentzVector>("my_p4_array", locMyComboP4, loc_i);
		*/

		/**************************************** EXAMPLE: HISTOGRAM BEAM ENERGY *****************************************/

		//Histogram beam energy (if haven't already)
		if(locUsedSoFar_BeamEnergy.find(locBeamID) == locUsedSoFar_BeamEnergy.end())
		{
		  //	dHist_BeamEnergy->Fill(locBeamP4.E()); // Fills in-time and out-of-time beam photon combos
			//dHist_BeamEnergy->Fill(locBeamP4.E(),locHistAccidWeightFactor); // Alternate version with accidental subtraction

			locUsedSoFar_BeamEnergy.insert(locBeamID);
		}

		/************************************ EXAMPLE: HISTOGRAM MISSING MASS SQUARED ************************************/

		//Missing Mass Squared
		double locMissingMassSquared = locMissingP4_Measured.M2();
		double locE_unused_showers= dComboWrapper->Get_Energy_UnusedShowers();
		//Uniqueness tracking: Build the map of particles used for the missing mass
			//For beam: Don't want to group with final-state photons. Instead use "Unknown" PID (not ideal, but it's easy).
		map<Particle_t, set<Int_t> > locUsedThisCombo_MissingMass;
		locUsedThisCombo_MissingMass[Unknown].insert(locBeamID); //beam
		locUsedThisCombo_MissingMass[PiPlus].insert(locPiPlus1TrackID);
		locUsedThisCombo_MissingMass[PiMinus].insert(locPiMinus1TrackID);
		locUsedThisCombo_MissingMass[Proton].insert(locProtonTrackID);
		locUsedThisCombo_MissingMass[PiMinus].insert(locPiMinus2TrackID);
		locUsedThisCombo_MissingMass[PiPlus].insert(locPiPlus2TrackID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton1NeutralID);
		locUsedThisCombo_MissingMass[Gamma].insert(locPhoton2NeutralID);

		//compare to what's been used so far
			if(locUsedSoFar_MissingMass.find(locUsedThisCombo_MissingMass) == locUsedSoFar_MissingMass.end())
			{

		Bool_t MissingMassSquaredcut = (locMissingMassSquared > -0.02) && (locMissingMassSquared < 0.02);
		Bool_t twindow = (locMandlestam_t > 0.) && (locMandlestam_t < 1);
		Bool_t pimip_deltapp = (locDeltaPPMass < 1.3) ;



		//Histograms before cuts
		//unique missing mass combo: histogram it, and register this combo of particles
		dHist_MissingMassSquared->Fill(locMissingMassSquared, tagWeight); // Fills in-time and out-of-time beam photon combos
		dHist_tmand->Fill(locMandlestam_t, tagWeight*sidebandWeight);
		dHist_BeamEnergy_accid->Fill(locBeamP4.E(), tagWeight);
		dHist_BeamEnergy_accid_sideband->Fill(locBeamP4.E(), tagWeight*sidebandWeight);
		if ( pimip_deltapp) {
		  dHist_BeamEnergy_deltapp->Fill(locBeamP4.E(), tagWeight*sidebandWeight);
		  if (MissingMassSquaredcut)dHist_tmand_cut->Fill(locMandlestam_t, tagWeight*sidebandWeight);
		  if ( twindow){
		  dHist_Chi2_cut->Fill(locKinFitChiSqPerNDF, tagWeight*sidebandWeight);
		  dHist_MissingMassSquared_cut->Fill(locMissingMassSquared, tagWeight*sidebandWeight);
		  dHist_EtaMass->Fill(locEtaMass, tagWeight);
		  dHist_EtaMass_measured->Fill(locEtaMassMeas, tagWeight);
		  }
		}
		dHist_DeltaPPMass_accid->Fill(locDeltaPPMass, tagWeight);
		if ( twindow &&  MissingMassSquaredcut)	dHist_DeltaPPMass_accid_cut->Fill(locDeltaPPMass, tagWeight*sidebandWeight);


		    //Delta++ window cut, no etaprime window cut since I'm doing sideband subtraction
		    if (pimip_deltapp && twindow &&  MissingMassSquaredcut){

		    dHist_EtaPrimeMass->Fill(locEtaPrimeMass);
		    //histograms with accidental subtraction
		    dHist_EtaPrimeMass_accid->Fill(locEtaPrimeMass, tagWeight);
		    dHist_EtaPrimePiMinusMass_accid->Fill(locEtaPrimePiMinusMass, tagWeight);
		    dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_accid->Fill(locEtaPrimePiMinusMass, cosTheta, tagWeight);
		   
		    //histograms with accidental and etaprime sideband subtraction
		    dHist_EtaPrimeMass_accid_sideband->Fill(locEtaPrimeMass, tagWeight*sidebandWeight);
		    dHist_EtaPrimePiMinusMass_accid_sideband->Fill(locEtaPrimePiMinusMass, tagWeight*sidebandWeight);
		    if(locMandlestam_t > 0.1) dHist_EtaPrimePiMinusMass_accid_sideband_tcut->Fill(locEtaPrimePiMinusMass, tagWeight*sidebandWeight);
		    dHist_CosThetaEta_GJ_vs_EtaPiMinusMass_accid_sideband->Fill(locEtaPrimePiMinusMass, cosTheta, tagWeight*sidebandWeight);



		     //Different combinations of pions and p to look for bayronic and resonance backgrounds
		      dHist_Bayron_pipl2->Fill(locBayron_pipl2_M, tagWeight*sidebandWeight);
		      dHist_Bayron_pimi1->Fill(locBayron_pimi1_M, tagWeight*sidebandWeight);
		      dHist_Bayron_pimi2->Fill(locBayron_pimi2_M, tagWeight*sidebandWeight);
		      dHist_rho_pim1pip1->Fill(locrho_pim1pip1_M, tagWeight*sidebandWeight);
		      dHist_rho_pim1pip2->Fill(locrho_pim1pip2_M, tagWeight*sidebandWeight);
		      dHist_rho_pim1pim2->Fill(locrho_pim1pim2_M, tagWeight*sidebandWeight);
		      dHist_rho_pip1pim2->Fill(locrho_pip1pim2_M, tagWeight*sidebandWeight);
		      dHist_rho_pip1pip2->Fill(locrho_pip1pip2_M, tagWeight*sidebandWeight);
		      dHist_rho_pim2pip2->Fill(locrho_pim2pip2_M, tagWeight*sidebandWeight);

		    }// Delta++ window cut

			//dHist_MissingMassSquared->Fill(locMissingMassSquared,locHistAccidWeightFactor); // Alternate version with accidental subtraction
		    //Unique combo cut
		    	locUsedSoFar_MissingMass.insert(locUsedThisCombo_MissingMass);
		    	}

		//E.g. Cut
		//if((locMissingMassSquared < -0.04) || (locMissingMassSquared > 0.04))
		//{
		//	dComboWrapper->Set_IsComboCut(true);
		//	continue;
		//}

		/****************************************** FILL FLAT TREE (IF DESIRED) ******************************************/

		// RECOMMENDED: FILL ACCIDENTAL WEIGHT
		// dFlatTreeInterface->Fill_Fundamental<Double_t>("accidweight",locHistAccidWeightFactor);

		/*
		//FILL ANY CUSTOM BRANCHES FIRST!!
		Int_t locMyInt_Flat = 7;
		dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int", locMyInt_Flat);

		TLorentzVector locMyP4_Flat(4.0, 3.0, 2.0, 1.0);
		dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4", locMyP4_Flat);

		for(int loc_j = 0; loc_j < locMyInt_Flat; ++loc_j)
		{
			dFlatTreeInterface->Fill_Fundamental<Int_t>("flat_my_int_array", 3*loc_j, loc_j); //2nd argument = value, 3rd = array index
			TLorentzVector locMyComboP4_Flat(8.0, 7.0, 6.0, 5.0);
			dFlatTreeInterface->Fill_TObject<TLorentzVector>("flat_my_p4_array", locMyComboP4_Flat, loc_j);
		}
		*/

		//FILL FLAT TREE
		//Fill_FlatTree(); //for the active combo
	} // end of combo loop

	//FILL HISTOGRAMS: Num combos / events surviving actions
	Fill_NumCombosSurvivedHists();

	/******************************************* LOOP OVER THROWN DATA (OPTIONAL) ***************************************/
/*
	//Thrown beam: just use directly
	if(dThrownBeam != NULL)
		double locEnergy = dThrownBeam->Get_P4().E();

	//Loop over throwns
	for(UInt_t loc_i = 0; loc_i < Get_NumThrown(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dThrownWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/
	/****************************************** LOOP OVER OTHER ARRAYS (OPTIONAL) ***************************************/
/*
	//Loop over beam particles (note, only those appearing in combos are present)
	for(UInt_t loc_i = 0; loc_i < Get_NumBeam(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dBeamWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over charged track hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumChargedHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dChargedHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}

	//Loop over neutral particle hypotheses
	for(UInt_t loc_i = 0; loc_i < Get_NumNeutralHypos(); ++loc_i)
	{
		//Set branch array indices corresponding to this particle
		dNeutralHypoWrapper->Set_ArrayIndex(loc_i);

		//Do stuff with the wrapper here ...
	}
*/

	/************************************ EXAMPLE: FILL CLONE OF TTREE HERE WITH CUTS APPLIED ************************************/
/*
	Bool_t locIsEventCut = true;
	for(UInt_t loc_i = 0; loc_i < Get_NumCombos(); ++loc_i) {
		//Set branch array indices for combo and all combo particles
		dComboWrapper->Set_ComboIndex(loc_i);
		// Is used to indicate when combos have been cut
		if(dComboWrapper->Get_IsComboCut())
			continue;
		locIsEventCut = false; // At least one combo succeeded
		break;
	}
	if(!locIsEventCut && dOutputTreeFileName != "")
		Fill_OutputTree();
*/

	return kTRUE;
}

void DSelector_pippimetapr__B3_M35_2018::Finalize(void)
{
	//Save anything to output here that you do not want to be in the default DSelector output ROOT file.

	//Otherwise, don't do anything else (especially if you are using PROOF).
		//If you are using PROOF, this function is called on each thread,
		//so anything you do will not have the combined information from the various threads.
		//Besides, it is best-practice to do post-processing (e.g. fitting) separately, in case there is a problem.

	//DO YOUR STUFF HERE

	//CALL THIS LAST
	DSelector::Finalize(); //Saves results to the output file
}
