# HadShowerTuningData

A package that uses geant4 to simulate the interaction between single particles and a calorimeter,
and to collect the required data for the tuning of FastSim's model for hadronic showers.

For more information about the calorimeter geometry, see comments in *FastSimulation/HadShowerTuning/python/createGeometry.py*

For more information about the simulation, see comments in *FastSimulation/HadShowerTuning/plugins/HadShowerTuningData.py*

# How to install

    # Create a fresh CMSSW area 
    # ( tested in CMSSW_8_1_0_pre3 and CMSSW_7_4_0 for SCRAM_ARCH=slc6_amd64_gcc493)
    cmsrel CMSSW_A_B_C
    cd CMSSW_A_B_C/src
    cmsenv
    mkdir FastSimulation
    cd FastSimulation
    git clone git@github.com:cms-fastsim/HadShowerTuning.git
    scram b -j 8

# How to configure and run locally

Run as follows

    cmsRun FastSimulation/HadShowerTuning/python/HadShowerTuningData_cfg.py

For more information about the configuration options, see comments in 
*FastSimulation/HadShowerTuning/python/HadShowerTuningData_cfi.py*

# How to run on a batch farm

(tested on lxplus and naf)

Install the job submission tool [grid-control](https://ekptrac.physik.uni-karlsruhe.de/trac/grid-control)

    cd /Path/To/Your/Software
    svn co https://ekptrac.physik.uni-karlsruhe.de/svn/grid-control/tags/stable/grid-control
   
Prepare a working directory for your job submission and submit a single test job to your login machine:

    mkdir /Path/To/Your/WorkingDirectory
    cd /Path/To/Your/WorkingDirectory
    python $CMSSW_BASE/src/FastSimulation/HadShowerTuning/python/prepareJobs.py
    /Path/To/Your/Software/grid-control/go.py -icG job.cfg

Find the resulting root file under /Path/To/Your/WorkingDirectory/results

Find and edit the configuration for the job submission under /Path/To/Your/WorkingDirectory/job.cfg
Pay special attention to:
   * all options under [parameters]
   * all options marked with !!!
   * wall time (maximum job time: lower values lead to higher priority)
   * memory (maximum memory: lower values lead to higher priority)

For more information about grid-control see the [grid-control web site](https://ekptrac.physik.uni-karlsruhe.de/trac/grid-control) and */Path/To/Your/Software/grid-control/docs/documentation.conf*

# How to create geometry files

Geometry files are shipped with this package and reside in the directory *FastSimulation/HadShowerTuning/data*.

These geometry files were generated with a script

    ./FastSimulation/HadShowerTuning/python/createGeometry.py

More information about the geometry files can be found in the script.

In case you wish to use an alternative geometry, 
it is fairly easy to modify the script according to your needs.

# Data format

The output root file of each job contains one directory per generated shower.
The content of each directory is as follows

| type      | name             | description                  | 
|-----------|------------------|------------------------------|
| TVector3	| primary_position | original position of primary |


 KEY: TVector3	primary_momentum;1	A 3D physics vector
 KEY: TVectorT<double>	primary_kinE;1	
 KEY: TVectorT<double>	primary_pdgId;1	
 KEY: TVector3	primary_endVertex_position;1	A 3D physics vector
 KEY: TVector3	primary_endVertex_momentum;1	A 3D physics vector
 KEY: TVectorT<double>	primary_endVertex_kinE;1	
 KEY: TVectorT<double>	primary_endVertex_pdgId;1	
 KEY: TH1F	longEProf_fineBin_mip;1	longEProf_fineBin_mip
 KEY: TH1F	longEProf_fineBin_had;1	longEProf_fineBin_had
 KEY: TH1F	longEProf_fineBin_pi0_1;1	longEProf_fineBin_pi0_1
 KEY: TH1F	longEProf_fineBin_pi0_2;1	longEProf_fineBin_pi0_2
 KEY: TH1F	longEProf_perLayer_mip;1	longEProf_perLayer_mip
 KEY: TH1F	longEProf_perLayer_had;1	longEProf_perLayer_had
 KEY: TH1F	longEProf_perLayer_pi0_1;1	longEProf_perLayer_pi0_1
 KEY: TH1F	longEProf_perLayer_pi0_2;1	longEProf_perLayer_pi0_2
