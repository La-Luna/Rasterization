#include"stdafx.h"
#include"LunaScene.h"
#include "LunaGLMath.h"
#include "funcs.h"
//LunaScene* scene = NULL;
LVert vertexShaderPrograme(const LMatrix4& modelMat, const LMatrix4&viewMat, const LMatrix4& projectionMat, const LVert& v){
	LVert temp_v=v;
	//temp_v.position=
	temp_v.position = modelMat*temp_v.position;
	return temp_v;

}
LunaScene::~LunaScene(){
	if(m_mesh)delete m_mesh;
	if(transformed_mesh)delete transformed_mesh;
	if (z_test_buffer)delete[] z_test_buffer;
}
void LunaScene::init(LVector4 viewport){

	//initiate the model matrix and view matrix
	m_modelMat = initModelMatrix();


	//calculate the viewport matrix according to the viewport size
	calculateViewportMatrix(viewport);
	initZBuffer();

	//create mesh
	m_mesh = new LMesh();
	transformed_mesh = new LMesh();
	//makeSimpleTriangle();
	makeSimpleCube();

}
void LunaScene::initZBuffer(){
	int z_buffer_size = (int)m_viewport.c*(int)m_viewport.d;
	z_test_buffer = new double[z_buffer_size];
	//for (int i = 0; i < z_buffer_size; i++){
	//	z_test_buffer[i] = -1;
	//}
}
void LunaScene::softRasterization(HDC hdc){
	//cout << "run into LunaScene::softRasterization(HDC hdc)" << endl;
	//clear zBuffer
	clearZBuffer();

	*transformed_mesh = *m_mesh;
	//transforn vertixes coordinates to viewport coordinates
	transformVertixes();
	//cout << "run out transformVertixes()" << endl;

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
void LunaScene::clearZBuffer(){
	int buffersize = (int)m_viewport.c*(int)m_viewport.d;
	for (int i = 0; i < buffersize; i++){
		z_test_buffer[i] = -1;
	}
}
void LunaScene::fillTriangleSolid(HDC hdc, const LVert& v0, const LVert& v1, const LVert& v2){
	//cout << "run into LunaScene::fillTriangleSolid(HDC hdc, const LVert& v0, const LVert& v1, const LVert& v2)" << endl;
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
		if (middle_2.position.a == vlist[middle].position.a){}

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
	const double right_dx = ((double)bottom_right.position.a - (double)top.position.a) / ((double)top.position.b  - (double)bottom_right.position.b);
	double left_x = lineInsertWithHorizontalLine(top.position, bottom_left.position, (double)(yBottom + 0.5));
	double right_x = lineInsertWithHorizontalLine(top.position, bottom_right.position, (double)(yBottom + 0.5));


	for (int i = yBottomInt; i < yTopInt; i++){

		int xLeftInt = ceil(left_x - 0.5);
		int xRightInt = ceil(right_x - 1.5);

		for (int j = xLeftInt; j <= xRightInt; j++){
			//zbuffer test
			LVector2 pixelCenter = LVector2(j + 0.5, i + 0.5);
			LearlyZOutput earlyZoutput = interpolateInTri_inViewportSpace_Zvalue(top, bottom_left, bottom_right, pixelCenter.a, pixelCenter.b);
			float zvalue_inbuffer = readZBuffer(j,i);
			//cout << " zvalue_inbuffer" << zvalue_inbuffer << "/tearlyZoutput.m_z" << earlyZoutput.m_z << endl;
			if (earlyZoutput.m_z>=zvalue_inbuffer){
			//fragment process
			LVector4 cur_color = top.color;
			drawPixel(hdc, j, i, cur_color);
			writeZBuffer(j, i, earlyZoutput.m_z);
			}

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


	for (int yInt = yTopInt; yInt >= yBottomInt; yInt--){
		int xRightInt = ceil(right_x-1.5);
		int xLeftInt = ceil(left_x - 0.5);
		for (int xInt = xLeftInt; xInt <= xRightInt; xInt++){

			//zbuffer test
			LVector2 pixelCenter = LVector2(xInt + 0.5, yInt + 0.5);
			LearlyZOutput earlyZoutput = interpolateInTri_inViewportSpace_Zvalue(low,top_left, top_right, pixelCenter.a, pixelCenter.b);
			float zvalue_inbuffer = readZBuffer(xInt, yInt);
			if (earlyZoutput.m_z >= zvalue_inbuffer){

				LVector4 cur_color = top_left.color;
				drawPixel(hdc, xInt, yInt, cur_color);
				
				writeZBuffer(xInt, yInt, earlyZoutput.m_z);
			}


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
void LunaScene::makeSimpleCube(){
	//cube mesh;
	vector<LVector4> temp_position_list;
	vector<LVector4> temp_pos_color_list;
	vector<LTriangle> temp_tri_ID_list;
	//front face
	{
		temp_position_list.push_back(LVector4(0.5, 0.5, 0.5, 1)); int vID0 = temp_position_list.size() - 1;//0
		temp_position_list.push_back(LVector4(0.5, -0.5, 0.5, 1)); int vID1 = temp_position_list.size() - 1;//1
		temp_position_list.push_back(LVector4(-0.5, -0.5, 0.5, 1)); int vID2 = temp_position_list.size() - 1;//2
		temp_position_list.push_back(LVector4(-0.5, 0.5, 0.5, 1)); int vID3 = temp_position_list.size() - 1;//3


		temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));
		temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));
		temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));
		temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));

		temp_tri_ID_list.push_back(LTriangle(0, 1, 3));
		temp_tri_ID_list.push_back(LTriangle(1, 2, 3));

	}
	//back face
	{
		temp_position_list.push_back(LVector4(0.5, 0.5, -0.5, 1)); int vID0 = temp_position_list.size() - 1;//4
		temp_position_list.push_back(LVector4(0.5, -0.5, -0.5, 1)); int vID1 = temp_position_list.size() - 1;//5
		temp_position_list.push_back(LVector4(-0.5, -0.5, -0.5, 1)); int vID2 = temp_position_list.size() - 1;//6
		temp_position_list.push_back(LVector4(-0.5, 0.5, -0.5, 1)); int vID3 = temp_position_list.size() - 1;//7


		temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));
		temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));
		temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));
		temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));

		temp_tri_ID_list.push_back(LTriangle(vID0, vID1, vID3));
		temp_tri_ID_list.push_back(LTriangle(vID1,vID2, vID3));

	}
	//right face
	{
		temp_position_list.push_back(LVector4(0.5, 0.5, -0.5, 1)); int vID0 = temp_position_list.size() - 1;//8
		temp_position_list.push_back(LVector4(0.5, -0.5, -0.5, 1)); int vID1 = temp_position_list.size() - 1;//9
		temp_position_list.push_back(LVector4(0.5, -0.5, 0.5, 1)); int vID2 = temp_position_list.size() - 1;//10
		temp_position_list.push_back(LVector4(0.5, 0.5, 0.5, 1)); int vID3 = temp_position_list.size() - 1;//11


		temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));
		temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));
		temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));
		temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));

		temp_tri_ID_list.push_back(LTriangle(vID0, vID1, vID3));
		temp_tri_ID_list.push_back(LTriangle(vID1, vID2, vID3));

	}
	//left face
	{
		temp_position_list.push_back(LVector4(-0.5, 0.5, -0.5, 1)); int vID0 = temp_position_list.size() - 1;//12
		temp_position_list.push_back(LVector4(-0.5, -0.5, -0.5, 1)); int vID1 = temp_position_list.size() - 1;//13
		temp_position_list.push_back(LVector4(-0.5, -0.5, 0.5, 1)); int vID2 = temp_position_list.size() - 1;//14
		temp_position_list.push_back(LVector4(-0.5, 0.5, 0.5, 1)); int vID3 = temp_position_list.size() - 1;//15


		temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));
		temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));
		temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));
		temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));

		temp_tri_ID_list.push_back(LTriangle(vID0, vID1, vID3));
		temp_tri_ID_list.push_back(LTriangle(vID1, vID2, vID3));

	}
	//up face
	{
		temp_position_list.push_back(LVector4(0.5, 0.5, 0.5, 1)); int vID0 = temp_position_list.size() - 1;//16
		temp_position_list.push_back(LVector4(0.5, 0.5, -0.5, 1)); int vID1 = temp_position_list.size() - 1;//17
		temp_position_list.push_back(LVector4(-0.5, 0.5, -0.5, 1)); int vID2 = temp_position_list.size() - 1;//18
		temp_position_list.push_back(LVector4(-0.5, 0.5, 0.5, 1)); int vID3 = temp_position_list.size() - 1;//19


		temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));
		temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));
		temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));
		temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));

		temp_tri_ID_list.push_back(LTriangle(vID0, vID1, vID3));
		temp_tri_ID_list.push_back(LTriangle(vID1, vID2, vID3));

	}
	//down face
	{
		temp_position_list.push_back(LVector4(0.5, -0.5, 0.5, 1)); int vID0 = temp_position_list.size() - 1;//20
		temp_position_list.push_back(LVector4(0.5, -0.5, -0.5, 1)); int vID1 = temp_position_list.size() - 1;//21
		temp_position_list.push_back(LVector4(-0.5, -0.5, -0.5, 1)); int vID2 = temp_position_list.size() - 1;//22
		temp_position_list.push_back(LVector4(-0.5, -0.5, 0.5, 1)); int vID3 = temp_position_list.size() - 1;//23


		temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));
		temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));
		temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));
		temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));

		temp_tri_ID_list.push_back(LTriangle(vID0, vID1, vID3));
		temp_tri_ID_list.push_back(LTriangle(vID1, vID2, vID3));

	}
	m_mesh->mesh_positionlist = temp_position_list;
	m_mesh->mesh_pointscolorlist = temp_pos_color_list;
	m_mesh->mesh_triangleslist = temp_tri_ID_list;


}
LMatrix4 LunaScene::initModelMatrix(){

	LMatrix4 temp_scale_mat = calculateScaleMatrix(0.25, 0.25, 0.25);
	LVector4 temp_trans(0.5, 0, 0, 0);
	LMatrix4 temp_trans_mat = calculateTranslateMatrix(temp_trans);
	LVector4 rotate_axis(0, 1, 0, 1);
	LMatrix4 temp_rotate_mat1 = calculateRotateMatrix(rotate_axis, 0.707, 0.707);//cosA=0.5,sinA=0.86;degree=60;
	LMatrix4 temp_rotate_mat2 = calculateRotateMatrix(LVector4(1, 0, 0, 1), 0.707, 0.707);
	//LMatrix4 temp_model_mat = temp_scale_mat*temp_trans_mat;
	//LMatrix4 temp_model_mat = temp_rotate_mat*temp_model_mat;
	LMatrix4 temp_model_mat;
	temp_model_mat = temp_rotate_mat2*temp_rotate_mat1;
	return temp_model_mat;
	//LMatrix4 temp_mat=calculateRotateMatrix()

}
LMatrix4 LunaScene::initViewMatrix(){
	m_viewportMat
}
LMatrix4 LunaScene::initProjectionMatrix(){

}
void LunaScene::transformVertixes(){
	//cout << "run into LunaScene::transformVertixes()" << endl;
	//run the VertexShaderPrograme (just like in real OpenGL)
	//use MVP to change the vetices properties
	int vertexNum = transformed_mesh->mesh_positionlist.size();
	for (int i = 0; i < vertexNum; i++){
		LVert v = transformed_mesh->getVert(i);
		LVert transformed_v = vertexShaderPrograme(m_modelMat, v);
		//transformed_mesh->mesh_positionlist[i] = transformed_v;
		transformed_mesh->setVert(i, transformed_v);
	}



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
LearlyZOutput LunaScene::interpolateInTri_inViewportSpace_Zvalue(const LVert&v0, const LVert&v1, const LVert&v2, float x, float y){
	const float x0 = v0.position.array[0];
	const float y0 = v0.position.array[1];
	const float z0 = v0.position.array[2];
	const float x1 = v1.position.array[0];
	const float y1 = v1.position.array[1];
	const float z1 = v1.position.array[2];
	const float x2 = v2.position.array[0];
	const float y2 = v2.position.array[1];
	const float z2 = v2.position.array[2];
	const float y1suby0 = y1 - y0;
	const float y2suby0 = y2 - y0;
	const float ysuby0 = y - y0;
	const float x1subx0 = x1 - x0;
	const float x2subx0 = x2 - x0;
	const float xsubx0 = x - x0;
	const float M = y1suby0*x2subx0 - x1subx0*y2suby0;
	float A = 0;
	float B = 0;
	float z = 1;
	if (M == 0){//  p0p1//p0p2, p0p1p2 is degenerated tri, this may not happen, so no matter return what
		//do nothing
	}
	else{
		A = -(y2suby0*xsubx0 - x2subx0*ysuby0) / M;
		B = (y1suby0*xsubx0 - x1subx0*ysuby0) / M;
		//----calculate z
		z = z0 + (z1 - z0)*A + (z2 - z0)*B;

	}
	LearlyZOutput earlyZOutput;
	earlyZOutput.m_A = A;
	earlyZOutput.m_B = B;
	earlyZOutput.m_x = x;
	earlyZOutput.m_y = y;
	earlyZOutput.m_z = z;
	return earlyZOutput;
}
float LunaScene::readZBuffer(int x_pixel, int y_pixel){
	int screen_width = (int)m_viewport.c;
	int screen_height = (int)m_viewport.d;
	if (x_pixel >= 0 && x_pixel < screen_width&&y_pixel >= 0 && y_pixel < screen_height){
		return z_test_buffer[x_pixel*screen_width + y_pixel];
	}
	return -1;//just  a test!
}
void LunaScene::writeZBuffer(int x_pixel, int y_pixel, float z_value){
	int screen_width = (int)m_viewport.c;
	int screen_height = (int)m_viewport.d;
	if (x_pixel >= 0 && x_pixel < screen_width&&y_pixel >= 0 && y_pixel < screen_height){
		z_test_buffer[x_pixel*screen_width + y_pixel]=z_value;
	}

}