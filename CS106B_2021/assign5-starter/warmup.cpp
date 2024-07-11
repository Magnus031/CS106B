#include "listnode.h"
#include "random.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function is a buggy implementation of freeing a
 * linked list. In particular, on every iteration of the while
 * loop, the function attempts to access memory that was just
 * deallocated.
 */
void badDeallocate(ListNode* list) {
    while (list != nullptr) {
        ListNode* temp=list;
        list = list->next;  // BAD: accesses deallocated memory
        delete temp;
    }
}

PROVIDED_TEST("Allocation counts: 1 new, 0 delete") {
    ListNode* single = new ListNode(5, nullptr);
    EXPECT_EQUAL(single->data, 5);
    delete single;
}

PROVIDED_TEST("Allocation counts: 10 new, 1 delete") {
    ListNode* list = nullptr;
    for (int i = 0; i < 10; i++) {
        list = new ListNode(i, list);
    }
    while(list!=nullptr){
        ListNode* temp = list;
        list = list->next;
        delete temp;
    }
}

PROVIDED_TEST("Allocation counts: 1 new, 2 delete") {
    ListNode* list = new ListNode(5, nullptr);
    delete list;  // delete same memory twice
}

PROVIDED_TEST("badDeallocate: low exposure, one attempt, short list") {
    ListNode* list = nullptr;
    for (int i = 0; i < 3; i++) {
        list = new ListNode(i, list);
    }
    badDeallocate(list);
}

PROVIDED_TEST("badDeallocate: more exposure, repeated attempts, long lists") {
    for (int n = 0; n < 5; n++) { // try larger number of trials to surface error?
        ListNode* list = nullptr;
        int size = randomInteger(1000, 2000);
        while (--size > 0) {
            list = new ListNode(5, list);
        }

        badDeallocate(list);
    }
}

/*
 * Buggy test that fails to properly initialize variables.
 * What are the consequences of using a garbage integer?
 * What are the consequences of using a garbage pointer?
 */
PROVIDED_TEST("Segmentation fault: use of uninitialized values") {
    ListNode* p = new ListNode; // allocate new node, don't assign data or next field, leave uninitialized

    p->data += 10;   // add 10 to garbage, what happens?
    p = new ListNode(10,p);
    p->next->data = 5; // dereference garbage address, what happens?

    while(p!=nullptr){
        ListNode* temp = p;
        p = p->next;
        delete temp;
    }
}
