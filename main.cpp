//GENETIC CARS
//
//Template: Michael Black, 2014
//
//main.cpp does the following in order:
//	1) creates a population of cars
//	2) creates a racecourse
//	3) genetically improves the cars by racing them
//	4) creates the window
//	5) shows the winning cars

#include <iostream>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <math.h>
#include "main.h"
#include "physicsengine.h"
#include "car.h"

const int GENERATIONS = 20;			//how many breeding generations. TODO: don't forget to change
const int MAXCARS = 1000;			//maximum # of cars.  more than this will segfault
const int KILLMAX = 20;				//kill all but this many cars. AKA number of survivors
const int INITIAL_POPULATION = 50;	//how many cars we start with
const int SIMULATION_LENGTH = 2200; //how long a car can run before we give up and kill it
const int NUM_BALLS_IN_CAR = 10;	//how many balls a car is generated with
const int BREED_RATE = 35;			//chance a car will breed
const int MUTATE_RATE = 25;			//chance a car will mutate


int WIDTH = 500, HEIGHT = 500;		//screen width and height
QGraphicsScene* thescene;			//window component
WindowView* view;					//window
int timeunit = 1000 / 660;			//when we're actually showing the car, do a frame every this many milliseconds

Car* car[MAXCARS];					//cars are stored here
int CarCount = INITIAL_POPULATION;	//keeps track of how many valid cars there are
int currentCar;				//which car we're currently racing

int iterations;				//how many frames we have simulated so far

extern bool simulating;			//TRUE if actively racing a car, FALSE during setup
extern bool dontdographics;		//TRUE if no window is open, FALSE if we want to see the cars in the window

//sets up a timer for visualization
TimerHandler::TimerHandler(int t)
{
	QTimer* timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	timer->start(t);
}

//when a timer goes off, ready to show a frame
//this only happens where we're ready to see the winning cars
void TimerHandler::onTimer()
{
	if (!simulating) return;

	doFrame();

	int pos = car[currentCar]->getCarPosition();

	iterations++;

	if (iterations >= SIMULATION_LENGTH || pos >= WIDTH)//if we exceed the simulation length or go past it
	{
		qDebug() << iterations << " iterations, position=" << pos << endl;
		car[currentCar]->score(iterations, pos);
		car[currentCar]->deconstructCar();

		currentCar++;//proceed to the next car
		if (currentCar >= CarCount)
		{
			simulating = FALSE;
			for (int i = 0; i < CarCount; i++)
			{
				qDebug() << "Car " << i << ": itr: " << car[i]->iterations << ", pos: " << car[i]->position << endl;
			}
		}
		else
		{
			car[currentCar]->constructCar();//if not done, make the next car
		}
		iterations = 0;
	}
}

WindowView::WindowView(QGraphicsScene *scene, QWidget* parent) :QGraphicsView(scene, parent) { }

//if a car is racing, pause it by clicking
void WindowView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton)
	{
		simulating = !simulating; return;
	}
}

//if no cars are racing, start them by double clicking
void WindowView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		currentCar = 0;
		car[currentCar]->constructCar();
		simulating = TRUE;
	}
}

void kill()//TODO: sort cars by score, kill off all but KILLMAX best
//scoring mech: 
//	1. car that gets the furthest wins
//  2. if car dist > 500, lowest framecount wins
{
	//delete car on cars that were killed
	//sort cars by their score
	for (int i = 0; i < CarCount; i++)
	{
		for (int j = 0; j < CarCount - 1; j++)
		{
			//determine whose score is better
			if ((car[j + 1]->position > car[j]->position) || ((car[j + 1]->position == 500 && car[j]->position == 500) && (car[j+1]->iterations>200 && car[j + 1]->iterations > car[j]->iterations)))
			{
				Car* temp = car[j + 1];
				car[j + 1] = car[j];
				car[j] = temp;
			}
		}
	}

	//CarCount = KILLMAX;
	for (int i = KILLMAX; i < CarCount; i++)
	{
		delete car[i];
	}
	CarCount = KILLMAX;
}

void breed()//consider every pair of cars, roll random, if we decide to breed, breed them
{
	int breedingPop = CarCount;

	//	breed. For all cars, for all cars (excluding the original)
	for (int i = 0; i < breedingPop; i++)
	{
		for (int j = 0; j < breedingPop; j++)
		{
			if (i == j)
				continue;
			if (rand() % 100 < BREED_RATE)//20% chance to  breed
			{
				car[CarCount++] = car[i]->breed(car[j]);
			}
		}
	}
}

void mutate()//consider every car, roll random, clone if we decide to and mutate. add it. 
{
	int cloningCars = CarCount;
	for (int i = 0; i < cloningCars; i++)
	{
		if (rand() % 100 < MUTATE_RATE)//if this car gets mutated
		{
			car[CarCount++] = car[i]->mutate();
		}
	}
}

void doCars()//does all the racing, breeding...behind the scenes. when it's finished, you have your super cars that are ready to display
{
	dontdographics = TRUE;//no window, so don't make the cars visible (yet. they're not ready.)

	for (int gen = 0; gen < GENERATIONS; gen++) {//whole genetic process GENERATIONS times.
		qDebug() << "****** GENERATION " << gen << " **********" << endl;

		for (int i = 0; i < CarCount; i++)//for every car, race t and get its score
		{
			car[i]->constructCar();//make a car and put it into the physics engine

			simulating = TRUE;					//now the engine is allowed to simulate
			int t = 0;							//frame count
			int pos;							//how far the car has gotten
			for (t = 0; t < SIMULATION_LENGTH; t++)//run maximum amount offrames and give up if the car is outside/done
			{
				doFrame();//move everything
				pos = car[i]->getCarPosition();//save the position
				if (pos >= WIDTH) break;//if we made it to the finish line, stop simulating
			}

			//qDebug() << t << " iterations, position=" << pos << endl; //print out how the car did
			car[i]->score(t, pos);//save the score
			simulating = FALSE;//stop simulation
			car[i]->deconstructCar();//remove the car from the track
		}
		
		kill();//kill the losers
		for (int i = 0; i < KILLMAX-1; i++)//for all cars, print out their scores
		{
			qDebug() << "Car " << i << ": itr: " << car[i]->iterations << ", pos: " << car[i]->position << endl;
		}
		breed();//breed the remaining
		mutate();//mutate the remaining
	}
	kill();//call kill one more time
	dontdographics = FALSE;//make it so that when I construct cars now, they need to appear on the screen
}

extern Wall* walls[];
extern int WallCount;
void makeRaceCourse()
{
	WallCount = 9;

	walls[0] = new Wall(1, 500, 499, 500);
	walls[1] = new Wall(-20, 132, 123, 285);
	walls[2] = new Wall(104, 285, 203, 277);
	walls[3] = new Wall(202, 275, 271, 344);
	walls[4] = new Wall(271, 344, 320, 344);
	walls[5] = new Wall(321, 345, 354, 318);
	walls[6] = new Wall(354, 318, 394, 324);
	walls[7] = new Wall(394, 324, 429, 390);
	walls[8] = new Wall(429, 391, 498, 401);

	for (int i = 0; i < WallCount; i++)
		thescene->addItem(walls[i]);
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));//seed random number gen

	thescene = new QGraphicsScene();//make the window
	thescene->setSceneRect(0, 0, WIDTH, HEIGHT);

	makeRaceCourse();//make the course

	for (int i = 0; i < CarCount; i++)//make the initial population of cars
		car[i] = new Car(NUM_BALLS_IN_CAR);

	doCars();//race cars, including the population breeding/killing/mutating

	//displays the winners one at a time
	currentCar = 0;//show the first car
	car[currentCar]->constructCar();
	simulating = TRUE;

	//make the window
	view = new WindowView(thescene);
	view->setWindowTitle("Genetic Cars");
	view->resize(WIDTH + 50, HEIGHT + 50);
	view->show();
	view->setMouseTracking(true);

	TimerHandler timer(timeunit);//make the timer

	return app.exec();
}
