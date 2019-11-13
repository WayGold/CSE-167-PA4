//
//  Geometry.hpp
//  CSE 167 PA3
//
//  Created by Wei Zeng on 10/27/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <stdio.h>
#include "Node.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <math.h>

class Geometry : public Node {
    
private:
    std::vector<glm::vec3> points;
    std::vector<glm::ivec3> triangles;
    std::vector<glm::vec3> normals;
    GLuint vao, ebo;
    GLuint vbos[2];
    GLfloat shininess;
    glm::mat4 model;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 color;
    
public:
    Geometry(std::string name);
    ~Geometry();
    
    glm::vec3 calc_center();
    glm::vec3 get_diffuse();
    glm::vec3 get_specular();
    GLfloat get_shininess();
    
    void loadModel(std::string objFilename);
    void draw(GLuint shaderProgram, glm::mat4 M);
    void update(glm::mat4 C);
    
    void set_diffuse(glm::vec3 input);
    void set_specular(glm::vec3 input);
    void set_shininess(float input);
    void setColor(glm::vec3 input);
    
    void scale(glm::mat4 matrix);
    void rotate(glm::mat4 matrix);
    void translate(glm::mat4 matrix);
    
    glm::vec3 getColor(){
        return color;
        
    };
    
    glm::mat4 getModel(){
        return model;
        
    };
};

#endif /* Geometry_hpp */
