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

void display(void) { g_pMainWindow->renderFrame();  }
void idle(void) { g_pMainWindow->idle(); }
void reshape(int width, int height) { ImGui_ImplGLUT_ReshapeFunc(width, height);  g_pMainWindow->resize(width, height); }
void keyboard(unsigned char key, int x, int y) { ImGui_ImplGLUT_KeyboardFunc(key, x, y); g_pMainWindow->keyEvent(key, x, y); }
void mouse(int button, int state, int x, int y) { ImGui_ImplGLUT_MouseFunc(button, state, x, y); g_pMainWindow->mouseButtonEvent(button, state, x, y); }
void move(int x, int y) { ImGui_ImplGLUT_MotionFunc(x, y); g_pMainWindow->mouseMoveEvent(x, y); }

char* chooseFilename() {
	std::cout << "Select model:\n"
		<< "1. cow.obj\n"
		<< "2. cube.obj\n"
		<< "3. sphere.obj\n"
		<< "4. suzanne.obj\n"
		<< "5. teapot.obj\n"
		<< "0. Exit\n";
	std::string input;

	while (true) {
		std::cout << "Enter model index (1-5, 0 to exit): ";
		std::cin >> input;
		if (input.length() == 1) {
			char index = input[0];
			switch (index) {
			case '1':
				std::cout << "Selected model: input\\cow.obj" << std::endl;
				return "input\\cow.obj";
				break;
			case '2':
				std::cout << "Selected model: input\\cube.obj" << std::endl;
				return "input\\cube.obj";
				break;
			case '3':
				std::cout << "Selected model: input\\sphere.obj" << std::endl;
				return "input\\sphere.obj";
				break;
			case '4':
				std::cout << "Selected model: input\\suzanne.obj" << std::endl;
				return "input\\suzanne.obj";
				break;
			case '5':
				std::cout << "Selected model: input\\teapot.obj" << std::endl;
				return "input\\teapot.obj";
				break;
			case '0':
				std::cout << "Exiting..." << std::endl;
				exit(0);
			default:
				std::cout << "Invalid input. Please enter a valid model index." << std::endl;
				break;
			}
		}
		else {
			std::cout << "Invalid input. Please enter a single character." << std::endl;
		}
	}
	return 0;
}

int main(int argc, char* argv[])
{

	while (true) {
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

		const char* filename = chooseFilename();

		g_pMainWindow = new ViewerWindow(filename);

		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keyboard);
		glutMouseFunc(mouse);
		glutMotionFunc(move);
		glutIdleFunc(idle);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGLUT_Init();
		ImGui_ImplOpenGL3_Init();

		glutMainLoop();
	}

	return 0;
}


