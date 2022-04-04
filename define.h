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
#define EXECUTABLE_FFMPEG ".\\bin\\ffmpeg.exe"
#define EXECUTABLE_FFPLAY ".\\bin\\ffplay.exe"
#define EXECUTABLE_FFPROBE ".\\bin\\ffprobe.exe"
#define EXECUTABLE_GLSLC ".\\bin\\glslc.exe"
#define EXECUTABLE_DXC ".\\bin\\dxc.exe"
#define EXECUTABLE_GLSLANG_VALIDATOR ".\\bin\\glslangValidator.exe"
#define EXECUTABLE_SPIRV_CROSS ".\\bin\\spirv-cross.exe"
#endif

// utils
class String;
class WString;
class PackString;
class PackWString;
class StringBuilder;
class File;
class ISerializable;
class IDeserializable;
class IMemorable;
class DataBuffer;
class PackDataBuffer;

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
class Rect;
class CCamera;
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
enum class ViewObjectType;
class Transform;
class RenderOptions;
class SelectInfo;
class AViewObject;
class MeshObject;
class BezierCurveObject;
class AudioSourceObject;
class AudioListenerObject;
class CameraObject;
class GUIManagerObject;
class AGUIObject;
class GUIMeshObject;

// main
enum class SelectionType;
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

// font
class Font;
class GLFont;

// shell
enum class ShaderType;

// colorboard
class ColorBoard;

// audio
class AudioUtils;
class AudioPlayerWindow;
class AudioCaptureWindow;

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
class Menu;
class MenuItem;
enum class MenuItemType;

// nodemap
class NodeMapWindow;

// paint
class UVEditWindow;
class PaintWindow;

// render
class RenderWindow;

// timetools
class Time;
class TimerManager;

// tree
class TreeWindow;

// uimgr
class UIManager;
class GUIManager;
class AGUI;
class IButton;
class IconButton;
class UIEditA;
class UIEditW;
class IOperation;
class ITool;
class IWindow;
class AWindow;

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

// manager
class WindowManager;
class WindowRegistry;
class PluginManager;
class Plugin;

// physics
class Rigidbody;
enum class ColliderType;
class ACollider;
class CubeCollider;
class SphereCollider;
class MeshCollider;

#endif