#pragma once
#include <fstream>
#include <vector>
#include<iostream>
#include<string.h>
#include <string>
using namespace std;
class Loader
{
public:
	Loader() {

	};
	~Loader()
	{

	};
	void LoadGrid(string filepath) {
		int left, top, right, bottom, x, y, type, cellwidth, cellheight;
		int bboxwidth; int bboxheight;
		ifstream ifs(filepath, ios::in);
		ofstream ofs("gridinfo.txt", ios::out);
		string a;
		bool allowoutput = false;
		int count = 0;
		ifs >> cellwidth >> cellheight;
		while (!ifs.eof())
		{

			ifs >> type;
			ifs >> x >> y;
			ifs >> bboxwidth >> bboxheight;
			ofs <<type<<" "<< (int)(x*1.0 / cellwidth) << " " << (int)(y*1.0 / cellheight) << " " << (int)((x + bboxwidth)*1.0 / cellwidth) << " " << (int)((y + bboxheight)*1.0 / cellheight) << endl;

		}

		ifs.close();
	};
};

