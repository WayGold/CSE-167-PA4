//
//  Transform.hpp
//  CSE 167 PA3
//
//  Created by Wei Zeng on 10/27/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#ifndef Transform_hpp
#define Transform_hpp

#include <stdio.h>
#include "Node.hpp"
#include "Geometry.hpp"

class Transform : public Node{

private:
    glm::mat4 T;
    std::list<Node*> children;
    glm::vec3 center;
    float radius = 5.0f;
    
public:
    Transform(std::string name, glm::mat4 T);
    ~Transform();
    
    void addChild(Node* input);
    void removeChild(Node* input);
    
    void draw(GLuint shaderProgram, glm::mat4 C);
    void update(glm::mat4 C);
    void set_center(glm::vec3 point);
    glm::vec3 get_center();
    float get_radius();
};

#endif /* Transform_hpp */
