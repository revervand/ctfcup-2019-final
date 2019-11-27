#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void foo()
{
   char buf[ 2048 ];

   printf( "{?} Enter your data: " );
   int nbytes = read( 0, buf, 2048 );
   buf[ nbytes ] = '\0';

   printf( "{+} Thank you! Your data is: " );
   printf( buf );

   puts( "{+} Goodbye!" );
};


int main(int argc, char** argv)
{
   setvbuf( stdout, NULL, _IONBF, 0 );
   setvbuf( stdin, NULL, _IONBF, 0 );
   setvbuf( stderr, NULL, _IONBF, 0 );

   foo();
   
   return 1;
}