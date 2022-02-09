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
    Vertex* FindUV(Vector2 uv, float err = 0.01f);
    size_t FindUVRect(Vector2 uv1, Vector2 uv2, List<Vertex*>& result);

    Vertex* AddVertex(Vector3 pos);
    Vertex* AddVertex(Vertex* v);
    Edge* AddEdge(Vertex* v1, Vertex* v2);
    Face* AddTriFace(Vertex* v1, Vertex* v2, Vertex* v3);
    void DeleteVertex(Vertex* v);
    void DeleteEdge(Edge* e);
    void DeleteTriFace(Face* f);

    void Render();
    void RenderUVMap();

    void WriteToOBJ(HANDLE hFile, bool uv = false, bool normal = false);

    // ID为-1时删除
    void SetTexture(int resid);
    void SetTexture(GLTexture2D* texture);
    void ResetTexture();
    bool EnableTexture();
    bool DisableTexture();
};

#endif