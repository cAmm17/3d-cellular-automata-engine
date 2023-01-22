# Description:

This is a [3d cellular automata engine](https://en.wikipedia.org/wiki/Cellular_automaton). Cellular automata is a system of cells in a grid where each cell's next state is driven by the number of cells around it and the rules of that simulation. The most famous example of this is [Conway's game of life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) in 2d.

This engine uses a cubic/square grid, and can simulate both 2d and 3d automata. It allows you to alter which cells are considered neighbours to a current cell. It also allows for changes to the rules that affect when cells die, survive, and reproduce.

You can also move around the simulation and place and remove cells while the simulation is paused.

# Specifications:

This project currently uses the following frameworks and libraries:
* OpenGL 4.6 - graphics
* GLFW 3.3.8 - window creation
* glad - simplifies opengl usage across platforms
* glm - linear algebra library, provides vectors and matrices
* Dear Imgui - for menus

This project was built in visual studio on Windows 10, so installation and run instructions are geared towards this system.

# Additional Credits:

Both the [Shader loading functions](https://learnopengl.com/Getting-started/Shaders) and the [Camera functions](https://learnopengl.com/Getting-started/Camera) were heavily referenced from [Learn Open Gl's tutorials](https://learnopengl.com/) by Joey de Vries. Please check these tutorials out if you are interested in learning more about open gl.

All other code aside from mentioned credits and specifications are mine.

# Installation and run instructions

To run this project, simply download and run the precomipled build under Releases (on the top right of the screen under about). Note that the simulation starts paused, so you'll need to press the u key to start it.

To set this project up for development, you must install glfw (this project was built using version 3.3.8). You can either build from source or download the precompiled binaries. 

Once it's downloaded, add glfw to your includes in visual studio.

You also will need to install glad and glm, and add them to you includes files.


# Controls:

## Movement:
* Mouse - look around
* WASD - move fowards, left, backwards, and right
* Up arrow - move vertically upwards without changing camera view direction
* Down arrow - move vertically downwards without changing camera view direction

## Block placement/deletion:
* Note - block placement/deletion is only enabled when the simulation is paused.
* Left click - place living cell
* Right click - delete (kill) living cell
* Both of these actions will update the neighbour counts on the next turn.

## Simulation controls:
* p - pauses the simulation
* u - unpauses the simulation
* m - opens the menu with option to change rule settings (pauses the simulation)
* c - closes the menu
* esc - closes the simulation


# Menu terminology:
* Neighbour: A usually adjacent cell that is counted in the current cell's rule set. This includes being counted in the cell's alive and dead neighbour counts.
* Survival State: This is the number of living neighbours a cell must have to survive to the next iteration. If the cell does not have a number of neighbours that matches one of the survival state numbers, then it will die or start to die.
* Birth States: This is the number of neighbours a dead cell must have to come to life (ie, the neighbouyr cells reproduce and create a new alive cell). Otherwise, the cell remains dead.
* Decay States: This is the number of states a cell must decay through before it is full dead. However, a cell is only counted as alive in the top decay state.
If a cell dies due to it's neighbour count, it will decend through the decay states by one each iteration until it reaches the last state and dies. This makes it so that the cell space is occupied (so neighbouring cells can't reproduce there) but the cell is not technically alive for a certain number of turns.
To turn off this functionality, simply set the number of decay states to be 1, so that a cell immediately dies when it does not meet the survival condition instead of decaying.
* [Von Neuman Neighbourhood](https://en.wikipedia.org/wiki/Von_Neumann_neighborhood): This is where cells directly adjecent to a face a considered neighbours. Cells diaganolly adjacent are not considered neighbours.
* [Moore Neighbourhood](https://en.wikipedia.org/wiki/Moore_neighborhood): This is where cells that are directly adjecent to a face and cells that are diaganolly adjcent to a face are considered neighbours.

# Screenshots:

### Simulation:
![Screenshot of Simulation](/Images/in_sim_1.png?raw=true "Screenshot of Simulation")

![Screenshot of Simulation 2](/Images/in_sim_2.png?raw=true "Screenshot of Simulation 2")

![Screenshot of Simulation - Random boxes](/Images/in_sim_3.png?raw=true "Screenshot of Simulation - Random Boxes")

### Menu:

![Menu Page 1](/Images/menu_1.png?raw=true "Menu Page 1")

![Menu Page 2](/Images/menu_2.png?raw=true "Menu Page 2")