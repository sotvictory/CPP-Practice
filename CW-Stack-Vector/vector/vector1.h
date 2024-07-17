#pragma once

class Vector {
    int size_;
    double *body;

public:
    explicit Vector(int sz) { 
        body = new int[size_ = sz]; 
    }

    Vector(const Vector& v) { 
        body = new int[size_ = v.size_];
        for (int i = 0; i < size_; i++) 
            body[i] = v.body[i];
    }

    ~Vector() { 
        delete [] body; 
    }

    Vector& operator = (const Vector& v) {
        if (this != &v){
            delete [] body;
            body = new int[size_ = v.size_];
            for (int i = 0; i < size_; i++) 
                body[i] = v.body[i];
        }
        return *this;
    }

    int size() { 
        return size_; 
    }

    double& operator [] (int index) { 
	    if (index < 0 || index >= size_)
		    error("Index out of range");
	    return body[index]; 
    }

    double sum() {
        double s = 0;
        for (int i = 0; i < size(); i++)
            s += body[i];
        return s;
    }
};