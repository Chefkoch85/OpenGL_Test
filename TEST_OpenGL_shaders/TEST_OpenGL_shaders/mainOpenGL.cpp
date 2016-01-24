
#include "OpenGLUtility.h"

#include "Control.h"

#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <random>
#include <ctime>

#include "ShaderProgram.h"

int WinWidth = 800, WinHeight = 600;
float WinRatio = 0.0f;

void onError(int error, const char* description)
{
	fputs(description, stderr);
}

void onKey(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
		glfwSetWindowShouldClose(win, GL_TRUE);

}

void onResize(GLFWwindow* win, int w, int h)
{
	//Cout << "onResize(" << w << ", " << h << ")" << Cnl;
	if (w < 640)
		w = 640;

	if (h < 480)
		h = 480;

	glfwSetWindowSize(win, w, h);

	glfwGetFramebufferSize(win, &WinWidth, &WinHeight);
	WinRatio = (float)WinWidth / (float)WinHeight;

	Cout << "onResize(" << w << ", " << h << ") : " << WinRatio << Cnl;
}

void DrawQuad()
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-50.0f, -50.0f, 0.f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(50.0f, -50.0f, 0.f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(50.0f, 50.0f, 0.f);

	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(-50.0f, 50.0f, 0.f);
}
void DrawCircle()
{
	glColor3f(1.0f, 0.0f, 0.0f);

	//glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-25.0f, -45.0f, 0.f);

	//glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(25.0f, -45.0f, 0.f);

	glVertex3f(55.0f, 0.0f, 0.f);

	//glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(25.0f, 45.0f, 0.f);

	//glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(-25.0f, 45.0f, 0.f);

	glVertex3f(-55.0f, 0.0f, 0.f);

	glVertex3f(0.0f, 0.0f, 0.0f);
}

struct SVertex
{
	glm::vec3 Position;
	glm::vec3 Color;
};

int main(int argc, char** argv)
{	
	// set on-error callback
	glfwSetErrorCallback(onError);

	// init GLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	// window hint, how sould the windoe and context be (set before create a window)
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* mainWin = glfwCreateWindow(WinWidth, WinHeight, "GLFW/OpenGL - Test", nullptr, nullptr);
	if (!mainWin)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetWindowPos(mainWin, 800, 50);

	// set window callbacks
	glfwSetKeyCallback(mainWin, onKey);
	glfwSetWindowSizeCallback(mainWin, onResize);

	// to Init the WinRatio
	onResize(mainWin, WinWidth, WinHeight);

	// make window curent context for OpenGL
	glfwMakeContextCurrent(mainWin);
	
	// init Glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// set swapinterval to monitor refresh rate
	glfwSwapInterval(1);

	// get GL-version informations
	int i1 = 0, i2 = 0;
	CStr glVersionString = (char*)glGetString(GL_VERSION);
	CStr glVersion = "GL-Version: ";
	i1 = glVersionString.find_first_of(' ');
	glVersion += glVersionString.substr(0, i1);
	CStr Graphics = "Graphics-Driver: ";
	Graphics += glVersionString.substr(i1+1);
	glVersionString = Graphics + "\t\t" + glVersion;
	Cout << glVersionString << Cnl;
	Cout << "Compiled against GLFW: " << GLFW_VERSION_MAJOR << "." << GLFW_VERSION_MINOR << "." << GLFW_VERSION_REVISION << Cnl;
	Cout << "Running  against GLFW: " << glfwGetVersionString() << Cnl;
		
	// shaders
	CShaderProgram prog;
	prog.Load("..\\RES\\SHADER\\SimpleColor.vs", "..\\RES\\SHADER\\SimpleColor.fs");

	// set clear color
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);

	// draw object
	GLuint VertexArrayID = 0;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	std::vector<glm::vec3> VertexData = {
		glm::vec3(-1.0f,-1.0f,0.0f),
		glm::vec3( 1.0f,-1.0f,0.0f),
		glm::vec3( 0.0f, 0.0f,0.0f),

		glm::vec3( 0.0f, 0.0f,0.0f),
		glm::vec3( 1.0f, 1.0f,0.0f),
		glm::vec3(-1.0f, 1.0f,0.0f),
	};

	std::srand((unsigned int)std::time(nullptr));
	std::vector<glm::vec3> VertexColor(VertexData.size());
	int s = VertexColor.capacity();
	for (int v = 0; v < (int)VertexData.size(); v++)
	{
		VertexColor.push_back(glm::vec3());
		VertexColor[v].r = (std::rand() % 255) / 255.0f;
		VertexColor[v].g = (std::rand() % 255) / 255.0f;
		VertexColor[v].b = (std::rand() % 255) / 255.0f;
	}
	
	GLuint VertexBufferID = 0;
	glGenBuffers(1, &VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, VertexData.size() * sizeof(glm::vec3), &VertexData[0], GL_STATIC_DRAW);

	GLuint ColorBufferID = 0;
	glGenBuffers(1, &ColorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferID);
	glBufferData(GL_ARRAY_BUFFER, VertexColor.size() * sizeof(glm::vec3), &VertexColor[0], GL_STATIC_DRAW);

	// Control
	CControl::Instance()->Init({ 0,0,5 }, mainWin);

	// activate Alphablending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// main loop
	float fTime = 0.0f, lastTime = 0.0f;
	while (!glfwWindowShouldClose(mainWin))
	{
		CControl::Instance()->Update(0.1f);

		prog.SetViewMatrix(CControl::Instance()->GetView());
		prog.SetProjMatrix(CControl::Instance()->GetProj());

		// change vertex color every second
		if (fTime > 1.0f && false)
		{	
			fTime = 0.0f;
			for (int v = 0; v < 6; v++)
			{
				VertexColor[v].r = (std::rand() % 256) / 255.0f;
				VertexColor[v].g = (std::rand() % 256) / 255.0f;
				VertexColor[v].b = (std::rand() % 256) / 255.0f;
			}

			//glBindBuffer(GL_ARRAY_BUFFER, ColorBufferID);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColor), glm::value_ptr(VertexColor[0]), GL_STATIC_DRAW);
		}

		fTime += 0.01f;

		// draw
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		prog.Use();

		prog.SetModelMatrix(glm::translate(glm::mat4(), glm::vec3(1.5, 0, 0)));
		prog.SetAlpha(1.0f);
		prog.AppliyMVP();
		
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, ColorBufferID);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		prog.SetModelMatrix(glm::translate(glm::mat4(), glm::vec3(-1.5, 0, 0)));
		prog.SetAlpha(0.3f);
		prog.AppliyMVP();

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, ColorBufferID);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(mainWin);
		glfwPollEvents();
	}


	glfwDestroyWindow(mainWin);
	glfwTerminate();

	return 0;
}

