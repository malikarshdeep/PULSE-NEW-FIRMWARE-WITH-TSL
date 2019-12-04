//#include "temperature.h"
//#include "math.h"
//#include "onewire.h"
//#include "tcptest.h"
//#include "nrf_log.h"
//
//static int isKthBitSet(int n, int k) {
//  if (n & (1 << (k - 1)))
//    return 1;
//  else
//    return 0;
//  ;
//}
//
//uint8_t dsCRC8(const uint8_t *addr, uint8_t len) //begins from LS-bit of LS-byte (OneWire.h)
//{
//  uint8_t crc = 0;
//  while (len--) {
//    uint8_t inbyte = *addr++;
//    for (uint8_t i = 8; i; i--) {
//      uint8_t mix = (crc ^ inbyte) & 0x01;
//      crc >>= 1;
//      if (mix)
//        crc ^= 0x8C;
//      inbyte >>= 1;
//    }
//  }
//  return crc;
//}
//
//float Read_Temperature(uint32_t pin) {
//  char get[9];
//  char temp_lsb, temp_msb, test;
//  int k;
//  char temp_f, temp_c;
//  float temp;
//  uint8_t buff[2];
//  uint8_t crc1;
//
//  if (onewire_reset(pin))
//    return NAN;
//  buff[0] = 0xcc; //skip ROM
//  buff[1] = 0x44; //start conversion
//  onewire_write(pin, buff, 2);
//
//  if (onewire_reset(pin))
//    return NAN;
//  buff[0] = 0xcc; //skip ROM
//  buff[1] = 0xbe; //read scratchpad
//  onewire_write(pin, buff, 2);
//  if (onewire_read(pin, get, 9))
//    return NAN;
//  // printf("\n ScratchPAD DATA = %X%X%X%X%X%X%X%X%X\n",get[8],get[7],get[6],get[5],get[4],get[3],get[2],get[1],get[0]);
//
//  crc1 = dsCRC8(get, 8);
//
//  // NRF_LOG_INFO("CRC = %d", crc1);
//  // NRF_LOG_INFO("get9 = %d", get[8]);
//
//  temp_msb = get[1];
//  temp_lsb = get[0];
//
//  if (crc1 == get[8]) {
//    temp = (isKthBitSet(get[1], 3)) * 64 + (isKthBitSet(get[1], 2)) * 32 + (isKthBitSet(get[1], 1)) * 16 + (isKthBitSet(get[0], 8)) * 8 + (isKthBitSet(get[0], 7)) * 4 + (isKthBitSet(get[0], 6)) * 2 + (isKthBitSet(get[0], 5)) * 1 + (isKthBitSet(get[0], 4)) * 0.5 + (isKthBitSet(get[0], 3)) * 0.25 + (isKthBitSet(get[0], 2)) * 0.125 + (isKthBitSet(get[0], 1)) * 0.0625;
//
//  if (pin == 33) {
//    sensors_state1.sltc11 = temp;
//    sensors_state2.sltc11 = temp;
//    sensors_state3.sltc11 = temp;
//    sensors_state4.sltc11 = temp;
//   // NRF_LOG_INFO("TEMP1: %d", temp);
// 
//  }
//
//  else if (pin == 11) {
//    sensors_state1.lltc11 = temp;
//    sensors_state2.lltc11 = temp;
//    sensors_state3.lltc11 = temp;
//    sensors_state4.lltc11 = temp;
//  //  NRF_LOG_INFO("TEMP2: %d", temp);
//  }
//
// // else if (pin == 6) {
// //   sensors_state1.dlt10 = temp;
//  ///  sensors_state2.dlt10 = temp;
//  //  sensors_state3.dlt10 = temp;
// //   sensors_state4.dlt10 = temp;
// //  NRF_LOG_INFO("TEMP3: %d", temp);
//
// // }
//
//  else if (pin == 40) {
//  //  sensors_state1.sltc21 = temp;
//    sensors_state2.sltc21 = temp;
//    sensors_state3.sltc21 = temp;
//    sensors_state4.sltc21 = temp;
//  //  NRF_LOG_INFO("TEMP3: %d", temp);
//  }
//
//  else if (pin == 8) {
//  //  sensors_state1.lltc21 = temp;
//   sensors_state2.lltc21 = temp;
//    sensors_state3.lltc21 = temp;
//    sensors_state4.lltc21 = temp;
//  //  NRF_LOG_INFO("TEMP4: %d", temp);
//  }
//
////  else if (pin == 40) {
//   // sensors_state1.dlt20 = temp;
// //   sensors_state2.dlt20 = temp;
// //   sensors_state3.dlt20 = temp;
// //   sensors_state4.dlt20 = temp;
// //   NRF_LOG_INFO("TEMP6: %d", temp);
// // }
//
//  else if (pin == 7) {
//    //sensors_state1.sltc31 = temp;
//   // sensors_state2.sltc31 = temp;
//    sensors_state3.sltc31 = temp;
//    sensors_state4.sltc31 = temp;
//  //  NRF_LOG_INFO("TEMP5: %d", temp);
//  }
//
//  else if (pin == 6) {
//   // sensors_state1.lltc31 = temp;
//  //  sensors_state2.lltc31 = temp;
//    sensors_state3.lltc31 = temp;
//    sensors_state4.lltc31 = temp;
//   // NRF_LOG_INFO("TEMP6: %d", temp);
//  }
//
// // else if (pin == 34) {
//   // sensors_state1.dlt30 = temp;
//   // sensors_state2.dlt30 = temp;
//  //  sensors_state3.dlt30 = temp;
//  //  sensors_state4.dlt30 = temp;
// //
////  }
//
//  else if (pin == 27) {
//  //  sensors_state1.sltc41 = temp;
//  //  sensors_state2.sltc41 = temp;
//  //  sensors_state3.sltc41 = temp;
//    sensors_state4.sltc41 = temp;
//   //  NRF_LOG_INFO("TEMP7: %d", temp);
//  }
//
//  else if (pin == 44) {
// //   sensors_state1.lltc41 = temp;
// //   sensors_state2.lltc41 = temp;
// //   sensors_state3.lltc41 = temp;
//    sensors_state4.lltc41 = temp;
//     NRF_LOG_INFO("TEMP8: %d", temp);
//  }
//
////  else if (pin == 37) {
// //   sensors_state1.dlt40 = temp;
// //   sensors_state2.dlt40 = temp;
// //   sensors_state3.dlt40 = temp;
// //   sensors_state4.dlt40 = temp;
// // }
//} else {
//
//
// //    if (pin == 33) {
// 
//// ++wrong_sltc11;
//// if(wrong_sltc11 == 20){
//// sensors_state1.sltc11 = 0;
//// wrong_sltc11 = 0;
// 
// // }
////}
////  else if (pin == 11) {
//
//// ++wrong_lltc11;
//// if(wrong_lltc11 == 20){
//// sensors_state1.lltc11 = 0;
//// wrong_lltc11 = 0;
// // }
////}
//
// // else if (pin == 40) {
// //  ++wrong_sltc21;
// //if(wrong_sltc21 == 20){
// //sensors_state2.sltc21 = 0;
// //wrong_sltc21 = 0;
//
// // }
////}
// // else if (pin == 8) {
//// ++wrong_lltc21;
//// if(wrong_lltc21 == 20){
//// sensors_state2.lltc21 = 0;
//// wrong_lltc21 = 0;
////  }
//
////}
////  else if (pin == 7) {
////   ++wrong_sltc31;
//// if(wrong_sltc31 == 20){
//// sensors_state3.sltc31 = 0;
//// wrong_sltc31 = 0;
// // }
////}
// // else if (pin == 6) {
//// ++wrong_lltc31;
// //if(wrong_lltc31 == 20){
// //sensors_state3.lltc31 = 0;
// //wrong_lltc31 = 0;
// // }
//
//
////}
// // else if (pin == 27) {
// //  ++wrong_sltc41;
//// if(wrong_sltc41 == 20){
//// sensors_state4.sltc41 = 0;
//// wrong_sltc41 = 0;
// // }
////}
// // else if (pin == 44) {
//// ++wrong_lltc41;
//// if(wrong_lltc41 == 20){
//// sensors_state4.lltc41 = 0;
//// wrong_lltc41 = 0;
// // }
////}
//
//
//
//  //   NRF_LOG_INFO("TEMP wrong crc at pin %i", pin);
//    return NAN;
//  }
//  return temp;
//}



#include "boards.h"
#include "math.h"
#include "tcptest.h"
#include "temperature.h"
#include "nrf_delay.h"
#include "nrf_log.h"

uint16_t bad_sltc11 =0;
uint16_t bad_lltc11 =0;
uint16_t bad_sltc21 =0;
uint16_t bad_lltc21 =0;
uint16_t bad_sltc31 =0;
uint16_t bad_lltc31 =0;
uint16_t bad_sltc41 =0;
uint16_t bad_lltc41 =0;
uint8_t flag_sltc11, flag_lltc11, flag_sltc21, flag_lltc21, flag_sltc31, flag_lltc31, flag_sltc41, flag_lltc41 = 0;
uint8_t x1_s1,x2_s1,x3_s1,x4_s1,x5_s1,x6_s1,x1_l1,x2_l1,x3_l1,x4_l1,x5_l1,x6_l1,x1_s4,x2_s4,x3_s4,x4_s4,x5_s4,x6_s4,x1_l4,x2_l4,x3_l4,x4_l4,x5_l4,x6_l4,x1_s2,x2_s2,x3_s2,x4_s2,x5_s2,x6_s2,x1_l2,x2_l2,x3_l2,x4_l2,x5_l2,x6_l2,x1_s3,x2_s3,x3_s3,x4_s3,x5_s3,x6_s3,x1_l3,x2_l3,x3_l3,x4_l3,x5_l3,x6_l3,x1_s5,x2_s5,x3_s5,x4_s5,x5_s5,x6_s5,x1_l5,x2_l5,x3_l5,x4_l5,x5_l5,x6_l5,x1_s6,x2_s6,x3_s6,x4_s6,x5_s6,x6_s6,x1_l6,x2_l6,x3_l6,x4_l6,x5_l6,x6_l6,x1_s7,x2_s7,x3_s7,x4_s7,x5_s7,x6_s7,x1_l7,x2_l7,x3_l7,x4_l7,x5_l7,x6_l7,x1_s8,x2_s8,x3_s8,x4_s8,x5_s8,x6_s8,x1_l8,x2_l8,x3_l8,x4_l8,x5_l8,x6_l8;

static void init(int m) {

  nrf_gpio_cfg_output(m);

  nrf_gpio_pin_write(m, 1);
  //DHT11_ONE_WIRE_OUTPUT_PIN = 1;

  nrf_gpio_pin_write(m, 0);
  nrf_delay_us(605);
  //ets_delay_us(22000);

  nrf_gpio_pin_write(m, 1);
  //	Delay_micro_second(30);
  nrf_delay_us(65);

  nrf_gpio_cfg_input(m, NRF_GPIO_PIN_PULLUP);
  //NRF_LOG_INFO("PRESENCE:  %d",nrf_gpio_pin_read(m));
  
  nrf_delay_us(520);

}

//static void write_bitt(uint8_t v, int r) {
//
//  if (v == 1) {
//   nrf_gpio_pin_write(r, 1);
//   nrf_gpio_pin_write(r, 0);
//    nrf_delay_us(10);
//    nrf_gpio_pin_write(r, 1);
//   nrf_delay_us(104);
//   // nrf_gpio_pin_write(r, 1);
//
//  } else {
//
//   nrf_gpio_pin_write(r, 1);
//    nrf_gpio_pin_write(r, 0);
//   //  nrf_delay_us(10);
//  //  nrf_gpio_pin_write(r, 0);
//    nrf_delay_us(104);
//    nrf_gpio_pin_write(r, 1);
//  }
//}

static void write_bitt(uint8_t v, int r) {

   
    nrf_gpio_pin_write(r, 0);

    if (v == 1) { 
    nrf_delay_us(15);
    nrf_gpio_pin_write(r, 1);}
     
    nrf_delay_us(70);
    nrf_gpio_pin_write(r, 1);
    
}

static void write(uint8_t v, int n) {
  nrf_gpio_cfg_output(n);

  uint8_t bitMask;

  for (bitMask = 0x01; bitMask; bitMask <<= 1) {
    write_bitt((bitMask & v) ? 1 : 0, n);
    //OneWire::write_bit( (bitMask & v)?1:0);
  }
nrf_delay_us(104);
//NRF_LOG_INFO("written");
}

//static void write_bit(char bitval) {
 // nrf_gpio_cfg_output(34);

 // nrf_gpio_pin_write(34, 0);
 // if (bitval == 1)
 //   nrf_gpio_pin_write(34, 1); // return DQ high if write 1
 // nrf_delay_us(85);            // hold value for remainder of timeslot
 // nrf_gpio_pin_write(34, 1);
//} // Delay provides 16us per loop, plus 24us. Therefore delay(5) = 104us

//void write_byte(char val) {
 // unsigned char i;
 // unsigned char temp;
 // for (i = 0; i < 8; i++) // writes byte, one bit at a time
 // {
 //   temp = val >> i; // shifts val right 'i' spaces
  //  temp &= 0x01;    // copy that bit to temp
  //  write_bit(temp); // write bit in temp into
 // }
 // nrf_delay_us(85);
//}

static int isKthBitSet(int n, int k) {
  if (n & (1 << (k - 1)))
    return 1;
  else
    return 0;
  ;
}

static unsigned char read_bit(int q) {
  unsigned char i;
  nrf_gpio_cfg_output(q);
  nrf_gpio_pin_write(q, 0); // pull DQ low to start timeslot
  nrf_delay_us(5);
  nrf_gpio_pin_write(q, 1); // then return high
  nrf_gpio_cfg_input(q, NRF_GPIO_PIN_NOPULL);
  nrf_delay_us(15);              // delay 15us from start of timeslot
  return (nrf_gpio_pin_read(q)); // return value of DQ line
}

static unsigned char read_byte(int s) {

  unsigned char i;
  unsigned char value = 0;
  for (i = 0; i < 8; i++) {
    if (read_bit(s))
      value |= 0x01 << i; // reads byte in, one byte at a time and then
    // shifts it left
    nrf_delay_us(125); // wait for rest of timeslot
  }
//    NRF_LOG_INFO("value read  %d", value);
  return (value);

}


uint8_t dsCRC8(const uint8_t *addr, uint8_t len)//begins from LS-bit of LS-byte (OneWire.h)
{
  uint8_t crc = 0;
  while (len--)
  {
    uint8_t inbyte = *addr++;
    for (uint8_t i = 8; i; i--)
    {
      uint8_t mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if (mix) crc ^= 0x8C;
      inbyte >>= 1;
    }
  }
  return crc;
}




//void Read_Temperature(int x) {
//  char get[9];
//  char temp_lsb, temp_msb, test;
//  int k;
//  char temp_f, temp_c;
//  uint8_t crc1;
//  float temp;
//
//  init(x);
//  //nrf_delay_us(100);
//  write(0xCC, x); //Skip ROM
//  write(0x44, x); // Start Conversion
//  nrf_delay_us(104);
//  init(x);
//  //nrf_delay_us(100);
//  write(0xCC, x); // Skip ROM
//  write(0xBE, x); // Read Scratch Pad
//  for (k = 0; k < 9; k++) {
//    get[k] = read_byte(x);
//  }
// // printf("\n ScratchPAD DATA = %X%X%X%X%X%X%X%X%X\n",get[8],get[7],get[6],get[5],get[4],get[3],get[2],get[1],get[0]);
//
//   crc1 = dsCRC8(get, 8);
//
//  // NRF_LOG_INFO("CRC = %d", crc1);
//  // NRF_LOG_INFO("get8 = %d", get[8]);
//  //  NRF_LOG_INFO("get0 = %d", get[0]);
//
//  temp_msb = get[1];
//  temp_lsb = get[0];
//
// 
//
//if(isKthBitSet(get[1], 4)==1 && isKthBitSet(get[1], 5)==1 && isKthBitSet(get[1], 6)==1 && isKthBitSet(get[1], 7)==1 &&isKthBitSet(get[1], 8)==1){
//  
//   temp =  ((isKthBitSet(get[1], 2)) * 32) + (isKthBitSet(get[1], 1)) * 16 + (isKthBitSet(get[0], 8)) * 8 + (isKthBitSet(get[0], 7)) * 4 + (isKthBitSet(get[0], 6)) * 2 + (isKthBitSet(get[0], 5)) * 1 + (isKthBitSet(get[0], 4)) * 0.5 + (isKthBitSet(get[0], 3)) * 0.25 + (isKthBitSet(get[0], 2)) * 0.125 + (isKthBitSet(get[0], 1)) * 0.0625 - (isKthBitSet(get[1], 3)) * 64;
//}
//
//else{
//   temp = ((isKthBitSet(get[1], 3)) * 64) + (isKthBitSet(get[1], 2)) * 32 + (isKthBitSet(get[1], 1)) * 16 + (isKthBitSet(get[0], 8)) * 8 + (isKthBitSet(get[0], 7)) * 4 + (isKthBitSet(get[0], 6)) * 2 + (isKthBitSet(get[0], 5)) * 1 + (isKthBitSet(get[0], 4)) * 0.5 + (isKthBitSet(get[0], 3)) * 0.25 + (isKthBitSet(get[0], 2)) * 0.125 + (isKthBitSet(get[0], 1)) * 0.0625;
//}
//  
// if(crc1 == get[8]){
////  if(get[8] != 255 && temp != 85 && temp>-55 && temp<125){
// // NRF_LOG_INFO("TEMP %d",temp);
//  //printf("\n ScratchPAD DATA = %d\n",temp);
//
//  if (x == 33) {
//    sensors_state1.sltc11 = temp;
//    sensors_state2.sltc11 = temp;
//    sensors_state3.sltc11 = temp;
//    sensors_state4.sltc11 = temp;
//   // NRF_LOG_INFO("TEMP1: %d", temp);
//   bad_sltc11 = 0;
// 
//  }
//
//  else if (x == 11) {
//    sensors_state1.lltc11 = temp;
//    sensors_state2.lltc11 = temp;
//    sensors_state3.lltc11 = temp;
//    sensors_state4.lltc11 = temp;
//   // NRF_LOG_INFO("TEMP2: %d", temp);
//   bad_lltc11 = 0;
//  }
//
// // else if (x == 6) {
// //   sensors_state1.dlt10 = temp;
//  ///  sensors_state2.dlt10 = temp;
//  //  sensors_state3.dlt10 = temp;
// //   sensors_state4.dlt10 = temp;
// //   NRF_LOG_INFO("TEMP3: %d", temp);
//
// // }
//
//  else if (x == 40) {
//  //  sensors_state1.sltc21 = temp;
//    sensors_state2.sltc21 = temp;
//    sensors_state3.sltc21 = temp;
//    sensors_state4.sltc21 = temp;
//   // NRF_LOG_INFO("TEMP3: %d", temp);
//   bad_sltc21 = 0;
//  }
//
//  else if (x == 8) {
//  //  sensors_state1.lltc21 = temp;
//   sensors_state2.lltc21 = temp;
//    sensors_state3.lltc21 = temp;
//    sensors_state4.lltc21 = temp;
//   // NRF_LOG_INFO("TEMP4: %d", temp);
//   bad_lltc21 = 0;
//  }
//
////  else if (x == 40) {
//   // sensors_state1.dlt20 = temp;
// //   sensors_state2.dlt20 = temp;
// //   sensors_state3.dlt20 = temp;
// //   sensors_state4.dlt20 = temp;
// //   NRF_LOG_INFO("TEMP6: %d", temp);
// // }
//
//  else if (x == 7) {
//    //sensors_state1.sltc31 = temp;
//   // sensors_state2.sltc31 = temp;
//    sensors_state3.sltc31 = temp;
//    sensors_state4.sltc31 = temp;
//    bad_sltc31 = 0;
//  //  NRF_LOG_INFO("TEMP5: %d", temp);
//  }
//
//  else if (x == 6) {
//   // sensors_state1.lltc31 = temp;
//  //  sensors_state2.lltc31 = temp;
//    sensors_state3.lltc31 = temp;
//    sensors_state4.lltc31 = temp;
//    bad_lltc31 = 0;
// //   NRF_LOG_INFO("TEMP6: %d", temp);
//  }
//
// // else if (x == 34) {
//   // sensors_state1.dlt30 = temp;
//   // sensors_state2.dlt30 = temp;
//  //  sensors_state3.dlt30 = temp;
//  //  sensors_state4.dlt30 = temp;
// //
////  }
//
//  else if (x == 27) {
//  //  sensors_state1.sltc41 = temp;
//  //  sensors_state2.sltc41 = temp;
//  //  sensors_state3.sltc41 = temp;
//    sensors_state4.sltc41 = temp;
//    bad_sltc41 = 0;
//   //  NRF_LOG_INFO("TEMP7: %d", temp);
//  }
//
//  else if (x == 44) {
// //   sensors_state1.lltc41 = temp;
// //   sensors_state2.lltc41 = temp;
// //   sensors_state3.lltc41 = temp;
//    sensors_state4.lltc41 = temp;
//    bad_lltc41 = 0;
//   //  NRF_LOG_INFO("TEMP8: %d", temp);
//  }
//
////  else if (x == 37) {
// //   sensors_state1.dlt40 = temp;
// //   sensors_state2.dlt40 = temp;
// //   sensors_state3.dlt40 = temp;
// //   sensors_state4.dlt40 = temp;
// // }
//}
//
//else{
//
////NRF_LOG_INFO("wrong temp crc");
//  if (x == 33) {
//  bad_sltc11++;
//  if(bad_sltc11>50){
//    sensors_state1.sltc11 = -32.125;
//    sensors_state2.sltc11 = -32.125;
//    sensors_state3.sltc11 = -32.125;
//    sensors_state4.sltc11 = -32.125;
//    bad_sltc11 = 0;
//    
// }
// NRF_LOG_INFO("wrong TEMP1: %d", bad_sltc11);
//  }
//
//  else if (x == 11) {
//  bad_lltc11++;
//  if(bad_lltc11>50){
//    sensors_state1.lltc11 = -32.125;
//    sensors_state2.lltc11 = -32.125;
//    sensors_state3.lltc11 = -32.125;
//    sensors_state4.lltc11 = -32.125;
//    bad_lltc11 = 0;
//    
//   }
// //  NRF_LOG_INFO("TEMP2: %d", bad_lltc11);
//  }
//
//
//  else if (x == 40) {
//  bad_sltc21++;
//  if(bad_sltc21>50){
//  //  sensors_state1.sltc21 = temp;
//    sensors_state2.sltc21 = -32.125;
//    sensors_state3.sltc21 = -32.125;
//    sensors_state4.sltc21 = -32.125;
//    bad_sltc21 = 0;
//    
//   }
//  // NRF_LOG_INFO("TEMP3: %d", bad_sltc21);
//  }
//
//  else if (x == 8) {
//  bad_lltc21++;
//  if(bad_lltc21>50){
//  //  sensors_state1.lltc21 = temp;
//   sensors_state2.lltc21 =  -32.125;
//    sensors_state3.lltc21 = -32.125;
//    sensors_state4.lltc21 = -32.125;
//    bad_lltc21 = 0;
//    
//   }
// //  NRF_LOG_INFO("TEMP4: %d", bad_lltc21);
//  }
//
//
//  else if (x == 7) {
//  bad_sltc31++;
//  if(bad_sltc31>50){
//    //sensors_state1.sltc31 = temp;
//   // sensors_state2.sltc31 = temp;
//    sensors_state3.sltc31 = -32.125;
//    sensors_state4.sltc31 = -32.125;
//    
//  bad_sltc31 = 0;
//  }
// // NRF_LOG_INFO("TEMP5: %d", bad_sltc31);
//  }
//
//  else if (x == 6) {
//  bad_lltc31++;
//  if(bad_lltc31>50){
//   // sensors_state1.lltc31 = temp;
//  //  sensors_state2.lltc31 = temp;
//    sensors_state3.lltc31 = -32.125;
//    sensors_state4.lltc31 = -32.125;
// 
// bad_lltc31 = 0;
// }
//  //  NRF_LOG_INFO("TEMP6: %d", bad_lltc31);
//  }
//
//
//
//  else if (x == 27) {
//  bad_sltc41++;
//  if(bad_sltc41>50){
//  //  sensors_state1.sltc41 = temp;
//  //  sensors_state2.sltc41 = temp;
//  //  sensors_state3.sltc41 = temp;
//    sensors_state4.sltc41 = -32.125;
//    
//   bad_sltc41 =0;
//   }
//  //  NRF_LOG_INFO("TEMP7: %d", bad_sltc41);
//  }
//
//  else if (x == 44) {
//  bad_lltc41++;
//  if(bad_lltc41>50){
// //   sensors_state1.lltc41 = temp;
// //   sensors_state2.lltc41 = temp;
// //   sensors_state3.lltc41 = temp;
//    sensors_state4.lltc41 = -32.125;
//   
//    bad_lltc41 = 0;
//    }
//    //  NRF_LOG_INFO("wrong TEMP8: %d",bad_lltc41 );
//  }
//
//
//return NAN;
//
//}
//}



void Read_Temperature_sltc11(int x) {
  char get_sltc11[9];
  char temp_lsb, temp_msb, test;
  int k;
  char temp_f, temp_c;
  uint8_t crc1;
  float temp;
  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); //Skip ROM
  write(0x44, x); // Start Conversion
  nrf_delay_us(104);
  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); // Skip ROM
  write(0xBE, x); // Read Scratch Pad
  for (k = 0; k < 9; k++) {
    get_sltc11[k] = read_byte(x);
  }    
     if(flag_sltc11 ==1){
     get_sltc11[2] = x1_s1;
     get_sltc11[3] = x2_s1;
     get_sltc11[4] = x3_s1;
     get_sltc11[5] = x4_s1;
     get_sltc11[6] = x5_s1;
     get_sltc11[7] = x6_s1; 
      //  NRF_LOG_INFO("X1 yyhoyyyyi %d", x1);
     }

   crc1 = dsCRC8(get_sltc11, 8);

  temp_msb = get_sltc11[1];
  temp_lsb = get_sltc11[0];

if(isKthBitSet(get_sltc11[1], 4)==1 && isKthBitSet(get_sltc11[1], 5)==1 && isKthBitSet(get_sltc11[1], 6)==1 && isKthBitSet(get_sltc11[1], 7)==1 &&isKthBitSet(get_sltc11[1], 8)==1){
  
   temp =  ((isKthBitSet(get_sltc11[1], 2)) * 32) + (isKthBitSet(get_sltc11[1], 1)) * 16 + (isKthBitSet(get_sltc11[0], 8)) * 8 + (isKthBitSet(get_sltc11[0], 7)) * 4 + (isKthBitSet(get_sltc11[0], 6)) * 2 + (isKthBitSet(get_sltc11[0], 5)) * 1 + (isKthBitSet(get_sltc11[0], 4)) * 0.5 + (isKthBitSet(get_sltc11[0], 3)) * 0.25 + (isKthBitSet(get_sltc11[0], 2)) * 0.125 + (isKthBitSet(get_sltc11[0], 1)) * 0.0625 - (isKthBitSet(get_sltc11[1], 3)) * 64;
}

else{
   temp = ((isKthBitSet(get_sltc11[1], 3)) * 64) + (isKthBitSet(get_sltc11[1], 2)) * 32 + (isKthBitSet(get_sltc11[1], 1)) * 16 + (isKthBitSet(get_sltc11[0], 8)) * 8 + (isKthBitSet(get_sltc11[0], 7)) * 4 + (isKthBitSet(get_sltc11[0], 6)) * 2 + (isKthBitSet(get_sltc11[0], 5)) * 1 + (isKthBitSet(get_sltc11[0], 4)) * 0.5 + (isKthBitSet(get_sltc11[0], 3)) * 0.25 + (isKthBitSet(get_sltc11[0], 2)) * 0.125 + (isKthBitSet(get_sltc11[0], 1)) * 0.0625;
}



 if(crc1 == get_sltc11[8]){

    sensors_state1.sltc11 = temp;
    sensors_state2.sltc11 = temp;
    sensors_state3.sltc11 = temp;
    sensors_state4.sltc11 = temp;
  //  NRF_LOG_INFO("TEMP1: %d", temp);
    bad_sltc11 = 0;
    flag_sltc11 = 1;
    x1_s1 = get_sltc11[2];
    x2_s1 = get_sltc11[3];
    x3_s1 = get_sltc11[4];
    x4_s1 = get_sltc11[5];
    x5_s1 = get_sltc11[6];
    x6_s1 = get_sltc11[7];
  //  NRF_LOG_INFO("X1 %d", x1);
}

else{

//NRF_LOG_INFO("wrong temp crc");

  bad_sltc11++;
  if(bad_sltc11>500){
    sensors_state1.sltc11 = -32.125;
    sensors_state2.sltc11 = -32.125;
    sensors_state3.sltc11 = -32.125;
    sensors_state4.sltc11 = -32.125;
    bad_sltc11 = 0;
    
 }
// NRF_LOG_INFO("WRONG TEMP1: %d", bad_sltc11);
 return NAN;

}
}

void Read_Temperature_lltc11(int x) {
  char get_lltc11[9];
  char temp_lsb, temp_msb, test;
  int k;
  char temp_f, temp_c;
  uint8_t crc1;
  float temp;


  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); //Skip ROM
  write(0x44, x); // Start Conversion
  nrf_delay_us(104);
  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); // Skip ROM
  write(0xBE, x); // Read Scratch Pad
  for (k = 0; k < 9; k++) {
    get_lltc11[k] = read_byte(x);
  }
     
     if(flag_lltc11 ==1){
     get_lltc11[2] = x1_l1;
     get_lltc11[3] = x2_l1;
     get_lltc11[4] = x3_l1;
     get_lltc11[5] = x4_l1;
     get_lltc11[6] = x5_l1;
     get_lltc11[7] = x6_l1; 
       // NRF_LOG_INFO("X lltc yyhoyyyyi %d", x1);
     }

   crc1 = dsCRC8(get_lltc11, 8);

  temp_msb = get_lltc11[1];
  temp_lsb = get_lltc11[0];

if(isKthBitSet(get_lltc11[1], 4)==1 && isKthBitSet(get_lltc11[1], 5)==1 && isKthBitSet(get_lltc11[1], 6)==1 && isKthBitSet(get_lltc11[1], 7)==1 &&isKthBitSet(get_lltc11[1], 8)==1){
  
   temp =  ((isKthBitSet(get_lltc11[1], 2)) * 32) + (isKthBitSet(get_lltc11[1], 1)) * 16 + (isKthBitSet(get_lltc11[0], 8)) * 8 + (isKthBitSet(get_lltc11[0], 7)) * 4 + (isKthBitSet(get_lltc11[0], 6)) * 2 + (isKthBitSet(get_lltc11[0], 5)) * 1 + (isKthBitSet(get_lltc11[0], 4)) * 0.5 + (isKthBitSet(get_lltc11[0], 3)) * 0.25 + (isKthBitSet(get_lltc11[0], 2)) * 0.125 + (isKthBitSet(get_lltc11[0], 1)) * 0.0625 - (isKthBitSet(get_lltc11[1], 3)) * 64;
}

else{
   temp = ((isKthBitSet(get_lltc11[1], 3)) * 64) + (isKthBitSet(get_lltc11[1], 2)) * 32 + (isKthBitSet(get_lltc11[1], 1)) * 16 + (isKthBitSet(get_lltc11[0], 8)) * 8 + (isKthBitSet(get_lltc11[0], 7)) * 4 + (isKthBitSet(get_lltc11[0], 6)) * 2 + (isKthBitSet(get_lltc11[0], 5)) * 1 + (isKthBitSet(get_lltc11[0], 4)) * 0.5 + (isKthBitSet(get_lltc11[0], 3)) * 0.25 + (isKthBitSet(get_lltc11[0], 2)) * 0.125 + (isKthBitSet(get_lltc11[0], 1)) * 0.0625;
}

 if(crc1 == get_lltc11[8]){

    sensors_state1.lltc11 = temp;
    sensors_state2.lltc11 = temp;
    sensors_state3.lltc11 = temp;
    sensors_state4.lltc11 = temp;
 //   NRF_LOG_INFO("TEMP2: %d", temp);
    bad_lltc11 = 0;
    flag_lltc11 = 1;
    x1_l1 = get_lltc11[2];
    x2_l1 = get_lltc11[3];
    x3_l1 = get_lltc11[4];
    x4_l1 = get_lltc11[5];
    x5_l1 = get_lltc11[6];
    x6_l1 = get_lltc11[7];
   // NRF_LOG_INFO("X1 lltc %d", x1);
}

else{

//NRF_LOG_INFO("wrong temp crc");

  bad_lltc11++;
  if(bad_lltc11>500){
    sensors_state1.lltc11 = -32.125;
    sensors_state2.lltc11 = -32.125;
    sensors_state3.lltc11 = -32.125;
    sensors_state4.lltc11 = -32.125;
    bad_lltc11 = 0;
    
 }
// NRF_LOG_INFO("WRONG TEMP2: %d", bad_lltc11);
 return NAN;

}
}

void Read_Temperature_sltc21(int x) {
  char get_sltc21[9];
  char temp_lsb, temp_msb, test;
  int k;
  char temp_f, temp_c;
  uint8_t crc1;
  float temp;


  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); //Skip ROM
  write(0x44, x); // Start Conversion
  nrf_delay_us(104);
  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); // Skip ROM
  write(0xBE, x); // Read Scratch Pad
  for (k = 0; k < 9; k++) {
    get_sltc21[k] = read_byte(x);
  }
     
     if(flag_sltc21 ==1){
     get_sltc21[2] = x1_s2;
     get_sltc21[3] = x2_s2;
     get_sltc21[4] = x3_s2;
     get_sltc21[5] = x4_s2;
     get_sltc21[6] = x5_s2;
     get_sltc21[7] = x6_s2; 
       // NRF_LOG_INFO("X lltc yyhoyyyyi %d", x1);
     }

   crc1 = dsCRC8(get_sltc21, 8);

  temp_msb = get_sltc21[1];
  temp_lsb = get_sltc21[0];

 

if(isKthBitSet(get_sltc21[1], 4)==1 && isKthBitSet(get_sltc21[1], 5)==1 && isKthBitSet(get_sltc21[1], 6)==1 && isKthBitSet(get_sltc21[1], 7)==1 &&isKthBitSet(get_sltc21[1], 8)==1){
  
   temp =  ((isKthBitSet(get_sltc21[1], 2)) * 32) + (isKthBitSet(get_sltc21[1], 1)) * 16 + (isKthBitSet(get_sltc21[0], 8)) * 8 + (isKthBitSet(get_sltc21[0], 7)) * 4 + (isKthBitSet(get_sltc21[0], 6)) * 2 + (isKthBitSet(get_sltc21[0], 5)) * 1 + (isKthBitSet(get_sltc21[0], 4)) * 0.5 + (isKthBitSet(get_sltc21[0], 3)) * 0.25 + (isKthBitSet(get_sltc21[0], 2)) * 0.125 + (isKthBitSet(get_sltc21[0], 1)) * 0.0625 - (isKthBitSet(get_sltc21[1], 3)) * 64;
}

else{
   temp = ((isKthBitSet(get_sltc21[1], 3)) * 64) + (isKthBitSet(get_sltc21[1], 2)) * 32 + (isKthBitSet(get_sltc21[1], 1)) * 16 + (isKthBitSet(get_sltc21[0], 8)) * 8 + (isKthBitSet(get_sltc21[0], 7)) * 4 + (isKthBitSet(get_sltc21[0], 6)) * 2 + (isKthBitSet(get_sltc21[0], 5)) * 1 + (isKthBitSet(get_sltc21[0], 4)) * 0.5 + (isKthBitSet(get_sltc21[0], 3)) * 0.25 + (isKthBitSet(get_sltc21[0], 2)) * 0.125 + (isKthBitSet(get_sltc21[0], 1)) * 0.0625;
}

 if(crc1 == get_sltc21[8]){

   // sensors_state1.sltc21 = temp;
    sensors_state2.sltc21 = temp;
    sensors_state3.sltc21 = temp;
    sensors_state4.sltc21 = temp;
  //  NRF_LOG_INFO("TEMP3: %d", temp);
    bad_sltc21 = 0;
    flag_sltc21 = 1;
    x1_s2 = get_sltc21[2];
    x2_s2 = get_sltc21[3];
    x3_s2 = get_sltc21[4];
    x4_s2 = get_sltc21[5];
    x5_s2 = get_sltc21[6];
    x6_s2 = get_sltc21[7];
   // NRF_LOG_INFO("X1 lltc %d", x1);
}

else{

//NRF_LOG_INFO("wrong temp crc");

  bad_sltc21++;
  if(bad_sltc21>500){
  //  sensors_state1.sltc21 = -32.125;
    sensors_state2.sltc21 = -32.125;
    sensors_state3.sltc21 = -32.125;
    sensors_state4.sltc21 = -32.125;
    bad_sltc21 = 0;
    
 }
// NRF_LOG_INFO("WRONG TEMP3: %d", bad_sltc21);
 return NAN;

}
}

void Read_Temperature_lltc21(int x) {
  char get_lltc21[9];
  char temp_lsb, temp_msb, test;
  int k;
  char temp_f, temp_c;
  uint8_t crc1;
  float temp;


  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); //Skip ROM
  write(0x44, x); // Start Conversion
  nrf_delay_us(104);
  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); // Skip ROM
  write(0xBE, x); // Read Scratch Pad
  for (k = 0; k < 9; k++) {
    get_lltc21[k] = read_byte(x);
  }
     
     if(flag_lltc21 ==1){
     get_lltc21[2] = x1_l2;
     get_lltc21[3] = x2_l2;
     get_lltc21[4] = x3_l2;
     get_lltc21[5] = x4_l2;
     get_lltc21[6] = x5_l2;
     get_lltc21[7] = x6_l2; 
       // NRF_LOG_INFO("X lltc yyhoyyyyi %d", x1);
     }

   crc1 = dsCRC8(get_lltc21, 8);

  temp_msb = get_lltc21[1];
  temp_lsb = get_lltc21[0];

 

if(isKthBitSet(get_lltc21[1], 4)==1 && isKthBitSet(get_lltc21[1], 5)==1 && isKthBitSet(get_lltc21[1], 6)==1 && isKthBitSet(get_lltc21[1], 7)==1 &&isKthBitSet(get_lltc21[1], 8)==1){
  
   temp =  ((isKthBitSet(get_lltc21[1], 2)) * 32) + (isKthBitSet(get_lltc21[1], 1)) * 16 + (isKthBitSet(get_lltc21[0], 8)) * 8 + (isKthBitSet(get_lltc21[0], 7)) * 4 + (isKthBitSet(get_lltc21[0], 6)) * 2 + (isKthBitSet(get_lltc21[0], 5)) * 1 + (isKthBitSet(get_lltc21[0], 4)) * 0.5 + (isKthBitSet(get_lltc21[0], 3)) * 0.25 + (isKthBitSet(get_lltc21[0], 2)) * 0.125 + (isKthBitSet(get_lltc21[0], 1)) * 0.0625 - (isKthBitSet(get_lltc21[1], 3)) * 64;
}

else{
   temp = ((isKthBitSet(get_lltc21[1], 3)) * 64) + (isKthBitSet(get_lltc21[1], 2)) * 32 + (isKthBitSet(get_lltc21[1], 1)) * 16 + (isKthBitSet(get_lltc21[0], 8)) * 8 + (isKthBitSet(get_lltc21[0], 7)) * 4 + (isKthBitSet(get_lltc21[0], 6)) * 2 + (isKthBitSet(get_lltc21[0], 5)) * 1 + (isKthBitSet(get_lltc21[0], 4)) * 0.5 + (isKthBitSet(get_lltc21[0], 3)) * 0.25 + (isKthBitSet(get_lltc21[0], 2)) * 0.125 + (isKthBitSet(get_lltc21[0], 1)) * 0.0625;
}

 if(crc1 == get_lltc21[8]){

   // sensors_state1.lltc21 = temp;
    sensors_state2.lltc21 = temp;
    sensors_state3.lltc21 = temp;
    sensors_state4.lltc21 = temp;
   // NRF_LOG_INFO("TEMP4: %d", temp);
    bad_lltc21 = 0;
    flag_lltc21 = 1;
    x1_l2 = get_lltc21[2];
    x2_l2 = get_lltc21[3];
    x3_l2 = get_lltc21[4];
    x4_l2 = get_lltc21[5];
    x5_l2 = get_lltc21[6];
    x6_l2 = get_lltc21[7];
   // NRF_LOG_INFO("X1 lltc %d", x1);
}

else{

//NRF_LOG_INFO("wrong temp crc");

  bad_lltc21++;
  if(bad_lltc21>500){
  //  sensors_state1.lltc21 = -32.125;
    sensors_state2.lltc21 = -32.125;
    sensors_state3.lltc21 = -32.125;
    sensors_state4.lltc21 = -32.125;
    bad_lltc21 = 0;
    
 }
// NRF_LOG_INFO("WRONG TEMP4: %d", bad_lltc21);
 return NAN;

}
}

void Read_Temperature_sltc31(int x) {
  char get_sltc31[9];
  char temp_lsb, temp_msb, test;
  int k;
  char temp_f, temp_c;
  uint8_t crc1;
  float temp;


  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); //Skip ROM
  write(0x44, x); // Start Conversion
  nrf_delay_us(104);
  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); // Skip ROM
  write(0xBE, x); // Read Scratch Pad
  for (k = 0; k < 9; k++) {
    get_sltc31[k] = read_byte(x);
  }
     
     if(flag_sltc31 ==1){
     get_sltc31[2] = x1_s3;
     get_sltc31[3] = x2_s3;
     get_sltc31[4] = x3_s3;
     get_sltc31[5] = x4_s3;
     get_sltc31[6] = x5_s3;
     get_sltc31[7] = x6_s3; 
       // NRF_LOG_INFO("X lltc yyhoyyyyi %d", x1);
     }

   crc1 = dsCRC8(get_sltc31, 8);

  temp_msb = get_sltc31[1];
  temp_lsb = get_sltc31[0];


if(isKthBitSet(get_sltc31[1], 4)==1 && isKthBitSet(get_sltc31[1], 5)==1 && isKthBitSet(get_sltc31[1], 6)==1 && isKthBitSet(get_sltc31[1], 7)==1 &&isKthBitSet(get_sltc31[1], 8)==1){
  
   temp =  ((isKthBitSet(get_sltc31[1], 2)) * 32) + (isKthBitSet(get_sltc31[1], 1)) * 16 + (isKthBitSet(get_sltc31[0], 8)) * 8 + (isKthBitSet(get_sltc31[0], 7)) * 4 + (isKthBitSet(get_sltc31[0], 6)) * 2 + (isKthBitSet(get_sltc31[0], 5)) * 1 + (isKthBitSet(get_sltc31[0], 4)) * 0.5 + (isKthBitSet(get_sltc31[0], 3)) * 0.25 + (isKthBitSet(get_sltc31[0], 2)) * 0.125 + (isKthBitSet(get_sltc31[0], 1)) * 0.0625 - (isKthBitSet(get_sltc31[1], 3)) * 64;
}

else{
   temp = ((isKthBitSet(get_sltc31[1], 3)) * 64) + (isKthBitSet(get_sltc31[1], 2)) * 32 + (isKthBitSet(get_sltc31[1], 1)) * 16 + (isKthBitSet(get_sltc31[0], 8)) * 8 + (isKthBitSet(get_sltc31[0], 7)) * 4 + (isKthBitSet(get_sltc31[0], 6)) * 2 + (isKthBitSet(get_sltc31[0], 5)) * 1 + (isKthBitSet(get_sltc31[0], 4)) * 0.5 + (isKthBitSet(get_sltc31[0], 3)) * 0.25 + (isKthBitSet(get_sltc31[0], 2)) * 0.125 + (isKthBitSet(get_sltc31[0], 1)) * 0.0625;
}


 if(crc1 == get_sltc31[8]){

   // sensors_state1.sltc31 = temp;
   // sensors_state2.sltc31 = temp;
    sensors_state3.sltc31 = temp;
    sensors_state4.sltc31 = temp;
  //  NRF_LOG_INFO("TEMP5: %d", temp);
    bad_sltc31 = 0;
    flag_sltc31 = 1;
    x1_s3 = get_sltc31[2];
    x2_s3 = get_sltc31[3];
    x3_s3 = get_sltc31[4];
    x4_s3 = get_sltc31[5];
    x5_s3 = get_sltc31[6];
    x6_s3 = get_sltc31[7];
   // NRF_LOG_INFO("X1 lltc %d", x1);
}

else{

//NRF_LOG_INFO("wrong temp crc");

  bad_sltc31++;
  if(bad_sltc31>500){
  //  sensors_state1.sltc31 = -32.125;
  //  sensors_state2.sltc31 = -32.125;
    sensors_state3.sltc31 = -32.125;
    sensors_state4.sltc31 = -32.125;
    bad_sltc31 = 0;
    
 }
// NRF_LOG_INFO("WRONG TEMP5: %d", bad_sltc31);
 return NAN;

}
}

void Read_Temperature_lltc31(int x) {
  char get_lltc31[9];
  char temp_lsb, temp_msb, test;
  int k;
  char temp_f, temp_c;
  uint8_t crc1;
  float temp;


  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); //Skip ROM
  write(0x44, x); // Start Conversion
  nrf_delay_us(104);
  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); // Skip ROM
  write(0xBE, x); // Read Scratch Pad
  for (k = 0; k < 9; k++) {
    get_lltc31[k] = read_byte(x);
  }
     
     if(flag_lltc31 ==1){
     get_lltc31[2] = x1_l3;
     get_lltc31[3] = x2_l3;
     get_lltc31[4] = x3_l3;
     get_lltc31[5] = x4_l3;
     get_lltc31[6] = x5_l3;
     get_lltc31[7] = x6_l3; 
       // NRF_LOG_INFO("X lltc yyhoyyyyi %d", x1);
     }

   crc1 = dsCRC8(get_lltc31, 8);

  temp_msb = get_lltc31[1];
  temp_lsb = get_lltc31[0];

if(isKthBitSet(get_lltc31[1], 4)==1 && isKthBitSet(get_lltc31[1], 5)==1 && isKthBitSet(get_lltc31[1], 6)==1 && isKthBitSet(get_lltc31[1], 7)==1 &&isKthBitSet(get_lltc31[1], 8)==1){
  
   temp =  ((isKthBitSet(get_lltc31[1], 2)) * 32) + (isKthBitSet(get_lltc31[1], 1)) * 16 + (isKthBitSet(get_lltc31[0], 8)) * 8 + (isKthBitSet(get_lltc31[0], 7)) * 4 + (isKthBitSet(get_lltc31[0], 6)) * 2 + (isKthBitSet(get_lltc31[0], 5)) * 1 + (isKthBitSet(get_lltc31[0], 4)) * 0.5 + (isKthBitSet(get_lltc31[0], 3)) * 0.25 + (isKthBitSet(get_lltc31[0], 2)) * 0.125 + (isKthBitSet(get_lltc31[0], 1)) * 0.0625 - (isKthBitSet(get_lltc31[1], 3)) * 64;
}

else{
   temp = ((isKthBitSet(get_lltc31[1], 3)) * 64) + (isKthBitSet(get_lltc31[1], 2)) * 32 + (isKthBitSet(get_lltc31[1], 1)) * 16 + (isKthBitSet(get_lltc31[0], 8)) * 8 + (isKthBitSet(get_lltc31[0], 7)) * 4 + (isKthBitSet(get_lltc31[0], 6)) * 2 + (isKthBitSet(get_lltc31[0], 5)) * 1 + (isKthBitSet(get_lltc31[0], 4)) * 0.5 + (isKthBitSet(get_lltc31[0], 3)) * 0.25 + (isKthBitSet(get_lltc31[0], 2)) * 0.125 + (isKthBitSet(get_lltc31[0], 1)) * 0.0625;
}

 if(crc1 == get_lltc31[8]){

   // sensors_state1.lltc31 = temp;
   // sensors_state2.lltc31 = temp;
    sensors_state3.lltc31 = temp;
    sensors_state4.lltc31 = temp;
  //  NRF_LOG_INFO("TEMP6: %d", temp);
    bad_lltc31 = 0;
    flag_lltc31 = 1;
    x1_l3 = get_lltc31[2];
    x2_l3 = get_lltc31[3];
    x3_l3 = get_lltc31[4];
    x4_l3 = get_lltc31[5];
    x5_l3 = get_lltc31[6];
    x6_l3 = get_lltc31[7];
   // NRF_LOG_INFO("X1 lltc %d", x1);
}

else{

//NRF_LOG_INFO("wrong temp crc");

  bad_lltc31++;
  if(bad_lltc31>500){
  //  sensors_state1.lltc31 = -32.125;
  //  sensors_state2.lltc31 = -32.125;
    sensors_state3.lltc31 = -32.125;
    sensors_state4.lltc31 = -32.125;
    bad_lltc31 = 0;
    
 }
// NRF_LOG_INFO("WRONG TEMP6: %d", bad_lltc31);
 return NAN;

}
}

void Read_Temperature_sltc41(int x) {
  char get_sltc41[9];
  char temp_lsb, temp_msb, test;
  int k;
  char temp_f, temp_c;
  uint8_t crc1;
  float temp;

  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); //Skip ROM
  write(0x44, x); // Start Conversion
  nrf_delay_us(104);
  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); // Skip ROM
  write(0xBE, x); // Read Scratch Pad
  for (k = 0; k < 9; k++) {
    get_sltc41[k] = read_byte(x);
  }
     
     if(flag_sltc41 ==1){
     get_sltc41[2] = x1_s4;
     get_sltc41[3] = x2_s4;
     get_sltc41[4] = x3_s4;
     get_sltc41[5] = x4_s4;
     get_sltc41[6] = x5_s4;
     get_sltc41[7] = x6_s4; 
       // NRF_LOG_INFO("X lltc yyhoyyyyi %d", x1);
     }

   crc1 = dsCRC8(get_sltc41, 8);

  temp_msb = get_sltc41[1];
  temp_lsb = get_sltc41[0];


if(isKthBitSet(get_sltc41[1], 4)==1 && isKthBitSet(get_sltc41[1], 5)==1 && isKthBitSet(get_sltc41[1], 6)==1 && isKthBitSet(get_sltc41[1], 7)==1 &&isKthBitSet(get_sltc41[1], 8)==1){
  
   temp =  ((isKthBitSet(get_sltc41[1], 2)) * 32) + (isKthBitSet(get_sltc41[1], 1)) * 16 + (isKthBitSet(get_sltc41[0], 8)) * 8 + (isKthBitSet(get_sltc41[0], 7)) * 4 + (isKthBitSet(get_sltc41[0], 6)) * 2 + (isKthBitSet(get_sltc41[0], 5)) * 1 + (isKthBitSet(get_sltc41[0], 4)) * 0.5 + (isKthBitSet(get_sltc41[0], 3)) * 0.25 + (isKthBitSet(get_sltc41[0], 2)) * 0.125 + (isKthBitSet(get_sltc41[0], 1)) * 0.0625 - (isKthBitSet(get_sltc41[1], 3)) * 64;
}

else{
   temp = ((isKthBitSet(get_sltc41[1], 3)) * 64) + (isKthBitSet(get_sltc41[1], 2)) * 32 + (isKthBitSet(get_sltc41[1], 1)) * 16 + (isKthBitSet(get_sltc41[0], 8)) * 8 + (isKthBitSet(get_sltc41[0], 7)) * 4 + (isKthBitSet(get_sltc41[0], 6)) * 2 + (isKthBitSet(get_sltc41[0], 5)) * 1 + (isKthBitSet(get_sltc41[0], 4)) * 0.5 + (isKthBitSet(get_sltc41[0], 3)) * 0.25 + (isKthBitSet(get_sltc41[0], 2)) * 0.125 + (isKthBitSet(get_sltc41[0], 1)) * 0.0625;
}


 if(crc1 == get_sltc41[8]){
   // sensors_state1.sltc41 = temp;
   // sensors_state2.sltc41 = temp;
  //  sensors_state3.sltc41 = temp;
    sensors_state4.sltc41 = temp;
  //  NRF_LOG_INFO("TEMP7: %d", temp);
    bad_sltc41 = 0;
    flag_sltc41 = 1;
    x1_s4 = get_sltc41[2];
    x2_s4 = get_sltc41[3];
    x3_s4 = get_sltc41[4];
    x4_s4 = get_sltc41[5];
    x5_s4 = get_sltc41[6];
    x6_s4 = get_sltc41[7];
   // NRF_LOG_INFO("X1 lltc %d", x1);
}

else{

//NRF_LOG_INFO("wrong temp crc");

  bad_sltc41++;
  if(bad_sltc41>500){
  //  sensors_state1.sltc41 = -32.125;
  //  sensors_state2.sltc41 = -32.125;
  //  sensors_state3.sltc41 = -32.125;
    sensors_state4.sltc41 = -32.125;
    bad_sltc41 = 0;
    
 }
// NRF_LOG_INFO("WRONG TEMP7: %d", bad_sltc41);
 return NAN;

}
}

void Read_Temperature_lltc41(int x) {
  char get_lltc41[9];
  char temp_lsb, temp_msb, test;
  int k;
  char temp_f, temp_c;
  uint8_t crc1;
  float temp;


  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); //Skip ROM
  write(0x44, x); // Start Conversion
  nrf_delay_us(104);
  init(x);
  //nrf_delay_us(100);
  write(0xCC, x); // Skip ROM
  write(0xBE, x); // Read Scratch Pad
  for (k = 0; k < 9; k++) {
    get_lltc41[k] = read_byte(x);
  }
     
     if(flag_lltc41 ==1){
     get_lltc41[2] = x1_l4;
     get_lltc41[3] = x2_l4;
     get_lltc41[4] = x3_l4;
     get_lltc41[5] = x4_l4;
     get_lltc41[6] = x5_l4;
     get_lltc41[7] = x6_l4; 
       // NRF_LOG_INFO("X lltc yyhoyyyyi %d", x1);
     }

   crc1 = dsCRC8(get_lltc41, 8);

  temp_msb = get_lltc41[1];
  temp_lsb = get_lltc41[0];

if(isKthBitSet(get_lltc41[1], 4)==1 && isKthBitSet(get_lltc41[1], 5)==1 && isKthBitSet(get_lltc41[1], 6)==1 && isKthBitSet(get_lltc41[1], 7)==1 &&isKthBitSet(get_lltc41[1], 8)==1){
  
   temp =  ((isKthBitSet(get_lltc41[1], 2)) * 32) + (isKthBitSet(get_lltc41[1], 1)) * 16 + (isKthBitSet(get_lltc41[0], 8)) * 8 + (isKthBitSet(get_lltc41[0], 7)) * 4 + (isKthBitSet(get_lltc41[0], 6)) * 2 + (isKthBitSet(get_lltc41[0], 5)) * 1 + (isKthBitSet(get_lltc41[0], 4)) * 0.5 + (isKthBitSet(get_lltc41[0], 3)) * 0.25 + (isKthBitSet(get_lltc41[0], 2)) * 0.125 + (isKthBitSet(get_lltc41[0], 1)) * 0.0625 - (isKthBitSet(get_lltc41[1], 3)) * 64;
}

else{
   temp = ((isKthBitSet(get_lltc41[1], 3)) * 64) + (isKthBitSet(get_lltc41[1], 2)) * 32 + (isKthBitSet(get_lltc41[1], 1)) * 16 + (isKthBitSet(get_lltc41[0], 8)) * 8 + (isKthBitSet(get_lltc41[0], 7)) * 4 + (isKthBitSet(get_lltc41[0], 6)) * 2 + (isKthBitSet(get_lltc41[0], 5)) * 1 + (isKthBitSet(get_lltc41[0], 4)) * 0.5 + (isKthBitSet(get_lltc41[0], 3)) * 0.25 + (isKthBitSet(get_lltc41[0], 2)) * 0.125 + (isKthBitSet(get_lltc41[0], 1)) * 0.0625;
}


 if(crc1 == get_lltc41[8]){

   // sensors_state1.lltc41 = temp;
   // sensors_state2.lltc41 = temp;
  //  sensors_state3.lltc41 = temp;
    sensors_state4.lltc41 = temp;
  //  NRF_LOG_INFO("TEMP8: %d", temp);
    bad_lltc41 = 0;
    flag_lltc41 = 1;
    x1_l4 = get_lltc41[2];
    x2_l4 = get_lltc41[3];
    x3_l4 = get_lltc41[4];
    x4_l4 = get_lltc41[5];
    x5_l4 = get_lltc41[6];
    x6_l4 = get_lltc41[7];
   // NRF_LOG_INFO("X1 lltc %d", x1);
}

else{

//NRF_LOG_INFO("wrong temp crc");

  bad_lltc41++;
  if(bad_lltc41>500){
  //  sensors_state1.lltc41 = -32.125;
  //  sensors_state2.lltc41 = -32.125;
  //  sensors_state3.lltc41 = -32.125;
    sensors_state4.lltc41 = -32.125;
    bad_lltc41 = 0;
    
 }
// NRF_LOG_INFO("WRONG TEMP8: %d", bad_lltc41);
 return NAN;

}
}