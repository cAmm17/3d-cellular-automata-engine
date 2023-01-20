#include "generalLifeLike.h"

//defining the type for the grid. It is a dynamic array of tuples. Each tuple contains the location
//of the box (generated when the grid is first created) and an int (0 or 1) determining if a box should
//be drawn at that position or not

generalLifeLike::generalLifeLike(int gridsz, std::vector<glm::vec3> initialBoxes, std::vector<glm::vec3> initialNeighbours, int decayStates, std::vector<int> s, std::vector<int> b, bool set2d) {
	/* Intiator function for the grid. Sets up the grid with the given rules and given initial boxes*/

	survives = s;
	born = b;
	numOfNeighbours = 0;
	is2d = set2d;
	update = true;
	paused = true;
	gridSetup = false;

	setDecayStates(decayStates);

	if (is2d) {
		//if the program is set to 2d then it only needs to save the x and y coordinates of neighbours blocks
		fullGridSize = gridsz * gridsz;
	}
	else {
		fullGridSize = gridsz * gridsz * gridsz;
	}

	//creating the char arrays for the grids themselves
	grid = new unsigned char[fullGridSize];
	swapGrid = new unsigned char[fullGridSize];

	std::cout << initialNeighbours.size() << "\n";

	//create the vector to put the neighbours that will be checked for each cell in. Due to the way cells are stored, the program checks if there are too many neighbours in the initial neighbours vector and restricts the number
	
	
	//adds the user inputed neighbours to the neighbour offset list

	setNeighbours(initialNeighbours);

	gridSize = gridsz;
	memset(grid, 0, fullGridSize);
	gridCenter = int(gridSize / 2);

	for (glm::vec3 &box : initialBoxes) {
		if (!is2d) addNewCell(int(box.x), int(box.y), int(box.z), false);
		else addNewCell(int(box.x), int(box.y), 0, false);
	}

	modelMatrices = new glm::mat4[fullGridSize];
	modelMatrices = calculateOffsets(gridSize, is2d);

	//copys data from the temp grid to the new grid
	memcpy(swapGrid, grid, fullGridSize);

	setupGridOutlineModel();
	gridSetup = true;
	
}


void generalLifeLike::setupGridOutlineModel() {
	gridOutlineModel = glm::mat4(1.0f);
	gridOutlineModel = glm::translate(gridOutlineModel, glm::vec3(gridSize / 2, gridSize / 2, gridSize / 2));
	gridOutlineModel = glm::scale(gridOutlineModel, glm::vec3(gridSize, gridSize, gridSize));
}

int generalLifeLike::getState(int xin, int yin, int zin) const {
	unsigned char *gridBoxPtr;
	if(is2d) gridBoxPtr = grid + xin + yin * gridSize;
	else gridBoxPtr = grid + xin + yin * gridSize + zin * gridSize * gridSize;

	//states are stored bitwise
	return *gridBoxPtr & compareNum;
}

void generalLifeLike::addNewCell(int xin, int yin, int zin, bool playerEdit) {
	unsigned char* gridToEdit;
	//The swap grid is the grid that is usually rendered, so this grid is directly edited by the player in order for their edits to be visible
	if (playerEdit) {
		gridToEdit = swapGrid;
	}
	else {
		gridToEdit = grid;
	}

	unsigned char *gridBoxPtr = gridToEdit + xin + yin * gridSize + zin * gridSize * gridSize;

	if (is2d) zin = 0;

	//First check the point is in the grid and that the cell there is currently dead and not in another state
	if (xin < gridSize && yin < gridSize && zin < gridSize && (*gridBoxPtr & compareNum) == 0) {
		//set the point to be the top decay state
		*(gridBoxPtr) |= decayStateNum;

		int x, y, z;

		// unsigned int neighb = *gridBoxPtr >> bitShiftNum;
		//update the neighbours to indicate there's a new block there
		for (int ni = 0; ni < numOfNeighbours * 3; ni += 3) {
			x = neighbourOffsets[ni];
			y = neighbourOffsets[ni + 1];
			z = neighbourOffsets[ni + 2];

			if (xin + x >= 0 && xin + x < gridSize &&
				yin + y >= 0 && yin + y < gridSize &&
				zin + z >= 0 && zin + z < gridSize &&
				!(x == 0 && y == 0 && z == 0)) {
				gridBoxPtr = gridToEdit + xin + x + (yin + y) * gridSize + (zin + z) * gridSize * gridSize;
				*(gridBoxPtr) += neighbourBitShiftNum * 2;
			}
		}
		//Copy the changes over from the swap grid, which is usually overwritten during the update process, to the regular grid.
		if (playerEdit) memcpy(grid, swapGrid, fullGridSize);
	}

}

void generalLifeLike::decayCell(int xin, int yin, int zin, bool playerEdit) {
	/* This function decays a cell by one state, and kills the cell if there is only one decay state. */
	unsigned char* gridToEdit;
	if (playerEdit) {
		gridToEdit = swapGrid;
	}
	else {
		gridToEdit = grid;
	}

	unsigned char *gridBoxPtr = gridToEdit + xin + yin * gridSize + zin * gridSize * gridSize;

	if (is2d) zin = 0;
	//check the point is in the grid and not already dead
	if (xin < gridSize && yin < gridSize && zin < gridSize && (*gridBoxPtr & compareNum) > 0) {
		//decay down one state
		*(gridBoxPtr) -= 1;

		//if the point is no longer in the top decay state, then don't count it as a neighbour
			if ((*gridBoxPtr & compareNum) + 1 == decayStateNum) {
				int x, y, z;
					for (int ni = 0; ni < numOfNeighbours * 3; ni += 3) {
						x = neighbourOffsets[ni];
						y = neighbourOffsets[ni + 1];
						z = neighbourOffsets[ni + 2];

						if (xin + x >= 0 && xin + x < gridSize &&
							yin + y >= 0 && yin + y < gridSize &&
							zin + z >= 0 && zin + z < gridSize &&
							!(x == 0 && y == 0 && z == 0)) {
							gridBoxPtr = gridToEdit + xin + x + (yin + y) * gridSize + (zin + z) * gridSize * gridSize;
							*(gridBoxPtr) -= neighbourBitShiftNum * 2;
						}
					}

			}
			//since player edits are applied to the active visible grip copy the changes over 
			if (playerEdit) memcpy(grid, swapGrid, fullGridSize);
	}
}

void generalLifeLike::removeCell(int xin, int yin, int zin, bool playerEdit) {
	/*This function kills the cell no matter how many decay states there are */

	unsigned char* gridToEdit;
	if (playerEdit) {
		gridToEdit = swapGrid;
	}
	else {
		gridToEdit = grid;
	}

	unsigned char *gridBoxPtr = gridToEdit + xin + yin * gridSize + zin * gridSize * gridSize;

	if (is2d) zin = 0;

	if (xin < gridSize && yin < gridSize && zin < gridSize && (*gridBoxPtr & compareNum) != 0) {
		int x, y, z;

		// decrease the neighbour number only if the count hasn't been decreased previously in decayState
		// if there is more than one decay state, then the number will have already been decreased except on a player edit
		if ( decayStateNum == 1 || (playerEdit && (*gridBoxPtr & compareNum) == decayStateNum)) {
			for (int ni = 0; ni < numOfNeighbours * 3; ni += 3) {
				x = neighbourOffsets[ni];
				y = neighbourOffsets[ni + 1];
				z = neighbourOffsets[ni + 2];
				if (xin + x >= 0 && xin + x < gridSize &&
					yin + y >= 0 && yin + y < gridSize &&
					zin + z >= 0 && zin + z < gridSize &&
					!(x == 0 && y == 0 && z == 0)) {
					gridBoxPtr = gridToEdit + xin + x + (yin + y) * gridSize + (zin + z) * gridSize * gridSize;
					*(gridBoxPtr) -= neighbourBitShiftNum * 2;
				}
			}

		}
		// get the point and kill the cell
		gridBoxPtr = gridToEdit + xin + yin * gridSize + zin * gridSize * gridSize;
		*(gridBoxPtr) &= ~compareNum;

		if (playerEdit) memcpy(grid, swapGrid, fullGridSize);
	}
}


void generalLifeLike::updateGrid() {
	/*This function updates all of the cells in the grid to their next state based on their neighbours*/
	int newStatus;
	unsigned int neighbours;
	unsigned char* gridPtr;

	//creating a temporary grid copy so that the current grid is not overwritten while updating
	m.lock();
	memcpy(swapGrid, grid, fullGridSize);
	
	m.unlock();

	int x, y = 0;
	gridPtr = swapGrid;
	for (int z = 0; z < gridSize; z++) {
		y = 0;
		do {
			x = 0;
			do {
				// if the pointer is zero then there's no cell and no living neighbours at that point, so skip
				while (*gridPtr == 0) {
					gridPtr++;

					if (++x >= gridSize) goto rowDone;
				}

				// neighbours are stored bitwise, so shift to get the number of neighbours
				neighbours = *gridPtr >> bitShiftNum;

				// the cell is dead, so check if it's neighbours can repoduce at this point
				if ((*gridPtr & compareNum) == 0) {

					//iterate over all given neighbour reproduction numbers
					for (int num : born) {
						// if the neighbour number matches the rules, create a new cell and skip decaying
						if (neighbours == num) {
							addNewCell(x, y, z, false);
							goto next;
						}

					}

				}
				else if ((*gridPtr & compareNum) == decayStateNum) {
					// the cell is in the top decay state, so check if it has enough neighbours to stay alive 
					// or if it should start to decay
					for (int num : survives) {
						//the cell's neighbour count matches the survive rule, so don't decay
						if (neighbours == num)  goto next;
					}
					decayCell(x, y, z, false);
				}
				else if ((*gridPtr & compareNum) > 0) {
					// the cell is already in decay, so continue decay regardless of neighbours
					decayCell(x, y, z, false);
				}

			next:;
			x++;
			gridPtr++;

			} while (x < gridSize);

		rowDone:;
			y++;
		} while (y < gridSize);
		//there's no z axis in the 2d version, so skip
		if (is2d) goto skip;
	}
	skip:;
}

void generalLifeLike::render(unsigned int &VAO, Shader &shaderProgram) {
	/*This function renders the cell grid, and sets the shader options to colour the grid in different ways*/

	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	unsigned char* gridPtr;
	gridPtr = swapGrid;
	int y, x = 0;
	int neighbours;
	m.lock();
	//float mult_num = 1.0 / gridSize;

	for (int z = 0; z < gridSize; z++)
	{
		y = 0;
		do {
			x = 0;
			do {
				//If there's no cell or neighbours here skip
				while (*gridPtr == 0) {
					gridPtr++;
					if (++x >= gridSize) goto rowDone;
				}

				//check if there's an alive cell or a decaying cell here
				if ((*gridPtr & compareNum) != 0) {
					// grab the precomputed model matrix for this point and render
					glm::mat4 model = modelMatrices[x + y * gridSize + z * gridSize * gridSize];

					//Code for translating boxes on the fly - currently the program uses precomputation instead
					//glm::mat4 model = glm::mat4(1.0f);
					//translate the box to the right position. adding .5 since the boxes are centered at the passed in vector
					//model = glm::translate(model, glm::vec3(x + 0.5, y + 0.5, z + 0.5));

					//assign the model uniform
					shaderProgram.setMat4("model", model);

					//glm::vec3 boxColor = glm::vec3(neighbours * 0.1, neighbours * 0.15, 1);
					float multNum = 1.0 / gridSize;
					glm::vec3 boxColor = glm::vec3(x * multNum, y * multNum, z * multNum);

					shaderProgram.setVec3("inColor", boxColor);
					//draw the box
					glBindVertexArray(VAO);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
				gridPtr++;
				x++;

			} while (x < gridSize);
		rowDone:;
			y++;
		} while (y < gridSize);

		if (is2d) goto skip;

	}

	skip:;
	//std::cout << sizeof(unsigned int) << " " << sizeof(unsigned char) << " \n";
	m.unlock();
	//Sleep(10);

}

void generalLifeLike::renderGridOutline(unsigned int &VAO, Shader &shaderProgram) {
	
	//Drawing the grid outline so that the user can see where the simulation area is
	glm::vec3 boxColor = glm::vec3(0.9f, 0.9f, 1.0f);
	shaderProgram.setVec3("inColor", boxColor);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	shaderProgram.setMat4("model", gridOutlineModel);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

glm::mat4* calculateOffsets(int gridSize, bool is2d) {
	/* This function precomputes all translations for each cell in the simulation. This uses up more memory but 
	makes the program more efficent at runtime
	*/
	glm::mat4 *translations;
	if (is2d) translations = new glm::mat4[gridSize * gridSize];
	else translations = new glm::mat4[gridSize * gridSize * gridSize];

	for (int z = 0; z < gridSize; z++) {
		for (int y = 0; y < gridSize; y++) {
			for (int x = 0; x < gridSize; x++) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(x + 0.5, y + 0.5, z + 0.5));
				translations[x + y * gridSize + z * gridSize * gridSize] = model;
			}
		}
		if (is2d) goto skip;
	}
	skip:;
	return translations;
}

//--------------------------------------------------------------Getters--------------------------------------------------------------------------------------------------------

bool generalLifeLike::inGrid(int x, int y, int z) {
	/* Returns if the point is in the grid or not */
	return (x > 0 && y > 0
		&& z > 0 && x < gridSize
		&& y < gridSize && z < gridSize);
}

int generalLifeLike::getGridSize1d() {
	return gridSize;
}

int generalLifeLike::getFullGridSize() {
	return fullGridSize;
}

std::vector<int> generalLifeLike::getBorn() {
	return born;
}

std::vector<int> generalLifeLike::getSurvives() {
	return survives;
}

std::vector<int> generalLifeLike::getNeighbours() {
	return neighbourOffsets;
}

int generalLifeLike::getNumOfDecayStates() {
	return decayStateNum;
}

bool generalLifeLike::getIs2d() {
	return is2d;
}


//--------------------------------------------Setters----------------------------------------------------------------------------------------------------------------------------------------------------------

void generalLifeLike::resetAllDecayingCells(int oldCompareNum) {
	/* This function resets all cells in the to the max decay state. This is generally used when the rules for the grid are changed by the user. */

	unsigned char* gridPtr;
	//Set the grid pointer to the the swap grid
	gridPtr = swapGrid;
	//Set the first grid to be zeroed out
	grid = new unsigned char[fullGridSize];
	memset(grid, 0, fullGridSize);

	//Here, it iterates over the swap grid to find where a cell currently lives and then spawns the cell in to the completely reset original grid. This prevents issues with removing cells after changing the number of decay states
	int x, y;
	for (int z = 0; z < gridSize; z++)
	{
		y = 0;
		do {
			x = 0;
			do {

				//Skip over empty cells with no neighbours
				while (*gridPtr == 0) {
					gridPtr++;
					if (++x >= gridSize) goto rowDone;
				}

				//Using a passed in old comparison number since the new comparison number may not work with the not yet updated grid
				if ((*gridPtr & oldCompareNum) > 0) {
					//Add a cell in to the grid, not the swap. The newly set comparison number (top decay state) will be used for the newly added cell
					//removeCell(x, y, z, false);
					addNewCell(x, y, z, false);
				}
				
				gridPtr++;
				x++;

			} while (x < gridSize);
		rowDone:;
			y++;
		} while (y < gridSize);

		if (is2d) goto skip;

	}

	skip:;

	//Since the grid is now correctly filled out with cells with the correct number of decay states, copy the grid over to the swap grid
	memcpy(swapGrid, grid, fullGridSize);
}

void generalLifeLike::setBorn(std::vector<int> b) {
	m.lock();
	if (paused){
		born = b;
	}
	m.unlock();
}

void generalLifeLike::setSurvives(std::vector<int> s) {
	m.lock();
	if (paused) {
		survives = s;
	}
	m.unlock();
}

void generalLifeLike::setNeighbours(std::vector<glm::vec3> neighs) {
	/* This sets which cells are considered "neighbours" to a cell based on the inputed array neigh of neighbours */
	m.lock();
	if (paused) {
		for (glm::vec3 neighbour : neighs) {

			if (!is2d) addToNeighbours(neighbour.x, neighbour.y, neighbour.z);
			else addToNeighbours(neighbour.x, neighbour.y);

			//if we've hit the max number of possible neighbours then skip
			if (numOfNeighbours > numOfTotalPossibleNeighbours) break;
		}
	}
	m.unlock();
}

void generalLifeLike::setDecayStates(int decayStates) {
	if (paused) {
		m.lock();
		int oldCompare = compareNum;
		decayStateNum = decayStates;
		bitShiftNum = 1;
		neighbourBitShiftNum = 1;
		while (decayStateNum > neighbourBitShiftNum) {
			bitShiftNum++;
			neighbourBitShiftNum *= 2;
		}
		compareNum = neighbourBitShiftNum * 2 - 1;

		//Calculate the number of neighbours possible given the number of decay states
		numOfTotalPossibleNeighbours = 255 - std::pow(2, bitShiftNum);
		std::cout << "With the current number of decay states, you can have at most: " << numOfTotalPossibleNeighbours << " neighbours for a cell. \n";

		if (numOfTotalPossibleNeighbours < 1) {
			std::cout << "ERROR :: TOO MANY DECAY STATES \n";
		}

		//this resizes the neighbours vector if the number of neighbour offsets is now too large
		if (gridSetup) {
			while (numOfTotalPossibleNeighbours < numOfNeighbours) {
				neighbourOffsets.pop_back();
				neighbourOffsets.pop_back();
				neighbourOffsets.pop_back();
				numOfNeighbours--;
			}

			resetAllDecayingCells(oldCompare);
		}

		m.unlock();
	}
}
//--------------------------------------------Mutators------------------------------------------------------------------------------------------------------------------------------------------------- 

void addBlocks(std::vector<int>& listToEdit, int numToAdd, int optionalEndRange) {
	if (numToAdd > 0 && optionalEndRange > numToAdd) {
		for (int i = numToAdd; i < optionalEndRange; i++) {
			listToEdit.push_back(i);
		}
	}
	else if (numToAdd > 0) {
		listToEdit.push_back(numToAdd);
	}
}

void removeBlocks(std::vector<int>& listToEdit, int numToRemove) {
	std::vector<int> indexsToRemove{};
		for (int i = 0; i < listToEdit.size(); i++) {
			if (listToEdit[i] == numToRemove) {
				indexsToRemove.push_back(i);
			}
		}
		int numRemoved = 0;
		for (int i : indexsToRemove) {
			listToEdit.erase(listToEdit.begin() + i - numRemoved);
			numRemoved++;
		}
}

void generalLifeLike::addBlocksToBorn(int numToAdd, int optionalEndRange) {
	if (paused) {
		addBlocks(born, numToAdd, optionalEndRange);
	}
}

void generalLifeLike::removeBlocksFromBorn(int numToRemove) {
	if (paused) {
		removeBlocks(born, numToRemove);
	}
}

void generalLifeLike::addBlocksToSurvives(int numToAdd, int optionalEndRange) {
	if (paused) {
		addBlocks(survives, numToAdd, optionalEndRange);
	}
}

void generalLifeLike::removeBlocksFromSurvives(int numToRemove) {
	if (paused) {
		removeBlocks(survives, numToRemove);
	}
}
void generalLifeLike::addToNeighbours(int x, int y, int z) {
	//This function adds a new neighbour offset to the list of offsets. z is set to zero by default incase the list is 2d, and the z value is not given

	bool alreadyInList = false;

	//First, it checks if the offset is already in the list. If it is, then it won't add the new block
	for (int ni = 0; ni < numOfNeighbours; ni+=3) {
		if (neighbourOffsets[ni] == x && neighbourOffsets[ni + 1] == y && neighbourOffsets[ni + 2] == z) {
			alreadyInList = true;
			break;
		}
	}
	//If it's not in the list, then it is added
	if (!alreadyInList) {
		neighbourOffsets.push_back(x);
		neighbourOffsets.push_back(y);
		neighbourOffsets.push_back(z);
		numOfNeighbours++;
		resetAllDecayingCells(compareNum);
	}
	
}

void generalLifeLike::removeFromNeighbours(int x, int y, int z) {
	//This function searches the list of neighbours and removes the neighbour offset if it is found.
	for (int ni = 0; ni < numOfNeighbours; ni++) {
		if (neighbourOffsets[ni] == x && neighbourOffsets[ni + 1] == y && neighbourOffsets[ni + 2] == z) {
			neighbourOffsets.erase(neighbourOffsets.begin() + ni);
			neighbourOffsets.erase(neighbourOffsets.begin() + ni);
			neighbourOffsets.erase(neighbourOffsets.begin() + ni);
			resetAllDecayingCells(compareNum);
			break;
		}
	}
}

void generalLifeLike::removeAllNeighbours() {
	if (paused) {
		while (numOfNeighbours > 0) {
			neighbourOffsets.pop_back();
			neighbourOffsets.pop_back();
			neighbourOffsets.pop_back();
			numOfNeighbours--;
		}
		resetAllDecayingCells(compareNum);
	}
}

void generalLifeLike::createMooreNeighbourhood() {
	if (paused) {
		while (numOfNeighbours > 0) {
			neighbourOffsets.pop_back();
			neighbourOffsets.pop_back();
			neighbourOffsets.pop_back();
			numOfNeighbours--;
		}

		for (int x = -1; x < 2; x++) {
			for (int y = -1; y < 2; y++) {
				for (int z = -1; z < 2; z++) {
					if (!(x == 0 && y == 0 && z == 0)) {
						addToNeighbours(x, y, z);
					}

				}
			}
		}
	}
}

void generalLifeLike::createVonNeumanNeighbourhood() {
	if (paused) {
		while (numOfNeighbours > 0) {
			neighbourOffsets.pop_back();
			neighbourOffsets.pop_back();
			neighbourOffsets.pop_back();
			numOfNeighbours--;
		}

		addToNeighbours(-1, 0, 0);
		addToNeighbours(0, -1, 0);
		addToNeighbours(1, 0, 0);
		addToNeighbours(0, 1, 0);
		if (!is2d) {
			addToNeighbours(0, 0, 1);
			addToNeighbours(0, 0, -1);
		}
	
	}
}

void generalLifeLike::generateRandomSeed(int numBlocksToGenerate) {
	m.lock();
	if (paused) {
		int x, y, z;
		for (int i = 0; i < numBlocksToGenerate; i++) {
			x = rand() % gridSize;
			y = rand() % gridSize;
			z = rand() % gridSize;
			if (!is2d) addNewCell(x, y, z, true);
			else addNewCell(x, y, 0, true);
		}
		
	}
	m.unlock();
}

//Resets exisiting grid to be empty
void generalLifeLike::resetGrid() {
	m.lock();
	if (paused) {
		
		if (is2d) {
			//if the program is set to 2d then it only needs to save the x and y coordinates of neighbours blocks
			fullGridSize = gridSize * gridSize;
		}
		else {
			fullGridSize = gridSize * gridSize * gridSize;
		}

		grid = new unsigned char[fullGridSize];
		swapGrid = new unsigned char[fullGridSize];

		modelMatrices = new glm::mat4[fullGridSize];
		modelMatrices = calculateOffsets(gridSize, is2d);

		memset(grid, 0, fullGridSize);
		//copys data from the temp grid to the new grid
		memcpy(swapGrid, grid, fullGridSize);

		setupGridOutlineModel();
		
	}
	m.unlock();
}

//Creates a new grid of requested size
void generalLifeLike::resetGrid(int newGridSize, bool twoD) {
	m.lock();
	if (paused) {
		
		is2d = twoD;
		gridSize = newGridSize;
		if (is2d) {
			//if the program is set to 2d then it only needs to save the x and y coordinates of neighbours blocks
			fullGridSize = newGridSize * newGridSize;
		}
		else {
			fullGridSize = newGridSize * newGridSize * newGridSize;
		}

		delete grid;
		delete swapGrid;
		delete modelMatrices;

		grid = new unsigned char[fullGridSize];
		swapGrid = new unsigned char[fullGridSize];

		modelMatrices = new glm::mat4[fullGridSize];
		modelMatrices = calculateOffsets(newGridSize, is2d);

		memset(grid, 0, fullGridSize);
		//copys data from the temp grid to the new grid
		memcpy(swapGrid, grid, fullGridSize);

		setupGridOutlineModel();
		
	}
	m.unlock();
}
