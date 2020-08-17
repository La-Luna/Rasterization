#include <iostream>
#include <vector>
#include "assert.h"
#include "LMatrix.h"
using namespace std;


//attention! scale_matrix must be multyplied at the beginning
LMatrix4 calculateTranslateMatrix(const LVector4& delta){
	assert(delta.array[3] == 0);
	float dx = delta.array[0];
	float dy = delta.array[1];
	float dz = delta.array[2];
	LMatrix4 ans(
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		dx,dy,dz,1
		);
	return ans;

};
LMatrix4 calculateScaleMatrix(float kx,float ky,float kz){
	LMatrix4 ans(
		kx, 0, 0, 0,//col 1
		0, ky, 0, 0,//col 2
		0, 0, kz, 0,//col 3
		0, 0, 0, 1  //col 4
		);
		return ans;
}; 
LMatrix4 calculateRotateMatrix(const LVector4& axis1,float cosA,float sinA){//a little bit complicated==
	float rx = axis1.array[0];//0
	float ry = axis1.array[1];//0
	float rz = axis1.array[2];//1
	//cosA=0.86;sinA=0.5
	float _cosA = 1 - cosA;
	LMatrix4 ans(
		cosA + rx*rx*_cosA, rx*ry*_cosA + rz*sinA, rz*rx*_cosA - ry*sinA, 0,
		rx*ry*_cosA - rz*sinA, cosA + ry*ry*_cosA, rz*ry*_cosA + rx*sinA, 0,
		rx*rz*_cosA + ry*sinA, ry*rz*_cosA - rx*sinA, cosA + rz*rz*_cosA, 0,
		0, 0, 0, 1
		);
	return ans;


	////axis must be a unit vector

	////assert(axis.w() == 0);

	////assert(getLength2(axis) == 1);

	////

	//float nx = axis.array[0];

	//float ny = axis.array[1];

	//float nz = axis.array[2];



	//float _cosA = 1 - cosA;

	//float nx_cosA = nx*_cosA;

	//float nz_cosA = nz*_cosA;

	//float nxnx_cosA = nx*nx_cosA;

	//float nxny_cosA = ny*nx_cosA;

	//float nxnz_cosA = nz*nx_cosA;

	//float nyny_cosA = ny*ny*_cosA;

	//float nynz_cosA = ny*nz_cosA;

	//float nznz_cosA = nz*nz_cosA;

	//float nxsinA = nx*sinA;

	//float nzsinA = nz*sinA;

	//float nysinA = ny*sinA;

	//float rotMat[16] = {

	//	nxnx_cosA + cosA, nxny_cosA + nzsinA, nxnz_cosA - nysinA, 0,//col 1

	//	nxny_cosA - nzsinA, nyny_cosA + cosA, nynz_cosA + nxsinA, 0,//col 2

	//	nxnz_cosA + nysinA, nynz_cosA - nxsinA, nznz_cosA + cosA, 0,//col 3

	//	0, 0, 0, 1//col 4

	//};
	//return LMatrix4(rotMat);
};