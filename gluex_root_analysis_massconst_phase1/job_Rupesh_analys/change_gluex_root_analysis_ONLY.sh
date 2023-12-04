#!/bin/bash

export BUILD_SCRIPTS="/group/halld/Software/build_scripts/"

#Remove old ROOT_ANALYSIS_HOME from PATH and LD_LIBRARY_PATH
if [ -z "$ROOT_ANALYSIS_HOME" ]; then eval `$BUILD_SCRIPTS/delpath.pl -b $ROOT_ANALYSIS_HOME/${BMS_OSNAME}/bin/`; fi
if [ -z "$ROOT_ANALYSIS_HOME" ]; then eval `$BUILD_SCRIPTS/delpath.pl -b -l $ROOT_ANALYSIS_HOME/${BMS_OSNAME}/lib/`; fi

xml_to_use="/w/halld-scshelf2101/home/jzarling/GLUEX_TOP/gluex_root_analysis/gluex_root_analysis_jzSaveTLorentzVector2Fundamental.xml"
#source /group/halld/Software/build_scripts/gluex_env_jlab.sh /group/halld/www/halldweb/html/halld_versions/version_4.28.0.xml
#source /group/halld/Software/build_scripts/gluex_env_jlab.sh /group/halld/www/halldweb/html/halld_versions/version_5.1.0.xml 
source /group/halld/Software/build_scripts/gluex_env_jlab.sh /group/halld/www/halldweb/html/halld_versions/version_5.3.0.xml
source $BUILD_SCRIPTS//gluex_env_jlab.sh $xml_to_use
#source /group/halld/Software/build_scripts/gluex_env_jlab.sh /group/halld/www/halldweb/html/halld_versions/version_4.28.0.xml

echo "Done changing gluex_root_analysis (DSelector) to branch jzSaveTLorentzVector2Fundamental"
