#ifndef __DEFINE__
#define __DEFINE__

#ifndef NULL
#define NULL nullptr
#endif

#define ARCH_LITTLE_ENDIAN

using uint = unsigned int;
using byte = unsigned char;
using uchar = unsigned char;
using ushort = unsigned short;
using luint = long unsigned int;
using ulong = unsigned long;

using uenum = unsigned int;
// 暂时我也用不到64位整数
using uhash = unsigned long;

#define EXECUTABLE_FFMPEG ".\\bin\\ffmpeg\\ffmpeg"
#define EXECUTABLE_FFPLAY ".\\bin\\ffmpeg\\ffplay"
#define EXECUTABLE_FFPROBE ".\\bin\\ffmpeg\\ffprobe"
#define EXECUTABLE_GLSLC ".\\bin\\opengl\\glslc"
#define EXECUTABLE_DXC ".\\bin\\directx\\dxc"
#define EXECUTABLE_GLSLANG_VALIDATOR ".\\bin\\opengl\\glslangValidator"
#define EXECUTABLE_SPIRV_CROSS ".\\bin\\spirv\\spirv-cross"

// 对象基类
#include <base/Object.h>

namespace simple3deditor {

// basic types
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

// utils
class String;
class WString;
class PackString;
class PackWString;
class StringBuilder;
class DataBuffer;
class PackDataBuffer;

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
typedef TVector2<int> Vector2Int;
typedef TPoint2<float> Point2;
typedef TPoint2<int> Point2Int;
typedef TVector3<float> Vector3;
typedef TVector3<int> Vector3Int;
typedef TPoint3<float> Point3;
typedef TPoint3<int> Point3Int;
typedef TVector4<float> Vector4;
typedef TVector4<int> Vector4Int;
typedef TComplex<float> Complex;
typedef TComplex<int> ComplexInt;
typedef TQuaternion<float> Quaternion;
typedef TQuaternion<int> QuaternionInt;
typedef TMatrix4x4<float> Matrix4x4;
typedef TMatrix4x4<int> Matrix4x4Int;

// viewobject
enum class ViewObjectType;
class Transform;
struct RenderOptions;
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
struct WindowInfo;
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

// base
class IAppendableA;
class IAppendableW;
class ICloseable;
class IFlushable;
class IReadable;
class IWritable;

// io
class File;
class ISerializable;
class IDeserializable;
class IMemorable;
class IInputStream;
class IOutputStream;
class AReader;
class AWriter;

}

// soundtouch
namespace soundtouch {
    class SoundTouch;
};

// glfw
typedef struct GLFWwindow GLFWwindow;

// imgui
struct ImGuiContext;
struct ImDrawList;
struct ImDrawCmd;

#endif