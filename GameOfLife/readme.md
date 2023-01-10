#Description:

This is a 3d cellular automata engine. Cellular automata is a system of cells in a grid where each cell's next state is driven by the cells around it and the rules of that simulation. The most famous example of this is Conway's game of life in 2d.

This engine uses a cubic/square grid, and can simulate both 2d and 3d automata. It allows you to alter which cells are considered neighbours to a current cell. It also allows for changes to the rules that affect when cells die, survive, and reproduce.

You can also move around the simulation and place and remove blocks while the simulation is paused.

#Specifications:

This project currently uses the following frameworks and libraries:
    OpenGL 4.6 - graphics
    GLFW 3.3.8 - window creation
    glad - simplifies opengl usage across platforms
    glm - linear algebra library, provides vectors and matrices
    Dear Imgui - for menus

This project was built in visual studio on Windows 10, so installation and run instructions are geared towards this system.

#Installation and run instructions

To run this project, simply run the precomipled build. Note that the simulation starts paused, so you'll need to press the u key to start it.

To set this project up for development, you must install glfw (this project was built using version 3.3.8). You can either build from source or download the precompiled binaries. 

Once it's downloaded, add glfw to your includes in visual studio.

You also will need to install glad and glm, and add them to you includes files as well.


#Controls:

Movement:
    Mouse - look around
    WASD - move fowards, left, backwards, and right
    Up arrow - move vertically upwards without changing camera view direction
    Down arrow - move vertically downwards without changing camera view direction

Block placement/deletion:
Note - block placement/deletion is only enabled when the simulation is paused.
    Left click - place living cell
    Right click - delete (kill) living cell
    Both of these actions will update the neighbour counts on the next turn.

Simulation controls:
    p - pauses the simulation
    u - unpauses the simulation
    m - opens the menu with option to change rule settings (pauses the simulation)
    c - closes the menu
    esc - closes the simulation


#Menu terminology