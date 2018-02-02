#!/usr/bin/env python3

import numpy as np

a = np.zeros((2,2))

b = [[1,2],[2,3]]
c = np.array(b)

d = ((1,2),(3,4))
e = np.array(d)

f = c.reshape(1,4)

print(f[0,:])
print(type(f[0,:]))

print(a)
print(type(a))

print(b)
print(type(b))

print(c)
print(type(c))

print(e)
print(type(d))

cond = np.array([1,2,3,4])
y1 = np.array([-1,-2,-3,-4])
y2 = np.array([1,2,3,4])
x = np.where(cond>2,y1,y2) # 长度须匹配

print(x)
