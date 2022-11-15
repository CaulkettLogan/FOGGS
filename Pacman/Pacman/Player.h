#pragma once

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
class Player : public Game
{
private:
	// Data to represent Player
	Vector2* _playerPosition;
	Rect* _playerSourceRect;
	Texture2D* _playerTexture;

	// Data to represent Munchie
	int _frameCount;
	Rect* _munchieRect;
	Vector2* _munchiePosition;
	Texture2D* _munchieBlueTexture;
	Texture2D* _munchieInvertedTexture;
	const float _cPlayerSpeed;
	// Position for String
	Vector2* _stringPosition;

	//Data for Pause Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;
	bool _pKeyDown;
	
	Texture2D* start_background;
	Rect* start_rectangle;
	Vector2* start_string_position;
	bool space_key_down;
	bool start;
	
	//animation variables PLAYER
	int player_direction;
	int player_frame;
	int player_current_frame_time;
	const int cplayer_frame_time;
	//animation variables MUNCHIE
	const int cMunchie_Frame_Time;
	int munchie_frame;
	int munchie_current_frame_time;
	Rect* _munchiesourceRect;

	//animation variables MUNCHIE

public:
	/// <summary> Constructs the Player class. </summary>
	Player(int argc, char* argv[]);

	void Input(int elapsedTime, Input::KeyboardState* state);

	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);

	void CheckViewportCollision();

	void UpdatePlayer(int elapsedTime);

	void UpdateMunchie(int elapsedTime);
	void CheckStart(Input::KeyboardState* state, Input::Keys startKey);



	/// <summary> Destroys any data associated with Player class. </summary>
	virtual ~Player();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};