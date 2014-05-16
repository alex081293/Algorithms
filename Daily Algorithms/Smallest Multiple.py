# 5/15/2014
'''
2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.

What is the smallest positive number that is evenly divisible by all of the numbers from 1 to 20?
'''


import itertools 
def checkMultiple(num):
	A = [2, 19, 17, 13, 11, 7, 5, 3, 20, 18, 16, 15, 14, 12, 10, 9, 8, 6, 4]
	for i in range (0, 19):
		if (num % A[i] != 0):
			return False
	return True

for i in itertools.count(2520, 121645100408832000):
	if (checkMultiple(i)):
		print i
		exit(1)
print "Failure"