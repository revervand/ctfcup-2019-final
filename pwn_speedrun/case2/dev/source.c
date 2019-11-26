#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char user[ 8 ];

int main(int argc, char** argv)
{
   setvbuf( stdout, NULL, _IONBF, 0);
   setvbuf( stdin, NULL, _IONBF, 0 );
   setvbuf( stderr, NULL, _IONBF, 0 );
   alarm( 5 );

   printf( "{?} Enter your name: " );
   read( 0, user, 8 );

   printf( "{+} Hello %s\n", user );

   char tmp_buf[ 128 ];
   printf( "{?} Enter data: " );
   read( 0, tmp_buf, 256 );
   
   printf( "{+} Your data is: %s", tmp_buf );

   printf( "Goodbye %s\n", user );

   return 1;
}