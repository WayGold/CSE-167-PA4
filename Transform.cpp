//
//  Transform.cpp
//  CSE 167 PA3
//
//  Created by Wei Zeng on 10/27/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#include "Transform.hpp"

Transform::Transform(std::string name, glm::mat4 T){
    this->name = name;
    this->T = T;
}

Transform::~Transform(){
    for(auto it = children.begin(); it != children.end(); ++it){
        if(*it != nullptr){
            //delete(*it);
        }
    }
}

void Transform::draw(GLuint shaderProgram, glm::mat4 C){
    glm::mat4 M_new = C * T;
    
    for(std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it){
//        std::cerr << std::endl << "calling draw on: " << (*it)->getName() << std::endl;
        (*it)->draw(shaderProgram, M_new);
    }
}

void Transform::addChild(Node* input){
//    std::cerr << "Adding child: " << input->getName() << " to " << this->getName() << std::endl;
    input->setParent(this);
    children.push_back(input);
}

void Transform::update(glm::mat4 C){
    this->T = C * T;
}

void Transform::set_center(glm::vec3 point){
    center = point;
}

glm::vec3 Transform::get_center(){
    return center;
}

float Transform::get_radius(){
    return radius;
}
