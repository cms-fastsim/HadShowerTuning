/*!
 *   Geant 4 simulation of a single particle hitting a calorimeter.
 *   Data is collected for the tuning of a model for hadronic (and other) showers.
 *   The edm::Analyzer is used for easy compilation, running and configuration.
 *   The only relevant code resides in the contstructor which prepares and runs the geant4 simulation.
 *   The analyze function simply aborts the program.
 */

// system include files
#include <memory>
#include <sstream>
#include <vector>

// CMSSW framework classes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// include root classes
#include "TFile.h"
#include "TTree.h"

// include geant4 classes
#include "G4RunManager.hh"
#include "FTFP_BERT.hh"
#include "G4UImanager.hh"
#include "G4PhysListFactory.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "Randomize.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

// include geant4 user classes
#include "FastSimulation/HadShowerTuning/include/DetectorConstructionASCI.h"
#include "FastSimulation/HadShowerTuning/include/PrimaryGeneratorAction.h"
#include "FastSimulation/HadShowerTuning/include/EventAction.h"
#include "FastSimulation/HadShowerTuning/include/SteppingAction.h"
#include "FastSimulation/HadShowerTuning/include/TrackingAction.h"
#include "FastSimulation/HadShowerTuning/include/Data.h"

class HadShowerTuningData : public edm::one::EDAnalyzer<>  {
public:
    explicit HadShowerTuningData(const edm::ParameterSet&);
    ~HadShowerTuningData(){;}

private:
    virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

    // ----------member data ---------------------------
};

HadShowerTuningData::HadShowerTuningData(const edm::ParameterSet& iConfig)
{
    
    //
    // read options
    //

    // geometry definition in ASCI
    edm::FileInPath geometryPath = iConfig.getParameter<edm::FileInPath>("geometry");
    
    // output file
    std::string ofile = iConfig.getParameter<std::string>("outputFile");

    // flag: make tree or not
    bool makeTree = iConfig.getUntrackedParameter<bool>("makeTree",false);

    // particle properties
    std::string particleType = iConfig.getParameter<std::string>("particleType");
    double particleEnergy = iConfig.getParameter<double>("particleEnergy");

    // number of events
    int nevents = iConfig.getParameter<int>("nevents");

    // seed intialisiation
    int iSeed = iConfig.getParameter<int>("seed");

    //
    // prepare geant4
    //
    
    // create main g4 object
    G4RunManager* runManager = new G4RunManager;

    // set the geometry
    hadshowertuning::DetectorConstructionASCI * detectorConstruction =new hadshowertuning::DetectorConstructionASCI(geometryPath.fullPath());
    runManager->SetUserInitialization(detectorConstruction);

    // set the physics list
    G4PhysListFactory factory;
    runManager->SetUserInitialization(factory.GetReferencePhysList("FTFP_BERT"));

    // set the properties of the primary particle
    hadshowertuning::PrimaryGeneratorAction * generator = new hadshowertuning::PrimaryGeneratorAction();
    runManager->SetUserAction(generator);
    generator->particleGun()->SetParticleEnergy(particleEnergy*GeV);
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    generator->particleGun()->SetParticleDefinition(particleTable->FindParticle(particleType));
    generator->particleGun()->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
    generator->particleGun()->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
    
    // define how to gather and store data
    TFile * file = TFile::Open(ofile.c_str(),"RECREATE");
    hadshowertuning::Data data;
    runManager->SetUserAction(new hadshowertuning::EventAction(file,&data,makeTree)); // takes care of storing relevant data
    runManager->SetUserAction(new hadshowertuning::TrackingAction(&data));                // gathers simTrack-level data
    std::map<const G4VPhysicalVolume*,int> volumeIdMap;
    runManager->SetUserAction(new hadshowertuning::SteppingAction(&data,&volumeIdMap));   // gather simStep-level data

    // initialise
    runManager->Initialize();
    
    //
    // store the geometry in the output file in TTree format
    //

    volumeIdMap[detectorConstruction->getWorldVolume()] = 0;
    G4LogicalVolume * world = detectorConstruction->getWorldVolume()->GetLogicalVolume();
    for(int d = 0 ; d < world->GetNoDaughters();d++)
    {
	auto daughter = world->GetDaughter(d);
	volumeIdMap[daughter] = d+1;
    }
    TTree * geomTree = new TTree("geometry","geometry");
    std::string name,material;
    double center_x,center_y,center_z,halfR_x,halfR_y,halfR_z;
    int id;
    geomTree->Branch("id",&id,"id/I");
    geomTree->Branch("name",&name);
    geomTree->Branch("material",&material);
    geomTree->Branch("center_x",&center_x,"center_x/D");
    geomTree->Branch("center_y",&center_y,"center_y/D");
    geomTree->Branch("center_z",&center_z,"center_z/D");
    geomTree->Branch("halfR_x",&halfR_x,"halfR_x/D");
    geomTree->Branch("halfR_y",&halfR_y,"halfR_y/D");
    geomTree->Branch("halfR_z",&halfR_z,"halfR_z/D");
    for( auto entry : volumeIdMap)
    {
	const G4VPhysicalVolume * physVol = entry.first;
	const G4LogicalVolume * logVol = physVol->GetLogicalVolume();
	const G4ThreeVector & center = physVol->GetTranslation();
	const G4Box * solid = dynamic_cast<G4Box *>(logVol->GetSolid());
	if(!solid)
	{
	    std::cout << "ERROR: geometry contains volumes that are not box-shaped. I cannot yet deal with that" << std::endl;
	    exit(1);
	}
	id = entry.second;
	name = physVol->GetName();
	material = logVol->GetMaterial()->GetName();
	center_x = center.x()/cm;
	center_y = center.y()/cm;
	center_z = center.z()/cm;
	halfR_x = solid->GetXHalfLength()/cm;
	halfR_y = solid->GetYHalfLength()/cm;
	halfR_z = solid->GetZHalfLength()/cm;
	geomTree->Fill();
    }
    geomTree->Write();

    //
    // set random seed
    //
    
    CLHEP::HepRandom::setTheSeed(iSeed);

    //
    // run geant4
    //

    runManager->BeamOn(nevents);
    
    //
    // clean-up
    //

    delete runManager;
    file->Close();
}


void HadShowerTuningData::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    exit(0);
}


//define this as a plug-in
DEFINE_FWK_MODULE(HadShowerTuningData);
