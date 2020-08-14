#ifndef _LMESH_H_
#define _LMESH_H_
#include"LVector.h"
#include<vector>
using namespace std;


class LMesh{

public:
	LMesh(){};
	~LMesh();




	vector<LVector4>mesh_positionlist;
	vector<LVector4>mesh_pointscolorlist;

};
#endif
