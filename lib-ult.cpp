#include <queue>
#include "UThread.cpp"
#include <ucontext.h>

using namespace std;

UThread* current_thread;

//This is a flag to see whether the returning thread is yielding
// static bool is_yielding = false;

priority_queue<UThread*, vector<UThread*>, CompareUThread> pq;

int uthread_yield(int priority){

    //The top thread should be the current one
    if(current_thread == pq.top()){
        pq.pop();
    }
    else{
        printf("ERROR");
        exit(-1);
    }
    //Do I have to check any other case?
    if(pq.empty()){
        pq.push(current_thread);
        return -1;
    }
    else{

        //Get the next thread
        UThread* next = pq.top();

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

    return 0;

}

void call_next_thread(){
    if(!pq.empty()){
        UThread* next = pq.top();
        ucontext_t next_cont = next->context;

        current_thread = next;

        setcontext(&next_cont);
    }
    else{
        exit(0);
    }
}

void clean_up(){
    UThread* finished = pq.top();
    if(current_thread == finished){
        pq.pop();
        delete finished;
    }
    if(pq.empty()){
        //Free the last few things and exit
        free(exit_context.uc_stack.ss_sp);
        //free exit context
        //exit(0);
    }
}

void uthread_exit(){
    clean_up();
    call_next_thread();
}

int uthread_create(void (*func)(), int priority){
    UThread *new_thread = new UThread(func, priority);
    pq.push(new_thread);
    return 0;
}


void print_hi(){
    printf("HI");
    //uthread_exit();
}

void print_hello(){
    printf("Hello");
    uthread_yield(3);
    uthread_create(print_hi, 4);
    //uthread_yield(3);
    printf("yielded");
    //uthread_exit();
}


void system_init(){
    //Make exit context
    getcontext(&exit_context);
    exit_context.uc_stack.ss_sp = malloc(STACK_SIZE);
    exit_context.uc_stack.ss_size = STACK_SIZE;
    makecontext(&exit_context, uthread_exit, 0);


    bool init = false;

    //Make starter thread
    uthread_create(system_init, 1);
    UThread *this_thread = pq.top();
    current_thread = this_thread;
    //Update where we are
    getcontext(&(this_thread->context));
    (this_thread->context).uc_link = &exit_context;

    if(!init){
        init = true;
        setcontext(&(this_thread->context));
    }
        
}

int main(int argc, char* args[]){
    system_init();
    uthread_create(print_hi, 2);
    uthread_create(print_hello, 1);
    uthread_create(print_hi, 3);
    uthread_create(print_hi, 3);
    printf("Created");
    uthread_exit();
}
