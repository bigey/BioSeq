#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

bool cmp(string a, string b)
{
	return a < b ? true : false;
}

int main(int argc, char** argv)
{
	string word("azertyuiopazertyuiop$");
	vector<string> SA;
	
	for(size_t i = 0; i < word.size(); i++)
		SA.push_back( word.substr(i, word.size()-i-1 ) );

	sort(SA.begin(), SA.end(), cmp);

	for(size_t i = 0; i < SA.size(); i++)
		cout << SA.at(i) << endl;

	return 0;
}
