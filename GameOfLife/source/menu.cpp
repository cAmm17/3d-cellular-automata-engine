#include "menu.h"


void menu::setupGui(GLFWwindow* window, const char* glsl_version, generalLifeLike* inGrid) {
	grid = inGrid;
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

}

void menu::renderGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Grid Options");
	if (ImGui::BeginMenu("Current Grid Stats")) {
		ImGui::Text("Current number of decay states: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(grid->getNumOfDecayStates()).c_str());
		int tempCount = 1;

		ImGui::Text("Survival States");
		char buffer[10];
		for (int surv : grid->getSurvives()) {
			ImGui::Text(std::to_string(surv).c_str());
			if (tempCount % 5 != 0) {
				ImGui::SameLine();
				ImGui::Text("         ");
				ImGui::SameLine();
			}
			tempCount++;
		}

		tempCount = 1;
		ImGui::Text("");
		ImGui::Text("Born States");
		for (int brn : grid->getBorn()) {
			ImGui::Text(std::to_string(brn).c_str());
			if (tempCount % 5 != 0) {
				ImGui::SameLine();
				ImGui::Text("         ");
				ImGui::SameLine();
			}
			tempCount++;
		}

		tempCount = 1;
		ImGui::Text("");
		ImGui::Text("Neighhbour Offsets");
		
		for (int neigh : grid->getNeighbours()) {
			ImGui::Text(std::to_string(neigh).c_str());
			if (tempCount % 3 != 0) ImGui::SameLine();
			tempCount++;
		}


		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit Grid Settings")) {

		ImGui::InputInt("Add Survival States", &newSurvStates);
		ImGui::InputInt("Optional End Range", &endRange);
		if (ImGui::Button("Add new Survival States")) {
			if (int(endRange) > 0) {
				grid->addBlocksToSurvives(int(newSurvStates), int(endRange));
			}
			else {
				grid->addBlocksToSurvives(int(newSurvStates), 0);
			}

		}

		ImGui::InputInt("Remove Survival States", &rmvSurvStates);
		if (ImGui::Button("Remove Survival State")) {
			grid->removeBlocksFromSurvives(int(rmvSurvStates));
		}

		ImGui::InputInt("Add Birth States", &newBirthStates);
		ImGui::InputInt("Optional End Range (3)", &endRange2);
		if (ImGui::Button("Add Birth State(s)")) {
			if (int(endRange2) > 0) {
				grid->addBlocksToBorn(int(newBirthStates), int(endRange2));
			}
			else {
				grid->addBlocksToBorn(int(newBirthStates), 0);
			}
		}

		ImGui::InputInt("Remove Birth States", &rmvBirthStates);
		if (ImGui::Button("Remove Birth State")) {
				grid->removeBlocksFromBorn(int(rmvBirthStates));
		}

		ImGui::InputInt("Add: X", &addNeighbourX);
		ImGui::InputInt("Add: Y", &addNeighbourY);
		ImGui::InputInt("Add: Z", &addNeighbourZ);
		if (ImGui::Button("Add Block(s) to Neighbours")) {
			if (grid->getIs2d()) {
				grid->addToNeighbours(int(addNeighbourX), int(addNeighbourY));
			}
			else {
				grid->addToNeighbours(int(addNeighbourX), int(addNeighbourY), int(addNeighbourZ));
			}

		}

		ImGui::InputInt("Remove: X", &removeNeighbourX);
		ImGui::InputInt("Remove: Y", &removeNeighbourY);
		ImGui::InputInt("Remove: Z", &removeNeighbourZ);
		if (ImGui::Button("Remove Block(s) from Neighbours")) {
			if (grid->getIs2d()) {
				grid->removeFromNeighbours(int(removeNeighbourX), int(removeNeighbourY));
			}
			else {
				grid->removeFromNeighbours(int(removeNeighbourX), int(removeNeighbourY), int(removeNeighbourZ));
			}

		}

		//clear neighbours
		if (ImGui::Button("Remove All Neighbours")) {
			grid->removeAllNeighbours();
		}

		//reset grid
		if (ImGui::Button("Clear and Reset Grid")) {
			grid->resetGrid();
		}

		ImGui::InputInt("New Grid Size (input one side's dimension, must be between 1 and 100)", &newGridSize);
		ImGui::Checkbox("Should the new grid be 2d?", &newGrid2d);
		if (ImGui::Button("Clear, Reset, and Resize the current grid")) {
			if (newGridSize > 0 && newGridSize < 101) {
				grid->resetGrid(newGridSize, newGrid2d);
			}
			
		}

		if (ImGui::Button("Create Moore Neighbourhood")) {
			grid->createMooreNeighbourhood();
		}
		if (ImGui::Button("Create Von Neuman Neighbourhood")) {
			grid->createVonNeumanNeighbourhood();
		}

		ImGui::InputInt("New Num of decay states", &newDecayStates);
		if (ImGui::Button("Change number of decay states a cell goes through")) {
			grid->setDecayStates(newDecayStates);
		}

		ImGui::InputInt("Number of cells to add", &numCellsToAdd);
		if (ImGui::Button("Fill grid with random cells")) {
			if (numCellsToAdd > 0 && numCellsToAdd < 100000) {
				grid->generateRandomSeed(numCellsToAdd);
			}
		}
		//Change num decay states
		//default neighbourhoods - 2 types (direct contact and direct + diaganol

		ImGui::EndMenu();
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void menu::cleanupGui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}