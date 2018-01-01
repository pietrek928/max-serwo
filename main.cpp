#include <stdint.h>
#include <stm32f4xx.h>

#include <dbg.h>

void SystemInit() { 
}

#define __CONCAT__( a, b, c ) a##b##c
#define CONCAT( a, b, c ) __CONCAT__( a, b, c )

#define mm 32
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

extern "C"
void TIM2_IRQHandler() { // FIXME: dirty pwm ;D
    int sr = TIM2->SR; TIM2->SR=0;
    int pa=0, pb=0;
    if ( TIM2->CR1 & TIM_CR1_DIR ) { // down counting
        if (sr & TIM_SR_CC1IF) P_down(1);
        if (sr & TIM_SR_CC2IF) P_down(2);
        if (sr & TIM_SR_CC3IF) P_down(3);
    } else { // up counting
        if (sr & TIM_SR_CC1IF) P_up(1);
        if (sr & TIM_SR_CC2IF) P_up(2);
        if (sr & TIM_SR_CC3IF) P_up(3);
    }
    GPIOA->BSRR = pa;
    GPIOB->BSRR = pb;
}

int main() {
    /*volatile int a = 20000000;
    while (a--);*/
    //RCC->CR |= RCC_CR_HSEON;
    //while (!(RCC->CR & RCC_CR_HSERDY)) asm("");
    //JPUTI(5);
    /*RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLN_0*192; // 192/2MHz
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM;
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLM_0*16; //*/
    //RCC->CR |= RCC_CR_PLLON;
    /*while (!(RCC->CR & RCC_CR_PLLRDY)) asm("");
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW)|RCC_CFGR_SW_PLL;*/

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
    int ma=0, mb=0;
    P_setup(1);
    P_setup(2);
    P_setup(3);
    GPIOA->MODER |= ma;
    GPIOB->MODER |= 1<<(12*2) | mb;
    GPIOB->ODR |= 1<<(12);

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->ARR = PWM_FREQ;
    TIM2->CCR1 = PWM_VAL; // wypelnienie 
    TIM2->CCR2 = PWM_VAL-(PWM_FREQ>>3);
    TIM2->CCR3 = PWM_VAL;
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
    //NVIC_EnableIRQ( TIM2_IRQn );

    int b = 0;
    while (1) {
        int pa=0, pb=0;
        if ( b ) {
            P_up(1);
            P_up(2);
            P_up(3);
        } else {
            P_down(1);
            P_down(2);
            P_down(3);
        }
        /*GPIOA->BSRR = pa;
        GPIOB->BSRR = pb;*/
        b ^= 1;
        //dbg_pulse();
        volatile int a = 2000000;
        do asm(""); while (--a);
        JPUTI(TIM2->SR);
    }
}

