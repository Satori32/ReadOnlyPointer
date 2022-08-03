#include <iostream>
//#include <crtdbg.h>//vc only

class RCount {
public:
	RCount() {}
	~RCount() {
		(*ptr)--;

		if (!(*ptr)) { delete ptr; ptr = nullptr; }
	}

	int Count() {
		return *ptr;
	}

	RCount(const RCount& In) {
		*In.ptr += 1;
		(*ptr) -= 1;
		if (!(*ptr)) { delete ptr; ptr = nullptr; }
		ptr = In.ptr;
	}

	RCount& operator=(RCount& In) {
		*In.ptr += 1;
		(*ptr) -= 1;
		if (!(*ptr)) { delete ptr; ptr = nullptr; }
		ptr = In.ptr;

		return *this;
	}

protected:
	int* ptr = new int(1);
};

template< class T>
class Const_Shared_Ptr{//un writeable pointer, it for distribution
public:
	Const_Shared_Ptr(T* p):P(p) {}
	~Const_Shared_Ptr() {
		if (R.Count()==1) { delete P; P = nullptr; }
	}

	const T* operator ->() const {
		return P;
	}

	const Const_Shared_Ptr& operator= (Const_Shared_Ptr& In) {

		if (R.Count() == 1) { delete P; P = nullptr;}
		R = In.R;
		P = In.P;

		return *this;
	}

	const Const_Shared_Ptr& operator *() {
		return *this;
	}

protected:
	RCount R;
	T* P = nullptr;
};


template<class T,class ... A>
Const_Shared_Ptr<T> MK_CSP(A&... X) {
	Const_Shared_Ptr<T> S(new T(X...));

	return S;
}

template<class T>
class ReadOnlyPointer {
public:
	ReadOnlyPointer() = delete;
	ReadOnlyPointer(std::shared_ptr<T> In) :P(In) {}
	const std::shared_ptr<const T> operator ->() const{
		return P;
	}
	
	const std::shared_ptr<const T> operator*() const{
		return P;
	}

protected:
	std::shared_ptr<const T> P = nullptr;
};

class Test {
public:
	Test(){}
	bool Inc() { N++; return true; }
	bool Dec() { N--; return true; }
	const int Show() const { return N; }
protected:
	int N = 0;
};


int main() {
	RCount R;
	RCount X;
	RCount Y;

	R = X;

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //vc only.

	Const_Shared_Ptr<int> CSP = MK_CSP<int>();

	//(*CSP) = 10;

	Const_Shared_Ptr<Test> T = MK_CSP<Test>();

	std::cout << T->Show() << std::endl;

	ReadOnlyPointer<Test> Z = std::make_shared<Test>();

	Z->Show();
	(*Z)->Show();

}