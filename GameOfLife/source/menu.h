#pragma once
#include "imgui.h"
#include "main.h"
#include "imgui_impl_glfw.h"
#include "generalLifeLike.h"
#include "imgui_impl_opengl3.h"
#include <string> 



class menu {
private:
	generalLifeLike* grid;
	int newSurvStates = 0;
	int newBirthStates = 0;
	int endRange = 0;
	int endRange2 = 0;
	int addNeighbourX = 0;
	int addNeighbourY = 0;
	int addNeighbourZ = 0;
	int removeNeighbourX = 0;
	int removeNeighbourY = 0;
	int removeNeighbourZ = 0;
	int rmvBirthStates = 0;
	int rmvSurvStates = 0;
	int newGridSize = 0;
	int newDecayStates = 0;
	int numCellsToAdd = 0;
	bool newGrid2d = false;


public:
	bool showMenu = false;
	menu() { };
	void setupGui(GLFWwindow* window, const char* glsl_version, generalLifeLike* inGrid);
	void renderGui();
	void cleanupGui();
};