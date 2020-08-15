#include "stdafx.h"
#include "funcs.h"
vector<int> sortfromHigh2Low(float a, float b, float c){
	//012
	//021
	//102
	//120
	//201
	//210   6 types
	vector<int> ans;
	int max_3 = min(min(a, b), c);
	int max_2;
	if (max_3 == a){
		ans.push_back(0);
		max_2 = min(b, c);
		if (max_2 == b){ ans.push_back(1); ans.push_back(2); return ans; }
		else{ ans.push_back(2); ans.push_back(1); return ans; }
	}
	if (max_3 == b){
		ans.push_back(1);
		max_2 = min(a, c);
		if (max_2 == a){ ans.push_back(0); ans.push_back(2); return ans; }
		else{ ans.push_back(2); ans.push_back(0); return ans; }

	}
	if (max_3 == c){
		ans.push_back(2);
		max_2 = min(a, b);
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
double lineInsertWithHorizontalLine(const LVector4& top, const LVector4& bottom, float y0){

}