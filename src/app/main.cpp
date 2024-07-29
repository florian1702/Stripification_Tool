// TerraGen.cpp : Defines the entry point for the console application.
//
#include"imgui.h"
#include"imgui_impl_glut.h"
#include"imgui_impl_opengl3.h"

#include "ViewerWindow.h"
#include <GL/glut.h>
#include <time.h>
#include <iostream>

CGlutWindow* g_pMainWindow = 0;

void display(void)										{ g_pMainWindow->renderFrame();  }
void idle(void)											{ g_pMainWindow->idle(); }
// --> Florian
void reshape(int width, int height)						{ ImGui_ImplGLUT_ReshapeFunc(width, height);  g_pMainWindow->resize(width, height); }
void keyboard(unsigned char key, int x, int y)			{ ImGui_ImplGLUT_KeyboardFunc(key, x, y); g_pMainWindow->keyEvent(key, x, y); }
void mouse(int button, int state, int x, int y)			{ ImGui_ImplGLUT_MouseFunc(button, state, x, y); g_pMainWindow->mouseButtonEvent(button, state, x, y); }
void move(int x, int y)									{ ImGui_ImplGLUT_MotionFunc(x, y); g_pMainWindow->mouseMoveEvent(x, y); }
// <--

int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	g_pMainWindow = new ViewerWindow();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(move);
	glutIdleFunc(idle);

	// --> Florian
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGLUT_Init();
	ImGui_ImplOpenGL3_Init();
	// <--

	glutMainLoop();

	return 0;
}


