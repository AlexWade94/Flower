#include "Board.h"
#include "Snake.h"
#include <assert.h>
#include "olcPixelGameEngine.h"
Board::Board(const GameSettings& settings, olc::PixelGameEngine* game)
	:
	dimension(settings.GetTileSize()),
	width(settings.GetBoardWidth()),
	height(settings.GetBoardHeight()),
	contents(width* height, CellContents::Empty),
	game(game)
{

}

void Board::DrawCell(const Location& loc, olc::Pixel c)
{

	const int off_x = (game->ScreenWidth() / 4)  +  borderWidth;
	const int off_y = (game->ScreenHeight() / 4) +  borderHeight;
	
	game->FillRect({ loc.x *dimension + off_x, loc.y*dimension+off_y}, {dimension-cellPadding, dimension-cellPadding }, c);
	
}

int Board::GetGridWidth() const
{
	return width;
}

int Board::GetGridHeight() const
{
	return height;
}

bool Board::IsInsideBoard(const Location& loc) const
{
	return loc.x >= 0 && loc.x < width&&
		loc.y >= 0 && loc.y < height;
}

Board::CellContents Board::GetContents(const Location& loc) const
{
	return contents[loc.y * width + loc.x]; 
}

void Board::ConsumeContents(const Location& loc)
{
	contents[loc.y * width + loc.x] = CellContents::Empty;
}

void Board::SpawnContents(std::mt19937& rng, const std::vector<Snake> snake, CellContents contentsType)
{
	std::uniform_int_distribution<int> xDist(0, GetGridWidth() - 1);
	std::uniform_int_distribution<int> yDist(0, GetGridHeight() - 1);

	Location newLoc;
	newLoc.x = xDist(rng);
	newLoc.y = yDist(rng);
	for (int i=0; i< snake.size(); i++)
	{
		if (snake[i].IsInTile(newLoc) || GetContents(newLoc) != CellContents::Empty)
		{
			newLoc.x = xDist(rng);
			newLoc.y = yDist(rng);
			i = 0;
		}
	}

	contents[newLoc.y * width + newLoc.x] = contentsType;
}

bool Board::PlaceFlower(int x, int y, const std::vector<Snake> snake)
{
	if (x < (game->ScreenWidth() / 4) || x > (game->ScreenWidth()*0.75))
	{
		return false;
	}
	if (y < (game->ScreenHeight() / 4) || y > (game->ScreenHeight() * 0.81))
	{
		return false;
	}
	const int off_x = (game->ScreenWidth() / 4) + cellPadding + borderWidth;
	const int off_y = (game->ScreenHeight() / 4) + cellPadding+ borderHeight;

	int new_x = (float)(x-off_x)/dimension;
	int new_y = (float)(y - off_y)/dimension;
	
	for (int i = 0; i < snake.size(); i++)
	{
		if (snake[i].IsInTile({new_x,new_y}) || GetContents({ new_x,new_y }) != CellContents::Empty)
		{
			return false;
		}
	}


	contents[new_y * width + new_x] = CellContents::Flower;

	return true;

}

void Board::DrawBorder()
{
	const int top = game->ScreenHeight()/4;
	const int left = game->ScreenWidth()/ 4;
	const int bottom = height * dimension+borderHeight;
	const int right =  width * dimension+borderWidth+cellPadding;
	
	//top
	game->FillRect({ left,top }, { right - cellPadding + borderWidth, borderHeight }, borderColor);
	//left
	game->FillRect({ left,top }, { borderWidth,  bottom }, borderColor);
	//bottom
	game->FillRect({ left, bottom +  top - borderHeight+ cellPadding }, { right - cellPadding + borderWidth, borderHeight }, borderColor);
	//right
	game->FillRect({ right + left-cellPadding , top }, { borderWidth,  bottom+ cellPadding }, borderColor);
}

Location Board::ConvertToBoardSpace(int x, int y)
{
	const int off_x = (game->ScreenWidth() / 4) + cellPadding + borderWidth;
	const int off_y = (game->ScreenHeight() / 4) + borderHeight + cellPadding;

	int new_x = (float)(x - off_x) / dimension;
	int new_y = (float)(y - off_y) / dimension;

	return { new_x, new_y };
}


void Board::DrawCells()
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			switch (GetContents({ x,y }))
			{
			case Board::CellContents::Flower:
				DrawCell({ x,y }, olc::RED);
				break;
			}
		}
	}
}
