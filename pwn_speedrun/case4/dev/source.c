#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linux/audit.h>
#include <linux/seccomp.h>
#include <linux/filter.h>

#include <sys/prctl.h>
#include <sys/ptrace.h>

#include <fcntl.h>

#include <seccomp.h>

char* load()
{
   char* flag = (char*)malloc( 70 );

   int i = 0;
   int fd = open( "/tmp/flag.txt", O_RDONLY );
   
	int result = read( fd, flag, 70 );
   
   flag[ result - 1 ] = '\0';
   
   close( fd );
   
   return flag;
}

void run_code( char* buf, int size )
{
   char* flag = load();

   // alloc chunk and run code
   void *ptr = mmap( 
      0, 
      size, 
      PROT_EXEC | PROT_WRITE | PROT_READ, 
      MAP_ANON | MAP_PRIVATE, 
      -1, 
      0
   );

   void (*code)(char*);

   // load rules
   scmp_filter_ctx ctx;

   memcpy( ptr, buf, size );
   ctx = seccomp_init( SCMP_ACT_KILL );
   seccomp_rule_add( ctx, SCMP_ACT_ALLOW, SCMP_SYS( exit ), 0 );
   seccomp_load( ctx );

   code = (void (*)(char*)) ptr;
   code( flag );
}

void foo()
{
   char buf[ 512 ];

   printf( "{?} Enter your code: " );
   
   int nybtes = read( 0, buf, 512 );
   
   buf[ nybtes - 1 ] = '\0';

   run_code( buf, nybtes );
}

int main()
{
   setvbuf( stdout, NULL, _IONBF, 0 );
   setvbuf( stdin, NULL, _IONBF, 0 );
   setvbuf( stderr, NULL, _IONBF, 0 );

	alarm( 2 );

   foo();

   exit( 0 );
};