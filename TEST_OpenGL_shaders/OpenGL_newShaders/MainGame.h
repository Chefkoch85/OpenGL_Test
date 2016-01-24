#pragma once

#include <SDL\SDL.h>
#include <GL/glew.h>

#include "GLSLProgram.h"
#include "GLTexture.h"

#include "Sprite.h"
#include <vector>

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
    MainGame();
    ~MainGame();

    void run();

private:
    void initSystems();
	void initRessources();
    void initShaders();
    void gameLoop();
    void processInput();
	void Move();
    void Draw();
	void releaseSystems();
	void releaseRessources();
	void calculateFPS();

    SDL_Window* _window;
    int _screenWidth;
    int _screenHeight;
    GameState _gameState;

    std::vector <Sprite*> _sprites;

    GLSLProgram _TextureColorProgram;

	std::string _Title;

    float _time;

	static const float MAX_FPS;
	float _fps;
	float _frameTime;
};

