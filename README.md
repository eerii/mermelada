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

Right now it contains two targets, one for OpenGL (`fresa-opengl`) and one for Vulkan (`fresa-vulkan`):
```
cmake -S . -B build && cmake --build build --target fresa-opengl
```
```
cmake -S . -B build && cmake --build build --target fresa-vulkan
```
You can rename the targets or add new ones in the `CMakeLists.txt` file.

**Output**

After the build, the `bin` folder will contain a binary named after the target you chose and a copy of the `res` folder. You can execute the binary for the application to run. Packaging it into an app container is still being worked on.