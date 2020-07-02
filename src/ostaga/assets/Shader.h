#pragma once

#include <Ostaga.h>
#include <Util.h>
#include <Maths.h>

#include <glad/glad.h>

#include <string>
#include <unordered_map>

namespace Ostaga { namespace Assets {

	class Shader
	{
	public:
		Shader();
		~Shader();

		inline void Bind() const { glUseProgram(this->m_ContextID); }
		inline void Unbind() const { glUseProgram(0); }

		inline void SetUniformFloat(const std::string &name, GLfloat v) const
		{
			glUniform1f(GetUniformLocation(name), v);
		}
		inline void SetUniformFloat2(const std::string &name, const glm::vec2 &v) const
		{
			glUniform2f(GetUniformLocation(name), v.x, v.y);
		}
		inline void SetUniformFloat3(const std::string &name, const glm::vec3 &v) const
		{
			glUniform3f(GetUniformLocation(name), v.x, v.y, v.z);
		}
		inline void SetUniformFloat4(const std::string &name, const glm::vec4 &v) const
		{
			glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w);
		}

		inline void SetUniformInt(const std::string &name, GLint v) const
		{
			glUniform1i(GetUniformLocation(name), v);
		}
		inline void SetUniformInt2(const std::string &name, const glm::ivec2 &v) const
		{
			glUniform2i(GetUniformLocation(name), v.x, v.y);
		}
		inline void SetUniformInt3(const std::string &name, const glm::ivec3 &v) const
		{
			glUniform3i(GetUniformLocation(name), v.x, v.y, v.z);
		}
		inline void SetUniformInt4(const std::string &name, const glm::ivec4 &v) const
		{
			glUniform4i(GetUniformLocation(name), v.x, v.y, v.z, v.w);
		}

		inline void SetUniformIntU(const std::string &name, GLuint v) const
		{
			glUniform1ui(GetUniformLocation(name), v);
		}
		inline void SetUniformInt2U(const std::string &name, const glm::uvec2 &v) const
		{
			glUniform2ui(GetUniformLocation(name), v.x, v.y);
		}
		inline void SetUniformInt3U(const std::string &name, const glm::uvec3 &v) const
		{
			glUniform3ui(GetUniformLocation(name), v.x, v.y, v.z);
		}
		inline void SetUniformInt4U(const std::string &name, const glm::uvec4 &v) const
		{
			glUniform4ui(GetUniformLocation(name), v.x, v.y, v.z, v.w);
		}

		inline void SetUniformBool(const std::string &name, bool v) const
		{
			glUniform1i(GetUniformLocation(name), v);
		}

		inline void SetUniformMat3(const std::string &name, const glm::mat3 &v) const
		{
			glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v));
		}
		inline void SetUniformMat4(const std::string &name, const glm::mat4 &v) const
		{
			glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(v));
		}

		inline void SetUniformFloatV(const std::string &name, GLfloat *v, GLsizei count) const
		{
			glUniform1fv(GetUniformLocation(name), count, v);
		}
		inline void SetUniformFloat2V(const std::string &name, glm::vec2 *v, GLsizei count) const
		{
			glUniform2fv(GetUniformLocation(name), count, glm::value_ptr(*v));
		}
		inline void SetUniformFloat3V(const std::string &name, glm::vec3 *v, GLsizei count) const
		{
			glUniform3fv(GetUniformLocation(name), count, glm::value_ptr(*v));
		}
		inline void SetUniformFloat4V(const std::string &name, glm::vec4 *v, GLsizei count) const
		{
			glUniform4fv(GetUniformLocation(name), count, glm::value_ptr(*v));
		}

		inline void SetUniformIntV(const std::string &name, GLint *v, GLsizei count) const
		{
			glUniform1iv(GetUniformLocation(name), count, v);
		}
		inline void SetUniformInt2V(const std::string &name, glm::ivec2 *v, GLsizei count) const
		{
			glUniform2iv(GetUniformLocation(name), count, glm::value_ptr(*v));
		}
		inline void SetUniformInt3V(const std::string &name, glm::ivec3 *v, GLsizei count) const
		{
			glUniform3iv(GetUniformLocation(name), count, glm::value_ptr(*v));
		}
		inline void SetUniformInt4V(const std::string &name, glm::ivec4 *v, GLsizei count) const
		{
			glUniform4iv(GetUniformLocation(name), count, glm::value_ptr(*v));
		}

		inline void SetUniformIntUV(const std::string &name, GLuint *v, GLsizei count) const
		{
			glUniform1uiv(GetUniformLocation(name), count, v);
		}
		inline void SetUniformInt2UV(const std::string &name, glm::uvec2 *v, GLsizei count) const
		{
			glUniform2uiv(GetUniformLocation(name), count, glm::value_ptr(*v));
		}
		inline void SetUniformInt3UV(const std::string &name, glm::uvec3 *v, GLsizei count) const
		{
			glUniform3uiv(GetUniformLocation(name), count, glm::value_ptr(*v));
		}
		inline void SetUniformInt4UV(const std::string &name, glm::uvec4 *v, GLsizei count) const
		{
			glUniform4uiv(GetUniformLocation(name), count, glm::value_ptr(*v));
		}


	private:
		void AttachShader(const std::string &source, GLenum type);
		void LinkProgram();
		void ReleaseShaders();
		void LoadShaderUniforms();

		GLint GetUniformLocation(const std::string &name) const;

	private:
		GLuint m_ContextID = 0;
		std::unordered_map<std::string, GLint> m_Uniforms;

	public:
		static Ref<Shader> LoadFromFiles(
			const std::string &vertexFile, 
			const std::string &fragmentFile);

		static Ref<Shader> LoadFromFiles(
			const std::string &vertexFile,
			const std::string &geometryFile,
			const std::string &fragmentFile);

		static Ref<Shader> LoadFromSource(
			const std::string &vertexSource,
			const std::string &fragmentSource);

		static Ref<Shader> LoadFromSource(
			const std::string &vertexSource,
			const std::string &geometrySource,
			const std::string &fragmentSource);
	};

} }