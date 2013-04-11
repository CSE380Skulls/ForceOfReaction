#pragma once
#include "stdafx.h"
#include "src\gsm\physics\Collision.h"

class CollisionsSortingTester
{
private:
	// WE'LL USE THESE FOR TESTING
	vector<Collision*> *cVector;

public:
	// FOR TESTING Collision SORTING
	void CollisionsSortingTester::testCollisionsSorting()
	{
		generateRandomCollisionsVector(20);
		printCollisionsVector();
		sort(cVector->begin(), cVector->end(), CollisionComparitor());
		printCollisionsVector();
	}
	
	void CollisionsSortingTester::generateRandomCollisionsVector(int numElements)
	{
		cVector = new vector<Collision*>();
		srand((unsigned int)time(NULL));
		for (int i = 0; i < 20; i++)
		{
			Collision *c = new Collision();
			int randomInt = rand();
			float randFloat = randomInt/((float)RAND_MAX);
			c->setTimeOfCollision(randFloat);
			cVector->push_back(c);
		}
	}
	
	void CollisionsSortingTester::printCollisionsVector()
	{
		cout << "Current Vector Contents\n";
		cout << "\t-is in sorted order: " << isInSortedOrder() << "\n";
		vector<Collision*>::iterator it = cVector->begin();
		while (it != cVector->end())
		{
			cout << "\t";
			Collision *c = (*it);
			float timeOfCollision = c->getTimeOfCollision();
			cout << "\tt: " << timeOfCollision << "\n";
			it++;
		}
		cout << "\n";
	}
	
	bool isInSortedOrder()
	{
		for (unsigned int i = 0; i < (cVector->size()-1); i++)
		{
			Collision *c1 = cVector->at(i);
			Collision *c2 = cVector->at(i+1);
			if (c1->getTimeOfCollision() < c2->getTimeOfCollision())
				return false;
		}
		return true;
	}
};