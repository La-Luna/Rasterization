#ifndef _LUNASCENE_H_
#define _LUNASCENE_H_
#include<iostream>
#include"LMesh.h"
#include "LMatrix.h"
#include "LVert.h"

#include"LVector.h"

LVert vertexShaderPrograme(const LMatrix4& modelMat,const LMatrix4&viewMat,const LMatrix4& projectionMat,const LVert& v);


class LunaScene;
extern LunaScene* scene;
class LunaScene{
public:
	LunaScene(){
		m_mesh = NULL;
		transformed_mesh = NULL;

	}
	~LunaScene();
private:
public:
	//mesh
	LMesh* m_mesh;			//save the original mesh datas
	LMesh* transformed_mesh;//save the transformed mesh datas
	
	
	
	//viewport_matrix
	LVector4 m_viewport;
	LMatrix4 m_viewportMat;
	LMatrix4 m_modelMat;
	LMatrix4 m_viewMat;
	LMatrix4 m_projectionMat;
	LMatrix4 initModelMatrix();
	LMatrix4 initViewMatrix();
	LMatrix4 initProjectionMatrix();
	//keep updating:transform matrixis


	//create a viewport matrix


	void calculateViewportMatrix(LVector4 viewport);

	//ztest
	double* z_test_buffer;
	LearlyZOutput interpolateInTri_inViewportSpace_Zvalue(const LVert&v0,const LVert&v1,const LVert&v2,float xp,float yp);
	void initZBuffer();
	void clearZBuffer();
	float readZBuffer(int x_pixel, int y_pixel);
	void writeZBuffer(int x_pixel, int y_pixel, float z_value);
	//draw mesh fucntions
	void init(LVector4 viewport);
	void makeSimpleTriangle();
	void makeSimpleCube();
	void softRasterization(HDC hdc);
	void fillTriangleSolid(HDC hdc,const LVert& v0,const LVert& v1,const LVert& v2);
	void fillPanBottomTri_solid(HDC hdc, const LVert&v0, const LVert& v1, const LVert& v2);
	void fillPanTopTri_solid(HDC hdc, const LVert& v0, const LVert& v1, const LVert& v2);

	//draw pixel functions
	void drawPixel(HDC hdc, float x, float y, LVector4 color);


	//transform coordinates functions
	//model view projetcion viewport...
	void transformVertixes();

};
#endif

