


#include "CLHEP/Random/SobolQRNG.h"
#include "CLHEP/Random/SobolQRNGB.h"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/DRand48Engine.h" 
#include "CLHEP/Random/DualRand.h"
#include "CLHEP/Random/Hurd160Engine.h"
#include "CLHEP/Random/Hurd288Engine.h"
#include "CLHEP/Random/MTwistEngine.h"
#include "CLHEP/Random/NonRandomEngine.h"
#include "CLHEP/Random/RandEngine.h"
#include "CLHEP/Random/RanecuEngine.h"
#include "CLHEP/Random/Ranlux64Engine.h"
#include "CLHEP/Random/RanluxEngine.h"
#include "CLHEP/Random/RanshiEngine.h"
#include "CLHEP/Random/TripleRand.h"

#include "CLHEP/Random/RandomException.h"


#include "CLHEP/Random/EngineInstantiator.h"


namespace CLHEP {

EngineInstantiator * EngineInstantiator::theInstantiator = NULL;
 
EngineInstantiator * EngineInstantiator::getInstance()
{
  if(theInstantiator != NULL) return theInstantiator;
  
  theInstantiator = new EngineInstantiator();
  
  theInstantiator->registerEngineClass("SobolQRNG",new TypedEngineClass<SobolQRNG>);
  theInstantiator->registerEngineClass("SobolQRNGB",new TypedEngineClass<SobolQRNGB>);
  theInstantiator->registerEngineClass("JamesRandom",new TypedEngineClass<HepJamesRandom>);
  theInstantiator->registerEngineClass("DRand48",new TypedEngineClass<DRand48Engine>);
  theInstantiator->registerEngineClass("DualRand",new TypedEngineClass<DualRand>);
  theInstantiator->registerEngineClass("Hurd160",new TypedEngineClass<Hurd160Engine>);
  theInstantiator->registerEngineClass("Hurd288",new TypedEngineClass<Hurd288Engine>);
  theInstantiator->registerEngineClass("MTwist",new TypedEngineClass<MTwistEngine>);
  theInstantiator->registerEngineClass("NonRandom",new TypedEngineClass<NonRandomEngine>);
  theInstantiator->registerEngineClass("Rand",new TypedEngineClass<RandEngine>);
  theInstantiator->registerEngineClass("Ranecu",new TypedEngineClass<RanecuEngine>);
  theInstantiator->registerEngineClass("Ranlux64",new TypedEngineClass<Ranlux64Engine>);
  theInstantiator->registerEngineClass("Ranlux",new TypedEngineClass<RanluxEngine>);
  theInstantiator->registerEngineClass("Ranshi",new TypedEngineClass<RanshiEngine>);
  theInstantiator->registerEngineClass("TripleRand",new TypedEngineClass<TripleRand>);

  return theInstantiator;
}
 
void EngineInstantiator::registerEngineClass(const std::string & id, EngineClass * clazz)
{
	registred[id] = clazz;
}


HepRandomEngine * EngineInstantiator::instantiateByName(const std::string & id)
{
	std::map<const std::string,EngineClass *>::iterator i = registred.find(id);
	if(i == registred.end()) throw new EngineNotFoundException("Engine " + id + " is not registred in the instantiator");
	return i->second->createInstance();
}


EngineInstantiator::~EngineInstantiator()
{
	for(std::map<const std::string,EngineClass *>::iterator i = registred.begin(); i != registred.end(); i++)
		delete i->second;
}



}
