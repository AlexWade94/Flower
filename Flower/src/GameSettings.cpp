#include "GameSettings.h"
#include <fstream>

GameSettings::GameSettings(const std::string& filename)
{
	std::fstream in(filename);
	for (std::string line; std::getline(in, line); )
	{
		if (line == "[Tile Size]")
		{
			in >> tileSize;
		}
		else if (line == "[Board Dimensions]")
		{
			in >> boardWidth >> boardHeight;
		}

		else if (line == "[Starting Budget]")
		{
			in >> startingBudget;
		}

		else if (line == "[Screen Resolution]")
		{
			in >> screenWidth >> ScreenHeight;
		}

		else if (line.empty())
		{

		}

		else
		{
			throw std::runtime_error("Bad line in settings file: " + line);
		}
	}
}

int GameSettings::GetTileSize() const
{
	return tileSize;
}

int GameSettings::GetBoardWidth() const
{
	return boardWidth;
}

int GameSettings::GetBoardHeight() const
{
	return boardHeight;
}

int GameSettings::GetStartingBudget() const
{
	return startingBudget;
}





