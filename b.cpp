#include <cstdio>

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

int main() {
    printf("%d\n",sizeof(a));
    return 0;
}

