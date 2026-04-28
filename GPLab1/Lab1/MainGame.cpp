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
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

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

	duckTexture.init("..\\res\\rubberduckyellow.jpg");
	ballBlueTexture.init("..\\res\\greenleather.jpg");
	ballYellowTexture.init("..\\res\\whiteleather.jpg");
	buoyRedTexture.init("..\\res\\redplastic.jpg");
	buoyWhiteTexture.init("..\\res\\whiteplastic.jpg");
	waterTexture.init("..\\res\\water1.jpg");

	duckTransform.SetPos(glm::vec3(0.0f, 0.0f, -5.0f));
	duckTransform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	ballTransform.SetPos(glm::vec3(4.0f, 0.0f, -5.5f));
	ballTransform.SetScale(glm::vec3(3.0f, 3.0f, 3.0f));

	buoyTransform.SetPos(glm::vec3(-0.5f, 0.0f, -5.5f));
	buoyTransform.SetScale(glm::vec3(3.0f, 3.0f, 3.0f));

	waterTransform.SetPos(glm::vec3(0.0f, -0.4f, -5.0f));
	waterTransform.SetScale(glm::vec3(25.0f, 1.0f, 18.0f));


	shader.init("..\\res\\shader.vert", "..\\res\\shader.frag"); //water shader
	ADS.init("..\\res\\ADS.vert", "..\\res\\ADS.frag"); //new shader

	myCamera.initCamera(glm::vec3(0.0f, 10.0f, -17.0f), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);
	myCamera.setLook(glm::vec3(0.0f, 0.0f, -5.5f));
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
	
}

void MainGame::updateScene()
{
	counter += 0.0035f;

	float t = counter;

	float centreX = 0.0f;
	float centreZ = -5.5f;

	float width = 7.5f;
	float depth = 4.8f;

	//Duck figure 8 path
	float duckX = centreX + sin(t) * width;
	float duckZ = centreZ + sin(t * 2.0f) * depth;

	duckTransform.SetPos(glm::vec3(duckX, 0.0f, duckZ));

	//Duck faces direction its travelling
	float nextX = centreX + sin(t + 0.02f) * width;
	float nextZ = centreZ + sin((t + 0.02f) * 2.0f) * depth;

	float dirX = nextX - duckX;
	float dirZ = nextZ - duckZ;

	float angle = atan2(dirX, dirZ);

	duckTransform.SetRot(glm::vec3(0.0f, angle, 0.0f));

	//objects drifting (makes it look like the water is pushing them)

	ballTransform.SetPos(glm::vec3(4.5f, sin(t * 1.5f) * 0.18f, -5.5f));
	ballTransform.SetRot(glm::vec3(sin(t) * 0.25f, 0.0f, cos(t) * 0.25f));

	

	buoyTransform.SetPos(glm::vec3(-4.5f, sin(t * 1.3f + 1.5f) * 0.15f, -5.5f));
	buoyTransform.SetRot(glm::vec3(sin(t * 1.4f) * 0.25f, 0.0f, -sin(t * 1.4f) * 0.25));
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	ADS.Bind();
	linkADS();

	glUniform1i(glGetUniformLocation(ADS.ID(), "diffuse"), 0);

	// Water
	ADS.setInt("isWater", 1);
	ADS.setFloat("time", counter);

	waterTexture.Bind(0);
	ADS.Update(waterTransform, myCamera);
	waterMesh.draw();

	// Duck
	ADS.setInt("isWater", 0);
	duckTexture.Bind(0);
	ADS.Update(duckTransform, myCamera);
	duckMesh.draw();

	// Ball blue part
	ADS.setInt("isWater", 0);
	ballBlueTexture.Bind(0);
	ADS.Update(ballTransform, myCamera);
	ballBlueMesh.draw();

	// Ball yellow part
	ADS.setInt("isWater", 0);
	ballYellowTexture.Bind(0);
	ADS.Update(ballTransform, myCamera);
	ballYellowMesh.draw();

	// Buoy red part
	ADS.setInt("isWater", 0);
	buoyRedTexture.Bind(0);
	ADS.Update(buoyTransform, myCamera);
	buoyRedMesh.draw();

	// Buoy white part
	ADS.setInt("isWater", 0);
	buoyWhiteTexture.Bind(0);
	ADS.Update(buoyTransform, myCamera);
	buoyWhiteMesh.draw();

	//counter = counter + 0.01f;

	_gameDisplay.swapBuffer();
}