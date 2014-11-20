#include "car.h"
#include "physicsengine.h"
#include "ball.h"
#include <QtGui>

//random number between [a,b] 
int randint(int a, int b)
{
	return (qrand() % (b - a + 1)) + a;
}

Car* Car::mutate()
{
	Car* newcar = new Car(nodes);
	return newcar;
}

Car* Car::breed(Car* c)
{
	Car* newcar = new Car(nodes);
	return newcar;
}

//makes a random car
Car::Car(int n)
{
	nodes = n;
	balls_x = new int[nodes];
	balls_y = new int[nodes];
	balls = new int[nodes];
	links = new int*[nodes];
	for (int i = 0; i < nodes; i++) links[i] = new int[nodes];
	numlinks = 0;
	linkslist = new int[nodes*nodes];

	for (int i = 0; i < nodes; i++)//creates random ball location between 5-50;
	{
		balls_x[i] = randint(5, 50);
		balls_y[i] = randint(5, 50);
	}
	for (int i = 0; i < nodes; i++)//creates random links with a 33% chance between any two balls
	{
		for (int j = 0; j < nodes; j++)
		{
			links[i][j] = 0;
			if (i <= j) continue;
			if (randint(1, 3) == 1)
			{
				links[i][j] = 1;
			}
		}
	}
}

//graphically construct the car
void Car::constructCar()
{
	balls = new int[nodes];
	numlinks = 0;
	linkslist = new int[nodes*nodes];

	for (int i = 0; i < nodes; i++)
	{
		balls[i] = makeBall(balls_x[i], balls_y[i]);
	}
	for (int i = 0; i < nodes; i++)
	{
		for (int j = 0; j < nodes; j++)
		{
			if (links[i][j])
			{
				linkslist[numlinks++] = makeLink(i, j);
			}
		}
	}
}

//graphically destroy the car so that another car can be raced
void Car::deconstructCar()
{
	for (int i = 0; i < numlinks; i++)
		removeLink(linkslist[i] - i);
	for (int i = 0; i < nodes; i++)
		removeBall(balls[i] - i);

	delete balls;
	delete linkslist;
}

//returns average of all the positions of balls in order to return where the car is at
int Car::getCarPosition()
{
	int sum = 0;
	for (int i = 0; i < nodes; i++)
		sum += getBall(i)->position->x;
	return sum / nodes;
}

void Car::score(int itr, int pos)//save the score
{
	iterations = itr;
	position = pos;
}
