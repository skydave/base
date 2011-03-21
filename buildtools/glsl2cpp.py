#
# converts a glsl file into a cpp array definition
# TODO: do some obfuscation to reduce filesize
#
# args: glsl file
# result will be printed to stdio
# usage: python2.5 glsl2cpp.py src/glsl/test.glsl > test.glsl.cpp
import sys
import os
import os.path
import string
import stat

args = sys.argv

#sys.stdout.write("const char *test_glsl[] = {\"void main(){}\"};\n")



# read the file
filename = args[1]
varname = os.path.basename(filename).replace( ".glsl", "" ).replace(".", "_")
fh = open(filename,"rb")



# no iterate over each byte
bytesPerRow = 10


bytes_read = fh.read()

sys.stdout.write("// begin "+varname+" data\n")

sys.stdout.write("char "+varname+"[] = { ")

count = 0
for b in bytes_read:
	# write item
	sys.stdout.write("0x%s" % hex(ord(b))[2:].upper().zfill(2) )
	
	#process_byte(b)
	count = count + 1

	# if not last item, write comma
	if count < len(bytes_read):
		sys.stdout.write(", ")

	# nextline when at the end of a row and more bytes to come
	if (count % bytesPerRow == 0)and( count < len(bytes_read)):
		sys.stdout.write("\n\t\t\t\t\t ")
		

# close the array
sys.stdout.write(" };\n")

sys.stdout.write("int "+varname+"_size = "+str(len(bytes_read))+";\n")
sys.stdout.write("// end "+varname+" data\n")
fh.close()


