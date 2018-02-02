#!/usr/bin/env python3

def hello():
    print("hello world!")

def area(width,height):
    return width * height

def ChangeMe(mylist):
    mylist.append([2,3,4,5])
    print("in fun:",mylist)

    return

hello()

w = 10
h = 4
print("w=",w," h=",h," area=",area(w,h))

mylist = [103,43,5]
ChangeMe(mylist)
print("fun ret:",mylist)
