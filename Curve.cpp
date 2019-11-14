//
//  curve.cpp
//  CSE-167-PA4
//
//  Created by Wei Zeng on 11/10/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#include "Curve.hpp"

Curve::Curve(glm::vec3 in_p1, glm::vec3 in_p2, glm::vec3 in_p3, glm::vec3 in_p4){
    p1 = in_p1;
    p2 = in_p2;
    p3 = in_p3;
    p4 = in_p4;
    // Sphere model loading
    controlSphe = new Geometry("sphere");
    anchor = new Geometry("sphere");
    
    controlSphe->loadModel("sphere.obj");
    anchor->loadModel("sphere.obj");
    anchor->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
    
    model = glm::mat4(1);
}

void Curve::getPoints(){
    
    glm::vec3 a = -1.0f * p1 + 3.0f * p2 - 3.0f * p3 + p4;
    glm::vec3 b = 3.0f * p1 - 6.0f * p2 + 3.0f * p3;
    glm::vec3 c = -3.0f * p1 + 3.0f * p2;
    glm::vec3 d = p1;
    
    //loop 150 points
    points.push_back(p1);
    a_points.push_back(p1);
    
    for(double i = 1.0; i <= 150; i++){
        // calculate the point with the Bernstein form
        glm::vec3 point = float(pow((i/150), 3.0f)) * a + float(pow((i/150), 2.0f)) * b + float(i/150) * c + d;
        // store every point twice to draw continuous lines
        points.push_back(point);
        points.push_back(point);
        // store every point once for animation
        a_points.push_back(point);
    }
    
    points.push_back(p4);
    a_points.push_back(p4);
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
        points.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void Curve::draw(glm::mat4 projection, glm::mat4 view, GLuint program){
    // get the vertexes
    getPoints();
    
    glUseProgram(program);
    // Bind to the VAO.
    glBindVertexArray(vao);
    glUniform3fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(glm::vec3(0,0,0)));
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    // Draw points
    glDrawArrays(GL_LINES, 0, points.size());
    // Unbind from the VAO.
    glBindVertexArray(0);
    drawControl(program);
}

Curve::~Curve(){
    delete root;
}

void Curve::drawControl(GLuint program){
    
    
    // Translation to sphere position
    c1 = new Transform("c1", glm::translate(p1) * glm::scale(glm::vec3(0.01f, 0.01f, 0.01f)));
    c2 = new Transform("c2", glm::translate(p2) * glm::scale(glm::vec3(0.01f, 0.01f, 0.01f)));
    c3 = new Transform("c3", glm::translate(p3) * glm::scale(glm::vec3(0.01f, 0.01f, 0.01f)));
    
    root = new Transform("root", glm::mat4(1));
    
    root->addChild(c1);
    root->addChild(c2);
    root->addChild(c3);
    
    c1->addChild(controlSphe);
    c2->addChild(anchor);
    c3->addChild(anchor);
    
    root->draw(program, glm::mat4(1));
}

void Curve::updatePt(std::vector<glm::vec3> input){
    std::cerr << "Updating with value: ";
    for(auto i:input){
        std::cerr << glm::to_string(i) << std::endl;
    }
    p1 = input[0];
    p2 = input[1];
    p3 = input[2];
    p4 = input[3];
}
