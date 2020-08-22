#include"stdafx.h"
#include"LVector.h"
#include<iostream>
using namespace std;
LVector4::LVector4(float a0, float b0, float c0, float d0){
	a= a0;
	b = b0;
	c = c0;
	d = d0;
	array[0] = a0;
	array[1] = b0;
	array[2] = c0;
	array[3] = d0;
}
LVector4::LVector4(const LVector4& v){
	a = v.a;
	b = v.b;
	c = v.c;
	d = v.d;
	for (int i = 0; i < 4; i++){
		array[i] = v.array[i];
	}
}

float LVector4::operator*(const LVector4& a){
	float ans=0;
	for (int i = 0; i < 4; i++){
		ans += this->array[i] * a.array[i];
	}
	return ans;
}
LVector4 LVector4::operator/(const float div){

	float temp[4];
	for (int i = 0; i < 4; i++){
		temp[i] = this->array[i] / div;
	}
	LVector4 ans(temp[0],temp[1],temp[2],temp[3]);
	return ans;


	//float t_a, t_b, t_c, t_d;
	//if (div != 0){
	//	this->array[0] = this->array[0] / div; this->a = this->array[0];
	//	this->array[1] = this->array[1] / div; this->b = this->array[1];
	//	this->array[2] = this->array[2] / div; this->c = this->array[2];
	//	this->array[3] = this->array[3] / div; this->c = this->array[3];
	//}

}