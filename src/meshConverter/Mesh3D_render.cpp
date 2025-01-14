
#include "Mesh3D.h"

#include <Windows.h>
#include <GL/glew.h>
#include <GL\GL.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void Mesh3D::createVertexBuffer() {

	int size = vertices.size() * 8;
	float* vertexBuffer = new float[size];

	vector<Vertex>::iterator it = vertices.begin();

	int vbIndex = 0;
	while (it != vertices.end()) {
		Vertex v = (*it);
		vertexBuffer[vbIndex++] = v.px;
		vertexBuffer[vbIndex++] = v.py;
		vertexBuffer[vbIndex++] = v.pz;
		it++;
	}
	
	vertices.clear();

	glGenBuffers(1, &vboID); // Create the buffer ID, this is basically the same as generating texture ID's
	glBindBuffer(GL_ARRAY_BUFFER, vboID); // Bind the buffer (vertex array data)

	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertexBuffer, GL_STATIC_DRAW);

	// When we get here, all the vertex data is effectively on the card
	delete[] vertexBuffer;


}

void Mesh3D::createIndexBuffer() {

	int size = faces.size() * 3;
	numPrimitives = size;
	unsigned short* indexBuffer = new unsigned short[size];
	vector<Triangle>::iterator it = faces.begin();

	int ibIndex = 0;
	while (it != faces.end()) {
		Triangle f = (*it);

		indexBuffer[ibIndex++] = f.i[0];
		indexBuffer[ibIndex++] = f.i[1];
		indexBuffer[ibIndex++] = f.i[2];
		it++;
	}
	faces.clear();

	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned short), indexBuffer, GL_STATIC_DRAW);

	delete[] indexBuffer;
}

// <Florian>
void Mesh3D::draw() {
	// Initialisiert den Zufallszahlengenerator mit einem festen Seed
	srand(0); //Wichtig damit die Farben bei jedem Aufruf gleich sind

	int counter = 0;

	// Zeichne die Strips
	for (const auto& strip : strips) {
		if (counter == strip_amount_limit) {
			break;
		}

		// Generiere eine zuf�llige Farbe
		float r = static_cast<float>(rand()) / RAND_MAX;
		float g = static_cast<float>(rand()) / RAND_MAX;
		float b = static_cast<float>(rand()) / RAND_MAX;
		glColor3f(r, g, b);  // Setze die Farbe f�r alle Vertices im Strip

		glBegin(GL_TRIANGLE_STRIP);
		for (const auto& index : strip) {
			const Vertex& currentVertex = vertices[index];
			glVertex3f(currentVertex.px, currentVertex.py, currentVertex.pz);
		}
		glEnd();

		counter++;
	}
}

int Mesh3D::getStripsCount() {
	return strips.size();
}


// </Florian>