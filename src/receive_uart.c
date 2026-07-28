#include "stm32f4xx.h"
#include "receive_uart.h"

#define UART_BAUD 115200

uint8_t rx_Buffer[RX_BUFFER_SIZE];


static void UART_Clock_Enable(void)
{
    /* GPIOA and USART1 CLOCK*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
}

static void DMA_Clock_Enable(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
}

static void UART_GPIO_Init(void)
{
    /* Alternate Function */
    GPIOA->MODER &= ~((3U << (9 * 2)) | (3U << (10 * 2)));

    GPIOA->MODER |= (2U << (9 * 2)) | (2U << (10 * 2));

    /* High Speed */
    GPIOA->OSPEEDR |= (3U << (9 * 2)) | (3U << (10 * 2));

    /* Alternater Function 7 (AF7 = USART1) */
    GPIOA->AFR[1] &= ~((0xF << 4) | (0xF << 8));

    GPIOA->AFR[1] |= (7 << 4) | (7 << 8);

    /* Pull-up on RX (PA10) */
    GPIOA->PUPDR &= ~(3U << (10 * 2));
    GPIOA->PUPDR |=  (1U << (10 * 2));

    /* no pull on TX (PA9) */
    GPIOA->PUPDR &= ~(3U << (9 * 2));
}


static void DMA_Config(void)
{
    /* Disable stream before configuring */
    DMA2_Stream2->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream2->CR & DMA_SxCR_EN);

    /* Clear pending flags */
    DMA2->LIFCR =
          DMA_LIFCR_CTCIF2
        | DMA_LIFCR_CHTIF2
        | DMA_LIFCR_CTEIF2
        | DMA_LIFCR_CDMEIF2
        | DMA_LIFCR_CFEIF2;

    /* Peripheral address */
    DMA2_Stream2->PAR = (uint32_t)&USART1->DR;

    /* Memory address */
    DMA2_Stream2->M0AR = (uint32_t)rx_Buffer;

    /* Number of bytes */
    DMA2_Stream2->NDTR = RX_BUFFER_SIZE;

    DMA2_Stream2->CR = 0;

    /* Channel 4 */
    DMA2_Stream2->CR |= (4 << DMA_SxCR_CHSEL_Pos);

    /* Peripheral -> Memory */
    DMA2_Stream2->CR &= ~DMA_SxCR_DIR;

    /* Increment memory pointer */
    DMA2_Stream2->CR |= DMA_SxCR_MINC;

    /* 8-bit peripheral */
    DMA2_Stream2->CR &= ~DMA_SxCR_PSIZE;

    /* 8-bit memory */
    DMA2_Stream2->CR &= ~DMA_SxCR_MSIZE;

    /*  Circular mode */
    DMA2_Stream2->CR |= DMA_SxCR_CIRC;

    /* High priority */
    DMA2_Stream2->CR |= DMA_SxCR_PL_1;
}


static void UART_Config(void)
{
    USART1->CR1 = 0;

    USART1->BRR = UART_BAUD; /* 16MHz / 0x008B */

    USART1->CR1 |= USART_CR1_TE;
    USART1->CR1 |= USART_CR1_RE;

    /* Enable DMA receiver */
    USART1->CR3 |= USART_CR3_DMAR;

    /* Enable USART IDLE interrupt */
    USART1->CR1 |= USART_CR1_IDLEIE;

    /* Enable USART1 interrupt in NVIC */
    NVIC_EnableIRQ(USART1_IRQn);

    USART1->CR1 |= USART_CR1_UE;
}



void RX_Init(void)
{
    UART_Clock_Enable();
    DMA_Clock_Enable();

    UART_GPIO_Init();
    DMA_Config();
    UART_Config();
}


void RX_Start(void)
{
    DMA2_Stream2->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream2->CR & DMA_SxCR_EN);

    DMA2_Stream2->M0AR = (uint32_t)rx_Buffer;
    DMA2_Stream2->NDTR = RX_BUFFER_SIZE;

    DMA2_Stream2->CR |= DMA_SxCR_EN;
}


static uint16_t old_pos = 0;

uint16_t RX_Process(uint8_t **data)
{
    uint16_t pos;
    uint16_t length;

    pos = RX_BUFFER_SIZE - DMA2_Stream2->NDTR;

    if(pos >= old_pos)
    {
        length = pos - old_pos;
    }
    else
    {
        length = RX_BUFFER_SIZE - old_pos + pos;
    }

    *data = &rx_Buffer[old_pos];

    old_pos = pos;

    return length;
}