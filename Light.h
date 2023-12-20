#pragma once
#include <glm/glm.hpp>
class AbstractLight {
protected:
	glm::vec3 l_ambient;
	glm::vec3 l_diffuse;
	glm::vec3 l_specular;
public:
	AbstractLight() { }
	AbstractLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
		l_ambient(ambient), l_diffuse(diffuse), l_specular(specular) {}
	glm::vec3& GetAmbient() { return l_ambient; }
	void SetAmbient(glm::vec3 amb) { l_ambient = amb; }	
	glm::vec3& GetDiffuse() { return l_diffuse; }
	void SetDiffuse(glm::vec3 dif) { l_diffuse = dif; }
	glm::vec3& GetSpecular() { return l_ambient; }
	void SetSpecular(glm::vec3 spec) { l_specular = spec; }
	static glm::vec4 GetVec4Comp(glm::vec3& param, float dirCom) {
		return glm::vec4{ param.x, param.y, param.z, dirCom };
	}
	virtual ~AbstractLight() {}
};

class DirectionLight : public AbstractLight {
	glm::vec3 l_direction;
public:
	DirectionLight() { }
	DirectionLight(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
		AbstractLight(ambient, diffuse, specular), l_direction(dir) {}
	glm::vec3& GetDirection() { return l_direction; }
	void SetDirection(glm::vec3 dir) { l_direction = dir; }
};

class PointLight : public AbstractLight {
	glm::vec3 l_position;
	float l_attenuation = 1.0f;
	float l_constAttenuation = 1.0f;
	float l_linAttenuation = 0.0f;
	float l_quadAttenuation = 0.0f;
public:
	PointLight(glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
			   float constAtt = 1.0f, float linAtt = 0.0f, float quadAtt = 0.0f ) :
		AbstractLight(ambient, diffuse, specular), 
		l_position(pos),
		l_constAttenuation(constAtt),
		l_linAttenuation(linAtt),
		l_quadAttenuation(quadAtt) {}
	glm::vec3& GetPosition() { return l_position; }
	void SetPosition(glm::vec3 pos) { l_position = pos; }
	float GetLinearAttenuation() const { return l_linAttenuation; }
	void SetLinearAttenuation(float linAtt) { l_linAttenuation = linAtt; }
	float GetConstAttenuation() const { return l_constAttenuation; }
	void SetConstAttenuation(float constAtt) { l_constAttenuation = constAtt; }
	float GetQuadAttenuation() const { return l_quadAttenuation; }
	void SetQuadAttenuation(float quadAtt) { l_quadAttenuation = quadAtt; }
};

class SpotLight : public DirectionLight {
	float l_cutOff = 180.0f;
	float l_exp = 0.0f;
public:
	SpotLight(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutOff = 180.0f, float exp = 0.0f) :
		DirectionLight(dir, ambient, diffuse, specular),
		l_cutOff(cutOff),
		l_exp(exp) {}
	float GetCutOff() const { return l_cutOff; }
	void SetCutOff(float cutOff) { l_cutOff = cutOff; }
	float GetExponent() const { return l_exp; }
	void SetExponent(float exp) { l_exp = exp; }
};