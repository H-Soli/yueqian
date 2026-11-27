#ifndef KEY_H
#define KEY_H
/* Raw PA23 level: 1=released, 0=pressed; updated by the 1 ms scan. */
extern volatile unsigned int key0_level;
void key_scan_1ms(void);
#endif
