// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* TODO: Replace this comment with a descriptive function
 * header comment.
 * Given a maze represented as a Grid of bool and a current cell represented by the GridLocation cur
 * return all valid moves from cur as a set.Valid moves are defined as follows:
 * 1. Only one step(neighbor)
 * 2. Direction: N S E W
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    /* TODO: Fill in the remainder of this function. */
    int row = cur.row;
    int col = cur.col;

    //Check for North;
    if(maze.inBounds(row-1,col)&&maze.get(row-1,col)){
        GridLocation temp = {row-1,col};
        neighbors.add(temp);
    }

    //Check for South;
    if(maze.inBounds(row+1,col)&&maze.get(row+1,col)){
        GridLocation temp = {row+1,col};
        neighbors.add(temp);
    }

    //Check for East;
    if(maze.inBounds(row,col+1)&&maze.get(row,col+1)){
        GridLocation temp = {row,col+1};
        neighbors.add(temp);
    }

    //Check for West;
    if(maze.inBounds(row,col-1)&&maze.get(row,col-1)){
        GridLocation temp = {row,col-1};
        neighbors.add(temp);
    }

    return neighbors;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit"); //Check the end of the path;
    }
    /* TODO: Fill in the remainder of this function. */

    Grid<bool>visited=maze; //Remember to initialize!
    visited.fill(false);//it indicates that the grid has not visited;
    visited.set(maze.numRows()-1, maze.numCols()-1,true);

    path.pop();
    GridLocation temp1 = mazeExit;
    GridLocation start = {0,0};
    while(!path.isEmpty()){
        GridLocation temp2 = path.peek();
        path.pop();
        auto maze_set = generateValidMoves(maze,temp1);
        auto it = find(maze_set.begin(),maze_set.end(),temp2);
        if(it == maze_set.end()){
            error("Here is my message about what has gone wrong");
            return ;
        }
        if(visited.get(temp2)==true){
            error("The grid has been visited!");
            return ;
        }
        //Here it indicates that temp2 belongs to the set;
        visited.set(temp2.row,temp2.col,true);
        temp1 = temp2;
        if(path.isEmpty()&&temp1!=start){
            error("Path does not start at the entry");
            return ;
        }

    }
    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without throwing any errors.
     */
    return ;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    /* TODO: Fill in the remainder of this function. */
    //We define a queue to store the path;
    //BFS Algorithm can do Good Job!
    Queue<Stack<GridLocation>>q;
    //Before Do into the BFS,we need to do some ready job for it;
    GridLocation Start = {0,0};
    GridLocation Exit = {maze.numRows()-1,maze.numCols()-1};
    Grid<bool> visited=maze;
    visited.fill(false);
    path.push(Start);
    visited.set(Start,true);
    q.enqueue(path);
    while(!q.isEmpty()){
        Stack<GridLocation>temp = q.dequeue();
        GridLocation end = temp.peek();
        if(end==Exit){
            path = temp;
            break;
        }//If find the path,we will break the while-loop;
        auto set = generateValidMoves(maze,end);
        for(auto it:set){
            if(visited.get(it)==false){
                temp.push(it);
                q.enqueue(temp);
                visited.set(it,true);
                temp.pop();
            }
        }
    }
    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);
    MazeGraphics::drawGrid(maze);
    MazeGraphics::highlightPath(soln,"red");
    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);
    MazeGraphics::drawGrid(maze);
    MazeGraphics::highlightPath(soln,"red");
    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
