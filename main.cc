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

    int a = 0;
};

class DerivedTest : public BaseTest{
public:
    DerivedTest(){

    }
    void DerivedFunction() override {
        cout << "Derived Function" << endl;
    }
};

void TestValue(Ptr<BaseTest> ptr){
    ptr->a = 1;
}
void TestReference(Ptr<BaseTest> & ptr){
    ptr->a = 1;
}

// Binary Tree Node structure
struct TreeNode {
    int value;
    Ptr<TreeNode> left;
    Ptr<TreeNode> right;

    TreeNode(int val) : value(val) {}
};

// Function to create and initialize a simple binary tree
Ptr<TreeNode> create_tree() {
    // Create root node
    Ptr<TreeNode> root(new TreeNode(5));
    
    // Create left child and assign using move semantics
    root->left = Ptr<TreeNode>(new TreeNode(3));
    
    // Create right child and assign using move semantics
    root->right = Ptr<TreeNode>(new TreeNode(7));
    
    return root;
}

// Example usage
int main() {
    Ptr<TreeNode> root = create_tree();
    
    // Access node values using Ptr dereference
    std::cout << "Root value: " << root->value << "\n";
    std::cout << "Left child value: " << root->left->value << "\n";
    std::cout << "Right child value: " << root->right->value << "\n";
    
    // Check reference counts
    std::cout << "Root reference count: " << root.Count() << "\n";
    std::cout << "Left child reference count: " << root->left.Count() << "\n";
    std::cout << "Right child reference count: " << root->right.Count() << "\n";
    
    return 0;
}

/*

// Basic test and demonstration of Ptr
int main(){
    
    Ptr<int> p1(1);
    Ptr<int> p2(2);
    Ptr<int> p3(3);
    Ptr<int> p4(p1);
    Ptr<int> p5();

    cout << p1.Count() << endl;
    cout << p2.Count() << endl;
    cout << p3.Count() << endl;
    cout << p4.Count() << endl;
    cout << (bool)p5 << endl;

    return 0;
    cout << "Demangled typename of p: " << p.Type() << endl; // int
    cout << "P is defined: " << (p ? "True" : "False") << endl; // True
    cout << "Address of p: " << &p << endl;     // address of p
    cout << "Value of p: " << *p << endl;       // 2
    cout << "Reference count of p: " << p.Count() << endl; // 1
    
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

    for(int i = 0; i < 100000; i++){
     
        Ptr<BaseTest> p(true);
        TestValue(p);
        //TestReference(p);

    }
    return 0;
}
*/