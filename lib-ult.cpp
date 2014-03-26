#include <queue>
#include "UThread.cpp"
#include <ucontext.h>

using namespace std;

UThread* current_thread;

//This is a flag to see whether the returning thread is yielding
// static bool is_yielding = false;

priority_queue<UThread*, vector<UThread*>, CompareUThread> pq;

int uthread_yield(int priority){
    if(initialized){

        //Do I have to check any other case?
        if(pq.empty()){
            return -1;
        }
        else{

            //Get the next thread
            UThread* next = pq.top();
            pq.pop();

            ucontext_t next_cont = next->context;

            //Set up yielding flag
            bool yielding = true;

            //The current thread is yielding
            //Put the new context into the current thread.
            current_thread->priority = priority;
            getcontext(&(current_thread->context));

            if(yielding){
                yielding = false;
                //Requeue it
                pq.push(current_thread);

                current_thread = next;

                setcontext(&next_cont);
            }

        }
    }
    else{
        fprintf(stderr, "Uthread system not initialized, but tried to use uthread_yield");
        exit(-1);
    }
    return 0;

}

void call_next_thread(){
    if(!pq.empty()){
        UThread* next = pq.top();
        pq.pop();

        ucontext_t next_cont = next->context;

        current_thread = next;

        setcontext(&next_cont);
    }
    else{
        exit(0);
    }
}

void clean_up(){
    delete current_thread;
    if(pq.empty()){
        //Free the last few things and exit
        free(exit_context.uc_stack.ss_sp);
        //free exit context
        //exit(0);
    }
}

void uthread_exit(){
    if(initialized){
        clean_up();
        call_next_thread();
    }
    else{
        fprintf(stderr, "Uthread system not initialized, but tried to use uthread_exit");
        exit(-1);
    }
}

int uthread_create(void (*func)(), int priority){
    if(initialized){
        UThread *new_thread = new UThread(func, priority);
        pq.push(new_thread);
        return 0;
    }
    else{
        fprintf(stderr, "Uthread system not initialized, but tried to use uthread_create");
        exit(-1);
    }
}


void system_init(){
    //Make exit context
    getcontext(&exit_context);
    exit_context.uc_stack.ss_sp = malloc(STACK_SIZE);
    exit_context.uc_stack.ss_size = STACK_SIZE;
    makecontext(&exit_context, uthread_exit, 0);

    initialized = true;

    //This next part is an attempt to get main to work without
    //the use of uthread_exit.  It doen't work right now.

    bool is_context_set = false;

    //Make starter thread
    uthread_create(system_init, 1);
    UThread *this_thread = pq.top();
    pq.pop();
    current_thread = this_thread;

    //Update where we are
    (this_thread->context).uc_link = &exit_context;
    getcontext(&(this_thread->context));

    if(!is_context_set){
        is_context_set = true;
        setcontext(&(this_thread->context));
    }
        
}

