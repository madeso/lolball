#include "Quaternion.hpp"
#include "vec3.hpp"
#include "Math.hpp"
#include <cassert>

using namespace math;

		Quaternion::Quaternion(const float p_x, const float p_y, const float p_z, const float p_w) {
			set(p_x, p_y, p_z, p_w);
			normalize();
		}
		Quaternion::Quaternion(const Quaternion& p_other) {
			set(p_other);
		}
		
		Quaternion::Quaternion() {
			setIdentity();
		}
		
		Quaternion::~Quaternion() {
		}
	
#if 0
		void Quaternion::toMatrix(Matrix& p_out) {
			const float matrix[3][3] =
			{
				{1 - 2*m_y*2 - 2*m_z*2,		2*m_x*m_y - 2*m_w*m_z,		2*m_x*m_z + 2*m_w*m_y},
				{2*m_x*m_y + 2*m_w*m_z,		1 - 2*m_x*2 - 2*m_z*2,		2*m_y*m_z - 2*m_w*m_x},
				{2*m_x*m_z - 2*m_w*m_y,		2*m_y*m_z + 2*m_w*m_x,		1 - 2*m_x*2 - 2*m_y*2}
			};
			p_out.set(matrix);
		}
#endif
		
		float Quaternion::length() const {
			return squareRoot( lengthSquared() );
		}
		float Quaternion::lengthSquared() const {
			return square(m_x) + square(m_y) + square(m_z) + square(m_w);
		}
		bool Quaternion::isUnit() const {
			const float ls = lengthSquared();
			return equal(ls, 1.0f);
		}
		
		void Quaternion::setIdentity() {
			m_x = m_y = m_z =0.0f;
			m_w = 1.0f;
		}
		
		void Quaternion::setEuler(const float p_x, const float p_y, const float p_z) {
			const vec3 aAxis((float)sin(p_x/2),	0,					0),
				bAxis(0,					(float)sin(p_y/2),	0),
				cAxis(0,					0,					(float)sin(p_z/2));
			const float aAngle = (float) cos(p_x/2),
				bAngle = (float) cos(p_y/2),
				cAngle = (float) cos(p_z/2);
			const Quaternion b(bAxis, bAngle), c(cAxis, cAngle);
			
			setRotation(aAxis,aAngle);
			(*this) *= b;
			(*this) *= c;
			normalize();
		}
		
		void Quaternion::conjugate(){
			set( -m_x, -m_y, -m_z, m_w );
		}
		Quaternion Quaternion::getConjugate() const {
			Quaternion temp(*this);
			temp.conjugate();
			return temp;
		}

		const Quaternion& Quaternion::operator*=(const Quaternion& p_quat) {
			vec3 v1(m_x, m_y, m_z);
			vec3 v2(p_quat.m_x, p_quat.m_y, p_quat.m_z);
			float sc1 = m_w;
			float sc2 = p_quat.m_w;
			
			float scalar = sc1*sc2 - (v1 dot v2);
			vec3 result = v2*sc1 + v1*sc2 + (v1 cross v2);

			m_x = result.getX();
			m_y = result.getY();
			m_z = result.getZ();
			m_w = scalar;

			normalize();
			return *this;
		}
		Quaternion Quaternion::operator*(const Quaternion& b) const {
			Quaternion temp(*this);
			temp *= b;
			return temp;
		}
		
		void Quaternion::set(const float p_x, const float p_y, const float p_z, const float p_w) {
			m_x = p_x;
			m_y = p_y;
			m_z = p_z;
			m_w = p_w;
			normalize();
		}
		void Quaternion::set(const Quaternion& p_quat) {
			set(p_quat.m_x, p_quat.m_y, p_quat.m_z, p_quat.m_w);
		}
		
		Quaternion::Quaternion(const vec3& angle, const float theta) {
			setRotation(angle, theta);
		}
		
		void Quaternion::setRotation(const vec3& pAxis, const float theta) {
			const float halfTheta = theta/2;
			const float sinHalfTheta = (float) sin(halfTheta);
			const float cosHalfTheta = (float) cos(halfTheta);
			
			const vec3 axis = pAxis.getNormalized();
			m_x = axis.getX() * sinHalfTheta;
			m_y = axis.getY() * sinHalfTheta;
			m_z = axis.getZ() * sinHalfTheta;
			m_w = cosHalfTheta;
			normalize();
		}
		
		/*void Quaternion::toAxisAngle(vec3* axis, float* theta) const {
			assert(axis);
			assert(theta);
			assert( equal(1.0f, length()));
			//normalize();
			const float scale = squareRoot(square(m_x) + square(m_y) + square(m_z));
			// const float scale = (float) sin(angle);
			
			if( equal(scale, 0.0f) ){
				*theta = 0.0f;
				*axis = vec3(0.0f, 0.0f, 1.0f);
			}
			else {
				const float halfAngle = (float) acos(m_w);
				assert(0<=halfAngle);
				assert(PI >= halfAngle);
				const float angle = 2.0f * halfAngle;
				*axis = vec3(	m_x / scale, 
								m_y / scale,
								m_z / scale );
				axis->normalize();
				*theta = angle;
			}
		}*/
		void Quaternion::toAxisAngle(vec3* axis, float* theta) const {
			assert(axis);
			assert(theta);
			const float len = length();
			assert( equal(1.0f, length()) ); // normalised
			const float cosAngle = m_w;
			const float angle = acos(cosAngle) * 2 ;
			float sinAngle = squareRoot(1 - square(cosAngle));
			if( abs(sinAngle) < 0.0005f ) {
				sinAngle = 1;
			}
			*axis = vec3( m_x / sinAngle,
				          m_y / sinAngle,
						  m_z / sinAngle);//.getNormalized();
			*theta = angle;
			const float l = axis->getLength();
		}
		
		float Quaternion::normalize() {
			float l = length();
			m_x /= l;
			m_y /= l;
			m_z /= l;
			m_w /= l;
			return l;
		}
		
		Quaternion::Quaternion(const vec3& vec) {
			set(vec.getX(), vec.getY(), vec.getZ(), 0.0f);
		}

		/*
		// optimized a little 2006-02-14
		vec3 Quaternion::rotateVectorAroundOrigin(const vec3& v) const {
			Quaternion q1 = getConjugate();
			Quaternion q2(v);
			//Quaternion q3(*this); 
			q2 *= (*this); // previously q3
			q1 *= q2;
			vec3 rv(q1.m_x, q1.m_y, q1.m_z);
			rv.normalize();
			return rv;
		}
		*/

		// optimized a little 2006-02-14
		vec3 Quaternion::rotateVectorAroundOrigin(const vec3& vec) const {
			const Quaternion point(vec);
			const Quaternion result = (*this) * point * getConjugate();
			return vec3(result.m_x, result.m_y, result.m_z);
		}

		vec3 Quaternion::getIn() const {
			return rotateVectorAroundOrigin(op::vec3::zAxisNegative);
		}
		vec3 Quaternion::getUp() const {
			return rotateVectorAroundOrigin(op::vec3::yAxisPositive);
		}
		vec3 Quaternion::getRight() const {
			return rotateVectorAroundOrigin(op::vec3::xAxisPositive);
		}

		const float Quaternion::getX() const {
			return m_x;
		}
		const float Quaternion::getY() const {
			return m_y;
		}
		const float Quaternion::getZ() const {
			return m_z;
		}
		const float Quaternion::getW() const {
			return m_w;
		}

		bool Quaternion::operator==(const Quaternion& pOther) const {
			return math::equal(getX(), pOther.getX()) && 
				math::equal(getY(), pOther.getY()) &&
				math::equal(getZ(), pOther.getZ()) &&
				math::equal(getW(), pOther.getW());
		}
		bool Quaternion::operator!=(const Quaternion& pOther) const {
			return !(*this == pOther);
		}

		void Quaternion::lookAt(const vec3& pFrom, const vec3& pTo, const vec3& pUp) {
			assert(pUp.isUnit());
			lookInDirection((pTo-pFrom).getNormalized(), pUp);
		}
		void Quaternion::lookInDirection(const vec3& pDirection, const vec3& pUp) {
			assert(pDirection.isUnit());
			assert(pUp.isUnit());
			const vec3 v = (pDirection cross pUp).getNormalized();
			const vec3 u = v cross pDirection;
			const vec3 n = -pDirection;
#define vecAsArray(v) {v.getX(), v.getY(), v.getZ()}
			const float mat[3][3] = { vecAsArray(v), vecAsArray(u), vecAsArray(n) };
			fromMatrix3(mat);
		}

		// this code is grabbed from somwhere
		// rework with my own code from mathfaq
		void Quaternion::fromMatrix3(const float mat[3][3])
		{
		  int   NXT[] = {1, 2, 0};
		  float q[4];

		  // check the diagonal
		  float tr = mat[0][0] + mat[1][1] + mat[2][2];
		  if( tr > 0.0f)
		  {
			float s = (float)squareRoot(tr + 1.0f);
			m_w = s * 0.5f;
			s = 0.5f / s;
			m_x = (mat[1][2] - mat[2][1]) * s;
			m_y = (mat[2][0] - mat[0][2]) * s;
			m_z = (mat[0][1] - mat[1][0]) * s;
		  }
		  else
		  {
			// diagonal is negative
			// get biggest diagonal element
			int i = 0;
			if (mat[1][1] > mat[0][0]) i = 1;
			if (mat[2][2] > mat[i][i]) i = 2;
			//setup index sequence
			int j = NXT[i];
			int k = NXT[j];

			float s = (float)squareRoot((mat[i][i] - (mat[j][j] + mat[k][k])) + 1.0f);

			q[i] = s * 0.5f;

			if (s != 0.0f) s = 0.5f / s;

			q[j] = (mat[i][j] + mat[j][i]) * s;
			q[k] = (mat[i][k] + mat[k][i]) * s;
			q[3] = (mat[j][k] - mat[k][j]) * s;

			m_x = q[0];
			m_y = q[1];
			m_z = q[2];
			m_w   = q[3];
		  }
		}

		// from math-faq
		/*void Quaternion::fromMatrix3(const float pMatrix[3][3]) {
			const real trace = pMatrix[0][0] + pMatrix[1][1] + pMatrix[2][2] + 1;
			#define mat(c, r) pMatrix[r][c]
			if( trace>0.0f ) {
				// instant calculation
				const float s = 0.5f / squareRoot(trace);
				m_w = 0.25f / s;
				m_x = (mat(2, 1) - mat(1, 2)) * s;
				m_y = (mat(0, 2) - mat(2, 0)) * s;
				m_z = (mat(1, 0) - mat(0, 1)) * s;
			}
			else {
				// diagonal is negative, identify the largest colum digital value
				int col = 0;
#define TEST_MATRIX(INDEX) if( pMatrix[col][col] < pMatrix[INDEX][INDEX]) col = INDEX
				TEST_MATRIX(1);
				TEST_MATRIX(2);
#undef TEST_MATRIX
				switch(col) {
					case 0:
						{
							const real s  = squareRoot( 1.0f + pMatrix[0][0] - pMatrix[1][1] - pMatrix[2][2] ) * 2;
							const real oneOverS = 1.0f / s;
							m_x = 0.5f * oneOverS;
							m_y = (mat(0, 1) + mat(1, 0) )  * oneOverS;
							m_z = (mat(0, 2) + mat(2, 0) )  * oneOverS;
							m_w = (mat(1, 2) + mat(2, 1) )  * oneOverS;
						}
						break;
					case 1:
						{
						// something is wrong here
							const real s  = squareRoot( 1.0f + pMatrix[1][1] - pMatrix[0][0] - pMatrix[2][2] ) * 2;
							const real oneOverS = 1.0f / s;
							m_x = (mat(0, 1) + mat(1, 0) )  * oneOverS;
							m_y = 0.5f * oneOverS;
							m_z = (mat(1, 2) + mat(2, 1) )  * oneOverS;
							m_w = (mat(2, 0) + mat(0, 2) )  * oneOverS;
						}
						break;
					case 2:
						{
							const real s  = squareRoot( 1.0f + pMatrix[2][2] - pMatrix[0][0] - pMatrix[1][1] ) * 2;
							const real oneOverS = 1.0f / s;
							m_x = (mat(0, 2) + mat(2, 0) )  * oneOverS;
							m_y = (mat(1, 2) + mat(2, 1) )  * oneOverS;
							m_z = 0.5f * oneOverS;
							m_w = (mat(0, 1) + mat(1, 0) )  * oneOverS;
						}
						break;
					default:
						assert(0 && "This shouldn't happen, bad programming code before case");
				}
			}
#undef mat
		}*/


		/*Points in space, the physical things, are normally represented as 3 or 4
    floats. The effect of a rotation on a collection of points can be
    computed from the representation of the rotation, and here matrices seem
    fastest, using three dot products. Using their own product twice,
    quaternions are a bit less efficient. (They are usually converted to
    matrices at the last minute.)
        p2 = q p1 q^(-1)*/

		/*math::vec3 Quaternion::rotateVector(const std::vec3& p_vector) {
			math::vec3 vector;
			q^(-1) = [-V,w]/(V.V+ww)
			q p1 q^(-1);
		}*/

/*		Sequences of rotations can be interpolated, so that the object being
    turned is rotated to specific poses at specific times. This motivated
    Ken Shoemake's early use of quaternions in computer graphics, as
    published in 1985. He used an analog of linear interpolation (sometimes
    called "lerp") that he called "Slerp", and also introduced an analog of
    a piecewise Bezier curve. A few years later in some course notes he
    described another curve variation he called "Squad", which still seems
    to be popular. Later authors have proposed many alternatives.
                            sin (1-t)A      sin tA
        Slerp(q1,q2;t) = q1 ---------- + q2 ------, cos A = q1.q2
                               sin A         sin A

        Squad(q1,a1,b2,q2;t) = Slerp(Slerp(q1,q2;t),
                                     Slerp(a1,b2;t);
                                     2t(1-t))
*/
// http://www.faqs.org/faqs/graphics/algorithms-faq/ 
		
#if 0
		// from minorlogic in gamedev.net
		inline void From2Vec( Quaternion& q, const vector3& from, const vector3& to ) {
			vector3 c = cross(from, to);
			float d = dot(from, to);
			q.set( c.x, c.y, c.z, d + (float)sqrt( from.len_squared()*to.len_squared() ) );
			// here we can take a 180 Deg case , "from" or "to" is 0 length
			// by check that q.w close to 0
			q.normalize(); 
		}
		//or simpler to understand :
		inline void From2Vec( Quaternion& q, const vector3& from, const vector3& to ) {
			vector3 c = cross(from, to);
			float d = dot(from, to);
			q.set( c.x, c.y, c.z, d );
			q.normalize(); // prevent of "from" or "to" is not unit
			q.w += 1.0f; // reducing angle by 2
			q.normalize(); 
		}
#endif

		Quaternion Quaternion::slerp(const Quaternion& p_to, const float p_time) const {
			float cosOmega =	m_x * p_to.m_x + 
								m_y * p_to.m_y + 
								m_z * p_to.m_z + 
								m_w * p_to.m_w ;

			const float sign = (cosOmega<0.0f) ? (-1.0f) : (1.0f);
			cosOmega *= sign;

			float scaleFrom;
			float scaleTo;
			const float DELTA = 0.0001f;
			if( (1.0f-cosOmega) > DELTA ) {
				// regular slerp
				const float omega = acos(cosOmega);
				const float sinOmega = sin(omega);
				scaleFrom = sin((1.0f - p_time) * omega) / sinOmega;
				scaleTo = sin(p_time * omega) / sinOmega;
			}
			else {
				// from and to are close, do a linear interpolation to speed things up a little
				scaleFrom = 1.0f - p_time;
				scaleTo = p_time;
			}
			scaleTo *= sign; // save two multiplications

			return Quaternion(	(scaleFrom * m_x) + (scaleTo * p_to.m_x),
								(scaleFrom * m_y) + (scaleTo * p_to.m_y),
								(scaleFrom * m_z) + (scaleTo * p_to.m_z),
								(scaleFrom * m_w) + (scaleTo * p_to.m_w)
							);
		}
		
#if 0	
		// gamasutra quat article ref:
		// http://www.gamasutra.com/features/19980703/quaternions_01.htm
		QuatSlerp(QUAT * from, QUAT * to, float t, QUAT * res)
		{
			float         to1[4];
			double        omega, cosom, sinom, scale0, scale1;
			
			
			// calc cosine
			cosom = from->x * to->x + from->y * to->y + from->z * to->z
				+ from->w * to->w;
			
			
			// adjust signs (if necessary)
			if ( cosom <0.0 ){
				cosom = -cosom;
				to1[0] = - to->x;
				to1[1] = - to->y;
				to1[2] = - to->z;
				to1[3] = - to->w;
			}
			else {
				to1[0] = to->x;
				to1[1] = to->y;
				to1[2] = to->z;
				to1[3] = to->w;
			}
			
			// calculate coefficients
			if ( (1.0 - cosom) > DELTA ) {
                // standard case (slerp)
                omega = acos(cosom);
                sinom = sin(omega);
                scale0 = sin((1.0 - t) * omega) / sinom;
                scale1 = sin(t * omega) / sinom;
			} else {        
				// "from" and "to" quaternions are very close 
				//  ... so we can do a linear interpolation
                scale0 = 1.0 - t;
                scale1 = t;
			}
			// calculate final values
			res->x = scale0 * from->x + scale1 * to1[0];
			res->y = scale0 * from->y + scale1 * to1[1];
			res->z = scale0 * from->z + scale1 * to1[2];
			res->w = scale0 * from->w + scale1 * to1[3];
		}
#endif