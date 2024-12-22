#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>

#include "CLHEP/Random/MTwistEngine.h"

int main ()
{
   CLHEP::MTwistEngine * s = new CLHEP::MTwistEngine();

   int fs;
   double f;
   unsigned int nbr;

   fs = open("./rngbForMarsagliaTests",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR);

   for(int i = 1; i < 3000000; i++)
   {
     f = s->flat();
     nbr = (unsigned int) (f * UINT_MAX);

     //printf("%f\n", f); ou mieux cout << f << endl;

     write(fs,&nbr,sizeof(unsigned int));
   }

   close(fs);

   delete s;

   return 0;
}

