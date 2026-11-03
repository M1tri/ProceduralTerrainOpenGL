# Procedural Terrain OpenGL

A C++ application that generates and renders procedurally generated voxel terrain using OpenGL.

The terrain is generated using **Perlin Noise** and divided into chunks, allowing the application to render a larger world as a collection of smaller, independently generated sections.

## Features

- Procedural terrain generation using multi-octave Perlin Noise.
- Chunk-based terrain representation (16 × 16 × 256 blocks per chunk).
- 8 × 8 chunk world generation.
- Mesh generation with face culling to avoid rendering hidden block faces.
- OpenGL rendering using Vertex Array Objects (VAOs), Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs).
- Basic directional diffuse lighting implemented in GLSL shaders.
- First-person camera with mouse look and keyboard movement.

## Technologies

- **C++17**
- **OpenGL 3.3 Core Profile** – real-time rendering.
- **GLFW** – window and input management.
- **GLEW** – OpenGL function loading.
- **GLM** – mathematics library.
- **[siv::PerlinNoise](https://github.com/Reputeless/PerlinNoise)** – header-only C++ library used for procedural terrain generation using Perlin Noise.
- **GLSL** – vertex and fragment shaders.
- **CMake** – build system.

## Project Structure

```text
ProceduralTerrainOpenGL/
├── ThirdParty/
│   └── PerlinNoise/
├── shaders/
│   ├── basic.vert
│   └── basic.frag
├── src/
│   ├── main.cpp
│   ├── Chunk.cpp
│   ├── Chunk.h
│   ├── Camera.h
│   └── Shader.h
└── CMakeLists.txt
```

## Building

### Prerequisites

Make sure the following dependencies are installed:

- C++ compiler with C++17 support
- CMake 3.16 or newer
- OpenGL development libraries
- GLFW
- GLEW
- GLM

The Perlin Noise implementation is included in the repository.

### Build Instructions

Clone the repository:

```bash
git clone https://github.com/M1tri/ProceduralTerrainOpenGL.git
cd ProceduralTerrainOpenGL
```

Create a build directory and configure the project:

```bash
cmake -S . -B build
```

Compile the application:

```bash
cmake --build build
```

Run the executable:

```bash
./build/MojOpenGL
```

## Controls

| Key / Input | Action |
|---|---|
| W | Move forward |
| S | Move backward |
| A | Move left |
| D | Move right |
| Space | Move upward |
| Left Shift | Move downward |
| Mouse | Rotate camera |
| Escape | Exit application |

## Terrain Generation

The terrain is generated using multi-octave Perlin Noise to calculate the height of each terrain column.

The resulting terrain is divided into chunks, each containing a 16 × 16 horizontal block grid and a maximum height of 256 blocks.

A mesh is constructed for each chunk by generating only the visible faces of non-empty blocks. The resulting geometry is uploaded to the GPU and rendered using OpenGL.

## Future Improvements

Potential areas for further development include:

- Configurable terrain generation parameters and random seeds.
- Additional terrain types and biome generation.
- Improved chunk management and mesh optimization.
- Frustum culling and Level of Detail (LOD).
- Texture mapping and additional terrain materials.
- Improved lighting and atmospheric effects.
- Performance evaluation and comparison of terrain generation techniques.

## Acknowledgements

This project uses the [siv::PerlinNoise](https://github.com/Reputeless/PerlinNoise) library by **Reputeless** for procedural noise generation.

Special thanks to the author for providing this open-source implementation.

## Project Status

This project is currently under development. The existing implementation serves as a foundation for further exploration of procedural terrain generation and real-time rendering techniques.
