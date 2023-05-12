#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 640
#define HEIGHT 640
#define N 100
#define M 3
#define PI 3.14159265359


GLuint vao[11], vbo[11];

GLfloat points[24] = {	
	-0.9f, 0.5f, 0.0f,
	-0.75f, 1.0f, 0.0f,
	-0.4f, 0.0f, 0.0f,
	-0.65f, -0.5f, 0.0f,	
	0.75f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,	
	1.0f, 0.3f, 0.0f,
	0.3f, 1.0f, 0.0f

};

GLfloat circle1[3 * N];
GLfloat circle2[3 * N];
GLfloat circle3[3 * N];
GLfloat circle4[3 * N];
GLfloat circle5[3 * N];
GLfloat circle6[3 * N];
GLfloat circle7[3 * N];
GLfloat circle8[3 * N];



GLint dragged = -1;

GLfloat HermiteCurve[3 * N * M];


void drawCircle(GLfloat circle[3*N], int n) {
	GLfloat r = 0.02f;
	GLfloat t;
	GLint i;
	GLfloat step = (2 * PI) / N;

	for (i = 0; i < N; i++) {
		t = i * step;
		circle[i * 3] = points[n*3] + r * cos(t);
		circle[i * 3 + 1] = points[n*3+1] + r * sin(t);
		circle[i * 3 + 2] = 0.0f;
	}
}


GLfloat dist2_2d(GLfloat P1x, GLfloat P1y, GLfloat P2x, GLfloat P2y) {

	GLfloat dx = P1x - P2x;
	GLfloat dy = P1y - P2y;
	return dx * dx + dy * dy;

}



void drawHermite(int n) {
	GLfloat t;
	GLint i;
	GLfloat step = 2.0f / (N - 1);
	HermiteCurve[3 * n * N] = points[n * 6 + 0];
	HermiteCurve[3 * n * N + 1] = points[n * 6 + 1];
	HermiteCurve[3 * n * N + 2] = 0.0f;

	for (i = 1; i < N - 1; i++) {
		t = -1 + i * step;
		HermiteCurve[(n * N + i) * 3] = points[n * 6 + 0] * (1.0f / 4.0f * t * t * t - 3.0f / 4.0f * t + 1.0f / 2.0f) + points[n * 6 + 6] * (-1.0f / 4.0f * t * t * t + 3.0f / 4.0f * t + 1.0f / 2.0f) + (points[n * 6 + 0] - points[n * 6 + 3]) * (1.0f / 4.0f * t * t * t - 1.0f / 4.0f * t * t - 1.0f / 4.0f * t + 1.0f / 4.0f) + (points[n * 6 + 6] - points[n * 6 + 9]) * (1.0f / 4.0f * t * t * t + 1.0f / 4.0f * t * t - 1.0f / 4.0f * t - 1.0f / 4.0f);
		HermiteCurve[(n * N + i) * 3 + 1] = points[n * 6 + 1] * (1.0f / 4.0f * t * t * t - 3.0f / 4.0f * t + 1.0f / 2.0f) + points[n * 6 + 7] * (-1.0f / 4.0f * t * t * t + 3.0f / 4.0f * t + 1.0f / 2.0f) + (points[n * 6 + 1] - points[n * 6 + 4]) * (1.0f / 4.0f * t * t * t - 1.0f / 4.0f * t * t - 1.0f / 4.0f * t + 1.0f / 4.0f) + (points[n * 6 + 7] - points[n * 6 + 10]) * (1.0f / 4.0f * t * t * t + 1.0f / 4.0f * t * t - 1.0f / 4.0f * t - 1.0f / 4.0f);
		HermiteCurve[(n * N + i) * 3 + 2] = 0.0f;
	}

	HermiteCurve[3 * (n * N + i)] = points[n * 6 + 6];
	HermiteCurve[3 * (n * N + i) + 1] = points[n * 6 + 7];
	HermiteCurve[3 * (n * N + i) + 2] = 0.0f;
}

void recalcHermite() {
	drawHermite(0);
	drawHermite(1);
	drawHermite(2);
	drawCircle(circle1, 0);
	drawCircle(circle2, 2);
	drawCircle(circle3, 4);
	drawCircle(circle4, 6);
	drawCircle(circle5, 1);
	drawCircle(circle6, 3);
	drawCircle(circle7, 5);
	drawCircle(circle8, 7);

	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * M * sizeof(GLfloat), HermiteCurve, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N *sizeof(GLfloat), circle1, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle2, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle3, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao[6]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle4, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(vao[7]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle5, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(vao[8]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle6, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(vao[9]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle7, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(vao[10]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle8, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

GLint getActivePoint(GLfloat* p, GLfloat sensitivity, GLfloat x, GLfloat y) {

	GLfloat s = sensitivity * sensitivity;
	GLfloat xNorm = -1 + x / (WIDTH / 2);
	GLfloat yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);

	for (GLint i = 0; i < 8; i++)
		if (dist2_2d(p[i * 3], p[i * 3 + 1], xNorm, yNorm) < s)
			return i;

	return -1;
}



void cursorPosCallback(GLFWwindow* window, double x, double y) {

	if (dragged >= 0) {

		GLfloat xNorm = -1 + x / (WIDTH / 2);
		GLfloat yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);

		points[3 * dragged] = xNorm;	 // x coord
		points[3 * dragged + 1] = yNorm; // y coord
	}
	recalcHermite();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double x, y;

		glfwGetCursorPos(window, &x, &y);
		dragged = getActivePoint(points, 0.1f, x, y);
		recalcHermite();
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		dragged = -1;
}


int main() {
	GLFWwindow* window = NULL;
	const GLubyte* renderer;
	const GLubyte* version;

	const char* vertex_shader =
		"#version 330\n"
		"in vec3 vp;"
		"void main () {"
		"  gl_Position = vec4(vp, 1.0);"
		"}";

	const char* fragment_shader1 =
		"#version 330\n"
		"out vec4 frag_colour;"
		"void main () {"
		"  frag_colour = vec4(1.0, 0.0, 0.0, 1.0);"
		"}";


	const char* fragment_shader2 =
		"#version 330\n"
		"out vec4 frag_colour;"
		"void main () {"
		"  frag_colour = vec4(0.0, 1.0, 0.0, 1.0);"
		"}";


	const char* fragment_shader3 =
		"#version 330\n"
		"out vec4 frag_colour;"
		"void main () {"
		"  frag_colour = vec4(0.0, 0.0, 1.0, 1.0);"
		"}";

	GLuint vert_shader, frag_shader1, frag_shader2, frag_shader3;
	GLuint shader_programme1, shader_programme2, shader_programme3;

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Drag&Drop", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenBuffers(11, vbo);
	glGenVertexArrays(11, vao);



	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * M * sizeof(GLfloat), HermiteCurve, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle1, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(vao[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle3, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(vao[6]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle4, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(vao[7]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle5, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glBindVertexArray(vao[8]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle6, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glBindVertexArray(vao[9]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle7, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glBindVertexArray(vao[10]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ARRAY_BUFFER, 3 * N * sizeof(GLfloat), circle8, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vertex_shader, NULL);
	glCompileShader(vert_shader);

	frag_shader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader1, 1, &fragment_shader1, NULL);
	glCompileShader(frag_shader1);

	frag_shader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader2, 1, &fragment_shader2, NULL);
	glCompileShader(frag_shader2);

	frag_shader3 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader3, 1, &fragment_shader3, NULL);
	glCompileShader(frag_shader3);

	shader_programme1 = glCreateProgram();
	glAttachShader(shader_programme1, frag_shader1);
	glAttachShader(shader_programme1, vert_shader);
	glLinkProgram(shader_programme1);

	shader_programme2 = glCreateProgram();
	glAttachShader(shader_programme2, frag_shader2);
	glAttachShader(shader_programme2, vert_shader);
	glLinkProgram(shader_programme2);


	shader_programme3 = glCreateProgram();
	glAttachShader(shader_programme3, frag_shader3);
	glAttachShader(shader_programme3, vert_shader);
	glLinkProgram(shader_programme3);

	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	//glPointSize(15.0f);

	while (!glfwWindowShouldClose(window)) {
		recalcHermite();
		

	

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_programme1);

		glBindVertexArray(vao[0]);
		glDrawArrays(GL_POINTS, 0, 8);
		glBindVertexArray(0);


		glUseProgram(shader_programme2);
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_LINE_STRIP, 0, N * M);
		glBindVertexArray(0);


		glUseProgram(shader_programme3);
		glBindVertexArray(vao[2]);
		glDrawArrays(GL_LINES, 0, 8);
		glBindVertexArray(0);


		glUseProgram(shader_programme1);
		glBindVertexArray(vao[3]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, N);
		glBindVertexArray(0);

		glUseProgram(shader_programme1);
		glBindVertexArray(vao[4]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, N);
		glBindVertexArray(0);

		glUseProgram(shader_programme1);
		glBindVertexArray(vao[5]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, N);
		glBindVertexArray(0);

		glUseProgram(shader_programme1);
		glBindVertexArray(vao[6]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, N);
		glBindVertexArray(0);

		glUseProgram(shader_programme1);
		glBindVertexArray(vao[7]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, N);
		glBindVertexArray(0);

		glUseProgram(shader_programme1);
		glBindVertexArray(vao[8]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, N);
		glBindVertexArray(0);

		glUseProgram(shader_programme1);
		glBindVertexArray(vao[9]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, N);
		glBindVertexArray(0);

		glUseProgram(shader_programme1);
		glBindVertexArray(vao[10]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, N);
		glBindVertexArray(0);
		


		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
