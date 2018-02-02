#!/user/bin/env python3

import sys

class MyClass:
    name = ''
    sex = ''
    def __init__(self,name,sex):
        self.name = name
        self.sex = sex

    def speak(self):
        print("我说:",self.name,",",self.sex)

if __name__ == '__main__':
    p = MyClass("feng","man")
    p.speak()
