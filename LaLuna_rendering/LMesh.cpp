#include"stdafx.h"
#include "LMesh.h"
LVert LMesh::getVert(int ID){
	LVert v;
	v.position = mesh_positionlist[ID];
	v.color = mesh_pointscolorlist[ID];
	v.model_normal = mesh_model_normallist[ID];
	v.model_position = mesh_model_positionlist[ID];
	v.texcoord = mesh_texcoordlist[ID];
	v.origin_orth_z = mesh_orth_z[ID];
	v.vertnomal = mesh_normalist[ID];
	v.material_ID = mesh_matirialIDlist[ID];
	return v;
}
LMesh::LMesh(const LMesh& tm){

}
void LMesh::setVert(int i,LVert v){
	mesh_positionlist[i] = v.position;
	mesh_pointscolorlist[i] = v.color;
	mesh_model_positionlist[i] = v.model_position;
	mesh_model_normallist[i] = v.model_normal;
	mesh_texcoordlist[i] = v.texcoord;
	mesh_orth_z[i] = v.origin_orth_z;
	mesh_normalist[i] = v.vertnomal;
	mesh_matirialIDlist[i] = v.material_ID;
}