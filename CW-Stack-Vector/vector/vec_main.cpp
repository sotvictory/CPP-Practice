#include <cstdio>
#include "vector.h"

int main() {
	Vector v(20000), v1(20000), v2(20000);

	for (int i = 0; i < v.size(); i++) 
        v[i] = i + 1;

	double sum = 0;
	for (int i = 0; i < v.size(); i++)  
        sum += v[i];

	Vector vv(v);
	v1 = v2 = vv;
	v1 = v1;

	for (int i = 0; i < v1.size(); i++)
		v1[i] = i;
	v1 = v1 + v2;
	vv = v1 + v2;
	double sc = v2 * (v1 + v1);
	printf("Sum = %lf\n", (v1 + v2).sum());

	return 0;
}