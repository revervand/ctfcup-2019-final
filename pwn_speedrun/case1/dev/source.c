#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void foo()
{
   char buf[ 512 ];
   
   printf( "{?} Enter data: " );
   read( 0, buf, 1024 );
   printf( "{+} Your data: %s\n", buf );
}

int main(int argc, char** argv)
{
   setvbuf( stdout, NULL, _IONBF, 0);
   setvbuf( stdin, NULL, _IONBF, 0 );
   setvbuf( stderr, NULL, _IONBF, 0 );

   foo();

   return 1;
}