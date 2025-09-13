#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <zephyr/drivers/uart.h>

#define UART_DEVICE DT_NODELABEL(uart0)
/* Alternatively, you can use this, for nrf52832dk, it will have the same
 * behavior, since uart0 is chosen as zephyr_console
 * #define UART_DEVICE DT_CHOSEN(zephyr_console)
 */
static const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE);

int main(void) {
  // printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

  if (!device_is_ready(uart_dev)) {
    return -1;
  }
  u_int8_t data[] = "Hello World! UART\n";
  for (size_t i = 0; i < sizeof(data); i++) {
    uart_poll_out(uart_dev, data[i]);
  }
  while (true) {
    while (!uart_poll_in(uart_dev, &data[0])) {
      uart_poll_out(uart_dev, data[0]);
    }
  }
  return 0;
}
