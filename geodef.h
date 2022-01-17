#ifndef __GEODEF__
#define __GEODEF__

#include <stddef.h>

#include "vecmath.h"
#include "list.h"

class Vertex;
class Edge;
class Face;

class Vertex {
public:
    Vector3 pos;
    Vector3 normal = Vector3::forward;
    Vector3 uv = Vector3::zero;
    Vector3 color = Vector3::one;
    List<Edge*> edges;
    List<Face*> faces;
    size_t index;

    void UpdateNormal();
    void AddEdge(Edge* e);
    void AddFace(Face* f);
};

class Edge {
public:
    Vertex* v1;
    Vertex* v2;
    Face* f1;
    Face* f2;

    void AddFace(Face* f);
};

class Face {
public:
    Vector3 normal;
    List<Vertex*> vertices;
    List<Edge*> edges;
};

#endif
