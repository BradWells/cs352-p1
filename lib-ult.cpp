#include <queue>
#include "UThread.cpp"
#include <ucontext.h>

using namespace std;

static ucontext_t *current_context_p;

//This is a flag to see whether the returning thread is yielding
// static bool is_yielding = false;

priority_queue<UThread, vector<UThread>, CompareUThread> pq;


// //This is the handler for all of the UThreads
// void run_uthreads(){

//     while(!pq.empty()){
//         next_thread = pq.top();
//         pq.pop();
//         swapcontext(runner_pointer, &(next_thread.get_context()))
//         if(!is_yielding){
//             free(next_thread.get_sp());
//         }
//         else{
//             is_yielding = false;
//         }
//         //Even if it is yielding, a new object is made.
//         delete next_thread;
//     }
//     //Free the stack pointer for runner_pointer:
//     free((*runner_pointer).uc_stack.ss_sp);
// }

int uthread_yield(int priority){
    //Do I have to check any other case?
    if(pq.empty()){
        return -1;
    }
    else{
        //Temporary storage for the context
        getcontext(current_context_p);
        

        UThread next = pq.top();
        pq.pop();
        ucontext_t next_cont = next.context;
        current_context_p = &next_cont;

        swapcontext()
    }

    return 0;

}

void call_next_thread(){
    if(!pq.empty()){
        UThread next = pq.top();
        pq.pop();
        ucontext_t next_cont = next.context;
        current_context_p = &next_cont;

        setcontext(&next_cont);
    }
    else{
        exit(0);
    }
}

void clean_up(){
    //free stack_pointer?
    //delete current_thread?
    //free exit_context_stackpointer?
    //free exit_context?
}

void uthread_exit(){
    clean_up();
    call_next_thread();
}

int uthread_create(void (*func)(), int priority){
    const UThread *new_thread = new const UThread(func, priority);
    pq.push(*new_thread);
    return 0;
}


void print_hi(){
    printf("HI");
    //uthread_exit();
}

void print_hello(){
    printf("Hello");
    //uthread_exit();
}


void system_init(){

    //Make exit context
    getcontext(&exit_context);
    exit_context.uc_stack.ss_sp = malloc(STACK_SIZE);
    exit_context.uc_stack.ss_size = STACK_SIZE;
    makecontext(&exit_context, uthread_exit, 0);

    current_context_p = (ucontext_t*) malloc(sizeof(ucontext_t));
    getcontext(current_context_p);

}

int main(int argc, char* args[]){
    system_init();
    uthread_create(print_hi, 2);
    uthread_create(print_hi, 3);
    uthread_create(print_hi, 3);
    uthread_create(print_hello, 1);
    printf("Created");
    uthread_exit();
    return 0;
}
