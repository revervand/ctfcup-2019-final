buf = [133, 228, 228, 229, 225, 71, 158, 78, 157, 155, 
	73, 80, 82, 92, 170, 83, 174, 91, 161, 80, 88, 190, 112, 162,
	77, 186, 117, 73, 112, 71, 120, 175, 163, 116, 119, 120, 162, 
	155, 78, 72, 77, 75, 102, 102, 73, 92, 92, 109, 89, 100, 90, 
	84, 87, 77, 127, 99, 79, 101, 49, 55, 53, 109, 109, 99, 51, 
	125, 112, 119, 67]

res = ''

for i, val in enumerate( reversed( buf ) ):

	val ^= i
	val -= i 

	res += chr( val )

print ( "res = %s" % res  )