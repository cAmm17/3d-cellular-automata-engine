#pragma once
#include "main.h"
#include <tuple>
#include <vector>
#include <windows.h>

typedef std::vector< std::tuple<glm::vec3, int> > grid_type;

class gridManager {
private:
	std::vector<std::future<void>> futures;
	int checkBoxNeighbours(glm::vec3 boxPos, int status, int live_lower, int live_upper, int replicate_lower, int replicate_upper);
	void updateCheckEdge(glm::vec3 boxPos);
	void updateGridRow(int row, int live_lower, int live_upper, int replicate_lower, int replicate_upper);
	std::mutex m;

public:
	//Variables to store the grid size and grid itself in a 1D array
	int gridSize;
	int gridCenter;
	grid_type grid, tempgrid;

	//Variables to store the outer limit of the checking functions. This will save time in the 
	//grid update algorithm, as it will not need to check the entire grid.
	float xLimitMin, xLimitMax;
	float yLimitMin, yLimitMax;
	float zLimitMin, zLimitMax;

	//1 is currently updating grid, 0 is not updating
	std::atomic<int> updateState;
	bool update;

	gridManager(int gridsz, std::vector<glm::vec3> initialBoxes);

	//void initializeGrid(int gridsz, std::vector<glm::vec3> initialBoxes);
	void editBox(glm::vec3 boxPos, int remove);
	void updateGrid(int live_lower, int live_upper, int replicate_lower, int replicate_upper);
};
