/*
Todo:
	-Keep Track of Boxes:
		-initialize one D array of vectors (for faster accessing)
		-calculate grid size (space between each block)
		-divide by 2 to get the center for the first block (won't be exact but who cares)
		-
		-possible box class?

	-Implement rules:
		-Number of live cells needed to live/die(over and under population)/reproduce.
			-make this customizable
		-try to make it run faster that a tortise
		-Add some sort of delay between updates so that boxes are on the screen for more than a frame

	-Figure out skybox so player doesn't zoom off in to the distance

	-Add box placement mechanisms

	-Add menu window for options (eventually)

*/
#include "gridManager.h"

//defining the type for the grid. It is a dynamic array of tuples. Each tuple contains the location
//of the box (generated when the grid is first created) and an int (0 or 1) determining if a box should
//be drawn at that position or not

gridManager::gridManager(int gridsz) {
	grid = new unsigned char[gridsz * gridsz * gridsz];
	tempgrid = new unsigned char[gridsz * gridsz * gridsz];
	gridSize = gridsz;
	memset(grid, 0, gridsz * gridsz * gridsz);
	gridCenter = int(gridSize / 2);

	//for (glm::vec3 &box : initialBoxes) {
	//	editBox(int(box.x), int(box.y), int(box.z), 1);
	//}
	//
	//memcpy(tempgrid, grid, gridSize * gridSize * gridSize);

	update = true;
	notPaused = false;
	//replicateLower = rL;
	//replicateUpper = rU;
	//liveLower = lL;
	//liveUpper = lU;
}

gridManager::~gridManager() {
	delete[] grid;
	delete[] tempgrid;
}
//
//int gridManager::getState(int xin, int yin, int zin) {
//	unsigned char *gridBoxPtr = grid + xin + yin * gridSize + zin * gridSize * gridSize;
//
//	return *gridBoxPtr & 0x01;
//}
//
//
//void gridManager::editBox(int xin, int yin, int zin, int editType, bool playerEdit) {
//	unsigned char* gridToEdit;
//	if (playerEdit) {
//		gridToEdit = tempgrid;
//	}
//	else {
//		gridToEdit = grid;
//	}
//
//	if (xin < gridSize && yin < gridSize && zin < gridSize) {
//		//1 adds a box
//		unsigned char *gridBoxPtr = gridToEdit + xin + yin * gridSize + zin * gridSize * gridSize;
//
//		if (editType == 1 && !(*gridBoxPtr & 0x01)) {
//			*(gridBoxPtr) |= 0x01;
//			for (int x = -1; x < 2; x++) {
//				for (int y = -1; y < 2; y++) {
//					for (int z = -1; z < 2; z++) {
//						//this nightmare if statement checks for boxes that are out of range and makes sure to not count the actual box itself
//						if (xin + x >= 0 && xin + x < gridSize &&
//							yin + y >= 0 && yin + y < gridSize &&
//							zin + z >= 0 && zin + z < gridSize &&
//							!(x == 0 && y == 0 && z == 0)) {
//							gridBoxPtr = gridToEdit + xin + x + (yin + y) * gridSize + (zin + z) * gridSize * gridSize;
//							*(gridBoxPtr) += 0x02;
//						}
//					}
//				}
//			}
//
//
//		}
//		//0 removes it
//		else if (editType == 0 && *gridBoxPtr & 0x01) {
//			*(gridBoxPtr) &= ~0x01;
//
//			for (int x = -1; x < 2; x++) {
//				for (int y = -1; y < 2; y++) {
//					for (int z = -1; z < 2; z++) {
//						//this nightmare if statement checks for boxes that are out of range and makes sure to not count the actual box itself
//						if (xin + x >= 0 && xin + x < gridSize &&
//							yin + y >= 0 && yin + y < gridSize &&
//							zin + z >= 0 && zin + z < gridSize &&
//							!(x == 0 && y == 0 && z == 0)) {
//							gridBoxPtr = gridToEdit + xin + x + (yin + y) * gridSize + (zin + z) * gridSize * gridSize;
//							*(gridBoxPtr) -= 0x02;
//						}
//					}
//				}
//			}
//
//			}
//		if (playerEdit) memcpy(grid, tempgrid, gridSize * gridSize * gridSize);
//
//	}
//}
//
//
//void gridManager::updateGrid() {
//	int newStatus;
//	unsigned int neighbours;
//	unsigned char* gridPtr;
//
//	//creating a temporary grid copy so that the current grid is not overwritten while updating
//	m.lock();
//	memcpy(tempgrid, grid, gridSize * gridSize * gridSize);
//	m.unlock();
//
//	//std::tuple <glm::vec3, int> temp;
//	int x, y = 0;
//	gridPtr = tempgrid;
//	for(int z = 0; z < gridSize; z++) {
//		y = 0;
//		do {
//			x = 0;
//			do {
//
//				while (*gridPtr == 0) {
//					gridPtr++;
//
//					if (++x >= gridSize) goto rowDone;
//				}
//				neighbours = *gridPtr >> 1;
//				
//				if (!(*gridPtr & 0x01) && *gridPtr && neighbours >= replicateLower && neighbours <= replicateUpper) {
//					gridManager::editBox(x, y, z, 1);
//					
//				}
//				else if (*gridPtr & 0x01 && *gridPtr && neighbours < liveLower || *gridPtr & 0x01 == 1 && neighbours > liveUpper) {
//					gridManager::editBox(x, y, z, 0);
//				}
//				x++;
//				gridPtr++;
//
//			} while (x < gridSize);
//
//			rowDone:;
//			y++;
//		} while (y < gridSize);
//	} 
//
//}
//
