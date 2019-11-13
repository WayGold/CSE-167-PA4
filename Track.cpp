//
//  Track.cpp
//  CSE-167-PA4
//
//  Created by Wei Zeng on 11/11/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#include "Track.hpp"

Track::Track(){
    
    glm::vec3 paddleft = glm::vec3(2.0f, 0.0f, 0.0f);
    glm::vec3 paddright = glm::vec3(-2.0f, 0.0f, 0.0f);
    glm::vec3 paddup = glm::vec3(0.0f, 10.0f, 0.0f);
    glm::vec3 padddown = glm::vec3(0.0f, -10.0f, 0.0f);
    glm::vec3 padddep = glm::vec3(0.0f, 0.0f, -4.0f);
    
    c1.push_back(glm::vec3(15.0f, 8.0f, 0.0f));
    c1.push_back(glm::vec3(10.0f, 6.0f, 0.0f));
    c1.push_back(glm::vec3(8.0f, 2.0f, 0.0f));
    c1.push_back(glm::vec3(6.0f, 0.0f, 0.0f));
    
    c2.push_back(c1.at(3));
    c2.push_back(c1.at(3) + paddright + paddup);
    c2.push_back(c1.at(3) + 2.0f * paddright + padddown);
    c2.push_back(c1.at(3) + 4.0f * paddright);
    
    c3.push_back(c2.at(3));
    c3.push_back(c2.at(3) + paddright + padddown);
    c3.push_back(c2.at(3) + 2.0f * paddright + padddown);
    c3.push_back(c2.at(3) + 4.0f * paddright);
    
    c4.push_back(c3.at(3));
    c4.push_back(c3.at(3) + paddright + padddown);
    c4.push_back(c3.at(3) + 2.0f * paddright + padddown);
    c4.push_back(c3.at(3) + 4.0f * paddright);
    
    c5.push_back(c4.at(3));
    c5.push_back(c4.at(3) + paddleft + paddup + padddep);
    c5.push_back(c4.at(3) + 2.0f * paddleft + paddup + padddep);
    c5.push_back(c4.at(3) + 4.0f * paddleft + padddep);
    
    c6.push_back(c5.at(3));
    c6.push_back(c5.at(3) + paddleft + padddown);
    c6.push_back(c5.at(3) + 2.0f * paddleft + paddup);
    c6.push_back(c5.at(3) + 4.0f * paddleft);
    
    c7.push_back(c6.at(3));
    c7.push_back(c6.at(3) + 2.0f * paddleft + padddown);
    c7.push_back(c6.at(3) + 2.0f * paddleft + padddown);
    c7.push_back(c6.at(3) + 4.0f * paddleft);
    
    c8.push_back(c7.at(3));
    c8.push_back(c7.at(3) + paddleft + paddup);
    c8.push_back(c7.at(3) + 2.0f * paddleft + padddown);
    c8.push_back(c1.at(0));
    
    Curve *cv1 = new Curve(c1.at(0), c1.at(1), c1.at(2), c1.at(3));
    Curve *cv2 = new Curve(c2.at(0), c2.at(1), c2.at(2), c2.at(3));
    Curve *cv3 = new Curve(c3.at(0), c3.at(1), c3.at(2), c3.at(3));
    Curve *cv4 = new Curve(c4.at(0), c4.at(1), c4.at(2), c4.at(3));
    Curve *cv5 = new Curve(c5.at(0), c5.at(1), c5.at(2), c5.at(3));
    Curve *cv6 = new Curve(c6.at(0), c6.at(1), c6.at(2), c6.at(3));
    Curve *cv7 = new Curve(c7.at(0), c7.at(1), c7.at(2), c7.at(3));
    Curve *cv8 = new Curve(c8.at(0), c8.at(1), c8.at(2), c8.at(3));

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
}

std::vector<Curve*> Track::getTrack(){
    return track;
}

Track::~Track(){
    for(auto it = std::begin(track); it!=std::end(track); ++it){
        delete(*it);
    }
}

