#include "stdafx.h"

#include "PhysicsObject.h"

#include "Application.h"
#include "FrameListener.h"
#include "InputListener.h"
#include "BulletListener.h"
#include "LuaIntegration.h"

PhysicsObject::PhysicsObject()
{

}

PhysicsObject::~PhysicsObject()
{

}

void PhysicsObject::create(PhysicsObjectCreationParams params)
{
	Object::create(params);

	switch(params.shape.type)
	{
	case ShapeParams::BoxShape:
		if(params.shape.boundingBox != Ogre::Vector3(0,0,0))
			mShape = new OgreBulletCollisions::BoxCollisionShape(params.shape.boundingBox);
		else
			mShape = new OgreBulletCollisions::BoxCollisionShape(mEntity->getBoundingBox().getHalfSize());
		break;
	case ShapeParams::CylinderShape:
		mShape = new OgreBulletCollisions::CylinderCollisionShape(params.shape.halfExtents, params.shape.axis);
		break;
	case ShapeParams::SphereShape:
		mShape = new OgreBulletCollisions::SphereCollisionShape(params.shape.radius);
		break;
	case ShapeParams::ConvexHullShape:
		if(params.shape.isStatic)
		{
			OgreBulletCollisions::StaticMeshToShapeConverter* converter = new OgreBulletCollisions::StaticMeshToShapeConverter(mEntity);
			mShape = converter->createConvex();
			delete converter;
		}
		else
		{
			OgreBulletCollisions::AnimatedMeshToShapeConverter* converter = new OgreBulletCollisions::AnimatedMeshToShapeConverter(mEntity);
			mShape = converter->createConvex();
			delete converter;
		}
		break;
	case ShapeParams::CompoundShape:
		if(params.shape.isStatic)
		{
			OgreBulletCollisions::StaticMeshToShapeConverter* converter = new OgreBulletCollisions::StaticMeshToShapeConverter(mEntity);
			mShape = converter->createConvexDecomposition();
			delete converter;
		}
		else
		{
			OgreBulletCollisions::AnimatedMeshToShapeConverter* converter = new OgreBulletCollisions::AnimatedMeshToShapeConverter(mEntity);
			mShape = converter->createConvexDecomposition();
			delete converter;
		}
		break;
	case ShapeParams::GImpactShape:
		if(params.shape.isStatic)
		{
			OgreBulletCollisions::StaticMeshToShapeConverter* converter = new OgreBulletCollisions::StaticMeshToShapeConverter(mEntity);
			mShape = converter->createConcave();
			delete converter;
		}
		else
		{
			OgreBulletCollisions::AnimatedMeshToShapeConverter* converter = new OgreBulletCollisions::AnimatedMeshToShapeConverter(mEntity);
			mShape = converter->createConcave();
			delete converter;
		}
		break;
	default:
		break;
	}

	mBody = new OgreBulletDynamics::RigidBody(params.name + "_rigidBody", gEnv->bulletListener->getWorld());
	mBody->setShape(mNode, mShape, params.restitution, params.friction, params.mass, params.position, params.orientation);
	mBody->disableDeactivation();
}

void PhysicsObject::destroy()
{
	if(mBody)
		delete mBody;
	if(mShape)
		delete mShape;

	mBody = 0;
	mShape = 0;

	Object::destroy();
}