#ifndef __DEFINE__
#define __DEFINE__

#ifndef NULL
#define NULL __null
#endif

#if defined(WIN32) ||defined(_WIN32) || defined(_WIN64) ||defined(__WIN32__) || defined(__WINDOWS__)
    #define WINDOWS_OS
#elif defined(__APPLE__)     // Apple platform
    #define APPLE_OS
#elif defined(__linux__) || defined(linux) || defined(__linux)    // Linux platform
    #define LINUX_OS
#endif

#ifndef _WIN64
typedef unsigned int size_t;
#else
typedef unsigned long long size_t;
#endif

#ifndef interface
#define interface class
#endif

using uint = unsigned int;
using uchar = unsigned char;
using ushort = unsigned short;
using luint = long unsigned int;
using ulong = unsigned long;

using uenum = unsigned int;
// 暂时我也用不到64位整数
using uhash = unsigned long;

#ifdef PLATFORM_WINDOWS
typedef void* handle;
#define EXECUTABLE_FFMPEG ".\\bin\\ffmpeg\\ffmpeg.exe"
#define EXECUTABLE_FFPLAY ".\\bin\\ffmpeg\\ffplay.exe"
#define EXECUTABLE_FFPROBE ".\\bin\\ffmpeg\\ffprobe.exe"
#define EXECUTABLE_GLSLC ".\\bin\\opengl\\glslc.exe"
#define EXECUTABLE_DXC ".\\bin\\directx\\dxc.exe"
#define EXECUTABLE_GLSLANG_VALIDATOR ".\\bin\\opengl\\glslangValidator.exe"
#define EXECUTABLE_SPIRV_CROSS ".\\bin\\spirv\\spirv-cross.exe"
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
class CCamera;
template <typename T>
class TRect;
template <typename T>
class TVector2;
template <typename T>
class TPoint2;
template <typename T>
class TVector3;
template <typename T>
class TPoint3;
template <typename T>
class TVector4;
template <typename T>
class TComplex;
template <typename T>
class TQuaternion;
template <typename T>
class TMatrix4x4;
typedef TRect<float> Rect;
typedef TVector2<float> Vector2;
typedef TPoint2<float> Point2;
typedef TVector3<float> Vector3;
typedef TPoint3<float> Point3;
typedef TVector4<float> Vector4;
typedef TComplex<float> Complex;
typedef TQuaternion<float> Quaternion;
typedef TMatrix4x4<float> Matrix4x4;

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
class PointLightObject;
class AWindowObject;

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