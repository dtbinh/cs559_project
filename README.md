###### Author: L.C. Chu
###### Date: 2014/12/11, UW-Madison

![beauty shot](https://github.com/lcchu/cs559_project/blob/master/bueaty_shot.png)
<div style="text-align:center">video link: http://youtu.be/aYlcorjD2z8a</div> 

####   (0) The story
  Several years ago, I went to a little island in pacific ocean (GPS: 22.0440943,121.5503227). That was an unforgettable experience. For this project, I try to render this island as real as possible :)

####  (1) Navigation, Program and Code

1. Use the mouse to rotate the scene and move forward/backward
  * Use the keyboard 'j' to lift up, 'k' to move down
2. The excutable program "cs559pj2.exe" is in the "code_and_exe" folder. Click button 'Run' to excute the program.
3. The visual studio project is in the "code_and_exe" folder.
  * Note: most of the shader code is inside the cpp file, not in the "shader/basic" folder.

####  (2) Features
1. The island:
  * I crop the map image from the Bing Map and Google Map, and manually convert it into the height map and texture map.
2. The trees:
  * There are roguhly 1.5 thousand trees on the island.  Each tree is a billboard which always faces the camera. And There are 5 different of trees picked randomly as texture.
  * Two technical points here:
    * How to draw so many trees at once
      - Ans: Put all the tree vertex and postions in vertex buffer, and use put one random value as flag to choose differenent kind of tree texture. So draw all the trees in just one draw call !
    * How to deal with alpha blending when look at differenet angle (e.g. back)
      - Ans: Sort their z-distance and update the vertex buffer object at each frame.
3. The ocean:
  * It's a multi-pass rendering using framebuffer object. We need the normal map and Du-Dv map to create a dynamic waving effect, and render the reflection.
    * 1st pass)
      - Render the refection of the island and the sky to the framebuffer, and use the buffer as texture. And use the slide projeter technique to project the reflection texture.
    * 2nd pass)
      - Combine the 4 textures: normal map, Du-Dv map, reflection txture and the fraction texture (basic deep blue color of the ocean). Also we need to calculate the fresnel factor to combine the reflection and fraction and use dudv to distorn the wave.
4. The cloud:
  * This is basiclly a moving billboard. The technique is like the trees. There are totoally 3 different kind of clouds randomly picked, and determined in the fragment shader.
5. The sky:
  * This is a skybox, which loads 6 different textures, and the mesh is a sphere.
6. Bird:
  * The bird flies in the sky around the island. And it's wing waves periodically.
7. The debugging tool (using Framebuffer):
  * The multi-pass rendering is not easy to debug, so I make a debuging feature using framebuffer.
  * The debugging tool draw object's drawcall in the framebffer and show it in the sub-region of the window.
  * This supports three types of buffer: Color buffer, Depth buffer and Stencil Buffer.
8. Framework:
  * The project is implemented by C++ and modern OpenGL language, i.e. OpenGLES 3.0. 
  * The project can be easily porting to other platform such as mobile device or embedded system.

#### Rrefence:
  1. https://www.khronos.org/opengles/sdk/docs/man3/
  2. http://www.3dcpptutorials.sk/index.php?id=43
  3. http://www.gametutorials.com/tutorial/height-map-part6/
  4. https://ericpacelli.wordpress.com/2014/04/21/realistic-water-in-opengl-and-glsl/
  5. http://www.twodee.org/weblog/?p=666

