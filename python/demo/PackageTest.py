#!/usr/bin/env python3
import sys
from mypackage.submodule1.test1 import test1 as haha
#from mypackage.submodule2.test2 import MyClass 

import mypackage.submodule2.test2 as test2
from mypackage.submodule2.test2 import MyClass as MyClass
from mypackage.submodule2.test2 import test2 as haha2

haha()
haha2()

test2.test2()

p  = MyClass("feng","fsefsef")
p.speak()

print("\n python路径:",sys.path)
