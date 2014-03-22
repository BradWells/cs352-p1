#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 16384

extern void uthread_exit();

ucontext_t exit_context;

class UThread {
    void *stack_pointer;

public:

    ucontext_t context;
    void (*func_p)();
    int priority;

    UThread( void (*given_func_p)(), int given_priority){
        func_p = given_func_p;
        priority = given_priority;

        getcontext(&context);
        //Easier variable name.
        stack_pointer = malloc(STACK_SIZE);
        if(stack_pointer == NULL){
            exit(-1);
        }
        context.uc_stack.ss_sp = stack_pointer;
        context.uc_stack.ss_size = STACK_SIZE;
        context.uc_link = &exit_context;
        makecontext(&context, func_p, 0);

    }

    //Contstructor for yielding, uses the current context
    UThread( ucontext_t *context_p, int given_priority){
        context = *context_p;
        priority = given_priority;
    }

    ~UThread(){
        //This is gon' break
        if(stack_pointer != NULL){
            free(stack_pointer);
        }
    }
};


//Source:
//http://stackoverflow.com/questions/19535644/how-to-use-the-priority-queue-stl-for-objects
struct CompareUThread{
    bool operator()(const UThread* t1, const UThread* t2) {
        // return "true" if "p1" is ordered before "p2", for example:
        return t1->priority > t2->priority;
    }
};