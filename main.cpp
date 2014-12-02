/**
 * author: Henry West
 * date: 4/26/2013
 * description: A maze solver/player. Utilizes queueing and stacks as well as brain power to
 * solve some mazes. Somewhat relatable to rocket science.
 * proposed points: 44/50
 *      -Did not use recursive solver
 *      -Did not implement virtual function
 */

#include <iostream>
#include "maze.h"
#include <queue>
#include <stack>

using namespace std;


void humanPlayer(string filename);


/*
    Parameter: a filename containing the maze to use with the one-player game
    Return: none
    User output: the maze at various points in the user's search
    User input: the position in the maze to explore
*/
void humanPlayer(string filename)
{
    //create the maze, display the maze and the starting position
    Maze m(filename);
    m.display();
    cout << "Starting in " << m.getStartRow() << ", " << m.getStartCol() << endl;
    cout << "";
    bool finished = false;

    //the game loop
    while(!finished)
    {
        cout << "Enter the row and column you want to explore: " << endl;
        int r, c;
        cin >> r >> c; //read input
        char result = m.explore(r,c); //explores the given spot in the maze

        cout << "You found a " << result << " in " << r << ", " << c << endl;

        m.display(); //display our current view of the maze
        //check if the goal has been found
        if(result == '*')
        {
            cout << "You found the finish!" << endl;
            finished = true;
        }
    }

    //we're done, so we can display the whole maze - even unexplored territory
    m.displayNoFog();
}
void intelligent_agent(string filename) {
    Maze m(filename);
    m.display();
    queue<int> maze_queue;
    cout << "Starting in " << m.getStartRow() << ", " << m.getStartCol() << endl;
    cout << "";
    bool finished = false;

    //Shall we queue? We shall queue
    maze_queue.push(m.getStartRow());
    maze_queue.push(m.getStartCol());

    int r, c;
    char result;

    while (!finished) {
        //moves along the queue, old chap
        r = maze_queue.front();
        maze_queue.pop();
        c = maze_queue.front();
        maze_queue.pop();

        result = m.explore(r, c);
        m.display();
        if(result == '*') {
            cout << "I found the finish!" << endl;
            finished = true;
        }
        //adds new items to the queue if current space is 'legal'
        else if (result == '.' || result == 'o') {
            //down a row
            if (!m.haveVisited(r + 1, c) && r + 1 < m.getHeight() - 1 && r + 1 > 0) {
                maze_queue.push(r + 1);
                maze_queue.push(c);
            }
            //up a row
            if (!m.haveVisited(r - 1, c) && r - 1 > 0 && r - 1 < m.getHeight()) {
                maze_queue.push(r - 1);
                maze_queue.push(c);
            }
            //right a column
            if (!m.haveVisited(r, c + 1) && c + 1 < m.getWidth() - 1 && c + 1 > 0) {
                maze_queue.push(r);
                maze_queue.push(c + 1);
            }
            //left a column
            if (!m.haveVisited(r, c - 1) && c - 1 > 0 && c - 1 < m.getWidth()) {
                maze_queue.push(r);
                maze_queue.push(c - 1);
            }
        }

    }
    m.displayNoFog();
    exit(1);
}
void stack_agent(string filename) {
    Maze m(filename);

    m.display();

    stack<int> maze_stack;
    cout << "Starting in " << m.getStartRow() << ", " << m.getStartCol() << endl;
    cout << "";

    bool finished = false;
    //pushes start on the stack
    maze_stack.push(m.getStartCol());
    maze_stack.push(m.getStartRow());

    int r, c;
    char result;

    while (!finished) {
        //pops off the stack
        r = maze_stack.top();
        maze_stack.pop();
        c = maze_stack.top();
        maze_stack.pop();

        result = m.explore(r, c);

        m.display();
        if(result == '*') {
            cout << "I found the finish!" << endl;
            finished = true;
        }
        else if (result == '.' || result == 'o') {
            //down a row
            if (!m.haveVisited(r + 1, c) && r + 1 < m.getHeight() - 1 && r + 1 > 0) {
                maze_stack.push(c);
                maze_stack.push(r + 1);
            }
            //up a row
            if (!m.haveVisited(r - 1, c) && r - 1 > 0 && r - 1 < m.getHeight()) {
                maze_stack.push(c);
                maze_stack.push(r - 1);
            }
            //right a column
            if (!m.haveVisited(r, c + 1) && c + 1 < m.getWidth() - 1 && c + 1 > 0) {
                maze_stack.push(c + 1);
                maze_stack.push(r);
            }
            //left a column
            if (!m.haveVisited(r, c - 1) && c - 1 > 0 && c - 1 < m.getWidth()) {
                maze_stack.push(c - 1);
                maze_stack.push(r);
            }
        }

    }
    m.displayNoFog();
    //was getting strange erors w/out this
    exit(1);
}

/* Entry point of the program. Simply gets the filename from the user and then
    launches the one-player game. When you write your AIs for this, you should
    set up a menu in here to allow the user to say which AI (or human game) they'd
    like to run.
*/

int main()
{
    string mazeFileName;
    cout << "Enter a maze file name: ";
    cin >> mazeFileName;
    char response[5];
    do {
        cout << "Human, Queue, or Stack? [h/q/s] " << endl;
        cin >> response;
    } while (response[0] != 'h' && response[0] != 'q' && response[0] != 's');
    if (response [0] == 'h') {
        humanPlayer(mazeFileName);
    }
    else if (response[0] == 'q') {
        intelligent_agent(mazeFileName);
    }
    else if (response[0] == 's') {
        stack_agent(mazeFileName);
    }

    return 0;
}
