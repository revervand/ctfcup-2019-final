part_3 = "1f60642c8be7833b"
part_3_hex = 0x1f60642c8be7833b

flag = "Cup{%s}"

part_2_hex =  0xb1e95aae30ee5720 + part_3_hex
part_1_hex =  part_2_hex - 0x3d3937f217ff0482
part_0_hex =  0x666655d26e9b2691 + part_1_hex

buf = hex( part_0_hex )[ 2:-1 ] + hex( part_1_hex )[ 2:-1 ]  + hex( part_2_hex )[ 2:-1 ] + part_3

print( flag % buf )