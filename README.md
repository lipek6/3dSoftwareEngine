# CPU-Based 3D Software Rendering Engine

A purely software-based 3D Graphics Engine written in C++. This project demonstrates the fundamentals of 3D projection, rasterization, clipping, and texturing without relying on hardware acceleration (GPU) or modern 3D libraries like OpenGL, Vulkan or DirectX.

*(PICTURE)*

## About The Project

This engine renders 3D meshes by calculating the geometry and pixel data entirely on the CPU. It serves as a deep-dive study into how computer graphics work at a low level, covering the transformation pipeline from model space to screen space.

### Inspiration & Attribution

This project is based on the incredible **"Code-It-Yourself! 3D Graphics Engine"** series by [Javidx9 (OneLoneCoder)](https://github.com/OneLoneCoder/Javidx9/tree/master/ConsoleGameEngine/BiggerProjects/Engine3D).

While the core mathematical concepts and algorithms are derived from Javidx9's lectures, **this implementation diverges significantly in software architecture.**

### Key Differences & Refactoring

The primary goal of this version was to take the original procedural/single-file approach and refactor it into a **robust, Object-Oriented (OOP) architecture**.

* **Modular File Structure:** The code is split into logical `.h` and `.cpp` files (`Engine3d`, `Mesh`, `Matrix`, `Vector`, etc.) rather than a monolithic file.
* **Operator Overloading:** Mathematical operations are significantly more readable.
* *Old way:* `Vector_Add(v1, v2)`
* *This version:* `v1 + v2`


* **Encapsulation:** Rendering logic, matrix math, and file loading are encapsulated within their respective classes/structs, making the codebase easier to maintain and extend.
* **Modern C++ Standards:** Improved type safety and organization using `std::vector`, constructors, const correctness and struct methods.

## Features

* **Software Rasterization:** All pixel drawing is calculated mathematically on the CPU.
* **6 Degrees of Freedom (6DoF) Camera:** Move and look around the 3D world (First Person Camera).
* **OBJ File Loading:** Parses standard `.obj` files to load complex meshes (supports vertices and texture coordinates).
* **Texture Mapping:** Supports affine texture mapping with perspective correctness.
* **Lighting:** Basic directional lighting based on surface normals.
* **Frustum Clipping:** Triangles are clipped against the screen edges.
* **Near-Plane Clipping:** Prevents rendering artifacts when objects are too close to the camera.
* **Z-Buffer (Depth Buffer):** Solves the visibility problem (handling overlapping objects) per pixel.


* **Math Library:** A custom-built implementation of 3D Vectors (`Vec3d`) and 4x4 Matrices (`Matrix4x4`) with full linear algebra operations.

## Tech Stack

* **Language:** C++
* **Windowing/Input:** [`olcPixelGameEngine`](https://github.com/OneLoneCoder/olcPixelGameEngine) (A single-header library for creating windows and drawing pixels).
* **Renderer:** Custom Software Renderer (No GPU acceleration).

## Project Structure

* `main.cpp`: Entry point. Initializes the engine and window resolution.
* `Engine3d`: The core engine class. Handles the game loop (`OnUserUpdate`), pipeline logic, and rasterization.
* `Mesh` & `Triangle`: Data structures for storing geometric data and loading `.obj` files.
* `Vec3d` & `Vec2d`: Mathematical vector structures with operator overloading.
* `Matrix4x4`: Matrix math library for translations, rotations, and projections.

## Controls

* **W / S:** Move Forward / Backward (along look vector)
* **LEFT ARROW / RIGHT ARROW:** Strafe Left / Right
* **UP ARROW / DOWN ARROW:** Move Up / Down (Vertical)
* **A / D:** Yaw (Rotate Left / Right)

## How to Build

1. Clone the repository.
2. Ensure you have a C++ compiler compatible with C++17.
3. Open the solution in **Visual Studio** (recommended) or your preferred IDE.
4. Ensure `olcPixelGameEngine.h` is in your include path.
5. Build the solution.
6. **Important:** Make sure the `assets` folder (containing `.obj` files and `.png` textures) is located in the working directory of the executable.

## How to Execute 

- For the 256x240 version:
1. Dowload `3dEngine_executable_RetroGraphics.zip`.
2. Unzip the folder.
3. Run the executable.

- For the 1920x1080 version:
1. Dowload `3dEngine_executable_HighGraphics.zip`.
2. Unzip the folder.
3. Run the executable.

## Acknowledgments

Special thanks to **Javidx9** for making low-level graphics programming accessible.

* [OneLoneCoder YouTube Channel](https://www.youtube.com/javidx9)
* [OneLoneCoder GitHub](https://github.com/OneLoneCoder)

---

*This project is for educational purposes, focusing on code architecture and the fundamentals of 3D graphics.*
