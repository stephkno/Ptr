#include <ptr.h>
#include <iostream>
#include <vector>
#include <stdint.h>

using namespace std;

class BaseTest{
public:
    BaseTest(){}
    
    virtual ~BaseTest(){}

    void Function(){
        cout << "Function" << endl;
    }
    
    virtual void DerivedFunction() {}
};

class DerivedTest : public BaseTest{
public:
    DerivedTest(){

    }
    void DerivedFunction() override {
        cout << "DerivedFunction" << endl;
    }
};

int main(){

    int i = 2;
    Ptr<int> p(i);
    cout << "Demangled typename of p: " << p.Type() << endl; // int
    cout << "P is defined: " << (p ? "True" : "False") << endl; // 1
    cout << "Address of p: " << &p << endl;       // addr
    cout << "Value of p: " << *p << endl;       // 2
    
    vector<Ptr<int>> ints;

    cout << "Reference count of ints: " << p.Count() << endl; // 0
    cout << "Cloning 1000 ints into ints" << endl;

    Ptr<int> int_(42);
    
    for(int i = 0; i < 999; i++){
        
        ints.emplace_back(Ptr<int>(int_));
        cout << "Reference count of ints: " << ints.back().Count() << endl; // 1000
    
    }
    
    cout << "Reference count of p: " << p.Count() << endl; // 1000

    Ptr<BaseTest> a(true);
    a->Function();

    Ptr<DerivedTest> b(true);
    b->DerivedFunction();

    Ptr<BaseTest> c(b.Cast<BaseTest>());
    cout << &c << endl;

    c->DerivedFunction();
    c.Cast<DerivedTest>()->DerivedFunction();

    return 0;

}