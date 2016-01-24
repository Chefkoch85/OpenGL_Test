#pragma once

#include <string>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm.hpp>

typedef std::string CStr;

class CShaderProgram
{
protected:
	GLuint m_ProgramID = 0;
	GLuint m_VertexShaderID = 0;
	GLuint m_FragmentShaderID = 0;

	GLuint m_MVPID = 0;
	GLuint m_AlphaID = 0;
	GLuint m_VertexPosID = 0;
	GLuint m_VertexColID = 0;
	glm::mat4 m_mModel, m_mView, m_mProj;
	glm::mat4 m_mMVP;
	float m_Alpha = 1.0f;

protected:
	GLuint Load(CStr file, GLenum type);
	bool CheckCompilation(GLuint shaderID, CStr& info);
	bool CheckLinking(GLuint programID, CStr& info);

public:
	CShaderProgram();
	~CShaderProgram();

public:
	bool Load(CStr vertexFile, CStr fragmentfile, bool link = true);
	bool Link();
	void Use();
	void Delete();

	void ConectUniforms();

	void SetModelMatrix(glm::mat4 mModel);
	void SetViewMatrix(glm::mat4 mView);
	void SetProjMatrix(glm::mat4 mProj);

	glm::mat4& GetModelMatrix() { return m_mModel; }
	glm::mat4& GetViewMatrix() { return m_mView; }
	glm::mat4& GetProjMatrix() { return m_mProj; }

	void SetMVP(glm::mat4 mvp);
	glm::mat4& GetMVP();

	float GetAlpha() { return m_Alpha; }
	void SetAlpha(float a) { m_Alpha = a; }

	void SetPositionData(GLsizei stride, const GLvoid* data);
	void SetColorData(GLsizei stride, const GLvoid* data);

	void EnableAtrributes();
	void DisableAttributes();

	void AppliyMVP();
};

