
// Precompiled
#include "ospch.h"
//

#include "Shader.h"

#include <vector>

namespace Ostaga { namespace Assets {

	Shader::Shader()
		: m_ContextID(glCreateProgram())
	{
	}
	Shader::~Shader()
	{
		glDeleteProgram(m_ContextID);
	}

	void Shader::AttachShader(const std::string &source, GLenum type)
	{
		GLuint shader = glCreateShader(type);

		const char *c_source = source.c_str();
		glShaderSource(shader, 1, &c_source, nullptr);
		glCompileShader(shader);

		// Shader compilation check will only be available in debug builds
		//	distribution builds should succeeded shader compilation.
		OSTAGA_IF_DEBUG(
			GLint status = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
				GLint logLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

				std::vector<char> log(logLength);
				glGetShaderInfoLog(shader, logLength, &logLength, log.data());

				glDeleteShader(shader);
				LOG_ERROR("Failed to compile shader: {0}", log.data());
				return;
			}
		)
		//

		glAttachShader(m_ContextID, shader);

	}
	void Shader::LinkProgram()
	{
		glLinkProgram(m_ContextID);

		// Program linkage check will only be available in debug builds
		//	distribution builds should succeeded linking.
		OSTAGA_IF_DEBUG(
			GLint status = 0;
			glGetProgramiv(m_ContextID, GL_LINK_STATUS, &status);
			if (status == GL_FALSE)
			{
				GLint logLength = 0;
				glGetProgramiv(m_ContextID, GL_INFO_LOG_LENGTH, &logLength);

				std::vector<char> log(logLength);
				glGetProgramInfoLog(m_ContextID, logLength, &logLength, log.data());
				glDeleteProgram(m_ContextID);

				LOG_ERROR("Failed to link shader program: {0}", log.data());
				return;
			}
		)
		//
	}
	void Shader::ReleaseShaders()
	{
		GLint shaderCount = 0;
		glGetProgramiv(m_ContextID, GL_ATTACHED_SHADERS, &shaderCount);

		std::vector<GLuint> shaders(shaderCount);
		glGetAttachedShaders(this->m_ContextID, shaderCount, nullptr, shaders.data());

		for (auto shader : shaders)
		{
			// Shader must be detached in order to confirm deletion.
			glDetachShader(m_ContextID, shader);
			glDeleteShader(shader);
		}
	}

	GLint Shader::GetUniformLocation(const std::string &name)
	{
		auto it_location = m_Uniforms.find(name);

		if (it_location != m_Uniforms.end())
			return it_location->second;

		GLint location = glGetUniformLocation(this->m_ContextID, name.c_str());
		if (location != -1)
			m_Uniforms.insert(std::pair(name, location));
		else
			LOG_WARN("Attempting to modify uniform \"{0}\", however, the uniform was never found!", name);
		
		return location;
	}

	static std::string ReadFileAsString(const std::string &filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize((size_t)in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			LOG_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}
	Ref<Shader> Shader::LoadFromFiles(
		const std::string &vertexFile, 
		const std::string &fragmentFile)
	{
		std::string vertexSource = ReadFileAsString(vertexFile);
		std::string fragmentSource = ReadFileAsString(fragmentFile);

		return LoadFromSource(vertexSource, fragmentSource);
	}

	Ref<Shader> Shader::LoadFromFiles(
		const std::string &vertexFile, 
		const std::string &geometryFile, 
		const std::string &fragmentFile)
	{
		std::string vertexSource = ReadFileAsString(vertexFile);
		std::string geometrySource = ReadFileAsString(geometryFile);
		std::string fragmentSource = ReadFileAsString(fragmentFile);

		return LoadFromSource(vertexSource, geometrySource, fragmentSource);
	}

	Ref<Shader> Shader::LoadFromSource(
		const std::string &vertexSource, 
		const std::string &fragmentSource)
	{
		Ref<Shader> shader = CreateRef<Shader>();

		shader->AttachShader(vertexSource, GL_VERTEX_SHADER);
		shader->AttachShader(fragmentSource, GL_FRAGMENT_SHADER);

		shader->LinkProgram();
		shader->ReleaseShaders();

		return shader;
	}

	Ref<Shader> Shader::LoadFromSource(
		const std::string &vertexSource, 
		const std::string &geometrySource, 
		const std::string &fragmentSource)
	{
		Ref<Shader> shader = CreateRef<Shader>();

		shader->AttachShader(vertexSource, GL_VERTEX_SHADER);
		shader->AttachShader(geometrySource, GL_GEOMETRY_SHADER);
		shader->AttachShader(fragmentSource, GL_FRAGMENT_SHADER);

		shader->LinkProgram();
		shader->ReleaseShaders();

		return shader;
	}
} }

