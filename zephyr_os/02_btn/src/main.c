#include <stdio.h>
#include <zephyr/drivers/gpio.h>

#define LED0_NODE DT_ALIAS(led0)
#define BUTTON0_NODE DT_ALIAS(sw0)

static struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON0_NODE, gpios);
int main(void) {

  if (!device_is_ready(led.port)) {
    return -1;
  }
  if (!device_is_ready(button.port)) {
    return -1;
  }
  gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
  gpio_pin_configure_dt(&button, GPIO_INPUT);
  while (1) {
    int val = gpio_pin_get_dt(&button);
    gpio_pin_set_dt(&led, val);
    k_msleep(100);
  }
  return 0;
}
