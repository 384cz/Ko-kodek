from scipy.io import wavfile
import numpy as np
import sys,random
sys.set_int_max_str_digits(int(1e7))
samplerate, data = wavfile.read(sys.argv[1])
l=len(data)-20
fd=[]
ffd=[]
i=1

if len(sys.argv)>3:
	diffs=int(sys.argv[3])
else:
	diffs = 2

for ii in range(diffs):
	i=0
	while (l-(ii*8))>i:
		i+=1
		#print(i)
		diff=int(data[i+1])-int(data[i])
		fd.append(int(diff))
	data=fd
	fd=[]
ffd=data	
bd=""
i=1
td=""
#header
fffd=bytes("Katze-"+str(samplerate).zfill(6)+"@","utf-8")
l=len(ffd)
print(l)
while l>i:
	if (i%10000) == 0:
		print(i/(len(ffd)))
	if((i%5)==0):
		binaryFromTrit=(bin(int(td,3))[2:]).zfill(8)
		bd+=binaryFromTrit
		td=""
	val=ffd[i]
	if (4>val) and (val>(-4)): #(-threshold)<=val<=threshold:
		td+="0"
	elif val>0:
		td+="1"
	elif 0>val:
		td+="2"
		#print(val)
	i+=1
		



for x in range(0, len(bd), 8):
	fffd += bytes([int(bd[x:x+8], 2)])
f=open(sys.argv[2],"wb")
f.write(fffd)
f.close()
#"""
