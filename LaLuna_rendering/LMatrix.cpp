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
	LVector4 ans=temp;
	for (int i = 0; i < 4; i++){
		ans.array[i]=matrix[i]*temp.a+matrix[i+4]*temp.b+matrix[i+8]*temp.c+matrix[i+12]*temp.d;
	}
	ans.setr(ans.array[0]);
	ans.setg(ans.array[1]);
	ans.setb(ans.array[2]);
	ans.setalpha(ans.array[3]);

	return ans;
}
LMatrix4 LMatrix4::operator*(const LMatrix4& A)const{

	//try your best to reduce the compute time and occupied room! 
	const float* a = this->matrix;
	const float* b = A.matrix;
	float result[16];
	result[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
	result[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
	result[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
	result[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];

	result[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
	result[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
	result[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
	result[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];

	result[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
	result[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
	result[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
	result[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];

	result[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];
	result[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];
	result[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
	result[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];

	return LMatrix4(result);

}