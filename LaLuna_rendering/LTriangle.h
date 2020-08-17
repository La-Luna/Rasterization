#ifndef _LTRIANGLE_H_
#define _LTRIANGLE_H_


class LTriangle{

public:
	LTriangle(){};
	LTriangle(int id0, int id1, int id2){
		array[0] = id0;
		array[1] = id1;
		array[2] = id2;
	}
	LTriangle(const LTriangle& t){
		for (int i = 0; i < 3; i++){
			array[i] = t.array[i];
		}
	}
	~LTriangle(){};
	short array[3];


};


#endif
