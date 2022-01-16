#ifndef __MESH__
#define __MESH__

#include<stddef.h>

#include "geodef.h"

class Mesh;

class Mesh {
private:
    List<Vertex*> vertices;
    List<Edge*> edges;
    List<Face*> faces;
public:
    Mesh();
    ~Mesh();
    Vertex* Find(Vector3 ori, Vector3 dir);
    Vertex* AddVertex(Vector3 pos);
    Edge* AddEdge(Vertex* v1, Vertex* v2);
    Face* AddTriFace(Vertex* v1, Vertex* v2, Vertex* v3);
    void DeleteVertex(Vertex* v);
    void DeleteEdge(Edge* e);
    void DeleteTriFace(Face* f);
    void Render();
};

#endif