#include "stdafx.h"

#include "Application.h"
#include "FrameListener.h"
#include "InputListener.h"
#include "BulletListener.h"
#include "LuaIntegration.h"

#include "Object.h"

Object::Object()
{

}

Object::~Object()
{

}

void Object::create(ObjectCreationParams params)
{
	mEntity = gEnv->application->getSceneManager()->createEntity(params.name, params.mesh);

	mNode = gEnv->application->getSceneManager()->getRootSceneNode()->createChildSceneNode(params.name + "_node");
	mNode->attachObject(mEntity);

	mNode->setPosition(params.position);
	mNode->setOrientation(params.orientation);

	gEnv->objects.push_back(this);
	luabind::globals(gEnv->luaState->getState())["ObjectsInitialized"] = gEnv->objects.size();
}

void Object::destroy()
{
	// remove scene node and entity here

	if(mNode)
		delete mNode;
	if(mEntity)
		delete mEntity;

	mNode = 0;
	mEntity = 0;

	// FIX
	/*auto item = std::remove(gEnv->objects.begin(), gEnv->objects.end(), this);
	gEnv->objects.erase(item);*/
}