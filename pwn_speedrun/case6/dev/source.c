#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char username[ 8 ];

void hint(void)
{
   system( "ls" );
};

int ReadInt( void )
{
   char buf[ 32 ];
   int nbytes = read( 0, &buf, 32 );
   buf[ nbytes - 1 ] = '\0';

   int value = atoi( buf );

   return value;
}

void bar( int size )
{
   char buf[ 256 ];

   printf( "{?} Enter data: " );
   read( 0, buf, size );
   printf( "{+} Your data is: %s\n", buf );
}

void foo()
{
   printf( "{?} Enter data size: " );
   int size = ReadInt();

   if ( size <= 0 || size > 257 )
   {
      puts( "{-} Error size!" );
      exit( -1 );
   }

   bar( size );
}


int main()
{
   setvbuf( stdout, NULL, _IONBF, 0 );
   setvbuf( stdin, NULL, _IONBF, 0 );
   setvbuf( stderr, NULL, _IONBF, 0 );
   
   alarm( 5 );

   printf( "{?} Enter your name: " );
   read( 0, username, 8 );

   foo();

   return 0;
}