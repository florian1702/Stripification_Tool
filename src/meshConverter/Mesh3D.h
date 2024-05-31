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

	int strip_amount_limit = 3;
	int getStripsCount();

	vector<vector<unsigned short>> getStrips() {
		return strips;
	}

	vector<Triangle> getFaces() {
		return faces;
	}

	vector<Vertex> getVertices() {
		return vertices;
	}

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

	bool inited = false;
	vector<vector<unsigned short>> strips;
	unordered_set<int> processed_triagles_indices;
	map<int, vector<int>> triangle_neighbors_indices;
	map<string, unsigned short> vertexHashMap;
	void processStrips();
	size_t countUnprocessedNeighborTriangles(int targetIndex);
	vector<int> fetchUnprocessedNeighborTriangles(int targetIndex);
	bool compareByUnprocessedNeighborTrianglesCount(int a, int b);
	void sortTriaglesByLeastUnprocessedNeighborCount(vector<int>& targetIndex);
	void markTriagleAsProcessed(int targetIndex);
	void initTriagleNeighbors();
	void addTriagleToStrip(const int targetIndex, vector<unsigned short>& strip);
	bool isProcessedTriangle(int index);
};

