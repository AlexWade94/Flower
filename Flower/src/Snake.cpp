#include "Snake.h"
#include <assert.h>

Snake::Snake(const Location& loc)
{
	segments.emplace_back(loc);
	Grow();
}

void Snake::MoveBy(const Location& delta_loc)
{
	for (int i = segments.size() - 1; i > 0; --i)
	{
		segments[i].Follow(segments[i - 1]);
	}
	segments.front().MoveBy(delta_loc);
}

Location Snake::GetNextHeadLocation(const Location& delta_loc) const
{
	Location l(segments[0].GetLocation());
	l.Add(delta_loc);
	return l;
}

void Snake::Grow()
{
	segments.emplace_back(segments[segments.size()-1].GetLocation(),bodyColors[segments.size() % nBodyColors]);
	
}

void Snake::Draw(Board& brd) const
{
	for (const auto& s : segments)
	{
		s.Draw(brd);
	}
}

bool Snake::IsInTileExceptEnd(const Location& target) const
{
	for (size_t i = 0; i < segments.size() - 1; ++i)
	{
		if (segments[i].GetLocation() == target)
		{
			return true;
		}
	}
	return false;
}

bool Snake::IsInTile(const Location& target) const
{
	for (const auto& s : segments)
	{
		if (s.GetLocation() == target)
		{
			return true;
		}
	}
	return false;
}

void Snake::TrimTail()
{
	segments.pop_back();
}

bool Snake::IsDead()
{
	return segments.size()==0 || dead;
}

void Snake::Kill()
{
	dead = true;
}

int Snake::GetLength() const
{
	return segments.size();
}

Location Snake::GetDelta() const
{
	return delta;
}

void Snake::SetDelta(Location new_delta)
{
	delta = new_delta;
}

Snake::Segment::Segment(const Location& in_loc)
{
	loc = in_loc;
	c = olc::YELLOW;
}

Snake::Segment::Segment(const Location& in_loc, olc::Pixel c_in)
{
	loc = in_loc;
	c = c_in;
}

void Snake::Segment::Follow(const Segment& next)
{
	loc = next.loc;
}

void Snake::Segment::MoveBy(const Location& delta_loc)
{
	loc.Add(delta_loc);
}

void Snake::Segment::Draw(Board& brd) const
{
	brd.DrawCell(loc, c);
}

const Location& Snake::Segment::GetLocation() const
{
	return loc;
}
