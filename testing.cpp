
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
// #include "UThread.cpp"


#define STACK_SIZE 16384

ucontext_t ucp1, ucp2;

void do_this(){
    printf("hi");
}

void do_this_too(){
    printf("Yaaaay");
}

int main(int argc, char* args[]){


    ucontext_t exit_context;

    //Make exit context
    ucontext_t* exit_context_p = (ucontext_t*) malloc(sizeof(ucontext_t));
    getcontext(exit_context_p);
    (*exit_context_p).uc_stack.ss_sp = malloc(STACK_SIZE);
    (*exit_context_p).uc_stack.ss_size = STACK_SIZE;
    makecontext(exit_context_p, do_this_too, 0);

    //initialize the first context structure;
    //its stack pointer points to the stack of the current thread
    getcontext(&ucp1); 

    //initialize another context structure 
    //for a thread running another_thread(); 
    //its stack pointer points to a newly allocated stack 
    getcontext(&ucp2);
    ucp2.uc_stack.ss_sp=malloc(16384);
    ucp2.uc_stack.ss_size=16384;
    ucp2.uc_link = exit_context_p;
    makecontext(&ucp2, do_this ,0);

    swapcontext(&ucp1, &ucp2);

    // UThread* newthread = new UThread(do_this, 1);

    printf("hi2\n");

}
