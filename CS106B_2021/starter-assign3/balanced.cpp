/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
string operatorsFrom(string str) {
    /* TODO: Implement this function. */
    if(str=="")
        return "";
    else if(str[0]=='('||str[0]==')'||str[0]=='{'||str[0]=='}'||str[0]=='['||str[0]==']')
        return charToString(str[0])+operatorsFrom(str.substr(1,str.size()));
    else
        return operatorsFrom(str.substr(1,str.size()));
}



/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool operatorsAreMatched(string ops) {
    /* TODO: Implement this function. */
    if(ops=="")
        return true;
    for(int i=0;i<ops.size()-1;i++){
        string result = charToString(ops[i])+charToString(ops[i+1]);
        if(result=="()"||result=="{}"||result=="[]"){
            if(i==0){
                string temp = ops.substr(i+2,ops.size());
                return operatorsAreMatched(temp);
            }else{
                string temp1 = ops.substr(0,i);
                string temp2 = ops.substr(i+2,ops.size());
                string temp = temp1+temp2;
                return operatorsAreMatched(temp);
            }
        }
    }
    return false;
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("studentTest-operatorsForm"){
    EXPECT_EQUAL(operatorsFrom("{ve)c[3]"), "{)[]");
    EXPECT_EQUAL(operatorsFrom("{"), "{");
    EXPECT_EQUAL(operatorsFrom("{ve)c[3)]("), "{)[)](");
}

PROVIDED_TEST("student-Tests-operatorsAreMatched"){
    EXPECT(!operatorsAreMatched("{}("));
    EXPECT(operatorsAreMatched("({})"));
    EXPECT(operatorsAreMatched("{}[]"));
    EXPECT(!operatorsAreMatched("{[}]"));
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}
PROVIDED_TEST("isBalanced student-test"){
    EXPECT(isBalanced("{2(s )x y}"));
    EXPECT(!isBalanced("{2(s [)x y}]"));
}
