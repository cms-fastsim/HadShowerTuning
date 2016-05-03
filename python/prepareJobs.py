#!/bin/env python

import os

cfgTemplate = """
from FastSimulation.HadShowerTuning.HadShowerTuningData_cfg import process
process.hadShowerTuningData.geometry = "FastSimulation/HadShowerTuning/data/geometry_@GEOMLABEL@.txt"
process.hadShowerTuningData.outputFile = "output.root"
process.hadShowerTuningData.particleEnergy = @PARTICLEENERGY@
process.hadShowerTuningData.particleType = "@PARTICLETYPE@"
process.hadShowerTuningData.seed = @MY_JOBID@+1
process.hadShowerTuningData.nevents = @NEVENTSPERJOB@
"""

FILE = open("hadShowerTuningData_Template_cfg.py","w")
FILE.write(cfgTemplate)
FILE.close()

jobCfg = """
[global]
module     = CMSSW     ; Job uses user written scripts
backend    = local       ; Submit to local batch system

[jobs]
jobs = 1                 ; !!! SET TO 1 FOR TESTING. CHANGE TO -1 to run all jobs
wall time  = 8:00        ; Jobs will take aproximately 8h
max retry = 0
memory    = 4000

[CMSSW]
project area = {CMSSW_BASE}
config file = hadShowerTuningData_Template_cfg.py
events per job = 1

[storage]
se path = {PWD}/results
se output files = output.root
se output pattern = job_@MY_JOBID@_energy_@PARTICLEENERGY@_@PARTICLETYPE@_@GEOMLABEL@.root

[parameters]
parameters = PARTICLEENERGY NEVENTSPERJOB PARTICLETYPE GEOMLABEL
PARTICLEENERGY = 0.05 0.1 0.2 0.4 0.8 1.5 3 6 10 25 50 100 200 500 1000
GEOMLABEL = standard stretch2 stretch3
PARTICLETYPE = pi+
NEVENTSPERJOB = 1000
repeat = 40

[local]
wms = Host             ; !!! RUN ON HOST FOR TESTING. COMMENT OUT TO RUN ON BATCH SYSTEM
""".format(CMSSW_BASE=os.environ["CMSSW_BASE"],PWD=os.environ["PWD"])

FILE = open("job.cfg","w")
FILE.write(jobCfg)
FILE.close()


