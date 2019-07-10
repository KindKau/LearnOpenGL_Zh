#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader_s.h"

#include <iostream>


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
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
	
	const char* vertexShaderPath = "G:/GitHub/OpenGL/LearnOpenGL_Zh/LearnOpenGL/Shaders/vertex.vs";
	const char* fragmentShaderPath = "G:/GitHub/OpenGL/LearnOpenGL_Zh/LearnOpenGL/Shaders/fragment.fs";
	shader OurShader(vertexShaderPath, fragmentShaderPath);


	float vertices[] = {
	-0.5f, -0.5f, 0.0f,  1.0f,0.0f,0.0f ,// left  
	 0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f ,// right 
	 0.0f,  0.5f, 0.0f , 0.0f,0.0f,1.0f // top   
	};


	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	//uniform
	float offset = 0;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.3f, 0.5f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		OurShader.use();

		offset += 0.001f;
		OurShader.setFloat("offset", offset);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}