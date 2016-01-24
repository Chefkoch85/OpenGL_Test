#include "Control.h"

CControl* CControl::m_Instance = nullptr;

void CControl::onScrollCallback(GLFWwindow * win, double dx, double dy)
{
	CControl::Instance()->onScroll(dx, dy);
}

void CControl::onScroll(double dx, double dy)
{
	//Cout << "onScroll(" << dx << ", " << dy << ")" << Cnl;
	m_FoV += -m_WheelSpeed * (float)dy;

	if (m_FoV > m_MaxFoV)
		m_FoV = m_MaxFoV;
}

glm::vec3 CControl::Vec4ToVec3(glm::vec4& vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec4 CControl::Vec3ToVec4(glm::vec3& vec)
{
	return glm::vec4(vec.x, vec.y, vec.z, 1.0f);
}

CControl::CControl()
{
}


CControl::~CControl()
{
}

CControl * CControl::Instance()
{
	if (m_Instance)
		return m_Instance;

	m_Instance = new CControl();

	return m_Instance;
}

void CControl::Init(glm::vec3 pos, GLFWwindow* win)
{
	m_vPosition = pos;
	m_pWin = win;
	glfwSetScrollCallback(m_pWin, onScrollCallback);
}

void CControl::Update(float deltaTime)
{
	// mouse input
	double xPos = 0, yPos = 0;
	int winWidth = 0, winHeight = 0;
	glfwGetCursorPos(m_pWin, &xPos, &yPos);
	glfwGetFramebufferSize(m_pWin, &winWidth, &winHeight);
	glfwSetCursorPos(m_pWin, winWidth / 2, winHeight / 2);

	m_HorizontalAngle += m_MouseSpeed * deltaTime * float(winWidth / 2 - (int)xPos);
	m_VerticalAngle += m_MouseSpeed * deltaTime * float(winHeight / 2 - (int)yPos);

	// calculate view direction
	glm::vec3 vDirection(
		std::cosf(m_VerticalAngle) * std::sinf(m_HorizontalAngle),
		std::sinf(m_VerticalAngle),
		std::cosf(m_VerticalAngle) * std::cosf(m_HorizontalAngle)
		);

	// calculate right direction
	glm::vec3 vRight(
		std::sinf(m_HorizontalAngle - 3.14f / 2.0f),
		0,
		std::cosf(m_HorizontalAngle - 3.14f / 2.0f)
		);

	// calculate the up vector
	glm::vec3 vUp = glm::cross(vRight, vDirection);

	// rotate around look direction
	if (glfwGetKey(m_pWin, GLFW_KEY_Q))
	{
		m_ZRotateAngle += m_KeyRotateSpeed * deltaTime;
	}
	else if (glfwGetKey(m_pWin, GLFW_KEY_E))
	{
		m_ZRotateAngle -= m_KeyRotateSpeed * deltaTime;
	}
	
	// correct the up vector with the rotation around the look direction
	vUp = Vec4ToVec3(glm::rotate(glm::mat4(), m_ZRotateAngle, { 0,0,1 }) * Vec3ToVec4(vUp));

	// move position
	if (glfwGetKey(m_pWin, GLFW_KEY_W))
		m_vPosition += vDirection * deltaTime * m_KeySpeed;
	else if (glfwGetKey(m_pWin, GLFW_KEY_S))
		m_vPosition -= vDirection * deltaTime * m_KeySpeed;
	if (glfwGetKey(m_pWin, GLFW_KEY_D))
		m_vPosition += vRight * deltaTime * m_KeySpeed;
	else if (glfwGetKey(m_pWin, GLFW_KEY_A))
		m_vPosition -= vRight * deltaTime * m_KeySpeed;

	// update matricies
	m_mProj = glm::perspective(m_FoV, (float)winWidth / (float)winHeight, 0.1f, 100.0f);

	m_mView = glm::lookAt(m_vPosition, m_vPosition + vDirection, vUp);
}
