#include "Player.h"
#include "time.h"
#include <sstream>
using namespace std;




Player::Player(int argc, char* argv[]) : Game(argc, argv), cbones_frame_time(500), cMunchie_Frame_Time(500)
{
	_frameCount = 0;
	
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Collectable();
		_munchies[i]->current_frame_time = 0;
		_munchies[i]->frame = rand() % 1;
		_munchies[i]->current_frame_time = rand() % 500 + 50;
		
	}
	
	//_munchies = new Collectable();
	
	//Start player
	_Player = new player();
	
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

		delete _munchies[nCount]->Rect;
		delete _munchies[nCount]->Position;
		delete _munchies[nCount];
	}
	delete[]_munchies;
}

void Player::LoadContent()
{
	// Load Player
	_Player->Texture = new Texture2D();
	_Player->Texture->Load("Textures/skeleton.png", false);
	_Player->Position = new Vector2(350.0f, 350.0f);
	_Player->SourceRect = new Rect(0.0f, 0.0f, 64, 64);

	// Load Munchie
	
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png",false);
	
	for  (int i = 0; i < MUNCHIECOUNT; i++)
	{
	
		_munchies[i]->BlueTexture = munchieTex;
		_munchies[i]->Rect = new Rect(0.0f, 0.0f, 12, 12);
		_munchies[i]->Position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}
	
	/*_munchies->BlueTexture = new Texture2D();
	_munchies->BlueTexture->Load("Textures/Munchie.png", false);
	_munchies->Rect = new Rect(0.0f, 0.0f, 12, 12);
	_munchies->Position = new Vector2(100.0f, 100.0f);*/
	//_munchieInvertedTexture = new Texture2D();
	//_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	
	//Load bones
	bonesTexture = new Texture2D();
	bonesTexture->Load("Textures/BONES.png",false);
	bonesRect = new Rect(0.0f, 0.0f, 39, 35);
	bonesPosition = new Vector2(200.0f, 200.0f);



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

			UpdatePlayer(elapsedTime);
			CheckViewportCollision();
			Updatebones(elapsedTime);

			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				UpdateMunchie(elapsedTime, _munchies[i]);
			}
	    }
		
		
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

}
bool Player::CollisionCheck(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int left1 = x1, left2 = x2;
	int right1 = x1 + width1, right2 = x2 + width2;
	int top1 = y1, top2 = y2;
	int bottom1 = y1 + height1, bottom2 = y2 + height2;

	if (bottom1 < top2)
		return false;
	if (top1 > bottom2)
		return false;
	if (right1 < left2)
		return false;
	if (left1 > right2)
		return false;
	
	return true;
	

}

void Player::Updatebones(int elapsedTime)
{

	bones_current_frame_time += elapsedTime;
	if (bones_current_frame_time > cbones_frame_time)
	{
		bones_frame++;

		if (bones_frame >= 2)
			bones_frame = 0;

		bones_current_frame_time = 0;

	}
	bonesRect->X = bonesRect->Width * bones_frame;
}

void Player::UpdateMunchie(int elapsedTime, Collectable* collectable)
{

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{


		collectable->current_frame_time += elapsedTime;
		if (collectable->current_frame_time > cMunchie_Frame_Time)
		{
			collectable->frame++;

			if (collectable->frame >= 2)
				collectable->frame = 0;

			collectable->current_frame_time = 0;

		}
		_munchies[i]->Rect->X = _munchies[i]->Rect->Width * collectable->frame;
		CollisionCheck(x1, y1, width1, height1, x2, y2, width2, height2);
			
	}
}

void Player::UpdatePlayer(int elapsedTime)
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

void Player::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	stringstream stream;


	stream << "Player X: " << _Player->Position->X << " Y: " << _Player->Position->Y;
	
	

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_Player->Texture, _Player->Position, _Player->SourceRect); // Draws Player


	for (int i = 0; i < MUNCHIECOUNT; i++)
	{


		if (_frameCount < 0)
		{
			// Draws Red Munchie
			SpriteBatch::Draw(_munchies[i]->BlueTexture, _munchies[i]->Position, _munchies[i]->Rect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			SpriteBatch::Draw(bonesTexture, bonesPosition, bonesRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			//SpriteBatch::Draw(_munchieBlueTexture, _playerPosition, _playerSourceRect);
			//_frameCount++;
		}
		else
		{
			// Draw Blue Munchie
			//SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			SpriteBatch::Draw(_munchies[i]->BlueTexture, _munchies[i]->Position, _munchies[i]->Rect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			SpriteBatch::Draw(bonesTexture, bonesPosition, bonesRect, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			//_frameCount++;

			if (_frameCount >= 60)
				_frameCount = 0;
		}
	}
	
	//START
	if (!start)
	{


		stringstream start_stream;
		start_stream << "START!";

		SpriteBatch::Draw(start_background, start_rectangle, nullptr);
		SpriteBatch::DrawString(start_stream.str().c_str(), start_string_position, Color::Red);
	}

	//draws bones


	
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