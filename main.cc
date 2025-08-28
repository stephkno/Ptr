#include <cassert>
#include <iostream>
#include "ptr.h"

void testPtrFunction() {
    // Test Default Construction
    {
        Ptr<int> p;
        assert(!p);
        assert(p.Count() == 0);
        assert(p.Type() == "Null");
        try {
            *p;
            assert(false); // Should not reach here
        } catch (const UninitializedPointer& e) {}
        try {
            p.operator->();
            assert(false);
        } catch (const UninitializedPointer& e) {}
    }

    // Test Make Factory and Basic Functionality
    {
        auto p = Ptr<int>::Make(42);
        assert(p);
        assert(p.Count() == 1);
        assert(*p == 42);
        assert(p.Type() == "int");
        assert(p.Unique());
    }

    // Test Copy Constructor
    {
        auto p1 = Ptr<int>::Make(100);
        auto p2 = p1; // Copy
        assert(p1.Count() == 2);
        assert(p2.Count() == 2);
        *p2 = 200;
        assert(*p1 == 200);
    }

    // Test Move Constructor
    {
        auto p1 = Ptr<int>::Make(300);
        auto p2 = std::move(p1);
        assert(!p1); // p1 is moved from
        assert(p2.Count() == 1);
        assert(*p2 == 300);
    }

    // Test Assignment Operator
    {
        auto p1 = Ptr<int>::Make(400);
        Ptr<int> p2;
        p2 = p1;
        assert(p1.Count() == 2);
        assert(p2.Count() == 2);
    }

    // Test Move Assignment
    {
        auto p1 = Ptr<int>::Make(500);
        Ptr<int> p2;
        p2 = std::move(p1);
        assert(!p1);
        assert(p2.Count() == 1);
    }

    // Test Casting
    {
        class Base { public: virtual ~Base() {} };
        class Derived : public Base { public: Derived() {} };
        class Unrelated {};

        // Upcast from Derived to Base
        Ptr<Derived> derivedPtr = Ptr<Derived>::Make();
        Ptr<Base> basePtr = derivedPtr.Cast<Base>();
        assert(basePtr != nullptr);
        assert(derivedPtr.Count() == 2);
        assert(basePtr.Count() == 2);

        // Downcast back to Derived
        auto derivedPtr2 = basePtr.Cast<Derived>();
        assert(derivedPtr2 != nullptr);
        assert(derivedPtr2.Count() == 3);

        // Attempt to cast Base (not Derived) to Derived
        Ptr<Base> basePtr2 = Ptr<Base>::Make();
        auto derivedPtr3 = basePtr2.Cast<Derived>();
        assert(derivedPtr3 == nullptr);

        // Cast to unrelated type
        auto unrelatedPtr = basePtr.Cast<Unrelated>();
        assert(unrelatedPtr == nullptr);
    }

    // Test Exceptions
    {
        Ptr<int> nullPtr;
        try {
            nullPtr.Get();
            assert(nullPtr == nullptr);
        } catch (const UninitializedPointer&) {}
    }

    // Test Addr Method
    {
        auto p = Ptr<int>::Make(123);
        assert(!p.Addr().empty());
    }

    // Test Type Method
    {
        auto p = Ptr<std::string>::Make("test");
        std::string type = p.Type();
        assert(type.find("basic_string") != std::string::npos || type.find("string") != std::string::npos);
    }

    // Test Unique Method
    {
        auto p1 = Ptr<int>::Make(10);
        assert(p1.Unique());
        auto p2 = p1;
        assert(!p1.Unique());
        assert(!p2.Unique());
    }

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    testPtrFunction();
    return 0;
}