#ifndef _LUNALIGHT_H_
#define _LUNALIGHT_H_


#include <iostream>
#include "LVector.h"
using namespace std;
class LunaLight{
public:
	LunaLight(){
		light_ambient.init(0,0,0,1);
	}



public:
	void setLightPos(float x, float y, float z, float w){
		light_pos.init(x, y, z, w);
	}

public:
	LVector4 light_pos;
	LVector4 light_ambient;
	LVector4 light_specular;
	LVector4 light_diffuse;


};
#endif
