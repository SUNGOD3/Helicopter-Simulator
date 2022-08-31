# Helicopter Simulator
 A helicopter model made with basic techniques of computer graphics.

## Implemented features:

### The helicopter could be able to:
* Move vertically. (up and down)
* Fly forward and backward.
* Make a turn.
* Rotate about the vertical axis.（Assume it is the y-axis.）
* Take-off and landing 

### Basic Scene:
* Some tall building (colorful cubes, cylinders, spheres) and a wide ground in the scene
<img src="README_IMG/Scene.png" width="50%"/>

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
* ']' Displays the viewing angle range.
### Light and shadow switching: 
* '!' Switch light mode (sunlight/single-point movable light source (small golden ball)/helicopter flashlight)
* '@' Turn on/off all light sources, '#' switch the light color (R/G/B/normal light), 'space' move the small golden ball (circle)
* '$' Move the torch (down/a bit forward/forward), '[' show torch range, '+'/'-' increase/decrease light intensity
* '*'/'‘'/'’' increase/decrease specular index, 'c' switch emission light color (R/G/B/normal light), 'x' switch shadow mode (Smooth/Flat)
### Material/fog switch: 
* 'f' switch fog mode (0/1/2/3 no fog/LINER/EXP/EXP2), 'C' material screenshot.

## Draft of the helicopter:

<img src="README_IMG/draft.png" width="50%"/>
Design according to the concept of local coordinate system (LCS)
