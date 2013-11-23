#ifndef TYPEDEFS_VIEW_THHR_20130817
#define TYPEDEFS_VIEW_THHR_20130817

namespace Gui
{
	int const kTileHeight = 100;
	int const kTileWidth = kTileHeight;
	double const kFirstBorder = .375 * kTileHeight;
	double const kSecondBorder = kTileHeight - kFirstBorder;
	int const kMeepleWidth = 25;
	int const kMeepleHeight = 30;

	enum Color
	{
		kRed,
		kGreen,
		kBlue,
		kYellow,
		kBlack,
		kGray
	};
}

namespace View
{
	enum Rotation
	{
		kCw0,
		kCw90,
		kCw180,
		kCw270
	};

	enum Color
	{
		kRed,
		kGreen,
		kBlue,
		kYellow,
		kBlack,
		kGray
	};

	enum Piece
	{
		kFollower,
		kLargeFollower,
		kBuilder,
		kPig
	};
}

#endif // TYPEDEFS_VIEW_THHR_20130817
