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
float fscanFloat(FILE* fp){
	float ans;
	fscanf(fp, "%f", &ans);
	return ans;

}
vector<string> divideStr(const string&str, const string&dividerChars){
	const int dividerCharCount = (int)dividerChars.size();
	vector<string> strList;
	string t_str;
	int len = (int)str.size();
	for (int i = 0; i < len; i++){
		char c = str[i];
		bool cIsDividerCh = false;
		for (int j = 0; j < dividerCharCount; j++){
			char divierCh = dividerChars[j];
			if (c == divierCh){
				cIsDividerCh = true;
				break;
			}
		}//got cIsDividerCh
		if (cIsDividerCh == false){
			t_str += c;
		}
		else{
			strList.push_back(t_str);
			t_str.clear();
		}
	}
	if (t_str.empty() == false){
		strList.push_back(t_str);
	}//got strList;
	return strList;

}
int string2int(string strnum){
	//string to nums
	int ans = 0;
	int mult = 1;
	for (int i = strnum.size() - 1; i >= 0; i--){
		int temp = strnum[i] - '0';
		ans = mult*temp + ans;
		mult *= 10;
	}
	return ans;
}