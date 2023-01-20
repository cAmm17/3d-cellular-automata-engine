#include "main.h"
#include "menu.h"
#include "Shader.h"
#include "Camera.h"
#include "generalLifeLike.h"
#include "imgui.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void updateCurGrid();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

static const int WINDOWHEIGHT = 1200;
static const int WINDOWWIDTH = 2400;
generalLifeLike* gameGrid;
static bool hidden = false;

static Camera *programCamera = new Camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(10.0f, 10.0f, 10.0f),
	glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

static menu *gridMenu = new menu();

int main()
{
	//Initialize the grid manager for the game
	int gridSz = 50;
	bool twoD = false;
	float update_rate = 0.5;
	//Setting shader paths
	const char* vertexShaderPath = "4.6.shader2.vs";
	const char* fragmentShaderPath = "4.6.shader2.fs";

	//Initiate glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a new glfw window
	GLFWwindow* window = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "GameOfLife", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Set the current window for glfw
	glfwMakeContextCurrent(window);

	//Load in Glad and make sure it loaded in 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Additional settings (sets opengl to check for depth and captures the mouse for mouse input)
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	//Create the opengl viewport and use glfw to set the window
	glViewport(0, 0, WINDOWWIDTH, WINDOWHEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//Initialize the shader
	Shader shaderProgram = Shader::Shader(vertexShaderPath, fragmentShaderPath);
	shaderProgram.use();

	//test for grid, remove later
	std::vector<glm::vec3> startBlock;
	//startBlock.push_back(glm::vec3(10.0f, 10.0f, 10.0f));
	//startBlock.push_back(glm::vec3(11.0f, 10.0f, 10.0f));
	//startBlock.push_back(glm::vec3(10.0f, 11.0f, 10.0f));
	//startBlock.push_back(glm::vec3(11.0f, 11.0f, 10.0f));
	//startBlock.push_back(glm::vec3(10.0f, 10.0f, 11.0f));
	//startBlock.push_back(glm::vec3(11.0f, 10.0f, 11.0f));
	//startBlock.push_back(glm::vec3(10.0f, 11.0f, 11.0f));
	//startBlock.push_back(glm::vec3(11.0f, 11.0f, 11.0f));
	//startBlock.push_back(glm::vec3(11.0f, 11.0f, 11.0f));
	//startBlock.push_back(glm::vec3(11.0f, 12.0f, 11.0f));
	//startBlock.push_back(glm::vec3(12.0f, 11.0f, 11.0f));
	//startBlock.push_back(glm::vec3(12.0f, 12.0f, 11.0f));
	//startBlock.push_back(glm::vec3(int(gridSz / 2), int(gridSz / 2), int(gridSz / 2) + 1));
	//startBlock.push_back(glm::vec3(int(gridSz / 2), int(gridSz / 2) + 1, int(gridSz / 2) + 1));
	//startBlock.push_back(glm::vec3(int(gridSz / 2) + 1, int(gridSz / 2), int(gridSz / 2) + 1));
	//startBlock.push_back(glm::vec3(int(gridSz / 2) + 1, int(gridSz / 2) + 1, int(gridSz / 2) + 1));
	int randnum;
	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 20; y++) {
			for (int z = 0; z < 20; z++) {
					startBlock.push_back(glm::vec3(x, y, z));
			}
		}
	}
	//startBlock.push_back(glm::vec3(3.0f, 3.0f, 3.0f));
	std::vector<glm::vec3> initNeigh;
	
	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			for (int z = -1; z < 2; z++) {
				if (!(x == 0 && y == 0 && z == 0)) {
					initNeigh.push_back(glm::vec3(x, y, z));
				}

			}
		}
	}
	/*
	initNeigh.push_back(glm::vec3(0, 0, 1));
	initNeigh.push_back(glm::vec3(0, 1, 0));
	initNeigh.push_back(glm::vec3(1, 0, 0));
	initNeigh.push_back(glm::vec3(0, 0, -1));
	initNeigh.push_back(glm::vec3(0, -1, 0));
	initNeigh.push_back(glm::vec3(-1, 0, 0));
	*/

	std::vector<int> survives{ 4, 6, 8};
	std::vector<int> born{5, 6};

	gameGrid = new generalLifeLike(gridSz, startBlock, initNeigh, 4, survives, born, false);
	

	//gameGrid.initializeGrid(8, startBlock);

	//Set projection matrix once since it is not going to be reset during gameplay
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)WINDOWWIDTH / (float)WINDOWHEIGHT, 0.1f, 100.0f);
	shaderProgram.setMat4("projection", projection);
	
	
	std::thread updater(updateCurGrid);
	float mousex, mousey, mousez;

	gridMenu->setupGui(window, "#version 460 core", gameGrid);

	//Render loop
	while (!glfwWindowShouldClose(window))
	{
		//Clear color and depth each render iteration so that the next iteration is drawn correctly.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Checks for user input
		processInput(window);

		
		shaderProgram.use();

		glm::mat4 view = glm::mat4(1.0f);
		view = programCamera->setLookAt();
		shaderProgram.setMat4("view", view);


		if (gridMenu->showMenu) {
			gridMenu->renderGui();
		}
		else {
			gameGrid->render(VAO, shaderProgram);
			gameGrid->renderGridOutline(VAO, shaderProgram);

			mousex = int(programCamera->cameraPos.x + programCamera->cameraFront.x * 3) + .5;
			mousey = int(programCamera->cameraPos.y + programCamera->cameraFront.y * 3) + .5;
			mousez = int(programCamera->cameraPos.z + programCamera->cameraFront.z * 3) + .5;
			if (gameGrid->inGrid(mousex, mousey, mousez) && gameGrid->paused && !hidden)
			{
				glm::mat4 mouseoverModel = glm::mat4(1.0f);
				mouseoverModel = glm::translate(mouseoverModel, glm::vec3(mousex, mousey, mousez));

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				shaderProgram.setMat4("model", mouseoverModel);

				glBindVertexArray(VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}



		//Swap drawing and displayed buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	gameGrid->update = false;
	//if(updater.joinable())
	updater.join();

	gridMenu->cleanupGui();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//Clear program camera memory
	delete programCamera;
	delete gameGrid;

	//Clear glfw's memory
	glfwTerminate();
	return 0;
}

//Process user input
void processInput(GLFWwindow *window)
{
	programCamera->ProcessKeyboardInput(window);

	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && gameGrid->paused) {
		float mousex = int(programCamera->cameraPos.x + programCamera->cameraFront.x * 3) + .5;
		float mousey = int(programCamera->cameraPos.y + programCamera->cameraFront.y * 3) + .5;
		float mousez = int(programCamera->cameraPos.z + programCamera->cameraFront.z * 3) + .5;
		gameGrid->m.lock();
		gameGrid->addNewCell(int(mousex), int(mousey), int(mousez), true);
		gameGrid->m.unlock();
	}
	
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && gameGrid->paused) {
		float mousex = int(programCamera->cameraPos.x + programCamera->cameraFront.x * 3) + .5;
		float mousey = int(programCamera->cameraPos.y + programCamera->cameraFront.y * 3) + .5;
		float mousez = int(programCamera->cameraPos.z + programCamera->cameraFront.z * 3) + .5;
		gameGrid->m.lock();
		gameGrid->removeCell(int(mousex), int(mousey), int(mousez), true);
		gameGrid->m.unlock();
	}

	
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		gameGrid->paused = true;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		gameGrid->paused = false;
	}
		

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		hidden = !hidden;
	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		gridMenu->showMenu = true;
		if (gridMenu->showMenu) {
			gameGrid->paused = true;
			hidden = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		gridMenu->showMenu = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	programCamera->ProcessMouseInput(xpos, ypos);
}

void updateCurGrid() {
	while (gameGrid->update) {
		if (!gameGrid->paused) {
			gameGrid->updateGrid();
			Sleep(200);
		}
	}
}

//If the window is resized by the user, then the opengl window will adjust as well
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

