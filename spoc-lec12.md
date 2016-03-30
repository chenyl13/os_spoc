#lec12
##第一个用户进程执行的代码在哪里？它是什么时候加载到内存中的？
第一个用户进程是sh，代码在user/sh.c  

proc_init：创建两个内核进程idleproc、init_main  
init_main：创建user_main  
user_main：执行代码
```
KERNEL_EXECVE(sh);
```
开始执行进程sh