#ifndef __UTILS_MATH3D_GEOMETRY__
#define __UTILS_MATH3D_GEOMETRY__

#include <define.h>

#include <utils/List.h>
#include <utils/math3d/LinearAlgebra.h>

class Vertex final : public Object {
public:
    Point3 pos;
    Vector3 normal = Vector3::forward;
    Vector2 uv = Vector2::zero;
    Vector3 color = Vector3::one;
    List<Edge*> edges;
    List<Face*> faces;
    AViewObject* object = NULL;

    // 保留的数据，可局部功能使用
    size_t index;

    Vertex();
    Vertex(Point3 pos);
    ~Vertex();

    Point3 GetWorldPos();
    void SetWorldPos(Point3 pos);
    void UpdateNormal();
    void AddEdge(Edge* e);
    void AddFace(Face* f);
    bool FindEdge(Edge* e);
    bool FindFace(Face* f);
    // @return success
    bool DeleteEdge(Edge* e);
    bool DeleteFace(Face* f);
    Edge* EdgeRelateTo(Vertex* v);

    static bool Hit(Vector3 pos, Vector3 camPos, Quaternion camDir, Vector2 zBound, GLRect rect);

    bool Hit(Vector3 ori, Vector3 dir);
    bool HitUV(Vector2 uv, float err);
    bool HitUV(Vector2 uv1, Vector2 uv2);
};

class Edge final : public Object {
public:
    Vertex* v1;
    Vertex* v2;
    List<Face*> faces;
    AViewObject* object = NULL;

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

class Face final : public Object {
public:
    Vector3 normal;
    List<Vertex*> vertices;
    List<Edge*> edges;
    AViewObject* object = NULL;

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
    void DeleteSelfReferenceExcept(Edge* e);
};

#endif