#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	audio.cleanUp();
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

	duckTransform.SetPos(glm::vec3(0.0f, -0.4f, -5.0f));
	duckTransform.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	ballTransform.SetPos(glm::vec3(4.0f, 0.0f, -5.5f));
	ballTransform.SetScale(glm::vec3(3.0f, 3.0f, 3.0f));

	buoyTransform.SetPos(glm::vec3(-0.5f, 0.0f, -5.5f));
	buoyTransform.SetScale(glm::vec3(3.0f, 3.0f, 3.0f));

	waterTransform.SetPos(glm::vec3(0.0f, -0.4f, -5.0f));
	waterTransform.SetScale(glm::vec3(25.0f, 1.0f, 18.0f));


	shader.init("..\\res\\shader.vert", "..\\res\\shader.frag"); //water shader
	ADS.init("..\\res\\ADS.vert", "..\\res\\ADS.frag"); //ADS shader
	skyboxShader.init("..\\res\\skybox.vert", "..\\res\\skybox.frag"); //skybox shader

	std::vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	skybox.init(faces);

	myCamera.initCamera(glm::vec3(0.0f, 10.0f, -17.0f), 70.0f, (float)_gameDisplay.getWidth() / _gameDisplay.getHeight(), 0.01f, 1000.0f);
	myCamera.setLook(glm::vec3(0.0f, 0.0f, -5.5f));
	counter = 0.0f;

	cameraYaw = -90.0f;
	cameraPitch = -25.0f;
	firstMouse = true;
	lastMouseX = 0.0f;
	lastMouseY = 0.0f;

	deltaTime = 0.0f;
	lastFrameTime = (float)SDL_GetTicks();
	cameraVelocity = glm::vec3(0.0f);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	audio.init();

	waterBuffer = audio.loadWav("..\\res\\water.wav");
	duckBuffer = audio.loadWav("..\\res\\duck.wav");

	waterSource = audio.createSource(waterBuffer, true);
	duckSource = audio.createSource(duckBuffer, false);

	audio.play(waterSource);

	duckWasAtCentre = false;

	centreMarkerMesh.loadModel("..\\res\\centreMarker.obj");
	centreMarkerTexture.init("..\\res\\whiteplastic.jpg");

	centreMarkerTransform.SetPos(glm::vec3(0.0f, -0.35f, -5.5f));
	centreMarkerTransform.SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

}

void MainGame::gameLoop()
{
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;

	while (_gameState != GameState::EXIT)
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (float)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());

		processInput();
		updateScene();
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	//const float moveSpeed = 6.0f * deltaTime;
	//const float rotateSpeed = 90.0f * deltaTime;
	const float mouseSensitivity = 0.08f;

	while (SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type == SDL_QUIT)
		{
			_gameState = GameState::EXIT;
		}

		if (evnt.type == SDL_KEYDOWN)
		{
			if (evnt.key.keysym.sym == SDLK_ESCAPE)
			{
				_gameState = GameState::EXIT;
			}
		}
		if (evnt.type == SDL_MOUSEMOTION)
		{
			float xOffset = (float)evnt.motion.xrel * mouseSensitivity;
			float yOffset = (float)-evnt.motion.yrel * mouseSensitivity;

			cameraYaw += xOffset;
			cameraPitch += yOffset;

			if (cameraPitch > 89.0f)
				cameraPitch = 89.0f;

			if (cameraPitch < -89.0f)
				cameraPitch = -89.0f;
		}

	}

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	glm::vec3 camPos = myCamera.getPos();
	glm::vec3 camForward = myCamera.getForward();
	glm::vec3 camUp = myCamera.getUp();
	//glm::vec3 camRight = glm::normalize(glm::cross(camForward, camUp));

	glm::vec3 flatForward = glm::normalize(glm::vec3(camForward.x, 0.0f, camForward.z));
	glm::vec3 flatRight = glm::normalize(glm::cross(flatForward, camUp));

	glm::vec3 targetVelocity(0.0f);

	float speed = 6.0f;

	if (keys[SDL_SCANCODE_W])
		targetVelocity += flatForward * speed;

	if (keys[SDL_SCANCODE_S])
		targetVelocity -= flatForward * speed;

	if (keys[SDL_SCANCODE_A])
		targetVelocity -= flatRight * speed;

	if (keys[SDL_SCANCODE_D])
		targetVelocity += flatRight * speed;

	if (keys[SDL_SCANCODE_SPACE])
		targetVelocity += camUp * speed;

	if (keys[SDL_SCANCODE_LCTRL])
		targetVelocity -= camUp * speed;

	if (keys[SDL_SCANCODE_Q])
		cameraYaw -= 90.0f * deltaTime;

	if (keys[SDL_SCANCODE_E])
		cameraYaw += 90.0f * deltaTime;

	float smoothing = 12.0f;
	cameraVelocity += (targetVelocity - cameraVelocity) * smoothing * deltaTime;

	camPos += cameraVelocity * deltaTime;


	glm::vec3 direction;
	direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	direction.y = sin(glm::radians(cameraPitch));
	direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));

	myCamera.setPos(camPos);
	myCamera.setForward(direction);

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

	float distToCentre = glm::distance(
		glm::vec3(duckX, 0.0f, duckZ),
		glm::vec3(centreX, 0.0f, centreZ)
	);

	bool duckAtCentre = distToCentre < 0.35f;

	if (duckAtCentre && !duckWasAtCentre)
	{
		audio.play(duckSource);
	}

	duckWasAtCentre = duckAtCentre;

	duckTransform.SetPos(glm::vec3(duckX, -0.4f, duckZ));

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

	// Water
	shader.Bind();

	glUniform1i(glGetUniformLocation(shader.ID(), "diffuse"), 0);
	shader.setFloat("time", counter);

	waterTexture.Bind(0);
	shader.Update(waterTransform, myCamera);
	waterMesh.draw();

	ADS.Bind();
	linkADS();
	glUniform1i(glGetUniformLocation(ADS.ID(), "diffuse"), 0);

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

	// Centre marker
	centreMarkerTexture.Bind(0);
	ADS.Update(centreMarkerTransform, myCamera);
	centreMarkerMesh.draw();

	//counter = counter + 0.01f;

	glDepthFunc(GL_LEQUAL);

	skyboxShader.Bind();

	glm::mat4 view = glm::mat4(glm::mat3(myCamera.getView()));
	glm::mat4 projection = myCamera.getProjection();

	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID(), "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniform1i(glGetUniformLocation(skyboxShader.ID(), "skybox"), 0);

	skybox.draw();

	glDepthFunc(GL_LESS);

	_gameDisplay.swapBuffer();
}