#define JTAG_COMM_DATA_BUF_LEN 2048
#define JTAG_COMM_DATA_MAGIC 0x87654321

typedef struct {
    uint32_t id;
    uint32_t data_len;
    uint8_t  buf[ JTAG_COMM_DATA_BUF_LEN+1 ];
} __attribute__(( aligned(4) )) jtag_comm;

jtag_comm __global_comm = {JTAG_COMM_DATA_MAGIC, 0, {0}};

void __jtag_comm_put( jtag_comm *c, char *data, uint32_t l ) {
    l += c->data_len;
    if ( c->data_len+l > JTAG_COMM_DATA_BUF_LEN ) return;
    uint32_t i;
    for ( i=c->data_len; i<l; i++ ) c->buf[i] = *(data++);
    c->data_len = l;
    c->buf[ c->data_len ] = 0;
}

void __jtag_comm_reset( jtag_comm *c ) {
    c->data_len = 0;
}


void jtag_comm_put( char *data, uint32_t l )
{ __jtag_comm_put( &__global_comm, data, l ); }

void jtag_comm_reset( )
{ __jtag_comm_reset( &__global_comm ); }

#define JPUT( __v__ ) jtag_comm_put( (char*)&(__v__), sizeof( __v__ ) );
#define JPUTA( __data__, __l__ ) jtag_comm_put( (char*)(__data__), __l__ );
#define JPUTI( __i__ ) {int __oo__=__i__; JPUT(__oo__); }
#define JRESET() jtag_comm_reset();
#define DEBUG_panic( s ) { JPUT(s); CPU_crit_enter(); while(1); }

void dbg_init() {

    /*RCC->AHB1ENR |=  RCC_AHB1ENR_GPIODEN;
    GPIOD->MODER = 1<<(2*2);// | 1<<(1*2) | 1<<(3*2) | 1<<(4*2);
    GPIOD->ODR = 0;*/
}

void dbg_pulse() {
    GPIOD->ODR ^= 1<<(2);
}

void dbg_err() {
    GPIOD->BSRR = 1<<(2);
}

void dbg_err_pulse() {
    GPIOD->ODR ^= 1<<(2);
}

void dbg_err_clr() {
    GPIOD->BSRR = (1<<(2))<<16;
}

