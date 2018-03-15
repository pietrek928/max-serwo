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

volatile float u = 0.04;
template<class T>
void setup_pwm( uint &n, T &a ) {
    if ( a > 3.1415/6 ) {
        a -= 3.1415/3;
        n++;
    }

    T R1, R2, R3;
#include "tst.h"
    TIM2->CCR1 = ((float)(PWM_FREQ))*(R1*u);
    TIM2->CCR2 = ((float)(PWM_FREQ))*(R2*u);
    TIM2->CCR3 = ((float)(PWM_FREQ))*(R3*u);
}

