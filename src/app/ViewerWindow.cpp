#include "app/ViewerWindow.h"
#include "core/ArcBall.h"
#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <assert.h>
#include <sys/timeb.h>
#include "meshConverter\Mesh3D.h"
#include <iostream>


ViewerWindow::ViewerWindow(const char* filename)
{
	mesh = new Mesh3D();
	mesh->readOBJ(filename);
}

ViewerWindow::~ViewerWindow(void) {}

void ViewerWindow::renderGeometry() {
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mesh->draw();

}

void ViewerWindow::keyEvent(unsigned char key, int x, int y) {
	switch (key) {
	case '1':	// Ein Stip mehr anzeigen
		if (mesh->strip_amount_limit != mesh->getStripsCount()) {
			mesh->strip_amount_limit++;
		}
		break;
	case '2':	// Ein Stip weniger anzeigen
		if (mesh->strip_amount_limit != 0) {
			mesh->strip_amount_limit--;
		}
		break;
	case '3':	// Alle oder keine Stripes anzeigen
		if (mesh->strip_amount_limit > 0) {
			mesh->strip_amount_limit = 0;
		}
		else {
			mesh->strip_amount_limit = mesh->getStripsCount();
		}
		break;
	}
}
