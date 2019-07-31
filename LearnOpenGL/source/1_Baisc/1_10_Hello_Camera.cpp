#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader_s.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float maxInput(GLFWwindow* window);

int screenWidth = 800;
int screenHeight = 600;

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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to Load GL" << std::endl;
		return -1;
	}

	//---------------------------Texture------------------------------
	unsigned int texture[2];
	glGenTextures(2, texture);

	// Texture 1
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	//纹理环绕方式   纹理过滤方式 linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("G:/GitHub/OpenGL/LearnOpenGL_Zh/LearnOpenGL/resource/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Texture 2
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("G:/GitHub/OpenGL/LearnOpenGL_Zh/LearnOpenGL/resource/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	//---------------------------Shader-------------------------------
	const char* vertexShaderPath = "G:/GitHub/OpenGL/LearnOpenGL_Zh/LearnOpenGL/Shaders/3_coordinateSystems_3d.vs";
	const char* fragmentShaderPath = "G:/GitHub/OpenGL/LearnOpenGL_Zh/LearnOpenGL/Shaders/3_coordinateSystems_3d.fs";
	Shader OurShader(vertexShaderPath, fragmentShaderPath);


	//---------------------------Geometry-------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//---------------------------Buffer-------------------------------
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// TexCoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);





	//---------------------------Uniform-------------------------------
	float offset = 0;
	//设置每个着色器采样器属于哪个纹理单元,这个只设置一次即可。
	OurShader.use();
	OurShader.setInt("texture0", 0);
	OurShader.setInt("texture1", 1);//可以直接输入数字

	

	//------------------------------scene--------------------------------
	glm::vec3 cubePositions[] = {
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	
	//-----------------------------RENDER LOOP---------------------------------
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.3f, 0.5f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//《纹理单元》
		//一个纹理的位置值通常称为一个纹理单元，通过把纹理单元赋值给采样器，使用glActiveTexture激活纹理单元，传入我们需要使用的纹理单元。（每个纹理单元可采样一张图片）

		//绑定两个纹理到对应的纹理单元，然后定义哪个uniform采样器对应哪个纹理单元
		glActiveTexture(GL_TEXTURE0);//以通过GL_TEXTURE0 + 8的方式获得GL_TEXTURE8
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE0+1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		
		glBindVertexArray(VAO);
		
	
		
		//----------------------------------------- Camera -------------------------------------------
		// 1 摄像机位置  +Z轴指向屏幕（原始）
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		// 2  摄像机方向 这里是指向摄像机的矢量
		glm::vec3 cameraTarget = glm:: vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
		// 3 右轴
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		// 4 上轴
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
		// 5 LookAt 使用3个相互垂直（或非线性）的轴定义了一个坐标空间，你可以用这3个轴外加一个平移向量来创建一个矩阵，并且你可以用这个矩阵乘以任何向量来将其变换到那个坐标空间。这正是LookAt矩阵所做的。
		/*
		glm::mat4 view_lookat;
		view_lookat = glm::mat4(cameraRight, 0.0f,
								cameraUp, 0.0f,
								cameraDirection, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f);
		*/
		// GLM提供了LookAt矩阵创建
		glm::mat4 view_glm_lookat;
		view_glm_lookat = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),//摄像机位置
			glm::vec3(0.0f, 0.0f, 0.0f),//目标位置
			glm::vec3(0.0f, 1.0f, 0.0f));//世界空间向上向量


		// Example
		float radius = 10.f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view;
		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	
		//----------------------------------- Camera  End --------------------------------------


		// 3. Projection Matrix (Orthographic vs Perspective)            From Camera(Eye) Space To Clip Space
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / screenHeight, 0.1f, 100.f);

		//Depth Testing
		glEnable(GL_DEPTH_TEST);

		OurShader.use();

		unsigned int viewLoc = glGetUniformLocation(OurShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		unsigned int projectLoc = glGetUniformLocation(OurShader.ID, "projection");
		glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(projection));

		
		offset = 0.5f + maxInput(window);
		OurShader.setFloat("offset", offset);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float myTime = (float)glfwGetTime();
			float myAngle = 10.0f * i + 35.0f;
			model = glm::rotate(model, myAngle, glm::vec3(glm::sin(myTime*0.56), glm::cos(myTime*0.95), glm::sin(myTime*0.56) *  glm::cos(myTime*0.95 + myTime)));
			
			unsigned int modelLoc = glGetUniformLocation(OurShader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		
		


	

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	//---------------------------Close-------------------------------
	glfwTerminate();
	return 0;
}

float maxInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		return 0.01f;
	}else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		return -0.01f;
	}
	return 0.0f;
}