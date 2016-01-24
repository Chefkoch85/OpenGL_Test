#include "ShaderProgram.h"

#include <vector>
#include <iostream>
#include <fstream>


#define Cout std::cout
#define Cnl std::endl

GLuint CShaderProgram::Load(CStr file, GLenum type)
{
	GLuint shaderID = glCreateShader(type);

	// read shader file
	CStr shaderCode = "";
	std::ifstream shaderStream(file, std::ios::in);
	if (shaderStream.is_open())
	{
		CStr line = "";
		while (std::getline(shaderStream, line))
		{
			shaderCode += "\n" + line;
		}
		shaderStream.close();
	}
	else
	{
		return 0;
	}

	// compile shader
	char const * shaderSourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &shaderSourcePointer, nullptr);
	glCompileShader(shaderID);

	return shaderID;
}

bool CShaderProgram::CheckCompilation(GLuint shaderID, CStr & info)
{
	GLint result = GL_FALSE;

	int InfoLogLength = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (!result && InfoLogLength > 0)
	{
		info.resize(InfoLogLength + 1);
		glGetShaderInfoLog(shaderID, InfoLogLength, nullptr, &info[0]);
	}

	return result;
}

bool CShaderProgram::CheckLinking(GLuint programID, CStr & info)
{
	GLint result = GL_FALSE;

	int InfoLogLength = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (!result && InfoLogLength > 0)
	{
		info.resize(InfoLogLength + 1);
		glGetProgramInfoLog(programID, InfoLogLength, nullptr, &info[0]);
	}

	return result;
}

CShaderProgram::CShaderProgram()
{
}


CShaderProgram::~CShaderProgram()
{
}

bool CShaderProgram::Load(CStr vertexFile, CStr fragmentfile, bool link)
{
	CStr info = "";
	
	m_VertexShaderID = Load(vertexFile, GL_VERTEX_SHADER);
	if (!CheckCompilation(m_VertexShaderID, info))
	{
		Cout << "Error Compiling V-Shader:\n" << info << Cnl;
		return false;
	}

	m_FragmentShaderID = Load(fragmentfile, GL_FRAGMENT_SHADER);
	if (!CheckCompilation(m_FragmentShaderID, info))
	{
		Cout << "Error Compiling F-Shader:\n" << info << Cnl;
		return false;
	}

	if (link)
	{
		Link();
	}

	return true;
}

bool CShaderProgram::Link()
{
	CStr info = "";

	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, m_VertexShaderID);
	glAttachShader(m_ProgramID, m_FragmentShaderID);
	glLinkProgram(m_ProgramID);

	if (!CheckLinking(m_ProgramID, info))
	{
		Cout << "Error Linking program:\n" << info << Cnl;

		glDetachShader(m_ProgramID, m_VertexShaderID);
		glDetachShader(m_ProgramID, m_FragmentShaderID);

		glDeleteShader(m_VertexShaderID);
		glDeleteShader(m_FragmentShaderID);

		m_VertexShaderID = 0;
		m_FragmentShaderID = 0;
		m_ProgramID = 0;

		return false;
	}

	glDetachShader(m_ProgramID, m_VertexShaderID);
	glDetachShader(m_ProgramID, m_FragmentShaderID);

	glDeleteShader(m_VertexShaderID);
	glDeleteShader(m_FragmentShaderID);

	m_VertexShaderID = 0;
	m_FragmentShaderID = 0;

	ConectUniforms();

	return true;
}

void CShaderProgram::Use()
{
	glUseProgram(m_ProgramID);
}

void CShaderProgram::Delete()
{
	glDeleteProgram(m_ProgramID);
	m_ProgramID = 0;
}

void CShaderProgram::ConectUniforms()
{
	m_MVPID = glGetUniformLocation(m_ProgramID, "mModelViewProj");
	m_AlphaID = glGetUniformLocation(m_ProgramID, "fAlpha");
	m_VertexPosID = glGetAttribLocation(m_ProgramID, "vertexPosition");
	m_VertexColID = glGetAttribLocation(m_ProgramID, "vertexColor");
}

void CShaderProgram::SetModelMatrix(glm::mat4 mModel)
{
	m_mModel = mModel;
}

void CShaderProgram::SetViewMatrix(glm::mat4 mView)
{
	m_mView = mView;
}

void CShaderProgram::SetProjMatrix(glm::mat4 mProj)
{
	m_mProj = mProj;
}

void CShaderProgram::SetMVP(glm::mat4 mvp)
{
	m_mMVP = mvp;
}

glm::mat4 & CShaderProgram::GetMVP()
{
	return m_mMVP;
}

void CShaderProgram::SetPositionData(GLsizei stride, const GLvoid * data)
{
	glVertexAttribPointer(m_VertexPosID, 3, GL_FLOAT, GL_FALSE, stride, data);
}

void CShaderProgram::SetColorData(GLsizei stride, const GLvoid * data)
{
	glVertexAttribPointer(m_VertexColID, 3, GL_FLOAT, GL_FALSE, stride, data);
}

void CShaderProgram::EnableAtrributes()
{
	glEnableVertexAttribArray(m_VertexPosID);
	glEnableVertexAttribArray(m_VertexColID);
}

void CShaderProgram::DisableAttributes()
{
	glDisableVertexAttribArray(m_VertexPosID);
	glDisableVertexAttribArray(m_VertexColID);
}

void CShaderProgram::AppliyMVP()
{
	m_mMVP = m_mProj * m_mView * m_mModel;
	glUniformMatrix4fv(m_MVPID, 1, GL_FALSE, &m_mMVP[0][0]);
	glUniform1f(m_AlphaID, m_Alpha);
}
