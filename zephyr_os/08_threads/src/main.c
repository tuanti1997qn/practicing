#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024
#define PRIORITY 7
const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

void blink_led0(void) {
  if (!gpio_is_ready_dt(&led0)) {
    printk("Error: LED0 is not ready\n");
    return;
  }
  int ret;
  ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
  if (ret < 0) {
    printk("Error: failed to configure LED0 \n");
    return;
  }
  while (1) {
    gpio_pin_toggle_dt(&led0);
    printk("LED0 toggled\n");
    k_msleep(1000);
  }
}

void blink_led1(void *p1, void *p2, void *p3) {
  if (!gpio_is_ready_dt(&led1)) {
    printk("Error: LED1 is not ready\n");
    return;
  }
  int ret;
  ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
  if (ret < 0) {
    printk("Error: failed to configure LED1 \n");
    return;
  }
  while (1) {
    gpio_pin_toggle_dt(&led1);
    printk("LED1 toggled\n");
    k_msleep(500);
  }
}

// Define stack statically
// K_THREAD_STACK_DEFINE(blink_led1_stack, STACK_SIZE);

int main(void) {
  struct k_thread blink_led1_thread;
  // define stack dynamically, have to config dynamic thread in prj.conf
  k_thread_stack_t *blink_led1_stack = k_thread_stack_alloc(STACK_SIZE, 0);
  k_thread_create(&blink_led1_thread, blink_led1_stack, STACK_SIZE, blink_led1,
                  NULL, NULL, NULL, PRIORITY, K_USER, K_NO_WAIT);

  // create second thread, this will fail if pool size is 1
  // struct k_thread blink_led2_thread;
  // k_thread_stack_t *blink_led2_stack = k_thread_stack_alloc(STACK_SIZE, 0);
  // printk("Got stack %p\n", blink_led2_stack);

  while (1) {
    k_msleep(1000);
  }
  return 0;
}

// static define threads
K_THREAD_DEFINE(blink_led0_id, STACK_SIZE, blink_led0, NULL, NULL, NULL,
                PRIORITY, 0, 0);
// K_THREAD_DEFINE(blink_led1_id, STACK_SIZE, blink_led1, NULL, NULL, NULL,
//                 PRIORITY, 0, 0);
