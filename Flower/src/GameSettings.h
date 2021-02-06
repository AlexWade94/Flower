#pragma once
#include <string>

class GameSettings
{
public:
	GameSettings(const std::string& filename);
	int GetTileSize() const;
	int GetBoardWidth() const;
	int GetBoardHeight() const;
	int GetStartingBudget() const;
private:
	int tileSize;
	int boardWidth;
	int boardHeight;
	int startingBudget;
	int screenWidth;
	int ScreenHeight;
};