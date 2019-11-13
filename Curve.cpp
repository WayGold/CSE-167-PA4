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
    model = glm::mat4(1);
}

void Curve::getPoints(){
    
    glm::vec3 a = -1.0f * p1 + 3.0f * p2 - 3.0f * p3 + p4;
    glm::vec3 b = 3.0f * p1 - 6.0f * p2 + 3.0f * p3;
    glm::vec3 c = -3.0f * p1 + 3.0f * p2;
    glm::vec3 d = p1;
    
    //loop 150 points
    points.push_back(p1);
    
    for(double i = 1.0; i <= 150; i++){
        // calculate the point with the Bernstein form
        glm::vec3 point = float(pow((i/150), 3.0f)) * a + float(pow((i/150), 2.0f)) * b + float(i/150) * c + d;
        points.push_back(point);
        points.push_back(point);
    }
    
    points.push_back(p4);
    
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
    
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    
    // Draw points
    glDrawArrays(GL_LINES, 0, points.size());
    // Unbind from the VAO.
    glBindVertexArray(0);
}

Curve::~Curve(){
    
}
