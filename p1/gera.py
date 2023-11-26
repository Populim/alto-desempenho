import numpy as np 
import random

N = 1200


#random.seed(0)

A = np.random.rand(N,N)
B = np.random.rand(N,N)
C = np.random.rand(N,N)
D = np.random.rand(N,N)


print(N)
for i in A:
	for j in i:
		print(j,end = ' ')
	print()


for i in B:
	for j in i:
		print(j,end = ' ')
	print()


for i in C:
	for j in i:
		print(j,end = ' ')
	print()


for i in D:
	for j in i:
		print(j,end = ' ')
	print()


X = A.dot(B) + C.dot(D)

X = np.round(X,decimals=1)

with open('resultadopython.txt', 'w') as f:
	for i in X:
		for ind,j in enumerate(i):
			if(ind == N-1):
				print(j,end = '\n',file=f)
			else:
				print(j,end = ' ',file=f)