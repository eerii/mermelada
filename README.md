# aguacate :avocado:

A template repository for [fresa :strawberry:](https://github.com/josekoalas/fresa).

## building :hammer:

**Platforms**

Right now the project is in the **very pre-alpha** state. Only MacOS is tested, though Web, Linux and Windows support should come soon.

**Install CMake**

You can download `CMake` from the [official website](https://cmake.org/download/), which is probably the easiest way. If you are on MacOS and have homebrew installed, you can simply do `brew install cmake`. There is also a Debian/Ubuntu [repository](https://apt.kitware.com).

**Build**

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
cmake -S . -B build && cmake --build build --target TARGET
```

**Targets**

Right now it contains three targets:
-  OpenGL (`cmake -S . -B build && cmake --build build --target fresa-opengl`)
-  Vulkan (`cmake -S . -B build && cmake --build build --target fresa-vulkan`)
-  Web (`emcmake cmake -S . -B build && cmake --build build --target fresa-web`)

You can rename the targets or add new ones in the `CMakeLists.txt` file.

**Dependencies**

Some dependencies already come with this sample repository as submodules (imGUI, SPIR-V Cross, glm, stb and VulkanMemoryAllocator). You can find them in the [deps](https://github.com/josekoalas/aguacate/tree/main/deps) folder. Most of them

SDL2, which handles cross platform window and input management, needs to be installed for every configuration:
- MacOS: Install using `brew install sdl2`.
- Debian/Ubuntu: Install using `apt-get install libsdl2-dev`.
- Windows: Download development libraries [here](https://libsdl.org/download-2.0.php).
- You can check the [official documentation](https://wiki.libsdl.org/Installation) for more information.

Additionally, for Vulkan, you need to install the Vulkan SDK Development Kit, which contains the relevant Vulkan libraries. You can refer to the [official webpage](https://vulkan.lunarg.com/sdk/home) to download an installer for your operating system (choose "Latest SDK"). The installation should be one click and then CMake can find the library automatically when building.

OpenGL cross platform support is still in the works. For MacOS you shouldn't need to download anything, and other platforms still need to be tested.

Building for web requires [emscripten](https://emscripten.org/docs/getting_started/downloads.html), check their site for instructions. **The web build can't render anything right now, it has an issue with shaders that is being worked on.**

**Output**

After the build, the `bin` folder will contain a binary named after the target you chose and a copy of the `res` folder. You can execute the binary for the application to run. Packaging it into an app container is still being worked on.