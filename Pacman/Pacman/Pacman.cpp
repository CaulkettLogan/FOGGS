#include "Player.h"
#include "time.h"
#include <sstream>
#include <thread>
#include <chrono>
#include <random>
#include <iostream>
using namespace std;
using namespace std::this_thread; //sleep_for, sleep_until
using namespace std::chrono_literals;//for units
using std::chrono::system_clock;





Player::Player(int argc, char* argv[]) : Game(argc, argv), cbones_frame_time(500), cMunchie_Frame_Time(500), cbone_buff_count(3000),cGhost_frame_time(500)
{
	_frameCount = 0;
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new MovingEnemy();
		_ghosts[i]->direction = 0;
		_ghosts[i]->speed = 0.1f;
		_ghosts[i]->current_frame_time = rand() % 500 + 50;
		_ghosts[i] -> frame = rand() % 1;
		
	}


	for (int i = 0; i < MUNCHIECOUNT; i++)
	{

		_munchies[i] = new Collectable();
		_munchies[i]->frame = rand() % 1;
		_munchies[i]->current_frame_time = rand() % 500 + 50;
		_munchies[i]->Position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}

	for (int i = 0; i < BONECOUNT; i++)
	{
		_powerUps[i] = new Bone();
		_powerUps[i]->current_frame_time = 0;
		_powerUps[i]->frame = rand() % 1;
		_powerUps[i]->current_frame_time = rand() % 500 + 50;
		_powerUps[i]->speedIncrease = 0.2;
		_powerUps[i]->waitTime = 2;
		_powerUps[i]->Position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));

	}

	//_munchies = new Collectable();

	//Start player
	_Player = new player();
	_Player->Tscore = 0;
	_Player->Speed = 0.2;
	_Player->dead = false;

	//sound
	nom = new SoundEffect();
	deadS = new SoundEffect();
	bonepickup = new SoundEffect();
	//Load sound




	// for PAUSE
	_paused = false;
	_pKeyDown = false;

	//FOR START
	space_key_down = false;
	start = false;

	//for Player animatoin
	_Player->direction = 0;
	_Player->current_frame_time = 0;
	_Player->frame = 0;

	//Munchie animation

	_frameCount = 0;

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Player", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Player::~Player()
{
	delete _Player->Texture;
	delete _Player->SourceRect;
	
	//delete _munchieInvertedTexture;
	
	delete _Player->Position;
	int nCount = 0;
	delete _munchies[0]->BlueTexture;
	for (int i = 0; nCount < MUNCHIECOUNT; i++)
	{

		delete _munchies[nCount]->SourceRect;
		delete _munchies[nCount]->Position;
		delete _munchies[nCount];
	}
	delete[]_munchies;
	
	int pCount = 0;
	delete _powerUps[0]->Texture;
	for (int i = 0; pCount < BONECOUNT; i++)
	{
		delete _powerUps[pCount]->Rect;
		delete _powerUps[pCount]->Position;
		delete _powerUps[pCount];
	}
	delete[] _powerUps;

	int gCount = 0;
	delete _ghosts[0]->texture;
	for (int i = 0; gCount < GHOSTCOUNT; i++)
	{
		delete _ghosts[gCount]->sourceRect;
		delete _ghosts[gCount]->position;
		delete _ghosts[gCount];
	}
	delete[] _ghosts;
	
	delete nom;
	delete deadS;
	delete bonepickup;
}

void Player::LoadContent()
{

	nom->Load("Sounds/eatbone.wav");
	deadS->Load("Sounds/deadsound.wav");
	bonepickup->Load("Sounds/happybone.wav");
	
	// Load Player
	_Player->Texture = new Texture2D();
	_Player->Texture->Load("Textures/skeleton.png", false);
	_Player->Position = new Vector2(350.0f, 350.0f);
	_Player->SourceRect = new Rect(0.0f, 0.0f, 64, 64);
	
	_Player->deadTexture = new Texture2D();
	_Player->deadTexture->Load("Textures/Death.png", false);
	_Player->deadPos = new Vector2(350.0f, 350.0f);
	_Player->deadRect = new Rect(0.0f, 0.0f, 30, 45);
	
	// Load Munchie
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png",false);
	
	for  (int i = 0; i < MUNCHIECOUNT; i++)
	{	
		_munchies[i]->BlueTexture = munchieTex;
		_munchies[i]->SourceRect = new Rect(0.0f, 0.0f, 12, 12);
		_munchies[i]->Position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}
	
	//Load Enemy
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->texture = new Texture2D();
		_ghosts[i]->texture->Load("Textures/GhostBlue.png", false);
		_ghosts[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	}
	

	//Load bones
	Texture2D* boneTex = new Texture2D();
	boneTex->Load("Textures/BONES.png", false);
	
	for (int i = 0; i < BONECOUNT; i++)
	{

		_powerUps[i]->Texture = boneTex;
		_powerUps[i]->Rect = new Rect(0.0f, 0.0f, 39, 35);
		_powerUps[i]->Position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}



	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	//set Menu Params
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	
	//start params
	start_background = new Texture2D();
	start_background->Load("Textures/Transparency.png",true);
	start_rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	start_string_position = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

}


void Player::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouse_state = Input::Mouse::GetState();
	

	if (!start)
	{
		CheckStart(keyboardState, Input::Keys::SPACE);
	}

	else
	{
		CheckPaused(keyboardState, Input::Keys::P);

		if (!_paused)
		{
			Input(elapsedTime, keyboardState);

			UpdatePlayer(elapsedTime, bone);
			UpdateGhost(_ghosts[0], elapsedTime);			
			CheckGhostCollision();
			CheckViewportCollision();
			Updatebones(elapsedTime,nom);


			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				UpdateMunchie(elapsedTime, _munchies[i], nCount);
			}
		}	
	}
}
void Player::UpdateGhost(MovingEnemy* ghost, int elapsedTime)
{
	
	
	for (int i = 0; i < GHOSTCOUNT; i++)
	{

		if (_ghosts[i]->direction == 0)
		{
			_ghosts[i]->position->X += _ghosts[i]->speed * elapsedTime;
		}
		else if (_ghosts[i]->direction == 1)
		{
			_ghosts[i]->position->X += _ghosts[i]->speed * elapsedTime;
		}
		if (_ghosts[i]->position->X + _ghosts[i]->sourceRect->Width >= Graphics::GetViewportWidth())
		{
			_ghosts[i]->direction = 1;
		}
		else if (_ghosts[i]->position->X <= 0)
		{
			_ghosts[i]->direction = 0;
		}
	}
	for (int i = 0; i < GHOSTCOUNT; i++)
	{


		_ghosts[i]->current_frame_time += elapsedTime;
		if (_ghosts[i]->current_frame_time > cGhost_frame_time)
		{
			_ghosts[i]->frame++;

			if (_ghosts[i]->frame >= 2)
				_ghosts[i]->frame = 0;

			_ghosts[i]->current_frame_time = 0;

		}
		_ghosts[i]->sourceRect->Y = _ghosts[i]->sourceRect->Height * _ghosts[i]->frame;
	}
}

/*void Player::MouseUse(Input::MouseState* state, int elapsedTime)
{
	if (mouseState(Input::ButtonState::LeftButton);
	{

	}


}*/

void Player::CheckViewportCollision()
{
	_Player->SourceRect->X = _Player->SourceRect->Width * _Player->frame;

	if (_Player->Position->X + _Player->SourceRect->Width >= Graphics::GetViewportWidth())//right
		_Player->Position->X = 0;

	if (_Player->Position->X < 0)//left
		_Player->Position->X = Graphics::GetViewportWidth() - _Player->SourceRect->Width; //Graphics::GetViewportWidth() - _playerSourceRect->Width;

	if (_Player->Position->Y + _Player->SourceRect->Height > Graphics::GetViewportHeight())//bottom
		_Player->Position->Y = Graphics::GetViewportHeight() - _Player->SourceRect->Height;

	if (_Player->Position->Y < 0)//top
		_Player->Position->Y = 0;

	//Ghos
	for (int i = 0; i < GHOSTCOUNT; i++)
	{


		if (_ghosts[i]->position->X + _ghosts[i]->sourceRect->Width >= Graphics::GetViewportWidth())//right
			_ghosts[i]->position->X = 0;

		if (_ghosts[i]->position->X < 0)//left
			_ghosts[i]->position->X = Graphics::GetViewportWidth() - _ghosts[i]->sourceRect->Width; //Graphics::GetViewportWidth() - _playerSourceRect->Width;
	}





}
bool Player::MunchieCollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2, Collectable* collectable)
{

		int left1 = _Player->Position->X;
		int left2 = collectable->Position->X;//collision for left side

		int right1 = _Player->Position->X + _Player->SourceRect->Width;
		int right2 = collectable->Position->X + collectable->SourceRect->Width;//collision for the right side

		int top1 = _Player->Position->Y;
		int top2 = collectable->Position->Y;//collision for the right

		int bottom1 = _Player->Position->Y + _Player->SourceRect->Height;
		int bottom2 = collectable->Position->Y + collectable->SourceRect->Height;//collision for the bottom

		//ways collision could happen
		if (bottom1 < top2)
			return false; // if it returns false, collision made
		if (top1 > bottom2)
			return false;
		if (right1 < left2)
			return false;
		if (left1 > right2)
			return false;//
		  collectable->Position = new Vector2(1000.0f, 1000.0f); //to check if collision works
		  _Player->Speed = _Player->Speed + 0.01f;
		  Audio::Play(nom);
		return true;


}

/*void timer(player* _Player, Bone* bone)
{
	_Player->Speed = _Player->Speed + bone->speedIncrease;
	Sleep(3000);
	_Player->Speed = 0.2;
}*/

bool Player::BoneCollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2, Bone* bone, int elapsedTime,int cbone_buff_count)
{

	int left1 = _Player->Position->X;
	int left2 = bone->Position->X;//collision for left side

	int right1 = _Player->Position->X + _Player->SourceRect->Width;
	int right2 = bone->Position->X + bone->Rect->Width;//collision for the right side

	int top1 = _Player->Position->Y;
	int top2 = bone->Position->Y;//collision for the right

	int bottom1 = _Player->Position->Y + _Player->SourceRect->Height;
	int bottom2 = bone->Position->Y + bone->Rect->Height;//collision for the bottom

	//ways collision could happen
	if (bottom1 < top2)
		return false; // if it returns false, collision made
	if (top1 > bottom2)
		return false;
	if (right1 < left2)
		return false;
	if (left1 > right2)
		return false;//
	//_Player->Speed = _Player->Speed + bone->speedIncrease;
	bone->Position = new Vector2(500.0f, 1000.0f); //to check if collision works
	_Player->Speed = _Player->Speed + bone->speedIncrease;
	Audio::Play(bonepickup);
	//thread timer1(timer, _Player, bone);
	
//	timer1.join();
	

	
	return true;
}




void Player::Updatebones(int elapsedTime, SoundEffect* nom)
{
	//update bones every frame
	for (int i = 0; i < BONECOUNT; i++)
	{


		_powerUps[i]->current_frame_time += elapsedTime;
		if (_powerUps[i]->current_frame_time > cbones_frame_time)
		{
			_powerUps[i]->frame++;

			if (_powerUps[i]->frame >= 2)
				_powerUps[i]->frame = 0;

			_powerUps[i]->current_frame_time = 0;

		}
		_powerUps[i]->Rect->X = _powerUps[i]->Rect->Width * _powerUps[i]->frame;

		
			if (BoneCollisionCheck(x1, y1, width1, height1, x2, y2, width2, height2, _powerUps[i], elapsedTime,cbone_buff_count));
			{
				

			}
		
	}
}

void Player::UpdateMunchie(int elapsedTime, Collectable* collectable, int nCount)
{

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{


		_munchies[i]->current_frame_time += elapsedTime;
		if (_munchies[i]->current_frame_time > cMunchie_Frame_Time)
		{
			_munchies[i]->frame++;

			if (_munchies[i]->frame >= 2)
				_munchies[i]->frame = 0;

			_munchies[i]->current_frame_time = 0;

		}
		_munchies[i]->SourceRect->X = _munchies[i]->SourceRect->Width * _munchies[i]->frame;

		if (MunchieCollisionCheck(x1, y1, width1, height1, x2, y2, width2, height2, _munchies[i]))
		{
			ScoreInc(collectable);
		}

	
	}
}
void Player::CheckGhostCollision()
{
	int i = 0;
	int bottom1 = _Player->Position->Y + _Player->SourceRect->Height;
	int bottom2 = 0;
	int left1 = _Player->Position->X;
	int left2 = 0;
	int right1 = _Player->Position->X + _Player->SourceRect->Width;
	int right2 = 0;
	int top1 = _Player->Position->Y;
	int top2 = 0;

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		bottom2 = _ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height;
		left2 = _ghosts[i]->position->X;
		right2 = _ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;
		top2 = _ghosts[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_Player->dead = true;
			i = GHOSTCOUNT;
			Audio::Play(deadS);
		}
	}

}


void Player::UpdatePlayer(int elapsedTime,Bone*bone)
{
	_Player->current_frame_time += elapsedTime;
	_Player->SourceRect->Y = _Player->SourceRect->Height * _Player->direction;
	if (_Player->current_frame_time > _Player->frame_time)
	{
		_Player->frame++;

		if (_Player->frame > 3)
			_Player->frame = 0;

		_Player->current_frame_time = 0;
	}

	
}


void Player::ScoreInc(Collectable*collectable)
{
	collectable->Score = 2;

	_Player->Tscore = _Player->Tscore + collectable->Score;

}	

void Player::PlayerBuff(Bone* bone, int elapsedTime)
{
	for (int i = 0; i < BONECOUNT; i++)
	{
		if (BoneCollisionCheck(x1, y1, width1, height1, x2, y2, width2, height2, _powerUps[i], elapsedTime,cbone_buff_count));
		{
			_Player->Speed = _Player->Speed * _powerUps[i]->speedIncrease;

		}
	}
}

void Player::CheckStart(Input::KeyboardState* state, Input::Keys startKey)
{
	if (state->IsKeyDown(Input::Keys::SPACE))
	{
		space_key_down = true;
		start = !start;
	}
	if (state->IsKeyUp(Input::Keys::SPACE))
		space_key_down = false;
}

void Player::CheckPaused(Input::KeyboardState* state, Input::Keys P)
{


	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}

	if (state->IsKeyUp(Input::Keys::P))
		_pKeyDown = false;
}

void Player::Input(int elapsedTime, Input::KeyboardState* state)
{
	_frameCount++;
	// Checks if D key is pressed
	if ((state->IsKeyDown(Input::Keys::D)) || (state->IsKeyDown(Input::Keys::RIGHT))) //if D or right is pressed, the player moves to the right
	{
		_Player->Position->X += _Player->Speed * elapsedTime; //Moves Player across X axis
		_Player->direction = 3;
	}
	else if ((state->IsKeyDown(Input::Keys::W)) || (state->IsKeyDown(Input::Keys::UP)))
	{
		_Player->Position->Y -= _Player->Speed * elapsedTime; //Moves Player across X axis
		//_playerSourceRect->Y = 96.0f;
		_Player->direction = 0;
	}
	else if ((state->IsKeyDown(Input::Keys::S)) || (state->IsKeyDown(Input::Keys::DOWN)))
	{
		_Player->Position->Y += _Player->Speed * elapsedTime; //Moves Player across X axis
		_Player->direction = 2;
	}
	else if ((state->IsKeyDown(Input::Keys::A)) || (state->IsKeyDown(Input::Keys::LEFT)))
	{
		_Player->Position->X -= _Player->Speed * elapsedTime; //Moves Player across X axis
		//_playerSourceRect->Y = 64.0f;
		_Player->direction = 1;

	}
}

void Player::Draw(int elapsedTime)//)
{
	// Allows us to easily create a string
	stringstream stream;


	stream << "Player X: " << _Player->Position->X << " Y: " << _Player->Position->Y<<endl;
	
	stream << " Score:" << _Player->Tscore << "    speed: " << _Player->Speed;
	
	

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_Player->Texture, _Player->Position, _Player->SourceRect); // Draws Player
	


	for (int i = 0; i < MUNCHIECOUNT; i++)
	{


		if (_frameCount < 0)
		{
			// Draws Red Munchie
			SpriteBatch::Draw(_munchies[i]->BlueTexture, _munchies[i]->Position, _munchies[i]->SourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			//_frameCount++;
		}
		else
		{
			// Draw Blue Munchie
			SpriteBatch::Draw(_munchies[i]->BlueTexture, _munchies[i]->Position, _munchies[i]->SourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);	
			//_frameCount++;
			if (_frameCount >= 60)
				_frameCount = 0;
		}
	}
	//DRAW GHOST
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect);
	}


	for (int i = 0; i < BONECOUNT; i++)
	{


		if (_frameCount < 0)
		{
			SpriteBatch::Draw(_powerUps[i]->Texture, _powerUps[i]->Position, _powerUps[i]->Rect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

			//_frameCount++;
		}
		else
		{
			SpriteBatch::Draw(_powerUps[i]->Texture, _powerUps[i]->Position, _powerUps[i]->Rect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			//_frameCount++;
			if (_frameCount >= 60)
				_frameCount = 0;
		}
	}
	
	for (int i = 0; i < GHOSTCOUNT; i++)
	{


		if (_frameCount < 0)
		{
			SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

			//_frameCount++;
		}
		else
		{
			SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			//_frameCount++;
			if (_frameCount >= 60)
				_frameCount = 0;
		}
	}
	
	if (_Player->dead)
	{
		_Player->Texture = _Player->deadTexture;
		_Player->SourceRect = _Player->deadRect;
		_Player->Speed = 0;

		

		stringstream menu_stream;
		menu_stream << "YOU DIED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menu_stream.str().c_str(), _menuStringPosition, Color::Red);
		//SpriteBatch::Draw(_Player->deadTexture,_Player->deadPos, _Player->deadRect);

		
	}
	//START
	if (!start)
	{


		stringstream start_stream;
		start_stream << "START!";

		SpriteBatch::Draw(start_background, start_rectangle, nullptr);
		SpriteBatch::DrawString(start_stream.str().c_str(), start_string_position, Color::Red);
	}




	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	if (_paused)
	{
		stringstream menu_stream;
		menu_stream << "PAUSED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menu_stream.str().c_str(), _menuStringPosition, Color::Red);
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}