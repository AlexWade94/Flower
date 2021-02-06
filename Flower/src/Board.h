#pragma once
#include "olcPixelGameEngine.h"
#include "Location.h"
#include <random>
#include "GameSettings.h"
#include <vector>

class Board
{
public:
	enum class CellContents
	{
		Empty,
		Flower
	};
public:
	Board(const GameSettings& settings, olc::PixelGameEngine* game);
	~Board() { game = nullptr; }
	void DrawCell(const Location& loc, olc::Pixel c);
	int GetGridWidth() const;
	int GetGridHeight() const;
	bool IsInsideBoard(const Location& loc) const;
	CellContents GetContents(const Location& loc) const;
	void ConsumeContents(const Location& loc);
	void SpawnContents(std::mt19937& rng, const std::vector<class Snake> snake, CellContents contents);
	bool PlaceFlower(int x, int y, const std::vector<Snake> snake);
	void DrawBorder();
	Location ConvertToBoardSpace(int x, int y);
	void DrawCells();
private:

	const olc::Pixel borderColor		= olc::BLUE;
	const olc::Pixel obstacleColor		= olc::GREY;
	const olc::Pixel flowerColor		= olc::RED;
	olc::PixelGameEngine* game;
	int dimension;
	static constexpr int cellPadding	= 1;
	int width;
	int height;
	static constexpr int borderWidth	= 4;
	static constexpr int borderHeight	= 4;
	static constexpr int borderPadding	= 2;
	std::vector<CellContents> contents;
};