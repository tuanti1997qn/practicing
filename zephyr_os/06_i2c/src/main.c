#include <stdint.h>
#include <sys/types.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

#define I2C_DEV DT_NODELABEL(i2c0)

static const struct device *i2c_dev = DEVICE_DT_GET(I2C_DEV);

int main(void) {
  if (!device_is_ready(i2c_dev)) {
    printk("I2C device not ready\n");
    return -1;
  }

  // send hello world
  uint8_t send_buff[17];
  send_buff[0] = 0;
  send_buff[1] = 0;
  sprintf(&send_buff[2], "Hello, World!\n");
  u_int8_t receive_buff[15];

  // Write data to eeprom
  int ret = i2c_write(i2c_dev, send_buff, sizeof(send_buff), 0x50);
  if (ret) {
    printk("I2C write failed: %d\n", ret);
    return ret;
  }

  // delay 100ms in order to let the eeprom complete the write cycle
  k_sleep(K_MSEC(100));

  // Read data back from eeprom
  ret = i2c_write_read(i2c_dev, 0x50, send_buff, 2, receive_buff,
                       sizeof(receive_buff));
  if (ret) {
    printk("I2C read failed: %d\n", ret);
    return ret;
  }

  printk("Received data: %s\n", receive_buff);
  return 0;
}
