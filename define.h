#ifndef __DEFINE__
#define __DEFINE__

#ifndef NULL
#define NULL __null
#endif

#ifndef _WIN64
typedef unsigned int size_t;
#else
typedef unsigned long long size_t;
#endif

#ifndef interface
#define interface class
#endif

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef unsigned int uenum;
// 暂时我也用不到64位整数
typedef unsigned long uhash;

#ifdef PLATFORM_WINDOWS
typedef void* handle;
#endif

// utils
class String;
class WString;
class StringBuilder;
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
class Point2;
class Vector3;
class Point3;
class Vector4;
class Complex;
class Quaternion;
class Matrix4x4;
class Matrix3x4;
class Matrix2x3;
class Matrix2x4;

// viewobject
class Transform;
class RenderOptions;
class SelectInfo;
enum class ViewObjectType;
class AViewObject;
class MeshObject;
class BezierCurveObject;
class AudioSourceObject;
class AudioListenerObject;
class CameraObject;

// main
class IUndo;
enum class ObjectOperation;
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
class AnimationCurve;
class AnimationWindow;

// gltools
class GLRect;
class GLUtils;
class GLTexture2D;
class GLRenderTexture2D;
class GLComputeProgram;
class GLFrameBuffer;
class GLLights;
class GLProgram;
class GLShader;
class GLVertexArray;
class GLVertexBuffer;
class GLIndexBuffer;
class GLSkyBox;

// menu
class IMenuItem;
class Menu;
class MenuItem;

// nodemap
class NodeMapWindow;

// paint
class UVEditWindow;
class PaintWindow;

// render
class RenderWindow;

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

// property
class Property;
class PropertyVector3;
class PropertyQuaternion;

// soundtouch
namespace soundtouch {
    class SoundTouch;
};

#endif