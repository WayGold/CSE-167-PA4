#include "PointCloud.h"

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize) 
	: pointSize(pointSize)
{
    // set max and min tracker for each coordinates
    float x_max, y_max, z_max, x_min, y_min, z_min;
    x_max = y_max = z_max = x_min = y_min = z_min = 0;
    
    float x_mid = 0;
    float y_mid = 0;
    float z_mid = 0;
    
    float scale_factor = 1.0;
    // set boolean flag to check whether values are already assigned
    bool flag = false;
    
    // checking foo objFilename input
    if(objFilename == "foo"){
        points =
        {
            glm::vec3(-2.5, 2.5, 2.5),
            glm::vec3(-2.5, -2.5, 2.5),
            glm::vec3(2.5, -2.5, 2.5),
            glm::vec3(2.5, 2.5, 2.5),
            glm::vec3(-2.5, 2.5, -2.5),
            glm::vec3(-2.5, -2.5, -2.5),
            glm::vec3(2.5, -2.5, -2.5),
            glm::vec3(2.5, 2.5, -2.5)
        };
    }
    /*
    * TODO: Section 2: Currently, all the points are hard coded below.
    * Modify this to read points from an obj file.
    */
    else{
        std::ifstream objFile(objFilename); // The obj file we are reading.

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

                    if(flag == false){
                        x_max = point.x;
                        x_min = point.x;
                        y_max = point.y;
                        y_min = point.y;
                        z_max = point.z;
                        z_min = point.z;
                        flag = true;
                    }
                    else{
                        // track the max and min of each coordinates while reading in
                        x_max = (point.x > x_max) ? point.x : x_max;
                        x_min = (point.x < x_min) ? point.x : x_min;
                        
                        y_max = (point.y > y_max) ? point.y : y_max;
                        y_min = (point.y < y_min) ? point.y : y_min;
                        
                        z_max = (point.z > z_max) ? point.z : z_max;
                        z_min = (point.z < z_min) ? point.z : z_min;
                    }
                    
                    // Process the point. For example, you can save it to a.
                    points.push_back(point);
                }
                
                // Load triangles
                if(label == "f"){
                    glm::ivec3 triangle;
                    
                    std::string delimiter = "//";
                    std::string line_1, line_2, line_3;
                    
                    ss >> line_1 >> line_2 >> line_3;
                    
                    triangle.x = std::stoi(line_1.substr(0, line_1.find(delimiter))) - 1;
                    triangle.y = std::stoi(line_2.substr(0, line_2.find(delimiter))) - 1;
                    triangle.z = std::stoi(line_3.substr(0, line_3.find(delimiter))) - 1;
                    //std::cerr << "Reading in: "<< triangle.x << " " << triangle.y << " " << triangle.z << std::endl;
                    triangles.push_back(triangle);
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
    }
    
//    std::cerr << "Parsing file: " << objFilename << " Size of triangle: " << triangles.size() << " Size of vn: " << normals.size() << std::endl;
    
    // Calculate the center coordinates
    x_mid = (x_max + x_min) / 2;
    y_mid = (y_max + y_min) / 2;
    z_mid = (z_max + z_min) / 2;
    
	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen. 
	 */
    GLfloat sq_distance = 0;
    GLfloat tmp_distance = 0;
    bool dis_flag = true;
    
    for(int i = 0; i < points.size(); i++){
        points[i][0] = points[i][0] - x_mid;
        points[i][1] = points[i][1] - y_mid;
        points[i][2] = points[i][2] - z_mid;
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
    
    scale_factor = 9.5 / sqrt(sq_distance);
    
    glm::vec3 scale = glm::vec3(scale_factor, scale_factor, scale_factor);
    
	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Set the color. 
	color = glm::vec3(1, 0, 0);
    
    model = glm::scale(model, scale);
    
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

PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(2, vbos);
    glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw(glm::mat4 projection, glm::mat4 view, glm::vec3 eye, GLuint program, unsigned int cubemapTexture)
{
    glUseProgram(program);
	// Bind to the VAO.
	glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(program, "cameraPos"), eye.x, eye.y, eye.z);
    glUniform1i(glGetUniformLocation(program, "skybox"), 0);
    // Draw triangles
    glDrawElements(GL_TRIANGLES, 3 * triangles.size(), GL_UNSIGNED_INT, 0);
    
    // Unbind from the VAO.
	glBindVertexArray(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
}

// update for supporting the animation of roller coaster
void PointCloud::update(glm::vec3 point){
    model = glm::translate(point) * model;
}

void PointCloud::updatePointSize(GLfloat size) 
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
    pointSize += size;
}

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void PointCloud::scale(bool direc){
    
    // Scale up
    if(direc){
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
    }
    // Scale down
    else{
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
    }
}

void PointCloud::scale(glm::mat4 matrix){
    model = matrix * model;
}

// function to modify the model matrix with respect to the rotate matrix
void PointCloud::rotate(glm::mat4 matrix){
    model = matrix * model;
}

void PointCloud::translate(glm::mat4 matrix){
    model = matrix * model;
}

glm::vec3 PointCloud::get_diffuse(){
    return diffuse;
}

glm::vec3 PointCloud::get_specular(){
    return specular;
}

glm::vec3 PointCloud::get_center(){
    return center;
}

glm::vec3 PointCloud::calc_center(){
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

float PointCloud::get_shininess(){
    return shininess;
}

void PointCloud::set_diffuse(glm::vec3 input){
    diffuse = input;
}

void PointCloud::set_specular(glm::vec3 input){
    specular = input;
}

void PointCloud::set_shininess(float input){
    shininess = input;
}

void PointCloud::draw(){
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Draw points
    glDrawElements(GL_TRIANGLES, 3 * triangles.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void PointCloud::translateTo(glm::vec3 input){
    model = glm::translate(input) * glm::scale(glm::vec3(0.25, 0.25, 0.25)) * glm::mat4(1);
}

