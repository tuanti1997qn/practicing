#include "zephyr/kernel.h"
#include <stdio.h>
#include <zephyr/drivers/gpio.h>

#define LED0_NODE DT_ALIAS(led0)
#define BTN0_NODE DT_ALIAS(sw0)

struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BTN0_NODE, gpios);
static struct gpio_callback button_cb_data;
void button_pressed(const struct device *dev, struct gpio_callback *cb,
                    uint32_t pins) {
  gpio_pin_toggle(led.port, led.pin);
  printf("Button pressed! LED state toggled.\n");
}

int main(void) {

  if (!gpio_is_ready_dt(&led)) {
    printf("Error: LED device %s is not ready\n", led.port->name);
    return 1;
  }
  if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
    printf("Error: Failed to configure LED pin\n");
    return 1;
  }
  if (!gpio_is_ready_dt(&button)) {
    printf("Error: Button device %s is not ready\n", button.port->name);
    return 1;
  }

  if (gpio_pin_configure_dt(&button, GPIO_INPUT) < 0) {
    printf("Error: Failed to configure button pin\n");
    return 1;
  }

  if (gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE) < 0) {
    printf("Error: Failed to configure button pin\n");
    return 1;
  }

  gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));

  gpio_add_callback(button.port, &button_cb_data);
  while (true) {
    k_sleep(K_FOREVER);
  }
  return 0;
}
