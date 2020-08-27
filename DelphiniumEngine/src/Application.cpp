// From the include file we included in Properties/Inputs
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str(); // or &source[0] since the beginning of the str
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << 
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
			<< " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;	
	}

	return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{

	// The Strings are meant to be the actual source code
	// We usually read from text files

	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned  int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// We got two different files and now we need to compile and link them

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	// Delete the object files when dealing with Microsoft's compiler. We already have the program stored within the program
	// We don't want to detach shaders because that would delete the shader source, and since it takes a trivial amount of memory
	// Might also be helpful for debugging later
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	return program;
}

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

	std::string vertexShader = 
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = position;\n"
		"}\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n"; 

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);


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