#include "car.h"
#include "physicsengine.h"
#include "ball.h"
#include <QtGui>

const int MUTATION_RATE = 3;		//how likely each component in a car will be mutated

//random number between [a,b] 
int randint(int a, int b)
{
	return (qrand() % (b - a + 1)) + a;
}

Car* Car::mutate()
{
	Car* mutant = new Car(nodes);
	for (int j = 0; j < nodes; j++)//make our mutant a clone
	{
		mutant->balls_x[j] = balls_x[j];
		mutant->balls_y[j] = balls_y[j];
	}
	for (int j = 0; j < nodes; j++)//mutate balls
	{
		if (rand() % 100 < MUTATION_RATE)//if this ball gets mutated
		{
			mutant->balls_x[j] = rand() % (45) + 5;//random number between 5 and 50
			mutant->balls_y[j] = rand() % (45) + 5;//random number between 5 and 50
		}
	}
	for (int j = 0; j < nodes; j++)//mutate links
	{
		for (int k = 0; k < nodes; k++)
		{
			if (j <= k) continue;
			if (rand() % 100 < MUTATION_RATE)//if this link gets mutated
			{
				mutant->links[j][k] = mutant->links[j][k] == 1 ? 0 : 1;//if it has a 1, make it a 0 and vice versa
			}
		}
	}

	return mutant;
}

Car* Car::breed(Car* c)
{
	int twistPoint = rand() % nodes;
	Car* baby = new Car(nodes);
	for (int k = 0; k < nodes; k++)//breeds the balls in the car
	{
		if (k < twistPoint)
		{
			baby->balls_x[k] = balls_x[k];
			baby->balls_y[k] = balls_y[k];
		}
		else
		{
			baby->balls_x[k] = c->balls_x[k];
			baby->balls_y[k] = c->balls_y[k];
		}
	}
	twistPoint = rand() % nodes;
	{
		for (int k = 0; k < nodes; k++)//breeding the links
		{
			for (int g = 0; g < nodes; g++)
			{
				if (k <= g) continue;
				if (k < twistPoint)
				{
					baby->links[k][g] = links[k][g];
				}
				else
				{
					baby->links[k][g] = c->links[k][g];
				}
			}
		}
	}
	return baby;
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
