#include"stdafx.h"
#include "LMesh.h"
LVert LMesh::getVert(int ID){
	LVert v;
	v.position = mesh_positionlist[ID];
	v.color = mesh_pointscolorlist[ID];
	v.texcoord = mesh_texcoordlist[ID];
	v.origin_orth_z = mesh_orth_z[ID];
	v.vertnomal = mesh_normalist[ID];
	return v;
}
LMesh::LMesh(const LMesh& tm){

}
void LMesh::setVert(int i,LVert v){
	mesh_positionlist[i] = v.position;
	mesh_pointscolorlist[i] = v.color;
	mesh_texcoordlist[i] = v.texcoord;
	mesh_orth_z[i] = v.origin_orth_z;
	mesh_normalist[i] = v.vertnomal;
}