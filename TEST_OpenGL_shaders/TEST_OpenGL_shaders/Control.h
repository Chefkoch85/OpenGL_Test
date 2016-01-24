#pragma once

#include "OpenGLUtility.h"

class CControl
{
protected:
	static CControl* m_Instance;

	glm::vec3 m_vPosition;
	float m_HorizontalAngle = 3.14f;
	float m_VerticalAngle = 0.0f;
	float m_InitialFoV = 45.0f;
	float m_FoV = m_InitialFoV;
	float m_MaxFoV = 47.1f;
	float m_ZRotateAngle = 0.0f;


	float m_KeySpeed = 1.0f;
	float m_KeyRotateSpeed = 0.2f;
	float m_MouseSpeed = 0.005f;
	float m_WheelSpeed = 0.02f;

	glm::mat4 m_mProj;
	glm::mat4 m_mView;

	GLFWwindow* m_pWin = nullptr;

	static void onScrollCallback(GLFWwindow* win, double dx, double dy);
	void onScroll(double dx, double dy);

	static glm::vec3 Vec4ToVec3(glm::vec4& vec);
	static glm::vec4 Vec3ToVec4(glm::vec3& vec);

	CControl();
	~CControl();
public:
	static CControl* Instance();

	void Init(glm::vec3 pos, GLFWwindow* win);
	void Update(float deltaTime);

	glm::mat4& GetProj() { return m_mProj; }
	glm::mat4& GetView() { return m_mView; }
};

