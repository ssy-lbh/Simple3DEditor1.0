# Simple3DEditor

## 介绍

一个有基础功能的3D编辑器，使用C++编写，图形基于OpenGL
但可能经过这一段时间以来的修订，功能不再只有基础

此仓库分支为多媒体加强版(虽然编译成主程序也只是差不多1MB大小)

### 加入的库

1. OpenGL
2. OpenAL
3. stb_image
4. SoundTouch
5. FFmpeg

### 软件功能

1. 基础建模
2. 基础绘图
3. UV坐标编辑以及贴图
4. 音频解码
5. 变声器
6. 立体音频
7. 渲染输出打印
8. 动画制作

### 待加功能

1. 材质
2. 音频编码输出
3. 视频编解码
4. 着色器编辑器
5. 渲染3D音频、动画、视频并编码输出
6. 代码控制节点编辑器
7. 着色器节点图编辑器
8. 3D画刷
9. 水彩仿真画刷
10. 光线追踪渲染
11. 插件系统，期望能注册窗口、菜单、对象、动画函数、修改器，操作对象，文件输入输出工具，甚至游戏编译器等(没抱多大希望，毕竟没几个人会给我写代码)
12. 内置小型MC游戏窗口(有点开玩笑的意思)

同时为了编译使用方便，将所有依赖(C++基础库及其操作系统平台库除外)都加入了此仓库，
确保编译时找依赖的烦恼更少一些

此项目目前就是我对从高中开始OI快三年经历的留念，感觉不做个软件出来对不起这三年时光，很可能一直维护到大学及以后

## 软件架构

### 编辑器

主要窗口:

1. MainWindow 主窗口
2. PaintWindow 绘画窗口
3. UVEditWindow UV坐标编辑器
4. TreeWindow 树状对象管理窗口
5. AudioPlayerWindow 音频播放器
6. AudioCaptureWindow 录音以及变声器
7. AnimationWindow 动画控制窗口
8. RenderWindow 渲染窗口
9. NodeMapWindow 节点编辑器(尚未完成)

GUI容器、管理器、组件等:

1. AnimationCurve 动画曲线
2. Container 容器
3. Menu 菜单
4. UIManager UI管理器
5. ViewManager 视口管理器

外部对话框:

1. ColorBoard 颜色选择框
2. Tips 提示信息

### 工具库

#### 注:部分功能平台相关，源代码位于platform目录下

基本:

1. String 字符串(平台相关)
2. List 可变数组
3. File 文件(平台相关)
4. Queue 队列
5. StringBuilder 字符串构建类
6. AudioUtils 音频工具
7. DataBuffer 数据缓冲

图形库(如果有能力可以做其他图形API实现，甚至可以软光栅):

1. GLUtils 图形工具
2. GLSimplified 简化OpenGL函数
3. GLTexture2D 2D纹理
4. GLLights 光照管理
5. GLShader 着色器
6. GLProgram 着色程序
7. GLRenderTexture2D 2D可渲染纹理
8. GLVertexArray 顶点数组
9. GLVertexBuffer 顶点缓冲
10. GLIndexBuffer 索引缓冲
11. GLComputeProgram GPU计算程序

数学库:

1. Math 基本数学函数
2. LinearAlgebra 线性代数
3. Geometry 几何图形
4. Mesh 网格体
5. Property 属性
6. ViewObject 可见对象

操作系统库(期望跨平台的实现):

1. AppFrame 应用主窗口
2. Font 字体管理
3. GLFunc OpenGL扩展函数
4. Log 日志输出
5. Resource 资源管理
6. Shell 命令调用
7. Thread 线程管理
8. Time 时间

## 软件截图

![capture1](screenshots/capture1.png)
![capture2](screenshots/capture2.png)
![capture3](screenshots/capture3.png)
![capture4](screenshots/capture4.png)

## 安装教程

1. 直接下载release或者下载源代码编译
2. 确保mingw工具已经安装并配置路径（全依赖代码分支为AVDevelop）
3. 下载文件夹路径下执行make.exe

## 使用说明

1. 文档暂且没时间写，更多快捷键可以看.rc文件
2. 可能出现一些bug
3. 截止目前实际上贡献者就我一个人，若求更新也请体谅作者的肝

## 快捷键一览表

### 特别注意:Ctrl+W打开窗口控制菜单

1. "p"：添加点
2. "f"：添加线、面（选择两个或三个点）
3. "g"：移动
4. "r"：旋转
5. "s"：缩放
6. "c"：选择颜色（先选择顶点）
7. "x"：X轴操作
8. "y"：Y轴操作
9. "z"：Z轴操作
10. "X"(shift + x)：YZ平面操作
11. "Y"(shift + y)：ZX平面操作
12. "Z"(shift + z)：XY平面操作
13. delete：删除顶点
14. ESC：退出
15. Ctrl+S：保存
16. Alt+/ Alt+?：软件信息
17. Alt+(0-9): 工具选择
18. Ctrl+L: 加载
19. "e": 引出新顶点
20. Shift+A: 基本菜单
21. Ctrl+P: 打印

## 参与贡献

1. Fork 本仓库
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request

## 特技

1. 使用 Readme\_XXX.md 来支持不同的语言，例如 Readme\_en.md, Readme\_zh.md
2. Gitee 官方博客 [blog.gitee.com](https://blog.gitee.com)
3. 你可以 [https://gitee.com/explore](https://gitee.com/explore) 这个地址来了解 Gitee 上的优秀开源项目
4. [GVP](https://gitee.com/gvp) 全称是 Gitee 最有价值开源项目，是综合评定出的优秀开源项目
5. Gitee 官方提供的使用手册 [https://gitee.com/help](https://gitee.com/help)
6. Gitee 封面人物是一档用来展示 Gitee 会员风采的栏目 [https://gitee.com/gitee-stars/](https://gitee.com/gitee-stars/)
