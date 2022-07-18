#!/usr/bin/python

p=0
l=0
print ("Start fori test loop 1")

r1 = range(1,800001,1)

for i in range(1,101):
	for j in r1:
		l=i*j+32.1
		p=p+l
	print ("loop ",i,"p=",p)
print ("result is : "+str(p))

