#spoc-lab7
##cvp->count含义是什么？cvp->count是否可能<0, 是否可能>1？请举例或说明原因。

cvp->count表示正在等待此条件变量的进程数。  
cond_wait函数中对其操作如下：  
```
      cvp->count++;
      if(cvp->owner->next_count > 0)
         up(&(cvp->owner->next));
      else
         up(&(cvp->owner->mutex));
      down(&(cvp->sem));
      cvp->count --;
```  
可见cvp->count是先加后减的，因此不可能<0。  
当同时有多个进程处于等待此条件变量的状态时，cvp->count>1。

##cvp->owner->next_count含义是什么？cvp->owner->next_count是否可能<0, 是否可能>1？请举例或说明原因。

cvp->owner->next_count是发出信号后等待的进程数。  
cond_signal函数中的操作如下：  
```
if(cvp->count>0) {
        cvp->owner->next_count ++;
        up(&(cvp->sem));
        down(&(cvp->owner->next));
        cvp->owner->next_count --;
      }
```  
可见cvp->owner->next_count是先加后减的，因此不可能<0。  
当进程A发出信号后进入睡眠，next_count++，在唤醒另一进程执行完毕后，再次唤醒此进程，next_count--，因此cvp->owner->next_count不可能＞1。

##目前的lab7-answer中管程的实现是Hansen管程类型还是Hoare管程类型？请在lab7-answer中实现另外一种类型的管程。

lab7-answer中管程的实现是Hoare管程类型。
