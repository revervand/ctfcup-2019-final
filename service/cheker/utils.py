import struct
import msg
import sys
import signal

from consts import *

p_8     = lambda val : struct.pack( "!B", val )
p_16_le = lambda val : struct.pack( "<H", val )
p_32_le = lambda val : struct.pack( "<L", val )
p_64_le = lambda val : struct.pack( "<Q", val )

u_8     = lambda val : struct.unpack( "!B", val )[ 0 ]
u_16_le = lambda val : struct.unpack( "<H", val )[ 0 ]
u_32_le = lambda val : struct.unpack( "<L", val )[ 0 ]
u_64_le = lambda val : struct.unpack( "<Q", val )[ 0 ]

p_16_be = lambda val : struct.pack( ">H", val )
p_32_be = lambda val : struct.pack( ">L", val )
p_64_be = lambda val : struct.pack( ">Q", val )

u_16_be = lambda val : struct.unpack( ">H", val )[ 0 ]
u_32_be = lambda val : struct.unpack( ">L", val )[ 0 ]
u_64_be = lambda val : struct.unpack( ">Q", val )[ 0 ]

def print_f( buf ):
	
	print ( buf )
	sys.stdout.flush()

def print_errf( buf ):
	
	print ( msg.bcolors.FAIL + buf + msg.bcolors.ENDC )
	sys.stdout.flush()

def alarm_exit( _arg1, _arg2 ):
	
	print_errf( "[-] Connection timeout!" )
	sys.exit( TIMEOUT_ALARM_EXIT_CODE )

def init():

	signal.alarm( ALARM_TIME )
	signal.signal( signal.SIGALRM, alarm_exit )

def pack_value( value ):

	if value > 0xff and value <= 0xffff:
		return p_16_le( value )
	elif value > 0xffff and value <= 0xffffffff:
		return p_32_le( value )
	elif value > 0xffffffff:
		return p_64_le( value )
	else:
		return p_8( value )
