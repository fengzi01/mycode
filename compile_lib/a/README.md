## 普通编译hello过程
预编译
```bash
cpp hello.c -o hello.i -v

# 实际调用
/usr/libexec/gcc/x86_64-redhat-linux/4.4.6/cc1 -E -quiet -v hello.c -o hello.i -mtune=generic
```

汇编
```
/usr/libexec/gcc/x86_64-redhat-linux/4.4.6/cc1 -fpreprocessed hello.i -quiet -dumpbase hello.i -mtune=generic -auxbase-strip hello.o -o hello.s
```

目标文件（二进制文件）
```
as -o hello.o hello.s
```

链接
```
ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o hello /usr/lib/gcc/x86_64-redhat-linux/4.4.6/../../../../lib64/crt1.o /usr/lib/gcc/x86_64-redhat-linux/4.4.6/../../../../lib64/crti.o /usr/lib/gcc/x86_64-redhat-linux/4.4.6/crtbegin.o -L/usr/lib/gcc/x86_64-redhat-linux/4.4.6 -L/usr/lib/gcc/x86_64-redhat-linux/4.4.6 -L/usr/lib/gcc/x86_64-redhat-linux/4.4.6/../../../../lib64 -L/lib/../lib64 -L/usr/lib/../lib64 -L/usr/lib/gcc/x86_64-redhat-linux/4.4.6/../../.. hello.o -lgcc --as-needed -lgcc_s --no-as-needed -lc -lgcc --as-needed -lgcc_s --no-as-needed /usr/lib/gcc/x86_64-redhat-linux/4.4.6/crtend.o /usr/lib/gcc/x86_64-redhat-linux/4.4.6/../../../../lib64/crtn.o
```


# 动态编译

# 静态编译
