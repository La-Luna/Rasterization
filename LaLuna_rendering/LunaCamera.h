#ifndef _LUNACAMERA_H_
#define _LUNACAMERA_H_


#include "LVector.h"
#include"LMatrix.h"
#include "myDefine.h"
#include<iostream>
using namespace std;
class LunaCamera{
public:
	LunaCamera(LunaProjectionMode mode);

	LVector3 getEyePos()const { return m_eyePos; }
	float getFov()const { return m_fov; }
	float getNear()const { return m_near; }
	float getFar()const { return m_far; }
	float getAspec()const { return m_aspect; }
	LVector3 getUvector()const { return m_U; }
	LVector3 getVvector()const{ return m_V; }
	LVector3 getNvector()const{ return m_N; }
	LMatrix4 getViewMat()const{ return m_ViewMat; }
	LMatrix4 getProjectionMat()const { return m_ProjectionMat; }
	void setm_N(float myawn,float mpitch);
	void setEyePos(const LVector3&neweyepos){ m_eyePos = neweyepos; }
	void updateVector();
	void updateMatrix();
	void initViewMatrix();
	void initProjectionMatrix(LunaProjectionMode mode);

	//本来this作为指针常量，指针的指向不能修改，但是指针指向的值可以修改
	//成员函数加了const ,这下指针指向的值也不可以修改了

protected:
	LMatrix4 m_ViewMat;
	LMatrix4 m_ProjectionMat;

protected:
	float m_fov;
	float m_aspect;
	float m_near;
	float m_far;
	float m_range;


protected:
	LVector3 m_eyePos;
	LVector3 m_objectCenter;
	LVector3 m_U;
	LVector3 m_V;
	LVector3 m_N;

	LVector3 m_up;
};
#endif // ifndef _LUNACAMERA_H_