#ifndef _Object_h_
#define _Object_h_

#include "stdafx.h"

struct ObjectCreationParams
{
	ObjectCreationParams() {}

	Ogre::String name;
	Ogre::String mesh;
	Ogre::Vector3 position;
	Ogre::Quaternion orientation;
};

class Object
{
public:
	Object();
	~Object();

	virtual void create(ObjectCreationParams params);
	virtual void destroy();

	virtual void update() {}

	virtual void setPosition(Ogre::Vector3 position) { mNode->setPosition(position); }
	virtual void setOrientation(Ogre::Quaternion orientation) { mNode->setOrientation(orientation); }

	virtual Ogre::Vector3 getPosition() { return mNode->getPosition(); }
	virtual Ogre::Quaternion getOrientation() { return mNode->getOrientation(); }


	Ogre::Entity* getEntity() { return mEntity; }
	Ogre::SceneNode* getNode() { return mNode; }
protected:
	Ogre::Entity* mEntity;
	Ogre::SceneNode* mNode;
};

#endif // _Object_h_