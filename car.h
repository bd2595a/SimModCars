//car object made of balls and links
class Car
{
public:
	//actually defines the car
	int* balls_x;//x coordinate of each ball
	int* balls_y;//y coordinate of each ball
	int** links;//matrix of which ball is connected to which ball

	//used in graphicially depicting the car race
	int* linkslist;//array of all the links in the car
	int* balls; //array of all the balls in the car

	//information about the car & how it raced
	int nodes; //number of balls
	int numlinks;//number of links
	int iterations;//how many frames
	int position;//how far the car got
	
	Car(int);//constructor that takes in how many nodes/balls to create a car
	void constructCar();//given ballsx, ballsy, and links, actually makes balls and links for the car
	void deconstructCar();//removes the car from the racetrack
	int getCarPosition();//tells us how far the car went
	void score(int, int);//sets iterations and positions at the end of the race

	//methods we write
	Car* breed(Car*);//breeds the car with another car, returning a child car
	Car* mutate();//mutates a current car, returning the mutant
	//destructor called in delete car
	~Car()
	{
		delete balls_x;
		delete balls_y;
		for (int i = 0; i < nodes; i++)
			delete links[i];
		delete links;
	}
};
