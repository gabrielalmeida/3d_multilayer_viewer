#ifndef VECTOR3D_H
#define VECTOR3D_H
class Vector3D {
public:
    float x,y,z;

    Vector3D();
    Vector3D (float newx, float newy, float newz);
    Vector3D (float newx, float newy);

    Vector3D  operator + (Vector3D operand);
    Vector3D operator - (Vector3D operand);
    Vector3D operator * (Vector3D operand); // Cross Product

    Vector3D getNormalised(void);
    Vector3D getSquaredNormalised(void);
    float getSquaredLength(void);
    float getSquaredDistance(Vector3D operand);
    float getDotProduct(Vector3D operand);
    Vector3D multiply(float t);
    Vector3D projectOnto(Vector3D v);
};

#endif // VECTOR3D_H

