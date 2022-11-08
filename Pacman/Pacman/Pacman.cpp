#include "Pacman.h"

#include <sstream>
using namespace std;





Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.5f),cplayer_frame_time(250)
{
	_frameCount = 0;
	_paused = false;
	_pKeyDown = false;
	space_key_down = false;
	start = false;
	player_direction = 0;
	player_current_frame_time = 0;
	player_frame = 0;
	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _playerTexture;
	delete _playerSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_playerTexture = new Texture2D();
	_playerTexture->Load("Textures/skeleton.png", false);
	_playerPosition = new Vector2(350.0f, 350.0f);
	_playerSourceRect = new Rect(0.0f, 0.0f, 64, 64);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

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


void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	

	if (!start)
	{
		
		if (keyboardState->IsKeyDown(Input::Keys::SPACE))
		{
			space_key_down = true;
			start = !start;
		}
		if (keyboardState->IsKeyUp(Input::Keys::SPACE))
			space_key_down = false;
	}
	else
	{

		if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown)
		{
			_pKeyDown = true;
			_paused = !_paused;
		}

		if (keyboardState->IsKeyUp(Input::Keys::P))
			_pKeyDown = false;

		if (!_paused)
		{
			_frameCount++;
			// Checks if D key is pressed
			if ((keyboardState->IsKeyDown(Input::Keys::D)) || (keyboardState->IsKeyDown(Input::Keys::RIGHT))) //if D or right is pressed, the player moves to the right
			{
				_playerPosition->X += _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
				player_direction = 3;
			}
			else if ((keyboardState->IsKeyDown(Input::Keys::W)) || (keyboardState->IsKeyDown(Input::Keys::UP)))
			{
				_playerPosition->Y -= _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
				//_playerSourceRect->Y = 96.0f;
				player_direction = 0;
			}
			else if ((keyboardState->IsKeyDown(Input::Keys::S)) || (keyboardState->IsKeyDown(Input::Keys::DOWN)))
			{
				_playerPosition->Y += _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
				player_direction = 2;
			}
			else if ((keyboardState->IsKeyDown(Input::Keys::A)) || (keyboardState->IsKeyDown(Input::Keys::LEFT)))
			{
				_playerPosition->X -= _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
				//_playerSourceRect->Y = 64.0f;
				player_direction = 1;
			}

		

			player_current_frame_time += elapsedTime;
			_playerSourceRect->Y = _playerSourceRect->Height * player_direction;
			if (player_current_frame_time > cplayer_frame_time)
			{
				player_frame++;

				if (player_frame > 2)
					player_frame = 0;

				player_current_frame_time = 0;
			}
	    }
		_playerSourceRect->X = _playerSourceRect->Width * player_frame;

		if (_playerPosition->X + _playerSourceRect->Width >= Graphics::GetViewportWidth())//right
				_playerPosition->X = 0;

		if (_playerPosition->X < 0)//left
				_playerPosition->X = Graphics::GetViewportWidth() - _playerSourceRect->Width; //Graphics::GetViewportWidth() - _playerSourceRect->Width;

		if (_playerPosition->Y + _playerSourceRect->Height > Graphics::GetViewportHeight())//bottom
				_playerPosition->Y = Graphics::GetViewportHeight() - _playerSourceRect->Height;

		if (_playerPosition->Y < 0)//top
				_playerPosition->Y = 0;

	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	stringstream stream;


	stream << "Player X: " << _playerPosition->X << " Y: " << _playerPosition->Y;
	
	

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_playerTexture, _playerPosition, _playerSourceRect); // Draws Pacman

	if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		//_frameCount++;
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		_frameCount++;

		if (_frameCount >= 60)
			_frameCount = 0;
	}
	

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