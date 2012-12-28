#include "Engine.h"

gl4::VBO *obj;
gl4::Engine *engine;
float angle;

void keyboardCallback(int key, int state);
void myRenderFunc(void);
void myInitFunc(void);
void myUpdateFunc(float dt);

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
	if(key == 'A' && state == GLFW_PRESS) {
		std::cout << "Press A" << std::endl;
	}
}


void myInitFunc(void) 
{
	angle = 0.0f;
	obj = new gl4::VBO();
	obj->init();
}

void myRenderFunc(void) 
{
	gl4::ShaderManager::getInstance()->bindShader("Deferred1");
	glm::mat4 transform = glm::rotate(glm::mat4(1.0f),angle, glm::vec3(0.0f, 1.0f, 0.0f));
	engine->usePerspectiveProjection(transform);
	obj->render();
	gl4::ShaderManager::getInstance()->unbindShader();
}

void myUpdateFunc(float dt)
{
	angle += dt*50;
}