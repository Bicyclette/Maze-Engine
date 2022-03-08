# MAZE-engine
A simple, rudimentary and low level 3D game engine, written in C++17.

**Features**
* scene explorer camera and third person camera
* blinn phong and PBR materials (PBR is only supported with glTF/glb format at the moment)
* image based lighting
* point, spot and directional light sources (max 10 light sources per scene)
* skybox
* shadow mapping
* screen space ambient occlusion
* bloom (xml data)
* lightning
* Ordered transparency (xml data)
* vignette
* anti-aliasing
* 3D animated model loading with the ASSIMP library
* physics with the BULLET engine : rigid bodies, soft bodies, kinematic character controller and vehicle controller
* debug draw for the BULLET engine
* fire particle system
* audio management with the OPENAL library
* DDS textures for fast scene loading and efficient VRAM usage (tested with fbx files : approximately 7 to 8 times faster than glb files which have png/jpg textures)
* custom collision shapes for rigid bodies

**CMAKE build system**

The following variables must be defined :

* ${SDL2\_INCLUDE\_DIR}
* ${SDL2\_LIB\_DIR}
* ${SDL2\_LIBS}
* ${GLEW\_INCLUDE\_DIR}
* ${GLEW\_LIB\_DIR}
* ${GLEW\_LIBS}
* ${ASSIMP\_INCLUDE\_DIR}
* ${ASSIMP\_LIB\_DIR}
* ${ASSIMP\_LIBS}
* ${BULLET\_INCLUDE\_DIR}
* ${BULLET\_LIB\_DIR}
* ${BULLET\_LIBS}
* ${OPENAL\_INCLUDE\_DIR}
* ${OPENAL\_LIB\_DIR}
* ${OPENAL\_LIBS}
* ${LIBSNDFILE\_INCLUDE\_DIR}
* ${LIBSNDFILE\_LIB\_DIR}
* ${LIBSNDFILE\_LIBS}

On Windows, a good way to acquire those libraries is to use vcpkg with git bash,\
and store them in a short path location like "C:\dev".


About Bullet, the following linking order must be respected when setting the value\
for ${BULLET\_LIBS} : BulletSoftBody, BulletDynamics, BulletCollision, LinearMath.

## DEMO
### Character controller
![explorer camera](engine_imgs/character_controller.gif)
### Third person camera
![third person camera](engine_imgs/third_person.gif)
### Bullet physics debug draw
![physics debug draw](engine_imgs/btIDebugDraw.gif)
### Vehicle controller
![vehicle](engine_imgs/btRaycastVehicle.gif)
![vehicle night](engine_imgs/car.gif)
### Bloom
![car](engine_imgs/car_bloom.png)
![bloom green](engine_imgs/green_lightsaber.png)
![bloom yellow](engine_imgs/yellow_lightsaber.png)
![bloom purple](engine_imgs/purple_lightsaber.png)
![bloom purple](engine_imgs/red_lightsaber.png)
### Lightning
![podracer](engine_imgs/podracer.png)
![podracer lightning anim](engine_imgs/podracer_lightning.gif)
### 3D audio
![audio](engine_imgs/audio_debug.png)
![audio](engine_imgs/3D_audio_close_up.png)

## TO DO
**AI**
* Pathfinding
* Navmesh

**Physics**
* Podracer vehicle controller

**Graphics**
* Volumetric lighting
* Motion blur

**UI**
* OpenGL based user interface (buttons, menus, layout).
* Text rendering

**Others**
* Write documentation/how-to

## BUGS
* Wheels of vehicle controller rotate in wrong direction
