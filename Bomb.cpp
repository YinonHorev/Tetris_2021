#include "Bomb.h"

void Bomb::draw(char ch) const
{
	body.draw('@');
}

bool Bomb::move(char keyPressed, bool mark)
{
	int addScore;

	switch (keyPressed)
	{
	case eKEYS::DEFAULT:
		if (boardGame.isValid(body.getx(), body.gety() + 1))
		{
			boardGame.turnOffPoint(body.getx(), body.gety(), mark);
			boardGame.turnOnPoint(body.getx(), body.gety() + 1, 0, Sign::B, mark);
			body.setPoint(body.getx(), body.gety() + 1, true);
		}
		else if (!(boardGame.checkInGameZone(body.getx(), body.gety() + 1)) && boardGame.gameZone.bottom >= body.gety() + 1)
			return true;
		else // explode 
		{
			return explode(0, 0, mark, addScore);
		}
		break;
	case eKEYS::LEFT:
		if (boardGame.isValid(body.getx() - 1, body.gety()))
		{
			boardGame.turnOffPoint(body.getx(), body.gety(), mark);
			boardGame.turnOnPoint(body.getx() - 1, body.gety(), 0, Sign::B, mark);
			body.setPoint(body.getx() - 1, body.gety(), true);
		}
		else if (!(boardGame.checkInGameZone(body.getx() - 1, body.gety())) && boardGame.gameZone.bottom >= body.gety())
			return true;
		else // explode 
		{
			return explode(-1, 0, mark, addScore);
		}
		break;
	case eKEYS::RIGHT:
		if (boardGame.isValid(body.getx() + 1, body.gety()))
		{
			boardGame.turnOffPoint(body.getx(), body.gety(), mark);
			boardGame.turnOnPoint(body.getx() + 1, body.gety(), 0, Sign::B, mark);
			body.setPoint(body.getx() + 1, body.gety(), true);
		}
		else if (!(boardGame.checkInGameZone(body.getx() + 1, body.gety())) && boardGame.gameZone.bottom >= body.gety())
			return true;
		else // explode 
		{
			return explode(0, 0, mark, addScore);
		}
		break;
	case eKEYS::HARD_DOWN:
		while (true)
		{
			if (!(move(DEFAULT, mark)))
				break;
			if (mark)
				boardGame.updateScoreBoard(2); // hard_drop x2 distance 
		}
		return false;
		break;
	default:
		return false;
		break;
	}

}

bool Bomb::explode(int x, int y, bool mark, int& addScore)
{
	boardGame.turnOffPoint(body.getx() + x, body.gety()+ y, mark);
	addScore = boardGame.blowUpSquare(body.getx(), body.gety(), mark);
	if (mark)
		boardGame.updateScoreBoard(addScore);
	boardGame.updateBoard(mark);
	if (boardGame.cleanLines(boardGame.gameZone.bottom, mark))
		boardGame.updateBoard(mark);
	return false;
}
