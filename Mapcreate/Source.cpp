
#include <fstream>
#include <vector>
#include<iostream>
#include<string.h>
#include <string>
using namespace std;
int main()
{
	ifstream ifs("Hiddenscene3.txt", ios::in);
	ofstream ofs("out7.txt", ios::out);
	string a;
	bool allowoutput = false;
	int count = 0;
	while (!ifs.eof())
	{
		ifs >> a;
		for (int i = 0; i < a.length(); i++)
		{
			if (a[i] == '"')
				if (!allowoutput)
				{
					allowoutput = true;
					continue;
				}
				else {
					count++;
					if (count == 160)
					{
						ofs << endl;
						count = 0;
					}
					else
						ofs << "\t";
					allowoutput = false;
				}
			if (allowoutput)
			{
				ofs << a[i];
			}
		}
	}
	ifs.close();
	system("pause");
}