#ifndef VIEW_MEEPLE_20140504
#define VIEW_MEEPLE_20140504

#include "View/Typedefs.h"

namespace View
{
	class Meeple
	{
	public:
		enum MeepleType
		{
			kFollower,
			kLargeFollower,
			kBuilder,
			kPig
		};

	public:
		Meeple( MeepleType inType, Color inColor );

		MeepleType getType() const;
		Color getColor() const;

	private:
		MeepleType mType;
		Color mColor;
	};
}

#endif