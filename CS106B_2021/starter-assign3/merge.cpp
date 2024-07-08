/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;
/**
 * @brief Checkorder
 * @details Checkorder function is to check whether the queue is in order?!
 * @param a
 * @return
 */
bool Checkorder(Queue<int>a){
    if(a.isEmpty())
        return true;
    int m = a.peek();
    a.dequeue();
    while(!a.isEmpty()){
        int temp = a.peek();
        a.dequeue();
        if(temp<m){
            return false;
        }
        m = temp;
    }
    return true;
}



/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
/**
 * @brief binaryMerge
 * @details The elements of Queues one and two are expected to be in increasing order from front to back.
 * The returned result is a Queue containing the combined elements from one and two in increasing order from front to back.
 * @param a
 * @param b
 * @return Queue<int>
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    /* TODO: Implement this function. */
    //detect;
    if(!Checkorder(a)||!Checkorder(b)){
        error("An error has run across!");
    }

    while(!a.isEmpty()&&!b.isEmpty()){
        int m = a.peek();
        int n = b.peek();
        if(m <= n){
            result.enqueue(m);
            a.dequeue();
        }else{
            result.enqueue(n);
            b.dequeue();
        }
    }
    while(!a.isEmpty()){
        result.enqueue(a.peek());
        a.dequeue();
    }
    while(!b.isEmpty()){
        result.enqueue(b.peek());
        b.dequeue();
    }
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    /* TODO: Implement this function. */
    int n = all.size();
    if(n==0){
        result.clear();
        return result;
    }else if(n==1){
        return all[0];
    }else if(n==2){
        return binaryMerge(all[0],all[1]);
    }
    Vector L = all.subList(0,n/2);
    Queue<int> left = recMultiMerge(L);
    Vector R = all.subList(n/2,n-n/2);
    Queue<int> right = recMultiMerge(R);
    result = binaryMerge(left,right);
    return result;
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("Student-Test1 Detected for error") {
    Queue<int> a = {4, 2, 5};
    Queue<int> b = {1, 3, 3};
    EXPECT_ERROR(binaryMerge(a,b));
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("Student-Test naiveMultiMerge") {
    Vector<Queue<int>> all = {{},
        {1},
        {},
        {},
        {},
        {}
    };
    Queue<int> expected = {1};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}


PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 7;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 100000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}
