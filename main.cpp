#include "Engine.h"
#include "ShaderManager.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

gl4::VBO *obj;
gl4::Engine *engine;

void keyboardCallback(int key, int state);
void myRenderFunc(void);
void myInitFunc(void);

int main(int argc, char **argv) {

	// create engine object
	engine = new gl4::Engine(argc, argv);
	
	// set callbacks
	engine->setKeyBoardCallbackfunc(keyboardCallback);
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

void keyboardCallback(int key, int state) {
	if(key == 'A' && state == GLFW_PRESS) {
		std::cout << "Press A" << std::endl;
	}
}


void myInitFunc(void) {
	obj = new gl4::VBO();
	obj->init();
}

void myRenderFunc(void) {

	float t =glfwGetTime();

	gl4::ShaderManager::getInstance()->bindShader("Passthrough");
	glm::mat4 transform = glm::rotate(glm::mat4(1.0f),t*50.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	engine->setPerspectiveProjection(transform);
	obj->render();
	gl4::ShaderManager::getInstance()->unbindShader();
}