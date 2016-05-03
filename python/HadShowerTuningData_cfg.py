import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")
process.load("FastSimulation.HadShowerTuning.HadShowerTuningData_cfi")
process.source = cms.Source("EmptySource")
process.p = cms.Path(process.hadShowerTuningData)
