import random,sys
from scipy.io import wavfile
import numpy as np
f=open(sys.argv[1],"rb")
data=f.read()
i=1
amp=0
fd=[]
m=4
c=8
l=len(data)-20
while (l+8)>i:
	i+=1
	d=int.from_bytes(data[i:i+1], "big")
	dd=str(str(np.base_repr(d,base=3)).zfill(5))
	for j in dd:
		amp*=0.9
		if j == "1":
			amp+=1
		elif j == "2":
			amp-=1
		fd.append(amp)
		#print(amp)
amp=128
print(len(fd))
ffd=[]
i=4
while len(fd) > i:
	amp+=fd[i]
	amp=int((128*0.1)+(amp*0.9))
	amp=max(min(amp,255),0)
	#print(amp)
	ffd.append(amp)
	i+=1
wavfile.write(sys.argv[2],int(data[6:12]),np.array(ffd).astype(np.uint8))

