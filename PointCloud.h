#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

#include "Object.h"
#include "Curve.hpp"

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
    std::vector<glm::ivec3> triangles;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
	GLuint vao, ebo;
    GLuint vbos[2];
	GLfloat pointSize;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 center;
    GLfloat shininess;
public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw();
    void draw(GLuint program);
    void draw(glm::mat4 projection, glm::mat4 view, glm::vec3 eye, GLuint program, unsigned int cubemapTexture);
    
	void update();
    void update(glm::vec3 point);

	void updatePointSize(GLfloat size);
	void spin(float deg);
    void scale(bool direc);
    void scale(glm::mat4 matrix);
    void rotate(glm::mat4 matrix);
    void translate(glm::mat4 matrix);
    
    glm::vec3 get_diffuse();
    glm::vec3 get_specular();
    glm::vec3 get_center();
    glm::vec3 calc_center();
    GLfloat get_shininess();

    void set_diffuse(glm::vec3 input);
    void set_specular(glm::vec3 input);
    void set_shininess(float input);
    void translateTo(glm::vec3 input);
    
    GLfloat get_pointsize(){
        return pointSize;
    };
};

#endif

