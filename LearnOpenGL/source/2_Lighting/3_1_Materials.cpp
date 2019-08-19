#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader_s.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int screenWidth = 800;
int screenHeight = 600;

//时间变量
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 摄像机变量
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Camera
camera myCamera(cameraPos, cameraFront, cameraUp,screenWidth, screenHeight);

// Light Pos
glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

int main()
{
	//---------------------------Window-------------------------------
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Hello Triangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to Create Windows" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);//设置鼠标移动事件的回调函数——控制视角
	glfwSetScrollCallback(window, scroll_callback);  //设置鼠标滚轮事件的回调函数——控制fov
	
	//捕获光标并隐藏光标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//加载OpenGL Function Pointer
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to Load GL" << std::endl;
		return -1;
	}

	//---------------------------Render Settings---------------------------
	//Depth Testing
	glEnable(GL_DEPTH_TEST);


	//---------------------------Shader-------------------------------
	const char* vertexShaderPath = "G:/GitHub/OpenGL/LearnOpenGL_Zh/LearnOpenGL/Shaders/8_materials.vs";
	const char* fragmentShaderPath = "G:/GitHub/OpenGL/LearnOpenGL_Zh/LearnOpenGL/Shaders/8_materials.fs";
	const char* lightFragmentShaderPath = "G:/GitHub/OpenGL/LearnOpenGL_Zh/LearnOpenGL/Shaders/4_colors_light.fs";
	const char* lightVertexShaderPath = "G:/GitHub/OpenGL/LearnOpenGL_Zh/LearnOpenGL/Shaders/4_colors_light.vs";
	Shader OurShader(vertexShaderPath, fragmentShaderPath);
	Shader LightShader(lightVertexShaderPath, lightFragmentShaderPath);

	//---------------------------Geometry-------------------------------
	float vertices[] = {
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	//---------------------------Buffer-------------------------------
	unsigned int VBO, VAO, lightVAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	
	//-----------------------------RENDER LOOP---------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// 1 Update Time
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// 2 Input
		processInput(window);

		// 3 Buffer Clear
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//lightPos = glm::vec3(glm::sin(glfwGetTime()) * 2.0f, 2.0f , glm::cos(glfwGetTime()) * 2.0f);

		// 5 Activate Shader
		OurShader.use();
		OurShader.setVec3("light.position", lightPos);
		OurShader.setVec3("viewPos", myCamera.getCameraPosWS());
		float x = myCamera.getCameraPosWS().x;
		float y = myCamera.getCameraPosWS().y;
		float z = myCamera.getCameraPosWS().z;
		//std::cout << "camera position : " << x << "   " << y << "    " << z << std::endl;
		// light properties
		glm::vec3 lightColor = glm::vec3(1.0f);
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		OurShader.setVec3("light.ambient", ambientColor);
		OurShader.setVec3("light.diffuse", diffuseColor);
		OurShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		
		

		OurShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		OurShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		OurShader.setVec3("material.sepcular", glm::vec3(0.5f, 0.5f, 0.5f));
		OurShader.setFloat("material.shininess", 32.0f);


		glm::mat4 projection;
		projection = myCamera.getCameraPerspMatrix();
		OurShader.setMat4("projection", projection);

		glm::mat4 view;
		view = myCamera.getCameraViewMatrix();
		OurShader.setMat4("view", view);

		glm::mat4 model;
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f,10.0f,10.0f));
		OurShader.setMat4("model", model);

		
		
		// 9 render geo
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		// 10 render light
		LightShader.use();
		glm::mat4 lightModel;
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f, 0.2f, 0.2f));
		LightShader.setMat4("model", lightModel);
		LightShader.setVec3("lightColor", glm::vec3(1.0f));
		LightShader.setMat4("projection", projection);
		LightShader.setMat4("view", view);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 11
		glfwSwapBuffers(window);
		glfwPollEvents();


		//std::cout << "frame time : " << deltaTime << " FPS : " << floor(1 / deltaTime) << std::endl;
	}
	
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	//---------------------------Close-------------------------------
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		myCamera.cameraMotion(motionInput::forward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		myCamera.cameraMotion(motionInput::backward, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		myCamera.cameraMotion(motionInput::lift, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		myCamera.cameraMotion(motionInput::right, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		myCamera.cameraMotion(motionInput::up, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		myCamera.cameraMotion(motionInput::down, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	myCamera.cameraRotation(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	myCamera.cameraZoom(yoffset);
}