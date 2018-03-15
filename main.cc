#include <stdint.h>
#include <stm32f4xx.h>

#include <dbg.h>

typedef unsigned int uint;

void SystemInit() { 
}

#define __CONCAT__( a, b, c ) a##b##c
#define CONCAT( a, b, c ) __CONCAT__( a, b, c )

// 16 - 5.25 kHz
#define mm 16
#define PWM_FREQ (256*mm -1)
#define PWM_VAL (PWM_FREQ>>1)

#define P1_DP 8
#define P1_UP 13
#define P2_DP 9
#define P2_UP 0
#define P3_DP 10
#define P3_UP 1

#define P_setup( PN ) \
{ \
    ma |= 1<<(CONCAT(P,PN,_DP)*2); \
    mb |= 1<<(CONCAT(P,PN,_UP)*2); \
}

#define P_up( PN ) \
{ \
    pa |= (1<<(CONCAT(P,PN,_DP)))<<16; \
    pb |= (1<<(CONCAT(P,PN,_UP))); \
}

#define P_down( PN ) \
{ \
    pa |= (1<<(CONCAT(P,PN,_DP))); \
    pb |= (1<<(CONCAT(P,PN,_UP)))<<16; \
}

volatile int tm = 0;
volatile int tm_st = 0;

extern "C"
void TIM2_IRQHandler() { // FIXME: dirty pwm ;D
    int sr = TIM2->SR; TIM2->SR=0;
    int pa=0, pb=0;
    if ( TIM2->CR1 & TIM_CR1_DIR ) { // down counting
        if (tm_st) {tm++; tm_st=0;}
        if (sr & TIM_SR_CC1IF) P_down(1);
        if (sr & TIM_SR_CC2IF) P_down(2);
        if (sr & TIM_SR_CC3IF) P_down(3);
    } else { // up counting
        if (!tm_st) {tm++; tm_st=1;}
        if (sr & TIM_SR_CC1IF) P_up(1);
        if (sr & TIM_SR_CC2IF) P_up(2);
        if (sr & TIM_SR_CC3IF) P_up(3);
    }
    GPIOA->BSRR = pa;
    GPIOB->BSRR = pb;
    //JPUTI(1);
}

#include "sin.cc"

int main() {
    int a = 8000000;
    do asm(""); while (a--);//*/
    //JPUT("START");

    /* Enable FPU */
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));

    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY)) asm("");
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_HSE;
    JPUTI(5); 
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_0*64; // 86 // 192/2MHz
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_0*8; //*/
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY)) asm("");
    JPUTI(6);
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
    JPUTI(7);

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
    int ma=0, mb=0;
    P_setup(1);
    P_setup(2);
    P_setup(3);
    GPIOA->MODER |= ma;
    GPIOB->MODER |= (1<<(12*2)) | mb;
    GPIOB->BSRR = 1<<(12);

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->ARR = PWM_FREQ;
    TIM2->CCR1 = PWM_VAL; // wypelnienie 
    TIM2->CCR2 = PWM_VAL;
    TIM2->CCR3 = PWM_VAL; //+(PWM_VAL>>4);
    TIM2->EGR = TIM_EGR_CC1G
              | TIM_EGR_CC2G
              | TIM_EGR_CC3G;
    TIM2->CCER = TIM_CCER_CC1E // PWM ch1 en 
               | TIM_CCER_CC2E //PWM ch2 en
               | TIM_CCER_CC3E; //PWM ch3 en
    TIM2->DIER = TIM_DIER_CC1IE
               | TIM_DIER_CC2IE
               | TIM_DIER_CC3IE;

    TIM2->CR1 = TIM_CR1_CEN// TIM2 wlaczenie licznika
             |  3*TIM_CR1_CMS_0; // counting up and down
    NVIC_EnableIRQ( TIM2_IRQn );

    int b = 0;
    uint n = 0;
    volatile float ang = 0;
    while (1) {
        int pa=0, pb=0;
        /*if ( b ) {
            P_up(1);
            P_up(2);
            P_up(3);
        } else {
            P_down(1);
            P_down(2);
            P_down(3);
        }//*/
        /*GPIOA->BSRR = pa;
        GPIOB->BSRR = pb;*/
        //b ^= 1;
        //dbg_pulse();
        /*TIM2->CCR1 = PWM_VAL; // wypelnienie 
        TIM2->CCR2 = PWM_VAL;

        TIM2->CCR3 = PWM_VAL;
        switch(b) {
            case 0: TIM2->CCR1+=(PWM_VAL>>2); break;
            case 1: TIM2->CCR2+=(PWM_VAL>>2); break;
            case 2: TIM2->CCR3+=(PWM_VAL>>2); break;
        } //*/
        //int a = 100;
        //do asm(""); while (--a);
        while (tm<5) asm("");
        tm-=5;
        ang += 0.03*2*3.1415;
        //JPUTI(ang*10.0);
        //JPUTI(n);
        setup_pwm(n, ang);
#if 0
        static int oo = 0;
        if (!oo--) {
            JPUTI(((int)TIM2->CCR1));
            JPUTI(((int)TIM2->CCR2));
            JPUTI(((int)TIM2->CCR3)); // 
            /*JPUTI(((int)TIM2->CCR1)-((int)TIM2->CCR2));
            JPUTI(((int)TIM2->CCR2)-((int)TIM2->CCR3));
            JPUTI(((int)TIM2->CCR3)-((int)TIM2->CCR1)); // */
            JPUTI(n);
            oo = 10;
        } //*/
#endif
        //b++; if ( b>=3 ) b=0;
        //JPUTI(TIM2->CNT);
        //JPUTI(GPIOB->ODR);
        asm("");
    }
}
    
