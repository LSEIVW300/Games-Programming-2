#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
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

	// Define the object color (red object in this case)
	glm::vec3 objectColor(1.0f, 0.0f, 0.0f);

	// Set the light position uniform in your shader
	ADS.setVec3("lightPos", lightPos);

	// Set the light color uniform in your shader
	ADS.setVec3("lightColor", lightColor);

	// Set the object color uniform in your shader
	ADS.setVec3("objectColor", objectColor);

	glm::mat4 modelMatrix = transform.GetModel();

	// Set the model matrix uniform in your shader
	ADS.setMat4("model", modelMatrix);
}


void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	//mesh1.init(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0])); //size calcuated by number of bytes of an array / no bytes of one element
	mesh2.loadModel("..\\res\\monkey3.obj");
	
	texture.init("..\\res\\bricks.jpg"); //
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
	
}


void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);


	transform.SetPos(glm::vec3(0.0, 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 2, 0.0));
	transform.SetScale(glm::vec3(5.0, 5.0, 5.0));

	ADS.Bind();
	linkADS();
	ADS.Update(transform, myCamera);
	//shader.Bind();
	//shader.Update(transform, myCamera);
	texture.Bind(0);
	mesh2.draw();
	counter = counter + 0.01f;
				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 