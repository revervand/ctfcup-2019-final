from hashlib import sha256
import binascii

_hashes = []

flag = "Cup{f522cac768c5755f3d3cef64388a1ebf96a091b5bcbc4797b159140704895f81}"

for i in range(0,35):
	print( "&hash_%d[0]," % i, end='' )

for i in range( 0, len( flag ), 2 ):
	part = flag[ i : i + 2 ].encode('utf-8')

	_hashes.append( sha256( part ).hexdigest() )

for i in range( len( _hashes ) ):
	string = 'hash[' + str( i ) + "][SHA256_BLOCK_SIZE] = {" 
	
	_hash = _hashes[ i ]

	for j in range( 0, len( _hash ), 2 ):
		elem = hex(ord(binascii.unhexlify( _hash[ j : j + 2 ] )))
		string += elem + ","
		
	string += "};"


	#print( string.replace( ',}', '}' ) )