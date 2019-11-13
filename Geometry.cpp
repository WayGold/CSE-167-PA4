//
//  Geometry.cpp
//  CSE 167 PA3
//
//  Created by Wei Zeng on 10/27/19.
//  Copyright © 2019 Wei Zeng. All rights reserved.
//

#include "Geometry.hpp"
#include "Window.h"

Geometry::Geometry(std::string name){
    this->name = name;
}

Geometry::~Geometry(){
    
}

void Geometry::loadModel(std::string objFilename){
    
    std::ifstream objFile(objFilename); // The obj file we are reading.
    glm::vec3 pcenter = calc_center();
    GLfloat sq_distance = 0;
    GLfloat tmp_distance = 0;
    bool dis_flag = true;
    
    // Check whether the file can be opened.
    if (objFile.is_open())
    {
        std::string line; // A line in the file.
        
        // Read lines from the file.
        while (std::getline(objFile, line))
        {
            // Turn the line into a string stream for processing.
            std::stringstream ss;
            ss << line;
            
            // Read the first word of the line.
            std::string label;
            ss >> label;
            
            // If the line is about vertex (starting with a "v").
            if (label == "v"){
                // Read the later three float numbers and use them as the
                // coordinates.
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;
                
                // Process the point. For example, you can save it to a.
                points.push_back(point);
            }
            
            // Load triangles
            if(label == "f"){
                glm::ivec3 triangle;
                
                std::string delimiter = "//";
                std::string delimiter_2 = "/";
                std::string line_1, line_2, line_3;
                
                ss >> line_1 >> line_2 >> line_3;
                
                if(line_1.find(delimiter) != std::string::npos){
                    triangle.x = std::stoi(line_1.substr(0, line_1.find(delimiter))) - 1;
                    triangle.y = std::stoi(line_2.substr(0, line_2.find(delimiter))) - 1;
                    triangle.z = std::stoi(line_3.substr(0, line_3.find(delimiter))) - 1;
                    //std::cerr << "Reading in: "<< triangle.x << " " << triangle.y << " " << triangle.z << std::endl;
                    triangles.push_back(triangle);
                }
                else{
                    triangle.x = std::stoi(line_1.substr(0, line_1.find(delimiter_2))) - 1;
                    triangle.y = std::stoi(line_2.substr(0, line_2.find(delimiter_2))) - 1;
                    triangle.z = std::stoi(line_3.substr(0, line_3.find(delimiter_2))) - 1;
                    /*std::cerr << "File: " << objFilename << ". Reading in: "<< triangle.x << " " << triangle.y << " " << triangle.z << std::endl;*/
                    triangles.push_back(triangle);
                }
            }
            
            // Load vector normals
            if(label == "vn"){
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal);
            }
        }
    }
    else
    {
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }
    
    objFile.close();
    
    // Center the model
    for(int i = 0; i < points.size(); i++){
        points[i][0] = points[i][0] - pcenter.x;
        points[i][1] = points[i][1] - pcenter.y;
        points[i][2] = points[i][2] - pcenter.z;
    }
    
    for(int j = 0; j < points.size(); j++){
        if(dis_flag == true){
            sq_distance = points[j][0] * points[j][0] + points[j][1] * points[j][1] + points[j][2] * points[j][2];
            dis_flag = false;
        }
        else{
            tmp_distance = points[j][0] * points[j][0] + points[j][1] * points[j][1] + points[j][2] * points[j][2];
            sq_distance = (sq_distance > tmp_distance) ? sq_distance : tmp_distance;
        }
    }
    
    float scale_factor = 9.5 / sqrt(sq_distance);
    
    glm::vec3 scale = glm::vec3(scale_factor, scale_factor, scale_factor);
    
    // Set the model matrix to an identity matrix.
    this->model = glm::mat4(1);
    
    // Set the color.
    color = glm::vec3(1, 0, 0);
    
    this->model = glm::scale(model, scale);
    
    //    std::cerr << std::endl << "Model Matrix of: " << getName() << " after loading: " << std::endl << glm::to_string(model) << std::endl;
    
    // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbos);
    glGenBuffers(1, &ebo);
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    
    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
    
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Bind to the EBO. We will use it to store the indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // Pass in the data.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * triangles.size(), triangles.data(), GL_STATIC_DRAW);
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void Geometry::draw(GLuint shaderProgram, glm::mat4 M){
    
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
    
    // Specify the values of the uniform variables we are going to use.
    glm::vec3 color = getColor();
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(M * model));
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Draw triangles
    glDrawElements(GL_TRIANGLES, 3 * triangles.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    return;
}

glm::vec3 Geometry::calc_center(){
    float x_max_c, y_max_c, z_max_c, x_min_c, y_min_c, z_min_c, x_mid_c, y_mid_c, z_mid_c;
    
    x_max_c = y_max_c = z_max_c = x_min_c = y_min_c = z_min_c = 0;
    
    for(int i = 0; i < points.size(); i++){
        if(i == 0){
            x_max_c = points[i].x;
            x_min_c = points[i].x;
            y_max_c = points[i].y;
            y_min_c = points[i].y;
            z_max_c = points[i].z;
            z_min_c = points[i].z;
        }
        else{
            // track the max and min of each coordinates while reading in
            x_max_c = (points[i].x > x_max_c) ? points[i].x : x_max_c;
            x_min_c = (points[i].x < x_min_c) ? points[i].x : x_min_c;
            
            y_max_c = (points[i].y > y_max_c) ? points[i].y : y_max_c;
            y_min_c = (points[i].y < y_min_c) ? points[i].y : y_min_c;
            
            z_max_c = (points[i].z > z_max_c) ? points[i].z : z_max_c;
            z_min_c = (points[i].z < z_min_c) ? points[i].z : z_min_c;
        }
    }
    //std::cerr << "x max c = " << x_max_c << std::endl;
    //std::cerr << "x min c = " << x_min_c << std::endl;
    
    glm::vec3 center_new;
    x_mid_c = (x_max_c + x_min_c) / 2;
    y_mid_c = (y_max_c + y_min_c) / 2;
    z_mid_c = (z_max_c + z_min_c) / 2;
    
    center_new = glm::vec3(x_mid_c, y_mid_c, z_mid_c);
    //std::cerr << "Calc center: (" << x_mid_c << ", " << y_mid_c << ", " << z_mid_c << ")" << std::endl;
    return center_new;
}

void Geometry::set_diffuse(glm::vec3 input){
    diffuse = input;
}

void Geometry::set_specular(glm::vec3 input){
    specular = input;
}

void Geometry::set_shininess(float input){
    shininess = input;
}

void Geometry::setColor(glm::vec3 input){
    color = input;
}

glm::vec3 Geometry::get_diffuse(){
    return diffuse;
}

glm::vec3 Geometry::get_specular(){
    return specular;
}

float Geometry::get_shininess(){
    return shininess;
}

void Geometry::scale(glm::mat4 matrix){
    model = matrix * model;
}

// function to modify the model matrix with respect to the rotate matrix
void Geometry::rotate(glm::mat4 matrix){
    model = matrix * model;
}

void Geometry::translate(glm::mat4 matrix){
    model = matrix * model;
}

void Geometry::update(glm::mat4 C){
    
}
