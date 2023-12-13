# EvolveECS

## Overview
EvolveECS is a 3D engine designed for seamless constrained and unconstrained physics simulations.

## Table of Contents
- [Installation](#installation)
- [Features](#features)
- [Future Development Plans](#future-development-plans)
- [Credits](#credits)

## Installation

### Clone the Repository:
- Clone the EvolveECS repository into your desired folder:
```
     git clone https://github.com/antoniukoff/InfernoEngine-Demo
```

### Prerequisites:
- Ensure you have Visual Studio installed, as the project is configured for this IDE.
- The engine is built for 32-bit (x86) systems.
- All necessary libraries are already pre-built, allowing you to run the project immediately after cloning.

### Opening the Project:
- Open the cloned repository in Visual Studio.
- To see the physics constraints in action, uncomment the constraint functions from the **Update()** method and tweak the parameters in the **Scene0.xml** file.

### Building and Running:
- Build the project in 'Debug' mode for the x86 platform.
- Run the project to explore the features of EvolveECS.

## Features

### Building and Running:
- Build the project in 'Debug' mode for the x86 platform.
- Run the project to explore EvolveECS.

## Features

- **XML Data-Oriented Parsing**
- **3D Frustrum Culling**
- **Collision Debug Meshes**
- **Ray Cast Collision Detection**
- **Physics Constraints**

## Future Development Plans

As EvolveECS continues to grow, we are looking forward to adding:

- **Cross-Platform Build System**
- **Dedicated AI Engine**
- **Spatial Partitioning**
- **Renderer Abstraction**

## Credits

EvolveECS is built with the help of the following open-source libraries:
- **GLEW (OpenGL Extension Wrangler Library)**: [GLEW GitHub](https://github.com/nigels-com/glew)
- **TinyXML**:[TinyXML GitHub](https://github.com/leethomason/tinyxml2)
- **TinyOBJ**:[TinyOBJ GitHub](https://github.com/tinyobjloader/tinyobjloader)
- **SDL (Simple DirectMedia Layer)**: [SDL GitHub](https://github.com/libsdl-org/SDL)
- **Scott Fiedler's Math Library**: [Scott's Math Library](https://github.com/ScottFielder?tab=repositories)
