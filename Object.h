#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <list>

class Object
{
protected:
	glm::mat4 model;
    glm::vec3 color;
    
public:
	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }
    
    void setColor(glm::vec3 input) {
        color = input;
    }
    
	virtual void draw() = 0;
	virtual void update() = 0;
};

#endif

