#include "Board.h"
#include <conio.h>
#include <Windows.h>


void Board::printFrame() const
{
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(color, 7);

	for (int i = 1;i <= COLS; i++) // print frame top & bottom 
	{
		gotoxy(gameFrame.left_f + i, gameFrame.top_f);
		std::cout << "*" << std::endl;
		gotoxy(gameFrame.left_f + i, gameFrame.bottom_f);
		std::cout << "*" << std::endl;
	}

	for (int i = 0;i <= ROWS+1; i++) // print frame left & right
	{
		gotoxy(gameFrame.left_f, gameFrame.top_f + i);
		std::cout << "|" << std::endl;
		gotoxy(gameFrame.right_f, gameFrame.top_f + i);
		std::cout << "|" << std::endl;
	}


}

void Board::setBoard(bool pause)
{
	printFrame();
	topB->printTopBoard();

	if (pause) {
		printShapes();
		return;
	}
	for (int i = 0; i < ROWS - 2; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (i < 2) {
				boardGame[i][j].setPoint(j + gameZone.left, i + gameZone.top, false, -1, ' ');
				turnOffPoint(j + gameZone.left, i + gameZone.top);
			}
			else {
				boardGame[i][j].setPoint(j + gameZone.left, i + gameZone.top, false, -1, ' ');
				turnOffPoint(j + gameZone.left, i + gameZone.top);
			}
		}
	}
}

void Board::copygrid(const Board& b)
{
	for (int i = 0; i < ROWS - 2; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (b.isPointInBoardGameBusy(i, j))
				boardGame[i][j].setPoint(j + gameZone.left, i + gameZone.top, true , b.getSerial(i, j), b.getSign(i, j));
			else
				boardGame[i][j].setPoint(j + gameZone.left, i + gameZone.top, false, -1, ' ');
		}
	}
	//printShapes();
}

void Board::cleanGameOver()
{
	gotoxy(gameFrame.left_f + 2, gameFrame.bottom_f + 1);
	std::cout << "              " << std::endl;
	gotoxy(gameFrame.left_f - 2, gameFrame.bottom_f + 3);
	std::cout << "                                " << std::endl;
	gotoxy(gameFrame.left_f - 2, gameFrame.bottom_f + 5);
	std::cout << "                                " << std::endl;
	topB->resetTopBoard();
	

}

int Board::getScore() const {
return topB->getScore();	
}

//check if the line is full
bool Board::isFullLine(int curLine, bool cleanIfFound)
{

	//check FULL line
	for (const Point&p : boardGame[curLine - gameZone.top + 3])
	{
		if (!(p.isBusy()))
			return false;
	}
    
    if (cleanIfFound) {
        cleanLine(curLine);
    }
    
	return true;
}

void Board::cleanLine(int curLine)
{
    // marker the line before crush
    for (Point&p : boardGame[curLine - gameZone.top + 3])
    {
        turnOnPoint(p.getx(), p.gety(),-3,'x');
        Sleep(20);
    }

    // turn OFF line
    for (Point&p : boardGame[curLine - gameZone.top + 3])
    {
        turnOffPoint(p.getx(), p.gety());
    }
    
}

bool Board::isEmptyLine(int curLine) const
{
	for (const Point&p : boardGame[curLine - gameZone.top + 3])
	{
		if ((p.isBusy()))
			return false;
	}
	return true;
}

// clean all the full lines
bool Board::cleanLines(int startLine)
{
	int inARow = 0;
	bool res = false;
	for (int i = 0; i < 6 && (startLine - i) >= 5; i++)
	{
		bool fullLine = isFullLine(startLine - i);

		if ( fullLine == true)
		{
			res = true;
			inARow++;
			Sleep(200);
		}

		else if (inARow != 0)
		{
			if (inARow == 1)
				topB->updateScore(SCORE::SINGLE_LINE);
			else if (inARow == 2)
				topB->updateScore(SCORE::DOUBLE_LINE);
			else
				topB->updateScore(SCORE::TRIPLE_LINE);

			inARow = 0;
		}
				
	}
	return res;
}

//BOMB explode
int Board::blowUpSquare(int x, int y)
{
	int scoreCounter = 0;
	for (int i = -4; i <= 4; i++) // X
	{
		for (int j = -4; j <= 4; j++) // Y
		{
			HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(color, 7);
			// count score
			if (checkInGameZone(x + i, y + j) && (!(*this).isValid(x + i, y + j)))
			{
				turnOnPoint(x + i, y + j, -3, '*');
				scoreCounter -= 50;
				(*this).turnOffPoint(x + i, y + j);
		Sleep(150);
	
			}
		}
	}

	return scoreCounter; // return the score to be reduced
	
}

bool Board::updateBoard()
{
	Point temp[4];
	int tempSize;
	int row = gameZone.bottom - 1;
	bool endClean = true;
	while (row != gameZone.top )
	{
		for (Point&p : boardGame[row - gameZone.top + 3])
		{
			if (p.isBusy() == true && isValid(p.getx(), p.gety() + 1))
			{
				createSerialShape(p.getSerialNumber(), row, tempSize, temp);
				moveShape(temp, tempSize);
			}
		}
		row--;
	}

	return endClean;
}

void Board::printShapes()
{
	int row = gameZone.bottom;
	while (row != gameZone.top)
	{
		for (Point& p : boardGame[row - gameZone.top + 3])
		{
			if (p.isBusy() == true)
			{
				p.draw('#', colored);
			}
		}
		row--;
	}
}

void Board::updateScoreBoard(int addScore)
{
	topB->updateScore(addScore);
	topB->printTopBoard();
}

void Board::updateNumOfShapesBoard()
{
	topB->updateNumOfShapes();
	topB->printTopBoard();
}

Point * Board::createSerialShape(int serial, int row, int & shapeSize, Point * res)
{
	shapeSize = 0;
	for (int i = 0; i<4; i++)
	{
		for (Point&p : boardGame[row - i - gameZone.top + 3])
		{
			if (serial == p.getSerialNumber())
			{
				res[shapeSize].setPoint(p.getx(),p.gety(),true,p.getSerialNumber(),p.getSign());
				shapeSize++;
			}
		}
	}
	return res;

}

void Board::moveShape(Point * arr, int size)
{
	bool flag = false;
	bool flag1 = true;
	for (int i = 0;i < size; i++)
	{
		if (isValid(arr[i].getx(), arr[i].gety() + 1) && arr[i].gety()+1 <= gameZone.bottom)
			flag = true;
		else
		{
			flag1 = false;
			for (int j = 0; j < size; j++)
			{
				if (j != i)
				{
					if ((arr[i].getx() == arr[j].getx()) && ((arr[i].gety() + 1) == arr[j].gety()))
						flag1 = true;
				}
	
			}

			if (flag1 == false)
				break;
		}
	}

	if (flag==true && flag1==true)
	{
		hardDownShape(arr, size);
		for (int i = 0; i < size; i++)
			arr[i].setPoint(arr[i].getx(), arr[i].gety() + 1, true, arr[i].getSerialNumber(), arr[i].getSign());

		moveShape(arr, size);
	}
}

void Board::hardDownShape(Point * arr , int size)
{
	for (int i = 0; i < size; i++)
	{
		turnOffPoint(arr[i].getx(), arr[i].gety());
		turnOnPoint(arr[i].getx(), arr[i].gety() + 1, arr[i].getSerialNumber(), arr[i].getSign());
	}
}

int Board::aggregateHeight()
{
    int total = 0;
    for(int c = 0; c < COLS; c++)
    {
        total += columnHeight(c);
    }
    return total;
}

int Board::columnHeight(int column)
{
    int r = 0;
    for(; r < ROWS && !boardGame[r][column].isBusy(); r++);
    return ROWS - r;
}

int Board::lines()
{
    int count = 0;
    for(int r = 0; r < ROWS; r++){
        if (isFullLine(r)){
            count++;
        }
    }
    return count;
}

int Board::holes()
{
    int count = 0;
    for(int c = 0; c < COLS; c++){
        bool block = false;
        for(int r = 0; r < ROWS; r++){
            if (boardGame[r][c].isBusy()) {
                block = true;
            }
            else if (!boardGame[r][c].isBusy() && block){
                count++;
            }
        }
    }
    return count;
}

int Board::bumpiness()
{
    int total = 0;
    for(int c = 0; c < COLS - 1; c++){
        total += std::abs(columnHeight(c) - columnHeight(c+ 1));
    }
    return total;
}
