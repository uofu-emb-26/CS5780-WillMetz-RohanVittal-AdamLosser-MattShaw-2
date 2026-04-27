#ifndef ADC_MIC_H
#define ADC_MIC_H

#include <stdint.h>

void mic_init(void);
uint16_t mic_measure_peak_to_peak(uint32_t sample_count);

#endif /* ADC_MIC_H */
