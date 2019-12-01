#include <iostream>
#include <string>
#include <sstream>

// Cup{fa76dcbb1371fc6a941086e8a4d6d5d9d149bedabcd5da5b1f60642c8be7833b}

int main()
{
	std::cout << "=========== WELCOME TO OUR REVERSER PARTY ===========" << std::endl;
	std::cout << "Enter private token: ";

	std::string UserInput;
	std::cin >> UserInput;

	if ( UserInput[ 0 ] != 'C' )
	{
		abort();
	}

	if ( UserInput[ 1 ] != 'u' )
	{
		abort();
	}

	if ( UserInput[ 2 ] != 'p' )
	{
		abort();
	}

	if ( UserInput[ 3 ] != '{' )
	{
		abort();
	}

	if ( UserInput.size() != 69 )
	{
		abort();
	}

	if ( UserInput[ 68 ] != '}' )
	{
		abort();
	}

	std::string parts[ 4 ];

	parts[ 0 ] = UserInput.substr( 4, 16 );
	parts[ 1 ] = UserInput.substr( 20, 16 );
	parts[ 2 ] = UserInput.substr( 36, 16 );
	parts[ 3 ] = UserInput.substr( 52, 16 );

	if ( parts[ 3 ] != "1f60642c8be7833b" )
	{
		abort();
	}

	unsigned long long parts_hex[ 4 ];

	for ( int i = 0; i < 4; i++ )
	{
		std::stringstream ss;
		ss << std::hex << parts[ i ];
		ss >> parts_hex[ i ];
	}
	
	if ( ( parts_hex[ 2 ] - parts_hex[ 3 ] ) != 0xb1e95aae30ee5720 )
	{
		abort();
	}

	if ( ( parts_hex[ 2 ] - parts_hex[ 1 ] ) != 0x3d3937f217ff0482 )
	{
		abort();
	}

	if ( ( parts_hex[ 0 ] - parts_hex[ 1 ] ) != 0x666655d26e9b2691 )
	{
		abort();
	}
	
	std::cout << "{+} Valid flag!" << std::endl;
	return 1;
};

void abort( void )
{
	std::cout << "{-} Invalid flag!" << std::endl;
	exit( -1 );
}