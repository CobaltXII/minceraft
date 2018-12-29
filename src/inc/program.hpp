#include <iostream>
#include <string>

// Load a shader program from two files. One is a vertex shader, and one is a
// fragment shader. Geometry shader support is not implemented.

GLuint load_program(std::string vertex_path, std::string fragment_path)
{
	GLuint shader_program = glCreateProgram();

	GLuint vertex_shader = load_shader(vertex_path, GL_VERTEX_SHADER);

	GLuint fragment_shader = load_shader(fragment_path, GL_FRAGMENT_SHADER);

	glAttachShader(shader_program, vertex_shader);

	glAttachShader(shader_program, fragment_shader);

	glLinkProgram(shader_program);

	GLint success = 0;

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

	if (!success)
	{
		std::cout << "Could not link shader program loaded from \"" << vertex_path << "\" and \"" << fragment_path << "\"." << std::endl;

		GLchar crash_information[4096];

		glGetProgramInfoLog(shader_program, 4096, NULL, crash_information);

		std::cout << crash_information;

		exit(8);
	}

	glDeleteShader(vertex_shader);

	glDeleteShader(fragment_shader);

	return shader_program;
}