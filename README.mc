# Nene Engine

Nene Engine is an in-house game engine named after Sakura Nene's game engine in anime series [<<New Game!!>>](http://newgame-anime.com/).




## Getting Start

### Preliminary

Nene Engine use Python as main scripting language and tool-chain language. The following Python must be satisfied:

- Python >= 3.10
- Pyside2 == 5.15.2

Note that Nene Engine use Visual Studio's Project file  ( instead of CMake ) as a primary way to organize the source files. The following C++ environment should be satisfied in Windows:

- Visual Studio >= 2022.17.2
- Compiler C++ Standard >= C++20 ( MSVC >= 143 ) [*]

Currently Nene Engine only supports Microsoft Windows 10, 11 with Direct3D 12. 
Apple's MacOS, iPadOS, iOS with Metal 2, Linux with Vulkan will be supported in the future.

[*] [Microsoft adds support for `std::format` after Visual Studio 16.10]()


### Development

#### Build and Run

Currently Nene Engine is build with Visual Studio. To build Nene Engine, simply  press ▶ in the IDE.



#### Tool Chain

To deal with module dependency, we use a Python based custom build tool ( Nene Build Tool or NBT ) which modify the Visual Studio project and solution files.

All source of the NBT is located in `toolchain` folder, which is also the main entrance python module of NBT.

After adding, removing or modifying a module, the best way to ensure that IDE can successfully build is to run NBT once:

```bash
$ NeneEngine>: py -3 toolchain
```

The `--help` parameter is also supported by NBT.



#### Module Scheme

NBT finds the configuration of each module in `module_name.py` file.



#### Project Scheme

You can add, remove module directly in Visual Studio. Just don't forget to run NBT after doing this.



### Usage

The Nene Engine has two mode: editor mode and game mode. Both mode are built to a python library ( `nene.pyd` in windows for example ).

Hence, you need to start the engine in python.

For editor:

```bash
$ NeneEngine>: py -3 script/editor
```



For game:

```bash
$ NeneEngine>: py -3 $(path_to_your_project)/main.py
```



## Introduction

### Modules

{{ModuleDepency}}



## Open Source

Nene Engine cannot live without the forces of open sources. Especially the following brilliant open source library:

- [Qt5](https://www.qt.io/download-open-source) && [Pyside2](https://doc.qt.io/qtforpython-5/index.html)
- [Python3](https://www.python.org/)
- [Pybind11](https://github.com/pybind/pybind11)
- [RTTR](https://www.rttr.org/)
- [Taskflow](https://taskflow.github.io/)
- [Assimp](https://github.com/assimp/assimp)



Huge shout out to the developers of these open source library and the brilliant brains who helped or inspired Nene Engine's development.
