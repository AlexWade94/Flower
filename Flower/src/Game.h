#pragma once
#include "olcPixelGameEngine.h"
#include "GameSettings.h"
#include "Board.h"
#include "Snake.h"
#include <memory>

class Game : public olc::PixelGameEngine
{
public:
	Game();

public:
	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;
	void Restart();
	void Draw();
private:
	std::mt19937 rng;
	
	GameSettings settings = "settings.txt";
	Board board;
	

	std::vector<Snake> sneks;

	//amount of coins
	int budget;

	//Total amount
	int flowersCounter =				0;
	int flowerIncome =					3;
	int flowerCost =					50;
	
	float flowerIncomePeriod =			1.0f;
	float flowerIncomeCounter =			0.0f;
	
	//Maximum amount of snakes at the same time
	int sneksMax =						10;

	//Maximum snake size
	int maxSnekSize =					10;
	
	float snekAppearPeriod =			5.0f;
	float snekAppearCounter =			0.0f;

	//After given time new snakes will appear slower
	float snekAppearSlowerPeriod =		10.0f;
	float snekAppearSlowerCounter =		0.0f;
	float snekAppearSlowerCoef =		1.05f;

	float snekGrowPeriod =				3.0f;
	float snekGrowCounter =				0.0f;

	float snekMovePeriod =				0.3f;
	float snekMoveCounter =				0.0f;

	//check for speedup event 
	bool speedup;
	float speedupPeriod =				5.0f;
	float speedupCounter =				0.0f;
	float snekMoveNormal =				0.3f;
	float snekMoveFlowered =			0.15f;

	//Game flags (start screen and game over)
	bool gamestarted =					false;
	bool gameover =						false;
};
