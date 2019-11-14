//
//  Track.cpp
//  CSE-167-PA4
//
//  Created by Wei Zeng on 11/11/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#include "Track.hpp"

Track::Track(){
    
    glm::vec3 paddleft = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 paddright = glm::vec3(-1.0f, 0.0f, 0.0f);
    glm::vec3 paddup = glm::vec3(0.0f, 2.0f, 0.0f);
    glm::vec3 padddown = glm::vec3(0.0f, -2.0f, 0.0f);
    glm::vec3 padddep = glm::vec3(0.0f, 0.0f, -2.0f);
    
    c1.push_back(glm::vec3(15.0f, 8.0f, 0.0f));
    c1.push_back(glm::vec3(10.0f, 6.0f, 0.0f));
    c1.push_back(glm::vec3(8.0f, 2.0f, 0.0f));
    c1.push_back(glm::vec3(6.0f, 0.0f, 0.0f));
    
    c2.push_back(c1.at(3));
    c2.push_back(2.0f * c1.at(3) - c1.at(2));
    c2.push_back(c2.at(1) + paddright);
    c2.push_back(c2.at(2) + paddright + paddup);
    
    c3.push_back(c2.at(3));
    c3.push_back(2.0f * c2.at(3) - c2.at(2));
    c3.push_back(c3.at(1) + 2.0f * paddright + padddown);
    c3.push_back(c3.at(2) + 4.0f * paddright + padddown);
    
    c4.push_back(c3.at(3));
    c4.push_back(2.0f * c3.at(3) - c3.at(2));
    c4.push_back(c4.at(1) + 2.0f * paddright + paddup * 2.0f);
    c4.push_back(c4.at(2) + 4.0f * paddright);
    
    c5.push_back(c4.at(3));
    c5.push_back(2.0f * c4.at(3) - c4.at(2));
    c5.push_back(c5.at(1) + 2.0f * paddleft + paddup);
    c5.push_back(c5.at(2) + 4.0f * paddleft);
    
    c6.push_back(c5.at(3));
    c6.push_back(2.0f * c5.at(3) - c5.at(2));
    c6.push_back(c6.at(1) + 2.0f * paddleft + paddup);
    c6.push_back(c6.at(2) + 4.0f * paddleft);
    
    c7.push_back(c6.at(3));
    c7.push_back(2.0f * c6.at(3) - c6.at(2));
    c7.push_back(c7.at(1) + 2.0f * paddleft + paddup);
    c7.push_back(c7.at(2) + 4.0f * paddleft);
    
    c8.push_back(c7.at(3));
    c8.push_back(2.0f * c7.at(3) - c7.at(2));
    c8.push_back(c8.at(1) + 2.0f * paddleft + padddown + padddep * 2.0f);
    c8.push_back(c1.at(0));
    
    c1[1] = 2.0f * c8.at(3) - c8.at(2);
    
    cv1 = new Curve(c1.at(0), c1.at(1), c1.at(2), c1.at(3));
    cv2 = new Curve(c2.at(0), c2.at(1), c2.at(2), c2.at(3));
    cv3 = new Curve(c3.at(0), c3.at(1), c3.at(2), c3.at(3));
    cv4 = new Curve(c4.at(0), c4.at(1), c4.at(2), c4.at(3));
    cv5 = new Curve(c5.at(0), c5.at(1), c5.at(2), c5.at(3));
    cv6 = new Curve(c6.at(0), c6.at(1), c6.at(2), c6.at(3));
    cv7 = new Curve(c7.at(0), c7.at(1), c7.at(2), c7.at(3));
    cv8 = new Curve(c8.at(0), c8.at(1), c8.at(2), c8.at(3));

    track.push_back(cv1);
    track.push_back(cv2);
    track.push_back(cv3);
    track.push_back(cv4);
    track.push_back(cv5);
    track.push_back(cv6);
    track.push_back(cv7);
    track.push_back(cv8);
}

void Track::render(glm::mat4 projection, glm::mat4 view, GLuint program){
    
    for(auto it = std::begin(track); it!=std::end(track); ++it){
        (*it)->draw(projection, view, program);
    }
    // Get handles container
    load_handles();
    
    // Draw Handles
    glUseProgram(program);
    // Bind to the VAO.
    glBindVertexArray(vao);
    glUniform3fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(glm::vec3(1,1,0)));
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    
    // Draw points
    glDrawArrays(GL_LINES, 0, c_lines.size());
    // Unbind from the VAO.
    glBindVertexArray(0);
}

std::vector<Curve*> Track::getTrack(){
    return track;
}

Track::~Track(){
    for(auto it = std::begin(track); it!=std::end(track); ++it){
        delete(*it);
    }
}

void Track::load_handles(){
    
    c_lines.push_back(c8.at(2));
    c_lines.push_back(c1.at(1));
    c_lines.push_back(c1.at(2));
    c_lines.push_back(c2.at(1));
    c_lines.push_back(c2.at(2));
    c_lines.push_back(c3.at(1));
    c_lines.push_back(c3.at(2));
    c_lines.push_back(c4.at(1));
    c_lines.push_back(c4.at(2));
    c_lines.push_back(c5.at(1));
    c_lines.push_back(c5.at(2));
    c_lines.push_back(c6.at(1));
    c_lines.push_back(c6.at(2));
    c_lines.push_back(c7.at(1));
    c_lines.push_back(c7.at(2));
    c_lines.push_back(c8.at(1));
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c_lines.size(),
        c_lines.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}


void Track::update(int tracker, glm::vec3 input){
    
    int i = ((tracker - 1) / 3) + 1;            // calculate which line segment we're on e.g tracker = 4 represent curve 2 head
    int j  = (tracker - 1) % 3;                 // calculate which index the point is on that line
    
    switch (i) {
        case 1:
            switch (j) {
                case 0:
                    c1[0] += input;
                    break;
                case 1:
                    c1[1] += input;
                    break;
                case 2:
                    c1[2] += input;
                default:
                    break;
            }
            track[0]->updatePt(c1);
            break;
            
        case 2:
            switch (j) {
                case 0:
                    c2[0] += input;
                    break;
                case 1:
                    c2[1] += input;
                    break;
                case 2:
                    c2[2] += input;
                default:
                    break;
            }
            track[1]->updatePt(c2);
            break;
        
        case 3:
            switch (j) {
                case 0:
                    c3[0] += input;
                    break;
                case 1:
                    c3[1] += input;
                    break;
                case 2:
                    c3[2] += input;
                default:
                    break;
            }
            track[2]->updatePt(c3);
            break;
            
        case 4:
            switch (j) {
                case 0:
                    c4[0] += input;
                    break;
                case 1:
                    c4[1] += input;
                    break;
                case 2:
                    c4[2] += input;
                default:
                    break;
            }
            track[3]->updatePt(c4);
            break;
            
        case 5:
            switch (j) {
                case 0:
                    c5[0] += input;
                    break;
                case 1:
                    c5[1] += input;
                    break;
                case 2:
                    c5[2] += input;
                default:
                    break;
            }
            track[4]->updatePt(c5);
            break;
        
        case 6:
            switch (j) {
                case 0:
                    c6[0] += input;
                    break;
                case 1:
                    c6[1] += input;
                    break;
                case 2:
                    c6[2] += input;
                default:
                    break;
            }
            track[5]->updatePt(c6);
            break;
        
        case 7:
            switch (j) {
                case 0:
                    c7[0] += input;
                    break;
                case 1:
                    c7[1] += input;
                    break;
                case 2:
                    c7[2] += input;
                default:
                    break;
            }
            track[6]->updatePt(c7);
            break;
        
        case 8:
            switch (j) {
                case 0:
                    c8[0] += input;
                    break;
                case 1:
                    c8[1] += input;
                    break;
                case 2:
                    c8[2] += input;
                default:
                    break;
            }
            track[7]->updatePt(c8);
            break;

        default:
            break;
    }
}
