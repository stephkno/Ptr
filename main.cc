#include <ptr.h>
#include <iostream>

using namespace std;

class Test{
public:
    Test(){}
    void Function(){
        cout << "test" << endl;
    }
};

int main(){

    P<int> p(new int(2));
    cout << p.Name() << endl; // int
    cout << p << endl;        // 1
    cout << &p << endl;       // addr
    cout << *p << endl;       // 0
    cout << (bool)p << endl;  // 1

    vector<P<int>> ints;
    uint8_t * rawints[1000];
    cout << p.count() << endl;// 0

    for(int i = 0; i < 1000; i++){
        ints.push_back(P<int>(new int(i)));
        rawints[i] = new uint8_t(i);
    }
    
    cout << p.count() << endl;// 1000

    P<Test> t(new Test());
    t->Function();
    
}