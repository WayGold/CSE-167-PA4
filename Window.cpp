#include "Window.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int Window::width;
int Window::height;
int Window::event;
int Window::flag_n = 1;
int Window::mode = 1;
int Window::tracker = 1;
int Window::indexTrack = 0;
int Window::ptTrack = 0;

bool Window::indexflag = false;
bool Window::pause = true;
bool Window::board = false;

unsigned int Window::cubemapTexture;
const char* Window::windowTitle = "GLFW Starter Project";
double Window::fov = 60;
double Window::near = 1.0;
double Window::far = 1000.0;
double Window::pfd;             // per frame distance
// Objects to display.
Cube * Window::cube;
PointCloud* Window::sphere;
Track* Window::track;

// The object currently displaying.
Object * Window::currentObj;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 0, 20); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.
glm::vec3 Window::lastPoint;
glm::vec3 Window::curPos;
glm::vec3 Window::rotAxis;

glm::vec3 lightPos(1.0f, 1.0f, 8.0f);

GLfloat Window::angle;

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint Window::skyprogram; // The shader program id.
GLuint Window::sphereShader; // Sphere shader
GLuint Window::trackShader; // track shader

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.
GLuint Window::light_position;
GLuint Window::light_linear;
GLuint Window::viewPos;
GLuint Window::light_color;
GLuint Window::obj_color;
GLuint Window::material_diffuse;
GLuint Window::material_shininess;
GLuint Window::material_specular;
GLuint Window::flag;

bool Window::initializeProgram() {
    
	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glCullFace(GL_FRONT);
    skyprogram = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
    sphereShader = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    trackShader = LoadShaders("shaders/track.vert", "shaders/track.frag");
    
    // Check the shader program.
    if (!skyprogram)
    {
        std::cerr << "Failed to initialize skybox shader program" << std::endl;
    }
    
    if (!sphereShader)
    {
        std::cerr << "Failed to initialize sphere shader program" << std::endl;
    }
    if (!trackShader)
    {
        std::cerr << "Failed to initialize track shader program" << std::endl;
    }
	// Get the locations of uniform variables.
//	projectionLoc = glGetUniformLocation(program, "projection");
//	viewLoc = glGetUniformLocation(program, "view");
//	modelLoc = glGetUniformLocation(program, "model");
//	colorLoc = glGetUniformLocation(program, "color");
//    viewPos = glGetUniformLocation(program, "viewPos");
//    light_position = glGetUniformLocation(program, "lightPos");
//    light_linear = glGetUniformLocation(program, "light_linear");
//    light_color = glGetUniformLocation(program, "lightColor");
//    obj_color = glGetUniformLocation(program, "objectColor");
//    material_diffuse = glGetUniformLocation(program, "material.diffuse");
//    material_specular = glGetUniformLocation(program, "material.specular");
//    material_shininess  = glGetUniformLocation(program, "material.shininess");
//    flag = glGetUniformLocation(program, "flag");
    
    event = 0;
    
	return true;
}

bool Window::initializeObjects()
{
    std::vector<std::string> faces
    {
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/top.jpg",
        "skybox/bottom.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
    };
    // Load cube map
    cubemapTexture = loadCubemap(faces);
	// Create a cube of size 50.
	cube = new Cube(1.0f);
    // init the track
    track = new Track();
    // initialize the roller coaster
    sphere = new PointCloud("sphere.obj", 5.0f);
    sphere->scale(glm::scale(glm::vec3(0.06f, 0.06f, 0.06f)));
    sphere->translate(glm::translate(track->getTrack().at(0)->p1));
    
    return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
    delete sphere;
    delete track;
	// Delete the shader program.
	glDeleteProgram(skyprogram);
    glDeleteProgram(sphereShader);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);
    glfwSetScrollCallback(window, scroll_callback);
	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(fov),
                                          double(width) / (double)height, near, far);
}

void Window::idleCallback()
{
    if(pause)
        return;
    int i = indexTrack;
    int j = ptTrack;
    
    indexflag = false;
    pfd = glfwGetTime() * 0.0008;                                                  // Calculate per frame distance
//  pfd = 0.001;
    std::cerr << "Per Frame Distance = " << pfd << std::endl;
    
    while(pfd != 0){
        for(; i < track->getTrack().size(); i++){                          // Each curve level
            std::cerr << "In first loop with i = " << i << std::endl;
            
            for(; j < track->getTrack()[i]->a_points.size() - 1; j++){     // At curve i traverse all points of the curve
                std::cerr << "In second loop with j = " << j << std::endl;
                // Case when pfd is larger equal to distance between current point to next point
                std::cerr << "distance from sphere to next point " << glm::distance(glm::vec3(sphere->getModel() * glm::vec4(0,0,0,1)), track->getTrack()[i]->a_points[j + 1]) << std::endl;
                
                if((pfd - glm::distance(glm::vec3(sphere->getModel() * glm::vec4(0,0,0,1)), track->getTrack()[i]->a_points[j + 1])) >= 0){
                    sphere->translateTo(track->getTrack()[i]->a_points[j + 1]);
                    pfd = pfd - glm::distance(glm::vec3(sphere->getModel() * glm::vec4(0,0,0,1)), track->getTrack()[i]->a_points[j + 1]);
                    std::cerr << "Larger Equal" << std::endl;
                    // on board perspective update
                    if(board){
                        eye = glm::vec3(sphere->getModel() * glm::vec4(0,0,0,1));
                        center = track->getTrack()[i]->a_points[j + 1] - track->getTrack()[i]->a_points[j];
                        view = glm::lookAt(eye, center, up);
                        glUseProgram(skyprogram);
                        viewLoc = glGetUniformLocation(skyprogram, "view");
                        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                    }
                }
                // Case when pfd is smaller
                else{
                    std::cerr << "Smaller" << std::endl;
                    sphere->translate(glm::translate(glm::normalize(track->getTrack()[i]->a_points[j + 1] - track->getTrack()[i]->a_points[j]) * float(pfd)));
                    // on board perspective update
                    if(board){
                        eye = glm::vec3(sphere->getModel() * glm::vec4(0,0,0,1));
                        center = track->getTrack()[i]->a_points[j + 1] - track->getTrack()[i]->a_points[j];
                        view = glm::lookAt(eye, center, up);
                        glUseProgram(skyprogram);
                        viewLoc = glGetUniformLocation(skyprogram, "view");
                        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                    }
                    pfd = 0;            // 0 to exit loop
                    std::cerr << "Assigning ptTrack with j = " << j << ". Current pfd = " << pfd << std::endl;
                    ptTrack = j;        // Keep track of where we were in terms of index of points
                    indexflag = true;   // Exit loop flag
                    break;
                }
                std::cerr << "Assigning ptTrack with j = " << j << ". Current pfd = " << pfd << std::endl;
                ptTrack = j;
            }
            std::cerr << "Assigning indexTrack with i = " << i << ". Current pfd = " << pfd << std::endl;
            indexTrack = i;             // Where we were in terms of index of curves in track
            if(indexflag){
                
                break;
            }
            // reset j
            if(j == 151)
                j = 0;
            // reset i to -1. later will be added by 1 so that it goes to 0
            if(i == 7)
                i = -1;
        }
    }
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Render Cube Map
    cube->draw(projection, view, skyprogram, cubemapTexture);
    // Render the objects.
    track->render(projection, view, trackShader);
    //glCullFace(GL_FRONT);
    sphere->draw(projection, view, eye, sphereShader, cubemapTexture);
    
    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;
        
        case GLFW_KEY_Z:
            if(mods == GLFW_MOD_SHIFT){
                track->update(tracker, glm::vec3(0.0f, 0.0f, -0.5f));
                break;
            }
            track->update(tracker, glm::vec3(0.0f, 0.0f, 0.5f));
            break;
                
        case GLFW_KEY_X:
            if(mods == GLFW_MOD_SHIFT){
                track->update(tracker, glm::vec3(-0.5f, 0.0f, 0.0f));
                break;
            }
            track->update(tracker, glm::vec3(0.5f, 0.0f, 0.0f));
            break;
                
        case GLFW_KEY_Y:
            if(mods == GLFW_MOD_SHIFT){
                track->update(tracker, glm::vec3(0.0f, -0.5f, 0.0f));
                break;
            }
            track->update(tracker, glm::vec3(0.0f, 0.5f, 0.0f));
            break;
        
        case GLFW_KEY_P:
            pause = !pause;
            break;
        
        case GLFW_KEY_C:
            board = !board;
            if(board == false){
                eye = glm::vec3(0, 0, 20);
                center = glm::vec3(0, 0, 0);
                view = glm::lookAt(eye, center, up);
                glUseProgram(skyprogram);
                viewLoc = glGetUniformLocation(skyprogram, "view");
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            }
            break;
                
        case GLFW_KEY_1:
            if(tracker - 1 <= 0){
                tracker = 23 + tracker;
            }
            else{
                tracker--;
            }
            std::cerr <<  "tracker: " << tracker << std::endl;
            track->update(tracker, glm::vec3(0.0f, 0.0f, 0.0f));
            break;
                
        case GLFW_KEY_2:
            if(tracker + 1 >= 25){
                tracker = 25 - tracker;
            }
            else{
                tracker++;
            }
            std::cerr <<  "tracker: " << tracker << std::endl;
            track->update(tracker, glm::vec3(0.0f, 0.0f, 0.0f));
            break;
                
		default:
			break;
		}
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    // Scroll up
    if(yoffset > 0){
        if(fov + 1 > 175) return;
        fov = fov + 1;
        projection = glm::perspective(glm::radians(fov), double(width) / (double)height, near, far);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }
    // Scroll down
    else{
        if(fov - 1 < 5) return;
        fov = fov - 1;
        projection = glm::perspective(glm::radians(fov), double(width) / (double)height, near, far);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }
}

void Window::cursor_callback(GLFWwindow* window, double xpos, double ypos){ 
    // no action detected
    if(event == 0) return;
    // left press and hold
    if(event == 1){
        // Map the mouse position to a logical sphere location.
        // Keep it in the class variable lastPoint.
        glm::vec2 point = glm::vec2(xpos, ypos);
        
        // get world coord of first point click
        lastPoint = trackBallMapping(point);
        // set event to handle cursor moving
        event = 2;
        return;
    }
    
    // Get the current point in world coord
    curPos = trackBallMapping(glm::vec2(xpos, ypos));
    /* Calculate the angle in radians, and clamp it between 0 and 90 degrees */
    angle = glm::acos(std::min(1.0f, glm::dot(lastPoint, curPos)));
    /* Cross product to get the rotation axis, but it's still in camera coordinate */
    rotAxis  = glm::cross(lastPoint, curPos);
    
    // move camera
    glm::vec3 camDirec = glm::normalize(center - eye);
    camDirec = glm::vec3(glm::rotate(glm::degrees(angle) * 0.05f, rotAxis) * glm::vec4(camDirec.x, camDirec.y, camDirec.z, 0.0));
    view = glm::lookAt(eye, eye + camDirec, up);
    center = eye + camDirec;
    
    glUseProgram(skyprogram);
    viewLoc = glGetUniformLocation(skyprogram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    
    glm::vec2 point = glm::vec2(xpos, ypos);
           
    // get world coord of first point click
    lastPoint = trackBallMapping(point);
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods){
    event = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
}

glm::vec3 Window::trackBallMapping(glm::vec2 point){
    glm::vec3 v;
    float d;
    
    v.x = (2.0f * point.x - width) / width;
    v.y = (height - 2.0f * point.y) / height;
    v.z = 0.0f;
    
    d = glm::length(v);
    d = (d < 1.0f) ? d : 1.0f;
    v.z = sqrtf(1.001f - d * d);
    
    v = glm::normalize(v);
    return v;
}

unsigned int Window::loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
