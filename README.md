# gravity
dependencies:
* [SFML](https://github.com/SFML/SFML) (only for window and gl context creation)
* [GLM](https://github.com/g-truc/glm)
* [GLEW](https://github.com/nigels-com/glew)
* OpenGL 3.3+ version

controls: :relaxed:
* create gravity center - left mouse button
* move around - right mouse button
* zoom in/out - scroll wheel

You can pass number of particles as argument when invoking program. :fire:

For Linux users there is a Makefile which should work out of the box  
if dependencies are installed via package manager (apt, pacman).
### demo
[Apitrace](https://github.com/apitrace/apitrace) was used to generate images but it extremely slows down program,  
so animation feels laggy. On Linux with intel graphics I can run 1'000'000 particles with ~75 fps and it's  
much more visually appealing.

![](https://github.com/matimaci/gravity/blob/master/demo.gif)
