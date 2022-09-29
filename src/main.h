#ifndef __MAIN__
#define __MAIN__

#include <define.h>

#include <util/HashMap.h>
#include <util/RBTree.h>
#include <util/List.h>
#include <util/os/AppFrame.h>
#include <util/math3d/LinearAlgebra.h>

#include <json/nlohmann/json_fwd.hpp>

namespace simple3deditor {

//TODO ��ȫ��������
class IUndo : public Object {
public:
    virtual void Execute() = 0;
};

struct RenderOptions {
public:
    bool vertex = true;
    bool edge = true;
    bool face = true;
    bool light = false;
    bool editor = false;
    ObjectOperation objOp;
};

//TODO �ɼ��봰������ק����
class LocalData final : public Object {
public:
    Vector2 cursorPos;
    Vector2 cliSize;
    float aspect = 1.0f;

    Menu* menu = NULL;
    Vector2 menuPos;

    RenderOptions renderOptions;

    // �����ֶ����ڵ���������Ψһ
    AudioListenerObject* audioListener = NULL;
    CameraObject* camera = NULL;

    // �ⲿ����Ӧ�޸�
    float recTime;
    float deltaTime;

    LocalData();
    ~LocalData();

    static LocalData* GetLocalInst();

    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

    void SetMenu(Menu* m);

    void OnLeftDown(int x, int y);
    void OnLeftUp(int x, int y);
    void OnRightDown(int x, int y);
    void OnRightUp(int x, int y);
    void OnMenuAccel(int id, bool accel);

    void Render();

    void CreateAudioListener();
    void CreateCamera();
    void DestoryAudioListener();
    void DestoryCamera();
};

enum class SelectionType {
    SELECT_OBJECT,
    SELECT_VERTICES,
    SELECT_EDGES,
    SELECT_FACES
};

struct WindowInfo {
    std::function<AWindow*()> factory;
    String id;
    WString displayName;
};

struct ObjectInfo {
    std::function<AViewObject*()> factory;
    String id;
    WString displayName;
};

//TODO �ɼ���ȫ����ק���ܣ��ڴ���֮�䴫������
class GlobalData final : public Object {
public:
    // �ܳ�������
    AViewObject* scene = NULL;
    AViewObject* screen = NULL;

    // ��ǰѡ�еĶ���
    AViewObject* curObject = NULL;

    // ѡ�񲿷�
    //TODO �Ժ�ѡ�񲿷���Ϣ�����̱߳���
    SelectionType selType = SelectionType::SELECT_OBJECT;

    List<AViewObject*> selObjects;
    List<Vertex*> selPoints;
    List<Edge*> selEdges;
    List<Face*> selFaces;

    float animFrame = 0.0f;

    GlobalData();
    ~GlobalData();

    void SelectObject(AViewObject* o);
    void SelectType(SelectionType type);

    void OnTimer(int id);

    void OnAnimationFrame(float frame);

    List<WindowInfo*> windowReg;
    List<ObjectInfo*> objectReg;

    // id���ڱ���ļ��������е�������Ϣ������ÿһ�����ӦΨһ
    // ע��Ĵ�����������Ҫ��
    // 1. �̳���editor/main/Window.h�е�AWindow��
    // 2. �ڲ�����static constexpr const char* WINDOW_ID��Ϊȫ�ִ���Ψһ��ʶ����ע��Ψһ
    // 3. �ڲ�����static constexpr const wchar_t* WINDOW_DISPLAY_NAME��Ϊ������ʾ���ƣ����ݲ���Ҫ��
    // 4. ������ڲ��ܸ��û�����ѡ���ʹ��(��������)���ɽ�WINDOW_DISPLAY_NAME��ΪL""
    template <typename T>
    void RegisterWindow(){
        WindowInfo* info = new WindowInfo;
        info->factory = []{ return new T(); };
        info->id = T::WINDOW_ID;
        info->displayName = T::WINDOW_DISPLAY_NAME;
        windowReg.Add(info);
    }

    template <typename T>
    void RegisterObject(){
        ObjectInfo* info = new ObjectInfo;
        info->factory = []{ return new T(); };
        info->id = T::OBJECT_ID;
        info->displayName = T::OBJECT_DISPLAY_NAME;
        objectReg.Add(info);
    }

    void RegisterWindow(std::function<AWindow*()> factory, const char* id, const wchar_t* displayName);

    AWindow* ConstructWindow(const char* id);
    AWindow* ConstructWindow(nlohmann::json& o);

    AViewObject* ConstructObject(const char* id);
    AViewObject* ConstructObject(nlohmann::json& o);

    void LoadSettings(const char* path);
    void ApplySettings();
    void SaveSettings(const char* path);
};

class Main final : public Object {
public:
    static uint INIT_WINDOW_WIDTH;
    static uint INIT_WINDOW_HEIGHT;

    static GlobalData* data;

    static void SetCursor(int id);
    static void SetCursor(const char* res);
    static void SetMenu(Menu* m);
    static void SelectObject(AViewObject* o);
    static void SelectType(SelectionType type);
    static void AddObject(AViewObject* o);
    static void AddObjectToScreen(AViewObject* o);
    static bool AddObjectChild(AViewObject* parent, AViewObject* child);
    static void DeleteObject(AViewObject* o);
    static void OnAnimationFrame(float frame);
    static void SaveImage(String file, Rect rect);
    static void RenderAnimation(String dir, size_t start, size_t end, Rect rect);

    static void OnMouseMove(Point3 ori, Vector3 dir);
    static void OnLeftDown(Point3 ori, Vector3 dir);
    static void OnLeftUp(Point3 ori, Vector3 dir);
    static void OnRightDown(Point3 ori, Vector3 dir);
    static void OnRightUp(Point3 ori, Vector3 dir);
    static void OnMouseWheel(int delta);

    static void RenderScene();
    static void RenderScreen();

    static Mesh* GetMesh();
    static Mesh* GetMesh(AViewObject* o);

    static int MainEntry(int argc, char** argv);
    static void Init();
    static void Uninit();
};

}

#endif