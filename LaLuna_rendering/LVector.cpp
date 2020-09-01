#include"stdafx.h"
#include"LVector.h"
#include <cmath>
#include<iostream>
using namespace std;
LVector2 LVector2::operator* (float k){
	float temp_a = array[0] * k;
	float temp_b = array[1] * k;
	LVector2 temp(temp_a, temp_b);
	return temp;
}
LVector2 LVector2::operator/ (float k){
	 ////k==0ÈçºÎ±¨´í£¿
	float temp_a = array[0] / k;
	float temp_b = array[1] / k;
	LVector2 temp(temp_a, temp_b);
	return temp;

}
LVector2 LVector2::operator+ (LVector2 temp_A){
	float temp_a = a + temp_A.a;
	float temp_b = b + temp_A.b;
	LVector2 temp(temp_a, temp_b);
	return temp;
}
LVector2 LVector2::operator-(LVector2 temp_B){
	float temp_a, temp_b;
	temp_a = a - temp_B.a;
	temp_b = b - temp_B.b;
	LVector2 temp(temp_a, temp_b);
	return temp;
}
LVector3 LVector3::operator-(const LVector3& vector3_B){
	float temp_arr[3];
	for (int i = 0; i < 3; i++){
		temp_arr[i] = array[i] - vector3_B.array[i];
	}
	LVector3 ans(temp_arr);
	return ans;
}
LVector3 LVector3::operator /(float k){
	for (int i = 0; i < 3; i++){
		array[i] /= k;
	}
	return *this;
}
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
	
	w = v.w;
	for (int i = 0; i < 4; i++){
		array[i] = v.array[i];
	}
}
LVector4 LVector4::operator+(LVector4 lv){
	LVector4 temp;
	temp.array[0] = this->array[0] + lv.array[0]; temp.a = temp.array[0];
	temp.array[1] = this->array[1] + lv.array[1]; temp.b = temp.array[1];
	temp.array[2] = this->array[2] + lv.array[2]; temp.c = temp.array[2];
	temp.array[3] = this->array[3] + lv.array[3]; temp.d = temp.array[3];
	return temp;
}
float LVector4::operator*(const LVector4& a){
	float ans=0;
	for (int i = 0; i < 4; i++){
		ans += this->array[i] * a.array[i];
	}
	return ans;
}
LVector4 LVector4::operator*(float k){
	LVector4 temp(*this);
	temp.array[0] *= k; temp.a *= k;
	temp.array[1] *= k; temp.b *= k;
	temp.array[2] *= k; temp.c *= k;
	temp.array[3] *= k; temp.d *= k;

	return temp;

}

LVector4 LVector4::operator/(const float div){
	LVector4 ans = *this;
	float temp[4];
	for (int i = 0; i < 4; i++){
		temp[i] = this->array[i] / div;
	}

	ans.setr(temp[0]);
	ans.setg(temp[1]);
	ans.setb(temp[2]);
	ans.setalpha(temp[3]);
	return ans;


	//float t_a, t_b, t_c, t_d;
	//if (div != 0){
	//	this->array[0] = this->array[0] / div; this->a = this->array[0];
	//	this->array[1] = this->array[1] / div; this->b = this->array[1];
	//	this->array[2] = this->array[2] / div; this->c = this->array[2];
	//	this->array[3] = this->array[3] / div; this->c = this->array[3];
	//}

}