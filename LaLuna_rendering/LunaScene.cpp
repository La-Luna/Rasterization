#include"stdafx.h"
#include"LunaScene.h"
LunaScene* scene = NULL;
void LunaScene::init(LVector4 viewport){

	//calculate the viewport matrix according to the viewport size
	calculateViewportMatrix(viewport);

	//create mesh
	m_mesh = new LMesh();

	makeSimpleTriangle();

}
void LunaScene::softRasterization(){
	//transforn vertixes coordinates to viewport coordinates
	transformVertixes();


	//raster the triangle


}
void LunaScene::makeSimpleTriangle(){
	//set m_mesh
	vector<LVector4>temp_trianglePoints;
	vector<LVector4>temp_triangleColor;
	//just three simple position values
	LVector4 t1(0.0, 0.5, 0.0, 1.0);
	LVector4 t2(0.5, -0.5, 0.0, 1.0);
	LVector4 t3(-0.5, -0.5, 0.0, 1.0);

	temp_trianglePoints.push_back(t1);
	temp_trianglePoints.push_back(t2);
	temp_trianglePoints.push_back(t3);

	//just three simple color value
	t1.setr(1.0); t1.setg(0.0); t1.setb(0.0);
	t2.setr(1.0); t2.setg(0.0); t2.setb(0.0);
	t3.setr(1.0); t3.setg(0.0); t3.setb(0.0);
	temp_triangleColor.push_back(t1);
	temp_triangleColor.push_back(t2);
	temp_triangleColor.push_back(t3);

	m_mesh->mesh_positionlist = temp_trianglePoints;
	m_mesh->mesh_pointscolorlist = temp_triangleColor;
}
void LunaScene::transformVertixes(){
	


	 
	//use viewport matrix change the vertexes coordinates

	//for (){

	//}

}
void LunaScene::calculateViewportMatrix(LVector4 viewport){
	float wb=viewport.a;
	float hb = viewport.b;
	float w = viewport.c;
	float h = viewport.d;
	LMatrix4 temp(
		

		)
	m_viewportMat

}