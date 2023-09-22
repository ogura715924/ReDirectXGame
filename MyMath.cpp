#include "MyMath.h"

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
	    v.x * m.m[0][1] + v.y * m.m[1][0] + v.z * m.m[2][0],
	    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
	    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};
	return result;
}
float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

float Length(const Vector3& v) { return sqrtf(Dot(v, v)); }



Vector3 Multiply(float scalar, const Vector3& v) {
	    return {scalar * v.x, scalar * v.y, scalar * v.z};
}

Matrix4x4 Multiply(const Matrix4x4& matrix, const Matrix4x4& matrixa) {
	    Matrix4x4 result{};
	    for (int i = 0; i < 4; ++i) {
		    for (int j = 0; j < 4; ++j) {
			    result.m[i][j] = 0.0f;
			    for (int k = 0; k < 4; ++k) {
				    result.m[i][j] += matrix.m[i][k] * matrixa.m[k][j];
			    }
		    }
	    }
	    return result;
}

Matrix4x4 MakeRotstrXMatrix(float radian) {
	    Matrix4x4 resultX{};
	    resultX.m[0][0] = 1.0f;
	    resultX.m[0][1] = 0.0f;
	    resultX.m[0][2] = 0.0f;
	    resultX.m[0][3] = 0.0f;

	    resultX.m[1][0] = 0.0f;
	    resultX.m[1][1] = cosf(radian);
	    resultX.m[1][2] = sinf(radian);
	    resultX.m[1][3] = 0.0f;

	    resultX.m[2][0] = 0.0f;
	    resultX.m[2][1] = -sinf(radian);
	    resultX.m[2][2] = cosf(radian);
	    resultX.m[2][3] = 0.0f;

	    resultX.m[3][0] = 0.0f;
	    resultX.m[3][1] = 0.0f;
	    resultX.m[3][2] = 0.0f;
	    resultX.m[3][3] = 1.0f;

	    return resultX;
}

Matrix4x4 MakeRotstrYMatrix(float radian) {
	    Matrix4x4 resultY{};
	    resultY.m[0][0] = cosf(radian);
	    resultY.m[0][1] = 0.0f;
	    resultY.m[0][2] = -sinf(radian);
	    resultY.m[0][3] = 0.0f;

	    resultY.m[1][0] = 0.0f;
	    resultY.m[1][1] = 1.0f;
	    resultY.m[1][2] = 0.0f;
	    resultY.m[1][3] = 0.0f;

	    resultY.m[2][0] = sinf(radian);
	    resultY.m[2][1] = 0.0f;
	    resultY.m[2][2] = cosf(radian);
	    resultY.m[2][3] = 0.0f;

	    resultY.m[3][0] = 0.0f;
	    resultY.m[3][1] = 0.0f;
	    resultY.m[3][2] = 0.0f;
	    resultY.m[3][3] = 1.0f;

	    return resultY;
};
Matrix4x4 MakeRotstrZMatrix(float radian) {
	    Matrix4x4 resultZ{};
	    resultZ.m[0][0] = cosf(radian);
	    resultZ.m[0][1] = sinf(radian);
	    resultZ.m[0][2] = 0.0f;
	    resultZ.m[0][3] = 0.0f;

	    resultZ.m[1][0] = -sinf(radian);
	    resultZ.m[1][1] = cosf(radian);
	    resultZ.m[1][2] = 0.0f;
	    resultZ.m[1][3] = 0.0f;

	    resultZ.m[2][0] = 0.0f;
	    resultZ.m[2][1] = 0.0f;
	    resultZ.m[2][2] = 1.0f;
	    resultZ.m[2][3] = 0.0f;

	    resultZ.m[3][0] = 0.0f;
	    resultZ.m[3][1] = 0.0f;
	    resultZ.m[3][2] = 0.0f;
	    resultZ.m[3][3] = 1.0f;

	    return resultZ;
};


Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	    Matrix4x4 rotateXmatrix = MakeRotstrXMatrix(rotate.x);
	    Matrix4x4 rotateYmatrix = MakeRotstrYMatrix(rotate.y);
	    Matrix4x4 rotateZmatrix = MakeRotstrZMatrix(rotate.z);
	    Matrix4x4 rotateXYZmatrix = Multiply(rotateXmatrix, Multiply(rotateYmatrix, rotateZmatrix));

	    Matrix4x4 result{};

	    result.m[0][0] = scale.x * rotateXYZmatrix.m[0][0];
	    result.m[0][1] = scale.x * rotateXYZmatrix.m[0][1];
	    result.m[0][2] = scale.x * rotateXYZmatrix.m[0][2];
	    result.m[0][3] = 0.0f;

	    result.m[1][0] = scale.y * rotateXYZmatrix.m[1][0];
	    result.m[1][1] = scale.y * rotateXYZmatrix.m[1][1];
	    result.m[1][2] = scale.y * rotateXYZmatrix.m[1][2];
	    result.m[1][3] = 0.0f;

	    result.m[2][0] = scale.z * rotateXYZmatrix.m[2][0];
	    result.m[2][1] = scale.z * rotateXYZmatrix.m[2][1];
	    result.m[2][2] = scale.z * rotateXYZmatrix.m[2][2];
	    result.m[2][3] = 0.0f;

	    result.m[3][0] = translate.x;
	    result.m[3][1] = translate.y;
	    result.m[3][2] = translate.z;
	    result.m[3][3] = 1.0f;

	    return result;
}