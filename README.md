# MAZE-engine
A simple 3D game engine, written in C++17.

**features**
* scene explorer camera and third person camera
* blinn phong and PBR materials
* image based lighting
* point, spot and directional light sources (max 10 light sources per scene)
* skybox
* shadow mapping
* screen space ambient occlusion
* bloom effect
* vignette effect
* anti-aliasing
* 3D animated model loading with the ASSIMP library
* physics with the BULLET engine : rigid bodies, soft bodies and kinematic character controller
* fire particle system
* audio management with the OPENAL library

**CMAKE**
The following variables must be defined :

${SDL2\_INCLUDE\_DIR}
${GLEW\_INCLUDE\_DIR}
${ASSIMP\_INCLUDE\_DIR}
${BULLET\_INCLUDE\_DIR}
${OPENAL\_INCLUDE\_DIR}
${LIBSNDFILE\_INCLUDE\_DIR}

${SDL2\_LIB\_DIR}
${GLEW\_LIB\_DIR}
${ASSIMP\_LIB\_DIR}
${BULLET\_LIB\_DIR}
${OPENAL\_LIB\_DIR}
${LIBSNDFILE\_LIB\_DIR}

${SDL2\_LIBS}
${GLEW\_LIBS}
${ASSIMP\_LIBS}
${BULLET\_LIBS}
${OPENAL\_LIBS}
${LIBSNDFILE\_LIBS}

On Windows, a good way to acquire those libraries is to use vcpkg with git bash,
and store them in a short path location like "C:\dev".

## TO DO
**graphics**
* Fix brightness threshold for non emissive surfaces

**user interface**
* OpenGL based user interface (buttons, menus, layout)

## DEMO
### regular camera
![Maze-Engine](maze_engine.gif)
### third person camera
![Maze-Engine (third person camera)](third_person_camera.gif)
