buf = 'Cup{fc1ce16b5237cb71669db81a3d6f0f58b8770437c3447ce3a055dfd5dda37cfe}'

res = []

for i in range( len( buf ) ):
	value = ord( buf[ i ] ) - 1
	value = value ^ i
	value = value + ( i + 2 )

	res.append( value )

	print( "if ( ( ( ( buf[%d] - 1) ^ %d ) + (%d+2)) != _hash[%d] ) { " % ( i, i, i, i )  )
	print( "	return 0;" )
	print( "} " )

print( res )
