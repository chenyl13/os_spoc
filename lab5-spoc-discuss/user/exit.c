#include <stdio.h>
#include <ulib.h>

int magic = -0x10384;

int
main(void) {
    int pid, childpid, code;
    pid = getpid();
    cprintf("in user proc\tstart running pid = %d\n", pid);
	
    cprintf("in user proc\tforking a child pid = %d\n", pid);
    if ((childpid = fork()) == 0) {
	pid = getpid();
        cprintf("in user proc\tI am the child. pid = %d going to yield\n", pid);
	//sleep(1);

        yield();
/*        yield();
        yield();
        yield();
        yield();
        yield();
        yield();
*/
        exit(magic);
    }
    else {
        cprintf("in user proc\tI am parent, fork a child pid = %d\n",childpid);
	//wait();
    }

    assert(pid > 0);
    cprintf("in user proc\tI am the parent, waiting now.. my pid = %d\n", pid);

    assert(waitpid(childpid, &code) == 0 && code == magic);
    assert(waitpid(childpid, &code) != 0 && wait() != 0);
    cprintf("in user proc\twaitpid %d ok, pid = %d going to exit\n", childpid, pid);

    return 0;
}

