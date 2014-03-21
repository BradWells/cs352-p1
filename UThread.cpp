#include <ucontext.h>

class UThread {
    //int id
    ucontext_t* context;
    void (*func_p)();
    int priority;

    UThread( void (*given_func_p)(), int given_priority){
        func_p = given_func_p;
        priority = given_priority;
        context = (ucontext_t*) malloc(sizeof(ucontext_t));
        makecontext(context, func_p);
    }

    ~UThread(){
        free(context);
    }

public:

    get_context();
    get_priority();

    //swap_here(UThread other);

};


UThread::get_context(){
    return context;
}

UThread::get_priority(){
    return priority;
}
