# aguacate :avocado:

A template repository for [fresa :strawberry:](https://github.com/josekoalas/fresa).

## building :hammer:

**Platforms**

Right now the project is in the **very pre-alpha** state. Only MacOS is tested, though Web, Linux and Windows support should come soon.

#### Prerequisites

**CMake**

`Cmake` is our build system of choice. You can download it from the [official website](https://cmake.org/download/), which is probably the easiest way. If you are on MacOS and have homebrew installed, you can simply do `brew install cmake`. There is also a Debian/Ubuntu [repository](https://apt.kitware.com).

**SDL2**

SDL2 handles cross platform window and input management. It needs to be installed for all non web builds. You can do so by:
- MacOS: Install using `brew install sdl2`.
- Debian/Ubuntu: Install using `apt install libsdl2-dev`.
- Windows: Download development libraries [here](https://libsdl.org/download-2.0.php).
- You can check the [official documentation](https://wiki.libsdl.org/Installation) for more information.

**Vulkan**

If you are going to build for Vulkan, you need to install the Vulkan SDK Development Kit, which contains the relevant Vulkan libraries. You can refer to the [official webpage](https://vulkan.lunarg.com/sdk/home) to download an installer for your operating system (choose "Latest SDK").
- MacOS/Windows: The installation should be one click.
- Debian/Ubuntu: You can download a .tar file, which you can decompress in a folder such as `~/vulkan`. Then, add the line `source ~/vulkan/1.x.yy.z/setup-env.sh` to `.profile` or the equivalent shell file, replacing the letters with the corresponding folder version name. There is also a repository for the latest versions of Ubuntu.
- For other OS or clarification follow the [official docs](https://vulkan.lunarg.com/doc/sdk/latest/linux/getting_started.html).

**OpenGL**

On MacOS you don't need to do anything to build for OpenGL. For Linux, install `apt install glew-dev`. Windows is not supported yet.

**Web**

Building for web requires [emscripten](https://emscripten.org/docs/getting_started/downloads.html), check their site for instructions. **The web build can't render anything right now, it has an issue with shaders that is being worked on.**

**Other dependencies**

Some dependencies already come with this sample repository as submodules (imGUI, SPIR-V Cross, glm, stb and VulkanMemoryAllocator). You can find them in the [deps](https://github.com/josekoalas/aguacate/tree/main/deps) folder. For now OpenGL is just tested on MacOS, which alredy includes the libraries, so nothing has to be done. Support for other platforms is coming.

#### Build

**A compiler that supports C++20 is required, the latest versions of Clang or GCC are recommended.**

1. Clone the repository

```
git clone --recurse-submodules https://github.com/josekoalas/aguacate
```

2. Go to the folder

```
cd aguacate
```

3. Build using CMake (replace TARGET as indicated below)

```
cmake -S . -B build
cmake --build build --target TARGET
```

**Targets**

Right now it contains three targets:
-  OpenGL (`cmake -S . -B build && cmake --build build --target fresa-opengl`)
-  Vulkan (`cmake -S . -B build && cmake --build build --target fresa-vulkan`)
-  Web (`emcmake cmake -S . -B build && cmake --build build --target fresa-web`)

You can rename the targets or add new ones in the `CMakeLists.txt` file.

**Output**

After the build, the `bin` folder will contain a binary named after the target you chose and a copy of the `res` folder. You can execute the binary for the application to run. Packaging it into an app container is still being worked on.