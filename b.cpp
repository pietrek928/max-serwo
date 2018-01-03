#include <iostream>

using namespace std;

template<class T>
auto sincos( T x ) {
    T xx = x*x;
    T rcos = 1.0f;
    T rsin = x; // TODO: will sqrt(1-c^2) be better ?
    rcos -= xx*(T(1)/2); xx*=x;
    rsin -= xx*(T(1)/6); xx*=x;
    rcos += xx*(T(1)/24); xx*=x;
    rsin += xx*(T(1)/120); xx*=x;
    rcos -= xx*(T(1)/720);
    struct {
        T sin;
        T cos;
    } r;
    r.sin = rsin;
    r.cos = rcos;
    return r;
}

#define T double

int main() {
    double a = 0;
    int n = 0;
    for ( int i=0; i<100; i++ ) {
    a += 0.01*2*3.1415;
    if ( a > 3.1415/6 ) {
        a -= 3.1415/3 ;
        n++;
    }
    double R1, R2, R3;
#include "tst.h"
    cout << a << " " << R1-R2 << " " << R2-R3 << " " << R3-R1 << endl;
    //cout << a << " " << R1 << " " << R2 << " " << R3 << endl;
    }
    return 0;
}

