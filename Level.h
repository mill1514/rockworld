#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>

using namespace std;

class Level
{

	private:

		int numRows;
		int numCols;
		string name; // WITHOUT '.lvl'

	public:

		Level();
		void setName(string n);
		void createFromFile(string filename);
		void createRandomized(int r, int c, int m);
		void createDefault(string lvlName);
		void writeToFile();
		vector<vector<int>> map;
		int getNumFood();
		int play();
		int playcolorless();
		string getName();
		const char * getCName();
};

#endif
