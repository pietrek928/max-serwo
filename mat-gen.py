#!/usr/bin/python
from numpy import *

try:
    T = float128
except NameError:
    T = float64

"""class T(T):
    def __getattr__(s, n):
        print(n)
        return getattr(s, n)
    def __str__( s ):
        return 'str'
    def __repr__( s ):
        return 'repr' #"""

sin_tab = {
        0: T(0.0),
        30: T(0.5),
        60: sqrt(T(3))/2,
        90: T(1.0)
}

def scale_var( a, v ):
    if a==T(1.0): return v
    if a==T(-1.0): return ( -v if isinstance(v, T) else '-'+v )
    if a==T(0.0): return T(0.0)
    return '(({})*({}))'.format(a, v)

def add_var( a, b ):
    if a==T(0.0): return b
    if b==T(0.0): return a
    return '(({})+({}))'.format(a, b)

def nsin( n ):
    sc = T(1.0)
    n = (n+10*360)%360
    if n > 180:
        sc = T(-1.0)
        n = n-180
    if n > 90: n = 180-n
    return sc*sin_tab[n]

def ncos( n ):
    return nsin( 90-n )

def sins( s ):
    return add_var(scale_var(nsin(s), 'ca'), scale_var(ncos(s), 'sa'))

def coss( s ):
    return sins( 90-s )

def rot_m( a ):
    return [[  cos(a), sin(a) ],
            [ -sin(a), cos(a) ]]

def rot_v( a ):
    return [ cos(a), sin(a) ]

# x 3/2
M_3F = [[  T(1),  0 ],
        [ -T(1)/2,  sqrt(T(3))/2 ],
        #[ -T(1)/3, -T(1)/sqrt(T(3)) ]
        ]

def gen_mat_mul( r, m, v, tr='T' ):
    for i in range( len(m) ):
        for j in range( len(v[0]) ):
            print(
                '{} r_{}_{} = {};'.format( tr, i, j, '+'.join(
                    '({})*({})'.format(m[i][k],v[k][j])
                        for k in range(len(v))
                ))
            )

def setup_pwm( t, n, v, rev=False ):
    if rev:
        v = [*reversed(v)]
    n = (n+6*len(t))%len(t)
    for i in range(len(v)):
        print('{} = ({});'.format(t[i], v[(i+n)%len(t)]))

def var( n, v ):
    print( 'T {} = ({});'.format(n, v) )

def setup_sin( a ):
    A = 90
    sc = sqrt(T(3))*T(0.5)
    print( 'auto cs = sincos({});'.format( a ) )
    var( 'sc', sc )
    var( 'scm', -sc )
    var( 'ca', scale_var( sqrt(T(3)), 'cs.cos' ) )
    var( 'sa', 'cs.sin' )
    print( 'switch( n ) {' )
    for n in range( 6 ):
        print( 'case {}:{{'.format( n ) )
        p = n%2
        n //= 2
        if p:
            m, mn = 'scm', 'sc'
            n -= 1
        else:
            m, mn = 'sc', 'scm'
        setup_pwm(['R1', 'R2', 'R3'], n,
                [0, scale_var(mn, 'sa+ca'), scale_var(m, 'sa-ca') ])
        print('}break;')
    print('}')

#gen_mat_mul( 'r', M_3F, [*zip(rot_v(pi/6))] )
#print( matmul(rot_m(pi/2),[1, 1]) )

#setup_pwm( ['R1', 'R2', 'R3'], 0, ['v1', 'v2', 0] )

#print('aaaaaaa')
#for i in range( 6 ):
#    print(matmul(M_3F,rot_m(T(pi)*i/3+T(pi)/12)))

# for i in [0, 60, 120, 180, 240, 300, 360]:
    # print( '{} {}'.format( coss(i), coss(i+120) ) )
setup_sin( 'a' )

