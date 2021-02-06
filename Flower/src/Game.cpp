#include "Game.h"

Game::Game()
	:
	rng(std::random_device()()),
	board(settings, (olc::PixelGameEngine*)this)
{
	sAppName = "Flower";
}

bool Game::OnUserCreate()
{
	// Called once at the start, so create things here
	budget = settings.GetStartingBudget();
	
	return true;
}

bool Game::OnUserUpdate(float fElapsedTime)
{
	Clear(olc::BLACK);

	//Places flower and spawns 2 snakes in random places by right-click
	//Dismisses if click is outside of the board or you don't have enough money. Doesn't spawn snakes if maximum amount reached
	
	if (GetMouse(1).bPressed && budget>=flowerCost)
	{
		if(board.PlaceFlower(GetMousePos().x, GetMousePos().y,sneks))
		{
			if(sneks.size() < sneksMax)
			{
				std::uniform_int_distribution<int> xDist(0, settings.GetBoardWidth() - 1);
				std::uniform_int_distribution<int> yDist(0, settings.GetBoardHeight() - 1);
				sneks.push_back(Snake({ xDist(rng), yDist(rng) }));
				sneks.push_back(Snake({ xDist(rng), yDist(rng) }));
			}

		budget -= flowerCost;
		flowersCounter++;
		snekAppearCounter = 0.0f;
		}
	}

	//Game is on if you have at least 1 flower
	if (flowersCounter > 0)
	{
		gamestarted = true;
		gameover = false;
	}

	//If you left-click on a snake, remove one segment from tail
	//Convert mouse position to board space
	for(auto& snek : sneks)
	{
		if (GetMouse(0).bPressed && snek.IsInTile(board.ConvertToBoardSpace(GetMousePos().x, GetMousePos().y )))
		{
			snek.TrimTail();
		}
	}

	//if Snake has 0 segments-- remove it
	const auto new_end = std::remove_if(sneks.begin(), sneks.end(), [](Snake& snake)
		{
			return snake.IsDead();
		});
	// erase garbage husk element at end
	sneks.erase(new_end, sneks.end());


	//update counters for every event
	if(gamestarted && flowersCounter > 0)
	{
		snekMoveCounter			+= fElapsedTime;
		snekAppearCounter		+= fElapsedTime;
		snekGrowCounter			+= fElapsedTime;
		snekAppearSlowerCounter 	+= fElapsedTime;
		flowerIncomeCounter     	+= fElapsedTime;
	}

	//if you lose all of your flowers Game Over
	else if(gamestarted && flowersCounter == 0)
	{
		gameover = true;
		Restart();
	}

	//Every fixed period of time new snakes will appear less friquenly
	if (snekAppearSlowerCounter >= snekAppearSlowerPeriod)
	{
		snekAppearPeriod *= snekAppearSlowerCoef;
		snekAppearSlowerCounter -= snekAppearSlowerPeriod;
	}

	//if flower was eaten all snakes move faster, if they ate 2 flowers the counter gets nulified and they get additional time
	if (speedup)
	{
		speedupCounter += fElapsedTime;

		if (speedupCounter >= speedupPeriod)
		{
			speedup = false;
			snekMovePeriod = snekMoveNormal;
			speedupCounter -= speedupPeriod;
		}
	}

	//Flowers give you money every fixed period based on their amount * how many you get per 1 flower
	if (flowerIncomeCounter >= flowerIncomePeriod)
	{
		budget += flowersCounter * flowerIncome;
		flowerIncomeCounter -= flowerIncomePeriod;
	}

	//If there is only 1 flower, snakes can grow up to 10 segments
	if (flowersCounter > 1)
	{
		maxSnekSize = 5;
	}
	else
		maxSnekSize = 10;

	//Every fixed period of time all snakes are getting additional segment if snake is not maxsized yet
	if (snekGrowCounter > snekGrowPeriod)
	{
		for (auto& snek : sneks)
		{
			if(snek.GetLength()< maxSnekSize)
			{
				snek.Grow();
			}
			
		}
		snekGrowCounter -= snekGrowPeriod;
	}

	//Every fixed period of time a new snake appears if there is at least 1 flower and if amount of snakes is not maxsized yet
	if (snekAppearCounter>=snekAppearPeriod && flowersCounter>0 && sneks.size() <= sneksMax)
	{
		std::uniform_int_distribution<int> xDist(0, settings.GetBoardWidth()-1);
		std::uniform_int_distribution<int> yDist(0, settings.GetBoardHeight()-1);
		sneks.push_back(Snake({ xDist(rng), yDist(rng) }));
		snekAppearCounter -= snekAppearPeriod;
	}

	//moves the snake by fixtime step
	if (snekMoveCounter >= snekMovePeriod)
	{
		snekMoveCounter -= snekMovePeriod;

		std::uniform_int_distribution<int> xDist(-1, 1);
		std::uniform_int_distribution<int> yDist(-1, 1);
		Location new_delta_loc;
		Location next;
		int deltax;
		int deltay;
		int movecounter;

		//Update each snake
		for(auto& snek : sneks)
		{
			movecounter = 0;
			do {
				//prevents diagonal movement (x+y must equal to 1), reverse movement and movement out of border
				deltax = xDist(rng);
				if (std::abs(deltax) == 1)
				{
					deltay = 0;
				}
				else
				{
					deltay = yDist(rng);
				}

				new_delta_loc = { deltax,deltay };
				next = snek.GetNextHeadLocation({ new_delta_loc });
				movecounter++;

				//Prevents game-lock. If snake can't find a path in 40 tries-- delete it
				if (movecounter > 40)
				{
					snek.Kill();
					break;
				}

				//If delta is reverse to previous movement, if movement is diagonal, if snek will go out of border or if it will go inside itself, pick another delta 
				//snakes can move through each other on purpose  
				} while (snek.GetDelta() == -new_delta_loc || std::abs(deltax + deltay == 0) || snek.IsInTileExceptEnd(next) || !board.IsInsideBoard(next));

		//Set delta and move the snake
		snek.SetDelta(new_delta_loc);
		snek.MoveBy(snek.GetDelta());

		//Look inside the snake destination cell
		//if it contains flower-- remove it and speed up all snakes for given period
		const Board::CellContents contents = board.IsInsideBoard(next) ? board.GetContents(next) : Board::CellContents::Empty;

		if (contents == Board::CellContents::Flower)
		{
			board.ConsumeContents(next);
			speedup = true;
			snekMovePeriod = snekMoveFlowered;
			flowersCounter--;
			speedupCounter = 0.0f;
		}

		}
		//End of the snake loop
	}

	//Update every given time-step, but draw every frame
	Draw();

	return true;
}

void Game::Restart()
{
	budget = settings.GetStartingBudget();
	sneks.erase(sneks.begin(), sneks.end());
	snekMovePeriod = snekMoveNormal;

	snekMoveCounter				= 0.0f;
	snekMoveCounter				= 0.0f;
	snekAppearCounter			= 0.0f;
	snekGrowCounter 			= 0.0f;
	snekAppearSlowerCounter 		= 0.0f;
	flowerIncomeCounter			= 0.0f;
	speedupCounter				= 0.0f;
}

void Game::Draw()
{
	//Draw start screen
	if (!gamestarted)
	{
		DrawStringDecal({ (float)ScreenWidth() / 3,(float)ScreenHeight() / 2 }, "Place flower to start!");
	}
	//Draw game over screen
	else if(gamestarted && flowersCounter == 0)
	{
		DrawStringDecal({ (float)ScreenWidth() / 3,(float)ScreenHeight() / 2 }, "You lose! Place flower to start new game");
	}

	for (auto& snek : sneks)
	{
		snek.Draw(board);
	}

	board.DrawBorder();
	board.DrawCells();

	//Print budget
	std::stringstream ss;
	ss << budget;
	std::string str = ss.str();

	DrawStringDecal({ 100,100 }, str);
}
