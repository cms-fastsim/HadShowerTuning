# HadShowerTuningData

A package that uses geant4 to simulate the interaction between single particles and a calorimeter,
and to collect the required data for the tuning of FastSim's model for hadronic showers.

For more information about the calorimeter geometry, see comments in *FastSimulation/HadShowerTuning/python/createGeometry.py*

For more information about the simulation, see comments in *FastSimulation/HadShowerTuning/plugins/HadShowerTuningData.py*

# How to install

    # Create a fresh CMSSW area ( >= CMSSW_8_1_0_pre3)
    cmsrel CMSSW_A_B_C
    # Get this package from git
    cd CMSSW_A_B_C/src
    cmsenv
    git cms-init
    git cms-merge-topic lveldere:fastsimulation-hadshowertuning
    scram b -j 8

# How to configure and run

Run as follows

    cmsRun FastSimulation/HadShowerTuning/python/HadShowerTuningData_cfg.py

For more information about the configuration options, see comments in 
*FastSimulation/HadShowerTuning/python/HadShowerTuningData_cfi.py*

# How to create geometry files

Geometry files are shipped with this package and reside in the directory *FastSimulation/HadShowerTuning/data*.

These geometry files were generated with a script

    ./FastSimulation/HadShowerTuning/python/createGeometry.py

More information about the geometry files can be found in the script.

In case you wish to use an alternative geometry, 
it is fairly easy to modify the script according to your needs.
