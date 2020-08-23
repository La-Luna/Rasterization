#pragma once
#include "readBmp.h"
#include<iostream>
class LTexture{
public:

	LTexture(){
		m_Bmp = NULL;
	}
	virtual ~LTexture(){
		if (m_Bmp)delete m_Bmp;
	}
	bool initwithFile(const string &bmpfileName){
		m_Bmp = new Lunabmp;
		bool succ = m_Bmp->readBmp(bmpfileName.c_str());
		return succ;
	}

	LVector4 getColor(const LVector2& texcoord)const{
		float s = texcoord.a;
		float t =1-texcoord.b;

		s = s - floor(s);
		t = t - floor(s);

		float j = s*m_Bmp->bmpWidth;
		float i = t*m_Bmp->bmpHeight;
		LVector4 color;
		{
			float jL, jR, iU, iD;
			jL = floor(j - 0.5) + 0.5;
			jR = jL + 1;
			iU = floor(i - 0.5) + 0.5;
			iD = iU + 1;
			float djL=j-jL;
			float djR=jR-j;
			float diU=i-iU;
			float diD=iD-i;
			int jL_int = floor(jL);
			int jR_int = jL_int + 1;
			int iU_int = floor(iU);
			int iD_int = iU_int + 1;
			Lrgb LUcolor = m_Bmp->getPixelColor(max(iU_int, 0), max(jL_int, 0));
			Lrgb LDcolor = m_Bmp->getPixelColor(min(iD_int, m_Bmp->bmpHeight - 1), max(jL_int, 0));
			Lrgb RUcolor = m_Bmp->getPixelColor(max(iU_int, 0), min(jR_int, m_Bmp->bmpWidth - 1));
			Lrgb RDcolor = m_Bmp->getPixelColor(min(iD_int, m_Bmp->bmpHeight - 1), min(jR_int, m_Bmp->bmpWidth - 1));
			LVector4 LUcolorf((float)LUcolor.m_r / 255, (float)LUcolor.m_g / 255, (float)LUcolor.m_b / 255, 1);
			LVector4 LDcolorf((float)LDcolor.m_r / 255, (float)LDcolor.m_g / 255, (float)LDcolor.m_b / 255, 1);
			LVector4 RUcolorf((float)RUcolor.m_r / 255, (float)RUcolor.m_g / 255, (float)RUcolor.m_b / 255, 1);
			LVector4 RDcolorf((float)RDcolor.m_r / 255, (float)RDcolor.m_g / 255, (float)RDcolor.m_b / 255, 1);
			LVector4 Ucolorf, Dcolorf, CenterColor;
			Ucolorf = LUcolorf*djR + RUcolorf*djL;
			Dcolorf = LDcolorf*djR + RDcolorf*djL;
			CenterColor = Ucolorf*diD + Dcolorf*diU;
			color = CenterColor;

		}
		return color;

	}

	Lunabmp* m_Bmp;

};
