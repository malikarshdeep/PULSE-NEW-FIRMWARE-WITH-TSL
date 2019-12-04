/**
 * Copyright (c) 2014 - 2018, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
/** @file
 *
 * @defgroup ble_sdk_uart_over_ble_main main.c
 * @{
 * @ingroup  ble_sdk_app_nus_eval
 * @brief    UART over BLE application main file.
 *
 * This file contains the source code for a sample application that uses the Nordic UART service.
 * This application uses the @ref srvlib_conn_params module.
 */

#include "app_error.h"
#include "app_timer.h"
#include "app_uart.h"
#include "app_util_platform.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_hci.h"
#include "ble_nus.h"
#include "boards.h"
#include "bsp_btn_ble.h"
#include "math.h"
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_delay.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "nrfx_spim.h"
#include <stdint.h>
#include <string.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"
#include "network.h"
#include "tcptest.h"
#include "cmd.h"
#include "nrf_fstorage_sd.h"
#include "ble.h"
#include "ble_sensors.h"
#include "temperature.h"
#include "onewire.h"
#include "nrf_tls.h"
#include "mem_manager.h"

#define SAMPLES_IN_BUFFER 8
static nrf_saadc_value_t m_buffer_pool[4][SAMPLES_IN_BUFFER];
#define BOOTLOADER_DFU_START 0xB1

//BLE variables
uint16_t bytes_to_send = 20;

static uint8_t str[20];

#if defined(UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined(UARTE_PRESENT)
#include "nrf_uarte.h"
#endif

char pulse_name[8];
char iM_pulse_ID[6];
char update[] = "false";
char true_dhcp[] = "true";

//static char token_string1[40];
/* fstorage example */

static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt);


NRF_FSTORAGE_DEF(nrf_fstorage_t fstorage) =
{
    /* Set a handler for fstorage events. */
    .evt_handler = fstorage_evt_handler,

    /* These below are the boundaries of the flash space assigned to this instance of fstorage.
     * You must set these manually, even at runtime, before nrf_fstorage_init() is called.
     * The function nrf5_flash_end_addr_get() can be used to retrieve the last address on the
     * last page of flash available to write data. */
    .start_addr = 0xed000,
    .end_addr   = 0xf3000,
};



/**@brief   Helper function to obtain the last address on the last page of the on-chip flash that
 *          can be used to write user data.
 */
static uint32_t nrf5_flash_end_addr_get()
{
    uint32_t const bootloader_addr = NRF_UICR->NRFFW[0];
    uint32_t const page_sz         = NRF_FICR->CODEPAGESIZE;
    uint32_t const code_sz         = NRF_FICR->CODESIZE;

    return (bootloader_addr != 0xFFFFFFFF ?
            bootloader_addr : (code_sz * page_sz));
}



static void fstorage_evt_handler(nrf_fstorage_evt_t * p_evt)
{
    if (p_evt->result != NRF_SUCCESS)
    {
        NRF_LOG_INFO("--> Event received: ERROR while executing an fstorage operation.");
        return;
    }

    switch (p_evt->id)
    {
        case NRF_FSTORAGE_EVT_WRITE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: wrote %d bytes at address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        case NRF_FSTORAGE_EVT_ERASE_RESULT:
        {
            NRF_LOG_INFO("--> Event received: erased %d page from address 0x%x.",
                         p_evt->len, p_evt->addr);
        } break;

        default:
            break;
    }
}



void wait_for_flash_ready(nrf_fstorage_t const * p_fstorage)
{
    /* While fstorage is busy, sleep and wait for an event. */
    while (nrf_fstorage_is_busy(p_fstorage))
    {
          vTaskDelay(1);
    }
}


/**@brief Function for initializing the timer module.
 */


static void timers_init(void) {
  ret_code_t err_code = app_timer_init();
  APP_ERROR_CHECK(err_code);
}


/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
static void sleep_mode_enter(void) {
  uint32_t err_code;// = bsp_indication_set(BSP_INDICATE_IDLE);
  APP_ERROR_CHECK(err_code);

  // Prepare wakeup buttons.
  err_code = bsp_btn_ble_sleep_mode_prepare();
  APP_ERROR_CHECK(err_code);

  // Go to system-off mode (this function will not return; wakeup will cause a reset).
  err_code = sd_power_system_off();
  APP_ERROR_CHECK(err_code);
}



/**@brief Function for initializing buttons and leds.
 *
 * @param[out] p_erase_bonds  Will be true if the clear bonding button was pressed to wake the application up.
 */
static void buttons_leds_init(bool *p_erase_bonds) {
  bsp_event_t startup_event;

  uint32_t err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_event_handler);
  APP_ERROR_CHECK(err_code);

  //err_code = bsp_btn_ble_init(NULL, &startup_event);
  APP_ERROR_CHECK(err_code);

  *p_erase_bonds = (startup_event == BSP_EVENT_CLEAR_BONDING_DATA);
}

/**@brief Function for initializing the nrf log module.
 */
static void log_init(void) {
  ret_code_t err_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(err_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**@brief Function for initializing power management.
 */
static void power_management_init(void) {
  ret_code_t err_code;
  err_code = nrf_pwr_mgmt_init();
  APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void) {
  UNUSED_RETURN_VALUE(NRF_LOG_PROCESS());
  nrf_pwr_mgmt_run();
}


static TaskHandle_t network_task_handle;
static TaskHandle_t sensors_task_handle;
static TaskHandle_t temp_task_handle;
static TaskHandle_t wifi_task_handle;
static TaskHandle_t storage_task_handle;









void sensors_task_fn(void *arg) {
 
ret_code_t err_code;
uint8_t buff[10];
uint8_t get[9];
volatile float temp;
while (1) {

      vTaskDelay(10);
      #ifdef BOARD_PCA10056
      temp = Read_Temperature(2);
      if (isnan(temp))
      {
        NRF_LOG_INFO("temp2 ERR");    
      }    
      #endif
      #ifdef BOARD_CUSTOM

      err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_IN_BUFFER); //analog channel sampling
      APP_ERROR_CHECK(err_code);
      nrf_drv_saadc_sample();
  
    //  vTaskDelay(1);
    //  DHT_task_callback(36);
    //  vTaskDelay(1);
     // DHT_task_callback(35);
    //  vTaskDelay(1);
    //  DHT_task_callback(34);
      vTaskSuspendAll();
     //  vTaskDelay(10);
      Read_Temperature_sltc11(33);    //slt1
      // xTaskResumeAll();
    //  vTaskSuspendAll();
    //  vTaskDelay(10);
      Read_Temperature_lltc11(11);    //llt1
//   //    xTaskResumeAll();
//   //    vTaskSuspendAll();
//    //  vTaskDelay(10);
      Read_Temperature_sltc21(40);    //slt2
//   //    xTaskResumeAll();
//   //   vTaskSuspendAll();
//      //vTaskDelay(10);
      Read_Temperature_lltc21(8);     //llt2
//   //    xTaskResumeAll();
//   //   vTaskSuspendAll();
//  //    vTaskDelay(10);
      Read_Temperature_sltc31(7);     //slt3
//   //    xTaskResumeAll();
//   //   vTaskSuspendAll();
//  //    vTaskDelay(10);
      Read_Temperature_lltc31(6);     //llt3
// //      xTaskResumeAll();
// //     vTaskSuspendAll();
//   //   vTaskDelay(10);
      Read_Temperature_sltc41(27);    //slt4
// //      xTaskResumeAll();
// //     vTaskSuspendAll();
//   //   vTaskDelay(10);
      Read_Temperature_lltc41(44);    //llt4
      xTaskResumeAll();

      vTaskDelay(1);
      thermostat_signals();   //t_stat readings

      vTaskDelay(1);
      energy_measurement();   //power meter readings
      vTaskDelay(1);

  // vTaskSuspendAll();
      nrf_gpio_cfg_output(36);  //reset the i2c expander board
      nrf_gpio_pin_write(36,0);
      vTaskDelay(100);
     //  nrf_delay_ms(10);
      nrf_gpio_pin_write(36,1);
      vTaskDelay(100);
   //  nrf_delay_ms(10);
      htu21d_hw_uninit();   //uninitialize the i2c instance
      htu21d_hw_init();    //initialize the i2c instance 
      tcaselect(2);       //select port 2 of i2c exapnder 
     // vTaskDelay(50);
    //tcaselect(2);
    vTaskDelay(50);
 //  nrf_delay_ms(50);
      soft_reset_htu21(); //reset the htu21d sensor
      vTaskDelay(50);
   // nrf_delay_ms(50);
      tempRead_command();  //send command to read temp
      vTaskDelay(50);
   // nrf_delay_ms(50);
    //tca_read(); 
      read_sensor_data_temp(2);  //oadb
      vTaskDelay(100);
    // nrf_delay_ms(10);
      humRead_command();        //send command to read humidity 
      vTaskDelay(50);
   //  nrf_delay_ms(50);
      read_sensor_data_HUM(2);  //oarh
    //soft_reset_htu21();
    
 

      nrf_gpio_cfg_output(36);
      nrf_gpio_pin_write(36,0);
      vTaskDelay(100);
   //  nrf_delay_ms(10);
      nrf_gpio_pin_write(36,1);
      vTaskDelay(100);
    //nrf_delay_ms(10);
      htu21d_hw_uninit();
      htu21d_hw_init();
      tcaselect(4);
      vTaskDelay(50);
   // nrf_delay_ms(50);
    //tcaselect(4);
   // vTaskDelay(50);
      soft_reset_htu21();
      vTaskDelay(50);
   // nrf_delay_ms(50);
      tempRead_command();
      vTaskDelay(50);
    //tca_read(); 
  //  nrf_delay_ms(50);
      read_sensor_data_temp(4); //radb
      vTaskDelay(100);
 // nrf_delay_ms(10);
      humRead_command();
      vTaskDelay(50);
  //nrf_delay_ms(50);
      read_sensor_data_HUM(4);  //rarh
    //soft_reset_htu21();

      nrf_gpio_cfg_output(36);
      nrf_gpio_pin_write(36,0);
      vTaskDelay(100);
    // nrf_delay_ms(10);
      nrf_gpio_pin_write(36,1);
      vTaskDelay(100);
    // nrf_delay_ms(10);
      htu21d_hw_uninit();
      htu21d_hw_init();
      tcaselect(6);
      vTaskDelay(50);
    //tcaselect(6);
    //vTaskDelay(50);
  //  nrf_delay_ms(50);
      soft_reset_htu21();
      vTaskDelay(50);
   // nrf_delay_ms(50);
      tempRead_command();
      vTaskDelay(50);
   // nrf_delay_ms(50);
   // tca_read(); 
      read_sensor_data_temp(6);  //sadb
      vTaskDelay(100);
  //  nrf_delay_ms(10);
      humRead_command();
      vTaskDelay(50);
  //  nrf_delay_ms(50);
      read_sensor_data_HUM(6);  //sarh
   // soft_reset_htu21();
   // xTaskResumeAll();

      #endif
      NRF_LOG_INFO("ble sensors update %x", ble_sensors_update());  //send bluetooth values 
     // cmd_sensors_timer();  //for sending a command exclusively to get data through ble
  }
}




void storage_task_fn(void *arg) {

uint16_t no_internet = 0;
uint32_t iM_pulse_ID_addr = 0x10001080;          //PULSE ID stored at this location
uint8_t iM_pulse_ID_length = 6;
memcpy(iM_pulse_ID, (uint8_t *)iM_pulse_ID_addr, iM_pulse_ID_length);

  //config_string[0] = 'z';
volatile ret_code_t rc;
nrf_fstorage_api_t * p_fs_api;
p_fs_api = &nrf_fstorage_sd;

rc = nrf_fstorage_init(&fstorage, p_fs_api, NULL);  //Flash storage initialziation
APP_ERROR_CHECK(rc);

//if (rc == NRF_SUCCESS){
//  // NRF_LOG_INFO("fstorage init successful");  
//    }
//
//  else{
// //  NRF_LOG_INFO("fstorage init failed");
//   }
   
#define CONFIG_ADDR  0xed000  //Config quipment stored at 81000h (config[voltage......])
rc = nrf_fstorage_read(
    &fstorage,               /* The instance to use. */
    CONFIG_ADDR,            /* The address in flash where to read data from. */
    config_string1,        /* A buffer to copy the data into. */
    48                    /* Lenght of the data, in bytes. */
 );

#define FLASH_ADDR_TOKEN  0xee200   //API token stored at 82000h
rc = nrf_fstorage_read(
    &fstorage,              
    FLASH_ADDR_TOKEN,     
    API_token1,        
    240  
 );

#define FLASH_ADDR_LINK  0xee100    //Read API link stored at 83000h
rc = nrf_fstorage_read(
    &fstorage,   
    FLASH_ADDR_LINK,     
    API_link1,        
    240 
 );

#define FLASH_ADDR_IP  0xef000    //Read API link stored at 83000h
rc = nrf_fstorage_read(
    &fstorage,   
    FLASH_ADDR_IP,     
    &ipaddr_value[0],
    4 
 );

#define FLASH_ADDR_GW  0xef100    //Read API link stored at 83000h
rc = nrf_fstorage_read(
    &fstorage,   
    FLASH_ADDR_GW,     
    &gw_value[0],
    4 
 );

#define FLASH_ADDR_MASK  0xef200    //Read API link stored at 83000h
rc = nrf_fstorage_read(
    &fstorage,   
    FLASH_ADDR_MASK,     
    &mask_value[0],
    4 
 );


#define FLASH_ADDR_CHECK_WIFI  0xf0000    //Read API link stored at 83000h
rc = nrf_fstorage_read(
    &fstorage,   
    FLASH_ADDR_CHECK_WIFI,     
    dhcp_check_wifi,        
    8 
 );

#define FLASH_ADDR_DHCP  0xf1000    //Read API link stored at 83000h
rc = nrf_fstorage_read(
    &fstorage,   
    FLASH_ADDR_DHCP,     
    dhcp_check_eth,        
    8 
 );

#define FLASH_ADDR_IS_INTERNET  0xee000   //API token stored at 82000h
rc = nrf_fstorage_read(
    &fstorage,              
    FLASH_ADDR_IS_INTERNET,     
    is_internet1,        
    8  
 );

  #define FLASH_ADDR_SSID  0xee500   
rc = nrf_fstorage_read(
    &fstorage,              
    FLASH_ADDR_SSID,     
    ssid_new,        
    80  
 );

  #define FLASH_ADDR_KEY  0xee600   
rc = nrf_fstorage_read(
    &fstorage,              
    FLASH_ADDR_KEY,     
    key_new,        
    80  
 );

  ssid_len = strlen(ssid_new);

 if(strncmp(dhcp_check_wifi,"true",4)==0 || strncmp(dhcp_check_wifi,"false",4)==0 ){
 vTaskDelay(1);
 }
 else{
        rc = nrf_fstorage_write(  //write the token 
             &fstorage,   
             0xf0000,     
             true_dhcp,       
             240, 
             NULL           
            );
           APP_ERROR_CHECK(rc);
           wait_for_flash_ready(&fstorage);
           strcpy(dhcp_check_wifi,"true");
 }


  if(strncmp(dhcp_check_eth,"true",4)==0 || strncmp(dhcp_check_eth,"false",4)==0 ){
 vTaskDelay(1);
 }
 else{
        rc = nrf_fstorage_write(  //write the token 
             &fstorage,   
             0xf1000,     
             true_dhcp,       
             240, 
             NULL           
            );
           APP_ERROR_CHECK(rc);
           wait_for_flash_ready(&fstorage);
           strcpy(dhcp_check_eth,"true");
 }

while(1){



//    if(test_internet ==1 && check_internet ==1){
//    
//    
//                 int n;
//                 n = sprintf(str, "success internet\n");
//                ble_serial_send(str, n);
//                test_internet = 0;
//                NRF_LOG_INFO("success internet");
//    
//    }

    if(test_internet ==1 ){

             vTaskDelay(10000);
             if(check_internet ==1 && led_on == 1 )
    {
    
                 int n;
                 n = sprintf(str, "success internet\n");
                ble_serial_send(str, n);
                test_internet = 0;
                NRF_LOG_INFO("success internet");

    
    }
    else{
    

    NRF_LOG_INFO("NO INTERBET %d",no_internet );
 
                     int n;
                 n = sprintf(str, "failed internet\n");
                ble_serial_send(str, n);
                test_internet = 0;
                NRF_LOG_INFO("failed internet");
    
    
    }
    }

    if(restart_system ==1 && is_ble_on ==0){
     sd_nvic_SystemReset();
    }

       if (strncmp(API_token_first,"token",5) == 0)  // if we received a new token
       {    
//            static uint32_t pages_to_erase = 1;       
//            rc = nrf_fstorage_erase(  // erase the old memory
//                &fstorage,   
//                0xb2000,     
//                pages_to_erase, 
//                NULL            
//            );  
//            APP_ERROR_CHECK(rc);
//            wait_for_flash_ready(&fstorage);
//       
//            if(rc == NRF_SUCCESS){     //if successuffully erased 
//       
                 vTaskDelay(100);

                 rc = nrf_fstorage_write(  //write the token 
                    &fstorage,   
                    0xee200,     
                    API_token,       
                    240, 
                    NULL           
                  );
   
                APP_ERROR_CHECK(rc);
                wait_for_flash_ready(&fstorage);


                strcpy(API_token1,API_token);  //API_token1 has the new value  
                API_token[0] = 'z';            //API_token is empty again to detect if token changes again
   
                int n;
                n = sprintf(str, "success token\n");
                ble_serial_send(str, n);
                  NRF_LOG_INFO("TOKEN LIKHA");
                API_token_first[0] = 'z';     //API_token_first is empty again to detect a new command to change token
           // }    
       }
         
       if (strncmp(API_link_first,"link",4) ==0)
       {   NRF_LOG_INFO("link");
      
//            static uint32_t pages_to_erase = 1;
//      
//            rc = nrf_fstorage_erase(
//                &fstorage,   
//                0xb3000,     
//                pages_to_erase, 
//                NULL           
//                );      
//           APP_ERROR_CHECK(rc);
//           wait_for_flash_ready(&fstorage);
//  
//       
//             if(rc == NRF_SUCCESS){
       
                 vTaskDelay(100);
                 rc = nrf_fstorage_write(
                      &fstorage,  
                      0xee100,     
                      API_link,       
                      240,
                      NULL           
                    );   
                APP_ERROR_CHECK(rc);
                wait_for_flash_ready(&fstorage);
                       NRF_LOG_INFO("I AM WRITING LINK:%s", API_link);
                strcpy(API_link1,API_link); 
                API_link[0] = API_link_first[0] = 'z'; 
//                link_recvd = 1;
                 
                int n;
                n = sprintf(str, "success link\n");
                ble_serial_send(str, n);
                
                NRF_LOG_INFO("LINK LIKHA");
           //  }   
       }

                     if (ssid_changed == 1)
       {   NRF_LOG_INFO("ssid changed");
      
                 vTaskDelay(100);
                 rc = nrf_fstorage_write(
                      &fstorage,  
                      0xee500,     
                      ssid_saved_copy,       
                      80,
                      NULL           
                    );   
                APP_ERROR_CHECK(rc);
                wait_for_flash_ready(&fstorage);
                ssid_changed = 0;
               
       }

                     if (key_changed == 1)
       {   NRF_LOG_INFO("key changed");
      
                 vTaskDelay(100);
                 rc = nrf_fstorage_write(
                      &fstorage,  
                      0xee600,     
                      key_copy,       
                      80,
                      NULL           
                    );   
                APP_ERROR_CHECK(rc);
                wait_for_flash_ready(&fstorage);
                key_changed = 0;
               
       }

       if (strncmp(is_internet_first,"internet",8) == 0)  // if we received a new token
       {    
            static uint32_t pages_to_erase = 1;       
            rc = nrf_fstorage_erase(  // erase the old memory
                &fstorage,   
                0xee000,     
                pages_to_erase, 
                NULL            
            );  
            APP_ERROR_CHECK(rc);
            wait_for_flash_ready(&fstorage);
       
            if(rc == NRF_SUCCESS){     //if successuffully erased 
       
                 vTaskDelay(100);

                 rc = nrf_fstorage_write(  //write the token 
                    &fstorage,   
                    0xee000,     
                    is_internet,       
                    240, 
                    NULL           
                  );
   
                APP_ERROR_CHECK(rc);
                wait_for_flash_ready(&fstorage);


                strcpy(is_internet1,is_internet);  //API_token1 has the new value  
                is_internet[0] = 'z';            //API_token is empty again to detect if token changes again
   
                vTaskDelay(100);
                int n;
                n = sprintf(str, "success wifi_eth\n");
                ble_serial_send(str, n);
                NRF_LOG_INFO("success wifi_eth");

                is_internet_first[0] = 'z';     //API_token_first is empty again to detect a new command to change token
            }    
       }
           
        if((strcmp(config_string,"z")) != 0){   //config string changed
             NRF_LOG_INFO("config string");
            static uint32_t pages_to_erase = 1;

            rc = nrf_fstorage_erase(
                &fstorage,   
                0xed000,     
                pages_to_erase, 
                NULL            
                );            
           APP_ERROR_CHECK(rc);
           wait_for_flash_ready(&fstorage);
           if(rc == NRF_SUCCESS){    
               vTaskDelay(100);
               rc = nrf_fstorage_write(
               &fstorage,             
               0xed000,              
               config_string,      
               240,             
               NULL            
              );   
              APP_ERROR_CHECK(rc);
              if(rc == NRF_SUCCESS){
                  wait_for_flash_ready(&fstorage);
                  memcpy(config_string1,config_string,48);
                  strcpy(config_string,"z");
                  int n;
                  n = sprintf(str, "success equipment\n");
                  ble_serial_send(str, n); 
             }
          }   
       }
    //  }
    //  }    
//       if(strcmp(reset_pulse,"reset_pulse!") == 0)   //if received a command to reset pulse
//        {       
//            static uint32_t pages_to_erase = 5;       //erase 5 pages conatining link, token and config equipment info
//
//            rc = nrf_fstorage_erase(
//                  &fstorage,   
//                  0xb0000,     
//                  pages_to_erase, 
//                  NULL            
//                );             
//            APP_ERROR_CHECK(rc);
//            wait_for_flash_ready(&fstorage);
//            strcpy(reset_pulse,"x");    //reset pulse command is empty now to detect for the command again 
//            vTaskDelay(2000); 
//       }  

if(strcmp(ipaddress_changed,"changed") == 0){

    rc = nrf_fstorage_erase(
                &fstorage,   
                0xef000,     
                1, 
                NULL           
                );      
           APP_ERROR_CHECK(rc);
           //nrf_delay_ms(1);
           wait_for_flash_ready(&fstorage);
           NRF_LOG_INFO("OUTSIDE WAIT ERASE");
       
             if(rc == NRF_SUCCESS){
             NRF_LOG_INFO("INSIDE NRF SUCCESS");
      vTaskDelay(100);
    // nrf_delay_ms(1);
      rc = nrf_fstorage_write(  //write the token 
       &fstorage,   
       0xef000,     
       &ipaddr,       
       4, 
       NULL           
      );
   
    APP_ERROR_CHECK(rc);
    wait_for_flash_ready(&fstorage);
    vTaskDelay(100);
    NRF_LOG_INFO("DATA WRIITEN IN FLASH")
}


//    rc = nrf_fstorage_erase(
//                &fstorage,   
//                0xb6000,     
//                1, 
//                NULL           
//                );      
//           APP_ERROR_CHECK(rc);
//           wait_for_flash_ready(&fstorage);     
//             if(rc == NRF_SUCCESS){
               vTaskDelay(100);
                rc = nrf_fstorage_write(  //write the token 
                &fstorage,   
                0xef100,     
                &gwaddr,       
                4, 
                NULL           
                );
   
            APP_ERROR_CHECK(rc);
            wait_for_flash_ready(&fstorage);
            vTaskDelay(100);  
        //  }

//              rc = nrf_fstorage_erase(
//                &fstorage,   
//                0xb7000,     
//                1, 
//                NULL           
//                );      
//           APP_ERROR_CHECK(rc);
//           wait_for_flash_ready(&fstorage);     
//             if(rc == NRF_SUCCESS){
               vTaskDelay(100);
                rc = nrf_fstorage_write(  //write the token 
                &fstorage,   
                0xef200,     
                &netmask,       
                4, 
                NULL           
                );
   
            APP_ERROR_CHECK(rc);
            wait_for_flash_ready(&fstorage);
            vTaskDelay(100);   
       //   }
    strcpy(ipaddress_changed,"unchanged");
}

if((strcmp(dhcp_check_changed,"true") == 0)){

              rc = nrf_fstorage_erase(
                &fstorage,   
                0xf0000,     
                1, 
                NULL           
                );
           APP_ERROR_CHECK(rc);
           wait_for_flash_ready(&fstorage); 
             if(rc == NRF_SUCCESS){
      vTaskDelay(100);
    // nrf_delay_ms(1);
      rc = nrf_fstorage_write(  //write the token 
       &fstorage,   
       0xf0000,     
       dhcp_check_wifi,       
       8, 
       NULL           
      );
   
    APP_ERROR_CHECK(rc);
    wait_for_flash_ready(&fstorage);
    vTaskDelay(100); 
    NRF_LOG_INFO("Dhcp WRIITEN IN FLASH")
    }

              rc = nrf_fstorage_erase(
                &fstorage,   
                0xf1000,     
                1, 
                NULL           
                );
           APP_ERROR_CHECK(rc);
           wait_for_flash_ready(&fstorage); 
             if(rc == NRF_SUCCESS){
      vTaskDelay(100);
    // nrf_delay_ms(1);
      rc = nrf_fstorage_write(  //write the token 
       &fstorage,   
       0xf1000,     
       dhcp_check_eth,       
       8, 
       NULL           
      );
   
    APP_ERROR_CHECK(rc);
    wait_for_flash_ready(&fstorage);
    vTaskDelay(100); 
    NRF_LOG_INFO("Dhcp WRIITEN IN FLASH")
    }

        strcpy(dhcp_check_changed,"false");
}

      if(strcmp(update,"true")== 0){    // if received a command to update firmware
              sd_power_gpregret_set(0,BOOTLOADER_DFU_START);
              sd_nvic_SystemReset();
            }

      else{
          vTaskDelay(100);    // just a delay to keep the task from hanging
          }   
  }
}  

void app_error_handler(ret_code_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
  __disable_irq();
  while(1)
  {
  }
}

void vApplicationStackOverflowHook (void){
   __disable_irq();
  while(1)
  {
  } 
}

/**@brief Application main function.
 */
int main(void) {
 
  volatile ret_code_t ret;
  uint32_t i;
  bool erase_bonds;
  log_init();    
  ble_init();
  APP_ERROR_CHECK(nrf_mem_init());
  APP_ERROR_CHECK(nrf_tls_init());
  timers_init();
  buttons_leds_init(&erase_bonds);
  power_management_init();
  saadc_init();
  pf_spim_init();
  ret = nrf_drv_ppi_init();
  APP_ERROR_CHECK(ret);
  if (!nrf_drv_gpiote_is_init())
  {
     ret = nrf_drv_gpiote_init();
     APP_ERROR_CHECK(ret);
  }
  APP_ERROR_CHECK(onewire_init());
  init_keys();
  #ifdef BOARD_CUSTOM
  APP_ERROR_CHECK (htu21d_hw_init());
  #endif
  // randomize_session_Id();
  /* Create task for network*/
  UNUSED_VARIABLE(xTaskCreate(storage_task_fn, "storage", configMINIMAL_STACK_SIZE + 1024, NULL, 3, &storage_task_handle)); 
  UNUSED_VARIABLE(xTaskCreate(network_task_fn, "network", configMINIMAL_STACK_SIZE + 512, NULL, 2, &network_task_handle));
  UNUSED_VARIABLE(xTaskCreate(sensors_task_fn, "sensors", configMINIMAL_STACK_SIZE + 512, NULL, 3, &sensors_task_handle));
   
 
  /* Start FreeRTOS scheduler. */
  NRF_LOG_INFO("system started. \n\r");
  vTaskStartScheduler();  
  // Enter main loop.
  for (;;) {
    ASSERT(false);
    /* FreeRTOS should not be here... FreeRTOS goes back to the start of stack
         * in vTaskStartScheduler function. */
  }
}

