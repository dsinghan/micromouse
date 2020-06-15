#include <iostream>
#include <stack>
#include <set>
#include <utility>
#include <cstdlib>  // atoi

#include "Maze.h"
#include "MazeDefinitions.h"
#include "PathFinder.h"
using namespace std;

#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3


class Solution : public PathFinder {
public:
    Solution(bool shouldPause = false) : pause(shouldPause) {
        curDirection = UP;
        nextDirection = UP;
        shouldGoForward = false;
        visitedStart = false;
        // s.push(pair <int, int> (0, 0));
        visited.insert(pair <int, int> (0, 0));
        path_history.push(pair <int, int> (0, 0));

        curPosition.first = 0;
        curPosition.second = 0;
        nextPosition.first = -1;
        nextPosition.second = -1;
    }

    /*
    If we have a next cell to go to
        Attempt to go to that cell
                If we don't have a next cell to go to
                    Serach the cells around current cell
                    Pick the next cell from stack

    */

    MouseMovement nextMovement(unsigned x, unsigned y, const Maze &maze) {
        // int temp;
        std::cin.ignore(10000, '\n');
        std::cin.clear();
        std::cout << maze.draw(5) << std::endl << std::endl;
        const bool frontWall = maze.wallInFront();
        const bool leftWall  = maze.wallOnLeft();
        const bool rightWall = maze.wallOnRight();


        if (!frontWall) {
            if (curDirection == UP) {
                newCell(curPosition.first, curPosition.second + 1, curPosition.first, curPosition.second);
            } else if (curDirection == LEFT) {
                newCell(curPosition.first - 1, curPosition.second, curPosition.first, curPosition.second);
            } else if (curDirection == RIGHT) {
                newCell(curPosition.first + 1, curPosition.second, curPosition.first, curPosition.second);
            } else if (curDirection == DOWN) {
                newCell(curPosition.first, curPosition.second - 1, curPosition.first, curPosition.second);
            }
        }

        if (!leftWall) {
            if (curDirection == UP) {
                newCell(curPosition.first - 1 , curPosition.second, curPosition.first, curPosition.second);
            } else if (curDirection == LEFT) {
                newCell(curPosition.first, curPosition.second - 1, curPosition.first, curPosition.second);
            } else if (curDirection == RIGHT) {
                newCell(curPosition.first, curPosition.second + 1, curPosition.first, curPosition.second);
            } else if (curDirection == DOWN) {
                newCell(curPosition.first + 1, curPosition.second, curPosition.first, curPosition.second);
            }
        }

        if (!rightWall) {
            if (curDirection == UP) {
                newCell(curPosition.first + 1, curPosition.second, curPosition.first, curPosition.second);
            } else if (curDirection == LEFT) {
                newCell(curPosition.first, curPosition.second + 1, curPosition.first, curPosition.second);
            } else if (curDirection == RIGHT) {
                newCell(curPosition.first, curPosition.second - 1, curPosition.first, curPosition.second);
            } else if (curDirection == DOWN) {
                newCell(curPosition.first - 1, curPosition.second, curPosition.first, curPosition.second);
            }
        }

        // cout << "SIZE: " << s.size()  << endl;
        if (nextPosition.first == -1 && nextPosition.second == -1) {
            nextPosition = s.top();
            cout << nextPosition.first << " " << nextPosition.second << endl;
            s.pop();
        }

        if (isAtCenter(curPosition.first, curPosition.second)) {
            return Finish;
        }

        // if (curPosition.first == 0 && curPosition.second == 0) {
        //     cout << "Got back to original" << endl;
        //     return Finish;
        // }

        if (nextPosition.first == curPosition.first && nextPosition.second == curPosition.second) {
            // Got to goal
            nextPosition = s.top();
            s.pop();
            neededPosition = path_history.top();
            path_history.pop();
        }

        // cout << "CURRENT: (" << curPosition.first << ", " << curPosition.second << ") " << curDirection << endl;
        // cout << "NEXT: (" << nextPosition.first << ", " << nextPosition.second << ") " << nextDirection << endl;
        // cout << "NEEDED: (" << neededPosition.first << ", " << neededPosition.second << ") " << nextDirection << endl;


        if (curPosition.first != neededPosition.first || curPosition.second != neededPosition.second) {
            MouseMovement top = history.top();
            history.pop();

            if (top == TurnCounterClockwise) {
                curDirection -= 1;
                if (curDirection == -1) {
                    curDirection = 3;
                }
                return TurnClockwise;
            } else {
                if (curDirection == UP) {
                    curPosition.second -= 1;
                } else if (curDirection == LEFT) {
                    curPosition.first += 1;
                } else if (curDirection == RIGHT) {
                    curPosition.first -= 1;
                } else {
                    curPosition.second += 1;
                }
                return MoveBackward;
            }
        }

        if (nextPosition.first > curPosition.first) {
            nextDirection = RIGHT;
        } else if (nextPosition.first < curPosition.first) {
            // Go to left
            nextDirection = LEFT;
        } else if (nextPosition.second > curPosition.second) {
            // Go to up
            nextDirection = UP;
        } else if (nextPosition.second < curPosition.second) {
            // Go to down
            nextDirection = DOWN;
        } 

        if (nextDirection != curDirection) {
            curDirection += 1;
            if (curDirection == 4) {
                curDirection = 0;
            }
            // cout << "TURN" << endl;
            history.push(TurnCounterClockwise);
            return TurnCounterClockwise;
        } else { // Same Direction
            // cout << "MOVE" << endl;
            if (curDirection == UP) {
                curPosition.second += 1;
            } else if (curDirection == LEFT) {
                curPosition.first -= 1;
            } else if (curDirection == RIGHT) {
                curPosition.first += 1;
            } else {
                curPosition.second -= 1;
            }
            history.push(MoveForward);
            return MoveForward;
        }
    }

protected:
    stack <MouseMovement> history;
    stack <pair <int, int> > path_history;
    stack <pair <int, int> > s;
    set <pair <int, int> > visited;
    int curDirection;
    int nextDirection;
    pair <int, int> curPosition;
    pair <int, int> nextPosition;
    pair <int, int> neededPosition;

    // Helps us determine that we should go forward if we have just turned left.
    bool shouldGoForward;

    // Helps us determine if we've made a loop around the maze without finding the center.
    bool visitedStart;

    // Indicates we should pause before moving to next cell.
    // Useful for command line usage.
    const bool pause;

    bool isAtCenter(unsigned x, unsigned y) const {
        unsigned midpoint = MazeDefinitions::MAZE_LEN / 2;

        if(MazeDefinitions::MAZE_LEN % 2 != 0) {
            return x == midpoint && y == midpoint;
        }

        return  (x == midpoint     && y == midpoint    ) ||
        (x == midpoint - 1 && y == midpoint    ) ||
        (x == midpoint     && y == midpoint - 1) ||
        (x == midpoint - 1 && y == midpoint - 1);
    }

    void newCell(int x, int y, int cur_x, int cur_y) {
        if (visited.find(pair <int, int> (x, y)) == visited.end()) {
            // cout << "adding (" << x << ", " << y << ")" << endl;
            s.push(pair <int, int> (x, y));
            visited.insert(pair <int, int> (x, y));
            path_history.push(pair <int, int> (cur_x, cur_y));
        }
    }
};



int main(int argc, char * argv[]) {
    MazeDefinitions::MazeEncodingName mazeName = MazeDefinitions::MAZE_CAMM_2012;
    bool pause = false;

    // Since Windows does not support getopt directly, we will
    // have to parse the command line arguments ourselves.

    // Skip the program name, start with argument index 1
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-m") == 0 && i+1 < argc) {
            int mazeOption = atoi(argv[++i]);
            if(mazeOption < MazeDefinitions::MAZE_NAME_MAX && mazeOption > 0) {
                    mazeName = (MazeDefinitions::MazeEncodingName)mazeOption;
            }
        } else if(strcmp(argv[i], "-p") == 0) {
            pause = true;
        } else {
            std::cout << "Usage: " << argv[0] << " [-m N] [-p]" << std::endl;
            std::cout << "\t-m N will load the maze corresponding to N, or 0 if invalid N or missing option" << std::endl;
            std::cout << "\t-p will wait for a newline in between cell traversals" << std::endl;
            return -1;
        }
    }

    Solution solution(pause);
    Maze maze(mazeName, &solution);
    std::cout << maze.draw(5) << std::endl << std::endl;

    maze.start();
}
