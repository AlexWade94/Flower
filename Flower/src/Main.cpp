#include "Game.h"

int main()
{

#ifdef _NDEBUG
	FreeConsole();
#endif
	std::fstream in("Settings.txt");
	int screenWidth = 0, screenHeight= 0;
	for (std::string line; std::getline(in, line); )
	{
		if (line == "[Screen Resolution]")
		{
			in >> screenWidth >> screenHeight;
		}
		else 
		{

		}
	}

	if (screenWidth < 0 || screenWidth>5120)
	{
		screenWidth = 840;
	}
	if (screenHeight < 0 || screenHeight>1440)
	{
		screenWidth = 620;
	}

	Game flower;

	if (flower.Construct(screenWidth, screenHeight, 1, 1))
		flower.Start();

	return 0;
}
