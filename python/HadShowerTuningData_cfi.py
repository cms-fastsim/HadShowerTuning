import FWCore.ParameterSet.Config as cms

hadShowerTuningData = cms.EDAnalyzer(
    'HadShowerTuningData',
    # path to ASCI file defining the detector geometry
    geometry = cms.FileInPath("FastSimulation/HadShowerTuning/data/geometry_standard.txt"),
    # path to output file with tuning data
    outputFile = cms.string("test.root"),
    # particle energy [GeV]
    particleEnergy = cms.double(0.05),
    # particle type
    particleType = cms.string("pi+"),
    # number to inialise the random number generator
    seed = cms.int32(1),
    # number of events to generate
    nevents = cms.int32(10000)
    )
