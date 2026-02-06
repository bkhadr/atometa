# Build Instructions

Complete guide for building Atometa on Windows with Visual Studio.

## Prerequisites

### Required Software

1. Visual Studio 2019 or later
   - Install "Desktop development with C++" workload
   - Download: https://visualstudio.microsoft.com/

2. CMake 3.20 or later
   - Download: https://cmake.org/download/
   - During installation, select "Add CMake to system PATH"

3. Git
   - Download: https://git-scm.com/

4. vcpkg (Package Manager)
   - Will be installed in Step 1 below

### System Requirements

- Windows 10/11 (64-bit)
- 8 GB RAM minimum
- 2 GB free disk space
- OpenGL 3.3+ compatible graphics card

## Step-by-Step Build Process

### Step 1: Install vcpkg

Open Command Prompt or PowerShell:

```cmd
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
```

IMPORTANT: Remember the vcpkg installation path (C:\vcpkg in this example).

### Step 2: Install Project Dependencies

Still in the vcpkg directory, run:

```cmd
vcpkg install glfw3:x64-windows
vcpkg install glad:x64-windows
vcpkg install glm:x64-windows
vcpkg install imgui[glfw-binding,opengl3-binding]:x64-windows
vcpkg install eigen3:x64-windows
vcpkg install stb:x64-windows
```

This will take 5-15 minutes on first installation.

Alternative - Install all at once:

```cmd
vcpkg install glfw3:x64-windows glad:x64-windows glm:x64-windows imgui[glfw-binding,opengl3-binding]:x64-windows eigen3:x64-windows stb:x64-windows
```

### Step 3: Navigate to Project Directory

```cmd
cd C:\atometa
```

### Step 4: Configure CMake

```cmd
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Replace C:/vcpkg with your actual vcpkg installation path if different.

### Step 5: Build the Project

For Release build (optimized):

```cmd
cmake --build build --config Release
```

For Debug build (with debug symbols):

```cmd
cmake --build build --config Debug
```

Build both configurations:

```cmd
cmake --build build --config Release
cmake --build build --config Debug
```

### Step 6: Run the Application

After successful build:

```cmd
build\bin\Release\Atometa.exe
```

Or for Debug:

```cmd
build\bin\Debug\Atometa.exe
```

### Step 7: Open in Visual Studio

```cmd
start build\Atometa.sln
```

In Visual Studio:
1. Select "Release" or "Debug" configuration from the toolbar
2. Set Atometa as the startup project (right-click -> Set as Startup Project)
3. Press F5 to build and run
4. Press Ctrl+F5 to run without debugging

## Fast Build Methods

### Method 1: Ninja Generator (30-50% Faster)

Ninja is significantly faster than MSBuild for C++ projects.

Install Ninja:

```cmd
vcpkg install ninja:x64-windows
```

Configure with Ninja:

```cmd
cmake -G Ninja -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Build:

```cmd
cmake --build build
```

NOTE: Ninja builds will not generate Visual Studio solution files. Use MSBuild configuration if you need the .sln file.

### Method 2: Enable ccache (Faster Rebuilds)

ccache caches compilation results and dramatically speeds up rebuilds.

Install:

```cmd
vcpkg install ccache:x64-windows
```

CMake will automatically detect and use ccache. No additional configuration needed.

### Method 3: Precompiled Headers

Already enabled by default in CMakeLists.txt.

Precompiled headers cache frequently-included headers like:
- STL containers (vector, string, memory, unordered_map)
- GLM headers
- Third-party library headers

This reduces compilation time by 20-40% for large projects.

### Method 4: Multi-processor Compilation

Automatically enabled with /MP flag in CMakeLists.txt for Visual Studio.

Uses all available CPU cores during compilation.

## Build Configuration Options

### Disable Precompiled Headers

```cmd
cmake -B build -S . -DATOMETA_USE_PCH=OFF -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### Disable Tests

```cmd
cmake -B build -S . -DATOMETA_BUILD_TESTS=OFF -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### Disable Warnings

```cmd
cmake -B build -S . -DATOMETA_ENABLE_WARNINGS=OFF -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

## Troubleshooting

### Problem: CMake cannot find vcpkg

Solution: Verify the CMAKE_TOOLCHAIN_FILE path is correct.

```cmd
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

Use forward slashes (/) in the path, not backslashes (\).

### Problem: "LINK : fatal error LNK1104: cannot open file 'glfw3.lib'"

Solution: Ensure dependencies are installed for the correct architecture (x64):

```cmd
vcpkg install glfw3:x64-windows
```

NOT:

```cmd
vcpkg install glfw3  # This installs x86 by default
```

### Problem: "OpenGL version 3.3 is not supported"

Solution: Update your graphics drivers.

For Intel HD Graphics 4600:
1. Visit Intel Download Center
2. Download latest graphics driver for your model
3. Install and restart

### Problem: Build is very slow

Solutions:
1. Use Ninja generator (see Fast Build Methods)
2. Install ccache (see Fast Build Methods)
3. Close other applications to free up RAM
4. Build only Release OR Debug, not both

### Problem: "vcpkg not found"

Solution: Add vcpkg to your PATH or use full path in CMAKE_TOOLCHAIN_FILE.

## Library Usage After Build

### GLFW - Window Management

```cpp
#include <GLFW/glfw3.h>

// Already integrated in Window.cpp
// Use Atometa::Window class instead of raw GLFW
```

### GLAD - OpenGL Functions

```cpp
#include <glad/glad.h>

// Initialize OpenGL functions (done in Window::Init)
gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

// Use OpenGL functions
glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT);
```

### GLM - Mathematics

```cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Vectors
glm::vec3 position(0.0f, 1.0f, 0.0f);
glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);

// Matrices
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, position);
model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

// Projection
glm::mat4 projection = glm::perspective(
    glm::radians(45.0f),  // FOV
    800.0f / 600.0f,      // Aspect ratio
    0.1f, 100.0f          // Near, far planes
);
```

### ImGui - User Interface (Future)

```cpp
#include <imgui.h>

// In render loop
ImGui::Begin("Control Panel");
ImGui::Text("Atom Count: %d", atomCount);
ImGui::SliderFloat("Temperature", &temperature, 0.0f, 1000.0f);
if (ImGui::Button("Reset Simulation")) {
    ResetSimulation();
}
ImGui::End();
```

### Eigen - Linear Algebra

```cpp
#include <Eigen/Dense>

// Matrix operations
Eigen::Matrix3d rotationMatrix;
Eigen::Vector3d atomPosition(1.0, 2.0, 3.0);
Eigen::Vector3d rotatedPosition = rotationMatrix * atomPosition;

// Solve linear systems
Eigen::MatrixXd A(3, 3);
Eigen::VectorXd b(3);
Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);
```

## Build Performance Benchmarks

Approximate build times on Intel i5-4570 @ 3.2GHz, 8GB RAM:

| Configuration | Time (First Build) | Time (Rebuild) |
|---------------|-------------------|----------------|
| MSBuild Release | 45-60 seconds | 15-25 seconds |
| MSBuild Debug | 30-45 seconds | 10-20 seconds |
| Ninja Release | 25-35 seconds | 8-15 seconds |
| Ninja + ccache | 25-35 seconds | 3-8 seconds |

Note: Times will increase as more code is added to the project.

## Next Steps After Successful Build

1. Verify the application runs and displays a black window
2. Start implementing the rendering pipeline (Shader class, Camera class)
3. Add ImGui integration for UI
4. Implement basic atom rendering (spheres)
5. Add molecule support
6. Commit to Git and push to GitHub

## Continuous Development

### Rebuild After Code Changes

```cmd
cmake --build build --config Release
```

### Clean Build

```cmd
cmake --build build --target clean
cmake --build build --config Release
```

### Reconfigure CMake (After Adding Files)

```cmd
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

## Getting Help

- Build issues: Check Troubleshooting section above
- Library usage: See library documentation linked in README
- CMake problems: https://cmake.org/documentation/
- vcpkg problems: https://vcpkg.io/en/docs/troubleshooting.html

---

Last Updated: February 2026