# EvolveECS

<p align="left">
  <img src="/EvolveECS/branding/EvolveECS.gif" alt="InfernoEngine Logo" width="50%"/>
</p>


## Overview
EvolveECS is a 3D engine designed for seamless constrained and unconstrained physics simulations.

## Installation
1. **Clone the Repository**: 
   - Clone the EvolveECS repository into your desired folder:
     ```
     git clone https://github.com/antoniukoff/ECS-Framework
     ```

2. **Prerequisites**:
   - Ensure you have Visual Studio installed, as the project is configured for this IDE.
   - The engine is built for 32-bit (x86) systems.
   - All necessary libraries are pre-built for ease of setup.

3. **Opening the Project**:
   - Open the cloned repository in Visual Studio.
   - Choose any available demo projects as the startup project.
   - To see the physics constraints in action, uncomment the constraint functions from the **Update()** method and tweak the parameters in the **Scene0.xml** file.

4. **Building and Running**:
   - Build the project in 'Debug' mode for the x86 platform.
   - Run the project to explore the features of EvolveECS.

## Features
- **Physics Constraints**:
  - Straight Line Constraint
  - Plain Constraint
  - Mouse Constraint
- **3D Frustrum Culling**
- **Collision Debug Meshes**
- **Ray Cast Collision Detection**
- **Entity-Component Architecture**
- **XML Data-Oriented Parsing**



## Future Development Plans
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
