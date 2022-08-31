# Helicopter Simulator
 A helicopter model made with basic techniques of computer graphics.

## Implemented features:

### The helicopter could be able to:
* Move vertically (up and down)
* Fly forward and backward
* Make a turn
* Rotate about the vertical axis（Assume it is the y-axis）
* Take-off and landing 
* Control the movements by using keyboard events

### Basic Scene:
* Some tall building (colorful cubes, cylinders, spheres) and a wide ground in the scene

  <img src="README_IMG/Scene.png" width="50%">
### Viewing & Projection:
* Allow users to translate and rotate the camera position, focus, and view up vector (surge, heave, sway, roll, yaw, and pitchmotions)
* 5 projection methods:

   * Method 1: Orthographical projection along x-axis
   * Method 2: Orthographical projection along y-axis
   * Method 3: Orthographical projection along z-axis
   * Method 4: Perspective projection
   * Method 5: Divide the window into 4 view-ports, Method 1-3 and Method 4 (As shown below)

 <img src="README_IMG/Method5.png" width="50%">
(In the middle of each scene is the origin and 3 axes of world coordinate system (WCS))

### Lighting & Shading:
* Contain at least 3 light sources:

   * A directional light from the sun or other sources
   * A point light located at a fixed position in the scene (higher than any obstacle)
   * A spot light attached to your helicopter (also shows the range of light)
* Assign different material properties to object surfaces so that their appearances look like metallic, plastic, and diffuse surfaces
* Shade the scene by using the smooth shading method
* Being able to change direction/color/intensity/enable of these light sources

 <img src="README_IMG/Emission.png" width="25%"><img src="README_IMG/Light.png" width="25%"><img src="README_IMG/PointLight.png" width="25%">
 
 <img src="README_IMG/SpotLight.png" width="25%"><img src="README_IMG/LightIntensity.png" width="25%"><img src="README_IMG/RangeOfLight.png" width="25%">

### TextureMapping:
* Contain at least 3 types of texture maps
* Use billboards to draw complex objects (No matter how you move the camera, the texture always faces the player)
  <img src="README_IMG/Billboard.png" width="100%">
* A sky dome (sky.bmp)
* Foggy effect + being able to change the fog color, mode, and opacity
* Screenshot function (The image captured by the player will be pasted as a texture map on the middle column)
  <img src="README_IMG/Screenshot.png" width="100%">

## How to execute "Helicopter.cpp"?

* You must import the following 4 header files:

  "iostream"、"stdio.h"、"math.h"、"GL/glut.h" (or "glut.h")

* And you have to make your project readable to "sky.bmp" 

  If this step fails, the following screen will appear: 
  
  ![GITHUB]( https://github.com/SUNGOD3/Helicopter-Simulator/blob/main/README_IMG/Read%20Failed.png )
  
## How to play?:

### Control instructions: 
* 'w'/'a'/'s'/'d' go forward/backward/left/right 
* 'q' leave, 'p' pause 
* 'j'/'k' go up/down
* 'i'/'u' turn right/left
* 'o'/'l' left/right to fly
### Viewing Angle Switching: 
* '0'/'1'/'2'/'3'/'4' Display All (the other four) / Ortho X-axis / Ortho Y-axis / Ortho Z-axis / Perspective projection
* 'U'/'D' lens up/down, 'L'/'R' view left/right, 'I'/'O' view forward/back, 'P'/'H'/'r' pitching/heading/rolling
* ']' Displays the viewing angle range
### Light and shadow switching: 
* '!' Switch light mode (sunlight/single-point movable light source (small golden ball)/helicopter flashlight)
* '@' Turn on/off all light sources, '#' switch the light color (R/G/B/normal light), 'space' move the small golden ball (circle)
* '$' Move the torch (down/a bit forward/forward), '[' show torch range, '+'/'-' increase/decrease light intensity
* '*'/'‘'/'’' increase/decrease specular index, 'c' switch emission light color (R/G/B/normal light), 'x' switch shadow mode (Smooth/Flat)
### Material/fog switch: 
* 'f' switch fog mode (0/1/2/3 no fog/LINER/EXP/EXP2), 'C' material screenshot

## Draft of the helicopter:

<img src="README_IMG/draft.png" width="50%">
Design according to the concept of local coordinate system (LCS)
