//
//  Track.hpp
//  CSE-167-PA4
//
//  Created by Wei Zeng on 11/11/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#ifndef Track_hpp
#define Track_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <stdio.h>
#include <math.h>
#include "shader.h"
#include "Curve.hpp"

class Track{
    
private:
    std::vector<glm::vec3> c1, c2, c3, c4, c5, c6, c7, c8;
    std::vector<Curve*> track;
    
public:
    Track();
    ~Track();
    void render(glm::mat4 projection, glm::mat4 view, GLuint program);
    std::vector<Curve*> getTrack();
};

#endif /* Track_hpp */
