#ifndef _FUNCS_H_
#define _FUNCS_H_

#include "stdafx.h"
#include "LVert.h"
#include "LVector.h"
#include<vector>
using namespace std;

vector<int> sortfromHigh2Low(float a, float b, float c){
	//cout << "run into LunaScene::fillTriangleSolid(HDC hdc, const LVert& v0, const LVert& v1, const LVert& v2)" << endl;
	//012
	//021
	//102
	//120
	//201
	//210   6 types
	vector<int> ans;
	float max_3 = max(max(a, b), c);
	float max_2;
	if (max_3 == a){
		ans.push_back(0);
		max_2 = max(b, c);
		if (max_2 == b){ ans.push_back(1); ans.push_back(2); return ans; }
		else{ ans.push_back(2); ans.push_back(1); return ans; }
	}
	if (max_3 == b){
		ans.push_back(1);
		max_2 = max(a, c);
		if (max_2 == a){ ans.push_back(0); ans.push_back(2); return ans; }
		else{ ans.push_back(2); ans.push_back(0); return ans; }

	}
	if (max_3 == c){
		ans.push_back(2);
		max_2 = max(a, b);
		if (max_2 == a){ ans.push_back(0); ans.push_back(1); return ans; }
		else{ ans.push_back(1); ans.push_back(0); return ans; }
	}
}

LVert calculateMiddle2inTri(LVert high_v, LVert low_v, float y0){
	LVert ans;
	float x1 = high_v.position.a;
	float y1 = high_v.position.b;
	float x2 = low_v.position.a;
	float y2 = low_v.position.b;

	if (x1 == x2){
		ans = low_v;
		ans.position.a = x1;
		ans.position.b = y0;
	}
	else{
		float x0 = x2 - ((y2 - y0) / (y2 - y1))*(x2 - x1);
		ans = low_v;
		ans.position.a = x0;
		ans.position.b = y0;
	}
	return ans;
}
double lineInsertWithHorizontalLine(const LVector4& top, const LVector4& bottom, double y0){
	float x1 = top.a;
	float x2 = bottom.a;
	float y1 = top.b;
	float y2 = bottom.b;

	double x0;
	if (x1 == x2){ x0 = x1; }
	else{
		x0 = x1 - ((y1 - y0) / (y2 - y1))*(x2 - x1);
	}
	return x0;
}
LVector4 perComponentProduct(const LVector4&v0, const LVector4&v1){
	LVector4 temp(
		v0.a*v1.a,
		v0.b*v1.b,
		v0.c*v1.c,
		v0.d*v1.d
		);
	return temp;
}
#endif

