#ifndef _LUNASCENE_H_
#define _LUNASCENE_H_
#include<iostream>
#include"LMesh.h"
#include "LMatrix.h"
#include"LVector.h"
class LunaScene;
extern LunaScene* scene;
class LunaScene{
public:
	LunaScene(){}
private:
public:
	//mesh
	LMesh* m_mesh;
	//viewport_matrix
	LMatrix4 m_viewportMat;
	//keep updating:transform matrixis


	//create a viewport matrix
	LMatrix4 m_viewport;
	void calculateViewportMatrix(LVector4 viewport);
	//draw mesh fucntions
	void init(LVector4 viewport);
	void makeSimpleTriangle();
	void softRasterization();


	//transform coordinates functions
	//model view projetcion viewport...
	void transformVertixes();

};
#endif

