#include "lib-ult.cpp"

void print_hi(){
    printf("HI\n");
    //uthread_exit();
}

void print_hello(){
    printf("Hello\n");
    uthread_yield(3);
    uthread_create(print_hi, 4);
    //uthread_yield(3);
    printf("yielded");
    //uthread_exit();
}

int main(int argc, char* args[]){
    system_init();
    uthread_create(print_hi, 2);
    // uthread_create(print_hello, 1);
    // uthread_create(print_hi, 3);
    // uthread_create(print_hi, 3);
    // printf("Created!\n");
    // uthread_exit();
}