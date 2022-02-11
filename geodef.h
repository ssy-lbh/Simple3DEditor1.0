#ifndef __GEODEF__
#define __GEODEF__

#include "define.h"
#include "vecmath.h"
#include "list.h"

class Vertex;
class Edge;
class Face;

class Vertex : public Object {
public:
    Vector3 pos;
    Vector3 normal = Vector3::forward;
    Vector2 uv = Vector2::zero;
    Vector3 color = Vector3::one;
    List<Edge*> edges;
    List<Face*> faces;
    size_t index;

    Vertex();
    Vertex(Vector3 pos);
    ~Vertex();

    void UpdateNormal();
    void AddEdge(Edge* e);
    void AddFace(Face* f);
    bool FindEdge(Edge* e);
    bool FindFace(Face* f);
    // @return success
    bool DeleteEdge(Edge* e);
    bool DeleteFace(Face* f);
    Edge* EdgeRelateTo(Vertex* v);

    bool Hit(Vector3 ori, Vector3 dir);
    bool Hit(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2);
    bool HitUV(Vector2 uv, float err);
    bool HitUV(Vector2 uv1, Vector2 uv2);
};

class Edge : public Object {
public:
    Vertex* v1;
    Vertex* v2;
    List<Face*> faces;

    Edge();
    Edge(Vertex* v1, Vertex* v2);
    ~Edge();

    void AddFace(Face* f);
    bool FindFace(Face* f);
    bool DeleteFace(Face* f);
    void DeleteSelfReference();
    void DeleteSelfReferenceExcept(Vertex* v);
    Face* FaceRelateTo(Vertex* v);

    bool Hit(Vector3 ori, Vector3 dir);
};

class Face : public Object {
public:
    Vector3 normal;
    List<Vertex*> vertices;
    List<Edge*> edges;

    Face();
    ~Face();

    void AddVertex(Vertex* v);
    void AddEdge(Edge* e);
    bool FindVertex(Vertex* v);
    bool FindEdge(Edge* e);
    // @return success
    bool DeleteVertex(Vertex* v);
    bool DeleteEdge(Edge* e);
    void DeleteSelfReference();
    void DeleteSelfReferenceExcept(Vertex* v);
};

#endif
