#pragma once
#include <glm/glm/glm.hpp>

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <functional>
#include <cstdio>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include "PhysicsUtility.h"

class TextureUtils {

public:

	static void GenerateTexture(std::string path, unsigned int& texture, bool includeAlpha) {
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			if (includeAlpha) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}

			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	// A weird float error neccessitates this function
	static glm::vec3 getRGB(glm::vec3 color) {
		return glm::vec3(color.x / 255, color.y / 255, color.z / 255);
	}

};