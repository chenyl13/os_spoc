#spoc_lab5：理解用户进程的生命周期
##用户进程的启动、运行、就绪、等待、退出
启动：由do_fork函数建立PCB、堆栈、页表等，然后由wakeup_proc唤醒进入就绪态，等待调度。  
运行：由proc_run填写esp0和cr3，调用switch_to切换上下文后开始运行。  
就绪：启动后、等待占用cpu时为就绪态，等待调度。  
等待：do_wait函数中，进程处于waiting态，等待子进程运行完毕进入zombie态后，释放子进程PCB。  
退出：do_exit中，修改cr3，释放mm，进入zombie态。  

##用户进程的管理与简单调度
schedule函数用于进程调度，在当前进程时间片用完，或进入等待、退出状态时，会寻找下一个可执行的进程。

##用户进程的上下文切换过程
上下文切换发生于进程调度后，proc_run函数填写esp0和cr3，调用switch_to函数切换上下文。

##用户进程的特权级切换过程
用户进程在执行系统调用时进入内核态，

##用户进程的创建过程并完成资源占用
由do_fork函数建立PCB、堆栈、页表等。

##用户进程的退出过程并完成资源回收
do_exit函数完成修改cr3，释放mm，进程进入zombie态。  
由父进程在do_wait函数中释放子进程PCB。

##运行输出
```
in proc_run	switch from proc 0 to proc 1
in do_fork	for proc 2: alloc proc_struct, setup kstack, copy mm, copy thread
in wakeup	wake up proc 2
in do_fork	for proc 3: alloc proc_struct, setup kstack, copy mm, copy thread
in wakeup	wake up proc 3
in do_wait	change proc 1 state to PROC_SLEEPING
in proc_run	switch from proc 1 to proc 3
kernel_execve: pid = 3, name = "exit".
in kernel_execve	exec proc 3
in do_execve	current priv=0
in user proc	start running pid = 3
in user proc	forking a child pid = 3
in do_fork	for proc 4: alloc proc_struct, setup kstack, copy mm, copy thread
in wakeup	wake up proc 4
in user proc	I am parent, fork a child pid = 4
in user proc	I am the parent, waiting now.. my pid = 3
in do_wait	change proc 3 state to PROC_SLEEPING
in proc_run	switch from proc 3 to proc 2
kernel_execve: pid = 2, name = "exit".
in kernel_execve	exec proc 2
in do_execve	current priv=0
in user proc	start running pid = 2
in user proc	forking a child pid = 2
in do_fork	for proc 5: alloc proc_struct, setup kstack, copy mm, copy thread
in wakeup	wake up proc 5
in user proc	I am parent, fork a child pid = 5
in user proc	I am the parent, waiting now.. my pid = 2
in do_wait	change proc 2 state to PROC_SLEEPING
in proc_run	switch from proc 2 to proc 5
in user proc	I am the child. pid = 5 going to yield
in proc_run	switch from proc 5 to proc 4
in user proc	I am the child. pid = 4 going to yield
in proc_run	switch from proc 4 to proc 5
in schedule	switch proc 5 to proc 4
in do_exit	change cr3, mm destroy, change proc 5 state to PROC_ZOMBIE
in wakeup	wake up proc 2
in proc_run	switch from proc 5 to proc 4
in schedule	switch proc 4 to proc 5
in do_exit	change cr3, mm destroy, change proc 4 state to PROC_ZOMBIE
in wakeup	wake up proc 3
in proc_run	switch from proc 4 to proc 3
in schedule	switch proc 3 to proc 2
in user proc	waitpid 4 ok, pid = 3 going to exit
in do_exit	change cr3, mm destroy, change proc 3 state to PROC_ZOMBIE
in wakeup	wake up proc 1
in proc_run	switch from proc 3 to proc 2
in schedule	switch proc 2 to proc 5
in user proc	waitpid 5 ok, pid = 2 going to exit
in do_exit	change cr3, mm destroy, change proc 2 state to PROC_ZOMBIE
in proc_run	switch from proc 2 to proc 1
in schedule	switch proc 1 to proc 3
in schedule	switch proc 1 to proc 1
in schedule	switch proc 1 to proc 1
all user-mode processes have quit.
```