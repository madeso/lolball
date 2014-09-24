#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "vec3.hpp"

class Quaternion {
public:
	Quaternion(const Quaternion& p_other);
	Quaternion(const float p_x, const float p_y, const float p_z, const float p_w);
	Quaternion(const vec3& axis, const float theta);
	Quaternion(const vec3& vec);
	Quaternion();
	~Quaternion();
	void setEuler(const float p_x, const float p_y, const float p_z);
	void setRotation(const vec3& axis, const float theta);
	float length() const;
	float lengthSquared() const;
	bool isUnit() const;
	// returns length()
	float normalize();
	//void toMatrix(Matrix& p_out);
	void conjugate();
	Quaternion getConjugate() const;
	const Quaternion& operator*=(const Quaternion& p_quat);
	Quaternion operator*(const Quaternion& a) const;
	void set(const float p_x, const float p_y, const float p_z, const float p_w);
	void set(const Quaternion& p_quat);
	void setIdentity();
			
	vec3 rotateVectorAroundOrigin(const vec3& v) const ;

	vec3 getIn() const ;
	vec3 getUp() const ;
	vec3 getRight() const ;
	const float getX() const ;
	const float getY() const ;
	const float getZ() const ;
	const float getW() const ;

	Quaternion slerp(const Quaternion& p_to, const float p_time) const;

	// glRotate(theta, axis.x, axis.y, axis.z)
	void toAxisAngle(vec3* axis, float* theta) const;

	void lookAt(const vec3& pFrom, const vec3& pTo, const vec3& pUp);
	void lookInDirection(const vec3& pDirection, const vec3& pUp);
	void fromMatrix3(const float pMatrix[3][3]);

	bool operator==(const Quaternion& pOther) const;
	bool operator!=(const Quaternion& pOther) const;
private:
	float m_x;
	float m_y;
	float m_z;
	float m_w;
};

#endif
