#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>

class Skybox
{
	public:
		Skybox();
		~Skybox();

		void init(const std::vector<std::string>& faces);
		void draw();
		GLuint getTextureID();

	private:
		GLuint skyboxVAO;
		GLuint skyboxVBO;
		GLuint cubemapTexture;

		GLuint loadCubemap(const std::vector<std::string>& faces);

};