#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


#define BUF_SIZE 32

void setup( void );
int ReadInt( void );

struct User{
	char* name;
	char* surname;
	int age;
};

struct User *g_user;

void print_menu( void )
{
	puts( "---- Menu ----" );
	puts( "1. Edit name" );
	puts( "2. Edit suername" );
	puts( "3. Add note" );
	puts( "4. Delete this user" );
	puts( "5. Exit" );
	printf( "> " );
};

void get_flag( void )
{
	char* flag = (char*)malloc( 70 );

	int i = 0;
	int fd = open( "/tmp/flag.txt", O_RDONLY );
   
	int result = read( fd, flag, 70 );
   
   	flag[ result - 1 ] = '\0';

   	close( fd );
   	
   	printf( "Flag: %s\n", flag );
};

int main()
{
	setup();

	printf( "{?} Hello, enter your age: " );
	g_user = (struct User*) malloc( sizeof( struct User ) );

	g_user->age = ReadInt();

	printf( "{?} Enter your name: " );
	
	char* name = (char*) malloc( BUF_SIZE );
	int nbytes = read( 0, name, BUF_SIZE );
	name[ nbytes - 1 ] = '\0';

	printf( "{?} Enter your surname: " );
	
	char* surname = (char*) malloc( BUF_SIZE );
	nbytes = read( 0, surname, BUF_SIZE );
	surname[ nbytes - 1 ] = '\0';

	g_user->name = name;
	g_user->surname = surname;

	while ( 1 )
	{
		print_menu();
		int option = ReadInt();

		switch ( option )
		{
			case 1:
			{
				printf( "{?} Enter new name: " );
				nbytes = read( 0, g_user->name, BUF_SIZE );
				break;
			}
			case 2:
			{
				printf( "{?} Enter new surname: " );
	
				char* surname = (char*) malloc( BUF_SIZE );
				nbytes = read( 0, surname, BUF_SIZE );
				surname[ nbytes - 1 ] = '\0';

				g_user->surname = surname;
				break;
			}
			case 3:
			{
				printf( "{?} Enter note size: " );
				int note_size = ReadInt();

				char* note = (char*) malloc( note_size );

				printf( "{?} Enter note data: " );
				nbytes = read( 0, note, note_size );
				note[ nbytes - 1 ] = '\0';

				break;
			}
			case 4:
			{
				free( g_user );
				break;
			}
			case 5:
				exit( -1 );
			default:
				break;
		}
	}

	return 1;
};

void setup( void )
{
	setvbuf( stdout, NULL, _IONBF, 0 );
   	setvbuf( stdin, NULL, _IONBF, 0 );
   	setvbuf( stderr, NULL, _IONBF, 0 );

	alarm( 5 );
};

int ReadInt( void )
{
	char buf[ 32 ];
	int nbytes = read( 0, &buf, 32 );
	buf[ nbytes - 1 ] = '\0';

	int value = atoi( buf );

	return value;
};

