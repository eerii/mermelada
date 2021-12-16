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

You can rename the targets or add new ones in the `CMakeLists.txt` file. Building for web requires [emscripten](https://emscripten.org/docs/getting_started/downloads.html), check their site for instructions. **The web build can't render anything right now, it has an issue with shaders that is being worked on.**

**Dependencies**

Some dependencies already come with this sample repository as submodules (imGUI, SPIR-V Cross, glm, stb and VulkanMemoryAllocator). You can find them in the [deps](https://github.com/josekoalas/aguacate/tree/main/deps) folder.

OpenGL should be in your system already, and CMake will search for it automatically. Vulkan is also packaged in the deps folder.

The only dependency that has to be installed for this project to compile is SDL2.
- MacOS: Install using `brew install sdl2`.
- Debian/Ubuntu: Install using `apt-get install libsdl2-dev`.
- Windows: Download development libraries [here](https://libsdl.org/download-2.0.php).
- You can check the [official documentation](https://wiki.libsdl.org/Installation) for more information.

**Output**

After the build, the `bin` folder will contain a binary named after the target you chose and a copy of the `res` folder. You can execute the binary for the application to run. Packaging it into an app container is still being worked on.