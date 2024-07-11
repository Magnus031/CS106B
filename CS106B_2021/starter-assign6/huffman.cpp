#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * Your implementation may change the messageBits queue however you like. There
 * are no requirements about what it should look like after this function
 * returns. The encoding tree should be unchanged.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    /* TODO: Implement this function. */
    if(tree->isLeaf())
        return charToString(tree->getChar());
    string result="";
    //pre-order
    while(!messageBits.isEmpty()){
        EncodingTreeNode* root=tree;
        Bit temp = messageBits.peek();
        messageBits.dequeue();
        while(!root->isLeaf()){
            if(temp==true)
                root = root->one;
            else
                root = root->zero;
            if(root->isLeaf()){
                string ch = charToString(root->getChar());
                result+=ch;
                break;
            }
            temp = messageBits.peek();
            messageBits.dequeue();
        }
    }
    return result;
}
/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Your implementation may change the queue parameters however you like. There
 * are no requirements about what they should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    /* TODO: Implement this function. */
    EncodingTreeNode* root;
    if(treeShape.dequeue()==1){
        root = new EncodingTreeNode(nullptr,nullptr);
        root->zero = unflattenTree(treeShape,treeLeaves);
        root->one = unflattenTree(treeShape,treeLeaves);
    }else{
        root = new EncodingTreeNode(treeLeaves.dequeue());
    }
    return root;
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decompress(EncodedData& data) {
    /* TODO: Implement this function. */
    string result = "";
    EncodingTreeNode* root = unflattenTree(data.treeShape,data.treeLeaves);
    result = decodeText(root,data.messageBits);
    deallocateTree(root);
    return result;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    /* TODO: Implement this function. */
    if (text.size() < 2) {
        error("the text need at least 2 word");
    }
    Map<char, int> digit;
    for (auto c : text) {
        digit[c]++;
    }
    PriorityQueue<EncodingTreeNode*> pq;
    auto keys = digit.keys();
    for (auto &key : keys) {
        pq.enqueue(new EncodingTreeNode(key), digit[key]);
    }
    while (pq.size() >= 2) {
        double fp = pq.peekPriority();
        auto first = pq.dequeue();
        double sp = pq.peekPriority();
        auto second = pq.dequeue();
        EncodingTreeNode* newNode = new EncodingTreeNode(first, second);
        pq.enqueue(newNode, fp + sp);
    }
    auto result = pq.dequeue();
    return result;
}


void getHash(EncodingTreeNode* tree, Map<char, string> &mp,
             string s) {
    if (tree == nullptr) {
        return;
    }
    if (tree->isLeaf()) {
        mp[tree->getChar()] = s;
        return;
    }
    if (tree->one != nullptr) {
        getHash(tree->one, mp, s + '1');
    }
    if (tree->zero != nullptr) {
        getHash(tree->zero, mp, s + '0');
    }

}

Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    /* TODO: Implement this function. */
    Map<char, string> mp;
    Queue<Bit> res;
    string s;
    getHash(tree, mp, s);
    for (char c : text) {
        string& q = mp[c];
        for (auto c : q) {
            res.enqueue(c - '0');
        }
    }
    return res;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    /* TODO: Implement this function. */
    if (tree == nullptr) {
        return;
    }
    if (tree->isLeaf()) {
        treeShape.enqueue(0);
    } else {
        treeShape.enqueue(1);
    }
    flattenTree(tree->zero, treeShape, treeLeaves);
    if (tree->isLeaf()) {
        treeLeaves.enqueue(tree->getChar());
    }
    flattenTree(tree->one, treeShape, treeLeaves);
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText) {
    /* TODO: Implement this function. */
    EncodingTreeNode* huffman = buildHuffmanTree(messageText);
    Queue<Bit> messageBits = encodeText(huffman, messageText);
    Queue<Bit> treeShape;
    Queue<char> treeLeaves;
    flattenTree(huffman, treeShape, treeLeaves);
    deallocateTree(huffman);
    return {treeShape, treeLeaves, messageBits};
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    /* TODO: Implement this utility function needed for testing. */
    EncodingTreeNode* L1 = new EncodingTreeNode('T');
    EncodingTreeNode* RL1 = new EncodingTreeNode('R');
    EncodingTreeNode* RL2 = new EncodingTreeNode('S');
    EncodingTreeNode* RR = new EncodingTreeNode('E');
    EncodingTreeNode* RL = new EncodingTreeNode(RL1,RL2);
    EncodingTreeNode* R = new EncodingTreeNode(RL,RR);
    EncodingTreeNode* root = new EncodingTreeNode(L1,R);
    return root;
}

EncodingTreeNode* createExampleTree1() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             *     T
     *            / \
     *           *   E
     *          / \
     *         R   S
     */
    /* TODO: Implement this utility function needed for testing. */
    EncodingTreeNode* L1 = new EncodingTreeNode('T');
    EncodingTreeNode* RL1 = new EncodingTreeNode('R');
    EncodingTreeNode* RL2 = new EncodingTreeNode('S');
    EncodingTreeNode* RR = new EncodingTreeNode('E');
    EncodingTreeNode* RL = new EncodingTreeNode(RL1,RL2);
    EncodingTreeNode* R = new EncodingTreeNode(RR,RL);
    EncodingTreeNode* root = new EncodingTreeNode(R,L1);
    return root;
}


void deallocateTree(EncodingTreeNode* t) {
    /* TODO: Implement this utility function needed for testing. */
    //First confirm whether it is a leaf?
    if(t->isLeaf()){
        delete t;
        return;
    }
    //Then delete it subtree recursively;
    deallocateTree(t->zero);
    deallocateTree(t->one);
    //Finally delete it self;
    delete t;
    return;
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    /* TODO: Implement this utility function needed for testing. */
    if(a->isLeaf()&&b->isLeaf()&&a->getChar()==b->getChar())
        return true;
    else if(!a->isLeaf()&&!b->isLeaf())
        return areEqual(a->one,b->one)&&areEqual(a->zero,b->zero);
    return false;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */

STUDENT_TEST("Test-For deallocateTree"){
    EncodingTreeNode* root1 = createExampleTree();
    EncodingTreeNode* root2 = createExampleTree();
    EncodingTreeNode* root3 = new EncodingTreeNode('T');
    EncodingTreeNode* root4 = createExampleTree1();



    EXPECT(areEqual(root1,root2));
    EXPECT(!areEqual(root1,root3));
    EXPECT(!areEqual(root1,root4));

    deallocateTree(root1);
    deallocateTree(root2);
    deallocateTree(root3);
    deallocateTree(root4);
}





/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    /* TODO: Implement this utility function needed for testing. */
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E

    Queue<Bit> result = encodeText(reference, "E");

    EXPECT_EQUAL(result, messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
