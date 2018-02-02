#!/usr/bin/env python3
class MyClass:
    name = ''
    sex = ''
    def __init__(self,name,sex):
        self.name = name
        self.sex = sex

    def speak(self):
        print("我说:",self.name,",",self.sex)

def test2():
    print("call test2")

if __name__ == '__main__':
    print("submodule2.test2程序自身在执行")
    p = MyClass("fsfsf","fsfsf")
    p.speak()
else:
    print("submodule2.test2被导入到新模块中")
