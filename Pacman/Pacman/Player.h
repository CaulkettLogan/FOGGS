#pragma once
#define MUNCHIECOUNT 50

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Player class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.

struct player
{
	Vector2* Position;
	Rect* SourceRect;
	Texture2D* Texture;
	const float Speed;
	const int frame_time;
	player() : Speed(0.3f), frame_time(250)
	{};
	int direction;
	int frame;
	int current_frame_time;
};

struct Collectable
{
	//int Count;
	Rect* Rect;
	Vector2* Position;
	Texture2D* BlueTexture;
	int current_frame_time;
	int frame;
	
	//Texture2D* _munchieInvertedTexture;
};


class Player : public Game
{
private:

	player* _Player;
	Collectable* collectable;
	Collectable* _munchies[MUNCHIECOUNT];
	
	
	
	
	int _frameCount;
	//data for bones
	Texture2D* bonesTexture;
	Vector2* bonesPosition;
	Rect* bonesRect;
	

	
	
	// Position for String
	Vector2* _stringPosition;

	//Data for Pause Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;
	bool _pKeyDown;
	
	//start screen
	Texture2D* start_background;
	Rect* start_rectangle;
	Vector2* start_string_position;
	bool space_key_down;
	bool start;

	int x1;
	int y1;
	int width1;
	int height1;
	int x2;
	int y2;
	int width2;
	int height2;
	
	//animation variables PLAYER

	
	//animation variables MUNCHIE
	const int cMunchie_Frame_Time;
	//int munchie_frame;
	
	Rect* _munchiesourceRect;
	
	//bones animation
	const int cbones_frame_time;
	int bones_frame;
	int bones_current_frame_time;
	Rect* _bonessourceRect;

	//animation variables MUNCHIE

public:
	/// <summary> Constructs the Player class. </summary>
	Player(int argc, char* argv[]);

	void Input(int elapsedTime, Input::KeyboardState* state);

	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);

	void CheckViewportCollision();

	void UpdatePlayer(int elapsedTime);

	void UpdateMunchie(int elapsedTime,Collectable* collectable);

	void CheckStart(Input::KeyboardState* state, Input::Keys startKey);

	void Updatebones(int elapsedTime);

	void MouseUse(Input::MouseState* state, int elapsedTime);

	bool CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

	/// <summary> Destroys any data associated with Player class. </summary>
	virtual ~Player();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};