#pragma once
#include "Board.h"
#include <vector>
class Snake
{
private:
	class Segment
	{
	public:
		Segment(const Location& loc);
		Segment(const Location& loc, olc::Pixel c);
		void Follow(const Segment& next);
		void MoveBy(const Location& delta_loc);
		void Draw(Board& brd) const;
		const Location& GetLocation() const;
	private:
		Location loc;
		olc::Pixel c;
	};
public:
	Snake(const Location& loc);
	void MoveBy(const Location& delta_loc);
	Location GetNextHeadLocation(const Location& delta_loc) const;
	void Grow();
	void Draw(Board& brd) const;
	bool IsInTileExceptEnd(const Location& target) const;
	bool IsInTile(const Location& target) const;
	void TrimTail();
	bool IsDead();
	void Kill();
	int GetLength() const;
	Location GetDelta() const;
	void SetDelta(Location new_delta);
private:
	static constexpr int nBodyColors = 4;
	olc::Pixel bodyColors[nBodyColors] = {
		{ 10,100,32 },
		{ 10,130,48 },
		{ 18,160,48 },
		{ 10,130,48 }
	};
	std::vector<Segment> segments;
	Location delta = { 1,0 };
	bool dead=false;
};