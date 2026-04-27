#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


MainGame::MainGame()
{
	_gameState = GameState::PLAY;
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::linkADS()
{
	// Define the light position
	glm::vec3 lightPos(20.0f, 20.0f, 20.0f);

	// Define the light color (white light)
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

	// Define the object color
	glm::vec3 objectColor(1.0f, 1.0f, 1.0f);

	// Set the light position uniform in your shader
	ADS.setVec3("lightPos", lightPos);

	// Set the light color uniform in your shader
	ADS.setVec3("lightColor", lightColor);

	// Set the object color uniform in your shader
	ADS.setVec3("objectColor", objectColor);

}


void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	//mesh1.init(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0])); //size calcuated by number of bytes of an array / no bytes of one element
	
	duckMesh.loadModel("..\\res\\duck.obj");

	ballMesh.loadModel("..\\res\\ball.obj");
	buoyMesh.loadModel("..\\res\\buoy.obj");

	duckTexture.init("..\\res\\bricks.jpg");
	ballTexture.init("..\\res\\Water.jpg");
	buoyTexture.init("..\\res\\bricks.jpg");

	duckTransform.SetPos(glm::vec3(0.0f, 0.0f, -5.0f));
	ballTransform.SetPos(glm::vec3(2.0f, 0.0f, -5.0f));
	buoyTransform.SetPos(glm::vec3(0.0f, -1.0f, -5.0f));
	buoyTransform.SetScale(glm::vec3(5.0f, 0.5f, 5.0f));


	shader.init("..\\res\\shader.vert", "..\\res\\shader.frag"); //new shader
	ADS.init("..\\res\\ADS.vert", "..\\res\\ADS.frag"); //new shader

	myCamera.initCamera(glm::vec3(0, 0, -30), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);
	counter = 0.0f;
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	glm::vec3 duckPos = *duckTransform.GetPos();
	glm::vec3 duckRot = *duckTransform.GetRot();
	glm::vec3 duckScale = *duckTransform.GetScale();

	if (keys[SDL_SCANCODE_A])
		duckPos.x -= 0.05f;

	if (keys[SDL_SCANCODE_D])
		duckPos.x += 0.05f;

	if (keys[SDL_SCANCODE_W])
		duckPos.z += 0.05f;

	if (keys[SDL_SCANCODE_S])
		duckPos.z -= 0.05f;

	if (keys[SDL_SCANCODE_Q])
		duckRot.y -= 0.03f;

	if (keys[SDL_SCANCODE_E])
		duckRot.y += 0.03f;

	if (keys[SDL_SCANCODE_Z])
		duckScale -= glm::vec3(0.01f);

	if (keys[SDL_SCANCODE_X])
		duckScale += glm::vec3(0.01f);

	duckTransform.SetPos(duckPos);
	duckTransform.SetRot(duckRot);
	duckTransform.SetScale(duckScale);
}


void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	ADS.Bind();
	linkADS();
	//shader.Bind();
	//shader.Update(transform, myCamera);
	
	//Duck
	duckTexture.Bind(0);
	ADS.Update(duckTransform, myCamera);
	duckMesh.draw();

	//Ball
	ballTexture.Bind(0);
	ADS.Update(ballTransform, myCamera);
	ballMesh.draw();

	//Buoy
	buoyTexture.Bind(0);
	ADS.Update(buoyTransform, myCamera);
	buoyMesh.draw();

	counter = counter + 0.01f;

	_gameDisplay.swapBuffer();
} 