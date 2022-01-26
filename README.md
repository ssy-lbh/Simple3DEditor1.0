# Forest Engine

## Introduction

An simple&elegant Open-GL based open-code 3D game template written in C++;

## Architecture

1. colorboard: windowed colorboard for selecting a color
2. font: font manager
3. geodef: function definer for basic geo-elements.
4. gltools: The CORE
5. log: log moudle
6. main: main window
7. menu: a simple menu
8. mesh: mesh utils
9. nodemap: visual nodemap editor
10. container: window container
11. shell: windows-shell extension
12. vecmath: math libraries
13. uimgr: UI manager

## How-to-use

1. For users,just download the release(why people will try to play this?)
2. For developers trying to create a new game with this,clone this,write your game in main,then use mingw to compile.
3. For developers trying to use this as a library,read the "Architecture" part ;)

## Disclaimer

Bugs are expected fot this project is still under development.

## Default hotkeys

1. "q"：rotate（deprecated）
2. "e"：reverse-rotate（deprecated）
3. "p"：add a new point
4. "f"：add lines or planes（make point selection first!）
5. "g"：move
6. "r"：rotate（under dev）
7. "s"：zoom（under dev）
8. "c"：color selector（make point selection first!）
9. "x"：X-axis operation
10. "y"：Y-axis operation
11. "z"：Z-axis operation
12. "X"(shift + x)：YZ-plane operation
13. "Y"(shift + y)：ZX-plane operation
14. "Z"(shift + z)：XY-plane operation
15. delete：delete a point
16. ESC：exit
17. Ctrl+S：save
18. Ctrl+L: open
19. Alt+(0-9): toolbar selection
20. Alt+/ Alt+?：about
21. "e": export a new point(buggy)

## Contribution

Dev:

1,Fork the repository
2,Create Feat_xxx branch
3,Commit your code
4,Create Pull Request

User:
1,Create an issue
2,Describe your problem

## Known bugs

1. issues:I4S2IG memory leak in Mesh