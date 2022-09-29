#ifndef __UTIL_MATH3D_MESH__
#define __UTIL_MATH3D_MESH__

#include <define.h>

#include <util/List.h>

namespace simple3deditor {

class Mesh final : public Object {
private:
    List<Vertex*> vertices;
    List<Edge*> edges;
    List<Face*> faces;

    GLTexture2D* modeltex = NULL;

    AViewObject* object = NULL;
    
public:
    Mesh();
    Mesh(AViewObject* object);
    ~Mesh();

    Vertex* Find(Vector3 ori, Vector3 dir);
    size_t FindScreenRect(const SelectInfo* info, Matrix4x4 mat, List<Vertex*>& result);
    Edge* FindEdge(Vector3 ori, Vector3 dir);
    Vertex* FindUV(Vector2 uv, float err = 0.01f);
    size_t FindUVRect(Vector2 uv1, Vector2 uv2, List<Vertex*>& result);
    Face* Intersect(Point3 ori, Vector3 dir, Vector3* bary);
    // �ཻ���ʱ��uv����ΪNULL�ҲŻᱻд��
    bool IntersectUV(Point3 ori, Vector3 dir, Vector2* uv);

    Vertex* AddVertex(Vector3 pos);
    Vertex* AddVertex(Vertex* v);
    Edge* AddEdge(Vertex* v1, Vertex* v2);
    Face* AddTriFace(Vertex* v1, Vertex* v2, Vertex* v3);
    void DeleteVertex(Vertex* v);
    void DeleteEdge(Edge* e);
    void DeleteTriFace(Face* f);

    void OnSelectVertex(Point3 ori, Vector3 dir, List<Vertex*>& list);
    void OnSelectEdge(Point3 ori, Vector3 dir, List<Edge*>& list);
    void OnSelectUV(Vector2 uv, List<Vertex*>& list, float err = 0.01f);

    void Render();
    void Render(const RenderOptions* options);
    void RenderWithUV();
    void RenderWithUV(const RenderOptions* options);
    void RenderUVMap();

    void WriteToOBJ(File file, bool uv = false, bool normal = false);

    // IDΪ-1ʱɾ��
    void SetTexture(int resid);
    void SetTexture(GLTexture2D* texture);
    void ResetTexture();
    bool EnableTexture();
    bool DisableTexture();
};

}

#endif