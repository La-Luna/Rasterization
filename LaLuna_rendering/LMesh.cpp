#include"stdafx.h"
#include "LMesh.h"
LVert LMesh::getVert(int ID){
	LVert v;
	v.position = mesh_positionlist[ID];
	v.color = mesh_pointscolorlist[ID];
	return v;
}