#pragma once
#include "precision.h"

namespace cyclone{
	class Vector3{
		public:
			real x;
			real y;
			real z;
		private:
			real pad;
		public:
			Vector3():x(0), y(0), z(0){}
			Vector3(const real x, const real y, const real z):x(x), y(y), z(z){}
			
			void invert(){
				x=-x;
				y=-y;
				z=-z;
			}
			
			real magnitude() const{
				return real_sqrt(x*x+y*y+z*z);
			}

			real squareMagnitude() const{
				return x*x+y*y+z*z;
			}

			void normalize(){
				real l = magnitude();
				if(l>0){
					(*this) *= ((real)1)/l;
				}
			}

			void operator*=(const real value){
				x *= value;
				y *= value;
				z *= value;
			}

			Vector3 operator*(const real value) const{
				return Vector3(x*value, y*value, z*value);
			}

            Vector3 operator/(const real value) const{
                return Vector3(x/value, y/value, z/value);
            }

			void operator+=(const Vector3 &v){
				x += v.x;
				y += v.y;
				z += v.z;
			}

			Vector3 operator+(const Vector3 &v) const{
				return Vector3(x+v.x, y+v.y, z+v.z);
			}

			void operator-=(const Vector3 &v){
				x -= v.x;
				y -= v.y;
				z -= v.z;
			}

			Vector3 operator-(const Vector3 &v) const{
				return Vector3(x-v.x, y-v.y, z-v.z);
			}

			void addScaledVector(const Vector3 &v, const real scale){
				x += v.x*scale;
				y += v.y*scale;
				z += v.z*scale;
			}

			Vector3 componentProduct(const Vector3 &v) const{
				return Vector3(x*v.x, y*v.y, z*v.z);
			}

			// Similar to *= (Sets the result in same vector)
			void componentProductUpdate(const Vector3 &v){
				x *= v.x;
				y *= v.y;
				z *= v.z;
			}

			real scalarProduct(const Vector3 &v) const{
				return x*v.x + y*v.y + z*v.z;
			}

			real operator*(const Vector3 &v) const{
				return x*v.x + y*v.y + z*v.z;
			}

			Vector3 vectorProduct(const Vector3 &vector) const{
				return Vector3(y*vector.z-z*vector.y,
					z*vector.x-x*vector.z,
					x*vector.y-y*vector.x);
			}

			Vector3 operator%(const Vector3 &vector) const{
				return Vector3(y*vector.z-z*vector.y,
					z*vector.x-x*vector.z,
					x*vector.y-y*vector.x);
			}

            void clear(){
                x=y=z=0;
            }
	};
    
    class Matrix3{
        public:
            real data[9];

            Vector3 operator*(const Vector3 &v) const{
                return Vector3(
                    data[0]*v.x + data[1].v.y + data[2]*v.z,
                    data[3]*v.x + data[4].v.y + data[5]*v.z,
                    data[6]*v.x + data[7].v.y + data[8]*v.z
                );
            }

            Matrix3 operator*(const Matrix3 &m) const{
                return Matrix3(
                    data[0]*m.data[0] + data[1]*m.data[3] + data[2]*m.data[6],             
                    data[0]*m.data[1] + data[1]*m.data[4] + data[2]*m.data[7],
                    data[0]*m.data[2] + data[1]*m.data[5] + data[2]*m.data[8],

                    data[3]*m.data[0] + data[4]*m.data[3] + data[5]*m.data[6],             
                    data[3]*m.data[1] + data[4]*m.data[4] + data[5]*m.data[7],
                    data[3]*m.data[2] + data[4]*m.data[5] + data[5]*m.data[8],

                    data[6]*m.data[0] + data[7]*m.data[3] + data[8]*m.data[6],             
                    data[6]*m.data[1] + data[7]*m.data[4] + data[8]*m.data[7],
                    data[6]*m.data[2] + data[7]*m.data[5] + data[8]*m.data[8]
                );
            }

            // void setInverse(const Matrix3 &m) {
            //     real t0 = (m.data[4]*m.data[8] - m.data[5]*m.data[7]);
            //     real t3 = -(m.data[3]*m.data[8] - m.data[5]*m.data[6]);
            //     real t6 = (m.data[3]*m.data[7] - m.data[4]*m.data[6]);

            //     real det = m.data[0]*t0 + m.data[1]*t3 + m.data[2]*t6;

            //     if(det == (real)0.0f){
            //         return;
            //     }

            //     real invDet = 1.0f / det;

            //     data[0] =  invDet * t0;
            //     data[1] =  invDet * (m.data[2]*m.data[7] - m.data[1]*m.data[8]);
            //     data[2] =  invDet * (m.data[1]*m.data[5] - m.data[2]*m.data[4]);

            //     data[3] =  invDet * t3;
            //     data[4] =  invDet * (m.data[0]*m.data[8] - m.data[2]*m.data[6]);
            //     data[5] =  invDet * (m.data[2]*m.data[3] - m.data[0]*m.data[5]);

            //     data[6] =  invDet * t6;
            //     data[7] =  invDet * (m.data[1]*m.data[6] - m.data[0]*m.data[7]);
            //     data[8] =  invDet * (m.data[0]*m.data[4] - m.data[1]*m.data[3]);
            // }
            
            void setInverse(const Matrix3 &m) {
                real t4 = m.data[0]*m.data[4];
                real t6 = m.data[0]*m.data[5];
                real t8 = m.data[1]*m.data[3];
                real t10 = m.data[2]*m.data[3];
                real t12 = m.data[1]*m.data[6];
                real t14 = m.data[2]*m.data[6];

                real t16 = (t4*m.data[8] - t6*m.data[7] - t8*m.data[8] +
                t10*m.data[7] + t12*m.data[5] - t14*m.data[4]);

                if (t16 == (real)0.0f) return;
                real t17 = 1/t16;
                data[0] = (m.data[4]*m.data[8]-m.data[5]*m.data[7])*t17;
                data[1] = -(m.data[1]*m.data[8]-m.data[2]*m.data[7])*t17;
                data[2] = (m.data[1]*m.data[5]-m.data[2]*m.data[4])*t17;
                data[3] = -(m.data[3]*m.data[8]-m.data[5]*m.data[6])*t17;
                data[4] = (m.data[0]*m.data[8]-t14)*t17;
                data[5] = -(t6-t10)*t17;
                data[6] = (m.data[3]*m.data[7]-m.data[4]*m.data[6])*t17;
                data[7] = -(m.data[0]*m.data[7]-t12)*t17;
                data[8] = (t4-t8)*t17;
            }

            Matrix3 inverse() const{
                Matrix3 result;
                result.setInverse(*this);
                return result;
            }

            void invert(){
                setInverse(*this);
            }

            void setTranspose(const Matrix3 &m){
                data[0] = m.data[0];
                data[1] = m.data[3];
                data[2] = m.data[6];
                data[3] = m.data[1];
                data[4] = m.data[4];
                data[5] = m.data[7];
                data[6] = m.data[2];
                data[7] = m.data[5];
                data[8] = m.data[8];
            }

            Matrix3 transpose() const{
                Matrix3 result;
                result.setTranspose(*this);
                return result;
            }

            void SetOrientation(const Quaternion &q){
                data[0] = 1 - (2*q.j*q.j + 2*q.k*q.k);
                data[1] = 2*q.i*q.j + 2*q.k*q.r;
                data[2] = 2*q.i*q.k - 2*q.j*q.r;
                data[3] = 2*q.i*q.j - 2*q.k*q.r;
                data[4] = 1 - (2*q.i*q.i + 2*q.k*q.k);
                data[5] = 2*q.j*q.k + 2*q.i*q.r;
                data[6] = 2*q.i*q.k + 2*q.j*q.r;
                data[7] = 2*q.j*q.k - 2*q.i*q.r;
                data[8] = 1 - (2*q.i*q.i + 2*q.j*q.j);
            }
    };

    // 3x4 matrix
    class Matrix4{
        public:
            real data[12];

            Vector3 operator*(Vector3 &v) const{
                return Vector3(
                    data[0]*v.x + data[1].v.y + data[2]*v.z + data[3]
                    data[4]*v.x + data[5].v.y + data[6]*v.z + data[7],
                    data[8]*v.x + data[9].v.y + data[10]*v.z + data[11]
                );
            }

            Matrix4 operator*(const Matrix4 &m) const{
                return Matrix4(
                    data[0]*m.data[0] + data[1]*m.data[4] + data[2]*m.data[8], // + data[3]*0;
                    data[0]*m.data[1] + data[1]*m.data[5] + data[2]*m.data[9], // + data[3]*0;
                    data[0]*m.data[2] + data[1]*m.data[6] + data[2]*m.data[10], // + data[3]*0;
                    data[0]*m.data[3] + data[1]*m.data[7] + data[2]*m.data[11] + data[3], // + data[3]*1;

                    data[4]*m.data[0] + data[5]*m.data[4] + data[6]*m.data[8],             
                    data[4]*m.data[1] + data[5]*m.data[5] + data[6]*m.data[9],
                    data[4]*m.data[2] + data[5]*m.data[6] + data[6]*m.data[10],
                    data[4]*m.data[3] + data[5]*m.data[7] + data[6]*m.data[11] + data[7],

                    data[8]*m.data[0] + data[9]*m.data[4] + data[10]*m.data[8],             
                    data[8]*m.data[1] + data[9]*m.data[5] + data[10]*m.data[9],
                    data[8]*m.data[2] + data[9]*m.data[6] + data[10]*m.data[10],
                    data[8]*m.data[3] + data[9]*m.data[7] + data[10]*m.data[11] + data[11],
                );
            }

            real getDeterminant() const;

            void setInverse(const Matrix4 &m);

            Matrix4 inverse() const{
                Matrix4 result;
                result.setInverse(*this);
                return result;
            }

            void invert(){
                setInverse(*this);
            }

            void setOrientationAndPos(const Quaternion &q, const Vector3 &pos){
                data[0] = 1 - (2*q.j*q.j + 2*q.k*q.k);
                data[1] = 2*q.i*q.j + 2*q.k*q.r;
                data[2] = 2*q.i*q.k - 2*q.j*q.r;
                data[3] = pos.x;
                data[4] = 2*q.i*q.j - 2*q.k*q.r;
                data[5] = 1 - (2*q.i*q.i + 2*q.k*q.k);
                data[6] = 2*q.j*q.k + 2*q.i*q.r;
                data[7] = pos.y;
                data[8] = 2*q.i*q.k + 2*q.j*q.r;
                data[9] = 2*q.j*q.k - 2*q.i*q.r;
                data[10] = 1 - (2*q.i*q.i + 2*q.j*q.j);
                data[11] = pos.z;
            }

            Vector3 localToWorld(const Vector3 &local, const Matrix4 &transform){
                return transform.transform(local);
            }

            Vector3 worldToLocal(const Vector3 &world, const Matrix4 &transform){
                return transform.transformInverse(world);
            }

            Vector3 transformInverse(const Vector3 &vector) const
            {
                Vector3 tmp = vector;
                tmp.x -= data[3];
                tmp.y -= data[7];
                tmp.z -= data[11];

                return Vector3(
                    tmp.x * data[0] +
                    tmp.y * data[4] +
                    tmp.z * data[8],
                    tmp.x * data[1] +
                    tmp.y * data[5] +
                    tmp.z * data[9],
                    tmp.x * data[2] +
                    tmp.y * data[6] +
                    tmp.z * data[10]
                );
            }

            Vector3 transformDirection(const Vector3 &vector) const{
                return Vector3(
                    vector.x * data[0] +
                    vector.y * data[1] +
                    vector.z * data[2],
                    vector.x * data[4] +
                    vector.y * data[5] +
                    vector.z * data[6],
                    vector.x * data[8] +
                    vector.y * data[9] +
                    vector.z * data[10]
                );
            }

            Vector3 transformInverseDirection(const Vector3 &vector) const{
                return Vector3(
                    vector.x * data[0] +
                    vector.y * data[4] +
                    vector.z * data[8],
                    vector.x * data[1] +
                    vector.y * data[5] +
                    vector.z * data[9],
                    vector.x * data[2] +
                    vector.y * data[6] +
                    vector.z * data[10]
                );
            }

            Vector3 localToWorldDirn(const Vector3 &local, const Matrix4 &transform){
                return transform.transformDirection(local);
            }

            Vector3 worldToLocalDirn(const Vector3 &world, const Matrix4 &transform){
                return transform.transformInverseDirection(world);
            }
    };

    class Quaternion{
        public:
            union{
                struct{
                    real r;
                    real i;
                    real j;
                    real k;
                };
            }

            void normalize(){
                real d = r*r+i*i+j*j+k*k;

                if(d==0){
                    r = 1;
                    return;
                }

                d = ((real)1.0f)/real_sqrt(d);
                r *= d;
                i *= d;
                j *= d;
                k *= d;
            }
    };
}
