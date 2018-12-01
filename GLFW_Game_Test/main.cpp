#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <vector>

#define PI 3.1415926 //used in the drawCircle() method

using namespace std;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

float mouseX;
float mouseY;

struct Point { float x, y; };

vector<Point> circles; //vector of circles to be drawn

void drawCircle(float x, float y, float size);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {	
	mouseX = xpos;
	mouseY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mod) {
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		cout << mouseX << " " << mouseY << endl;

		Point circle;
		circle.x = mouseX;
		circle.y = mouseY;

		circles.push_back(circle);
	}
}

void drawCircle(float posx, float posy, float size) {
	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < 360; i++) {
		double x = (cos(i * (PI / 180)) * size) + posx;
		double y = (sin(i * (PI / 180)) * size) + posy;

		glVertex2f(x, y);
	}

	glEnd();
}

void render() {
	glColor3f(1.f, 1.f, 1.f);

	for (int i = 0; i < circles.size(); i++) {
		drawCircle(circles[i].x, circles[i].y, 50);
	}
}

void tick() {

}

int main() {
	glfwInit(); 

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Game Test", NULL, NULL); //creates a new window
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	/* Setting up the window */
	glfwMakeContextCurrent(window);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //notifies when window is resized
	glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1); //uses pixel coordinates instead of polar coordinates
	const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor()); //creates the videomode with primary monitor
	glfwSetWindowPos(window, vidMode->width / 2 - (WINDOW_WIDTH / 2), vidMode->height / 2 - (WINDOW_HEIGHT / 2)); //sets the window to be in the middle of the screen

	glfwSetCursorPosCallback(window, cursorPositionCallback); //gets the position of the cursor
	glfwSetMouseButtonCallback(window, mouseButtonCallback); //checks for mouse clicks and actions

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //sets the clear color to a dark greenish thing
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);

		/* Render here */
		glColor3f(1.f, 1.f, 1.f); //white
		render();
		
		/* Update Here */
		tick();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}