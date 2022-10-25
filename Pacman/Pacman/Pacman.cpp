#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.5f)
{
	_frameCount = 0;

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
	_playerTexture->Load("Textures/Pacman.tga", false);
	_playerPosition = new Vector2(350.0f, 350.0f);
	_playerSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Checks if D key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::D))
	{
		_playerPosition->X += _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
		_playerSourceRect->Y = 00.0f;


	}
	if (keyboardState->IsKeyDown(Input::Keys::W))
	{
		_playerPosition->Y -= _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
		_playerSourceRect->Y = 96.0f;


	}
	if (keyboardState->IsKeyDown(Input::Keys::S))
	{
		_playerPosition->Y += _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
		_playerSourceRect->Y = 32.0f;


	}
	if (keyboardState->IsKeyDown(Input::Keys::A))
	{
		_playerPosition->X -= _cPlayerSpeed * elapsedTime; //Moves Pacman across X axis
		_playerSourceRect->Y = 64.0f;


	}

	if (_playerPosition->X + _playerSourceRect->Width  > Graphics::GetViewportWidth())//right
	_playerPosition->X = 0;
	
	if (_playerPosition->X - _playerSourceRect->Width < Graphics::GetViewportWidth())//left
		_playerPosition->X = 1024; //Graphics::GetViewportWidth() - _playerSourceRect->Width;
	
	//if (_playerPosition->Y + _playerSourceRect->Width > Graphics::GetViewportHeight())//bottom
	//_playerPosition->Y = 768 - _playerSourceRect->Width;
	
	//if (_playerPosition->Y - _playerSourceRect->Width < Graphics::GetViewportHeight())//top
	//_playerPosition->Y = 32 + _playerSourceRect->Width;
	

}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Player X: " << _playerPosition->X << " Y: " << _playerPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_playerTexture, _playerPosition, _playerSourceRect); // Draws Pacman

	if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		_frameCount++;
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		_frameCount++;

		if (_frameCount >= 60)
			_frameCount = 0;
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}