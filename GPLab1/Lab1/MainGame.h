#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();
	void linkADS();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();

	Display _gameDisplay;
	GameState _gameState;
	
	Camera myCamera;
	Shader shader;
	Shader ADS;

	Mesh duckMesh;
	Mesh ballBlueMesh;
	Mesh ballYellowMesh;
	Mesh buoyRedMesh;
	Mesh buoyWhiteMesh;
	Mesh waterMesh;

	Texture duckTexture;
	Texture ballBlueTexture;
	Texture ballYellowTexture;
	Texture buoyRedTexture;
	Texture buoyWhiteTexture;
	Texture waterTexture;

	Transform duckTransform;
	Transform ballTransform;
	Transform buoyTransform;
	Transform waterTransform;


	float counter;


};

