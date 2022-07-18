#!/usr/bin/python

def fact(num):
	if num < 1:
		return 1
	else:
		f=num*fact(num-1)
		return f

print (fact(6))

p=0
r1 = range(1,800001,1)

for i in range(1,11):
	for j in r1:
		l=i*j+32
		p=p+l+fact(2)
	print ("loop ",i,"p=",p)
print ("result is : "+str(p))