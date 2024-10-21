# Typical Custom Engine
## Brief Overview
A custom written C++ Engine that aims to unify and simplify basic 3D games development using file-structures and philosophies that are common-place within most popular 3D Game Engines!
This engine supports numerous file formats, can export and load data during runtime and can even utilize the power of AngelScript to provide on-the-fly scripting without needing C++ hard-coded contents!

## Disclamers
- As of writing this, the only currently supported rendering API is OpenGL, however I aim to implement Vulken support later down the road at some point once the core engine has been developed enough!
- All development as of right now has been exclusively done in Windows with Visual Studio 2022, further testing on other platforms such as Linux will come later, for now, _I DO NOT GUARANTEE IT'LL WORK UNDER ANYTHING ELSE!_
- This engine does _NOT_ connect to the internet, it is a strictly offline engine for the time being, a fork later down the road may be produced to add network functionality, however, for the time being, I _do not plan on adding networking to the engine._

## Dependencies
To produce the final product, "Typical Custom Engine" utilizes multiple dependencies to ensure it's creation, please refer to the following links below for more information on all the dependencies used.

- [OpenGL](https://www.opengl.org/) : A widely known Rendering API that can run on several Operating Systems and devices.
- [GLFW](https://www.glfw.org/) : A utility library for OpenGL to make initalizing and managing OpenGL easier.
- [Assimp](https://assimp.org/) : An asset importer can import meshes with all their verticies, among other things into the application runtime.
- [MINIAUDIO](https://miniaud.io/index.html) : A single header audio library system, used to manage playback of all audio within the engine.
- [Lua](https://www.lua.org/) : An efficient and easy-to-use scripting language, used to implement scriptable objects and to manage code without the need of re-compiling or tampering with the engine code-base itself.

## Usage
If you simply want to use the engine without using it's source code, then head on over to the [releases](https://github.com/coreyabraham/typical-custom-engine/releases) tab!
Instructions are provided below, detailing how you can edit the source code of the engine if you so wish.

## Source Code Setup
1. Clone the repository to your local disk, here's the link to do so: https://github.com/coreyabraham/typical-custom-engine.git
2. Open your IDE of choice, in this example I'll use Visual Studio 2022.
3. {WORKING IN PROGRESS}

## Pull Requests
Once the engine is released, pull requests are encourged to further it's development, guidelines and soft "requirements" are not provided at this time, as my main focus is on the engine itself, thank you for understanding.

## Thank You!
Thank you for reading and or showing interest in my little pet project, any feedback is much appreciated!
