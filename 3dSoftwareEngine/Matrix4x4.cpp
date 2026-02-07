#include "Matrix4x4.h"


Vec3d Matrix4x4::operator*(const Vec3d& inVector) const
{
    Vec3d outVector;
    outVector.x = inVector.x * matrix[0][0] + inVector.y * matrix[1][0] + inVector.z * matrix[2][0] + inVector.w * matrix[3][0];
    outVector.y = inVector.x * matrix[0][1] + inVector.y * matrix[1][1] + inVector.z * matrix[2][1] + inVector.w * matrix[3][1];
    outVector.z = inVector.x * matrix[0][2] + inVector.y * matrix[1][2] + inVector.z * matrix[2][2] + inVector.w * matrix[3][2];
    outVector.w = inVector.x * matrix[0][3] + inVector.y * matrix[1][3] + inVector.z * matrix[2][3] + inVector.w * matrix[3][3];
    return outVector;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& inMatrix) const
{
    Matrix4x4 outMatrix;
    for (int col = 0; col < 4; col++)
        for (int row = 0; row < 4; row++)
            outMatrix.matrix[row][col] = matrix[row][0] * inMatrix.matrix[0][col] +
                                         matrix[row][1] * inMatrix.matrix[1][col] +
                                         matrix[row][2] * inMatrix.matrix[2][col] +
                                         matrix[row][3] * inMatrix.matrix[3][col];
    return outMatrix;
}

Matrix4x4 Matrix4x4::MakeIdentity()
{
    Matrix4x4 identityMatrix;
    identityMatrix.matrix[0][0] = 1.0f;
    identityMatrix.matrix[1][1] = 1.0f;
    identityMatrix.matrix[2][2] = 1.0f;
    identityMatrix.matrix[3][3] = 1.0f;
    return identityMatrix;
}

Matrix4x4 Matrix4x4::MakeRotationX(float fAngleRad)
{
    Matrix4x4 matrixRotX;
    matrixRotX.matrix[0][0] = 1.0f;
    matrixRotX.matrix[1][1] = cosf(fAngleRad);
    matrixRotX.matrix[1][2] = sinf(fAngleRad);
    matrixRotX.matrix[2][1] = -sinf(fAngleRad);
    matrixRotX.matrix[2][2] = cosf(fAngleRad);
    matrixRotX.matrix[3][3] = 1.0f;
    return matrixRotX;
}

Matrix4x4 Matrix4x4::MakeRotationY(float fAngleRad)
{
    Matrix4x4 matrixRotY;
    matrixRotY.matrix[0][0] = cosf(fAngleRad);
    matrixRotY.matrix[0][2] = sinf(fAngleRad);
    matrixRotY.matrix[2][0] = -sinf(fAngleRad);
    matrixRotY.matrix[1][1] = 1.0f;
    matrixRotY.matrix[2][2] = cosf(fAngleRad);
    matrixRotY.matrix[3][3] = 1.0f;
    return matrixRotY;
}

Matrix4x4 Matrix4x4::MakeRotationZ(float fAngleRad)
{
    Matrix4x4 matrixRotZ;
    matrixRotZ.matrix[0][0] = cosf(fAngleRad);
    matrixRotZ.matrix[0][1] = sinf(fAngleRad);
    matrixRotZ.matrix[1][0] = -sinf(fAngleRad);
    matrixRotZ.matrix[1][1] = cosf(fAngleRad);
    matrixRotZ.matrix[2][2] = 1.0f;
    matrixRotZ.matrix[3][3] = 1.0f;
    return matrixRotZ;
}

Matrix4x4 Matrix4x4::MakeTranslation(float x, float y, float z)
{
    Matrix4x4 matrixTrans;
    matrixTrans.matrix[0][0] = 1.0f;
    matrixTrans.matrix[1][1] = 1.0f;
    matrixTrans.matrix[2][2] = 1.0f;
    matrixTrans.matrix[3][3] = 1.0f;
    matrixTrans.matrix[3][0] = x;
    matrixTrans.matrix[3][1] = y;
    matrixTrans.matrix[3][2] = z;
    return matrixTrans;
}

Matrix4x4 Matrix4x4::MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
{
    float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
    Matrix4x4 matrixProjection;
    matrixProjection.matrix[0][0] = fAspectRatio * fFovRad;
    matrixProjection.matrix[1][1] = fFovRad;
    matrixProjection.matrix[2][2] = fFar / (fFar - fNear);
    matrixProjection.matrix[3][2] = (-fFar * fNear) / (fFar - fNear);
    matrixProjection.matrix[2][3] = 1.0f;
    matrixProjection.matrix[3][3] = 0.0f;
    return matrixProjection;
}

Matrix4x4 Matrix4x4::MakePointAt(const Vec3d& position, const Vec3d& target, const Vec3d& up)
{
    Vec3d newForward = target - position;
    newForward.Normalise();

    Vec3d newUp = up - (newForward * Vec3d::DotProduct(up, newForward));
    newUp.Normalise();

    Vec3d newRight = Vec3d::CrossProduct(newUp, newForward);

    Matrix4x4 matPointAt;
    matPointAt.matrix[0][0] = newRight.x;           matPointAt.matrix[0][1] = newRight.y;           matPointAt.matrix[0][2] = newRight.z;           matPointAt.matrix[0][3] = 0.0f;
    matPointAt.matrix[1][0] = newUp.x;              matPointAt.matrix[1][1] = newUp.y;              matPointAt.matrix[1][2] = newUp.z;              matPointAt.matrix[1][3] = 0.0f;
    matPointAt.matrix[2][0] = newForward.x;         matPointAt.matrix[2][1] = newForward.y;         matPointAt.matrix[2][2] = newForward.z;         matPointAt.matrix[2][3] = 0.0f;
    matPointAt.matrix[3][0] = position.x;           matPointAt.matrix[3][1] = position.y;           matPointAt.matrix[3][2] = position.z;           matPointAt.matrix[3][3] = 1.0f;
    return matPointAt;
}

Matrix4x4 Matrix4x4::MakeQuickInverse(const Matrix4x4& inMatrix)                                // Only for Rotation/Translation Matrices
{
    Matrix4x4 matQuickInv;
    
    matQuickInv.matrix[0][0] = inMatrix.matrix[0][0]; matQuickInv.matrix[0][1] = inMatrix.matrix[1][0]; matQuickInv.matrix[0][2] = inMatrix.matrix[2][0]; matQuickInv.matrix[0][3] = 0.0f;
    matQuickInv.matrix[1][0] = inMatrix.matrix[0][1]; matQuickInv.matrix[1][1] = inMatrix.matrix[1][1]; matQuickInv.matrix[1][2] = inMatrix.matrix[2][1]; matQuickInv.matrix[1][3] = 0.0f;
    matQuickInv.matrix[2][0] = inMatrix.matrix[0][2]; matQuickInv.matrix[2][1] = inMatrix.matrix[1][2]; matQuickInv.matrix[2][2] = inMatrix.matrix[2][2]; matQuickInv.matrix[2][3] = 0.0f;

    matQuickInv.matrix[3][0] = -(inMatrix.matrix[3][0] * matQuickInv.matrix[0][0] + inMatrix.matrix[3][1] * matQuickInv.matrix[1][0] + inMatrix.matrix[3][2] * matQuickInv.matrix[2][0]);
    matQuickInv.matrix[3][1] = -(inMatrix.matrix[3][0] * matQuickInv.matrix[0][1] + inMatrix.matrix[3][1] * matQuickInv.matrix[1][1] + inMatrix.matrix[3][2] * matQuickInv.matrix[2][1]);
    matQuickInv.matrix[3][2] = -(inMatrix.matrix[3][0] * matQuickInv.matrix[0][2] + inMatrix.matrix[3][1] * matQuickInv.matrix[1][2] + inMatrix.matrix[3][2] * matQuickInv.matrix[2][2]);
    matQuickInv.matrix[3][3] = 1.0f;
    
    return matQuickInv;
}   