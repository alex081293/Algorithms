# 5/15/2014
'''
A palindromic number reads the same both ways. The largest palindrome made from the product of two 2-digit numbers is 9009 = 91 × 99.

Find the largest palindrome made from the product of two 3-digit numbers.
'''

maxVal = 0
for x in xrange(999, 0, -1):
	for y in xrange(999, 0, -1):
		total = x * y
		totalStr = str(total)
		length = len(str(totalStr))
		a = totalStr[0:length/2]
		b = totalStr[length/2:length]
		b = b[::-1]
		if a == b:
			if total > maxVal:
				maxVal = total
print maxVal
