#pragma once
#include "LVector.h"
#include <iostream>
class LMaterial{

public:
	LMaterial(){
	
		shininess = 2.0;//what is the default value of shininess in opengl?

		ambient.init(0.2, 0.2, 0.2, 1);

		diffuse.init(0.8, 0.8, 0.8, 1);

		specular.init(0, 0, 0, 1);
	};
	~LMaterial(){};


public:
	LVector4 ambient;
	LVector4 diffuse;
	LVector4 specular;

	float shininess;

};