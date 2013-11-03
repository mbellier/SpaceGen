SpaceGen
========

Description
-------------

SpaceGen is a project in development experimenting with procedural generated space environments.
The code is in C/C++ and the graphics are rendered using Qt/OpenGL.

Modules
-------------
### Camera
A camera has been implemented using quaternions to provide an "FPS" experience to the user.

### Morton Code
The Morton code is used to generate seeds for a PRNG (Pseudo Random Number Generator). 
A specific seed can be determined for each spatial coordinates using a 3D Morton code.

### Icosphere
Spheres are generated using an icosphere (instead of a classic UV sphere) for improved performances.

### StarField
This module will render a procedural generated star field. It is planned to be used with a chunk system.

Screenshots
-------------

### Icosphere




![Icosphere image](/snapshots/icosphere.png "Icosphere")
