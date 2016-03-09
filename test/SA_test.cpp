#include "../src/SA.h"

using namespace std;

int main(int argc, char** argv)
{
        const string word("azertyuiopazertyuiop$");
        vector<size_t> SA = buildSA(word);
        
        for(size_t i = 0; i < SA.size(); i++)
                cout << SA.at(i) << endl;
        
        
        return 0;
}
