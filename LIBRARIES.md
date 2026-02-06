# Library Reference Guide

Complete reference for all libraries used in Atometa.

## Core Libraries

### 1. GLFW - Window and Input Management

**Purpose:** Cross-platform window creation and input handling  
**Version:** 3.3+  
**Website:** https://www.glfw.org/  
**Documentation:** https://www.glfw.org/documentation.html  

**Installation:**
```cmd
vcpkg install glfw3:x64-windows
```

**Key Features:**
- Window creation and management
- Keyboard and mouse input
- OpenGL context creation
- Multi-monitor support
- Joystick/gamepad support

**Usage in Atometa:**
```cpp
#include <GLFW/glfw3.h>

// Encapsulated in Atometa::Window class
// See: src/core/Window.cpp
```

**Common Functions:**
```cpp
glfwInit()                           // Initialize GLFW
glfwCreateWindow()                   // Create window
glfwMakeContextCurrent()            // Set OpenGL context
glfwPollEvents()                    // Process events
glfwSwapBuffers()                   // Swap front/back buffers
glfwGetKey()                        // Check keyboard state
glfwGetMouseButton()                // Check mouse state
glfwGetCursorPos()                  // Get mouse position
```

---

### 2. GLAD - OpenGL Loader

**Purpose:** Load OpenGL functions at runtime  
**Website:** https://glad.dav1d.de/  

**Installation:**
```cmd
vcpkg install glad:x64-windows
```

**Why Needed:**
OpenGL functions are not directly available on Windows. GLAD loads them dynamically at runtime based on your graphics driver.

**Usage:**
```cpp
#include <glad/glad.h>

// Initialize GLAD (done once in Window::Init)
gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

// Now you can use OpenGL functions
glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT);
glDrawArrays(GL_TRIANGLES, 0, 3);
```

**OpenGL Version:** 3.3 Core Profile

---

### 3. GLM - Mathematics Library

**Purpose:** Mathematics for graphics programming  
**Version:** 0.9.9+  
**Website:** https://glm.g-truc.net/  
**Documentation:** https://github.com/g-truc/glm/blob/master/manual.md  

**Installation:**
```cmd
vcpkg install glm:x64-windows
```

**Key Features:**
- Vector types (vec2, vec3, vec4)
- Matrix types (mat2, mat3, mat4)
- Quaternions for rotations
- Transformation functions
- Camera projection functions
- GLSL-like syntax

**Common Usage:**
```cpp
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Vectors
glm::vec3 position(0.0f, 1.0f, 0.0f);
glm::vec3 direction = glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f));

// Matrices
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, position);
model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
model = glm::scale(model, glm::vec3(2.0f));

// View matrix (camera)
glm::mat4 view = glm::lookAt(
    glm::vec3(0.0f, 0.0f, 5.0f),  // Camera position
    glm::vec3(0.0f, 0.0f, 0.0f),  // Look at point
    glm::vec3(0.0f, 1.0f, 0.0f)   // Up vector
);

// Projection matrix
glm::mat4 projection = glm::perspective(
    glm::radians(45.0f),    // Field of view
    16.0f / 9.0f,          // Aspect ratio
    0.1f,                  // Near plane
    100.0f                 // Far plane
);

// Pass to shader
glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(model));
```

---

### 4. ImGui - Immediate Mode GUI

**Purpose:** User interface framework  
**Version:** 1.89+  
**Website:** https://github.com/ocornut/imgui  
**Documentation:** https://github.com/ocornut/imgui/wiki  

**Installation:**
```cmd
vcpkg install imgui[glfw-binding,opengl3-binding]:x64-windows
```

**Key Features:**
- Immediate mode paradigm (no retained state)
- Built-in widgets (buttons, sliders, text inputs, trees, etc.)
- Docking and multi-viewport support
- Customizable themes
- Minimal dependencies

**Basic Usage:**
```cpp
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Initialize (once at startup)
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();
ImGui_ImplGlfw_InitForOpenGL(window, true);
ImGui_ImplOpenGL3_Init("#version 330");

// In render loop
ImGui_ImplOpenGL3_NewFrame();
ImGui_ImplGlfw_NewFrame();
ImGui::NewFrame();

// Create UI
ImGui::Begin("Control Panel");
ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
static float temperature = 298.15f;
ImGui::SliderFloat("Temperature (K)", &temperature, 0.0f, 1000.0f);
if (ImGui::Button("Reset")) {
    temperature = 298.15f;
}
ImGui::End();

// Render
ImGui::Render();
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
```

**Demo Application:**
The ImGui repository includes a comprehensive demo showing all widgets:
```cpp
ImGui::ShowDemoWindow();  // Add this to see all features
```

---

### 5. Eigen - Linear Algebra Library

**Purpose:** Advanced linear algebra for scientific computing  
**Version:** 3.4+  
**Website:** https://eigen.tuxfamily.org/  
**Documentation:** https://eigen.tuxfamily.org/dox/  

**Installation:**
```cmd
vcpkg install eigen3:x64-windows
```

**Key Features:**
- Matrix and vector operations
- Linear solvers
- Eigenvalue/eigenvector computation
- Sparse matrices
- Geometry transformations
- Header-only library (no linking required)

**Usage:**
```cpp
#include <Eigen/Dense>
#include <Eigen/Sparse>

// Basic matrix operations
Eigen::Matrix3d rotation;
rotation = Eigen::AngleAxisd(angle, Eigen::Vector3d::UnitZ());

Eigen::Vector3d position(1.0, 2.0, 3.0);
Eigen::Vector3d rotated = rotation * position;

// Solve linear system Ax = b
Eigen::MatrixXd A(3, 3);
Eigen::VectorXd b(3);
// Fill A and b...
Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);

// Eigenvalue decomposition
Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(matrix);
Eigen::Vector3d eigenvalues = solver.eigenvalues();
Eigen::Matrix3d eigenvectors = solver.eigenvectors();
```

**Use Cases in Chemistry:**
- Molecular orbital calculations
- Principal component analysis
- Covariance matrix calculations
- Coordinate transformations
- Force field matrix operations

---

### 6. stb_image - Image Loading

**Purpose:** Simple image loading library  
**Website:** https://github.com/nothings/stb  

**Installation:**
```cmd
vcpkg install stb:x64-windows
```

**Supported Formats:**
- PNG
- JPG/JPEG
- BMP
- TGA
- GIF
- PSD

**Usage:**
```cpp
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Load image
int width, height, channels;
unsigned char* data = stbi_load("texture.png", &width, &height, &channels, 0);

if (data) {
    // Use image data (create OpenGL texture, etc.)
    // ...
    
    // Free memory
    stbi_image_free(data);
} else {
    // Handle error
    const char* error = stbi_failure_reason();
}

// Force specific channel count
unsigned char* rgb_data = stbi_load("image.png", &width, &height, &channels, 3);  // Force RGB
unsigned char* rgba_data = stbi_load("image.png", &width, &height, &channels, 4);  // Force RGBA
```

---

## Chemistry Libraries (Future Integration)

### Open Babel

**Purpose:** Chemical file format conversion and molecular manipulation  
**Version:** 3.1+  
**Website:** http://openbabel.org/  
**Documentation:** http://openbabel.org/wiki/Main_Page  

**Installation:**
```cmd
vcpkg install openbabel:x64-windows
```

**Supported Formats:**
- PDB (Protein Data Bank)
- MOL/SDF (MDL Molfile)
- XYZ (Cartesian coordinates)
- CML (Chemical Markup Language)
- SMILES (Simplified Molecular Input Line Entry System)
- MOL2 (Sybyl Mol2)
- And 100+ other formats

**Basic Usage:**
```cpp
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>

OpenBabel::OBConversion conv;
conv.SetInFormat("PDB");

OpenBabel::OBMol molecule;
if (conv.ReadFile(&molecule, "protein.pdb")) {
    // Process molecule
    int numAtoms = molecule.NumAtoms();
    
    for (OpenBabel::OBAtomIterator it = molecule.BeginAtoms(); 
         it != molecule.EndAtoms(); ++it) {
        OpenBabel::OBAtom* atom = *it;
        int atomicNum = atom->GetAtomicNum();
        double x = atom->GetX();
        double y = atom->GetY();
        double z = atom->GetZ();
    }
}
```

---

## Build System Details

### CMake Configuration

The CMakeLists.txt file includes:

1. **Compiler Flags:**
   - /MP - Multi-processor compilation (Visual Studio)
   - /O2 - Optimize for speed (Release)
   - /GL - Whole program optimization (Release)
   - /LTCG - Link-time code generation (Release)

2. **Precompiled Headers:**
   - Frequently included headers are precompiled
   - Reduces compilation time by 20-40%

3. **Output Directories:**
   - Executables: build/bin/[Config]/
   - Libraries: build/lib/[Config]/

4. **Automatic Asset Copying:**
   - assets/ folder is automatically copied to build directory

### vcpkg Integration

vcpkg.json manifest ensures:
- Consistent dependencies across machines
- Automatic dependency installation
- Version control for libraries

---

## Performance Tips

### Compilation Speed

1. Use Ninja generator instead of MSBuild
2. Enable ccache for faster rebuilds
3. Use precompiled headers (already enabled)
4. Build only one configuration at a time
5. Close unnecessary applications during build

### Runtime Performance

1. Always use Release build for performance testing
2. Enable compiler optimizations (/O2, /GL, /LTCG)
3. Profile code with Visual Studio Profiler
4. Use Eigen for heavy linear algebra (optimized with SIMD)
5. Minimize OpenGL state changes in render loop

---

## Additional Resources

### Learning Resources

- LearnOpenGL: https://learnopengl.com/
- CMake Tutorial: https://cmake.org/cmake/help/latest/guide/tutorial/
- ImGui Wiki: https://github.com/ocornut/imgui/wiki
- Eigen Tutorial: https://eigen.tuxfamily.org/dox/GettingStarted.html
- vcpkg Documentation: https://vcpkg.io/en/docs/

### Reference Documentation

- OpenGL Reference: https://docs.gl/
- GLFW Reference: https://www.glfw.org/docs/latest/
- GLM Reference: https://glm.g-truc.net/0.9.9/api/index.html

---

Last Updated: February 2026