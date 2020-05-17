#pragma once

#include "OgreIncludes.h"
#include "OgreBulletIncludes.h"

#include <OgreOggSound.h>

/* for tostring() of luabind */
std::ostream& operator << (std::ostream& stream, const Ogre::Entity ent);

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

#include "Gorilla.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <Windows.h>
#include "Resources/Resource.h"
#endif

#include "Environment.h"