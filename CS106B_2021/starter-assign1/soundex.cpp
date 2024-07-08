/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <cctype>
#include <fstream>
#include <string>
#include <algorithm>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "SimpleTest.h"
#include "soundex.h"
using namespace std;

/* This function is intended to return a string which
 * includes only the letter characters from the original
 * (all non-letter characters are excluded)
 *
 * WARNING: The provided code is buggy!
 *
 * Add student test cases to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string lettersOnly(string s) {
    string result="";
    if(isalpha(s[0]))
        result = charToString(s[0]);
    for (int i = 1; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/* New Function That we convert a char into a code;
 *
 */
int ConvertEncode(char s){
    int flag=-1;
    if(isalpha(s)){
        s = toUpperCase(s);//We change it into uppercase word;
        if(s=='A'||s=='E'||s=='I'||s=='O'||s=='U'||s=='H'||s=='W'||s=='Y'){
            flag = 0;
        }else if(s=='B'||s=='F'||s=='P'||s=='V'){
            flag = 1;
        }else if(s=='C'||s=='G'||s=='J'||s=='K'||s=='Q'||s=='S'||s=='X'||s=='Z')
            flag = 2;
        else if(s=='D'||s=='T')
            flag = 3;
        else if(s=='L')
            flag = 4;
        else if(s=='M'||s=='N')
            flag = 5;
        else if(s=='R')
            flag = 6;
    }
    return flag;
}

/* New Function to deal with the duplicate
 *
 */
string NonDuplicate(string s){
    //Deal with the problem of Duplicate;
    string result="";
    int ptr = 0;
    int flag = 0;
    for(int i=1;i<s.length();i++){
        while(s[i]==s[ptr]){
            i++;
            if(i==s.length()){
                flag = 1;
                break;
            }

        }
        if(flag)
            break;
        result+=s[ptr];
        ptr = i;
    }
    result+=s[ptr];
    return result;
}

/* New Function :Reuse Zero
 */
string ReuseZero(string s){
    string result=charToString(s[0]);
    for(int i=1;i<s.length();i++){
        if(s[i]!='0')
            result+=s[i];
    }
    int size = result.length();
    if(size<4){
        for(int i=0;i<4-size;i++){
            result+='0';
        }
    }
    return result;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
string soundex(string s) {
    /* TODO: Fill in this function. */
    //Here we do deal with the problem of non-letter;
    s = lettersOnly(s);
    string result = "";
    for(int i=0;i<s.length();i++){
        char temp = s[i];
        result += (ConvertEncode(temp)+'0');
    }
    result = NonDuplicate(result);
    result = toUpperCase(charToString(s[0]))+result.substr(1,result.length());
    result = ReuseZero(result);

    if(result.length()>4)
        result = result.substr(0,4);

    return result;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // This provided code opens the specified file
    // and reads the lines into a vector of strings
    ifstream in;
    Vector<string> allNames;

    if (openFile(in, filepath)) {
        allNames = readLines(in);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;

    // The names read from file are now stored in Vector allNames

    /* TODO: Fill in the remainder of this function. */
    Vector<pair<string,string>> vec;
    for(int i=0;i<allNames.size();i++){
        string name = allNames[i];
        string result = soundex(name);
        pair<string,string>a;
        a.first = result;
        a.second = name;
        vec.add(a);
    }
    vec.sort();
    string input;
    input = getLine("Enter a surname (RETURN to quit):");
    while(input!=""){
        string ans = soundex(input);
        cout<<"Soundex code is "<<ans<<endl;
        for(int i=0;i<vec.size();i++){
            if(vec[i].first==soundex(input)){
                cout<<"Matches from databases: {"<<vec[i].second<<"\"";
                while(vec[i].first==ans){
                    cout<<", "<<vec[i].second<<"\"";
                    i++;
                }
                cout<<"}"<<endl;
                cout<<endl;
                break;
            }
        }
        input = getLine("Enter a surname (RETURN to quit):");
    }
    cout<<"Back in main(): FINISHED!"<<endl;
}


/* * * * * * Test Cases * * * * * */

// TODO: add your STUDENT_TEST test cases here!


/* Please not add/modify/remove the PROVIDED_TEST entries below.
 * Place your student tests cases above the provided tests.
 */

PROVIDED_TEST("Test exclude of punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");
}
PROVIDED_TEST("StudentTest For lettersOnly"){
    EXPECT_EQUAL(lettersOnly("'"),"");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

PROVIDED_TEST("StuTest-convert"){
    EXPECT_EQUAL(ConvertEncode('f'),1);
    EXPECT_EQUAL(ConvertEncode('$'),-1);
    EXPECT_EQUAL(ConvertEncode('E'),0);
    EXPECT_EQUAL(ConvertEncode('D'),3);
}
PROVIDED_TEST("StuTest_NonDuplicate"){
    EXPECT_EQUAL(NonDuplicate("C40000"),"C40");
    EXPECT_EQUAL(NonDuplicate("C4"),"C4");
    EXPECT_EQUAL(NonDuplicate("C432"),"C432");
    EXPECT_EQUAL(NonDuplicate("C432002"),"C43202");
    EXPECT_EQUAL(NonDuplicate("C432442"),"C43242");
}
PROVIDED_TEST("StudentTest_ReuseZero"){
    EXPECT_EQUAL(ReuseZero("Z1000"),"Z100");
    EXPECT_EQUAL(ReuseZero("Z10708"),"Z178");
    EXPECT_EQUAL(ReuseZero("Z0706"),"Z760");
    EXPECT_EQUAL(ReuseZero("C0600"),"C600");
}
