// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "stdlib.h"
#include "simpio.h"
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
/**
 * @brief cleanToken
 * @param s the original string
 * @return result the string after cleaned
 */
string cleanToken(string s)
{
    int start =0;
    int ptr =s.size()-1;
    string result="";
    //To find the first character.
    while(!isalpha(s[start])){
        start++;
    }
    //To find the last character.
    while(!isalpha(s[ptr])){
        ptr--;
    }
    if(ptr<start)
        return "";
    else{
        for(int i=start;i<ptr+1;i++)
            result+= charToString(toLowerCase(s[i]));
        return result;
    }

}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
/**
 * @brief gatherTokens
 * @param text the orignal text;
 * @return set of the tokens;
 */
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    string temp = "";
    Vector<string>a;
    a = stringSplit(text," ");
    for(int i=0;i<a.size();i++){
        temp = cleanToken(a[i]);
        if(temp!=""){
            tokens.add(temp);
        }
    }
    return tokens;
}

void ReadDbFile(string dbfile,Vector<string>& Files){
    //open the files
    ifstream in;
    if(!openFile(in,dbfile)){
        error("Cannot open file named "+dbfile);
    }

    readEntireFile(in,Files);
}


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
/**
 * @brief buildIndex reads the content from the databases file and processes it into the form of an inverted index;
 * @param dbfile
 * @param index
 * @return
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    Vector<string>Lines;
    ReadDbFile(dbfile,Lines);
    int n = Lines.size();
    Vector<string>L;//Used to store the number of URL;
    for(int i=0;i<n;i+=2)
        L.add(Lines[i]);
    for(int i=1;i<n;i+=2){
        Set<string>temp;
        temp = gatherTokens(Lines[i]);
        for(auto it:temp){
            if(!index.containsKey(it)){
                Set<string>tmp;
                tmp.add(Lines[i-1]);
                index.put(it,tmp);
            }else{
                index[it].add(Lines[i-1]);
            }
        }
    }
    return n/2;
}

/**
 * @brief returnflag
 * @param s
 * @return flag = { 0 , 1 , 2 }->{ null , + , - };
 */
int returnflag(string s){
    char temp = s[0];
    if(temp=='+')
        return 1;
    else if(temp=='-')
        return 2;
    return 0;
}


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
/**
 * @brief findQueryMatches
 * @param index
 * @param query
 * @return
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // TODO: your code here
    Vector<string>Processed;
    Processed = stringSplit(query," ");
    Vector<Set<string>>Group;
    vector<int>a;
    for(int i=0;i<Processed.size();i++){
        a.push_back(returnflag(Processed[i]));
    }
    //Now Vector a has recored the situation of the Group;
    map<int,int>mymap;
    int start = 0;
    int ptr=1;
    while(start+ptr<a.size()){
        while(a[start+ptr]!=0){
            ptr++;
        }
        mymap.insert(make_pair(start,start+ptr-1));
        start+=ptr;
        ptr=1;
    }
    if(start<a.size())
        mymap.insert(make_pair(start,start+ptr-1));//put the last pair into the mymap;
    Set<string> set0;
    Vector<Set<string>>VS;
    for(auto temp:mymap){
        Set<string>tmp = index.get(cleanToken(Processed[temp.first]));
        VS.add(tmp);
        for(int i=temp.first+1;i<=temp.second;i++){
            tmp = index.get(cleanToken(Processed[i]));
            VS.add(tmp);
        }
        for(int i=0;i<VS.size();i++){
            if(a[i]==0){
                set0+=VS[i];
            }else if(a[i]==1){
                set0*=VS[i];
            }else if(a[i]==2){
                set0-=VS[i];
            }
        }
        Group.add(set0);
        set0.clear();
        VS.clear();
    }
    for(int i=0;i<Group.size();i++){
        result.unionWith(Group[i]);
    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    // TODO: your code here
    Map<string, Set<string>> index;
    int n = buildIndex(dbfile,index);
    cout << "Stand by while building index..."<<endl;
    cout << "Indexed "<<n<<" pages contained "<<index.size()<<" unique terms";
    string input;
    input =getLine("Enter query sentence (RETURN/ENTER to quit): ");
    while(input!=""){
        Set<string>temp;
        temp = findQueryMatches(index,input);
        cout<<"Found "<<temp.size()<<" matching pages"<<endl;
        int flag=0;
        for(auto s:temp){
            if(!flag){
                cout << "{\""<<s<<"\"";
                    flag=1;
            }else{
                cout<<","<<endl;
                cout<<"\""<<s<<"\"";
            }
        }
        cout<<"}"<<endl;
        cout<<endl;
        input = getLine("Enter query sentence (RETURN/ENTER to quit): ");
    }
    cout<<"main() completed."<<endl;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
    EXPECT_EQUAL(index.get("red").size(),2);
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());

}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
