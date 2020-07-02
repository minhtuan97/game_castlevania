#pragma once
#include "Cell.h"
class Grid
{
	int cellwidth;
	int cellheight;
	Cell** cells;
	int cell_cloumn;
	int cell_row;
	vector <LPGAMEOBJECT>* listObject;
	static Grid* _instance;
	bool checkObj(vector<LPGAMEOBJECT>* listobj, LPGAMEOBJECT e);
public:
	static Grid* GetInstance();
	Grid();
	~Grid();
	void addObject(LPGAMEOBJECT object);
	void GetListOfObjects(vector<LPGAMEOBJECT>* list_object);
	void ClearObject();
	void deleteObject(LPGAMEOBJECT object);
};

