//
// Created by patates on 7/4/18.
//

#include "testmylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "CUnit/CUnit.h"
#include "CUnit/Automated.h"
#include "mylib.h"

int inarr[] = { 3,9,7,8,6,4};
int empty[] = {};

/**//*---- test cases ------------------*/
void testinArr()
{
    CU_ASSERT_EQUAL(find(inarr,6,3),0);
    CU_ASSERT_EQUAL(find(inarr,6,8),3);
    CU_ASSERT_EQUAL(find(inarr,6,4),5);
}

void testNotinArr()
{
    CU_ASSERT_EQUAL(find(inarr,6,11),-1);
    CU_ASSERT_EQUAL(find(inarr,6,10),-1);
    CU_ASSERT_EQUAL(find(inarr,6,0),-1);
}

void testEmptyArr()
{
    CU_ASSERT_EQUAL(find(empty,0,0),-1);
    CU_ASSERT_EQUAL(find(empty,0,9),-1);

}

CU_TestInfo testcases[] = {
        {"Testing in Array :", testinArr},
        {"Testing not in Array :", testNotinArr},
        {"Testing Empty Array:", testEmptyArr},
        CU_TEST_INFO_NULL
};
/**//*---- test suites ------------------*/
int suite_success_init(void) { return 0; }
int suite_success_clean(void) { return 0; }

CU_SuiteInfo suites[] = {
        {"Testing the function find:", suite_success_init, suite_success_clean, NULL, NULL, testcases},
        {"Testing the function find again:",suite_success_init, suite_success_clean, NULL, NULL, testcases},
        CU_SUITE_INFO_NULL
};
/**//*---- setting enviroment -----------*/
void AddTests(void)
{
    assert(NULL != CU_get_registry());
    assert(!CU_is_test_running());
    /**//* shortcut regitry */

    if(CUE_SUCCESS != CU_register_suites(suites)){
        fprintf(stderr, "Register suites failed - %s ", CU_get_error_msg());
        exit(1);
    }
}