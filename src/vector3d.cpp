#include "vector3d.h"
#include <cmath>
// Constructors
Vector3D::Vector3D(void) { // default constructor
    x=0.0f;	y=0.0f;	z=0.0f;
}
Vector3D::Vector3D(float newx, float newy, float newz) { // (x,y,z) float constructor
    x = newx; y = newy; z = newz;
}

Vector3D::Vector3D(float newx, float newy) { // (x,y) float constructor
    x = newx; y = newy; z = 0;
}

// Vector operator overloads & functions

Vector3D Vector3D::operator + (Vector3D operand) {
    return Vector3D(x + operand.x,y + operand.y,z + operand.z);
}

Vector3D Vector3D::operator - (Vector3D operand) {
    return Vector3D(x - operand.x,y - operand.y,z - operand.z);
}

Vector3D Vector3D::multiply(float t) {
    return Vector3D(x*t,y*t,z*t);
}

Vector3D Vector3D::operator * (Vector3D operand) {
    return Vector3D ((y * operand.z) - (z * operand.y),
        (z * operand.x) - (x * operand.z),
        (x * operand.y) - (y * operand.x));
}

Vector3D Vector3D::getNormalised(void) {
    float len = sqrt((x * x) + (y* y) + (z * z));
    return Vector3D(x/len,y/len,z/len);
}

Vector3D Vector3D::getSquaredNormalised(void) {
    float len = (x * x) + (y* y) + (z * z);
    return Vector3D(x/len,y/len,z/len);
}

float Vector3D::getSquaredLength(void) {
    return  (x * x) + (y * y) + (z * z);
}

float Vector3D::getSquaredDistance(Vector3D operand) {
    return Vector3D(operand.x - x,operand.y - y, operand.z - z).getSquaredLength();
}

float Vector3D::getDotProduct (Vector3D operand) {
    return ((x * operand.x) + (y * operand.y) + (z * operand.z));
}

Vector3D Vector3D :: projectOnto(Vector3D v) {
    return v.multiply(v.getDotProduct(Vector3D(x,y,z)) / v.getSquaredLength()  );
}

