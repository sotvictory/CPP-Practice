#pragma once
#include <algorithm>

inline void error(const char* msg) { throw msg; }

class Vector {
	double* body;
	int size_;

public:
	explicit Vector(int n) {
		if (n < 0) 
            error("Illegal length");
		body = new double[size_ = n];
	}

	Vector(const Vector& rhs) {
		size_ = rhs.size_;
		body = new double[size_];
		for (int i = 0; i < size_; i++)
			body[i] = rhs.body[i];
	}

	Vector(Vector&& rhs) {
		size_ = rhs.size_;
		body = rhs.body;
		rhs.body = nullptr;
		rhs.size_ = 0;
	}

	Vector& operator = (Vector&& rhs) {
		std::swap(body, rhs.body);
		std::swap(size_, rhs.size_);
		return *this;
	}

	Vector& operator = (const Vector& rhs) {
		if (this == &rhs) 
            return *this;
		delete[] body;
		size_ = rhs.size_;
		body = new double[size_];
		for (int i = 0; i < size_; i++)
			body[i] = rhs.body[i];
		return *this;
	}

	const double& operator [] (int index) const {
		if (index < 0 || index >= size_)
			error("Index out of range");
		return body[index];
	}

	double& operator [] (int index) {
		if (index < 0 || index >= size_)
			error("Index out of range");
		return body[index];
	}

	~Vector() { delete [] body; }

	int size() const { return size_; }

	double sum() const {
		double s = 0.0;
		for (int i = 0; i < size_; i++) 
            s += body[i];
		return s;
	}

	Vector& operator += (const Vector& v) {
		for (int i = 0; i < size_; i++) 
			body[i] += v.body[i];
		return *this;
	}

	friend Vector operator + (const Vector& v1, const Vector& v2) {
		if (v1.size() != v2.size()) error("Different sizes");
		Vector v(v1.size());
		for (int i = 0; i < v.size(); i++)
			v.body[i] = v1.body[i] + v2.body[i];
		return v;
	}

	friend double operator * (const Vector& v1, const Vector& v2) {
		if (v1.size() != v2.size()) error("Different sizes");
		double sum = 0.0;
		for (int i = 0; i < v1.size(); i++)
			sum += v1.body[i] * v2.body[i];
		return sum;
	}
};



