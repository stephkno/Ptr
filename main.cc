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
        cout << "Base Function" << endl;
    }
    
    virtual void DerivedFunction() {}
};

class DerivedTest : public BaseTest{
public:
    DerivedTest(){

    }
    void DerivedFunction() override {
        cout << "Derived Function" << endl;
    }
};

int main(){

    Ptr<int> p(2);
    cout << "Demangled typename of p: " << p.Type() << endl; // int
    cout << "P is defined: " << (p ? "True" : "False") << endl; // 1
    cout << "Address of p: " << &p << endl;       // addr
    cout << "Value of p: " << *p << endl;       // 2
    cout << "Reference count of p: " << p.Count() << endl; // 1000
    
    vector<Ptr<int>> ints;
    
    cout << "Reference count of ints: " << p.Count() << endl; // 0
    cout << "Cloning 1000 ints into ints" << endl;
    
    Ptr<int> int_(42);
    
    for(int i = 0; i <= 1000; i++){
        
        Ptr<int> p = Ptr<int>(int_);
        ints.push_back(p);
        ints[i] = i;

    }

    cout << "Value of ints[0]" << endl;
    cout << ints[0].Get() << endl;

    cout << "Value of ints[100]" << endl;
    cout << ints[100].Get() << endl;

    cout << "Reference count of ints: " << ints.back().Count() << endl; // 1000
    
    cout << "Create base class ptr" << endl;
    Ptr<BaseTest> a(true);
    
    cout << "Call base class function" << endl;
    a->Function();

    cout << "Create derived class ptr" << endl;
    Ptr<DerivedTest> b(true);
    
    cout << "Call derived class function" << endl;
    b->DerivedFunction();

    cout << "Create base class ptr from derived class ptr" << endl;
    Ptr<BaseTest> c(b.Cast<BaseTest>());

    cout << "Call base function from derived base class ptr" << endl;
    c->Function();

    cout << "Call derived function from derived base class ptr" << endl;
    c->DerivedFunction();
    c.Cast<DerivedTest>()->DerivedFunction();

    return 0;

}