/* Author: G. Jungman
 */
#ifndef __QRNG_H__
#define __QRNG_H__

#include <stdlib.h>
//#include <gsl/gsl_types.h>
//#include "errno.h"

/* Once again, more inane C-style OOP... kill me now. */

/* Structure describing a type of generator.
 */
 
class State
{
  protected:
    unsigned int dimension;
  public:
    State( unsigned int dimension ) { this->dimension = dimension ; }
    State( const State * src ) { dimension = src->dimension ; }
    virtual void init() = 0;
    virtual void get ( double x[] ) = 0;
}; 
 
 
 
class qrng
{
   public:
     const char * name;
     unsigned int max_dimension;
     
     unsigned int dimension;
     State * state;
     
     
     /* Allocate and initialize a generator
      * of the specified type, in the given
      * space dimension.
    */
    qrng ( unsigned int dimension );

    /* Copy a generator. */
    qrng(const qrng * src);

    /* Free a generator. */
    ~qrng (){};
    
    /* Intialize a generator. */
    void init ( );
    
    
    /* Get the standardized name of the generator. */
    const char * getName () { return name; };
    State * getState () { return state; };


    /* Retrieve next vector in sequence. */
    void get (double x[]);
    
   
     
};


#endif /* !__GSL_QRNG_H__ */
