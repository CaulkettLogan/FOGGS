#pragma once
#define MUNCHIECOUNT 50
#define BONECOUNT 5
#define GHOSTCOUNT 1

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
	Vector2* deadPos;
	Texture2D* deadTexture;
	Rect* deadRect;
	float Speed;
	const int frame_time;
	player() : frame_time(250)
	{};
	int direction;
	int frame;
	int current_frame_time;
	int Tscore;
	bool dead;
};

struct Collectable
{
	//int Count;
	Rect* SourceRect;
	Vector2* Position;
	Texture2D* BlueTexture;
	int current_frame_time;
	int frame;
	int Score;
	
	//Texture2D* _munchieInvertedTexture;
};

struct Bone
{
	int frame;
	int current_frame_time;
	Rect* sourceRect;
	Texture2D* Texture;
	Vector2* Position;
	Rect* Rect;
	float speedIncrease;
	int waitTime;
};

struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};




class Player : public Game
{
private:

	player* _Player;
	Collectable* collectable;
	Collectable* _munchies[MUNCHIECOUNT];
	int nCount;
	Bone* bone;
	Bone* _powerUps[BONECOUNT];
	MovingEnemy* _ghosts[GHOSTCOUNT];
	
	int _frameCount;
	//data for bones

	
	const int cbone_buff_count;
	int Tscore;
	int BoneScore;
	
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


	//animation variables MUNCHIE

public:
	/// <summary> Constructs the Player class. </summary>
	Player(int argc, char* argv[]);

	void Input(int elapsedTime, Input::KeyboardState* state);

	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);

	void CheckViewportCollision();

	void UpdatePlayer(int elapsedTime, Bone*bone);

	void UpdateMunchie(int elapsedTime,Collectable* collectable, int nCount);

	void CheckStart(Input::KeyboardState* state, Input::Keys startKey);

	void Updatebones(int elapsedTime);

	void MouseUse(Input::MouseState* state, int elapsedTime);
	
	void CheckGhostCollision();
	void UpdateGhost(MovingEnemy*, int elapsedTime);
	bool MunchieCollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2, Collectable* collectable);
	bool BoneCollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2, Bone* bone, int elapsedTime, int cbone_buff_count);

	void ScoreInc(Collectable*collectable);
	void PlayerBuff(Bone* bone, int elapsedTime);
	/// <summary> Destroys any data associated with Player class. </summary>
	virtual ~Player();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime); //Score*score);
	
};