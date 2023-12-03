#include "ShaderComponent.h"
#include "Debug.h"
#include <fstream>
#include <string.h>

ShaderComponent::ShaderComponent(Component* parent_, const char* vsFilename_, const char* fsFilename_):
	Component(parent_),
	shaderID(0),vertShaderID(0),fragShaderID(0) {
	vsFilename = vsFilename_;
	fsFilename = fsFilename_;
}

ShaderComponent::~ShaderComponent() {
	OnDestroy();
}

bool ShaderComponent::OnCreate() {

	if (isCreated) return true;
	bool status;
	status = CompileAttach();
	if (status == false) return false;
	// Take components in the frag and vert shader
	// and link them. Passing information from the vert to the frag
	status = Link();
	if (status == false) return false;

	SetUniformLocations();

	isCreated = true;
	return true;
}

void ShaderComponent::OnDestroy() {
	glDetachShader(shaderID, fragShaderID);
	glDetachShader(shaderID, vertShaderID);  
	glDeleteShader(fragShaderID);
	glDeleteShader(vertShaderID);
	glDeleteProgram(shaderID);
	isCreated = false;
}



bool ShaderComponent::CompileAttach(){
    GLint status;
	try { 		
		const char* vsText = ReadTextFile(vsFilename);
		const char* fsText = ReadTextFile(fsFilename);
		if (vsText == nullptr || fsText == nullptr) {
			return false;
		}
		/// GL_VERTEX_SHADER and GL_FRAGMENT_SHADER are defined in glew.h
		// This just creates the name, come back as handles (say a 1 and a 2)
		vertShaderID = glCreateShader(GL_VERTEX_SHADER);
		fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		/// Check for errors
		if (vertShaderID == 0 || fragShaderID == 0) {
			std::string errorMsg("Can't create a new shader");
			throw errorMsg;
		}
		// Have to open up the port to the graphics card
		// requires a handle. vsText is already a pointer
		// but GL shader source signature want a character **
		// want the address to the pointer to the charcter array
		// reason it's not neccessary to send the entire document
		// could do it one line at a time, an array of lines of code
		// but we'll just read the whole thing (the 1)
		// and start at the start of the character array (the 0)
		glShaderSource(vertShaderID, 1, &vsText, 0);   
		glShaderSource(fragShaderID, 1, &fsText, 0);
    
		// CPU is telling the GPU to compile it
		glCompileShader(vertShaderID);
		/// Check for errors
		glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &status);
			if(status == 0) {
				// How would you ever figure out what the error was?
				// Scott wrote this stuff if status == 0 (means an error)
				GLsizei errorLogSize = 0;
				GLsizei titleLength;
				std::string errorLog = vsFilename;
				// we're working on the vertex shader, so add word VERT
				errorLog +="\nVERT:\n";
				titleLength = errorLog.length();
				// get information from integer form (the i) as a pointer (the v)
				glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
				// resize to fit title and message
				errorLog.resize( titleLength + errorLogSize );
				// last argument is the pointer to the bit after the title
				glGetShaderInfoLog(vertShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
				// this will get it to show on the console
				throw errorLog;
			}

		glCompileShader(fragShaderID);
		/// Check for errors
		glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &status);
			if(status == 0) {
				GLsizei errorLogSize = 0;
				GLsizei titleLength;
				std::string errorLog = fsFilename;
				errorLog += "\nFrag:\n";
				titleLength = errorLog.length();
				glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
				errorLog.resize( titleLength + errorLogSize );
				glGetShaderInfoLog(fragShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
				throw errorLog;
			}
    
		// we've built the vertex and fragment shader
		// now the actual shader program
		shaderID = glCreateProgram();
		// attach the frag and vert
		glAttachShader(shaderID, fragShaderID);
		glAttachShader(shaderID, vertShaderID);
		if(vsText) delete[] vsText;
		if(fsText) delete[] fsText;

	}catch(std::string error){
		// Catch the exception, put on console
		printf("%s\n",&error[0]);
		Debug::Error(error, __FILE__, __LINE__);
		return false;
	}
	return true;
}


bool ShaderComponent::Link(){
	GLint status;
	try{
			glLinkProgram(shaderID);
			/// Check for errors
			glGetProgramiv(shaderID, GL_LINK_STATUS, &status);
			if(status == 0) {
				GLsizei errorLogSize = 0;
				std::string errorLog;
				glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
				errorLog.resize(errorLogSize);
				glGetProgramInfoLog(shaderID, errorLogSize, &errorLogSize, &errorLog[0]);
				throw errorLog;
			}
		
	}catch(std::string error){
		Debug::Error(error, __FILE__, __LINE__);
		return false;
	}
	return true;
}

// Uniforms like projection, view, light position etc...
// Constant info for the draw run
GLuint ShaderComponent::GetUniformID(std::string name) { 	
	auto id = uniformMap.find(name);
#ifdef _DEBUG
	if (id == uniformMap.end()) {
		///Debug::Error(name + " :No such uniform name", __FILE__, __LINE__);

		// negative 1 is specific for OpenGL
		// as its actually an unsigned integer, it wraps around to the largest possible unsigned integer
		// then it gets ignored in the gl calls in render. Phew!
		return -1;
	}
#endif 
	return id->second;
}

void ShaderComponent::SetUniformLocations(){
	int count;
	GLsizei actualLen;
	GLint size;
	GLenum type;
	char *name;
	int maxUniformListLength;
	unsigned int loc;
	char buffer[512];

	// have to asynchronously ask GPU all the active uniforms
	// in this shader, and get the ID. Can only refer to them
	// by the handle (1, 2, 3, 4 for example)
	std::string activeUniformList = "Uniforms:\n";
	glGetProgramiv(shaderID, GL_ACTIVE_UNIFORMS, &count);
	sprintf_s(buffer,512,"There are %d active Uniforms\n",count);

	/// get the length of the longest named uniform 
	// Then we can allocate to the biggest name
	glGetProgramiv(shaderID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformListLength);

	/// Create a little buffer to hold the uniform's name - old C memory call just for fun 
	name = (char *)malloc(sizeof(char) * maxUniformListLength);

	
	for (int i = 0; i < count; ++i) {
		
		/// Get the name of the ith uniform
		// Now get actual name length
		glGetActiveUniform(shaderID, i, maxUniformListLength, &actualLen, &size, &type, name);
		/// Get the (unsigned int) loc for this uniform
		loc = glGetUniformLocation(shaderID, name);
		// Name of the uniform mapped to a handle ID
		std::string uniformName = name;
		uniformMap[uniformName] = loc;

		// This will write formatted information into a buffer limited to 512 bytes
		// need uniform name, and ID for that name
		sprintf_s(buffer,512,"\"%s\" loc:%d\n", uniformName.c_str() , uniformMap[uniformName]);
		activeUniformList += buffer;
	}
	Debug::Info(activeUniformList, __FILE__, __LINE__);
	free(name);
}

/// Read from a specified file and return a char array from the heap 
/// The memory must be deleted within this class. It may not be the best way 
/// to do this but it is all private and I did delete it! SSF
char* ShaderComponent::ReadTextFile(const char *filename){
	char* buffer = nullptr;
	// reading file in 8 byte bits
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {								/// Open the file
        file.seekg(0, std::ios::end);					/// goto the end of the file
        int bufferSize = (int) file.tellg();			/// Get the length of the file
		if(bufferSize == 0) {							/// If zero, bad file
			std::string errorMsg("Can't read shader file: ");
			std::string str2(filename);
			errorMsg += str2;
			throw errorMsg;								/// Bail out
		}
		// allocate memory. OpenGL requires you to do it like this
		// the very last character on a hard-drive is a
		// EOF end of file character. But it's not the same as a newline
		buffer = new char[(int)(bufferSize + 1)];		/// Need to NULL terminate the array
        file.seekg(0, std::ios::beg);
        file.read(buffer, bufferSize);
		// NULL is defined as zero.
		// the array needs to be NULL terminated
		buffer[bufferSize] = NULL;						/// Add the NULL
		file.close();
    } else { 
		std::string errorMsg  ("Can't open shader file: ");
		errorMsg += filename;
		Debug::Error(errorMsg, __FILE__, __LINE__);
	}
	return buffer;
}

/// Not used, but must be made concrete as it's a component
void ShaderComponent::Update(const float deltaTime) {}
void ShaderComponent::Render() const {}