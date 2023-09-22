#include<Normalize.h>
#include <MyMath.h>


Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	return {v.x / length, v.y / length, v.z / length};
}