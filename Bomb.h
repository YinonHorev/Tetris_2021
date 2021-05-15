#pragma once
#include "GameObjects.h"
#include "Special.h"

class Bomb : public Special {
	int bla = 0;
public:

	//ctor
	Bomb(Board &_boardGame) : Special(_boardGame) {
		body.setPoint((boardGame.gameZone.left + boardGame.gameZone.right) / 2, boardGame.gameZone.top, true, getSerialObj(), Sign::B);
	}

	//draw
	void draw(char ch = '#')const override;
    
    CLONEABLE(Bomb)
	
	// move
	bool move(char keyPressed = DEFAULT, bool mark = true) override;

	bool isBomb() override { return true; };


};
