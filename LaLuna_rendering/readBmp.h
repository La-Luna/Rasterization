#pragma once
#include "Windows.h"
#include<iostream>
class Lrgb{
public:
	unsigned char m_r;
	unsigned char m_g;
	unsigned char m_b;
	void init(unsigned char r, unsigned char g, unsigned char b){
		m_r = r;
		m_g = g;
		m_b = b;
	}
};
class Lunabmp{
public:
	unsigned char* pBmpBuf;
	int bmpHeight;
	int bmpWidth;
	RGBQUAD* pColorTable;
	int biBitCount;
	int lineByte;



public:
	Lunabmp(){
		lineByte =0;
		pColorTable = NULL;
		pBmpBuf = NULL;

	}
	virtual ~Lunabmp(){
		if (pBmpBuf){
			delete[] pBmpBuf;
		}
		if (pColorTable){
			delete[] pColorTable;
		}
	}

	bool readBmp(const char* bmpName);
	void drawBmp(HDC hdc);
	Lrgb getPixelColor(int i, int j);
};