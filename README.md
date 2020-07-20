# Realms of Ostaga

The Realms of Ostaga, establish yourself on the strange mythical island of Ostaga. Uncover the mysteries of the ancients. Harness the power of the celestials. Use your strength in combat, and become the single most powerful being of this mysterious world.

### Cloning

This repository will rely on *sub-modules*. In order to clone this repository you *will need to* specify the `--recursive` flag like so:

    git clone --recursive <repository>

This will ensure that all necessary dependencies are downloaded along with the project.

### System Requirements
---

The goal for this project is to have cross-platform support. That being said, it will primarily be tested on Windows 10 before being tested on other platforms. Some of the premake files may need tinkering at the moment in order to build for platforms other than vs2019.

This project will also be using the latest version of OpenGL (4.6 as of this time) for the development process. This may be subject to change in the final product.

### Building
---
This project makes use of premake5 in order to build the necessary project files. By using premake this project can be ported to several different platforms and it can be built on several different development systems (e.g. Visual Studio). This means that the repository only contains the source code and nothing else (no project files, make files, etc.).

Premake binaries are located in a single directory labeled "build" along with the premake5 license and some accompanying scripts useful for quickly building the project files on your platform. Currently, the available script files are:
 - Win-Build.bat (for Windows)

The quickest way to create the project files for your environment of choice is to simply run one of the previously mentioned script files for your system.

However, if you wish to manually bulid the project files via the command line or there is no current support for your build system of choice, then navigate to the root directory of this repository (the one containing this *README*) and on the command line and run:

    ./build/premake5 [action]

The `[action]` variable will specify for which build system you intend to build project files. For example, if you wanted to build project files for Visual Studio 2019 you would use:

    ./build/premake5 vs2019

This would create all the necessary project files required to build the project in Visual Studio 2019. Use the following command to view which build systems are available: ```premake5 --help```

### Dependencies
---
The following is a list of 3rd party dependencies that *are* used in this project:

 - [GLFW](https://gitlab.com/Sympleque/glfw)
 - [Glad](https://glad.dav1d.de/)
 - [Spdlog](https://github.com/gabime/spdlog)
 - [ImGUI](https://gitlab.com/Sympleque/imgui)
 - [glm](https://github.com/g-truc/glm)
 - [stb](https://github.com/nothings/stb)
 - [openal-soft](https://gitlab.com/Sympleque/openal-soft)
 - [dr_wav](https://github.com/mackron/dr_libs/blob/master/dr_wav.h)