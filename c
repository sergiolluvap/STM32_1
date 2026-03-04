/* --- Variables Globales / Buffers de Datos --- */
uint32_t raw_sensor_data[12] = {1050, 1020, 1050, 1080, 1050, 1100, 1050, 1030, 1050, 1070, 1050, 1090};

// Estructura para guardar los resultados finales
StatsResults_t sensorStats;

void vStatsProcessorTask(void *pvParameters) {
    while (1) {
        // Llenamos la estructura usando nuestras funciones
        sensorStats.min      = stats_u32_min(raw_sensor_data, 12);
        sensorStats.max      = stats_u32_max(raw_sensor_data, 12);
        sensorStats.mean     = stats_u32_mean(raw_sensor_data, 12);



#include "stats.h"

/* --- Implementación para uint32_t --- */

float stats_u32_min(const uint32_t *data, uint16_t size) {
    if (size == 0) return 0;
    uint32_t min = data[0];
    for (uint16_t i = 1; i < size; i++) if (data[i] < min) min = data[i];
    return (float)min;
}

float stats_u32_max(const uint32_t *data, uint16_t size) {
    if (size == 0) return 0;
    uint32_t max = data[0];
    for (uint16_t i = 1; i < size; i++) if (data[i] > max) max = data[i];
    return (float)max;
}

float stats_u32_mean(const uint32_t *data, uint16_t size) {
    if (size == 0) return 0;
    uint64_t sum = 0; // Usamos 64 bits para evitar overflow en la suma
    for (uint16_t i = 0; i < size; i++) sum += data[i];
    return (float)sum / (float)size;
}

float stats_u32_median(uint32_t *data, uint16_t size) {
    if (size == 0) return 0;
    // Bubble sort (u32 version)
    for (uint16_t i = 0; i < size - 1; i++) {
        for (uint16_t j = 0; j < size - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                uint32_t temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
    if (size % 2 == 0) return (float)(data[size/2 - 1] + data[size/2]) / 2.0f;
    return (float)data[size/2];
}

float stats_u32_variance(const uint32_t *data, uint16_t size) {
    if (size == 0) return 0;
    float mean = stats_u32_mean(data, size);
    float sum_sq_diff = 0;
    for (uint16_t i = 0; i < size; i++) {
        float diff = (float)data[i] - mean;
        sum_sq_diff += diff * diff;
    }
    return sum_sq_diff / (float)size;
}

float stats_u32_std_dev(const uint32_t *data, uint16_t size) {
    return sqrtf(stats_u32_variance(data, size));
}

// Nota: stats_u32_mode se implementaría igual que el ejemplo float del post anterior
// pero comparando uint32_t.




#ifndef STATS_H
#define STATS_H

#include <stdint.h>
#include <math.h>

/* Estructura para almacenar los resultados estadísticos */
typedef struct {
    float min;
    float max;
    float mean;
    float median;
    float mode;
    float variance;
    float std_dev;
} StatsResults_t;

/* --- Prototipos para uint8_t --- */
float stats_u8_min(const uint8_t *data, uint16_t size);
float stats_u8_max(const uint8_t *data, uint16_t size);
float stats_u8_mean(const uint8_t *data, uint16_t size);
float stats_u8_median(uint8_t *data, uint16_t size);
float stats_u8_mode(const uint8_t *data, uint16_t size);
float stats_u8_variance(const uint8_t *data, uint16_t size);
float stats_u8_std_dev(const uint8_t *data, uint16_t size);

/* --- Prototipos para uint32_t --- */
float stats_u32_min(const uint32_t *data, uint16_t size);
float stats_u32_max(const uint32_t *data, uint16_t size);
float stats_u32_mean(const uint32_t *data, uint16_t size);
float stats_u32_median(uint32_t *data, uint16_t size);
float stats_u32_mode(const uint32_t *data, uint16_t size);
float stats_u32_variance(const uint32_t *data, uint16_t size);
float stats_u32_std_dev(const uint32_t *data, uint16_t size);

#endif

#include "stats.h"

float stats_min(const float *data, uint16_t size) {
    if (size == 0) return 0.0f;
    float min = data[0];
    for (uint16_t i = 1; i < size; i++) {
        if (data[i] < min) min = data[i];
    }
    return min;
}

float stats_max(const float *data, uint16_t size) {
    if (size == 0) return 0.0f;
    float max = data[0];
    for (uint16_t i = 1; i < size; i++) {
        if (data[i] > max) max = data[i];
    }
    return max;
}

float stats_mean(const float *data, uint16_t size) {
    if (size == 0) return 0.0f;
    float sum = 0.0f;
    for (uint16_t i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum / (float)size;
}

float stats_median(float *data, uint16_t size) {
    if (size == 0) return 0.0f;
    
    // Ordenamiento simple (Bubble Sort) - In-place
    for (uint16_t i = 0; i < size - 1; i++) {
        for (uint16_t j = 0; j < size - i - 1; j++) {
            if (data[j] > data[j + 1]) {
                float temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }

    if (size % 2 == 0) {
        return (data[size / 2 - 1] + data[size / 2]) / 2.0f;
    } else {
        return data[size / 2];
    }
}

float stats_mode(const float *data, uint16_t size) {
    if (size == 0) return 0.0f;
    float mode = data[0];
    int max_count = 0;

    for (uint16_t i = 0; i < size; i++) {
        int count = 0;
        for (uint16_t j = 0; j < size; j++) {
            if (data[j] == data[i]) count++;
        }
        if (count > max_count) {
            max_count = count;
            mode = data[i];
        }
    }
    return mode;
}

float stats_variance(const float *data, uint16_t size) {
    if (size == 0) return 0.0f;
    float mean = stats_mean(data, size);
    float sum_sq_diff = 0.0f;
    for (uint16_t i = 0; i < size; i++) {
        float diff = data[i] - mean;
        sum_sq_diff += diff * diff;
    }
    return sum_sq_diff / (float)size;
}

float stats_std_dev(const float *data, uint16_t size) {
    return sqrtf(stats_variance(data, size));
}

#ifndef STATS_H
#define STATS_H

#include <stdint.h>
#include <math.h>

/* Prototipos de funciones */
float stats_min(const float *data, uint16_t size);
float stats_max(const float *data, uint16_t size);
float stats_mean(const float *data, uint16_t size);
float stats_median(float *data, uint16_t size); // Nota: Ordena el array original
float stats_mode(const float *data, uint16_t size);
float stats_variance(const float *data, uint16_t size);
float stats_std_dev(const float *data, uint16_t size);

#endif /* STATS_H */


Conceptos de Optimización con TCM
ITCM (Instruction TCM): Ideal para el código crítico (la función de cálculo). Al ejecutar desde aquí, el procesador no depende de la I-Cache.

DTCM (Data TCM): Ideal para datos críticos (la tabla de búsqueda LUT y el buffer de datos). Se accede a la velocidad del núcleo (216 MHz) sin pasar por el bus AXI.


/* Memorias del STM32F767ZIT6 */
MEMORY
{
  ITCMRAM (xrw)      : ORIGIN = 0x00000000, LENGTH = 16K
  DTCMRAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 128K
  RAM (xrw)          : ORIGIN = 0x20020000, LENGTH = 368K
  FLASH (rx)         : ORIGIN = 0x08000000, LENGTH = 2048K
}

SECTIONS
{
  /* ... (secciones estándar .text, .rodata, etc.) ... */

  /* Sección para código en ITCM */
  .itcm_text :
  {
    . = ALIGN(4);
    _sitcm_text = .;        /* Inicio en RAM (VMA) */
    *(.itcm_text)
    *(.itcm_text*)
    . = ALIGN(4);
    _eitcm_text = .;        /* Fin en RAM */
  } >ITCMRAM AT> FLASH
  
  _sitcm_text_flash = LOADADDR(.itcm_text); /* Dirección de carga en Flash (LMA) */

  /* Sección para datos en DTCM */
  .dtcm_data :
  {
    . = ALIGN(4);
    _sdtcm_data = .;
    *(.dtcm_data)
    *(.dtcm_data*)
    . = ALIGN(4);
    _edtcm_data = .;
  } >DTCMRAM AT> FLASH
  
  _sdtcm_data_flash = LOADADDR(.dtcm_data);
  
  /* ... (resto del linker script) ... */
}





crc.h
// Añadir el atributo de sección a la tabla (en el .h si es static const)
// O mejor, quitar static y definirla en el .c con el atributo.
extern const uint16_t crc_ccitt16_table[256] __attribute__((section(".dtcm_data")));

uint16_t CRC(uint16_t initial_crc, uint8_t *array, uint16_t len) __attribute__((section(".itcm_text")));


crc.c
// Colocamos la función en ITCM
__attribute__((section(".itcm_text")))
uint16_t CRC(uint16_t initial_crc, uint8_t *array, uint16_t len)
{


main.c
/* Símbolos del Linker Script */
extern uint32_t _sitcm_text, _eitcm_text, _sitcm_text_flash;
extern uint32_t _sdtcm_data, _edtcm_data, _sdtcm_data_flash;

/* Buffer de prueba en DTCM para máxima velocidad */
uint8_t data_test[512] __attribute__((section(".dtcm_data")));


void Copy_To_TCM(void) {
    uint32_t *pSrc, *pDest;
    
    // Copiar código a ITCM
    pSrc = &_sitcm_text_flash;
    pDest = &_sitcm_text;
    while(pDest < &_eitcm_text) *pDest++ = *pSrc++;
    
    // Copiar datos/LUT a DTCM
    pSrc = &_sdtcm_data_flash;
    pDest = &_sdtcm_data;
    while(pDest < &_edtcm_data) *pDest++ = *pSrc++;
}

DWT_Init();
    
// 2. IMPORTANTE: Copiar lógica a TCM antes de ejecutar
Copy_To_TCM();

Linker Script modificado sea el que está seleccionando el proyecto en Project Settings -> C/C++ Build -> Settings -> MCU GCC Linker -> General

Sin Cache Misses: En el modo normal (Flash/RAM estándar), si la caché decide expulsar la tabla LUT para guardar otra variable, el siguiente cálculo de CRC será lento. En DTCM, la tabla está "pegada" al procesador para siempre.

Paralelismo de Bus: Al usar ITCM para las instrucciones y DTCM para los datos, el procesador puede leer la siguiente instrucción y el siguiente dato de la tabla simultáneamente en el mismo ciclo de reloj, ya que usan buses independientes.

Resultados: Notarás que la StdDev (desviación estándar) del método por software bajará drásticamente, volviéndose extremadamente predecible y rápido, compitiendo directamente con el hardware en tiempos totales para buffers medianos.

// Limpiar caché por seguridad (aunque en DTCM no es estrictamente necesario
