# 5/27/2014
'''
By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, we can see that the 6th prime is 13.

What is the 10 001st prime number?
'''
import sys

A = [2]
i = 1
flag = 0
for x in range(3, 100000000):
	flag = 0
	for y in range(0, len(A)):
		if (x % A[y] == 0):
			flag = 1
	if (flag == 0):
		A.append(x)
		i += 1;
		if (i == 10001):
			print x
			sys.exit()