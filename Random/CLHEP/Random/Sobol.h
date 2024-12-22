#ifndef __SOBOL_H__
#define __SOBOL_H__


#include "CLHEP/Random/QRNG.h"

/* maximum allowed space dimension */
#define SOBOL_MAX_DIMENSION 40

/* bit count; assumes sizeof(int) >= 32-bit */
#define SOBOL_BIT_COUNT 62


class SobolState : public State
{

/* Sobol generator state.
 *   sequence_count       = number of calls with this generator
 *   last_numerator_vec   = last generated numerator vector
 *   last_denominator_inv = 1/denominator for last numerator vector
 *   v_direction          = direction number table
 */

  unsigned long long  sequence_count;
  double        last_denominator_inv;
  long long           last_numerator_vec[SOBOL_MAX_DIMENSION];
  long long           v_direction[SOBOL_BIT_COUNT][SOBOL_MAX_DIMENSION];
  
  public:
    SobolState( unsigned int dimension );
    SobolState( SobolState * src );
    virtual void init();
    virtual void get ( double x[] );
  
  
};


class Sobol : public qrng
{

  public:
   Sobol(unsigned int dimension);
   Sobol(const Sobol * src);
   ~Sobol(){ delete state; };

}; 






#endif

