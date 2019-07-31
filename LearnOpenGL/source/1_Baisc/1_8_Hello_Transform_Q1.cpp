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

int main()
{
	//---------------------------Window-------------------------------
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
	const char* vertexShaderPath = "G:/GitHub/OpenGL/LearnOpenGL_Zh/LearnOpenGL/Shaders/2_transform.vs";
	const char* fragmentShaderPath = "G:/GitHub/OpenGL/LearnOpenGL_Zh/LearnOpenGL/Shaders/2_transform.fs";
	Shader OurShader(vertexShaderPath, fragmentShaderPath);


	//---------------------------Geometry-------------------------------
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // 左上
	};

	unsigned int index[] = {// 注意索引从0开始!
		0, 1, 3, 1, 2, 3
	};

	//---------------------------Buffer-------------------------------
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// TexCoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	//---------------------------Uniform-------------------------------
	float offset = 0;
	//设置每个着色器采样器属于哪个纹理单元,这个只设置一次即可。
	OurShader.use();
	OurShader.setInt("texture0", 0);
	OurShader.setInt("texture1", 1);//可以直接输入数字

	





	//-----------------------------RENDER LOOP---------------------------------
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.3f, 0.5f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//《纹理单元》
		//一个纹理的位置值通常称为一个纹理单元，通过把纹理单元赋值给采样器，使用glActiveTexture激活纹理单元，传入我们需要使用的纹理单元。（每个纹理单元可采样一张图片）

		//绑定两个纹理到对应的纹理单元，然后定义哪个uniform采样器对应哪个纹理单元
		glActiveTexture(GL_TEXTURE0);//以通过GL_TEXTURE0 + 8的方式获得GL_TEXTURE8
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE0+1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		
		
		//---------------------------Transform-------------------------------
		glm::mat4 trans;
		float rotAngle = (float)glfwGetTime();
		// 先旋转 再位移
		trans = glm::rotate(trans, rotAngle, glm::vec3(0.0, 0.0, 1.0));
		trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 1.0f));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

		OurShader.use();
		unsigned int transLocation = glGetUniformLocation(OurShader.ID, "transform");
		glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(trans));
		//----------------------Transform-----End-------------------------------

		offset += maxInput(window);
		OurShader.setFloat("offset", offset);
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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