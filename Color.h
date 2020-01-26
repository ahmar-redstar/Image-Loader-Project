#ifndef _COLOR_H
#define	_COLOR_H

class Color
{
public:
	bool operator==(const Color& other) const
	{
		return r == other.r && g == other.g && b == other.b;
	}

	unsigned int r;
	unsigned int g;
	unsigned int b;
};

#endif