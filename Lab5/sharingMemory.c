#include "types.h"
#include "user.h"


int main(int argc, char* argv[]){

    init_sharedmem();
    char* shared_mem = open_sharedmem(1);
    char* value = (char*) shared_mem;
    *value = 0;
    

    for (int i = 0; i < 5; i++){
        if (fork() == 0){
            char* shared_mem = open_sharedmem(1);
            char* value = (char*) shared_mem;
          
            *value += 1;
            printf(1, "child process %d : %d\n", i, *value);
            
            close_sharedmem(1);
            exit();
        }  
    }

    for(int i = 0; i < 5; i++) {
        wait();
    }
    
    printf(1, "Parent process: %d\n", *value);
    close_sharedmem(1);
        
    exit();
}






