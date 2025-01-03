//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"

#include <iostream>

//int glWindowWidth = 800;
//int glWindowHeight = 600;
int glWindowWidth = 1200;
int glWindowHeight = 1000;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;

glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat4 lightRotation;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

glm::vec3 lightPos1;
GLuint lightPos1Loc;
glm::vec3 lightPos2;
GLuint lightPos2Loc;
glm::vec3 lightPos3;
GLuint lightPos3Loc;
glm::vec3 lightPos4;
GLuint lightPos4Loc;

glm::vec3 lightPointColor;
GLuint lightPointColorLoc;
GLuint pointLightOnLoc;

glm::vec3 nightLightColor;

GLuint textureID;

/*gps::Camera myCamera(
	glm::vec3(0.0f, 1.0f, 5.5f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f));*/

gps::Camera myCamera(
	glm::vec3(0.200329f, 0.9f, 11.1015f),
	glm::vec3(0.592025f, 0.9f, 10.8138f),
	glm::vec3(0.0f, 1.0f, 0.0f));
float cameraSpeed = 0.005f;

bool pressedKeys[1024];
float angleY = 0.0f;
float angleElice = 0.0f;
float speedElice = 1.0f;
GLfloat lightAngle;

gps::Model3D nanosuit;
gps::Model3D ground;
gps::Model3D lightCube;
gps::Model3D screenQuad;
gps::Model3D dormitor;
gps::Model3D masa_rosie;
gps::Model3D pahar;
gps::Model3D scrumiera;
gps::Model3D elice;

gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader screenQuadShader;
gps::Shader depthMapShader;
gps::Shader transparentShader;

GLuint shadowMapFBO;
GLuint depthMapTexture;

gps::SkyBox mySkyBox;
gps::Shader skyboxShader;

bool showDepthMap;
bool lightOn = true;
bool pointLightOn = true;
bool wireframeView = false;
bool smoothShading = false;
bool polygonalView = false;
bool solidView = false;
bool isRotating = false;

bool isSinusoidalMovementActive = false;
bool isSinusoidalAnimationActive = false;
glm::vec3 initialCameraPosition = myCamera.getPosition();
glm::vec3 initialCameraTarget = myCamera.getTarget();
glm::vec3 punct1(0.218494f, 0.956928f, 11.0895f);//inauntru
glm::vec3 punct2(1.11234f, 0.956928f, 10.623f);//afara

bool isMiddleMousePressed = false;
double lastMouseX, lastMouseY;

float amplitude = 2.0f;
float frequency = 0.5f;
float initialCameraX = 14.9972f;
float initialCameraZ = 4.96847f;

std::vector<glm::vec3> waypoints = {
	glm::vec3(0.35863f, 0.91f, 11.1742f),
	glm::vec3(0.429876f, 0.91f, 10.8479f),
	glm::vec3(0.650374f, 0.91f, 10.7214f),
	glm::vec3(1.17508, 0.91f, 10.5944f),
	glm::vec3(0.767401f, 0.91f, 11.0361f)
};

int currentSegment = 0;
float progress = 0.0f;
float animationDuration = 5.0f;
float elapsedTime = 0.0f;
GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO	
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		wireframeView = !wireframeView;
		if (wireframeView)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		smoothShading = !smoothShading;
		if (smoothShading) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		isSinusoidalMovementActive = !isSinusoidalMovementActive;
		myCamera.setPosition(initialCameraPosition);
		myCamera.setTarget(initialCameraTarget);
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		isSinusoidalAnimationActive = !isSinusoidalAnimationActive;
		myCamera.setPosition(initialCameraPosition);
		myCamera.setTarget(initialCameraTarget);
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		showDepthMap = !showDepthMap;
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	static bool firstMouse = true;
	if (isMiddleMousePressed) {
		if (firstMouse) {
			lastMouseX = xpos;
			lastMouseY = ypos;
			firstMouse = false;
		}
		double deltaX = xpos - lastMouseX;
		double deltaY = ypos - lastMouseY;
		float sensitivity = 0.1f;
		deltaX *= sensitivity;
		deltaY *= sensitivity;
		myCamera.rotate(deltaY, deltaX);

		lastMouseX = xpos;
		lastMouseY = ypos;
	}
	else {
		firstMouse = true;
	}
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (action == GLFW_PRESS) {
			isMiddleMousePressed = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else if (action == GLFW_RELEASE) {
			isMiddleMousePressed = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}
float cubicEaseInOut(float t) {
	return t * t * (3.0f - 2.0f * t);
}

glm::vec3 interpolateBetweenPoints(glm::vec3 start, glm::vec3 end, float t) {
	return glm::mix(start, end, t);
}
void processSinusoidalMovement() {
	float time = static_cast<float>(glfwGetTime());
	float newX = initialCameraX + amplitude * sin(frequency * time);
	float newZ = initialCameraZ + amplitude * cos(frequency * time);
	glm::vec3 newPosition = glm::vec3(newX, myCamera.getPosition().y, newZ);
	myCamera.setPosition(newPosition);
	glm::vec3 lookAtPoint = glm::vec3(0.0f, myCamera.getPosition().y, 0.0f);
	myCamera.setTarget(lookAtPoint);
}
void processSinusoidalAnimation(float deltaTime) {
	if (waypoints.empty()) return;
	glm::vec3 startPoint = waypoints[currentSegment];
	glm::vec3 endPoint = waypoints[(currentSegment + 1) % waypoints.size()];
	elapsedTime += deltaTime;
	if (elapsedTime >= animationDuration) {
		elapsedTime = 0.0f;
		currentSegment = (currentSegment + 1) % waypoints.size();
	}
	float t = glm::clamp(elapsedTime / animationDuration, 0.0f, 1.0f);
	float smoothProgress = cubicEaseInOut(t);
	glm::vec3 interpolatedPosition = interpolateBetweenPoints(startPoint, endPoint, smoothProgress);
	myCamera.setPosition(interpolatedPosition);
	glm::vec3 lookAtPoint = endPoint;
	myCamera.setTarget(lookAtPoint);
}
void processRotation() 
{
	float time = static_cast<float>(glfwGetTime());
	if (isRotating) {
		angleElice += speedElice * time;
		if (angleElice >= 360.0f) {
			angleElice -= 360.0f;
		}
	}
}
void processMovement()
{
	/*if (pressedKeys[GLFW_KEY_Q]) {
		//angleY -= 1.0f;
		myCamera.rotate(0.0f, 0.2f);
	}
	if (pressedKeys[GLFW_KEY_E]) {
		//angleY += 1.0f;
		myCamera.rotate(0.0f, -0.2f);
	}*/
	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 0.05f;
	}
	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle += 0.05f;
	}
	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_Q])
	{
		myCamera.move(gps::MOVE_UP, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_E])
	{
		myCamera.move(gps::MOVE_DOWN, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_Z]) {
		myCamera.rotate(0.0f, -0.5f);
	}
	if (pressedKeys[GLFW_KEY_X]) {
		myCamera.rotate(0.0f, 0.5f);
	}
	if (pressedKeys[GLFW_KEY_C]) {
		myCamera.rotate(0.5f, 0.0f);
	}
	if (pressedKeys[GLFW_KEY_V]) {
		myCamera.rotate(-0.5f, 0.0f);
	}
	if (isSinusoidalMovementActive) {
		processSinusoidalMovement();
	}

	if (isSinusoidalAnimationActive) {
		float deltaTime = glfwGetTime();
		glfwSetTime(0.0);
		processSinusoidalAnimation(deltaTime);
	}
	if (isRotating)
	{
		processRotation();
	}
}
bool key0Pressed = false;
bool key9Pressed = false;
bool key3Pressed;

void processInput(GLFWwindow* window) {
	if (pressedKeys[GLFW_KEY_0]) {
		if (!key0Pressed) {
			key0Pressed = true;
			lightOn = !lightOn;
			myCustomShader.useShaderProgram();
			nightLightColor = glm::vec3(0.1f, 0.1f, 0.1f);
			if (lightOn)
			{
				glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
			}
			else
			{
				glUniform3fv(lightColorLoc, 1, glm::value_ptr(nightLightColor));
			}
			transparentShader.useShaderProgram();
			if (lightOn)
			{
				glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
			}
			else
			{
				glUniform3fv(lightColorLoc, 1, glm::value_ptr(nightLightColor));
			}
		}
	}
	else {
		key0Pressed = false;
	}
	if (pressedKeys[GLFW_KEY_9])
	{
		if (!key9Pressed)
		{
			key9Pressed = true;
			pointLightOn = !pointLightOn;
			myCustomShader.useShaderProgram();
			nightLightColor = glm::vec3(0.0f, 0.0f, 0.0f);
			if (pointLightOn)
			{
				glUniform3fv(lightPointColorLoc, 1, glm::value_ptr(lightPointColor));
			}
			else
			{
				glUniform3fv(lightPointColorLoc, 1, glm::value_ptr(nightLightColor));
			}
			glUniform1i(pointLightOnLoc, pointLightOn);
			transparentShader.useShaderProgram();
			if (pointLightOn)
			{
				glUniform3fv(lightPointColorLoc, 1, glm::value_ptr(lightPointColor));
			}
			else
			{
				glUniform3fv(lightPointColorLoc, 1, glm::value_ptr(nightLightColor));
			}
			glUniform1i(pointLightOnLoc, pointLightOn);
		}
	}
	else
	{
		key9Pressed = false;
	}
	if (pressedKeys[GLFW_KEY_3]) 
	{
		if (!key3Pressed)
		{
			key3Pressed = true;
			isRotating = !isRotating;
		}
	}
	else
	{
		key3Pressed = false;
	}

}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//window scaling for HiDPI displays
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

	//for sRBG framebuffer
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

	//for antialising
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	glfwSetMouseButtonCallback(glWindow, mouseButtonCallback);
	//glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

#if not defined (__APPLE__)
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	//glEnable(GL_CULL_FACE); // cull face
	//glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects() {
	nanosuit.LoadModel("objects/nanosuit/nanosuit.obj");
	ground.LoadModel("objects/ground/ground.obj");
	lightCube.LoadModel("objects/cube/cube.obj");
	screenQuad.LoadModel("objects/quad/quad.obj");
	dormitor.LoadModel("objects/scena/scena.obj");
	masa_rosie.LoadModel("objects/masa_rosie/masa_rosie.obj");
	pahar.LoadModel("objects/pahar/pahar.obj");
	scrumiera.LoadModel("objects/scrumiera/scrumiera.obj");
	elice.LoadModel("objects/elice/elice.obj");
}

void initShaders() {
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();
	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	lightShader.useShaderProgram();
	screenQuadShader.loadShader("shaders/screenQuad.vert", "shaders/screenQuad.frag");
	screenQuadShader.useShaderProgram();
	depthMapShader.loadShader("shaders/depthMap.vert", "shaders/depthMap.frag");
	depthMapShader.useShaderProgram();
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader.useShaderProgram();
	transparentShader.loadShader("shaders/shaderTransparent.vert", "shaders/shaderTransparent.frag");
	transparentShader.useShaderProgram();
}

glm::mat4 computeLightSpaceTrMatrix() {
	//TODO - Return the light-space transformation matrix
	glm::mat4 lightView = glm::lookAt(glm::mat3(lightRotation) * punct2, punct1, glm::vec3(0.0f, 1.0f, 0.0f));
	const GLfloat near_plane = 0.0f, far_plane = 10.0f;
	glm::mat4 lightProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near_plane, far_plane);
	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;
	return lightSpaceTrMatrix;
}
void initUniforms() {
	myCustomShader.useShaderProgram();

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	lightDir = glm::normalize(punct2 - punct1);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	//lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.215954f, 0.994091f, 11.0959f));
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	//set light color
	//lightColor = glm::vec3(0.7f, 0.7f, 0.7f);
	lightColor = glm::vec3(1.0f);
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	//lightOnLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightOn");
	//glUniform1i(lightOnLoc, lightOn ? 1 : 0);

	lightPos1 = glm::vec3(0.995647f, 0.870056f, 10.9276f);
	lightPos1Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos1");
	glUniform3fv(lightPos1Loc, 1, glm::value_ptr(lightPos1));

	lightPos2 = glm::vec3(3.10567f, 0.277135f, 7.49805f);
	lightPos2Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos2");
	glUniform3fv(lightPos2Loc, 1, glm::value_ptr(lightPos2));

	lightPos3 = glm::vec3(6.61487f, 0.314494f, 10.342f);
	lightPos3Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos3");
	glUniform3fv(lightPos3Loc, 1, glm::value_ptr(lightPos3));

	lightPos4 = glm::vec3(6.34272f, 0.281135f, 8.09111f);
	lightPos4Loc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPos4");
	glUniform3fv(lightPos4Loc, 1, glm::value_ptr(lightPos4));

	lightPointColor = glm::vec3(1.0f, 1.0f, 0.0f);
	lightPointColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightPointColor");
	glUniform3fv(lightPointColorLoc, 1, glm::value_ptr(lightPointColor));

	pointLightOnLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLightOn");
	glUniform1i(pointLightOnLoc, pointLightOn ? 1 : 0);

	transparentShader.useShaderProgram();

	/*model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(transparentShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(transparentShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(transparentShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(transparentShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	lightDir = glm::normalize(punct2 - punct1);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(transparentShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	lightColor = glm::vec3(0.3f, 0.3f, 0.3f);
	lightColorLoc = glGetUniformLocation(transparentShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
	*/
	modelLoc = glGetUniformLocation(transparentShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	viewLoc = glGetUniformLocation(transparentShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	normalMatrixLoc = glGetUniformLocation(transparentShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	projectionLoc = glGetUniformLocation(transparentShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(transparentShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));
	lightColorLoc = glGetUniformLocation(transparentShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightPos1Loc = glGetUniformLocation(transparentShader.shaderProgram, "lightPos1");
	glUniform3fv(lightPos1Loc, 1, glm::value_ptr(lightPos1));
	lightPos2Loc = glGetUniformLocation(transparentShader.shaderProgram, "lightPos2");
	glUniform3fv(lightPos2Loc, 1, glm::value_ptr(lightPos2));
	lightPos3Loc = glGetUniformLocation(transparentShader.shaderProgram, "lightPos3");
	glUniform3fv(lightPos3Loc, 1, glm::value_ptr(lightPos3));
	lightPos4Loc = glGetUniformLocation(transparentShader.shaderProgram, "lightPos4");
	glUniform3fv(lightPos4Loc, 1, glm::value_ptr(lightPos4));

	lightPointColorLoc = glGetUniformLocation(transparentShader.shaderProgram, "lightPointColor");
	glUniform3fv(lightPointColorLoc, 1, glm::value_ptr(lightPointColor));
	pointLightOnLoc = glGetUniformLocation(transparentShader.shaderProgram, "pointLightOn");
	glUniform1i(pointLightOnLoc, pointLightOn ? 1 : 0);

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUseProgram(depthMapShader.shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"), 1, GL_FALSE, glm::value_ptr(computeLightSpaceTrMatrix()));

}
void  initSkybox()
{
	std::vector<const GLchar*> faces;
	faces.push_back("skybox/right.tga");
	faces.push_back("skybox/left.tga");
	faces.push_back("skybox/top.tga");
	faces.push_back("skybox/bottom.tga");
	faces.push_back("skybox/back.tga");
	faces.push_back("skybox/front.tga");
	mySkyBox.Load(faces);
}
void initFBO() {
	//TODO - Create the FBO, the depth texture and attach the depth texture to the FBO
	//generate FBO ID
	glGenFramebuffers(1, &shadowMapFBO);
	//create depth texture for FBO
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture,
		0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void drawObjects(gps::Shader shader, bool depthPass) {

	shader.useShaderProgram();

	model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	dormitor.Draw(shader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glm::vec3 rotationPoint = glm::vec3(0.354297f, 0.901578f, 11.3234f);
	glm::mat4 eliceModel = glm::mat4(1.0f);
	eliceModel = glm::translate(eliceModel, -rotationPoint);
	eliceModel = glm::rotate(eliceModel, glm::radians(angleElice), glm::vec3(0.354297f, 0.901578f, 11.3234f));
	eliceModel = glm::translate(eliceModel, rotationPoint);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(eliceModel));
	elice.Draw(shader);
}
void drawTransparentObjects(bool depthPass)
{
	transparentShader.useShaderProgram();
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(transparentShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	masa_rosie.Draw(transparentShader);
	pahar.Draw(transparentShader);
	scrumiera.Draw(transparentShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f));
	glUniformMatrix4fv(glGetUniformLocation(transparentShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}
void renderScene() {

	// depth maps creation pass
	//TODO - Send the light-space transformation matrix to the depth map creation shader and
	//		 render the scene in the depth map

	depthMapShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	drawObjects(depthMapShader, showDepthMap);
	glDisable(GL_DEPTH_TEST);
	drawTransparentObjects(showDepthMap);
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// render depth map on screen - toggled with the M key

	if (showDepthMap) {
		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT);

		screenQuadShader.useShaderProgram();

		//bind the depth map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

		glDisable(GL_DEPTH_TEST);
		screenQuad.Draw(screenQuadShader);
		glEnable(GL_DEPTH_TEST);
	}
	else {

		// final scene rendering pass (with shadows)

		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myCustomShader.useShaderProgram();

		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		//bind the shadow map
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(computeLightSpaceTrMatrix()));

		drawObjects(myCustomShader, false);
		mySkyBox.Draw(skyboxShader, view, projection);

		transparentShader.useShaderProgram();
		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));
		glUniform1i(glGetUniformLocation(transparentShader.shaderProgram, "shadowMap"), 3);

		glUniformMatrix4fv(glGetUniformLocation(transparentShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(computeLightSpaceTrMatrix()));
		drawTransparentObjects(false);
	}
}

void cleanup() {
	glDeleteTextures(1, &depthMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &shadowMapFBO);
	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}

int main(int argc, const char* argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}

	initOpenGLState();
	initObjects();
	initSkybox();
	initShaders();
	initUniforms();
	initFBO();

	glCheckError();

	while (!glfwWindowShouldClose(glWindow)) {
		processMovement();
		processInput(glWindow);
		renderScene();
		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	cleanup();

	return 0;
}
