#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "Track.hpp"
#include "shader.h"

class Window
{
public:
	static int width;
	static int height;
    static int event;
    static int flag_n;
    static int mode;
    static int tracker, indexTrack, ptTrack;
    static bool indexflag, pause, board;
    static unsigned int cubemapTexture;
    static double fov, near, far, pfd;
	static const char* windowTitle;
    static Track* track;
	static Cube* cube;
	static PointCloud * cubePoints;
    static PointCloud * bear;
    static PointCloud * bunny;
    static PointCloud * dragon;
    static PointCloud * cat;
    static PointCloud * sphere;
	static Object * currentObj;
	static glm::mat4 projection;
	static glm::mat4 view;
    static glm::vec3 eye, center, up, lastPoint, curPos, rotAxis;
	static GLuint trackShader, skyprogram, sphereShader, projectionLoc, viewLoc, modelLoc, colorLoc, light_position, light_linear, viewPos, light_color, obj_color, material_diffuse, material_specular, material_shininess, flag;
    static GLfloat angle;

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static glm::vec3 trackBallMapping(glm::vec2 point);
    static glm::mat4 rotMatrix();
    static unsigned int loadCubemap(std::vector<std::string> faces);
};

#endif
