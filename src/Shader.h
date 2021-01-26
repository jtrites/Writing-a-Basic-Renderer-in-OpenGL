#pragma once

#include <string>

/* Vid#15: (17:40) include hash table (unordered_map)  */
#include <unordered_map>


/*** Vid#15: (9:55) Move struct ShaderProgramSource that allows returning multiple items
	from Application.cpp to Shader.h ***/

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{

private:
	/* save file path */
	std::string m_FilePath;

	/* save attributes */
	unsigned int m_RendererID;

	/* Vid#15:(17:40) caching for UniformLocationCache */
	std::unordered_map<std::string, int> m_UniformLocationCache;


public:

	/* Shadow constructor */
	Shader(const std::string& filepath);

	/* Shadow destructor */
	~Shader();

	/* Shadow Bind method to execute glUseProgram */
	void Bind() const;

	/* Shadow Unbind method */
	void Unbind() const;

	/* Vid#15: (20:00) Add Uniform1f declaration with string& name, float value */
	void SetUniform1f(const std::string& name, float value);

	/* Set Uniform 4f with string refptr to name and 4 floats  */
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	/*** Vid#15: (9:45)  Move ParseShader method declaration to Shader.cpp to parse external Basic.shader file
		returns - struct ShaderProgramSource above which contains two strings (variables)
		note: C++ functions are normally capable of only returning one variable ***/

	ShaderProgramSource ParseShader(const std::string& filepath);

	/*** Vid#15: (9:45) Move CreateShader method declaration with parameters:
		const string pointer vertexShader(actual source code),
		const string pointer fragmentShader (actual source code)
		returns a static int, takes in the actual source code of these two Shader strings ***/

	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	/*** Vid#15: (9:45) Move CompileShader function with parameters:
		unsigned int type (used raw C++ type instead of OpenGL GLuint type to allow other non-OpenGL GPU driver implementations),
		const std::string& source
		returns a static unsigned int, takes in a type and a string ptr reference to a source ***/

	unsigned int CompileShader(unsigned int type, const std::string& source);

	/* Vid#15: (9:30) Declare GetUniformLocation used to retrieve OpenGL Uniform Locations 
		and Refactor defintion in Shader.cpp */

	unsigned int GetUniformLocation(const std::string& name);
};

