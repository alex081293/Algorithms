# 5/15/2014
'''
The prime factors of 13195 are 5, 7, 13 and 29.

What is the largest prime factor of the number 600851475143 ?
'''
import itertools 
def checkPrime(number):
	for i in itertools.count(3, number):
		i = i+1
		if (number % i == 0):
			return True
	return False

for x in itertools.count(600851475143, 1):
	print x
	if checkPrime(x) == False:
		print x
		exit(1)

print "failed"