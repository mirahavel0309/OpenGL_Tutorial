#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <openglErrorReporting.h>
#include <shader.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


float triangleData[] = {
	//positions         colors
	//x  y  z           r  g  b
	 0.5, 0.5, 0,       1, 0, 0,  // vertex 1
	-0.5, 0.5, 0,       0, 1, 0,   // vertex 2
	-0.5, -0.5, 0,      0, 0, 1,  // vertex 3
	 0.5, -0.5, 0,      0, 0, 1 , // vertex 4
};

unsigned short indices[] =
{
	0,1,2,	//first triangle
	0,2,3	//second triangle
};


int main(void)
{


	if (!glfwInit())
	{
		std::cout << "GLFW Init Error!\n";
		return 1;
	}

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

	GLFWwindow *window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		std::cout << "Window Error!\n";
		return 1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return 1;
	}

	enableReportGlErrors();

#pragma region imgui
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::GetIO().FontGlobalScale = 2;
#pragma endregion


#pragma region vao

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

#pragma endregion


#pragma region buffer
	//create the buffer
	GLuint buffer = 0;
	glGenBuffers(1, &buffer);

	//send the data to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), triangleData, GL_STATIC_DRAW);

	//the atribute representing position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));

#pragma endregion

#pragma region index buffer

	//create the buffer
	GLuint indexBuffer = 0;
	glGenBuffers(1, &indexBuffer);

	//send the data to the buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


#pragma endregion


	//it is good practice to unbind your vao after you are done
	glBindVertexArray(0);


#pragma region shader loading
	Shader shader;

	shader.loadShaderProgramFromFile("resources/myshader.vert", "resources/myshader.frag");

	shader.bind();

	GLint u_time = shader.getUniformLocation("u_time");
	GLint u_color = shader.getUniformLocation("u_color");

#pragma endregion


	while (!glfwWindowShouldClose(window))
	{
		//tell OpenGL that we resized our window.
		int w = 0, h = 0;
		glfwGetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);


		glClear(GL_COLOR_BUFFER_BIT);

	#pragma region imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	#pragma endregion


		ImGui::Begin("Window");
		ImGui::Text("Color test");
		static float color[3] = {0.5,0.5,0.5};
		ImGui::ColorPicker3("Color: ", color);
		ImGui::End();

		//ImGui::ShowDemoWindow();

		//make sure the shader is bound
		shader.bind();
		//glUniform1f(u_time, (float)(clock()) / 100.f);

		glUniform3fv(u_color, 1, color);

		//bind the vao to specify the vertex buffer, index buffer, and the atributes.
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);






	#pragma region imgui
		ImGui::Render();
		int display_w = 0, display_h = 0;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	#pragma endregion

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}

