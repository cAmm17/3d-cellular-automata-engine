#pragma once
#include "main.h"
#include <tuple>
#include <vector>
#include <windows.h>
#include "Shader.h"

/*
add blocks to survive
remove blocks from survive
add blocks to born
remove blocks from born
add blocks to neigherbours
remove blocks from neighbours
reset num of decay states

*/

class generalLifeLike {
private:
	//unsigned int *neighbourOffsets;
	std::vector<int> survives, born, neighbourOffsets;
	int decayStateNum, neighbourBitShiftNum, compareNum;
	//Variables to store the outer limit of the checking functions. This will save time in the 
	//grid update algorithm, as it will not need to check the entire grid.

	int numOfTotalPossibleNeighbours;
	int numOfNeighbours;
	int bitShiftNum;

	unsigned int gridSize;
	unsigned int fullGridSize;
	int gridCenter;
	unsigned char* grid;
	unsigned char* swapGrid;
	glm::mat4* modelMatrices;
	glm::mat4 gridOutlineModel;
	

	bool gridSetup;
	bool is2d;

	void setupGridOutlineModel();
public:
	//States is the number of states a cell can have. neighbours offsdets controls which blocks nearby will be checked as neighbours. The default would be 

	
	//Variables to store the outer limit of the checking functions. This will save time in the 
	//grid update algorithm, as it will not need to check the entire grid.


	//1 is currently updating grid, 0 is not updating

	//This pauses the updating function. This is public so that the updating function ca
	bool update;
	bool paused;

	//mutexs to control when the grid can be updated
	std::mutex m, m2;


	generalLifeLike(int gridsz, std::vector<glm::vec3> initialBoxes, std::vector<glm::vec3> initialNeighbours, int decayStates, std::vector<int> s, std::vector<int> b, bool set2d);

	int getState(int xin, int yin, int zin) const;
	void updateGrid();
	void render(unsigned int &VAO, Shader &shaderProgram);
	void renderGridOutline(unsigned int &VAO, Shader &shaderProgram);
	void resetAllDecayingCells(int oldCompareNum);
	void setUpGrid(int gridSz);
	void resizeGrid(int newGridSize);
	void createMooreNeighbourhood();
	void createVonNeumanNeighbourhood();
	void generateRandomSeed(int numBlocksToGenerate);
	void resetGrid();
	void resetGrid(int newGridSize, bool twoD);

	//----------------------------------------------Getter functions-------------------------------------------------------------------------------------------------------------------
	bool inGrid(int x, int y, int z);
	int getGridSize1d();
	int getFullGridSize();
	std::vector<int> getBorn();
	std::vector<int> getSurvives();
	std::vector<int> getNeighbours();
	int getNumOfDecayStates();
	bool getIs2d();


	//----------------------------------------------Setter functions-------------------------------------------------------------------------------------------------------------------
	void setBorn(std::vector<int> b);
	void setSurvives(std::vector<int> s);
	void setNeighbours(std::vector<glm::vec3> neighs);
	void setDecayStates(int decayStates);

	//----------------------------------------------Mutator functions------------------------------------------------------------------------------------------------------------------
	void addBlocksToBorn(int numToAdd, int optionalEndRange = -1);
	void removeBlocksFromBorn(int numtoAdd);
	void addBlocksToSurvives(int numToAdd, int optionalEndRange = -1);
	void removeBlocksFromSurvives(int numToAdd);
	void addToNeighbours(int x, int y, int z = 0);
	void removeFromNeighbours(int x, int y, int z = 0);
	void removeAllNeighbours();

	void addNewCell(int xin, int yin, int zin, bool playerEdit);
	void decayCell(int xin, int yin, int zin, bool playerEdit);
	void removeCell(int xin, int yin, int zin, bool playerEdit);
};

glm::mat4* calculateOffsets(int gridSize, bool is2d);