#include <vector>
#include <string>
#include <sstream>

using namespace std;

vector<string>
separate (string target, char delim) 
{

        vector<string> ret;

        istringstream f(target);
        string s;

        while (getline(f, s, delim)) 
        {   
                ret.push_back(s);
        }   

        return ret;
} 
