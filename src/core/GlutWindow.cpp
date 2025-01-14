#include"imgui.h"
#include"imgui_impl_glut.h"
#include"imgui_impl_opengl3.h"

#include "GL\glew.h"
#include "GlutWindow.h"
#include "ArcBall.h"

#include <math.h>
#include <GL/glut.h>
#include <sys/timeb.h>
#include <time.h>
#include <stdio.h>

#define DEG2RAD(a) ((a)/57.295)

#define ZOOM_MIN (0.05)
#define ZOOM_MAX (10.0)
//#define DISABLE_DRAGGING

CGlutWindow::CGlutWindow(void)
{
	m_pCameraArcball = new CArcBall();
	m_pLightArcball = new CArcBall();


	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Stripification Tool");

	m_dFieldOfView = 30.0;
	m_nWidth = 1200.f;
	m_nHeight = 800.f;

	m_dSceneRadius = 15.0;
	m_vecCameraPosition.set(0.0, 0.0, 0.0, 1.0);

	m_dCenter = m_dSceneRadius / sin(DEG2RAD(m_dFieldOfView) / 2.0);

	m_dZoom = 1.0;
	m_dTranslateX = 0.0;
	m_dTranslateY = 0.0;
	m_nMouseState = 0;

	m_bDragging = false;
	m_bZooming = false;
	m_bRotating = false;
	m_bExternalMouseHandling = false;

	initializeGL();
}

CGlutWindow::~CGlutWindow(void)
{
	// <Florian>
	// Bereinige ImGui-Ressourcen
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
	// </Florian>
	if (NULL != m_pCameraArcball) delete m_pCameraArcball;
	if (NULL != m_pLightArcball)  delete m_pLightArcball;
}


void CGlutWindow::renderFrame() {

	float dAspectRatio = m_nWidth / m_nHeight;

	// The usual OpenGL stuff to clear the screen and set up viewing.
	glClearColor(0.2, 0.2, 0.2, 1.);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// <Florian>
	// Beginne einen neuen ImGui-Frame f�r OpenGL3 und GLUT
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGLUT_NewFrame();
	ImGui::NewFrame();
	// </Florian>

	GLfloat fTop, fRight, fNear, fFar;

	fNear = float(m_dCenter - m_dSceneRadius);
	fFar = float(m_dCenter + m_dSceneRadius);

	if (dAspectRatio > 1.0) {
		fRight = fNear * float(tan(DEG2RAD(m_dFieldOfView) / 2.0) / m_dZoom);
		fTop = fRight * float(dAspectRatio);
	}
	else {
		fTop = fNear * float(tan(DEG2RAD(m_dFieldOfView) / 2.0) / m_dZoom);
		fRight = fTop / float(dAspectRatio);
	}

	glViewport(0, 0, m_nWidth, m_nHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-fTop, fTop, -fRight, fRight, fNear, 10 * fFar);

	CMatrix matModel = m_pCameraArcball->getRotatonMatrix().getInverse();

	double matrix[16];
	matModel.get(matrix[0], matrix[4], matrix[8], matrix[12],
		matrix[1], matrix[5], matrix[9], matrix[13],
		matrix[2], matrix[6], matrix[10], matrix[14],
		matrix[3], matrix[7], matrix[11], matrix[15]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(m_dTranslateX * m_dSceneRadius / m_dZoom, m_dTranslateY * m_dSceneRadius / m_dZoom, -m_dCenter);
	glMultMatrixd(matrix);
	//glTranslated(m_vecCameraPosition[0], m_vecCameraPosition[1], m_vecCameraPosition[2]);

	glClear(GL_COLOR_BUFFER_BIT);

	renderGeometry();

	// <Florian>
	// Beginne das Rendering der ImGui-Oberfl�che
	renderGui();
	// </Florian>

	glutSwapBuffers();
}


void CGlutWindow::resize(int width, int height) {
	m_nWidth = width;
	m_nHeight = height;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	m_pCameraArcball->place(CPosition(width / 2, height / 2),
		0.5 * sqrt(double(width * width + height * height)));
}

void CGlutWindow::keyEvent(unsigned char key, int x, int y) {
}

void CGlutWindow::mouseButtonEvent(int button, int state, int x, int y) {

	if ((false == m_bDragging) && (false == m_bZooming) && (false == m_bRotating)) {
		if (handleButtonEvent(button, state, x, y)) {
			m_bExternalMouseHandling = true;
			return;
		}
	}

	m_bExternalMouseHandling = false;

	CArcBall* pArcball = NULL;
	switch (m_nInteractionMode) {
	default:
	case MOVE_CAMERA:
		pArcball = m_pCameraArcball;
		break;
	case MOVE_LIGHT:
		pArcball = m_pLightArcball;
		break;
	}

	switch (state) {
	case GLUT_DOWN:
	{
		m_vecMouseDown.set(double(x) / double(m_nWidth), double(y) / double(m_nHeight));

		switch (button) {
		case GLUT_LEFT_BUTTON:
		{
			m_nMouseState |= (MBUTTON_LEFT);
			pArcball->setMouse(CPosition(x, m_nHeight - 1 - y));
			pArcball->startDragging();
			m_bRotating = true;
		}
		break;
		case GLUT_RIGHT_BUTTON:
		{
			m_nMouseState |= (MBUTTON_RIGHT);
			m_bDragging = false;
			m_bZooming = true;
		}
		break;
		case GLUT_MIDDLE_BUTTON:
		{
			m_nMouseState |= (MBUTTON_MIDDLE);
#ifdef DISABLE_DRAGGING
			m_bDragging = false;
#else
			m_bDragging = true;
#endif
			m_bZooming = false;
		}
		break;
		default:
			break;
		}
		break;
	case GLUT_UP:
	{
		switch (button) {
		case GLUT_LEFT_BUTTON:
			m_nMouseState ^= MBUTTON_LEFT;
			pArcball->finishDragging();
			break;
		case GLUT_RIGHT_BUTTON:
			m_nMouseState ^= MBUTTON_RIGHT;
			break;
		case GLUT_MIDDLE_BUTTON:
			m_nMouseState ^= MBUTTON_MIDDLE;
			break;
		default:
			break;
		}
		m_bRotating = false;
		m_bDragging = false;
		m_bZooming = false;
		m_bExternalMouseHandling = false;
	}
	}


	}
}

void CGlutWindow::mouseMoveEvent(int x, int y) {

	//if ((false == m_bDragging) && (false == m_bZooming) && (false == m_bRotating) && (m_bExternalMouseHandling) {
	if (m_bExternalMouseHandling) {
		if (handleMoveEvent(x, y)) {
			glutPostRedisplay();
		}
		return;
	}


	CArcBall* pArcball = NULL;
	switch (m_nInteractionMode) {
	default:
	case MOVE_CAMERA:
		pArcball = m_pCameraArcball;
		break;
	case MOVE_LIGHT:
		pArcball = m_pLightArcball;
		break;
	}

	CPosition pos(double(x) / double(m_nWidth), double(y) / double(m_nHeight));

	CPosition drag = pos - m_vecMouseDown;

	if ((m_bRotating) && (m_nMouseState & (MBUTTON_LEFT))) {
		pArcball->setMouse(CPosition(x, m_nHeight - 1 - y));
		glutPostRedisplay();

	}
	else if ((m_bDragging) && (m_nMouseState & MBUTTON_MIDDLE)) {

		m_dTranslateX += 2.0 * drag[0];
		m_dTranslateY += -2.0 * drag[1];

		if (m_dTranslateX > m_dZoom) m_dTranslateX = m_dZoom;
		if (m_dTranslateX < -m_dZoom) m_dTranslateX = -m_dZoom;
		if (m_dTranslateY > m_dZoom) m_dTranslateY = m_dZoom;
		if (m_dTranslateY < -m_dZoom) m_dTranslateY = -m_dZoom;
		glutPostRedisplay();
	}
	else if ((m_bZooming) && (m_nMouseState & MBUTTON_RIGHT)) {

		double dZoom = 10.0 * drag[0];
		m_dZoom += dZoom;
		if (m_dZoom < ZOOM_MIN) m_dZoom = ZOOM_MIN;
		if (m_dZoom > ZOOM_MAX) m_dZoom = ZOOM_MAX;
		if (m_dTranslateX > m_dZoom) m_dTranslateX = m_dZoom;
		if (m_dTranslateX < -m_dZoom) m_dTranslateX = -m_dZoom;
		if (m_dTranslateY > m_dZoom) m_dTranslateY = m_dZoom;
		if (m_dTranslateY < -m_dZoom) m_dTranslateY = -m_dZoom;
		glutPostRedisplay();
	}
	m_vecMouseDown = pos;
}

void CGlutWindow::idle() {

	glutPostRedisplay();
}

void CGlutWindow::initializeGL()
{

	glewInit();

	GLfloat ambient[] =
	{ 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular[] =
	{ 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat lmodel_ambient[] =
	{ 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat local_view[] =
	{ 0.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	/* Select Color and Projection*/
	glClearColor(1.0, 1.0, 1.0, 0.0);
	//glColor3f(1.0, 1.0, 1.0);
	glShadeModel(GL_SMOOTH);


}

void CGlutWindow::renderGeometry() {

}

bool CGlutWindow::handleButtonEvent(int button, int state, int x, int y) {
	return false;
}

bool CGlutWindow::handleMoveEvent(int x, int y) {
	return false;
}

// <Florian>
void CGlutWindow::renderGui(){
	
}
// </Florian>