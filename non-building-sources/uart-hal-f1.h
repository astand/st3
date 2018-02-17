#pragma once

#include <stdint.h>
#include "stm32f10x.h"

/* next section uses for any STM32Fx family (F1, F2, F4) */
class UartHal {
 public:
  UartHal(USART_TypeDef* uart) : uart_(uart) {}

 public:
  /* EnableTCI - Transmission Complete Interrupt Enable */
  void enable_TCI() {
    uart_->CR1 |= USART_CR1_TCIE;
  }

  /* EnableTCI - Transmission Complete Interrupt Disable */
  void disable_TCI() {
    uart_->CR1 &= ~(USART_CR1_TCIE);
  }

  // Check is Transmission completed interrupt enabled
  bool is_TCI_enabled() {
    return (uart_->CR1 & USART_CR1_TCIE);
  }

  // check if Transmission completed
  bool is_TC() {
    return (uart_->SR & USART_SR_TC);
  }

  // clear Transmission completed flag
  void clear_TC() {
    uart_->SR &= ~(USART_SR_TC);
  }

  void enable_TXEI() {
    uart_->CR1 |= USART_CR1_TXEIE;
  }

  /* EnableTXEI - Transmission Complete Interrupt Disable */
  void disable_TXEI() {
    uart_->CR1 &= ~(USART_CR1_TXEIE);
  }

  // Check is Transmission completed interrupt enabled
  bool is_TXEI_enabled() {
    return (uart_->CR1 & USART_CR1_TXEIE);
  }

  // check if Transmission completed
  bool is_TXE() {
    return (uart_->SR & USART_SR_TXE);
  }

  // clear Transmission completed flag
  void clear_TXE() {
    uart_->SR &= ~(USART_SR_TXE);
  }

  bool is_byte_received() {
    return (uart_->SR & USART_SR_RXNE);
  }

  uint32_t get_sr() {
    return uart_->SR;
  }

  uint8_t get_dr() {
    return (uint8_t)(uart_->DR & 0x01FF);
  }

  void flow_byte(const uint8_t d) {
    uart_->DR = (0x01FF) & (d);
  }

 private:
  USART_TypeDef* const uart_;

};
