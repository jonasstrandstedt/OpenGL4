#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "Geometry.h"



namespace gl4
{
	class Model: public Geometry 
	{
	public:
		Model(std::string filename, std::string texturename, glm::vec3 position = glm::vec3(0.0,0.0,0.0), float scale = 1.0, glm::vec3 rotation = glm::vec3(0.0,0.0,0.0));
	private:
		void loadObj(const char *filename);

	};
}

#endif