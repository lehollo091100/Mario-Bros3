
#include <fstream>
#include <vector>
#include<iostream>
#include<string.h>
#include <string>
using namespace std;
string a;
bool allowoutput = false;
int count = 0;

int main()
{
	int left, top, right, bottom, x, y, type, cellwidth, cellheight;
	int bboxwidth; int bboxheight;
	ifstream ifs("inputObjects.txt", ios::in);
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
		ofs << floorf(x*1.0 / cellwidth) << " " << floorf(y*1.0 / cellheight) << " " << ceilf((x + bboxwidth)*1.0 / cellwidth) << " " << ceilf((y + bboxheight)*1.0 / cellheight) << endl;

	}

	ifs.close();
	system("pause");
}
