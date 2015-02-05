 *Author: L.C. Chu*
 *Date: 2014/12/11, UW-Madison*

####   (0) The story
  Several years ago, I had a change to go a little island in pacific ocean. That's a unforgettable beautiful experience, So I decide to make this as my final project :)

####  (1) Navigation, Program and Code

1. Use the mouse to rotate the scene and move forward/backward
  * Use the keyboard 'j' to lift up, 'k' to move down
2. The excutable program "cs559pj2.exe" is in the "code_and_exe" folder. Click button 'Run' to excute the program.
3. The visual studio project is in the "code_and_exe" folder.
  * Note: most of the shader code is inside the cpp file, not in the "shader/basic" folder.

####  (2) Features
1. The island:
  * I crop the google map image from the Bing Map, and convert it into the height map and texture map.
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
7. Framebuffer:
  * The multi-pass rendering is not easy to debug, so I make a debuging tool which is also a framebuffer.
  * This can draw other's object drawcall into the framebffer and present it in
  * the sub-region of the window for debugging.
  * This supports three types: Color buffer, Depth buffer and Stencil Buffer.
8. Framework:
  * I decide to write a framework using the modern OpenGL language, mostly are
  * OpenGLES 3.0. This framework can be easily porting to mobile platform, and other embedded system.

#### Rrefence:
  1. https://www.khronos.org/opengles/sdk/docs/man3/
  2. http://www.3dcpptutorials.sk/index.php?id=43
  3. http://www.gametutorials.com/tutorial/height-map-part6/
  4. https://ericpacelli.wordpress.com/2014/04/21/realistic-water-in-opengl-and-glsl/
  5. http://www.twodee.org/weblog/?p=666

