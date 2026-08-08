# PBR Renderer

This is a small pbr renderer/mini engine for my own needs. I noticed that almost
every project I do needs some kind of 2D/3D graphics and I got a little tired of
jumping between graphic libraries all the time. THREE.js, Open Scene Graph or
game engine specific APIs try to achieve the same thing with vastly different
core concepts, so I wanted to create something of my own, that could run both
in the web and on desktop with a unified API. Besides, that's a pretty good
excuse to do some graphics programming in my free time.

This project is heavily inspired by the concepts in [LearnOpenGL](https://learnopengl.com/)
course as well as [Building a Simple Engine](https://docs.vulkan.org/tutorial/latest/Building_a_Simple_Engine/introduction.html).
I also took inspiration from [THREE.js](https://github.com/mrdoob/three.js/) and
[OpenSceneGraph](https://github.com/openscenegraph/OpenSceneGraph), as I like
their way of structuring graphics-related code. I won't be doing Vulkan for now
though, as my goal is to create something usable, rather than learning a new
graphics API and abandoning the project. Additionally, from what I know, the
Vulkan API is not really supported in any of the web environments, so it would
fail to provide the core functionality of this project.

This readme file will get updated as I go.
