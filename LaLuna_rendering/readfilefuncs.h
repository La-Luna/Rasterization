#include <iostream>
#include <string>
#include "LVector.h"
#include "assert.h"
using namespace std;
char buff[1024];
string fscanStr(FILE *fp, const string& expectedstring){
	assert(fp);
	string str;
	fscanf(fp, "%s", buff);
	str = buff;
	if (!expectedstring.empty()){
		assert(expectedstring == str);
	}
	return str;

}
int fscanInt(FILE* fp){
	assert(fp);
	int temp;
	fscanf(fp, "%d", &temp);
	return temp;
}
string fscanOneQuatation(FILE* fp){
	assert(fp);
	string quatation;
	string str;
	fscanf(fp, "%s", buff);
	str = buff;
	assert(str[0] == '\"');
	quatation += str;
	while (1){
		char endch = quatation[(int)quatation.size() - 1];
		if (endch == '\"'){
			break;
		}
		fscanf(fp, "%s", buff);
		str = buff;
		quatation += str;
	}
	
	return quatation;
}
LVector3 fscanVector3(FILE* fp){
	float x, y, z;
	fscanf(fp, "%f", &x);
	fscanf(fp, "%f", &y);
	fscanf(fp, "%f", &z);
	LVector3 ans = LVector3(x, y, z);
	return ans;
}