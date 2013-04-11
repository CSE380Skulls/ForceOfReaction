// ColisionsTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "src\gsm\physics\Collision.h"
#include "src\gsm\physics\CollidableObject.h"
#include "src\gsm\physics\AABB.h"
#include "src\gsm\physics\Physics.h"
#include "CollisionsSortingTester.h"
#include "CollidableObjectsSortingTester.h"

int _tmain(int argc, _TCHAR* argv[])
{
	// TEST SORTING COLLISIONS
	CollisionsSortingTester cst;
	cst.testCollisionsSorting();

	// TEST SORTING COLLIDABLE OBJECTS
	CollidableObjectsSortingTester cost;
	cost.testCollidableObjectsSorting();
	
	cout << "Press ENTER";
	std::getchar();

	return 0;
}

