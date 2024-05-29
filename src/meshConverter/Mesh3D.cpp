#include "Mesh3D.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <iomanip>
#include <cassert>

using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Mesh3D::Mesh3D(void)
{
	initialized = false;
}

Mesh3D::~Mesh3D(void)
{

}

void Mesh3D::readOBJ(const char* filename) {

	std::cout << "Loading OBJ-File " << filename << " ..." << "\n";

	ifstream stream;
	stream.open(filename);

	string line;

	while (getline(stream, line)) {
		vector<string> c = splitString(line, ' ');

		if (c.size() == 0) continue;

		if (c[0].compare("v") == 0) {
			processPosition(c);
		}
		else if (c[0].compare("vt") == 0) {
			continue;
		}
		else if (c[0].compare("vn") == 0) {
			continue;
		}
		else if (c[0].compare("f") == 0) {
			processFace(c);
		}
	}

	std::cout << "OBJ-File loaded" << "\n";
	std::cout << "Starting Stripification process..." << "\n";
	processStrips();
	std::cout << "Stripification finished" << "\n";

	cout << "Created " << vertices.size() << " unique vertices " << endl;
	cout << "Created " << faces.size() << " triangular faces" << endl;

	cout << "\n";
	cout << "Created " << strips.size() << " strips" << "\n";

	int total_count_strip_vertices = 0;

	for (const auto& strip : strips) {
		total_count_strip_vertices += strip.size();
	}

	cout << "Sending " << total_count_strip_vertices << " vertices to GPU instead of " << faces.size() * 3 << "\n";
	cout << "Saved " << (1.0F - (static_cast<float>(total_count_strip_vertices) / (faces.size() * 3))) * 100 << "% (" << (faces.size() * 3) - total_count_strip_vertices << " vertices)\n\n";

	positions.clear();
	texCoords.clear();
	normals.clear();

}

vector<string> Mesh3D::splitString(string& str, char delimiter)
{
	vector<string> words;
	string word;
	stringstream stream(str);

	while (getline(stream, word, delimiter))
		words.push_back(word);

	return words;
}

bool Mesh3D::isProcessedTriangle(int index)
{
	return processed_triagles_indices.count(index) != 0;
}

void Mesh3D::processPosition(vector<string>& c) {
	//cout << "Position "<< positions.size() << ": " << c[1] << "," << c[2] << "," << c[3] << "                        \r";	
	positions.push_back(Position(
		(float)atof(c[1].c_str()),
		(float)atof(c[2].c_str()),
		(float)atof(c[3].c_str()))
	);
}

void Mesh3D::processFace(vector<string>& c) {

	int numVertices = c.size() - 1;
	vector<unsigned short> indices;

	for (int i = 0; i < numVertices; ++i) {
		indices.push_back(indexOfVertex(c[i + 1]));
	}

	unsigned short index0 = indices[0];
	unsigned short index1 = indices[1];

	for (int i = 2; i < numVertices; ++i) {
		unsigned short index2 = indices[i];
		faces.push_back(Triangle(index0, index1, index2));
		index1 = index2;
	}

}

// Strips generieren
void Mesh3D::processStrips() {

	std::vector<int> triangle_indices;
	// Generiere eine Liste mit Indices für Liste faces 
	for (unsigned int i = 0; i < faces.size(); i++) {
		triangle_indices.push_back(i);
	}
	// Merke die Nachbardreiecke für jedes Dreieck
	initTriagleNeighbors();

	int active_triangle = -1;
	cout << "\n";
	// "Wenn es keine Dreiecke mehr zu verarbeiten gibt, beende den Algorithmus"
	while (processed_triagles_indices.size() < triangle_indices.size()) {
		std::cout << "%\r"
			<< processed_triagles_indices.size() << "\/"
			<< triangle_indices.size() << std::setw(20)
			<< "| " << std::setw(3) << (processed_triagles_indices.size() * 100) / triangle_indices.size();
		//  Sortiere triangle_indices nach wenigsten Nachbarn
		sortTriaglesByLeastUnprocessedNeighborCount(triangle_indices);


		// "Finde das Dreieck active_triangle mit den wenigsten Nachbarn 
		// (wenn mehrere existieren, wähle ein beliebiges)"
		auto it = std::find_if(triangle_indices.begin(), triangle_indices.end(), [&](int i) {
			return !isProcessedTriangle(i);
			});
		if (it != triangle_indices.end()) {
			active_triangle = *it;
		}

		// "Beginne einen neuen Strip"
		std::vector<unsigned short> strip;
		while (true) {
			//  "Füge das Dreieck zu dem Strip hinzu [und merke es als processed]"
			addTriagleToStrip(active_triangle, strip);

			// "Wenn es keine benachbarten Dreiecke mehr gibt, 
			// gehe zu [der äußeren Schleife]"
			if (countUnprocessedNeighborTriangles(active_triangle) == 0)
				break;

			// "Wähle ein benachbartes Dreieck mit den wenigsten Nachbarn"
			std::vector<int>& neighbors = triangle_neighbors_indices.at(active_triangle);
			sortTriaglesByLeastUnprocessedNeighborCount(neighbors);
			int unprocessed_triagle_index_least_neighbor = -1;

			unsigned int index = 0;
			for (; index < neighbors.size(); index++) {
				if (!isProcessedTriangle(neighbors[index])) {
					unprocessed_triagle_index_least_neighbor = neighbors[index];
					index++;
					break;
				}
			}
			for (; index < neighbors.size(); index++)
			{
				if (!isProcessedTriangle(neighbors[index])) {

					// "Wenn es mehrere Dreiecke gibt mit der gleichen Anzahl Nachbarn, 
					// schaue einen Schritt in die Zukunft."
					// "Wenn es wieder gleich viele sind, wähle ein beliebiges. 

					int index_triangle_min_2deg_neighbors_count = -1;
					int current_triangle_min_2deg_neighbors_count = -1;
					size_t temp_count;

					// Vergleich nach minimalen Wert eines Nachbarn zweiten Grades der Dreicke
					//auto index_triangle_2deg_neighbors = triangle_neighbors_indices.at(neighbors[index]);
					//if (!index_triangle_2deg_neighbors.empty()) {
					//	auto index_triangle_min_2deg_neighbors_count_iterator = std::min_element(index_triangle_2deg_neighbors.begin(), index_triangle_2deg_neighbors.end(),
					//		[&](int a, int b) {
					//			return compareByUnprocessedNeighborTrianglesCount(a, b);
					//		});
					//	index_triangle_min_2deg_neighbors_count = countUnprocessedNeighborTriangles(*index_triangle_min_2deg_neighbors_count_iterator);
					//}
					//
					//auto current_triangle_2deg_neighbors = triangle_neighbors_indices.at(unprocessed_triagle_index_least_neighbor);
					//if (!current_triangle_2deg_neighbors.empty()) {
					//	auto current_triangle_min_2deg_neighbors_count_iterator = std::min_element(current_triangle_2deg_neighbors.begin(), current_triangle_2deg_neighbors.end(),
					//		[&](int a, int b) {
					//			return compareByUnprocessedNeighborTrianglesCount(a, b);
					//		});
					//	current_triangle_min_2deg_neighbors_count = countUnprocessedNeighborTriangles(*current_triangle_min_2deg_neighbors_count_iterator);
					//
					//}
					//
					//if (index_triangle_min_2deg_neighbors_count < current_triangle_min_2deg_neighbors_count)
					//	unprocessed_triagle_index_least_neighbor = neighbors[index];


					// Vergleich nach Summer aller Wert der Nachbarn zweiten Grades der Dreicke
					if (countUnprocessedNeighborTriangles(neighbors[index])
						== countUnprocessedNeighborTriangles(unprocessed_triagle_index_least_neighbor)) {

						int index_vertex_min_2deg_neighbors_count = 0;
						int current_vertex_min_2deg_neighbors_count = 0;

						for (auto index : triangle_neighbors_indices.at(neighbors[index]))
							index_vertex_min_2deg_neighbors_count += countUnprocessedNeighborTriangles(index);

						for (auto index : triangle_neighbors_indices.at(unprocessed_triagle_index_least_neighbor))
							current_vertex_min_2deg_neighbors_count += countUnprocessedNeighborTriangles(index);

						if (index_vertex_min_2deg_neighbors_count < current_vertex_min_2deg_neighbors_count)
							unprocessed_triagle_index_least_neighbor = neighbors[index];
					}

				}


			}

			assert(unprocessed_triagle_index_least_neighbor != -1);
			active_triangle = unprocessed_triagle_index_least_neighbor;

		}
		strips.push_back(strip);
	}
	// Lade status in  der Konsole
	std::cout << "%\r"
		<< processed_triagles_indices.size() << "\/"
		<< triangle_indices.size() << std::setw(20)
		<< "| " << std::setw(3) << (processed_triagles_indices.size() * 100) / triangle_indices.size();
	std::cout << endl;
	strip_amount_limit = strips.size();
}

void processSecondDegreeNeighborsByMinimale() {

}


// Gibt die Anzahl der Nachbar-Dreiecke an, welche noch nicht behandelt wurden
size_t Mesh3D::countUnprocessedNeighborTriangles(int targetIndex) {
	int count = std::count_if(triangle_neighbors_indices.at(targetIndex).begin(), triangle_neighbors_indices.at(targetIndex).end(), [&](int i) {
		return !isProcessedTriangle(i);
		});
	return count;
}

// Gibt ein Vektor mit den Indezes der Nachbar-Dreiecke eines bestimmten Dreiecks zurück
std::vector<int> Mesh3D::fetchUnprocessedNeighborTriangles(int targetIndex) {
	std::vector<int> neighbor_triangles_of_target_triagle;
	for (unsigned short i = 0; i < faces.size(); ++i) {
		// überspringe den Vergleich eines Dreiecks mit sich selbst
		if (i == targetIndex) continue;

		const Triangle& currentTriangle = faces[i];

		// Überprüfe, ob 2 gemeinsame Indizes vorhanden sind
		int count_matched_indices = 0;
		for (unsigned short j : faces[targetIndex].i) {
			for (unsigned short k : currentTriangle.i) {
				if (k == j) {
					count_matched_indices++;
				}
				if (count_matched_indices == 2) {
					neighbor_triangles_of_target_triagle.push_back(i);
					count_matched_indices = 0;
					break;
				}
			}
		}
	}
	return neighbor_triangles_of_target_triagle;
}

// Vergleichsfunktion zum Sortieren des Vektors basierend auf countUnprocessedNeighborTriangles
bool Mesh3D::compareByUnprocessedNeighborTrianglesCount(int a, int b) {
	return countUnprocessedNeighborTriangles(a) < countUnprocessedNeighborTriangles(b);
}

// Funktion zum sortieren
void Mesh3D::sortTriaglesByLeastUnprocessedNeighborCount(std::vector<int>& triagles_indices) {
	std::sort(triagles_indices.begin(), triagles_indices.end(), [&](int a, int b) {
		return compareByUnprocessedNeighborTrianglesCount(a, b);
		});
}

// Markiere Dreieck als verarbeitet, passe neighbor_triangles_indices an.
void Mesh3D::markTriagleAsProcessed(int i) {
	processed_triagles_indices.insert(i);
}


void Mesh3D::initTriagleNeighbors() {

	// Konstruiere die Map
	for (unsigned int i = 0; i < faces.size(); i++)
		triangle_neighbors_indices[i] = std::vector<int>();

	// Initialisiere die Map
	for (unsigned int i = 0; i < faces.size(); i++) {
		std::vector<int> fetched_neighbors = fetchUnprocessedNeighborTriangles(i);
		for (int neighbor_index : fetched_neighbors) {
			triangle_neighbors_indices.at(i).push_back(neighbor_index);
		}
	}
}

void Mesh3D::addTriagleToStrip(const int targetIndex, std::vector<unsigned short>& strip) {

	const Triangle& currentTriangle = faces[targetIndex];
	int current_strip_size = strip.size();

	if (strip.empty()) {
		std::copy(std::begin(currentTriangle.i), std::end(currentTriangle.i), std::back_inserter(strip));
	}
	else {
		int count_matched_vertices = 0;
		for (int i : currentTriangle.i) {
			if (i == strip.at(current_strip_size - 1)) {
				count_matched_vertices++;
			}
			else if (i == strip.at(current_strip_size - 2)) {
				count_matched_vertices++;
			}
		}

		assert(count_matched_vertices <= 2 && count_matched_vertices > 0);

		if (count_matched_vertices == 1) {
			int temp = strip.at(current_strip_size - 1);
			strip.at(current_strip_size - 1) = strip.at(current_strip_size - 3);
			strip.push_back(temp);
			current_strip_size++;
		}

		std::copy_if(std::begin(currentTriangle.i), std::end(currentTriangle.i), std::back_inserter(strip), [&](int i) {
			return i != strip.at(current_strip_size - 2) && i != strip.at(current_strip_size - 1);
			});

	}
	markTriagleAsProcessed(targetIndex);
}

unsigned short Mesh3D::indexOfVertex(string vs) {

	map<string, unsigned short> ::iterator it = vertexHashMap.find(vs);

	unsigned int vertexIndex;

	// check if the vertex already exists
	if (it == vertexHashMap.end()) { // it's a new vertex

		vector<string> indices = splitString(vs, '/');
		int positionIndex = atoi(indices[0].c_str()) - 1;
		int texCoordIndex = atoi(indices[1].c_str()) - 1;
		int normalIndex = atoi(indices[2].c_str()) - 1;

		Vertex newVertex(positions[positionIndex]);

		vertexIndex = vertices.size();
		vertices.push_back(newVertex);
		vertexHashMap[vs] = vertexIndex;


	}
	else {
		vertexIndex = vertexHashMap[vs];
	}

	return vertexIndex;
}