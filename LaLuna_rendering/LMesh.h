#ifndef _LMESH_H_
#define _LMESH_H_
#include"LVector.h"
#include "LTriangle.h"
#include<vector>
#include "LVert.h"
using namespace std;


class LMesh{

public:
	LMesh(){};
	LMesh(const LMesh& tm);
	~LMesh(){};




	vector<LVector4>mesh_positionlist;
	vector<LVector4>mesh_pointscolorlist;
	vector<LTriangle>mesh_triangleslist;
	vector<LVector2>mesh_texcoordlist;

	//get some values
	LVert getVert(int ID);
	void setVert(int i,LVert v);

};
#endif
