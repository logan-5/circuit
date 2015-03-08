#pragma once

struct Color
{
public:
							Color( const unsigned char red = 0, const unsigned char green = 0, const unsigned char blue = 0, const unsigned char alpha = 0 );
							~Color();
							Color( const Color& other ) : red( other.red ), green( other.green ), blue( other.blue ), alpha( other.alpha ) {}
	bool operator			==( const Color& c ) { return red == c.red && green == c.green && blue == c.blue && alpha						== c.alpha; }
	bool operator			!=( const Color& c ) { return !( *this == c ); }
	unsigned char			red, green, blue, alpha;
};

