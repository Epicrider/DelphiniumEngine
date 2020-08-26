// From the include file we included in Properties/Inputs
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;



	// Supply GPU with data, which is stored in the memory of the GPU
	// Use a shader to have the GPU draw what's stored in the memory
	// Reads it from the buffer --> Needs to know how it's layed out and where the attributes are

	// We need to tell what the data means
	// what the LAYOUT is
	// Here, contains 3 points
	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	unsigned int buffer; // 0 is usually an invalid state
						// Remember that OpenGL is like a state machine
						// buffer id for an object and it refers to a specific
						// shader, and you call that id when you need to buffer something
						// Can you buffer # pls
	// Selecting which buffer to use to render your triangle
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer); // Kind of like selecting a layer in a Digital Art Program
	// Now we need to provide the buffer with data
	// Can do it later or now since we know what the data is now
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	// 0 refers to the position attribute as the first attribute of a vertex
	// 2 refers to how many floats/objects of type represent this specific attribute
	// GL_FLOAT is the type we're using
	// GL_FALSE is an integers pre-determined by OpenGL that tells it not to normalize the data
	// and the sizeof tells how much space the position takes up
	// and 0 is the distance between two positions in a series of vertexes (because there could be color or something in between)
	// in the same vertex, so we must know how much data is taken up between the two positions
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); // One Attribute in a Vertex (and at the moment, we only have one)

	// We want stuff that is changed in this loop, but if it is not changed
	// We would want to keep it in a buffer and leave it alone
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		// This uses legacy OpenGL that we probably don't want to use
		// in the final version of our program

		/*glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f( 0.0f,  0.5f);
		glVertex2f( 0.5f, -0.5f);
		glEnd();*/

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}