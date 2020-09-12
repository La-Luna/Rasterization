#include "stdafx.h"
#include "LunaCamera.h"
#include "funcs.h"
#include <time.h>
#include <iostream>
using namespace std;
#define PI 3.1415926
LunaCamera::LunaCamera(LunaProjectionMode mode){
	m_fov = 45.0;
	m_aspect = 1.0;
	m_near = 0.1;
	m_far = 100;


	//keep rotate

	
	LVector3 tempEyepos( 0, 0,10);
	//LVector3 tempEyepos( 10,0, 0);
	

	LVector3 tempU(1, 0, 0);
	LVector3 tempCentor(0, 0, 0);
	LVector3 tempV(0, 1, 0);
	LVector3 tempN(0, 0, 1);

	//LVector3 tempU(0, 0, -1);
	//LVector3 tempCentor(0, 0, 0);
	//LVector3 tempV(0, 1, 0);
	//LVector3 tempN(1, 0, 0);


		m_eyePos = tempEyepos;
		m_V = tempV;
		m_N = tempN;
		m_U = tempU;
		m_objectCenter = tempCentor;

	initProjectionMatrix(mode);

}
void LunaCamera::initViewMatrix(){

	//r:U,V,N
	LMatrix4 temp_view_mr(
		m_U.array[0], m_V.array[0], m_N.array[0], 0,
		m_U.array[1], m_V.array[1], m_N.array[1], 0,
		m_U.array[2], m_V.array[2], m_N.array[2], 0,
		0, 0, 0, 1
		);

	//t:cameraPos
	LMatrix4 temp_view_mt(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-m_eyePos.array[0], -m_eyePos.array[1], -m_eyePos.array[2], 1
		);
	LMatrix4 temp_viewm = temp_view_mr*temp_view_mt;
	//LMatrix4 temp_viewm ;
	m_ViewMat = temp_viewm;

}
void LunaCamera::initProjectionMatrix(LunaProjectionMode mode){
	//calculate the vector(cameraPos-objectCenter) length
	float object_distance=Vector3Length(m_eyePos,m_objectCenter);
	float width_n, width_see;
	float height_n, height_see;
	float cube_pos_z = 10;			//!!change!!
	height_n = 2 * m_near*tan((m_fov / 2.0)*(PI / 180));
	height_see = height_n*object_distance / m_near;
	if (mode == lunaPerspectivepromode){
		float n = m_near*(-1);
		float f = m_far*(-1);
		float tan_fov_2 = tan((m_fov / 2.0)*(PI / 180));
		//cout << tan_fov_2 << endl; 
		float t11 = 1 / (m_aspect*tan_fov_2);
		float t22 = 1 / tan_fov_2;
		float t33 = (f + n) / (n - f);
		float t34 = -(2 * n * f) / (n - f);
		LMatrix4 temp_proj(
			t11, 0, 0, 0,
			0, t22, 0, 0,
			0, 0, t33, -1,
			0, 0, t34, 0
			);
		m_ProjectionMat=temp_proj;
	}
	else{
		width_see = m_aspect*height_see;
		float n = m_near*(-1);
		float f = m_far*(-1);
		float t11 = 2 / width_see;
		float t22 = 2 / height_see;
		float t33 = 2 / (n - f);
		float t34 = -(n + f) / (n - f);
		LMatrix4 temp_proj(
			t11, 0, 0, 0,
			0, t22, 0, 0,
			0, 0, t33, 0,
			0, 0, t34, 1
			);
		m_ProjectionMat=temp_proj;

	}

}
void LunaCamera::updateVector(){
	//keep rotate
	float radius = 10.0;
	clock_t curtime_t = clock();
	float curtime = curtime_t / 100.0;

	float camX = sin(curtime*(PI/180))*radius;
	float camZ = cos(curtime*(PI/180))*radius;

	LVector3 tempEyepos(camX, 0, camZ);


	LVector3 tempU(1, 0, 0);
	LVector3 tempCentor(0, 0, 0);
	LVector3 tempV(0, 1, 0);
	LVector3 tempN(0, 0, 1);


	m_eyePos = tempEyepos;
	m_N = normalizeVector3( tempEyepos-tempCentor);
	m_U = crossVector3(m_V, m_N);

}
void LunaCamera::updateMatrix(){
	initViewMatrix();
}
void LunaCamera::setm_N(float myawn, float mpitch){
	//if (myawn <= -180)myawn = -180.0;
	//if (myawn >= 180.0)myawn = 180.0;
	//if (mpitch <= -90.0)mpitch = -90.0;
	//if (mpitch >= 90.0)mpitch = 90.0;

	if (myawn <= -90)myawn = -90.0;
	if (myawn >= 90.0)myawn = 90.0;
	if (mpitch <= -45.0)mpitch = -45.0;
	if (mpitch >= 45.0)mpitch = 45.0;
	float tx = cos(mpitch*(PI / 180))*cos(myawn*(PI / 180));
	float ty = sin(mpitch*(PI / 180));
	float tz = cos(mpitch*(PI / 180))*sin(myawn*(PI / 180));

	LVector3 tempN(tx, ty, tz);
	m_N =m_N+ tempN;
	m_N = normalizeVector3(m_N);
	m_U = crossVector3(m_V, m_N);
	m_U = normalizeVector3(m_U);
	m_V = crossVector3(m_N, m_U);
	m_V = normalizeVector3(m_V);
}