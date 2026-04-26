#ifndef NIXIE_DRIVERS_H
#define NIXIE_DRIVERS_H

#include <stdint.h>

void write_int_to_tubes(uint16_t integer_to_write);
void init_tube_gpios(void);
void test_tubes(void);

#endif /* NIXIE_DRIVERS_H */