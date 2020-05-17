#include "stdafx.h"

std::ostream& operator << (std::ostream& stream, const Ogre::Entity ent)
{
	return stream << ent.getName();
}