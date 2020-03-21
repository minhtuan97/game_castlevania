#pragma once
#include <unordered_map>
#include"Animation.h"
using namespace std;

class Animations
{
	static Animations* _instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);

	static Animations* GetInstance();
};

