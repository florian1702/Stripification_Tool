#pragma once

struct Position {
	Position(float x, float y, float z) { px = x; py = y; pz = z; }
	float px;
	float py;
	float pz;
};

struct Normal {
	Normal(float x, float y, float z) { nx = x; ny = y; nz = z; }
	float nx;
	float ny;
	float nz;
};

struct TexCoord {
	TexCoord(float x, float y) { u = x; v = y; }
	float u;
	float v;
};

struct Vertex {
	Vertex(Position p) {
		//Vertex hat keine Normalen und Texturkoordinaten mehr
		px = p.px;	py = p.py;	pz = p.pz;
	}
	float px;
	float py;
	float pz;
};

struct Triangle {
	Triangle(unsigned short a, unsigned short b, unsigned short c) {
		i[0] = a;
		i[1] = b;
		i[2] = c;
	}
	unsigned short i[3];
};

