#include <stdio.h>
#include <zephyr/drivers/spi.h>

#define SPI_NODE DT_NODELABEL(w25q64_spi)
const struct spi_dt_spec spi_dev = SPI_DT_SPEC_GET(
    SPI_NODE, SPI_WORD_SET(8) | SPI_TRANSFER_MSB, 0);

int main(void) {
  if (!spi_is_ready_dt(&spi_dev)) {
    printf("SPI device not ready\n");
    return -1;
  }

  // prepare buffers
  uint8_t tx_buf[3];
  struct spi_buf tx_spi_buf = {
      .buf = tx_buf,
      .len = sizeof(tx_buf),
  };
  struct spi_buf_set tx = {
      .buffers = &tx_spi_buf,
      .count = 1,
  };

  uint8_t rx_buf[3];
  struct spi_buf rx_spi_buf = {
      .buf = rx_buf,
      .len = sizeof(rx_buf),
  };
  struct spi_buf_set rx = {
      .buffers = &rx_spi_buf,
      .count = 1,
  };

  // perform query W25Q flash ID
  // send reset command
  tx_buf[0] = 0x66; // reset enable
  tx_buf[1] = 0x99; // reset memory
  if (spi_transceive_dt(&spi_dev, &tx, NULL) != 0) {
    printf("SPI transceive failed\n");
    return -1;
  }

  k_msleep(1); // wait for reset to complete

  // send read ID command
  tx_buf[0] = 0x9F; // read ID command
  if (spi_transceive_dt(&spi_dev, &tx, &rx) != 0) {
    printf("SPI transceive failed\n");
    return -1;
  }

  printf("Flash ID: %02X %02X\n", rx_buf[1], rx_buf[2]);

  return 0;
}
