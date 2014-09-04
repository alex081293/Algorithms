import math
import sys

# Functions to find the min, mid, and max
def findMax(A, B, C):
	return max(A, B, C)
def findMin(A, B, C):
	return min(A, B, C)
def findMid(A, B, C):
	while (A > B or B > C):
		if A >= B:
			temp = A
			A = B
			B = temp
		if B >= C:
			temp = B
			B = C
			C = temp
	return B

# finds winner of the game
def recursiveFindWinner(A, B, C, i):
	maxNum = findMax(A, B, C)
	minNum = findMin(A, B, C)
	midNum = findMid(A, B, C)
	
	# base case if all three numbers are the same
	if (maxNum == midNum and minNum == midNum):
		return "Second"

	# will find the winner, when two numbers are the same, the next round will be the winner
	if (maxNum == midNum or minNum == midNum):
		if (i+1 % 2 == 0):
			return "Second"
		else:
			return "First"

	# depending on which two numbers are further apart, we call the recursive function
	# on the other set
	if (maxNum - midNum > midNum - minNum):
		return recursiveFindWinner(maxNum, midNum, int(math.floor((maxNum+midNum)/2)), i+1)
	else:
		return recursiveFindWinner(minNum, midNum, int(math.floor((minNum+midNum)/2)), i+1)

print recursiveFindWinner(3, 3, 3, 0)
print recursiveFindWinner(3, 3, 4, 0)
print recursiveFindWinner(10, 12, 17, 0)
print recursiveFindWinner(1, 11, 47, 0)