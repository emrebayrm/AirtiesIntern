#include <stdio.h>
#include <stdlib.h>
#include "testmylib.h"

int main() {
    if(CU_initialize_registry()){
        fprintf(stderr, " Initialization of Test Registry failed. ");
        exit(1);
    }else{
        fprintf(stderr,"asd");
        AddTests();
        CU_set_output_filename("TestFind");
        CU_list_tests_to_file();
        CU_automated_run_tests();
        CU_cleanup_registry();
    }
    return 0;
}
