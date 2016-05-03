/*!
 * define the primary
 */

#ifndef HADSHOWERTUNING_PRIMARYGENERATORACTION_H_
#define HADSHOWERTUNING_PRIMARYGENERATORACTION_H_

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"

namespace hadshowertuning {

    class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
    {
	public:
	
	PrimaryGeneratorAction() 
	    : G4VUserPrimaryGeneratorAction()
	    , fParticleGun(0)
	{
	    G4int n_particle = 1;
	    fParticleGun = new G4ParticleGun(n_particle);
	}
	
	~PrimaryGeneratorAction(){
	    delete fParticleGun;
	}
	
	virtual void GeneratePrimaries(G4Event* anEvent){
	    fParticleGun->GeneratePrimaryVertex(anEvent);
	}

	G4ParticleGun * particleGun(){return fParticleGun;}
	
	private:
	
	G4ParticleGun* fParticleGun;
    };
}

#endif
