#ifndef _NODE_H_
#define _NODE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <stdio.h>
#include "glm/gtx/string_cast.hpp"

class Node {
    
protected:
    std::string name;
    Node* parent;

public:
    virtual void draw(GLuint shaderProgram, glm::mat4 C) = 0;
    virtual void update(glm::mat4 C) = 0;
    
    void setParent(Node* input){
        this->parent = input;
    };
    
    std::string getName(){
        return this->name;
    };
};

#endif
