// $Id: JamesRandom.h,v 1.16 2002/04/12 15:02:44 evc Exp $
// -*- C++ -*-
//
// -----------------------------------------------------------------------
//                             HEP Random
//                        --- SobolQRNG ---
//                          class header file
// -----------------------------------------------------------------------
// This file is part of Geant4 (simulation toolkit for HEP).
//
// SobolQRNG implements the algorithm by Marsaglia-Zaman RANMAR
// described in "F.James, Comp. Phys. Comm. 60 (1990) 329" and implemented
// in FORTRAN77 as part of the MATHLIB HEP library for pseudo-random
// numbers generation.
// This is the default random engine invoked by each distribution unless
// the user sets a different one.

// =======================================================================
// Gabriele Cosmo - Created: 5th September 1995
//                - Minor corrections: 31st October 1996
//                - Added methods for engine status: 19th November 1996
//                - setSeed(), setSeeds() now have default dummy argument
//                  set to zero: 11th July 1997
// J.Marraffino   - Added stream operators and related constructor.
//                  Added automatic seed selection from seed table and
//                  engine counter: 16th Feb 1998
// Ken Smith      - Added conversion operators:  6th Aug 1998
// V. Innocente   - changed pointers to indices     3 may 2000
// =======================================================================

#ifndef SobolQRNG_h
#define SobolQRNG_h 1


#include <string>
#include <iostream>

#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/Sobol.h"
#include "CLHEP/Random/RandomException.h"
#include "CLHEP/Random/defs.h"

namespace CLHEP {


/**
 * @author
 * @ingroup random
 */
class SobolQRNG: public HepRandomEngine {

public:

  //SobolQRNG(HepStd::istream& is);
  SobolQRNG(int dimension = 1);
  //SobolQRNG(long seed);
  //SobolQRNG(int rowIndex, int colIndex);
  virtual ~SobolQRNG();
  // Constructor and destructor.

  //SobolQRNG(const SobolQRNG &p);
  // Copy constructor

  //SobolQRNG & operator = (const SobolQRNG &p);
  // Overloaded assignment operator, to retrieve the engine status.

  double flat();
  // Returns a pseudo random number between 0 and 1 
  // (excluding the end points)

  void flatArray (const int size, double* vect);
  // Fills the array "vect" of specified size with flat random values.

 void setSeed(long seed, int dum=0){std::cout << "setSeed" << std::endl;throw new NotImplementedException("Methode non implementee");};
  // Sets the state of the algorithm according to seed.

  void setSeeds(const long * seeds, int dum=0){std::cout << "setSeeds" << std::endl;throw new NotImplementedException("Methode non implementee");};
  // Sets the state of the algorithm according to the zero terminated
  // array of seeds. Only the first seed is used.

  void saveStatus( const char filename[] = "JamesRand.conf" ) const{std::cout << "saveStatus" << std::endl;throw new NotImplementedException("Methode non implementee");};
  // Saves on file JamesRand.conf the current engine status.

  void restoreStatus( const char filename[] = "JamesRand.conf" ){std::cout << "restoreStatus" << std::endl;throw new NotImplementedException("Methode non implementee");};
  // Reads from file JamesRand.conf the last saved engine status
  // and restores it.

  void showStatus() const{std::cout << "showStatus" << std::endl;throw new NotImplementedException("Methode non implementee");};
  // Dumps the engine status on the screen.

  
  virtual std::string name() const {return "SobolQRNG";}
  
 // operator unsigned int();
  // 32-bit flat, but slower than double or float.

  //friend HepStd::ostream& operator<< (HepStd::ostream& os, const SobolQRNG& e);
 // friend HepStd::istream& operator>> (HepStd::istream& is,       SobolQRNG& e);

private:

  // Members defining the current status of the generator.
  /*double u[97];
  double c, cd, cm;
  int i97, j97;
  static int numEngines;
  static int maxIndex;*/
  Sobol * q;
  int dimension;
  double * currentVector;
  int currentIndex;

};

}  // namespace CLHEP

#ifdef ENABLE_BACKWARDS_COMPATIBILITY
//  backwards compatibility will be enabled ONLY in CLHEP 1.9
using namespace CLHEP;
#endif


#endif
