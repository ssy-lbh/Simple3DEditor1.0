#ifndef __MESH__
#define __MESH__

#include <windef.h>

#include "geodef.h"

#include "gltools.h"

class Mesh;

class Mesh {
private:
    List<Vertex*> vertices;
    List<Edge*> edges;
    List<Face*> faces;

    GLTexture2D* modeltex = NULL;
    
public:
    Mesh();
    ~Mesh();
    Vertex* Find(Vector3 ori, Vector3 dir);
    size_t FindScreenRect(Vector3 camPos, Quaternion camDir, float zNear, float zFar, float x1, float x2, float y1, float y2, List<Vertex*>& result);
    Vertex* AddVertex(Vector3 pos);
    Vertex* AddVertex(Vertex* v);
    Edge* AddEdge(Vertex* v1, Vertex* v2);
    Face* AddTriFace(Vertex* v1, Vertex* v2, Vertex* v3);
    void DeleteVertex(Vertex* v);
    void DeleteEdge(Edge* e);
    void DeleteTriFace(Face* f);
    void Render();
    void WriteToOBJ(HANDLE hFile);
    // ID为-1时删除
    void SetTexture(int resid);
};

#endif