#pragma once
#include "stdafx.h"
#include "src\gsm\physics\CollidableObject.h"

class CollidableObjectsSortingTester
{
private:
	// WE'LL USE THESE FOR TESTING
	vector<CollidableObject*> *coVector;
	unsigned int sortingCriteria;

public:
	// FOR TESTING CollidableObject SORTING

	// NOTE THAT WE ARE ONLY USING THESE METHODS IN ONE PLACE, 
	// SO WE'LL JUST INLINE THEM

	void testCollidableObjectsSorting()
	{
		generateRandomCollidableObjectsVector(20);
	
		sortingCriteria = LEFT_EDGE;

		// NOT SORTED YET
		printCollidableObjectsVector();

		// NOW IT'S SORTED
		sort(coVector->begin(), coVector->end(), SweptShapesComparitorByLeft());
		printCollidableObjectsVector();

		sortingCriteria = RIGHT_EDGE;
		sort(coVector->begin(), coVector->end(), SweptShapesComparitorByRight());
		printCollidableObjectsVector();

		sortingCriteria = TOP_EDGE;
		sort(coVector->begin(), coVector->end(), SweptShapesComparitorByTop());
		printCollidableObjectsVector();

		sortingCriteria = BOTTOM_EDGE;
		sort(coVector->begin(), coVector->end(), SweptShapesComparitorByBottom());
		printCollidableObjectsVector();
	}

	void generateRandomCollidableObjectsVector(int numElements)
	{	
		coVector = new vector<CollidableObject*>();
		srand((unsigned int)time(NULL));
		for (int i = 0; i < 20; i++)
		{
			CollidableObject *co = new CollidableObject();
			AABB *aabb = co->getBoundingVolume();
			float randX = (float)(rand() % 1000);
			float randY = (float)(rand() % 1000);
			aabb->setCenterX(randX);
			aabb->setCenterY(randY);
			aabb->setWidth(64);
			aabb->setHeight(64);
			float randVx = (float)(rand() % 20) - 10.0f;
			float randVy = (float)(rand() & 20) - 10.0f;
			co->getPhysicalProperties()->setVelocity(randVx, randVy);
			co->updateSweptShape(1.0f);
			coVector->push_back(co);
		}
	}

	void printCollidableObjectsVector()
	{
		cout << "Current Vector Contents\n";
		cout << "\t-is in sorted order: " << isInSortedOrder() << "\n";
		vector<CollidableObject*>::iterator it = coVector->begin();
		while (it != coVector->end())
		{
			cout << "\t";
			CollidableObject *co = (*it);
			AABB *aabb = co->getSweptShape();
			float left = aabb->getLeft();
			float right = aabb->getRight();
			float top = aabb->getTop();
			float bottom = aabb->getBottom();
			cout << "\tLeft: " << left << ", Right: " << right << ", Top: " << top << ", Bottom: " << bottom << "\n";
			it++;
		}
		cout << "\n";
	}

	bool isInSortedOrder()
	{
		for (unsigned int i = 0; i < (coVector->size()-1); i++)
		{
			CollidableObject *co1 = coVector->at(i);
			AABB *ss1 = co1->getSweptShape();
			CollidableObject *co2 = coVector->at(i+1);
			AABB *ss2 = co2->getSweptShape();
			if (sortingCriteria == LEFT_EDGE)
			{
				if (ss1->getLeft() > ss2->getLeft())
					return false;
			}
			else if (sortingCriteria == RIGHT_EDGE)
			{
				if (ss1->getRight() > ss2->getRight())
					return false;
			}
			else if (sortingCriteria == TOP_EDGE)
			{
				if (ss1->getTop() > ss2->getTop())
					return false;
			}
			else if (sortingCriteria == BOTTOM_EDGE)
			{
				if (ss1->getBottom() > ss2->getBottom())
					return false;
			}		
		}
		return true;
	}
};