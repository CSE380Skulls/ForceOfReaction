#pragma once
#include "stdafx.h"
#include "src\gsm\physics\PhysicalProperties.h"
#include <Box2D\Box2D.h>

class BoxPhysicsObject
{
protected:
	//TEST OBJECT FOR BOX 2D
	b2Body *box2d_body;
	//boolen that indicates whether the object is not jumping
	bool objectJumping;
	bool collidingStatic;

public:
	BoxPhysicsObject(){box2d_body = NULL; objectJumping = false;}
	~BoxPhysicsObject(){};

	bool				isDynamicObject()			{ return (box2d_body->GetType() == b2_dynamicBody);}
	//TEST ACCESSORS FOR BOX2D
	float32				getCurrentBodyX()			{ return box2d_body->GetPosition().x;	}
	float32				getCurrentBodyY()			{ return box2d_body->GetPosition().y;	}
	//Get the current body's angle in radians
	float32				getCurrentBodyAngle()		{ return box2d_body->GetAngle();		}
	void				setCurrentBodyAngleVelocity(float32 a) { box2d_body->SetAngularVelocity(a); }
	b2Body*				getPhysicsBody()			{ return box2d_body;					}
	bool				isJumping()					{ return objectJumping;					}
	void				setIsJumping(bool jump)		{ objectJumping = jump;					}

	void initPhysicsBody(b2Body *body){
		//we only want to assign the bodies once to a physics object
		if(box2d_body == NULL){
			box2d_body = body;
		}
	}

	// Return this box2d body to box2d
	void returnPhysicsBody(){
		if(box2d_body != NULL){
			box2d_body->GetWorld()->DestroyBody(box2d_body);
		}
		box2d_body = NULL;
	}
};