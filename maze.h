/* File: Maze.h
    This file contains the specification for a Maze class.
    The Maze is stored as a two-dimensional character array
    in which each character has the following meaning:

        # - wall
        o - starting position
        * - goal position
        . - open space

    Initially, all positions in the maze are unexplored and
    will be displayed as a question mark (?) - i.e. the fog
    of war. The position of start is available with public
    member functions. Non-start positions can only be explored
    after a vertically or horizontally adjacent position has
    already been explored.

    This maze is designed to be solved interactively by a user
    or automatically by an automated method (AI).
*/


#ifndef MAZE_H
#define MAZE_H

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

class Maze
{
    private:
        char **mazeArray; //our maze representation
        bool **visited; //whether or not the corresponding
                        //position in mazeArray has been explored
        int startc;     //The column of the starting positions
        int startr;     //The row of the starting positions
        int finishc;    //The column of the goal
        int finishr;    //The row of the goal
        int width;      //The number of columns in the map
        int height;     //The number of rows in the map

        void display(bool);

    public:
        Maze(string);   //constructor
        ~Maze();        //destructor
        void display(); //display the maze - unexplored represented by ?
        void displayNoFog(); //display the full maze without ? - use fore debugging
        char explore(int,int); //for exploring a particular positions

        //accessor member functions
        int getWidth(){return width;}
        int getHeight(){return height;}
        int getStartRow(){return startr;}
        int getStartCol(){return startc;}
        bool haveVisited(int,int);
};

/* Constructor
    Parameter: the filename containing the maze to be read in as
    a 2D array. The first line of a maze file contains the number
    of columns and rows respectively. All following lines gives the
    actual 2@ character array, with each line representing a row.

    Nothing is returned. There is console output only when the
    file fails to open.

    This is the only way to create a Maze - there are no other
    constructors, and no other member functions for setting up
    the maze array.
*/
Maze::Maze(string filename)
{
    ifstream in;
    in.open(filename.c_str());

    if(!in.fail())
    {
        //get the width and the height
        in >> width;
        in >> height;

        //allocate enough memory for the 2D array
        //once we do this, the size never changes
        mazeArray = new char*[height];
        visited = new bool*[height];
        //we need to allocate each row separately
        for (int i = 0 ; i < width ; i++)
        {
            mazeArray[i] = new char[width];
            visited[i] = new bool[width];
        }


        in.get(); //eating a newline

        //nested for loop for stepping through
        //each position in the array
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                mazeArray[i][j] = (char)in.get(); //gets one character
                visited[i][j] = false; //initially all spots are unexplored

                //check if this is the origin or the goal and save
                //the info to our member variables
                if( mazeArray[i][j] == 'o' )
                {
                    startc = j;
                    startr = i;
                }
                else if(mazeArray[i][j] == '*')
                {
                    finishc = j;
                    finishr = i;
                }

            }
            //eat the newline character at the end of the row
            in.get();
        }
    }
    else  //the file failed to open
    {
        cerr << "couldn't open maze file" << endl;
        exit(1);
    }
    in.close();

}

/* Destructor
    The mazeArray and visited 2D arrays were allocated dynamically,
    so this simply goes row by row, deleting each array

    No parameters or return. Nothing input or output.
*/
Maze::~Maze()
{
    for (int i = 0 ; i < height ; i++)
    {
        delete [] mazeArray[i];
        delete [] visited[i];
    }

}

/* Takes a boolean flag fog as a parameter. If fog is true,
    this function displays the maze map to the console with
    unexplored spaces represented by question marks (?).
    Otherwise, the full map is displayed without question marks.

    Nothing returned. No user input.

    This is a private, auxilliary function meant to be called
    internally for the two different versions of display.
*/
void Maze::display(bool fog)
{
    cout << endl; //putting in some whitespace
    //traverse the 2D array and print each value
    int i, j;
    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            //checks if the ? is necessary
            if(fog == false || visited[i][j])
            {
                cout << mazeArray[i][j];
            }
            else
            {
                cout << "?";
            }
        }
        cout << endl;

    }
    cout << endl;
}

/* No parameters or returns. No user input. This function outputs
    the maze map with unexplored spaces represented by '?'. Simply
    calls the auxilliary function for display with the "fog" flag true.
*/
void Maze::display()
{
    display(true);
}

/* No parameters or returns. No user input. This function outputs
    the maze map - both unexplored and explored spaces. Simply
    calls the auxilliary function for display with the "fog" flag false.

    This is primarily intended to be used with debugging.
*/
void Maze::displayNoFog()
{
    display(false);
}


/*
    Parameters: the row and column of the maze to explore
    Return: the character stored in the array at the position
        indicated by the parameters. This tells the caller what
        kind of spot they've found:

        # - wall
        o - starting position
        * - goal position
        . - open space

        There are also two "error" cases which output an error message
        and give a special sentinal return value.

        X - the move is out-of-bounds - i.e. the parameter coordinates
            are not on the map
        ? - the given position cannot be explored because it is not the
            origin and not adjacent to an already explored position.

    User output: only in the error cases above
    User input: none

    After this function is called in a non-error case on the given position
    it will be considered to have been explored, and so the haveVisited array
    will indicate this.
*/
char Maze::explore(int row, int col)
{
    //checks if the parameters are within the map limites
    if( row >= height || col >= width )
    {
        cout << "That move is out of bounds" << endl;
        return 'X'; //an error return value
    }

    //we can successfully explore this position if (1) we've already visited it,
    // (2) it is vertically or horizontally adjacent to an already visited positions
    // or (3) if it is the start position.
    //There are several things to check for (2) - i.e. whether or not we're on the edge of the map and the
    //adjacent position exists and whether or not the adjacent explored space is not a wall (i.e. origin or open space)
    if( visited[row][col] //(1)
       || (row != 0 && visited[row-1][col] && (mazeArray[row-1][col] == 'o' || mazeArray[row-1][col] == '.' ) ) //(2)
       || (row+1 != height && visited[row+1][col] && (mazeArray[row+1][col] == 'o' || mazeArray[row+1][col] == '.' ) ) //(2)
       || (col != 0 && visited[row][col-1] && (mazeArray[row][col-1] == 'o' || mazeArray[row][col-1] == '.' ) ) //(2)
       || (col+1 != width && visited[row][col+1] && (mazeArray[row][col+1] == 'o' || mazeArray[row][col+1] == '.' )  ) //(2)
       || mazeArray[row][col] == 'o' )
    {
        visited[row][col] = true;   //we've now visited this space
        return mazeArray[row][col]; //return what's in this position
    }
    else
    {
        cout << "Cannot explore row " << row << ", column " << col << " because you have not yet reached an adjacent square.";
        return '?';
    }

}

/*
    Parameters: a row and column of the maze
    Return: whether or not the position indicated by the parameters has
        yet been explored in this maze
    User output: none
    User input: none
*/
bool Maze::haveVisited(int row, int col)
{
    return visited[row][col];
}

#endif // MAZE_H
