#include <iostream>
#include <vector>

using namespace std;

class A
{
public:
	void resize(){
		names.resize(10000);
		for (int i = 0; i < 10000; ++i)
		{
			names[i] = 1;
		}
	}
private:
	int names[];
};

class B : public A
{
public:
	void setX(int a){
		x = a;
	};
	int getX(){
		return x;
	}
private:
	int x;	
};


int main(int argc, char const *argv[])
{
	B b;
	b.setX(2);
	cout << sizeof(b) << endl;
	b.resize();
	cout << sizeof(b) << endl;
	cout<< b.getX() << endl;
	return 0;
}