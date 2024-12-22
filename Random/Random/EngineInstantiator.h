

#ifndef EngineInstantiator_h
#define EngineInstantiator_h 1

#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/defs.h"

#include <map>

namespace CLHEP {


class EngineInstantiator
{


	class EngineClass
	{
		virtual HepRandomEngine * createInstance()=0;
		friend class EngineInstantiator;
	};
	
	
	template < class EngineClassName >
	class TypedEngineClass : public EngineClass
	{
		virtual HepRandomEngine * createInstance()
		{
			return new EngineClassName();
		}
		friend class EngineInstantiator;
	};

	public :
		static EngineInstantiator * getInstance();
		void registerEngineClass(const std::string &, EngineClass *);
		HepRandomEngine * instantiateByName(const std::string &);
	private :
		EngineInstantiator(){};
		~EngineInstantiator();
		static EngineInstantiator * theInstantiator;
		std::map<const std::string,EngineClass *> registred; 
		
  
    
};


}

#ifdef ENABLE_BACKWARDS_COMPATIBILITY
//  backwards compatibility will be enabled ONLY in CLHEP 1.9
using namespace CLHEP;
#endif


#endif
