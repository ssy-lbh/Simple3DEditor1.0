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

#ifndef interface
#define interface class
#endif

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef unsigned int uenum;

// utils
class String;
class WString;
class File;
class DataBuffer;

// 对象基类
class Object;

class Object {};

// geodef
class Vertex;
class Edge;
class Face;

// mesh
class Mesh;

// vecmath
class Vector2;
class Vector3;
class Vector4;
class Quaternion;
class Matrix4x4;

// viewobject
class Transform;
class AViewObject;
class MeshObject;
class BezierCurveObject;
class AudioListenerObject;

// main
class IUndo;
class MainWindow;
class LocalData;
class GlobalData;
class Main;

// container
class LRContainer;
class UDContainer;
class SelectionWindow;

// appframe
class AppFrame;

// colorboard
class ColorBoard;

// audio
class AudioUtils;
class AudioPlayerWindow;
class AudioCaptureWindow;

// attrtable
enum class AttributeType;
class Attribute;
class AttributeTable;

// anim
class IAnimationFunction;
class AnimationWindow;

// gltools
class GLRect;
class GLUtils;
class GLTexture2D;
class GLComputeProgram;
class GLFrameBuffer;
class GLLights;
class GLProgram;
class GLShader;
class GLVertexArray;
class GLVertexBuffer;
class GLIndexBuffer;

// menu
class IMenuItem;
class Menu;
class MenuItem;

// nodemap
class NodeMapWindow;

// paint
class UVEditWindow;
class PaintWindow;

// timetools
class TimeUtils;
class TimerManager;

// tree
class TreeWindow;

// uimgr
class UIManager;
class IButton;
class IconButton;
class UIEditA;
class UIEditW;
class IOperation;
class ITool;
class IWindow;

// appframe
class AppFrame;

// viewmgr
class ViewManager;

#endif