#lab4
##练习1：分配并初始化一个进程控制块（需要编码）
###设计实现过程
对proc_struct结构的每个成员初始化，初始cr3置为boot_cr3，其余清零或置空。

###proc_struct中struct context context和struct trapframe *tf成员变量含义和在本实验中的作用
‘’‘
struct context {
    uint32_t eip;
    uint32_t esp;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
};
’‘’
context结构保存了通用寄存器信息，其用处为：  
1、进程被创建时，在copy_thread函数中设置context中的eip和esp，用于创建完成后跳转执行。  
2、用于在切换进程时保存当前运行状态，并在下次运行前恢复。  
用处1在本实验中用到。

‘’‘
struct trapframe {
    struct pushregs tf_regs;
    uint16_t tf_gs;
    uint16_t tf_padding0;
    uint16_t tf_fs;
    uint16_t tf_padding1;
    uint16_t tf_es;
    uint16_t tf_padding2;
    uint16_t tf_ds;
    uint16_t tf_padding3;
    uint32_t tf_trapno;
    /* below here defined by x86 hardware */
    uint32_t tf_err;
    uintptr_t tf_eip;
    uint16_t tf_cs;
    uint16_t tf_padding4;
    uint32_t tf_eflags;
    /* below here only when crossing rings, such as from user to kernel */
    uintptr_t tf_esp;
    uint16_t tf_ss;
    uint16_t tf_padding5;
} __attribute__((packed));
’‘’
trapframe结构用于在中断、异常或系统调用时，保存进程上下文，记录异常类型，用于执行异常处理以及之后恢复现场。  
本实验中，load_icode函数设置好trapframe，相当于创建了进程的上下文，通过do_exit函数从中断返回，就可以切换到用户态。

##练习2：为新创建的内核线程分配资源（需要编码）
###设计实现过程
本实验在do_fork函数中，完成进程的创建，步骤为：  
1、调用alloc_proc，首先获得一块用户信息块。  
2、调用setup_kstack为进程分配一个内核栈。  
3、调用copy_mm复制原进程的内存管理信息到新进程（但内核线程不必做此事）  
4、调用copy_thread复制原进程上下文到新进程  
5、将新进程添加到进程列表，获取pid  
6、调用wakeup_proc唤醒新进程  
7、返回新进程号  
如果上述过程失败，需要回收已分配的内核栈，用户信息块。
    
###ucore是否做到给每个新fork的线程一个唯一的id？请说明你的分析和理由。
ucore中定义最大pid大于最大线程数，并通过查询整个当前进程链表获得不重复的pid，因此是唯一的。

##练习3：阅读代码，理解 proc_run 函数和它调用的函数如何完成进程切换的。（无编码工作）

###proc_run函数分析
‘’‘
void
proc_run(struct proc_struct *proc) {
    if (proc != current) {
        bool intr_flag;
        struct proc_struct *prev = current, *next = proc;
        local_intr_save(intr_flag);
        {
            current = proc;
            load_esp0(next->kstack + KSTACKSIZE);
            lcr3(next->cr3);
            switch_to(&(prev->context), &(next->context));
        }
        local_intr_restore(intr_flag);
    }
}
’‘’
proc_run函数的作用是完成两个进程的上下文切换，包括切换当前进程控制块、内核堆栈、页表和通用寄存器信息。

###在本实验的执行过程中，创建且运行了几个内核线程？
两个，idle和init。

###语句local_intr_save(intr_flag);....local_intr_restore(intr_flag);在这里有何作用?请说明理由

这两句用于在切换进程前关闭中断，之后恢复中断。  
进程切换时遇到中断可能会改变当前环境从而出错。

##与参考答案的区别
练习1：基本一致  
练习2：答案中加入新进程时关闭了中断

##本实验中重要的知识点
###进程创建
创建新的进程时，需要为其分配所需资源并完成初始设置工作。这点与原理课讲述相似，增加了一些实用细节，比如通过填写trapframe，在创建完毕后返回到用户态， 以及创建失败时对分配资源的回收。

###进程状态
实验涉及不多，主要是进程尚未创建完毕时设为UNINIT态，创建完成后唤醒进程进入RUNNABLE态。

###进程切换
进程切换包括切换当前进程控制块、内核堆栈、页表和通用寄存器信息。实验中强调了切换过程中需要禁止中断。

##OS原理中很重要，但在实验中没有对应上的知识点
###资源分配
实验中只创建了内核线程，没有创建用户进程为其分配资源。