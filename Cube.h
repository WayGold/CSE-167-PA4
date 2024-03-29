#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Object.h"
#include "shader.h"


class Cube : public Object
{
private:
	GLuint vao;
	GLuint vbos[2];
public:
	Cube(float size);
	~Cube();

	void draw(glm::mat4 projection, glm::mat4 view, GLuint program, unsigned int cubemapTexture);
    void draw();
	void update();

	void spin(float deg);
};

#endif

