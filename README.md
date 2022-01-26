# Simple3DEditor

## 介绍

一个有基础功能的3D编辑器，使用C++编写，图形基于OpenGL

## 软件架构

1. colorboard: 调色板窗口，用于选取颜色
2. font: 管理字体及其绘制
3. geodef: 几何元素结构体和函数定义
4. gltools: 一些关于OpenGL的封装函数
5. log: 日志管理
6. main: 主窗口部分
7. menu: 简易菜单
8. mesh: 网格体类，管理几何图形元素
9. nodemap: 节点编辑器窗口，计划作为流程蓝图、材质编辑器等
10. container: 窗口容器
11. shell: windows的一些特殊功能
12. vecmath: 数学基础库
13. uimgr: 简单的UI管理器

## 安装教程

1. 直接下载release或者下载源代码编译
2. 确保mingw工具已经安装并配置路径（全依赖代码分支为AVDevelop）
3. 下载文件夹路径下执行make.exe

## 使用说明

1. 文档暂且没时间写，快捷键可以看.rc文件
2. 可能出现一些bug

## 快捷键一览表

1. "q"：逆时针旋转（目前考虑弃用）
2. "e"：顺时针旋转（目前考虑弃用）
3. "p"：添加点
4. "f"：添加线、面（选择两个或三个点）
5. "g"：移动
6. "r"：旋转
7. "s"：缩放
8. "c"：选择颜色（先选择顶点）
9. "x"：X轴操作
10. "y"：Y轴操作
11. "z"：Z轴操作
12. "X"(shift + x)：YZ平面操作
13. "Y"(shift + y)：ZX平面操作
14. "Z"(shift + z)：XY平面操作
15. delete：删除顶点
16. ESC：退出
17. Ctrl+S：保存
18. Alt+/ Alt+?：软件信息
19. Alt+(0-9): 工具选择
20. Ctrl+L: 加载
21. "e": 引出新顶点（不稳定）

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
