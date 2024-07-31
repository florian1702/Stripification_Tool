#pragma once
#include "VertexStructs.h"
#include <vector>
#include <map>
#include <string>
#include <unordered_set>

using namespace std;

class Mesh3D
{
public:
	Mesh3D(void);
	virtual ~Mesh3D(void);

	void readOBJ(const char* filename);
	void draw();
	
	// <Florian>
	// Gibt die maximale Anzahl von Stips zur�ck, die angezeigt werden
	int strip_amount_limit;

	// Gibt die Anzahl der generierten Stips zur�ck
	int getStripsCount();

	// Gibt die Liste der Stips zur�ck
	vector<vector<unsigned short>> getStrips() {
		return strips;
	}

	// Gibt die Liste der Dreiecke (Faces) zur�ck
	vector<Triangle> getFaces() {
		return faces;
	}

	// Gibt die Liste der Vertices zur�ck
	vector<Vertex> getVertices() {
		return vertices;
	}
	// </Florian>

protected:
	vector<Position> positions;
	vector<Vertex> vertices;
	vector<Triangle> faces;

	void processPosition(vector<string>& c);
	void processFace(vector<string>& c);

	vector<string> splitString(string& str, char delimiter);
	unsigned short indexOfVertex(string vs);

	void createVertexBuffer();
	void createIndexBuffer();
	bool initialized;
	unsigned int vboID;
	unsigned int iboID;
	int numPrimitives;

	// <Florian>
	// Vektor zur Speicherung der Stips
	vector<vector<unsigned short>> strips;

	// Set zur Verfolgung bereits verarbeiteter Dreiecke
	unordered_set<int> processedTriangles;

	// Map zur Speicherung der Nachbar-Dreiecke f�r jedes Dreieck
	map<int, vector<int>> triangleNeighbors;

	// Map zum Hashing von Vertex-Strings auf ihre Indizes
	map<string, unsigned short> vertexHashMap;

	// Generiert die Stips f�r das Mesh
	void generateStrips();

	// Gibt die Anzahl der noch nicht verarbeiteten Nachbar-Dreiecke eines bestimmten Dreiecks zur�ck
	size_t countUnprocessedNeighbors(int targetIndex);

	// Gibt eine Liste der noch nicht verarbeiteten Nachbar-Dreiecke eines bestimmten Dreiecks zur�ck
	vector<int> getUnprocessedNeighbors(int targetIndex);

	// Vergleichsfunktion zum Sortieren der Dreiecke nach der Anzahl der noch nicht verarbeiteten Nachbarn
	bool compareNeighborCount(int a, int b);

	// Sortiert die Indizes der Dreiecke nach der Anzahl der noch nicht verarbeiteten Nachbarn
	void sortTrianglesByNeighborCount(vector<int>& targetIndex);

	// Markiert ein Dreieck als verarbeitet
	void markTriagleAsProcessed(int targetIndex);

	// Initialisiert die Nachbar-Dreiecke f�r jedes Dreieck im Mesh
	void initTriagleNeighbors();

	// F�gt ein Dreieck zu einem Strip hinzu
	void addTriagleToStrip(const int targetIndex, vector<unsigned short>& strip);

	// �berpr�ft, ob ein Dreieck bereits verarbeitet wurde
	bool isTriangleProcessed(int index);
	// </Florian>
};

