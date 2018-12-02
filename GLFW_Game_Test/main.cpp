#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <vector>

#define PI 3.1415926 //used in the drawCircle() method
#define GRAVITY 0.001;

using namespace std;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

double mouseX;
double mouseY;

struct Point { 
	Point(double x, double y) : x(x), y(y) {} 
	double x, y; 
};

struct Velocity { 
	Velocity(double velX, double velY) : velX(velX), velY(velY) {}
	double velX, velY; 
};

vector<Point> circles; //vector of circles to be drawn
vector<Velocity> circlesVelocities;

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

		Point circle(mouseX, mouseY);

		Velocity circVel(0, 0.2);

		circles.push_back(circle);
		circlesVelocities.push_back(circVel);
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
	glColor3f(1.f, 1.f, 1.f); //white

	for (int i = 0; i < circles.size(); i++) {
		drawCircle(circles[i].x, circles[i].y, 25);
	}
}

void tick() {
	for (int i = 0; i < circles.size(); i++) {
		circlesVelocities[i].velY += GRAVITY;
		circles[i].y += circlesVelocities[i].velY;

		if (circles[i].y > WINDOW_HEIGHT) { //if the coordinate if the ball is off the screen
			/* Makes balls disapear when off screen */
			circles.erase(circles.begin() + i);
			circlesVelocities.erase(circlesVelocities.begin() + i);

			/* Bounces the balls */
			//circles[i].y = WINDOW_HEIGHT - 100;
			//circlesVelocities[i].velY = -circlesVelocities[i].velY + 0.2;
		}
	}
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

	/* Mouse position and action callbacks */
	glfwSetCursorPosCallback(window, cursorPositionCallback); //gets the position of the cursor
	glfwSetMouseButtonCallback(window, mouseButtonCallback); //checks for mouse clicks and actions

	/* Main Game Loop*/
	while (!glfwWindowShouldClose(window)) {
		processInput(window); //keyboard input

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //sets the clear color to a dark greenish thing
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);

		/* Render here */
		render();

		/* Update Here */
		tick();

		glfwSwapBuffers(window); //swaps the front and back buffers
		glfwPollEvents(); 
	}

	glfwTerminate();
	return 0;
}