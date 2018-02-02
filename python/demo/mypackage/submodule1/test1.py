#!/usr/bin/env python3

def test1():
    print("call test1")

if __name__ == '__main__':
    print("submodule1.test1程序自身在执行")
else:
    print("submodule1.test1被导入到新模块中")
