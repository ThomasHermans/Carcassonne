#include "View/Meeple.h"

View::Meeple::Meeple( MeepleType inType, Color inColor )
:
	mType( inType ),
	mColor( inColor )
{
}

View::Meeple::MeepleType
View::Meeple::getType() const
{
	return mType;
}

View::Color
View::Meeple::getColor() const
{
	return mColor;
}