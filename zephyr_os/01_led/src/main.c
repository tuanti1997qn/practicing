#include <stdio.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#define LED0_NODE DT_ALIAS(led01)

const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void) {
  bool led_state = false;

  if (!gpio_is_ready_dt(&led)) {
    return -1;
  }

  gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
  while (1) {
    led_state = !led_state;
    gpio_pin_set_dt(&led, (int)led_state);
    k_msleep(1000);
  }

  return 0;
}
