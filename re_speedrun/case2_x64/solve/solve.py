from ctypes import c_int

g_rnd_val = c_int( 0x00 )

from_binary = [233, 101, 94, 71, 72, 238, 98, 12, 80, 160, 218, 213, 
	43, 122, 18, 151, 234, 52, 26, 61, 233, 139, 128, 34, 114, 22, 171, 
	175, 200, 28, 98, 181, 209, 214, 54, 8, 8, 254, 244, 149, 64, 100, 
	26, 206, 188, 62, 82, 2, 162, 166, 222, 126, 41, 79, 151, 228, 100, 
	9, 58, 180, 216, 220, 32, 117, 16, 195, 249, 154, 87]


def get_random_value( seed ):
	global g_rnd_val

	g_rnd_val.value += seed
	g_rnd_val.value += ( ( g_rnd_val.value * 0x13377331 ) + 0xc00fe ) & 0xff

	return g_rnd_val.value & 0xff


if __name__ == "__main__":

	key_stream = []
	seed = 70

	for i in range( 0, 256 ):
		key_stream.append( get_random_value( i + seed ) )

	print( key_stream )
	flag = ''

	for i in range( len( from_binary ) ):
		flag += chr( from_binary[ i ] ^ key_stream[ i ] )

	print( "{+} flag: %s" % flag )