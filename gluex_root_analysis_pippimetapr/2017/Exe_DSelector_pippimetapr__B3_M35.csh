#!/usr/bin/env csh

#root -l <<EOF
#gEnv->SetValue("ProofLite.Sandbox", "/volatile/halld/home/Mariana/PROOF/.proof/");
#.L "$ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C" -x "Run_DSelector_pippimetapr__B3_M35.C()"
#EOF

root -l -x  "$ROOT_ANALYSIS_HOME/scripts/Load_DSelector.C" -x "Run_DSelector_pippimetapr__B3_M35.C()"
