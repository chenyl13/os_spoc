#lab6
##练习1: 使用 Round Robin 调度算法（不需要编码）

###请理解并分析sched_calss中各个函数指针的用法，并接合Round Robin 调度算法描ucore的调度执行过程
```
struct sched_class {
    // the name of sched_class
    const char *name;
    // Init the run queue
    void (*init)(struct run_queue *rq);
    // put the proc into runqueue, and this function must be called with rq_lock
    void (*enqueue)(struct run_queue *rq, struct proc_struct *proc);
    // get the proc out runqueue, and this function must be called with rq_lock
    void (*dequeue)(struct run_queue *rq, struct proc_struct *proc);
    // choose the next runnable task
    struct proc_struct *(*pick_next)(struct run_queue *rq);
    // dealer of the time-tick
    void (*proc_tick)(struct run_queue *rq, struct proc_struct *proc);
};
```

sched_class使用函数指针，使调度类的函数实现和结构体分离。使用时函数指针指向具体的调度算法实现函数。  
init函数用于调度算法的相关初始化工作，enqueue、dequeue函数用于进程入队和出队，pick_next函数用于在调度时选择需要执行的进程，proc_tick函数用于在时钟中断时做相应处理。

Round Robin算法中进程按FIFO排列，每个进程有一个时间片，每次时钟中断时，当前执行进程的时间片减1，当前进程时间片为0后，加入就绪队列的末尾，再从头部取出新进程执行。
    
###请在实验报告中简要说明如何设计实现”多级反馈队列调度算法“，给出概要设计，鼓励给出详细设计

在多级反馈队列调度算法中，设置不同优先级的多个就绪队列，不同优先级的时间片大小不同。  
仅当比当前就绪队列优先级高的所有队列都为空时，从当前队列首取出一个进程执行。当进程进入队列时，先加入优先级最高的队列的末尾，若是该进程没有在时间片内运行完，则之后加入到下一优先级的队列中。  
若有比正在执行的进程优先级高的进程加入队列，则会抢占CPU，将原执行进程放回原队列，执行新进程。

##练习2: 实现 Stride Scheduling 调度算法（需要编码）
###设计思路

Stride Scheduling算法中，每个进程有stride和priority值，决定调度。每次调度时，选取stride值最小的进程执行，并将此进程stride值加上步进pass=BIG_STRIDE/priority。每次时钟中断时，将时间片减1。

###实现过程
1、用default_sched_stride_c覆盖default_sched.c  
2、取BIG_STRIDE值为0x7fffffff  
3、init函数：初始化run_list、lab6_run_pool，将proc_num置0  
4、enqueue函数：进程加入堆，重新计算时间片，更改proc_num计数  
5、dequeue函数：进程出堆，更改proc_num计数  
6、pick_next函数：选取stride值最小的进程执行，并将此进程stride值加上步进pass=BIG_STRIDE/priority  
7、proc_tick函数：将时间片减1，为0时设置需要调度  

##与参考答案实现上的区别
练习二：基本一致

##本实验中重要的知识点

###进程调度算法
本次实验主要围绕调度算法，与原理课讲述基本一致，更偏向于具体实现，比如使用函数指针，达到调度结构和具体算法的分离，还有stride算法中，BIG_STRIDE的取值范围，priority为0的特殊处理等。