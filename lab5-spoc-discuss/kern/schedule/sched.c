#include <list.h>
#include <sync.h>
#include <proc.h>
#include <sched.h>
#include <assert.h>

static void print_state(int id, int state){
	if(state == 0){
		cprintf("\tproc %d state unint\n", id);
	}else if(state == 1){
		cprintf("\tproc %d state sleeping\n", id);
	}else if(state == 2){
		cprintf("\tproc %d state runnable\n", id);
	}else if(state == 3){
		cprintf("\tproc %d state zombie\n", id);
	}
}

void
wakeup_proc(struct proc_struct *proc) {
    assert(proc->state != PROC_ZOMBIE);
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        if (proc->state != PROC_RUNNABLE) {
            proc->state = PROC_RUNNABLE;
            proc->wait_state = 0;
        }
        else {
            warn("wakeup runnable process.\n");
        }
    }
    local_intr_restore(intr_flag);
}

void
schedule(void) {
    bool intr_flag;
    list_entry_t *le, *last;
    struct proc_struct *next = NULL;
    local_intr_save(intr_flag);
    {
        current->need_resched = 0;
        last = (current == idleproc) ? &proc_list : &(current->list_link);
        le = last;
        do {
            if ((le = list_next(le)) != &proc_list) {
                next = le2proc(le, list_link);
                if (next->state == PROC_RUNNABLE) {
                    break;
                }
            }
        } while (le != last);
        if (next == NULL || next->state != PROC_RUNNABLE) {
            next = idleproc;
        }
        next->runs ++;
        if (next != current) {
            proc_run(next);
        }
    }
    local_intr_restore(intr_flag);

    cprintf("    schedule:\n");
    for(int i = 0; i < 10; i++){
    	struct proc_struct * proc = find_proc(i);
    	if(proc == NULL){
    		continue;
    	}
    	//cprintf("\tpid %d state=%d\n",i, proc->state);
    	print_state(i, proc->state);
    }
}

