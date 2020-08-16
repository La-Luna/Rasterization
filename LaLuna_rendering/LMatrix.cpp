#include"stdafx.h"
#include "LMatrix.h"
LMatrix4::LMatrix4(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10, float a11, float a12, float a13, float a14, float a15){

	matrix[0] = a0;
	matrix[1] = a1;
	matrix[2] = a2;
	matrix[3] = a3;
	matrix[4] = a4;
	matrix[5] = a5;
	matrix[6] = a6;
	matrix[7] = a7;
	matrix[8] = a8;
	matrix[9] = a9;
	matrix[10] = a10;
	matrix[11] = a11;
	matrix[12] = a12;
	matrix[13] = a13;
	matrix[14] = a14;
	matrix[15] = a15;

}
LMatrix4::LMatrix4(const float array[16]){
	for (int i = 0; i < 16; i++){
		matrix[i] = array[i];
	}
}
LMatrix4::LMatrix4(const LMatrix4& tm){
	for (int i = 0; i < 16; i++){
		matrix[i] = tm.matrix[i];
	}
}
LVector4 LMatrix4::operator*(const LVector4& temp)const{
	LVector4 ans;
	for (int i = 0; i < 4; i++){
		ans.array[i]=matrix[i]*temp.a+matrix[i+4]*temp.b+matrix[i+8]*temp.c+matrix[i+12]*temp.d;
	}
	ans.a = ans.array[0];
	ans.b = ans.array[1];
	ans.c = ans.array[2];
	ans.d = ans.array[3];
	return ans;
}