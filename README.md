# Atometa

> **A → meta** · From the atom to what lies beyond.  
3D Chemistry Simulation Engine for Research and Education

---

## Overview

Atometa is an advanced molecular visualization and chemistry simulation engine developed as part of a PhD research project. Built with modern C++ and OpenGL, it provides real-time molecular dynamics simulation capabilities.

## Project Vision

- Academic Research: PhD thesis on molecular simulation and visualization
- Education: Interactive chemistry teaching tool
- Medical Applications: Drug discovery and molecular interaction analysis
- Commercial Platform: Scalable solution for professional and educational use

## Technology Stack

- Language: C++17
- Graphics API: OpenGL 3.3+
- Build System: CMake 3.20+
- Package Manager: vcpkg
- Window/Input: GLFW 3.3+
- UI Framework: ImGui (planned)
- Math Libraries: GLM, Eigen

## Required Libraries

### Core Dependencies

- GLFW 3.3+ - Window and input management
- GLAD - OpenGL function loader
- GLM 0.9.9+ - Mathematics (vectors, matrices, transformations)
- ImGui 1.89+ - Immediate mode GUI framework
- Eigen 3.4+ - Linear algebra for scientific computing
- stb_image - Image loading utilities

### Chemistry Libraries (Future)

- Open Babel 3.1+ - Molecular file I/O (PDB, MOL, XYZ formats)
- RDKit - Cheminformatics toolkit for drug discovery

## Quick Start

### Prerequisites

1. Visual Studio 2019 or later with C++ Desktop Development
2. CMake 3.20+
3. vcpkg package manager
4. Git

### Installation Steps

#### 1. Install vcpkg

```cmd
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
```

#### 2. Install Dependencies

```cmd
vcpkg install glfw3:x64-windows
vcpkg install glad:x64-windows
vcpkg install glm:x64-windows
vcpkg install imgui[glfw-binding,opengl3-binding]:x64-windows
vcpkg install eigen3:x64-windows
vcpkg install stb:x64-windows
```

Installation time: 5-15 minutes
Disk space required: Approximately 500 MB

#### 3. Configure CMake

```cmd
cd C:\atometa
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

#### 4. Build Project

```cmd
cmake --build build --config Release
```

#### 5. Open in Visual Studio

```cmd
start build\Atometa.sln
```

Set configuration to Release or Debug and press F5 to run.

## Build Optimization

### Method 1: Use Ninja Generator (Recommended)

Ninja is 30-50% faster than MSBuild.

```cmd
vcpkg install ninja:x64-windows
cmake -G Ninja -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

### Method 2: Enable ccache

ccache significantly speeds up rebuilds by caching compilation results.

```cmd
vcpkg install ccache:x64-windows
```

CMake will automatically detect and use ccache if available.

### Method 3: Precompiled Headers

Precompiled headers are enabled by default in CMakeLists.txt. This reduces compilation time for frequently included headers like STL containers and GLM.

### Method 4: Multi-processor Compilation

Automatically enabled for Visual Studio builds with the /MP flag. Uses all available CPU cores during compilation.

## Project Structure

```
Atometa/
├── src/                    Source code
│   ├── core/              Core engine (Application, Window, Input, Logger)
│   ├── renderer/          OpenGL rendering system
│   ├── chemistry/         Chemistry simulation (Atom, Molecule, Bonds)
│   └── ui/               ImGui user interface
│
├── include/Atometa/       Public headers
│   ├── Core/
│   ├── Renderer/
│   ├── Chemistry/
│   └── UI/
│
├── assets/                Resources
│   ├── shaders/          GLSL shader files
│   ├── models/           3D molecular models
│   ├── textures/         Texture files
│   └── branding/         Logo and branding assets
│
├── docs/                  Documentation
│   ├── api/              API reference
│   ├── research/         Research notes and papers
│   └── tutorials/        User guides
│
├── research/              PhD research materials
│   ├── papers/           Reference papers and literature
│   ├── notes/            Research notes and observations
│   └── experiments/      Experimental data and results
│
├── tests/                Unit tests
│
├── CMakeLists.txt        Build configuration
├── vcpkg.json            Dependency manifest
├── .gitignore            Git ignore rules
└── LICENSE               MIT License
```

## Development Roadmap

### Phase 1: Foundation (Months 1-3)
- [x] Project setup and build system
- [x] Core architecture (Application, Window, Input, Logger)
- [ ] Basic OpenGL rendering pipeline
- [ ] Camera system with orbital controls
- [ ] ImGui integration

### Phase 2: Chemistry Core (Months 3-6)
- [ ] Atom and molecule data structures
- [ ] Periodic table database
- [ ] Bond visualization (single, double, triple bonds)
- [ ] Molecular file I/O (XYZ, PDB formats)
- [ ] Basic molecular editor

### Phase 3: Simulation (Months 6-12)
- [ ] Force field implementation (AMBER, UFF)
- [ ] Molecular dynamics engine
- [ ] Energy minimization algorithms
- [ ] Collision detection and response

### Phase 4: Advanced Features (Months 12-18)
- [ ] Advanced rendering (surface, volumetric)
- [ ] Quantum chemistry integration
- [ ] Python scripting API
- [ ] Performance optimization and GPU acceleration

### Phase 5: Research and Publication (Months 18-24)
- [ ] Thesis writing and documentation
- [ ] Performance benchmarking
- [ ] User studies and validation
- [ ] Academic publications

## Current Status

| Component | Status | Progress |
|-----------|--------|----------|
| Build System | Complete | 100% |
| Core Engine | In Progress | 25% |
| Renderer | Planned | 0% |
| Chemistry Core | Planned | 0% |
| UI System | Planned | 0% |
| Simulation | Planned | 0% |

## Academic Context

This project is developed as part of a PhD program focusing on:

- Computational chemistry and molecular dynamics
- Real-time 3D visualization of chemical systems
- Interactive educational tools for chemistry education
- Scalable architecture for scientific computing

### Research Applications

1. Molecular Dynamics - Simulating atomic and molecular interactions
2. Drug Discovery - Visualizing protein-ligand binding mechanisms
3. Chemical Education - Interactive teaching and learning platform
4. Materials Science - Studying molecular structures and properties

## System Requirements

### Minimum Requirements

- Operating System: Windows 10/11 (64-bit)
- CPU: Intel i5-4570 or equivalent (4 cores)
- RAM: 8 GB
- GPU: Intel HD Graphics 4600 or better (OpenGL 3.3+ support)
- Disk Space: 2 GB free space

### Recommended Requirements

- CPU: Intel i7 or AMD Ryzen 7
- RAM: 16 GB
- GPU: Dedicated GPU with 2GB+ VRAM (NVIDIA GTX 1060 / AMD RX 580 or better)
- Disk Space: 5 GB free space

## Contributing

While this is primarily a PhD research project, contributions are welcome.

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Open a Pull Request

## Issues & Feedback

Found a bug or have an idea for a new feature?

→ Open an issue here: [https://github.com/venits/atometa/issues/new](https://github.com/venits/atometa/issues/new)

Please use the provided templates:
- Bug Report
- Feature Request

Search existing issues first to avoid duplicates.

## License

This project is licensed under the MIT License. See LICENSE file for details.

This allows commercial use, modification, distribution, and private use.

## Author

PhD Candidate
Department of Chemistry

## Acknowledgments

- Academic supervisors and research committee
- Open-source community (GLFW, ImGui, Dear ImGui, GLM, Eigen)
- Chemistry and computational science communities
- vcpkg maintainers

---

Last Updated: February 2026
