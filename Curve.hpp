//
//  curve.hpp
//  CSE-167-PA4
//
//  Created by Wei Zeng on 11/10/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#ifndef Curve_hpp
#define Curve_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <stdio.h>
#include <math.h>
#include "shader.h"
#include "Geometry.hpp"
#include "Transform.hpp"

class Curve
{
private:
    glm::mat4 model;
    GLuint vao, vbo;
    
public:
    glm::vec3 p1, p2, p3, p4;
    float length;
    std::vector<glm::vec3> a_points;
    std::vector<glm::vec3> points;
    Geometry* controlSphe, *anchor;
    Transform* c1, *c2, *c3, *root;
    
    Curve();
    Curve(glm::vec3 in_p1, glm::vec3 in_p2, glm::vec3 in_p3, glm::vec3 in_p4);
    ~Curve();
    
    void getPoints();
    void draw(glm::mat4 projection, glm::mat4 view, GLuint program);
    void drawControl(GLuint program);
    void updatePt(std::vector<glm::vec3> input);
};

#endif /* curve_hpp */
