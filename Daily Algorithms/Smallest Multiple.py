# 5/15/2014
'''
2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.

What is the smallest positive number that is evenly divisible by all of the numbers from 1 to 20?
'''


import itertools 
def checkMultiple(num):
	A = [20, 18, 11, 13, 12, 14, 17, 19, 16, 12]
	for i in range (0, 9):
		if (num % A[i] != 0):
			return False
	return True

for i in itertools.count(2520, 121645):
	if (checkMultiple(i)):
		print i
print "Failure"