#include <iostream>
#include <fstream>
using namespace std;

char SudokuBoard[9][9];
char SudokuSolution[9][9];

void PrintBoard(char Board[9][9])
{
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
            cout << Board[x][y];
        cout << "\n";
    }
    cout << endl;
}

bool LoadInput()
{
    ifstream input("input.txt");
    
    if(!input.is_open())
    {
        cout << "file error\n";
        return 0;   
    }

    for(int y = 0; y < 9; y++)
        for(int x = 0; x < 9; x++)
            input >>  SudokuBoard[x][y];
        
    cout << "Your board:\n";

    PrintBoard(SudokuBoard);

    input.close();
    return 1;
}

/**
 * Board is numerated from x = 0, y = 0
 * to x = 8, y = 8
 * changeField(x, y, +) goes to next field
 * changeField(x, y, -) goes to previous field
 */
void changeField(int &x, int &y, char z)
{
    if(z == '+')
    {
        x += 1;
        y += x / 9;
        x %= 9;
    }
    else if(z == '-')
    {
        x-= 1;
        if(x < 0)
        {
            x = 8;
            y--;
        }
    }
}

bool SolveInput(int counter)
{
    //Array that checks if number is in ,,Object"
    //BoolNum[Number][Object][ObjectNumber]
    //Object: {Row, Column, Block}
    char BoolNum[10][3][9];
    for(int z = 0; z < 10; z++)
        for(int y = 0; y < 3; y++)
            for(int x = 0; x < 9; x++)
                BoolNum[z][y][x] = false;

    for(int y = 0; y < 9; y++)
        for(int x = 0; x < 9; x++)
        {
            int act = SudokuBoard[x][y] - '0';
            
            BoolNum[act][0][y] = true;
            BoolNum[act][1][x] = true;
            BoolNum[act][2][(x/3)+3*(y/3)] = true;

            SudokuSolution[x][y] = SudokuBoard[x][y];
        }


    //Backtracking until all solutions are checked
    //or solutions is found
    int x = 0, y = 0;
    bool isBack = false;

    while(true)
    {
        if(y < 0) return 0;
        if(y > 8)
        { 
            if(counter == 0)
                return 1;
            
            changeField(x, y, '-');
            counter--;
            isBack = true;
        }
        
        //Checks if field is given by user
        if(SudokuBoard[x][y] != '0')
        {
            if(isBack)changeField(x, y, '-');
            else changeField(x, y, '+');
            continue;
        }

        //Checks if current field can be increased
        int act = SudokuSolution[x][y] + 1 - '0';
        int prev = SudokuSolution[x][y] - '0';
        while(act < 10)
        {
            if( BoolNum[act][0][y] == true or
                BoolNum[act][1][x] == true or
                BoolNum[act][2][(x/3)+3*(y/3)] == true)  
                {
                    //can not be increased
                    act++; 
                    continue;
                }

            //can be increased
            BoolNum[act][0][y] = true;
            BoolNum[act][1][x] = true;
            BoolNum[act][2][(x/3)+3*(y/3)] = true;
            BoolNum[prev][0][y] = false;
            BoolNum[prev][1][x] = false;
            BoolNum[prev][2][(x/3)+3*(y/3)] = false;
            SudokuSolution[x][y] = act + '0';
            changeField(x, y, '+');
            isBack = false;
            break;
        }

        if(act > 9) 
        {   
            //field was not increased so is reseted
            BoolNum[prev][0][y] = false;
            BoolNum[prev][1][x] = false;
            BoolNum[prev][2][(x/3)+3*(y/3)] = false;
            SudokuSolution[x][y] = '0';
            changeField(x, y, '-');
            isBack = true;
        }
    }
    return 404;
}

bool Next()
{
    cout << "Do you want to find another solution? y/n ";
    char z; cin >> z;
    if(z == 'y') return 1;
    return 0;
}

int main()
{
    cout << "\n<--SimpleSudokuSolver-->\n\n";

    if(!LoadInput()) return 0;

    int counter = 0;
    do
    {
        if(!SolveInput(counter))
        {
            cout << "No solutions exist";
            return 0;
        }

        cout << "Solution:\n";
        PrintBoard(SudokuSolution);
        counter++;
    }while(Next());
    
    return 0;
}