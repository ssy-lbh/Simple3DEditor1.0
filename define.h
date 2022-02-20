#ifndef __DEFINE__
#define __DEFINE__

#ifndef NULL
#define NULL __null
#endif

#ifdef _WIN64
typedef unsigned long long size_t;
#else
typedef unsigned int size_t;
#endif

// 对象基类
class Object;

class Object {};

// utils.h
class String;
class WString;
class File;

// geodef.h
class Vertex;
class Edge;
class Face;

// mesh.h
class Mesh;

// vecmath.h
class Vector2;
class Vector3;
class Vector4;
class Quaternion;
class Matrix4x4;

// viewobject.h
class Transform;
class AViewObject;
class MeshObject;
class BezierCurveObject;

// main.h
class IUndo;
class MainWindow;
class MainData;
class Main;

// container.h
class LRContainer;
class UDContainer;
class SelectionWindow;

// colorboard.h
class ColorBoard;

// audio.h
class AudioUtils;
class AudioPlayerWindow;
class AudioCaptureWindow;

// attrtable.h
enum class AttributeType;
class Attribute;
class AttributeTable;

// anim.h
class IAnimationFunction;
class AnimationWindow;

// gltools.h
class GLUtils;
class GLTexture2D;
class GLComputeProgram;
class GLFrameBuffer;
class GLLights;

// menu.h
class IMenuItem;
class Menu;
class MenuItem;

// nodemap.h
class NodeMapWindow;

// paint.h
class UVEditWindow;
class PaintWindow;

// timetools.h
class TimeUtils;
class TimerManager;

// tree.h
class TreeWindow;

// uimgr.h
class UIManager;
class ViewportManager;
class IButton;
class IconButton;
class UIEditA;
class IOperation;
class ITool;
class IWindow;

#endif