#include "stm32f0xx.h"

static void gpio_init(void)
{
    /* Enable GPIOA and GPIOB clocks */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;

    /* PA1: TRIG (output) */
    GPIOA->MODER &= ~(3u << (1 * 2));
    GPIOA->MODER |=  (1u << (1 * 2));  // 01: output

    /* PB0, PB1: motor direction outputs */
    GPIOB->MODER &= ~((3u << (0 * 2)) | (3u << (1 * 2)));
    GPIOB->MODER |=  (1u << (0 * 2)) | (1u << (1 * 2));
}

static void delay(volatile uint32_t t)
{
    while (t--) __NOP();
}

int main(void)
{
    gpio_init();

    while (1)
    {
        /* TRIG test pulse */
        GPIOA->BSRR = (1u << 1);
        delay(4000);
        GPIOA->BRR  = (1u << 1);

        /* Motor forward: PB0=1, PB1=0 */
        GPIOB->BSRR = (1u << 0);
        GPIOB->BRR  = (1u << 1);
        delay(500000);

        /* Motor backward: PB0=0, PB1=1 */
        GPIOB->BRR  = (1u << 0);
        GPIOB->BSRR = (1u << 1);
        delay(500000);
    }
}
