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
	glm::vec3 lightPos(0.0f, 8.0f, -8.0f);

	// Define the light color (white light)
	glm::vec3 lightColor(2.0f, 2.0f, 2.0f);

	// Define the object color
	glm::vec3 objectColor(1.0f, 1.0f, 1.0f);

	// Set the light position uniform in your shader
	ADS.setVec3("lightPos", lightPos);

	// Set the light color uniform in your shader
	ADS.setVec3("lightColor", lightColor);

	// Set the object color uniform in your shader
	ADS.setVec3("objectColor", objectColor);

	ADS.setVec3("viewPos", myCamera.getPos());

}


void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	//mesh1.init(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0])); //size calcuated by number of bytes of an array / no bytes of one element
	
	duckMesh.loadModel("..\\res\\duck.obj");

	ballBlueMesh.loadModel("..\\res\\VolleyBallBlue.obj");
	ballYellowMesh.loadModel("..\\res\\VolleyBallYellow.obj");
	buoyRedMesh.loadModel("..\\res\\lifebuoyred.obj");
	buoyWhiteMesh.loadModel("..\\res\\lifebuoywhite.obj");
	waterMesh.loadModel("..\\res\\water.obj");

	duckTexture.init("..\\res\\yellow.jpg");
	ballBlueTexture.init("..\\res\\blue.jpg");
	ballYellowTexture.init("..\\res\\yellow.jpg");
	buoyRedTexture.init("..\\res\\red.jpg");
	buoyWhiteTexture.init("..\\res\\white.jpg");
	waterTexture.init("..\\res\\water.jpg");

	duckTransform.SetPos(glm::vec3(0.0f, 0.0f, -5.0f));
	duckTransform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	ballTransform.SetPos(glm::vec3(4.0f, 0.0f, -5.5f));
	ballTransform.SetScale(glm::vec3(3.0f, 3.0f, 3.0f));

	buoyTransform.SetPos(glm::vec3(-0.5f, 0.0f, -5.5f));
	buoyTransform.SetScale(glm::vec3(3.0f, 3.0f, 3.0f));

	waterTransform.SetPos(glm::vec3(0.0f, -0.4f, -5.0f));
	waterTransform.SetScale(glm::vec3(10.0f, 1.0f, 10.0f));


	shader.init("..\\res\\shader.vert", "..\\res\\shader.frag"); //new shader
	ADS.init("..\\res\\ADS.vert", "..\\res\\ADS.frag"); //new shader

	myCamera.initCamera(glm::vec3(0.0f, 9.0f, -14.0f), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);
	myCamera.setLook(glm::vec3(0.0f, 0.0f, -5.0f));
	counter = 0.0f;
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		updateScene();
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
	
	/*const Uint8* keys = SDL_GetKeyboardState(NULL);

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
	duckTransform.SetScale(duckScale); */
}

void MainGame::updateScene()
{
	counter += 0.005f;

	float t = counter;

	//Duck figure 8 path
	float duckX = sin(t) * 5.5f;
	float duckZ = -5.5f + sin(t * 2.0f) * 3.0f;

	duckTransform.SetPos(glm::vec3(duckX, 0.0f, duckZ));

	//Duck faces direction its travelling
	float nextX = sin(t + 0.01f) * 5.5f;
	float nextZ = -5.5f + sin((t + 0.01f) * 2.0f) * 3.0f;

	float dirX = nextX - duckX;
	float dirZ = nextZ - duckZ;

	float angle = atan2(dirX, dirZ);

	duckTransform.SetRot(glm::vec3(0.0f, angle, 0.0f));

	//objects drifting (makes it look like the water is pushing them)

	ballTransform.SetPos(glm::vec3(4.0f, sin(t * 1.5f) * 0.15f, -5.5f));
	ballTransform.SetRot(glm::vec3(sin(t * 1.4f) * 0.25f, 0.0f, -sin(t * 1.4f) * 0.25f));

	float buoyY = sin(t * 1.2f + 1.5f) * 0.12f;
	float buoyRot = sin(t * 1.4f) * 0.25f;

	buoyTransform.SetPos(glm::vec3(-2.5f, buoyY, -5.5f));
	buoyTransform.SetRot(glm::vec3(buoyRot, 0.0f, -buoyRot));
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	ADS.Bind();
	linkADS();

	glUniform1i(glGetUniformLocation(ADS.ID(), "diffuse"), 0);

	// Water
	waterTexture.Bind(0);
	ADS.Update(waterTransform, myCamera);
	waterMesh.draw();

	// Duck
	duckTexture.Bind(0);
	ADS.Update(duckTransform, myCamera);
	duckMesh.draw();

	// Ball blue part
	ballBlueTexture.Bind(0);
	ADS.Update(ballTransform, myCamera);
	ballBlueMesh.draw();

	// Ball yellow part
	ballYellowTexture.Bind(0);
	ADS.Update(ballTransform, myCamera);
	ballYellowMesh.draw();

	// Buoy red part
	buoyRedTexture.Bind(0);
	ADS.Update(buoyTransform, myCamera);
	buoyRedMesh.draw();

	// Buoy white part
	buoyWhiteTexture.Bind(0);
	ADS.Update(buoyTransform, myCamera);
	buoyWhiteMesh.draw();

	//counter = counter + 0.01f;

	_gameDisplay.swapBuffer();
}