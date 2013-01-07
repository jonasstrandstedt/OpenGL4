/**
Copyright (C) 2012-2014 Jonas Strandstedt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "Engine.h"
#include "Sphere.h"

gl4::VBO *obj;
gl4::Sphere *sphere;
gl4::Sphere *sphere_notess;
gl4::Engine *engine;
glm::vec2 angle;

GLuint textureID[2];

int tess;

void keyboardCallback(int key, int state);
void myRenderFunc(void);
void myInitFunc(void);
void myUpdateFunc(float dt);

void loadTexture(const char *filename, GLuint texID) {
  
  GLFWimage img;

  if(!glfwReadImage(filename, &img, GLFW_NO_RESCALE_BIT))
  	std::cerr << "Failed to load texture from TGA file." << std::endl;
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture( GL_TEXTURE_2D, texID );

  glfwLoadTextureImage2D( &img, 0 );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
  glfwFreeImage(&img); // Clean up the malloc()'ed data pointer
}

int main(int argc, char **argv) {

	// create engine object
	engine = new gl4::Engine(argc, argv);
	
	// set callbacks
	engine->setKeyBoardCallbackfunc(keyboardCallback);
	engine->setUpdateFunc(myUpdateFunc);
	engine->setRenderFunc(myRenderFunc);
	engine->setInitFunc(myInitFunc);
	
	// init
	if( ! engine->initGL()) {
		return 1;
	}

	// do render
	engine->render();

	// cleanup
	delete obj;
	delete engine;

	// return success
	return 0;
}

void keyboardCallback(int key, int state) 
{

	// increase and decrease tessellation levels
	if(key == 'I' && state == GLFW_PRESS) {
		if(tess < 64)
			tess++;
	}
	if(key == 'O' && state == GLFW_PRESS) {
		if(tess > 1)
			tess--;
	}
}


void myInitFunc(void) 
{
	glEnable (GL_DEPTH_TEST);

	angle[0] = 0.0f;
	angle[1] = 0.0f;
	obj = new gl4::VBO();
	obj->init();

	sphere = new gl4::Sphere(1.0, 30);
	sphere->init();

	tess = 1;

	// load textures
	textureID[0]= 0;
	textureID[1]= 0;
	glGenTextures(2, textureID);
	loadTexture("data/earth_nasa.tga",textureID[0]);
	loadTexture("data/earth_height.tga",textureID[1]);
	glBindTexture(GL_TEXTURE_2D,0);
}

void myRenderFunc(void) 
{
	// draw a colored plane
	gl4::ShaderManager::getInstance()->bindShader("Passthrough");
	glm::mat4 plane_transform = glm::scale(glm::mat4(1.0), glm::vec3(5));
	plane_transform = glm::translate(plane_transform,glm::vec3(-0.5,-0.3, 0.0));
	plane_transform = glm::rotate(plane_transform,-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	engine->usePerspectiveProjection(plane_transform);
	obj->render();
	gl4::ShaderManager::getInstance()->unbindShader();


	// draw the earth
	gl4::ShaderManager::getInstance()->bindShader("Deferred1_sphere");

	// bind earth texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID[0]);
	glUniform1i(3, 0);

	// bind heightmap
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID[1]);
	glUniform1i(4, 1);

	// rotate around the x-axis and y-axis
	glm::mat4 transform = glm::rotate(glm::mat4(1.0f),angle[1], glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform,angle[0], glm::vec3(1.0f, 0.0f, 0.0f));
	engine->usePerspectiveProjection(transform);

	// set tessellation levels
	glUniform1f(5, tess); // inner
	glUniform1f(6, tess); // outer
	glUniform2f(7, 800.0,600.0);
	glUniform1f(8, 100.0); // LOD
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	sphere->render();

	// unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	gl4::ShaderManager::getInstance()->unbindShader();
	
}

void myUpdateFunc(float dt)
{
	angle[1] += dt*10;

	float speed = 50.0f;
	if(engine->isKeyPressed(GLFW_KEY_RIGHT)) {
		angle[1] += dt*speed;
	}
	if(engine->isKeyPressed(GLFW_KEY_LEFT)) {
		angle[1] -= dt*speed;
	}
	if(engine->isKeyPressed(GLFW_KEY_UP)) {
		angle[0] += dt*speed;
	}
	if(engine->isKeyPressed(GLFW_KEY_DOWN)) {
		angle[0] -= dt*speed;
	}
}