/* Define to prevent recursive inclusion -------------------------------------*/  
#ifndef __SAMPLE_SERVICE
#define __SAMPLE_SERVICE

/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"
#include "hal_types.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_gap.h"
#include "string.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "hci_const.h"
#include "gp_timer.h"
#include "bluenrg_hal_aci.h"
#include "bluenrg_aci_const.h"   
#include "hci.h"
#include "hal.h"
#include "sm.h"
#include "debug.h"

#include <stdlib.h>

#define IDB04A1 0
#define IDB05A1 1

typedef int i32_t;


tBleStatus Add_Nucleo_Service(void);
tBleStatus x_Val_Update(float value);
tBleStatus y_Val_Update(float value);
tBleStatus z_Val_Update(float value);

#endif
