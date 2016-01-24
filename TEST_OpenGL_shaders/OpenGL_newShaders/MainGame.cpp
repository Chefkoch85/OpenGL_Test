#include "MainGame.h"
#include "Errors.h"

#include <iostream>
#include <string>

#include "OpenGLUtility.h"

#include <Windows.h>

const float MainGame::MAX_FPS = 60;

//Constructor, just initializes private member variables
MainGame::MainGame() : 
    _screenWidth(800),
    _screenHeight(600), 
    _time(0.0f),
    _window(nullptr), 
    _gameState(GameState::PLAY),
	_fps(0.0f),
	_frameTime(0.0f),
	_Title("Game-Engine")
{

}

//Destructor
MainGame::~MainGame()
{
}

//This runs the game
void MainGame::run() 
{
    initSystems();
	initRessources();
  
    //This only returns when the game ends
    gameLoop();

	releaseRessources();
	releaseSystems();
}

//Initialize SDL and Opengl and whatever else we need
void MainGame::initSystems()
{
    //Initialize SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    //Open an SDL window
    _window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
    if (_window == nullptr) {
        fatalError("SDL Window could not be created!");
    }

    //Set up our OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(_window);
    if (glContext == nullptr) {
        fatalError("SDL_GL context could not be created!");
    }

    //Set up glew (optional but recommended)
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        fatalError("Could not initialize glew!");
    }

    //Tell SDL that we want a double buffered window so we dont get
    //any flickering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(0);

	//for (int i = 0; i < 256; i++)
	//{
	//	printf("%X : %c\n", i, i);
	//}
	//Cout << Cnl;

	// show the OpenGL version
	CStr temp = _Title + " : debug info";
	SetConsoleTitleA(temp.c_str());
	Cout << "\xC9";
	for (int i = 0; i < 78; i++)
		Cout << "\xCD";
	Cout << "\xBB";

	Cout << "\xBA Your Maschine:" << Cnl;
	Cout << "\xBA OpenGL-Version:\t" << glGetString(GL_VERSION) << Cnl;
	Cout << "\xBA GLSL-Shader Version:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << Cnl;
	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 79,1 });
	Cout << "\xBA";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 79,2 });
	Cout << "\xBA";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 79,3 });
	Cout << "\xBA";

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,4 });
	Cout << "\xCC";
	for (int i = 0; i < 78; i++)
		Cout << "\xCD";
	Cout << "\xB9";

	Cout << "\xBA Reqiured:" << Cnl;
	Cout << "\xBA OpenGL-Version:\t" << "4.5.0" << Cnl;
	Cout << "\xBA GLSL-Shader-Version:\t" << "4.50" << Cnl;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 79,5 });
	Cout << "\xBA";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 79,6 });
	Cout << "\xBA";
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 79,7 });
	Cout << "\xBA";

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,8});
	Cout << "\xC8";
	for (int i = 0; i < 78; i++)
		Cout << "\xCD";
	Cout << "\xBC";


	// activate Alphablending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Set the background color to blue
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    initShaders();
}

void MainGame::initRessources()
{
	//Initialize our sprites. (temporary)
	_sprites.push_back(new Sprite());
	_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "../res/Textures/player/right_stand.png");

	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.0f, 0.0f, .2f, .2f, "../res/Textures/player/right_stand.png");
	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.2f, 0.0f, .2f, .2f, "../res/Textures/player/right_stand.png");
	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.4f, 0.2f, .2f, .2f, "../res/Textures/player/right_stand.png");
	_sprites.push_back(new Sprite());
	_sprites.back()->init(0.6f, 0.4f, .2f, .2f, "../res/Textures/player/right_stand.png");

}

void MainGame::initShaders() 
{
	_TextureColorProgram.compileShaders("../res/Shader/TextureColorShading.vs", "../res/Shader/TextureColorShading.fs");
	_TextureColorProgram.addAttribute("vertexPosition");
	_TextureColorProgram.addAttribute("vertexColor");
	_TextureColorProgram.addAttribute("vertexUV");
	_TextureColorProgram.linkShaders();
}

//This is the main game loop for our program
void MainGame::gameLoop()
{

    //Will loop until we set _gameState to EXIT
    while (_gameState != GameState::EXIT)
	{
		float startTicks = SDL_GetTicks();

        processInput();
		Move();
        Draw();
		calculateFPS();

		float frameTime = SDL_GetTicks() - startTicks;

		// delay frame if it is to fast
		if (1000.0f / MAX_FPS > frameTime)
		{
			SDL_Delay(1000.0f / MAX_FPS - frameTime);
		}
    }
}

//Processes input with SDL
void MainGame::processInput()
{
    SDL_Event evnt;

    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                _gameState = GameState::EXIT;
                break;

			case SDL_KEYUP:
				_gameState = GameState::EXIT;
				break;

            case SDL_MOUSEMOTION:
                //std::cout << evnt.motion.x << " " << evnt.motion.y << std::endl;
                break;
        }
    }
}

void MainGame::Move()
{
	_time += 0.05;
}

//Draws the game using the almighty OpenGL
void MainGame::Draw() 
{
    //Set the base depth to 1.0
    glClearDepth(1.0);
    //Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Enable the shader
	_TextureColorProgram.use();

    //We are using texture unit 0
    glActiveTexture(GL_TEXTURE0);
    //Get the uniform location
    GLint textureLocation = _TextureColorProgram.getUniformLocation("mySampler");
    //Tell the shader that the texture is in texture unit 0
    glUniform1i(textureLocation, 0);

    //Set the constantly changing time variable
    GLint timeLocation = _TextureColorProgram.getUniformLocation("time");
    glUniform1f(timeLocation, _time);

    //Draw our sprite!
    for (int i = 0; i < _sprites.size(); i++) {
        _sprites[i]->draw();
    }

    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //disable the shader
	_TextureColorProgram.unuse();

    //Swap our buffer and draw everything to the screen!
    SDL_GL_SwapWindow(_window);
}

void MainGame::releaseSystems()
{
	SDL_Quit();
}

void MainGame::releaseRessources()
{
	for each (auto var in _sprites)
	{
		if (var)
		{
			delete var;
			var = nullptr;
		}
	}
	_sprites.clear();
}

void MainGame::calculateFPS()
{
	static const int NUM_SAMPLES = 100;
	static float frameTimes[NUM_SAMPLES];
	static int frames = 0;
	static float prevTicks = SDL_GetTicks();

	float currentTicks = SDL_GetTicks();

	_frameTime = currentTicks - prevTicks;
	prevTicks = currentTicks;

	static int sample = 0;
	frameTimes[sample++] = _frameTime;
	sample %= NUM_SAMPLES;

	if (frames < NUM_SAMPLES)
		frames = sample;
	else
		frames = NUM_SAMPLES;

	float averageFrameTime = 0.0f;
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		averageFrameTime += frameTimes[i];
	}

	averageFrameTime /= frames;

	if (averageFrameTime > 0.0f)
	{
		_fps = 1000.0f / averageFrameTime;
	}

	if (frames == NUM_SAMPLES - 1)
	{
		CStr temp = _Title + " : " + std::to_string(_fps);
		SDL_SetWindowTitle(_window, temp.c_str());
	}
}
