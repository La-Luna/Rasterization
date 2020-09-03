#include"stdafx.h"
#include"LunaScene.h"
#include "LunaGLMath.h"
#include "funcs.h"
#include <time.h>
#define PI 3.1415926
//LunaScene* scene = NULL;
//, const LMatrix4&viewMat, const LMatrix4& projectionMat

LVert vertexShaderProgram(const LMatrix4& modelMat, const LMatrix4&viewMat, const LMatrix4& projectionMat,const LVert& v){
	LVert temp_v=v;
	//temp_v.position=
	//float z = temp_v.position.c;
	temp_v.position = modelMat*temp_v.position;
	temp_v.position = viewMat*temp_v.position;
	temp_v.position = projectionMat*temp_v.position;
	//temp_v.position = temp_v.position / z;
	return temp_v;

}
LFrag fragmentShaderProgram( LVert& interpolateV, LTexture* texture,LunaLight* light){
	float ambientStrength = 0.1;
	LVector4 lightColor = LVector4(1, 1, 1,0);
	LVector4 ambient =lightColor*ambientStrength;
	LVector4 temp_vert = normalizeVector4(interpolateV.position);
	LVector4 lightdirection = light->light_pos - temp_vert;
	lightdirection = normalizeVector4(lightdirection);
	 
	float diffusecoso = 0.3*max(dot2vector4(lightdirection, interpolateV.vertnomal),0.0);
 	LVector4 diffuse = lightColor*diffusecoso;


	LFrag tempfrag;
	tempfrag.m_position = interpolateV.position;
	tempfrag.m_color = interpolateV.color + ambient+diffuse;
	//tempfrag.m_color = perComponentProduct(interpolateV.color, texture->getColor(interpolateV.texcoord));
	return tempfrag;
}
LunaScene::~LunaScene(){
	if(m_mesh)delete m_mesh;
	if(transformed_mesh)delete transformed_mesh; 
	if (z_test_buffer)delete[] z_test_buffer;
	for(int i = 0; i < m_texturelist.size(); i++){
		delete m_texturelist[i];
	}m_texturelist.clear();
}
void LunaScene::init(LVector4 viewport){
	//set camera
	m_camera = new LunaCamera;

	//calculate the viewport matrix according to the viewport size
	calculateViewportMatrix(viewport);
	initZBuffer();
	clearZBuffer();

	//create a light
	m_light = new LunaLight;
	m_light->setLightPos(10, 10, 0, 0);

	//create mesh
	m_mesh = new LMesh();
	transformed_mesh = new LMesh();
	//makeSimpleTriangle();
	makeSimpleCube();

}
void LunaScene::initZBuffer(){
	int z_buffer_size = (int)m_viewport.c*(int)m_viewport.d;
	z_test_buffer = new double[z_buffer_size];
	for (int i = 0; i < z_buffer_size; i++){
		z_test_buffer[i] = 10;
	}
}
void LunaScene::softRasterization(HDC hdc){

	cout << "run into LunaScene::softRasterization(HDC hdc)" << endl;
	//clear zBuffer
	clearZBuffer();
	//initiate the model matrix and view matrix
	m_modelMat = initModelMatrix();
	m_camera->updateMatrix(lunaOrthogonalpromode);



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
		fillTriangleSolid(hdc,v0,v1,v2,lunaOrthogonalpromode);
		//cout << "v0:" << v0.position.a << "," << v0.position.b << "," << v0.position.c << "," << v0.position.d<<endl;
		//cout << "v1:" << v1.position.a << "," << v1.position.b << "," << v1.position.c << "," << v1.position.d<<endl;
		//cout << "v2:" << v2.position.a << "," << v2.position.b << "," << v2.position.c << "," << v2.position.d<<endl;
		cout << endl;

	}

}
void LunaScene::clearZBuffer(){
	int buffersize = (int)m_viewport.c*(int)m_viewport.d;
	for (int i = 0; i < buffersize; i++){
		z_test_buffer[i] =10;
	}
}
void LunaScene::fillTriangleSolid(HDC hdc, const LVert& v0, const LVert& v1, const LVert& v2,LunaProjectionMode mode){

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
		else if (vlist[middle].position.a < vlist[low].position.a)fillPanBottomTri_solid(hdc, vlist[high], vlist[middle], vlist[low],mode);
		else fillPanBottomTri_solid(hdc, vlist[high], vlist[low], vlist[middle],mode);
	}
	else if (is_pan_top_tri&&!is_pan_bottom_tri){
		if (vlist[high].position.a == vlist[middle].color.a){}
		else if (vlist[high].position.a < vlist[middle].position.a)fillPanTopTri_solid(hdc, vlist[high], vlist[low], vlist[middle],mode);
		else fillPanTopTri_solid(hdc, vlist[middle], vlist[low], vlist[high],mode);

	}
	else if (!is_pan_bottom_tri&&!is_pan_top_tri){

		LVert middle_2 = calculateMiddle2inTri(vlist[high], vlist[low],vlist[middle].position.b);
		middle_2 = interpolate_inViewportSpace(vlist[high], vlist[low], middle_2.position.a, middle_2.position.b,mode);
		// 3 points in a same line
		if (middle_2.position.a == vlist[middle].position.a){}

		//calculate the last cross points ,and do both fillPanTop() and fillPanBottom() 
		else if (middle_2.position.a < vlist[middle].position.a){
			fillPanBottomTri_solid(hdc, vlist[high], middle_2, vlist[middle],mode);
			fillPanTopTri_solid(hdc, middle_2, vlist[low], vlist[middle],mode);
		}
		else{
			fillPanBottomTri_solid(hdc, vlist[high], vlist[middle], middle_2,mode);
			fillPanTopTri_solid(hdc, vlist[middle], vlist[low], middle_2,mode);
		}

	}


}
void LunaScene::fillPanBottomTri_solid(HDC hdc, const LVert&top, const LVert& bottom_left, const LVert& bottom_right, LunaProjectionMode mode){
	clock_t lasttime = clock();
	float yTop = top.position.b;
	float yBottom = bottom_left.position.b;

	int yTopInt = ceil(yTop - 1.5);
	int yBottomInt = ceil(yBottom - 0.5);


	const double left_dx = ((double)top.position.a - (double)bottom_left.position.a) / ((double)top.position.b - (double)bottom_left.position.b);//+?-?+
	const double right_dx = ((double)bottom_right.position.a - (double)top.position.a) / ((double)top.position.b  - (double)bottom_right.position.b);//+?+?-
	double left_x = lineInsertWithHorizontalLine(top.position, bottom_left.position, (double)(yBottom + 0.5));
	double right_x = lineInsertWithHorizontalLine(top.position, bottom_right.position, (double)(yBottom + 0.5));
	clock_t curtime = clock();
	double runtime = (curtime - lasttime) / (double)CLOCKS_PER_SEC;
	cout << "runtime is " << runtime << endl;

	for (int i = yBottomInt; i <= yTopInt; i++){

		int xLeftInt = ceil(left_x - 0.5);
		int xRightInt = ceil(right_x - 1.5);

		for (int j = xLeftInt; j <= xRightInt; j++){
			//zbuffer test
			LVector2 pixelCenter = LVector2(j + 0.5, i + 0.5);
			LearlyZOutput earlyZoutput = interpolateInTri_inViewportSpace_Zvalue(top, bottom_left, bottom_right, pixelCenter.a, pixelCenter.b,mode);
			float zvalue_inbuffer = readZBuffer(j,i);
			if (mode == lunaOrthogonalpromode)earlyZoutput.m_z *= (-1);
			//cout << " zvalue_inbuffer" << zvalue_inbuffer << "/tearlyZoutput.m_z" << earlyZoutput.m_z << endl;
			if (earlyZoutput.m_z<zvalue_inbuffer){
				int temp_textureHeight =m_texturelist[top.texture_ID]->m_Bmp->bmpHeight;
				int temp_textureWidth = m_texturelist[top.texture_ID]->m_Bmp->bmpWidth;
				//cout << "Height of the texture=" << temp_textureHeight << endl;
				//cout << "Width of the texture=" << temp_textureWidth << endl;
			LVert interpolatedV = interpolate_inViewportSpace_otherAttrib(top, bottom_left, bottom_right, earlyZoutput, temp_textureWidth,temp_textureHeight,mode);
			//fragment process
			LFrag fragment = fragmentShaderProgram(interpolatedV, this->m_texturelist[interpolatedV.texture_ID],m_light);
				
			LVector4 cur_color = fragment.m_color;
			drawPixel(hdc, j, i, cur_color);
			writeZBuffer(j, i, earlyZoutput.m_z);
			}

		}
		left_x = left_x + left_dx;
		right_x = right_x - right_dx;

	}


}
void LunaScene::fillPanTopTri_solid(HDC hdc, const LVert&top_left, const LVert& low, const LVert& top_right, LunaProjectionMode mode){
	float yTop = top_left.position.b;
	float tBottom = low.position.b;

	int yTopInt = ceil(yTop - 1.5);
	int yBottomInt = ceil(tBottom - 0.5);

	const double left_dx = ((double)low.position.a - (double)top_left.position.a) / ((double)top_left.position.b - (double)low.position.b);//+?-?+
	const double right_dx = ((double)top_right.position.a - (double)low.position.a) / ((double)top_right.position.b - (double)low.position.b);//+?+?-
	double left_x = lineInsertWithHorizontalLine(top_left.position, low.position, (double)(yTop + 0.5));
	double right_x = lineInsertWithHorizontalLine(top_right.position, low.position, (double)(yTop + 0.5));


	for (int yInt = yTopInt; yInt >= yBottomInt; yInt--){
		int xRightInt = ceil(right_x-1.5);
		int xLeftInt = ceil(left_x - 0.5);
		for (int xInt = xLeftInt; xInt <= xRightInt; xInt++){

			//zbuffer test
			LVector2 pixelCenter = LVector2(xInt + 0.5, yInt + 0.5);
			LearlyZOutput earlyZoutput = interpolateInTri_inViewportSpace_Zvalue(low, top_left,top_right, pixelCenter.a, pixelCenter.b,mode);
			float zvalue_inbuffer = readZBuffer(xInt, yInt);
			if (mode == lunaOrthogonalpromode)earlyZoutput.m_z *= (-1);
			if (earlyZoutput.m_z < zvalue_inbuffer){
				int temp_textureHeight = m_texturelist[low.texture_ID]->m_Bmp->bmpHeight;
				int temp_textureWidth = m_texturelist[low.texture_ID]->m_Bmp->bmpWidth;
				LVert interpolatedV = interpolate_inViewportSpace_otherAttrib(low, top_left, top_right, earlyZoutput, temp_textureWidth, temp_textureHeight,mode);

				//fragment shader
				LFrag fragment = fragmentShaderProgram(interpolatedV, this->m_texturelist[interpolatedV.texture_ID],m_light);

				LVector4 cur_color = fragment.m_color;
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
void LunaScene::makecubemesh(){
	//cube mesh;
	vector<LVector4> temp_position_list;
	vector<LVector4> temp_pos_color_list;
	vector<LTriangle> temp_tri_ID_list;
	vector<LVector4> temp_normal_list;
	//front face
	{
		temp_position_list.push_back(LVector4(5, 5, 5, 1)); int vID0 = temp_position_list.size() - 1;//0
		temp_position_list.push_back(LVector4(5, -5, 5, 1)); int vID1 = temp_position_list.size() - 1;//1
		temp_position_list.push_back(LVector4(-5, -5, 5, 1)); int vID2 = temp_position_list.size() - 1;//2
		temp_position_list.push_back(LVector4(-5, 5, 5, 1)); int vID3 = temp_position_list.size() - 1;//3


		//temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));
		//temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));
		//temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 0, 0));

		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));

		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));

		temp_tri_ID_list.push_back(LTriangle(0, 1, 3));
		temp_tri_ID_list.push_back(LTriangle(1, 2, 3));

		textureCoordlist.push_back(LVector2(1, 1));
		textureCoordlist.push_back(LVector2(1,0));
		textureCoordlist.push_back(LVector2(0, 0));
		textureCoordlist.push_back(LVector2(0, 1));

		temp_normal_list.push_back(LVector4(0, 0, 1, 0));
		temp_normal_list.push_back(LVector4(0, 0, 1, 0));
		temp_normal_list.push_back(LVector4(0, 0, 1, 0));
		temp_normal_list.push_back(LVector4(0, 0, 1, 0));
	}
	//back face
	{

		temp_position_list.push_back(LVector4(5, 5, -5, 1)); int vID0 = temp_position_list.size() - 1;//4
		temp_position_list.push_back(LVector4(5, -5, -5, 1)); int vID1 = temp_position_list.size() - 1;//5
		temp_position_list.push_back(LVector4(-5, -5, -5, 1)); int vID2 = temp_position_list.size() - 1;//6
		temp_position_list.push_back(LVector4(-5, 5, -5, 1)); int vID3 = temp_position_list.size() - 1;//7


		//temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));
		//temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));
		//temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 0, 0));

		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));

		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));

		temp_tri_ID_list.push_back(LTriangle(vID0, vID1, vID3));
		temp_tri_ID_list.push_back(LTriangle(vID1, vID2, vID3));

		textureCoordlist.push_back(LVector2(1, 1));
		textureCoordlist.push_back(LVector2(1, 0));
		textureCoordlist.push_back(LVector2(0, 0));
		textureCoordlist.push_back(LVector2(0, 1));

		temp_normal_list.push_back(LVector4(0, 0, -1, 0));
		temp_normal_list.push_back(LVector4(0, 0, -1, 0));
		temp_normal_list.push_back(LVector4(0, 0, -1, 0));
		temp_normal_list.push_back(LVector4(0, 0, -1, 0));
	}
	//right face
	{

		temp_position_list.push_back(LVector4(5, 5, -5, 1)); int vID0 = temp_position_list.size() - 1;//8
		temp_position_list.push_back(LVector4(5, -5, -5, 1)); int vID1 = temp_position_list.size() - 1;//9
		temp_position_list.push_back(LVector4(5, -5, 5, 1)); int vID2 = temp_position_list.size() - 1;//10
		temp_position_list.push_back(LVector4(5, 5, 5, 1)); int vID3 = temp_position_list.size() - 1;//11


		//temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));
		//temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));
		//temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 0, 0));

		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));

		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));

		temp_tri_ID_list.push_back(LTriangle(vID0, vID1, vID3));
		temp_tri_ID_list.push_back(LTriangle(vID1, vID2, vID3));

		textureCoordlist.push_back(LVector2(1, 1));
		textureCoordlist.push_back(LVector2(1, 0));
		textureCoordlist.push_back(LVector2(0, 0));
		textureCoordlist.push_back(LVector2(0, 1));

		temp_normal_list.push_back(LVector4(1, 0, 0, 0));
		temp_normal_list.push_back(LVector4(1, 0, 0, 0));
		temp_normal_list.push_back(LVector4(1, 0, 0, 0));
		temp_normal_list.push_back(LVector4(1, 0, 0, 0));
	}
	//left face
	{
		temp_position_list.push_back(LVector4(-5, 5, -5, 1)); int vID0 = temp_position_list.size() - 1;//12
		temp_position_list.push_back(LVector4(-5, -5, -5, 1)); int vID1 = temp_position_list.size() - 1;//13
		temp_position_list.push_back(LVector4(-5, -5, 5, 1)); int vID2 = temp_position_list.size() - 1;//14
		temp_position_list.push_back(LVector4(-5, 5, 5, 1)); int vID3 = temp_position_list.size() - 1;//15


		//temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));
		//temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));
		//temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 0, 0));

		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));

		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));

		temp_tri_ID_list.push_back(LTriangle(vID0, vID1, vID3));
		temp_tri_ID_list.push_back(LTriangle(vID1, vID2, vID3));

		textureCoordlist.push_back(LVector2(1, 1));
		textureCoordlist.push_back(LVector2(1, 0));
		textureCoordlist.push_back(LVector2(0, 0));
		textureCoordlist.push_back(LVector2(0, 1));

		temp_normal_list.push_back(LVector4(-1, 0, 0, 0));
		temp_normal_list.push_back(LVector4(-1, 0, 0, 0));
		temp_normal_list.push_back(LVector4(-1, 0, 0, 0));
		temp_normal_list.push_back(LVector4(-1, 0, 0, 0));
	}
	//up face
	{
		temp_position_list.push_back(LVector4(5, 5, -5, 1)); int vID0 = temp_position_list.size() - 1;//16
		temp_position_list.push_back(LVector4(5, 5, 5, 1)); int vID1 = temp_position_list.size() - 1;//17
		temp_position_list.push_back(LVector4(-5, 5, 05, 1)); int vID2 = temp_position_list.size() - 1;//18
		temp_position_list.push_back(LVector4(-5, 5, -5, 1)); int vID3 = temp_position_list.size() - 1;//19


		//temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));
		//temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));
		//temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 0, 0));

		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));

		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));

		temp_tri_ID_list.push_back(LTriangle(vID0, vID1, vID3));
		temp_tri_ID_list.push_back(LTriangle(vID1, vID2, vID3));

		textureCoordlist.push_back(LVector2(1, 1));
		textureCoordlist.push_back(LVector2(1, 0));
		textureCoordlist.push_back(LVector2(0, 0));
		textureCoordlist.push_back(LVector2(0, 1));

		temp_normal_list.push_back(LVector4(0, 1, 0, 0));
		temp_normal_list.push_back(LVector4(0, 1, 0, 0));
		temp_normal_list.push_back(LVector4(0, 1, 0, 0));
		temp_normal_list.push_back(LVector4(0, 1, 0, 0));
	}
	//down face
	{
		temp_position_list.push_back(LVector4(5, -5, -5, 1)); int vID0 = temp_position_list.size() - 1;//20
		temp_position_list.push_back(LVector4(5, -5, 5, 1)); int vID1 = temp_position_list.size() - 1;//21
		temp_position_list.push_back(LVector4(-5, -5, 5, 1)); int vID2 = temp_position_list.size() - 1;//22
		temp_position_list.push_back(LVector4(-5, -5, -5, 1)); int vID3 = temp_position_list.size() - 1;//23


		//temp_pos_color_list.push_back(LVector4(1, 0, 0, 0));
		//temp_pos_color_list.push_back(LVector4(0, 1, 0, 0));
		//temp_pos_color_list.push_back(LVector4(0, 0, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 0, 0));

		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));
		//temp_pos_color_list.push_back(LVector4(1, 1, 1, 0));

		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));
		temp_pos_color_list.push_back(LVector4(1.0, 0.5, 0.31, 0));

		temp_tri_ID_list.push_back(LTriangle(vID0, vID1, vID3));
		temp_tri_ID_list.push_back(LTriangle(vID1, vID2, vID3));

		textureCoordlist.push_back(LVector2(1, 1));
		textureCoordlist.push_back(LVector2(1, 0));
		textureCoordlist.push_back(LVector2(0, 0));
		textureCoordlist.push_back(LVector2(0, 1));
		
		temp_normal_list.push_back(LVector4(0, -1, 0, 0));
		temp_normal_list.push_back(LVector4(0, -1, 0, 0));
		temp_normal_list.push_back(LVector4(0, -1, 0, 0));
		temp_normal_list.push_back(LVector4(0, -1, 0, 0));

	}
	for (int i = 0; i < temp_position_list.size(); i++){
		temp_position_list[i].a /= 5; temp_position_list[i].array[0] /= 5;
		temp_position_list[i].b /= 5; temp_position_list[i].array[1] /= 5;
		temp_position_list[i].c /= 5; temp_position_list[i].array[2] /= 5;
	}
	m_mesh->mesh_positionlist = temp_position_list;
	m_mesh->mesh_orth_z.resize(temp_position_list.size());
	m_mesh->mesh_pointscolorlist = temp_pos_color_list;
	m_mesh->mesh_triangleslist = temp_tri_ID_list;
	m_mesh->mesh_texcoordlist = textureCoordlist;
	m_mesh->mesh_normalist = temp_normal_list;

}
void LunaScene::makeSimpleCube(){


	makecubemesh();
	LTexture* cubetexture = new LTexture();
	string texFileName = "tex.bmp";
	bool success = cubetexture->initwithFile(texFileName.c_str());
	m_texturelist.push_back(cubetexture);


}

//for rotate function:
//axis(1,0,0)alpha +x anticlockwise
//axis(-1,0,0)alpha -x anticlockwise

//axis(0,1,0)alpha
//axis(0,-1,0)alpha

//axis(0,0,1)alpha
//axis(0,0,-1)alpha
LMatrix4 LunaScene::initModelMatrix(){

	LMatrix4 temp_scale_mat = calculateScaleMatrix(0.25, 0.25, 0.25);
	LVector4 temp_trans(50, 50, -50 , 0);
	LMatrix4 temp_trans_mat = calculateTranslateMatrix(temp_trans);
	LVector4 rotate_axis(0, 1, 0, 1);
	LMatrix4 temp_rotate_mat2 = calculateRotateMatrix(rotate_axis, 0.707, 0.707);//cosA=0.5,sinA=0.86;degree=60;
	LMatrix4 temp_rotate_mat1 = calculateRotateMatrix(LVector4(1, 0, 0, 1), 0.707, 0.707);// 0.707, 0.707  //0.819,,0.574
	//LMatrix4 temp_model_mat = temp_scale_mat*temp_trans_mat;
	//LMatrix4 temp_model_mat = temp_rotate_mat*temp_model_mat;
	LMatrix4 temp_model_mat;
	//temp_model_mat = temp_rotate_mat1*temp_rotate_mat2;
	return temp_model_mat;
	//LMatrix4 temp_mat=calculateRotateMatrix()

}
LMatrix4 LunaScene::initViewMatrix(){
	LMatrix4 temp_view_mr(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
	LMatrix4 temp_view_mt(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, -10, 1
		);
	LMatrix4 temp_viewm = temp_view_mr*temp_view_mt;
	//LMatrix4 temp_viewm ;
	m_viewMat = temp_viewm;
	return m_viewMat;
}
LMatrix4 LunaScene::initProjectionMatrix(LunaProjectionMode mode){
	float fov = 45;//cot(fov)=2n/h
	float respect=1.0;//width_view/height_view
	float n = 0.1;//mutipul -1;
	float f = 100;//multiple -1;
	float width_n,width_see;
	float height_n,height_see;
	float cube_pos_z=10;			//!!change!!
	height_n =2*n*tan((fov/2.0)*(PI/180));
	height_see = height_n*cube_pos_z/n;
	if (mode == lunaPerspectivepromode){  
		n = n*(-1);
		f = f*(-1);
		float tan_fov_2 = tan((fov / 2.0)*(PI / 180));
		//cout << tan_fov_2 << endl;
		float t11 = 1 / (respect*tan_fov_2);
		float t22 = 1 / tan_fov_2;
		float t33 = (f + n) / (n-f);
		float t34 = -(2 * n * f) / ( n-f );
		LMatrix4 temp_proj(
			t11, 0, 0, 0,
			0, t22, 0, 0,
			0, 0, t33, -1,
			0, 0, t34, 0
			);
		return temp_proj;
	}
	else{
		width_see = respect*height_see;
		n = n*(-1);
		f = f*(-1);
		float t11 = 2 /width_see ;
		float t22 = 2/ height_see;
		float t33 = 2/ (n - f);
		float t34 = -(n+f) / (n - f);
		LMatrix4 temp_proj(
			t11, 0, 0, 0,
			0, t22, 0, 0,
			0, 0, t33, 0,
			0, 0, t34, 1
			);
		//LMatrix4 temp_per2or(
		//	n, 0, 0, 0,
		//	0, n, 0, 0,
		//	0, 0, n + f, 1,
		//	0, 0, -n*f, 0
		//	);
		//temp_proj = temp_proj*temp_per2or;
		return temp_proj;

	}
}

void LunaScene::transformVertixes(){
	//cout << "run into LunaScene::transformVertixes()" << endl;
	//run the VertexShaderProgram (just like in real OpenGL)
	//use MVP to change the vetices properties
	int vertexNum = transformed_mesh->mesh_positionlist.size();
	for (int i = 0; i < vertexNum; i++){
		LVert v = transformed_mesh->getVert(i);

		LVert transformed_v = vertexShaderProgram(m_modelMat, m_camera->getViewMat(),m_camera->getProjectionMat(),v);


		//transformed_mesh->mesh_positionlist[i] = transformed_v;
		transformed_mesh->setVert(i, transformed_v);
	}



	//use viewport matrix change the vertexes coordinates
	int vertices_num = transformed_mesh->mesh_positionlist.size();
	for (int i = 0; i < vertices_num;i++){
	
		//first to NDC
		transformed_mesh->mesh_positionlist[i].w = transformed_mesh->mesh_positionlist[i].d;
		transformed_mesh->mesh_orth_z[i]=transformed_mesh->mesh_positionlist[i].d;
		transformed_mesh->mesh_positionlist[i]=transformed_mesh->mesh_positionlist[i] / (transformed_mesh->mesh_positionlist[i].d);
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
LVert LunaScene::interpolate_inViewportSpace_otherAttrib(const LVert&v0, const LVert& v1, const LVert&v2, const LearlyZOutput& earlyOutput, int texWidth, int texHeight, LunaProjectionMode mode){

	const float A = earlyOutput.m_A;
	const float B = earlyOutput.m_B;
	const float x = earlyOutput.m_x;
	const float y = earlyOutput.m_y;
	const float z = earlyOutput.m_z;

	const float x0 = v0.position.array[0];
	const float y0 = v0.position.array[1];
	const float z0 = v0.position.array[2];
	const float w0 = v0.position.array[3];
	LVector2 texcoord0=v0.texcoord;
	const float oldz0 = v0.origin_orth_z;
	LVector4 color0 = v0.color;

	const float x1 = v1.position.array[0];
	const float y1 = v1.position.array[1];
	const float z1 = v1.position.array[2];
	const float w1 = v1.position.array[3];
	LVector2 texcoord1=v1.texcoord;
	const float oldz1 = v1.origin_orth_z;
	 LVector4 color1 = v1.color;
	
	const float x2 = v2.position.array[0];
	const float y2 = v2.position.array[1];
	const float z2 = v2.position.array[2];
	const float w2 = v2.position.array[3];
	LVector2 texcoord2=v2.texcoord;
	const float oldz2 = v2.origin_orth_z;
	LVector4 color2 = v2.color;
	
	float s, t;
	{

		if (mode == lunaOrthogonalpromode){
			s = (1 - A - B)*texcoord0.a + A*texcoord1.a + B*texcoord2.a;
			t = (1 - A - B)*texcoord0.b + A*texcoord1.b + B*texcoord2.b;
		}
		else {

			LVector2 tc0divoldz0 = texcoord0 / oldz0;
			LVector2 tc1divoldz1 = texcoord1 / oldz1;
			LVector2 tc2divoldz2 = texcoord2 / oldz2;

			float revz0 = 1 / oldz0;
			float revz1 = 1 / oldz1;
			float revz2 = 1 / oldz2;
			LVector2 curtcdivoldz = tc0divoldz0*(1 - A - B) + tc1divoldz1*A + tc2divoldz2*B;

			float revz;
			revz = (1 - A - B)*revz0 + A*revz1 + B*revz2;
			curtcdivoldz = curtcdivoldz / revz;
			s = curtcdivoldz.a;
			t = curtcdivoldz.b;

		}

		/*	n = -15;
			f = -100;*/
		//const float f =-100;//100
		//const float n = -0.1;//0.1
		//const float M =  f / (f - n);//1.11
		//const float s0_mul_z0SubM = s0*(z0 - M);
		//const float s1_mul_z1SubM = s1*(z1 - M);
		//const float s2_mul_z2SubM = s2*(z2 - M);
		//float s0_mul_zSubM = s0_mul_z0SubM + (s1_mul_z1SubM - s0_mul_z0SubM)*A + (s2_mul_z2SubM - s0_mul_z0SubM)*B;
		//const float t0_mul_z0SubM = t0*(z0 - M);
		//const float t1_mul_z1SubM = t1*(z1 - M);
		//const float t2_mul_z2SubM = t2*(z2 - M);
		//float t_mul_zSubM = t0_mul_z0SubM + (t1_mul_z1SubM - t0_mul_z0SubM)*A + (t2_mul_z2SubM - t0_mul_z0SubM)*B;
		//s = s0_mul_zSubM / (z - M);
		//t = t_mul_zSubM / (z - M);

		//float p0w = v0.position.w; 
		//float p1w = v1.position.w;
		//float p2w = v2.position.w;

		//double zr = (1 - A - B)*(1 / p0w) + A*(1 / p1w) + B*(1 / p2w);
		//s = (((1-A-B)*(s0/p0w)+A*(s1/p1w)+B*(s2/p2w))/zr)*texWidth;
		//t = (((1 - A - B)*(t0 / p0w) + A*(t1 / p1w) + B*(t2 / p2w)) / zr)*texHeight;
		//cout << "s:" << s << "   t:" << t << endl;
	}

	float cal_w = (1 - A - B)*w0 + A*w1 + B*w2;
	LVector4 cal_color = color0*(1 - A - B) + color1*A + color2*B;

	LVector4 t_lv4(x, y, z, cal_w);
	LVert temp=v0;
	temp.color = cal_color;
	temp.position = t_lv4;
	temp.texcoord = { s, t };

	return temp;

}
LearlyZOutput LunaScene::interpolateInTri_inViewportSpace_Zvalue(const LVert&v0, const LVert&v1, const LVert&v2, float x, float y, LunaProjectionMode mode){
	const float x0 = v0.position.array[0];
	const float y0 = v0.position.array[1];
	const float z0 = v0.position.array[2];
	const float w0 = v0.position.w;
	const float z0_inverse = 1 / w0;

	const float x1 = v1.position.array[0];
	const float y1 = v1.position.array[1];
	const float z1 = v1.position.array[2];
	const float w1 = v1.position.w;
	const float z1_inverse = 1 / w1;

	const float x2 = v2.position.array[0];
	const float y2 = v2.position.array[1];
	const float z2 = v2.position.array[2];
	const float w2 = v2.position.w;
	const float z2_inverse = 1 / w2;

	LearlyZOutput earlyZOutput1;

	/*if (z0<0 || z1<0 || z2<0)
	{ cout << "test z value:" << z0 << "/t" << z1 << "/t" << z2 << endl; }*/

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
	float z_inverse;
	if (M == 0){//  p0p1//p0p2, p0p1p2 is degenerated tri, this may not happen, so no matter return what
		//do nothing
	} 
	else{
		A = -(y2suby0*xsubx0 - x2subx0*ysuby0) / M;
		B = (y1suby0*xsubx0 - x1subx0*ysuby0) / M;
		//----calculate z
		//if(mode==lunaOrthogonalpromode)
			//z = 1/z0_inverse + (1/z1_inverse - 1/z0_inverse)*A + (1/z2_inverse - 1/z0_inverse)*B;
			z = z0 + (z1-z0)*A + (z2-z0)*B;
		//else{
		//	z_inverse = (1 - A - B)*z0_inverse + A*z1_inverse + B*z2_inverse;
		//	z = 1 / z_inverse;
		//}

	}

	earlyZOutput1.m_A = A;
	earlyZOutput1.m_B = B;
	earlyZOutput1.m_x = x;
	earlyZOutput1.m_y = y;
	earlyZOutput1.m_z = z;



	return earlyZOutput1;



	//const float xa = v0.position.array[0];
	//const float ya = v0.position.array[1];
	//const float za = v0.position.array[2];

	//const float xb = v1.position.array[0];
	//const float yb = v1.position.array[1];
	//const float zb = v1.position.array[2];

	//const float xc = v2.position.array[0];
	//const float yc = v2.position.array[1];
	//const float zc = v2.position.array[2];


	//float x_xb = x - xb;
	//float yc_yb = yc - yb;
	//float y_yb = y - yb;
	//float xa_xb = xa - xb;
	//float ya_yb = ya - yb;
	//float xc_xb = xc - xb;
	//float x_xc = x - xc;
	//float y_yc = y - yc;
	//float xb_xc = xb - xc;
	//float ya_yc = ya - yc;
	//float yb_yc = yb - yc;
	//float xa_xc = xa - xc;

	//float myalpha, mybeta, mygamma;
	//myalpha = (y_yb*xc_xb - x_xb*yc_yb) / (ya_yb*xc_xb - xa_xb*yc_yb);
	//mybeta = (y_yc*xa_xc - x_xc*ya_yc) / (yb_yc*xa_xc - xb_xc*ya_yc);

	//mygamma = 1 - myalpha - mybeta;

	//float des_z = myalpha*za + mybeta*zb + mygamma*zc;
	////if(des_z<0||des_z>1)cout << "des_z"<<des_z << endl;
	//LearlyZOutput ans;
	//ans.m_x = x;
	//ans.m_y = y;
	//ans.m_z = des_z;
	//ans.m_A = myalpha;
	//ans.m_B = mybeta;

	//cout << "syl.z yc.z:" << earlyZOutput1.m_z <<"   "<< ans.m_z << endl;
	//return ans;

}
void LunaScene::test01(){
	//cout << "pixel:(" << x_pixel << "," << y_pixel << ")	z_value:" << z_test_buffer[x_pixel*screen_width + y_pixel] << endl;
	cout << "winter is coming!" << endl;
}
double LunaScene::readZBuffer(int x_pixel, int y_pixel){
	int screen_width = (int)m_viewport.c;
	int screen_height = (int)m_viewport.d;
	if (x_pixel>= 0 && x_pixel < screen_width&&y_pixel >= 0 && y_pixel < screen_height){
		//if (x_pixel >=740)test01();
		double ans = z_test_buffer[y_pixel*screen_width + x_pixel];
		return ans;
	}
	return -1;//just  a test!
}
void LunaScene::writeZBuffer(int x_pixel, int y_pixel, double z_value){
	int screen_width = (int)m_viewport.c;
	int screen_height = (int)m_viewport.d;
	if (x_pixel >= 0 && x_pixel < screen_width&&y_pixel >= 0 && y_pixel < screen_height){
		if(z_value>10)cout <<"pixel:("<<x_pixel<< ","<<y_pixel<<")	z_value:" << z_value << endl;
		z_test_buffer[y_pixel*screen_width + x_pixel]=z_value;
	}

}
LVert LunaScene::interpolate_inViewportSpace(const LVert& v1, const LVert&v2, float x, float y,LunaProjectionMode mode){
	//in viewportspace calculate some important value base on the triangle figures
	const float x1 = v1.position.a;
	const float y1 = v1.position.b;
	const float z1 = v1.position.c;
	LVector2 texcoord1 = v1.texcoord;
	const float oldz1 = v1.origin_orth_z;


	const float x2 = v2.position.a;
	const float y2 = v2.position.b;
	const float z2 = v2.position.c;
	LVector2 texcoord2 = v2.texcoord;
	const float oldz2 = v2.origin_orth_z;

	const float dx = x1 - x2;
	const float dy = y1 - y2;
	float k;

	if (abs(dx) < abs(dy)){
		k = (y - y2) / dy;
	}
	else{
		k = (x - x2) / dx;
	}

	if (mode==lunaOrthogonalpromode){	
		
	float des_z = k*(z1 - z2) + z2;
	LVector2 des_tc = (texcoord1 - texcoord2)*k + texcoord2;

	LVert v = v1;
	v.position = LVector4(x, y, des_z, v1.position.d);
	v.texcoord = des_tc;
	
	return v;
	}
	else{
		float revz1 = 1 / oldz1;
		float revz2 = 1 / oldz2;
		float z_temp = k*(z1 - z2) + z2;
		float revz = k*(revz1 - revz2) + revz2;
		
		LVector2 tc1divoldz1 = texcoord1 / oldz1;
		LVector2 tc2divoldz2 = texcoord2 / oldz2;

		LVector2 curtc = (tc1divoldz1 - tc2divoldz2)*k + tc2divoldz2;

		curtc = curtc / revz;

		LVert v = v1;
		float des_z = 1 / revz;
		v.position = LVector4(x, y, z_temp, v1.position.d);
		v.origin_orth_z = des_z;
		v.position.w = des_z;
		v.texcoord = curtc;
		return v;
	}


}
