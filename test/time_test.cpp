#include <time.h>
#include <iostream>

using namespace std;

int main() {
    
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    /*
    * Do the work.
    */
    for(unsigned long int i = 0; i < 1000000000; i++) {}
        
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cout << cpu_time_used << endl;
    return 0;
}
