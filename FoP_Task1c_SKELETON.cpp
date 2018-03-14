//---------------------------------------------------------------------------
//Program: Skeleton for Task 1c – group assignment
//Author: Pascale Vacher
//Last updated: 26 February 2018
//---------------------------------------------------------------------------

//Go to 'View > Task List' menu to open the 'Task List' pane listing the initial amendments needed to this program

//---------------------------------------------------------------------------
//----- include libraries
//---------------------------------------------------------------------------

//include standard libraries
#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>
using namespace std;

//include our own libraries
#include "RandomUtils.h"    //for Seed, Random
#include "ConsoleUtils.h"	//for Clrscr, Gotoxy, etc.
#include "TimeUtils.h"		//For time related functions

//---------------------------------------------------------------------------
//----- define constants
//---------------------------------------------------------------------------

//defining the size of the grid
const int  SIZEX(25);    	//horizontal dimension
const int  SIZEY(15);		//vertical dimension
//defining symbols used for display of the grid and content
const char SPOT('@');   	//spot
const char TUNNEL(' ');    	//tunnel
const char WALL('#');    	//border
const char HOLE('0');		//hole
const char PILL('*');		//pill
const char ZOMBIE('Z');		//Zombie
//defining the command letters to move the spot on the maze
const int  UP(72);			//up arrow
const int  DOWN(80); 		//down arrow
const int  RIGHT(77);		//right arrow
const int  LEFT(75);		//left arrow
//defining the other command letters
const char QUIT('Q');		//to end the game

struct Item {
	int x, y;
	char symbol;
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	system("color 8a");
	//function declarations (prototypes)
	void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Item& spot, Item Zoms[3]);
	void paintGame(const char g[][SIZEX], string mess, int Lives);
	bool wantsToQuit(const int key);
	void updateLives(int& currentLives, int Change);
	bool isArrowKey(const int k);
	int  getKeyPress();
	void updateGameData(char g[][SIZEX], Item& spot, const int key, string& mess, int& Lives);
	void updateGrid(char g[][SIZEX], char m[][SIZEX], const Item spot);
	void endProgram();

	//local variable declarations 
	char grid[SIZEY][SIZEX];			//grid for display
	char maze[SIZEY][SIZEX];			//structure of the maze
	Item spot = { 0, 0, SPOT }; 		//spot's position and symbol
	Item Zombies[3];
	string message("LET'S START...");	//current message to player
	int Lives(3);

	//action...
	Seed();								//seed the random number generator
	SetConsoleTitle("Spot and Zombies Game - FoP 2017-18");
	Zombies[0] = { 1, 1, ZOMBIE };
	Zombies[1] = { SIZEY - 2, 1, ZOMBIE };
	Zombies[2] = { 1, SIZEX - 2, ZOMBIE };
	Zombies[3] = { SIZEY - 2, SIZEX - 2, ZOMBIE };
	initialiseGame(grid, maze, spot, Zombies);	//initialise grid (incl. walls and spot)
	paintGame(grid, message, Lives);			//display game info, modified grid and messages
	int key;							//current key selected by player
	do {
		//TODO: command letters should not be case sensitive
		key = getKeyPress(); 	//read in  selected key: arrow or letter command
		if (isArrowKey(key))
		{
			if (Lives <= 0)
			{				
				key = 'Q';
			}
			else
			{
				updateGameData(grid, spot, key, message, Lives);		//move spot in that direction
				Gotoxy(40, 20);
				cout << grid[spot.y][spot.x];
				updateGrid(grid, maze, spot);					//update grid information
			}			
		}
		else
			message = "INVALID KEY!";	//set 'Invalid key' message
		paintGame(grid, message, Lives);		//display game info, modified grid and messages
	} while (!wantsToQuit(key));		//while user does not want to quit
	endProgram();						//display final message
	return 0;
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Item& spot, Item Zoms[3])
{ //initialise grid and place spot in middle
	void setInitialMazeStructure(char maze[][SIZEX], Item Zoms[3]);
	void setSpotInitialCoordinates(Item& spot);
	void updateGrid(char g[][SIZEX],char m[][SIZEX], Item b);

	setInitialMazeStructure(maze, Zoms);		//initialise maze
	setSpotInitialCoordinates(spot);
	updateGrid(grid, maze, spot);		//prepare grid
}

void setSpotInitialCoordinates(Item& spot)
{ //set spot coordinates inside the grid at random at beginning of game
//TODO: Spot should not spwan on inner walls
	spot.y = Random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
	spot.x = Random(SIZEX - 2);      //horizontal coordinate in range [1..(SIZEX - 2)]
}

void setInitialMazeStructure(char maze[][SIZEX], Item Zoms[3])
{ //set the position of the walls in the maze
//TODO: initial maze configuration should be amended (size changed and inner walls removed)
  //initialise maze configuration
	char initialMaze[SIZEY][SIZEX] 	//local array to store the maze structure
		= { { '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };		
	//with '#' for wall, ' ' for tunnel, etc. 
	//copy into maze structure with appropriate symbols
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (initialMaze[row][col])
			{
			//not a direct copy, in case the symbols used change
			case '#': maze[row][col] = WALL; break;
			case ' ': maze[row][col] = TUNNEL; break;
			}
	for (int count(0); count < 4; count++)
	{
		maze[Zoms[count].x][Zoms[count].y] = Zoms[count].symbol;
	}
	//Loop for each Hole that will be placed
	for (int i = 0; i < 12; i++)
	{
		int randomx = Random(SIZEX - 2);//get a random x value
		int randomy = Random(SIZEY - 2);//get a random y value
		while(maze[randomx][randomy] != TUNNEL)//Make sure the random X,y are over a tunnel and nothing else
		{
			randomx = Random(SIZEX - 2);
			randomy = Random(SIZEY - 2);
		}		
		maze[randomx][randomy] = HOLE; //when it is above a tunnel, replace with a hole
	}
	//Loop through for each pill
	for (int k = 0; k < 8; k++)
	{
		int randomx = Random(SIZEX - 2);//get a random x value
		int randomy = Random(SIZEY - 2);//get a random y value
		while (maze[randomx][randomy] != TUNNEL || maze[randomx][randomy] == HOLE)//Make sure the random X,y are over a tunnel and nothing else
		{
			randomx = Random(SIZEX - 2);
			randomy = Random(SIZEY - 2);
		}
		maze[randomx][randomy] = PILL; //when it is above a tunnel, replace with a hole
	}
}

//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------

void updateLives(int& currentLives, int Change)
{
	currentLives += Change;
}


void updateGrid(char grid[][SIZEX], char maze[][SIZEX], const Item spot)
{ //update grid configuration after each move
	void setMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item spot);
	void checkCollision(const Item spot, char maze[][SIZEX]);

	setMaze(grid, maze);	//reset the empty maze configuration into grid
	placeItem(grid, spot);	//set spot in grid
	checkCollision(spot, maze); //Check for collisions with zombies or pills
	//Set random 
}

void setMaze(char grid[][SIZEX], const char maze[][SIZEX])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void checkCollision(const Item spot, char grid[][SIZEX])
{ //Checks for collisions between zombies, pills and spot
	if (grid[spot.y][spot.x] == PILL) //If collided with pill
	{
		grid[spot.y][spot.x] = TUNNEL; //Replace with tunnel
	}
}

void placeItem(char g[][SIZEX], const Item item)
{ //place item at its new position in grid
	g[item.y][item.x] = item.symbol;
}

void placeChar(char g[][SIZEX], const char Symbol, int x, int y)
{ //place item at its new position in grid
	g[y][x] = Symbol;
}

//---------------------------------------------------------------------------
//----- move items on the grid
//---------------------------------------------------------------------------
void updateGameData(char g[][SIZEX], Item& spot, const int key, string& mess, int& Lives) //Changed g to not a constant, so we can update the space to a tunnel
{ //move spot in required direction
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy);
	void updateLives(int& currentLives, int Change);	
	void placeChar(char g[][SIZEX], const char Symbol, int x, int y);
	assert(isArrowKey(key));

	//reset message to blank
	mess = "                                         ";		//reset message to blank

	//calculate direction of movement for given key
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);

	//check new target position in grid and update game data (incl. spot coordinates) if move is possible
	switch (g[spot.y + dy][spot.x + dx])
	{			//...depending on what's on the target position in grid...
	case TUNNEL:		//can move
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		break;
	case WALL:  		//hit a wall and stay there
		mess = "CANNOT GO THERE!";
		break;
	case HOLE:
		mess = "SPOT FELL INTO A HOLE";
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		updateLives(Lives, -1);
		break;
	case PILL:
		mess = "SPOT ATE A PILL";
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		updateLives(Lives, 1);		
		break;
	}
}
//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy)
{ //calculate direction indicated by key
	bool isArrowKey(const int k);
	assert(isArrowKey(key));
	switch (key)	//...depending on the selected key...
	{
	case LEFT:  	//when LEFT arrow pressed...
		dx = -1;	//decrease the X coordinate
		dy = 0;
		break;
	case RIGHT: 	//when RIGHT arrow pressed...
		dx = +1;	//increase the X coordinate
		dy = 0;
		break;
	case UP:        //when UP arrow pressed...
		dx = 0;		
		dy = -1;	//Decrease the Y coordinate
		break;
	case DOWN:		//when the DOWN arrow pressed...
		dx = 0;
		dy = 1;		//Increase the Y coordinate
		break;
	}
}

int getKeyPress()
{ //get key or command (in uppercase) selected by user
  //KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224) 		//ignore symbol following cursor key
		keyPressed = _getch();
	return keyPressed;		//return it in uppercase 
}

bool isArrowKey(const int key)
{	//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
//TODO: UP and DOWN arrow keys should be detected
	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}
bool wantsToQuit(const int key)
{	//check if the user wants to quit (when key is 'Q' or 'q')
	return key == QUIT;
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

string tostring(int x)
{	//convert an integer to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
string tostring(char x) 
{	//convert a char to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message)
{	//display a string using specified colour at a given position 
	Gotoxy(x, y);
	SelectBackColour(backColour);
	SelectTextColour(textColour);
	cout << message;
}
void paintGame(const char g[][SIZEX], string mess, int Lives)
{ //display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void paintGrid(const char g[][SIZEX]);
	//display game title
	showMessage(clDarkGrey, clYellow, 0, 0, "___GAME___");
	showMessage(clGrey, clYellow, 40, 0, GetDate() + GetTime());

	//display menu options available
	showMessage(clWhite, clBlack, 40, 3, "TO MOVE USE KEYBOARD ARROWS ");
	showMessage(clWhite, clBlack, 40, 4, "TO QUIT ENTER 'Q'           ");

	//print auxiliary messages if any
	showMessage(clBlack, clWhite, 40, 8, mess);	//display current message

	//print current lives
	string s = to_string(Lives);
	showMessage(clBlack, clWhite, 40, 10, "Lives: " + s);

//TODO: Show your course, your group number and names on screen

	//display grid contents
	paintGrid(g);
}

void paintGrid(const char g[][SIZEX])
{ //display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col)
			switch (g[row][col])
			{
			case HOLE:
				SelectTextColour(clRed);
				cout << g[row][col];
				break;
			case PILL:
				SelectTextColour(clYellow);
				cout << g[row][col];
				break;
			case WALL:
				SelectTextColour(clWhite);
				cout << g[row][col];
				break;
			case TUNNEL:
				cout << g[row][col];
				break;
			case SPOT:
				SelectTextColour(clWhite);
				cout << g[row][col];
				break;
			case ZOMBIE:
				SelectTextColour(clGreen);
				cout << g[row][col];
				break;
			}
			//cout << g[row][col];	//output cell content
		cout << endl;
	}
}

void endProgram()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	showMessage(clRed, clYellow, 40, 8, "");	
	showMessage(clGrey, clRed, 40, 11, "GAME OVER");
	Gotoxy(40, 15);
	system("pause");	//hold output screen until a keyboard key is hit
}
