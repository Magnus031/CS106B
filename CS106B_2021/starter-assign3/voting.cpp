// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
#include "Set.h"
using namespace std;


/**
 * @brief DFS
 * @details
 * @return
 */
void DFS(Vector<int>&blocks,int index,Vector<Vector<int>>&a){
    int n = blocks.size();
    Vector<int> temp;
    Stack<std::pair<int, Vector<int>>> stack;
    stack.push({0, temp});

    while (!stack.isEmpty()) {
        auto [index, temp] = stack.peek();
        stack.pop();

        if (index == n) {
            a.add(temp);
        } else {
            // 不选择当前元素
            stack.push({index + 1, temp});

            // 选择当前元素
            temp.add(blocks[index]);
            stack.push({index + 1, temp});
        }
    }
}

/**
 * @brief GetTotalVoting
 * @return
 */
int GetTotalVoting(const Vector<int>blocks,Vector<int>a){
    int sum = 0;
    for(int i=0;i<a.size();i++)
        sum+=blocks.get(a[i]);
    return sum;
}



// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int>result;
    Vector<Vector<int>>a;
    Vector<int>p;
    int num = blocks.size();//record the blocks' number;
    int total = 0;//Here total is total sum;
    for(int i=0;i<num;i++){
        p.add(i);
        total+=blocks.get(i);
        result.add(0);//initialize;
    }
    DFS(p,0,a);
    for(int i=0;i<a.size();i++){
        //对于每一个组合subset进行检验 然后把结果记录在result数组中
        int  sum = GetTotalVoting(blocks,a.get(i));
        if(2*sum>total){
            Vector<int>tmp = a.get(i);
            for(int j=0;j<tmp.size();j++){
                if(2*(sum-blocks[tmp[j]])<=total){
                    result[tmp[j]]++;
                }
            }
        }
    }
    int T = 0;
    for(int i=0;i<num;i++)
        T+=result[i];
    for(int i=0;i<num;i++){
        result[i]=(int)(100*result[i])/T;
    }
    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}


