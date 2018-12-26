#ifndef SWAN_GUI_RECT2D_HPP
#define SWAN_GUI_RECT2D_HPP

#include <algorithm>
#include "Core/Defs.hpp"
#include "Utility/Math.hpp"
#include "Maths/Vector.hpp"

namespace SWAN
{
    struct Rect2D{
	Rect2D() {}
	Rect2D(int x, int y, int w, int h, vec4 color = {1,1,1,1})
	    : Position(x, y), Size(w, h)
	{
	    Fix();
	    SetColor(color);
	}

	Rect2D(ivec2 Position, ivec2 Size, vec4 color = {1,1,1,1})
	    : Position(Position), Size(Size)
	{
	    Fix();
	    SetColor(color);
	}

	void SetColor(vec4 color = {1,1,1,1})
	{
	    for(int i = 0; i < 4; i++)
		Colors[i] = color;
	}

	void SetHorizontalGradient(vec4 left, vec4 right)
	{
	    Colors[0] = Colors[1] = left;
	    Colors[2] = Colors[3] = right;
	}

	void SetVerticalGradient(vec4 up, vec4 down)
	{
	    Colors[0] = Colors[2] = up;
	    Colors[1] = Colors[3] = down;
	}

	void Fix() {
	    if(Size.x < 0) {Position.x -= Size.x; Size.x = -Size.x;}
	    if(Size.y < 0) {Position.y -= Size.y; Size.y = -Size.y;}
	}

	static Rect2D FromMinMax(ivec2 Min, ivec2 Max, vec4 color = {1,1,1,1})
	{
	    Rect2D res = Rect2D(Min, Max-Min, color);
	    res.Fix();
	    return res;
	}

	/// Clips the rectangle with another.
	void ClipAgainst(Rect2D clip)
	{
	    *this = ClippedAgainst(clip);
	}

	/// Returns a clipped version of the rectangle.
	Rect2D ClippedAgainst(Rect2D clip) const
	{
	    ivec2 TopLeft(
		Util::Clamp(Position.x, clip.Position.x, (clip.Position + clip.Size).x),
		Util::Clamp(Position.y, clip.Position.y, (clip.Position + clip.Size).y)
	    );

	    ivec2 BottomRight(
		Util::Clamp((Position + Size).x, clip.Position.x, (clip.Position + clip.Size).x),
		Util::Clamp((Position + Size).y, clip.Position.y, (clip.Position + clip.Size).y)
	    );

	    return FromMinMax(TopLeft, BottomRight);
	}

	bool IsPointInside(ivec2 point) const
	{
	    return Position.x <= point.x
		&& Position.y <= point.y
		&& (Position + Size).x >= point.x
		&& (Position + Size).y >= point.y;
	}

	ivec2 Position;
	ivec2 Size;
	Array<vec4, 4> Colors
	{
	    {1, 1, 1, 1},
	    {1, 1, 1, 1},
	    {1, 1, 1, 1},
	    {1, 1, 1, 1},
	};
    };
}

#endif
