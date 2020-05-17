#ifndef _PhysicsObject_h_
#define _PhysicsObject_h_

#include "Object.h"

#include "BulletListener.h"

struct ShapeParams
{
	ShapeParams() {}

	enum ShapeType
	{
		BoxShape,
		CylinderShape,
		SphereShape,
		ConvexHullShape,
		CompoundShape,
		GImpactShape
	};

	ShapeType type;

	bool isStatic; // only for trimesh

	// set to 0,0,0 to use entity's bounding box
	Ogre::Vector3 boundingBox; // for box
	Ogre::Vector3 halfExtents; // for cylinder
	Ogre::Vector3 axis; // for cylinder
	Ogre::Real radius; // for sphere
};

struct PhysicsObjectCreationParams : ObjectCreationParams
{
	PhysicsObjectCreationParams() {}

	Ogre::Real restitution;
	Ogre::Real friction;
	Ogre::Real mass;

	ShapeParams shape;
};

class PhysicsObject : public Object
{
public:
	PhysicsObject();
	~PhysicsObject();

	virtual void create(PhysicsObjectCreationParams params);
	virtual void destroy();

	void setDamping(Ogre::Real linearDamping, Ogre::Real angularDaming) { mBody->setDamping(linearDamping, angularDaming); }

	Ogre::Real getLinearDamping() { return mBody->getBulletRigidBody()->getLinearDamping(); }
	Ogre::Real getAngularDamping() { return mBody->getBulletRigidBody()->getAngularDamping(); }

	void setLinearFactor(Ogre::Vector3 factor) { mBody->getBulletRigidBody()->setLinearFactor(OgreBulletCollisions::convert(factor)); }
	void setAngularFactor(Ogre::Vector3 factor) { mBody->getBulletRigidBody()->setAngularFactor(OgreBulletCollisions::convert(factor)); }

	void setLinearVelocity(Ogre::Vector3 velocity) { mBody->getBulletRigidBody()->setLinearVelocity(OgreBulletCollisions::convert(velocity)); }
	void setAngularVelocity(Ogre::Vector3 velocity) { mBody->getBulletRigidBody()->setAngularVelocity(OgreBulletCollisions::convert(velocity)); }

	void setGravity(Ogre::Vector3 gravity) { mBody->getBulletRigidBody()->setGravity(OgreBulletCollisions::convert(gravity)); }
	void disableGravity() { mBody->getBulletRigidBody()->setGravity(btVector3(0,0,0)); }
	void enableGravity() { mBody->getBulletRigidBody()->setGravity(gEnv->bulletListener->getWorld()->getBulletDynamicsWorld()->getGravity()); }

	btRigidBody* getBulletRigidBody() { return mBody->getBulletRigidBody(); }


	OgreBulletCollisions::CollisionShape* PhysicsObject::getShape() { return mShape; }
	OgreBulletDynamics::RigidBody* PhysicsObject::getBody()	{ return mBody;	}
protected:
	OgreBulletCollisions::CollisionShape* mShape;
	OgreBulletDynamics::RigidBody* mBody;
};

#endif // _PhysicsObject_h_