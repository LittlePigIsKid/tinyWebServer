#include<iostream>
using namespace std;

class MyClass {
	public:
	MyClass(){}
	~MyClass(){}
	void operation();
};

extern "C"
void MyClass::operation() {
	cout << "Hello world" << endl;
}
