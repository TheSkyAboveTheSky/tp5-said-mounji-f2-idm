// $Id: JamesRandom.cc,v 1.20 2002/03/30 14:57:50 evc Exp $
// -*- C++ -*-
//
// -----------------------------------------------------------------------
//                             HEP Random
//                       --- SobolQRNG ---
//                      class implementation file
// -----------------------------------------------------------------------
// This file is part of Geant4 (simulation toolkit for HEP).
//
// This algorithm implements the original universal random number generator
// as proposed by Marsaglia & Zaman in report FSU-SCRI-87-50 and coded
// in FORTRAN77 by Fred James as the RANMAR generator, part of the MATHLIB
// HEP library.

// =======================================================================
// Gabriele Cosmo - Created: 5th September 1995
//                - Fixed a bug in setSeed(): 26th February 1996
//                - Minor corrections: 31st October 1996
//                - Added methods for engine status: 19th November 1996
//                - Fixed bug in setSeeds(): 15th September 1997
// J.Marraffino   - Added stream operators and related constructor.
//                  Added automatic seed selection from seed table and
//                  engine counter: 16th Feb 1998
// Ken Smith      - Added conversion operators:  6th Aug 1998
// J. Marraffino  - Remove dependence on hepString class  13 May 1999
// V. Innocente   - changed pointers to indices     3 may 2000
// =======================================================================

#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/SobolQRNGB.h"
#include <string.h>


namespace CLHEP {

//static const int MarkerLen = 64; // Enough room to hold a begin or end marker.

// Number of instances with automatic seed selection
//int SobolQRNG::numEngines = 0;

// Maximum index into the seed table
//int SobolQRNG::maxIndex = 215;

/*SobolQRNG::SobolQRNG(long seed)
{
  setSeed(seed,0);
  setSeeds(&theSeed,0);
}*/



SobolQRNGB::SobolQRNGB(int dimension, int inSizeOfBrew, HepRandomEngine * brewer)
{
  q = new Sobol(dimension);
  b = brewer;

  sizeOfBrew = inSizeOfBrew;
  brewingTable = new std::vector<double *>;
  this->dimension = dimension;
  nextValueIndex = 0;
  indexOfCurrentVector = (int) floor(b->flat() * sizeOfBrew);

  for(int i = 0; i < sizeOfBrew; i++)
  {
    double * uni = new double[dimension];
    q->get(uni);
    brewingTable->push_back(uni);
  }
  
}

/*SobolQRNG::SobolQRNG(int rowIndex, int colIndex) // 15 Feb. 1998  JMM
{
  long seed;
   long seeds[2];

  int cycle = abs(int(rowIndex/maxIndex));
  int row = abs(int(rowIndex%maxIndex));
  int col = abs(int(colIndex%2));
  long mask = ((cycle & 0x000007ff) << 20);
  HepRandom::getTheTableSeeds( seeds, row );
  seed = (seeds[col])^mask;
  setSeed(seed,0);
  setSeeds(&theSeed,0);
}*/

/*SobolQRNG::SobolQRNG(HepStd::istream& is)
{
  is >> *this;
}*/

SobolQRNGB::~SobolQRNGB() 
{
  delete q;
  delete b;
  for(std::vector<double *>::iterator i = brewingTable->begin(); i != brewingTable->end(); i++)
    delete [] *i;

  delete brewingTable;
}

/*SobolQRNG::SobolQRNG(const SobolQRNG &p)
{
  int ipos, jpos;
  if ((this != &p) && (&p)) {
    theSeed = p.getSeed();
    setSeeds(&theSeed,0);
    for (int i=0; i<97; ++i)
      u[i] = p.u[i];
    c = p.c; cd = p.cd; cm = p.cm;
    jpos = p.j97;
    ipos = (64+jpos)%97;
    i97 = ipos; j97 = jpos;
  }
}*/

/*SobolQRNG & SobolQRNG::operator = (const SobolQRNG &p)
{
  int ipos, jpos;
  if ((this != &p) && (&p)) {
    theSeed = p.getSeed();
    setSeeds(&theSeed,0);
    for (int i=0; i<97; ++i)
      u[i] = p.u[i];
    c = p.c; cd = p.cd; cm = p.cm;
    jpos = p.j97;
    ipos = (64+jpos)%97;
    i97 = ipos; j97 = jpos;
  }
  return *this;
}*/

/*void SobolQRNG::saveStatus( const char filename[] ) const
{
   HepStd::ofstream outFile( filename, HepStd::ios::out ) ;

   if (!outFile.bad()) {
     int pos = j97;
     outFile << theSeed << HepStd::endl;
     for (int i=0; i<97; ++i)
       outFile << HepStd::setprecision(20) << u[i] << " ";
     outFile << HepStd::endl;
     outFile << HepStd::setprecision(20) << c << " ";
     outFile << HepStd::setprecision(20) << cd << " ";
     outFile << HepStd::setprecision(20) << cm << HepStd::endl;
     outFile << pos << HepStd::endl;
   }
}*/

/*void SobolQRNG::restoreStatus( const char filename[] )
{
   int ipos, jpos;
   HepStd::ifstream inFile( filename, HepStd::ios::in);

   if (!inFile.bad() && !inFile.eof()) {
     inFile >> theSeed;
     for (int i=0; i<97; ++i)
       inFile >> u[i];
     inFile >> c; inFile >> cd; inFile >> cm;
     inFile >> jpos;
     ipos = (64+jpos)%97;
     i97 = ipos;
     j97 = jpos;
   }
}*/

/*void SobolQRNG::showStatus() const
{
   HepStd::cout << HepStd::endl;
   HepStd::cout << "----- SobolQRNG engine status -----" << HepStd::endl;
   HepStd::cout << " Initial seed = " << theSeed << HepStd::endl;
   HepStd::cout << " u[] = ";
   for (int i=0; i<97; ++i)
     HepStd::cout << u[i] << " ";
   HepStd::cout << HepStd::endl;
   HepStd::cout << " c = " << c << ", cd = " << cd << ", cm = " << cm
	     << HepStd::endl;
   HepStd::cout << " i97 = " << i97 << ", u[i97] = " << u[i97] << HepStd::endl;
   HepStd::cout << " j97 = " << j97 << ", u[j97] = " << u[j97] << HepStd::endl;
   HepStd::cout << "----------------------------------------" << HepStd::endl;
}*/

/*void SobolQRNG::setSeed(long seed, int)
{
  // The input value for "seed" should be within the range [0,900000000]

  int m, n;
  float s, t;
  long mm;

  long ij = seed/30082;
  long kl = seed - 30082*ij;
  long i = (ij/177) % 177 + 2;
  long j = ij % 177 + 2;
  long k = (kl/169) % 178 + 1;
  long l = kl % 169;

  theSeed = seed;

  for ( n = 1 ; n < 98 ; n++ ) {
    s = 0.0;
    t = 0.5;
    for ( m = 1 ; m < 25 ; m++) {
      mm = ( ( (i*j) % 179 ) * k ) % 179;
      i = j;
      j = k;
      k = mm;
      l = ( 53 * l + 1 ) % 169;
      if ( (l*mm % 64 ) >= 32 )
        s += t;
      t *= 0.5;
    }
    u[n-1] = s;
  }
  c = 362436.0 / 16777216.0;
  cd = 7654321.0 / 16777216.0;
  cm = 16777213.0 / 16777216.0;

  i97 = 96;
  j97 = 32;

}*/

/*void SobolQRNG::setSeeds(const long* seeds, int)
{
  setSeed(seeds ? *seeds : 19780503L, 0);
  theSeeds = seeds;
}*/

double SobolQRNGB::flat()
{

   //if(!(++nbAppel % 1000000))std::cout << "appel " << nbAppel << std::endl;
  
   /* test if the quasi random vector as been entierly read */ 
   if(nextValueIndex >= dimension)
  {
 //   printf("%d\n", nextValueIndex);
    q->get((*brewingTable)[indexOfCurrentVector]);
    indexOfCurrentVector = (int) floor(b->flat() * sizeOfBrew);
    nextValueIndex = 0;
  }

  // std::cout << "APEL" << std::endl;

   /*do {
      uni = u[i97] - u[j97];
      if ( uni < 0.0 ) uni++;
      u[i97] = uni;
      
      if (i97 == 0) i97 = 96;
      else i97--;

      if (j97 == 0) j97 = 96;
      else j97--;
      
      c -= cd;
      if (c < 0.0) c += cm;
      
      uni -= c;
      if (uni < 0.0) uni += 1.0;
   } while ( uni <= 0.0 || uni >= 1.0 );
   */

   /*return the part of the current QR vector */
   return (*brewingTable)[indexOfCurrentVector][nextValueIndex++];
}

void SobolQRNGB::flatArray(const int size, double* vect)
{
//   double uni;
   int i;

   std::cout << "taille " << size << std::endl;
   
   for (i=0; i<size; ++i) {
     vect[i] = flat();
   }   
}

/*SobolQRNG::operator unsigned int() {
   return (unsigned int)(flat() * exponent_bit_32) & 0xffffffff   |
         ((unsigned int)( u[i97] * exponent_bit_32)>>16)  & 0xff;
}*/

/*HepStd::ostream & operator<< ( HepStd::ostream& os, const SobolQRNG& e )
{
   char beginMarker[] = "JamesRandom-begin";
   char endMarker[]   = "JamesRandom-end";

   int pos = e.j97;
   os << " " << beginMarker << " ";
   os <<  e.theSeed << " ";
   for (int i=0; i<97; ++i) {
     os << HepStd::setprecision(20) << e.u[i] << " ";
   }
   os << HepStd::setprecision(20) << e.c << " ";
   os << HepStd::setprecision(20) << e.cd << " ";
   os << HepStd::setprecision(20) << e.cm << " ";
   os << pos << " ";
   os << endMarker << " ";
   return os;
}*/

/*HepStd::istream & operator >> ( HepStd::istream& is, SobolQRNG& e )
{
  int ipos, jpos;
  char beginMarker [MarkerLen];
  char   endMarker [MarkerLen];

  is >> HepStd::ws;
  is.width(MarkerLen);  // causes the next read to the char* to be <=
			// that many bytes, INCLUDING A TERMINATION \0 
			// (Stroustrup, section 21.3.2)
  is >> beginMarker;
  if (strcmp(beginMarker,"JamesRandom-begin")) {
     is.clear(HepStd::ios::badbit | is.rdstate());
     HepStd::cerr << "\nInput stream mispositioned or"
	       << "\nJamesRandom state description missing or"
	       << "\nwrong engine type found." << HepStd::endl;
     return is;
  }
  is >> e.theSeed;
  for (int i=0; i<97; ++i) {
     is >> e.u[i];
  }
  is >> e.c; is >> e.cd; is >> e.cm;
  is >> jpos;
  is >> HepStd::ws;
  is.width(MarkerLen);
  is >> endMarker;
  if(strcmp(endMarker,"JamesRandom-end")) {
     is.clear(HepStd::ios::badbit | is.rdstate());
     HepStd::cerr << "\nJamesRandom state description incomplete."
	       << "\nInput stream is probably mispositioned now." << HepStd::endl;
     return is;
  }

  ipos = (64+jpos)%97;
  e.i97 = ipos;
  e.j97 = jpos;
  return is;
}*/

}

