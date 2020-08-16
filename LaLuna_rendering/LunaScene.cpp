#include"stdafx.h"
#include"LunaScene.h"
#include "funcs.h"
//LunaScene* scene = NULL;
LunaScene::~LunaScene(){
	if(m_mesh)delete m_mesh;
	if(transformed_mesh)delete transformed_mesh;
}
void LunaScene::init(LVector4 viewport){

	//calculate the viewport matrix according to the viewport size
	calculateViewportMatrix(viewport);

	//create mesh
	m_mesh = new LMesh();
	transformed_mesh = new LMesh();
	makeSimpleTriangle();

}
void LunaScene::softRasterization(HDC hdc){
	cout << "run into LunaScene::softRasterization(HDC hdc)" << endl;

	*transformed_mesh = *m_mesh;
	//transforn vertixes coordinates to viewport coordinates
	transformVertixes();
	cout << "run out transformVertixes()" << endl;

	//raster the triangle
	//actually the model consists of many triangles,now we only try to render one of them. 
	int nums_triangles = transformed_mesh->mesh_triangleslist.size();
	for (int i = 0; i < nums_triangles; i++){
		LTriangle & IDtri = transformed_mesh->mesh_triangleslist[i];
		int v0ID = IDtri.array[0];
		int v1ID = IDtri.array[1];
		int v2ID = IDtri.array[2];
		LVert v0=transformed_mesh->getVert(v0ID);
		LVert v1=transformed_mesh->getVert(v1ID);
		LVert v2=transformed_mesh->getVert(v2ID);
		fillTriangleSolid(hdc,v0,v1,v2);
	}

}
void LunaScene::fillTriangleSolid(HDC hdc, const LVert& v0, const LVert& v1, const LVert& v2){
	cout << "run into LunaScene::fillTriangleSolid(HDC hdc, const LVert& v0, const LVert& v1, const LVert& v2)" << endl;
		//cull back face


		//judge the triangle:panbottom,pantop,nonpan

	vector<LVert> vlist;
	vlist.push_back(v0);
	vlist.push_back(v1);
	vlist.push_back(v2);
	vector<int> high2low;
	high2low = sortfromHigh2Low(v0.position.b,v1.position.b,v2.position.b);
	int high = high2low[0];
	int middle = high2low[1];
	int low = high2low[2];

	bool is_pan_top_tri = false;
	bool is_pan_bottom_tri = false;
	if (vlist[high].position.b == vlist[middle].position.b)is_pan_top_tri = true;
	if (vlist[middle].position.b == vlist[low].position.b)is_pan_bottom_tri = true;
	if (is_pan_bottom_tri&&is_pan_top_tri){}
	else if (is_pan_bottom_tri&&!is_pan_top_tri){
		if (vlist[middle].position.a == vlist[low].position.a){}
		else if (vlist[middle].position.a < vlist[low].position.a)fillPanBottomTri_solid(hdc, vlist[high], vlist[middle], vlist[low]);
		else fillPanBottomTri_solid(hdc, vlist[high], vlist[low], vlist[middle]);
	}
	else if (is_pan_top_tri&&!is_pan_bottom_tri){
		if (vlist[high].position.a == vlist[middle].color.a){}
		else if (vlist[high].position.a < vlist[middle].position.a)fillPanTopTri_solid(hdc, vlist[high], vlist[low], vlist[middle]);
		else fillPanTopTri_solid(hdc, vlist[middle], vlist[low], vlist[high]);

	}
	else if (!is_pan_bottom_tri&&!is_pan_top_tri){

		LVert middle_2 = calculateMiddle2inTri(vlist[high], vlist[low],vlist[middle].position.b);

		// 3 points in a same line
		if (middle_2.position.a = vlist[middle].position.a){}

		//calculate the last cross points ,and do both fillPanTop() and fillPanBottom() 
		else if (middle_2.position.a < vlist[middle].position.a){
			fillPanBottomTri_solid(hdc, vlist[high], middle_2, vlist[middle]);
			fillPanTopTri_solid(hdc, middle_2, vlist[low], vlist[middle]);
		}
		else{
			fillPanBottomTri_solid(hdc, vlist[high], vlist[middle], middle_2);
			fillPanTopTri_solid(hdc, vlist[middle], vlist[low], middle_2);
		}

	}


}
void LunaScene::fillPanBottomTri_solid(HDC hdc, const LVert&top, const LVert& bottom_left, const LVert& bottom_right){

	float yTop = top.position.b;
	float yBottom = bottom_left.position.b;

	int yTopInt = ceil(yTop - 1.5);
	int yBottomInt = ceil(yBottom - 0.5);


	const double left_dx = ((double)top.position.a - (double)bottom_left.position.a) / ((double)top.position.b - (double)bottom_left.position.b);
	const double right_dx = ((double)bottom_right.position.a - (double)top.position.a) / ((double)top.position.b - (double)top.position.b - (double)bottom_right.position.b);
	double left_x = lineInsertWithHorizontalLine(top.position, bottom_left.position, (double)(yBottom + 0.5));
	double right_x = lineInsertWithHorizontalLine(top.position, bottom_right.position, (double)(yBottom + 0.5));


	for (int i = yBottomInt; i < yTopInt; i++){

		int xLeftInt = ceil(left_x - 0.5);
		int xRightInt = ceil(right_x - 1.5);

		for (int j = xLeftInt; j <= xRightInt; j++){
			//zbuffer test

			//fragment process
			LVector4 cur_color = top.color;
			drawPixel(hdc, j, i, cur_color);

		}
		left_x = left_x + left_dx;
		right_x = right_x - right_dx;

	}


}
void LunaScene::fillPanTopTri_solid(HDC hdc, const LVert&top_left, const LVert& low, const LVert& top_right){
	float yTop = top_left.position.b;
	float tBottom = low.position.b;

	int yTopInt = ceil(yTop - 1.5);
	int yBottomInt = ceil(tBottom - 0.5);

	const double left_dx = ((double)low.position.a - (double)top_left.position.a) / ((double)top_left.position.b - (double)low.position.b);
	const double right_dx = ((double)top_right.position.a - (double)low.position.a) / ((double)top_right.position.b - (double)low.position.b);
	double left_x = lineInsertWithHorizontalLine(top_left.position, low.position, (double)(yTop + 0.5));
	double right_x = lineInsertWithHorizontalLine(top_right.position, low.position, (double)(yTop + 0.5));


	for (int yInt = yTopInt; yInt <= yBottomInt; yInt--){
		int xRightInt = ceil(right_x-1.5);
		int xLeftInt = ceil(left_x - 0.5);
		for (int xInt = xLeftInt; xInt <= xRightInt; xInt++){
			LVector4 cur_color = top_left.color;
			drawPixel(hdc, xInt, yInt, cur_color);
		}

		left_x += left_dx;
		right_x -= right_dx;

	}



}
void LunaScene::drawPixel(HDC hdc, float x, float y, LVector4 color){
	int r_clamp = max(0, min(1, color.a)) * 255;
	int g_clamp = max(0, min(1, color.b)) * 255;
	int b_clamp = max(0, min(1, color.c)) * 255;
	SetPixel(hdc, x, m_viewport.d - y, RGB(r_clamp, g_clamp, b_clamp));
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

	LTriangle tr1;
	tr1.array[0] = 0;
	tr1.array[1] = 1;
	tr1.array[2] = 2;
	//just three simple color value
	t1.setr(1.0); t1.setg(0.0); t1.setb(0.0);
	t2.setr(1.0); t2.setg(0.0); t2.setb(0.0);
	t3.setr(1.0); t3.setg(0.0); t3.setb(0.0);
	temp_triangleColor.push_back(t1);
	temp_triangleColor.push_back(t2);
	temp_triangleColor.push_back(t3);
	m_mesh->mesh_positionlist.clear();
	m_mesh->mesh_pointscolorlist.clear();
	m_mesh->mesh_triangleslist.clear();
	m_mesh->mesh_positionlist = temp_trianglePoints;
	m_mesh->mesh_pointscolorlist = temp_triangleColor;
	m_mesh->mesh_triangleslist.push_back(tr1);
}
//3 float nums

void LunaScene::transformVertixes(){
	cout << "run into LunaScene::transformVertixes()" << endl;
	//use MVP to change the vetices properties


	//use viewport matrix change the vertexes coordinates
	int vertices_num = transformed_mesh->mesh_positionlist.size();
	for (int i = 0; i < vertices_num;i++){
		cout << i << endl;
		//first to NDC

		//and then to viewport coordinates
		transformed_mesh->mesh_positionlist[i] = m_viewportMat*transformed_mesh->mesh_positionlist[i];

	}

}
void LunaScene::calculateViewportMatrix(LVector4 viewport){
	float wb=viewport.a;
	float hb = viewport.b;
	float w = viewport.c;
	float h = viewport.d;
	m_viewport = viewport;
	LMatrix4 temp(
		w / 2.0, 0.0, 0.0, 0.0,//col1
		0, h / 2.0, 0.0, 0.0,//col2
		0.0, 0.0, 0.5, 0.0,//col3
		wb + w / 2.0, hb + h / 2.0, 0.5, 1.0//col4
		);
		m_viewportMat = temp;

}