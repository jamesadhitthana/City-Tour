<div align="center">
<h1 >🚕 City Tour 🚁</h1>
<p ><sup><b>A virtual tour taking you around the city accompanied by immersive atmosphere and sound. </b></sup></p>
</div>

![CityTourGallery](https://raw.githubusercontent.com/jamesadhitthana/City-Tour/main/Documentation/Screenshots/title.png)

# Introduction

City Tour is an interactive app/game that we created using OpenGL, where the audience can participate in a virtual tour of our city. Our virtual city is equipped with many types of buildings and magnificent roads. The city atmosphere can also change over time, which affects the lighting and shading in the town. The tour will take the camera/audience around the city by following a predetermined path from end to end. Viewers can see the city using three types of camera perspectives. First, there is a car perspective camera (camera following the streets), a helicopter perspective camera (camera from above), and a free-mode perspective camera (free camera). During the trip, the audience can look around using the mouse.

![CityTourPoster](https://raw.githubusercontent.com/jamesadhitthana/City-Tour/main/Documentation/Screenshots/citytourposter.png)

## Getting Started

### Running the app

You can run the main.cpp file in the CityTour folder (do not move it out or rename it). The reason is that the required assets and libraries are located in the same folder. However, we recommend that you open and run the CityTour.cbp file using CodeBlocks. The cbp (code blocks point) file contains configurations to the linkers and dependencies. Running the cbp file will ensure the City Tour to work as it has been tested on multiple Windows devices.

### How to Use the App

We provide three-camera perspective options selectable by the user in the welcoming window, namely car mode, helicopter mode, and free mode.

![CityTourSelectMode](https://raw.githubusercontent.com/jamesadhitthana/City-Tour/main/Documentation/Screenshots/citytourselectmode.png)

After the user selects a mode, the city will be displayed and run according to the selected mode. For helicopter mode, the user will be taken to see from an upper perspective, while for car mode, the user will be taken to see from a street perspective. Both of them can only follow the path of the journey that we have designed, and the user can move the mouse to look around while enjoying the view of our city. On the other hand, the user can move the camera anywhere using the keyboard keys (W A S D) for the free mode.

## Screenshots

### Helicopter Mode

![CityTourHelicopter](https://raw.githubusercontent.com/jamesadhitthana/City-Tour/main/Documentation/Screenshots/helimode.png)

### Car Mode

![CityTourCar](https://raw.githubusercontent.com/jamesadhitthana/City-Tour/main/Documentation/Screenshots/carmode.png)

# How it's made

We did this project by forming boxes (cubes), which we placed with a pattern, and then we gave the squares textures to form a building. The shapes of buildings are made by combining several boxes into one. There are four types of structures that we created, and we put the types of buildings randomly.

We use a skybox for the day and night backgrounds, which is a combination of 6 texture images and forms a 3-dimensional cube map. Skybox will change to day or night according to the time we have set. We also added the sun, moon, car, and people using the texture image and removed the fragments of the parts of the texture that we wanted to make transparent. We also use translation and rotation in our texture image to make it move more realistically.

We also utilized the Camera class from learnopengl.com to create a camera perspective of our program.
The animation for the city tour is done by moving the camera location coordinates to make it move like a car or helicopter.We also employed city ambiance sounds and even car and helicopter sounds according to the camera perspective. To run the background music, we use the Windows library and MMSystem and use the PlaySound function, which plays in the background and loops the song until the application is closed.

### Documentation:

- [Documentation (Bahasa Indonesia)](<https://github.com/jamesadhitthana/City-Tour/raw/main/Documentation/City%20Tour%20(Bahasa%20Indonesia)%20-Christopher%20Yefta-James%20Adhitthana.pdf>)

### Notes:

- City Tour was created for a university project and therefore we will not be liable for any loss or damage of any nature.

## Built With

- [C++](https://isocpp.org/) - iostream, vector, Windows.h, MMSystem.h
- [OpenGL](https://www.opengl.org/) - OpenGL
  - [GLFW](https://www.glfw.org/) - GLFW
  - [GLAD](https://github.com/Dav1dde/glad) - GLAD
  - [STB_Image](https://github.com/nothings/stb) - STB_Image
  - [GLM](https://glm.g-truc.net/0.9.9/index.html) - GLM, matrix_transform.hpp, type_ptr.hpp
- [shader.h](https://learnopengl.com/Getting-started/Shaders) - Shader
- [camera.h](https://learnopengl.com/Getting-started/Camera) - Camera
- [model.h](https://learnopengl.com/Model-Loading/Model) - Model

## Authors

- **Christopher Yefta** - [ChrisYef](https://github.com/ChrisYef)

* **James Adhitthana** - [jamesadhitthana](https://github.com/jamesadhitthana)

![CityTourGallery](https://raw.githubusercontent.com/jamesadhitthana/City-Tour/main/Documentation/Screenshots/gallery.png)
