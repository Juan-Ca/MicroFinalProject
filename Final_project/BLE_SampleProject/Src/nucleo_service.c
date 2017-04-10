#include "nucleo_service.h"
/**
 * @brief  Add a nucleo service using a vendor specific profile.
 *
 * @param  None
 * @retval tBleStatus Status
 */

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)


#define COPY_NUCLEO_SERVICE_UUID(uuid_struct)			COPY_UUID_128(uuid_struct,0x02,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_X_VALUES_UUID(uuid_struct)						COPY_UUID_128(uuid_struct,0xe2,0x3e,0x78,0xa0, 0xcf,0x4a, 0x11,0xe1, 0x8f,0xfc, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_Y_VALUES_UUID(uuid_struct)       		COPY_UUID_128(uuid_struct,0xcd,0x20,0xc4,0x80, 0xe4,0x8b, 0x11,0xe2, 0x84,0x0b, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_Z_VALUES_UUID(uuid_struct)     			COPY_UUID_128(uuid_struct,0x01,0xc5,0x0b,0x60, 0xe4,0x8c, 0x11,0xe2, 0xa0,0x73, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_ANGLE_VALUES_UUID(uuid_struct)  			COPY_UUID_128(uuid_struct,0x08,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)

/* Store Value into a buffer in Little Endian Format */
#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) )
								   
extern float x_values[250], y_values[250], z_values[250];

uint16_t nucleoServHandle, xAxisValHandle, yAxisValHandle, zAxisValHandle, angleValHandle;
uint8_t angle_index = 0;
float angles[10];

tBleStatus Add_Nucleo_Service(void)
{
  tBleStatus ret;

  uint8_t uuid[16];
  
  COPY_NUCLEO_SERVICE_UUID(uuid);
  ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 16,
                          &nucleoServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;    
  
  COPY_X_VALUES_UUID(uuid);
  ret =  aci_gatt_add_char(nucleoServHandle, UUID_TYPE_128, uuid, 4,
                           CHAR_PROP_NOTIFY|CHAR_PROP_READ, ATTR_PERMISSION_NONE, 
													 GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                           16, 0, &xAxisValHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
		
	COPY_Y_VALUES_UUID(uuid);
  ret =  aci_gatt_add_char(nucleoServHandle, UUID_TYPE_128, uuid, 4,
                           CHAR_PROP_NOTIFY|CHAR_PROP_READ, ATTR_PERMISSION_NONE, 
													 GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                           16, 0, &yAxisValHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
	
	COPY_Z_VALUES_UUID(uuid);
  ret =  aci_gatt_add_char(nucleoServHandle, UUID_TYPE_128, uuid, 4,
                           CHAR_PROP_NOTIFY|CHAR_PROP_READ, ATTR_PERMISSION_NONE, 
													 GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                           16, 0, &zAxisValHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
	
	COPY_ANGLE_VALUES_UUID(uuid);
	ret =  aci_gatt_add_char(nucleoServHandle, UUID_TYPE_128, uuid, 4,
                           CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
                           16, 0, &angleValHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
    
  PRINTF("Service NUCLEO added. Handle 0x%04X, X axis Charac handle: 0x%04X, Y axis Charac handle: 0x%04X, Z axis Charac handle: 0x%04X, Angle Charac handle: 0x%04X\n", nucleoServHandle, xAxisValHandle, yAxisValHandle, zAxisValHandle, angleValHandle);	
  return BLE_STATUS_SUCCESS; 
  
fail:
  PRINTF("Error while adding NUCLEO service.\n");
  return BLE_STATUS_ERROR ;
    
}

/**
 * @brief  Update Nucleo characteristic value.
 *
 * @param  Array of float values
 * @retval Status
 */
tBleStatus x_Val_Update(float value)
{  
  tBleStatus ret;    
  uint8_t buff[4];
  float value_buff = value;
	
  memcpy(buff, &value_buff, 4);
  ret = aci_gatt_update_char_value(nucleoServHandle, xAxisValHandle, 0, 4, buff);
	
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating X characteristic.\n") ;
    return BLE_STATUS_ERROR ;
  }
	PRINTF("Sent data\n") ;
  return BLE_STATUS_SUCCESS;	
}

/**
 * @brief  Update Nucleo characteristic value.
 *
 * @param  Array of float values
 * @retval Status
 */
tBleStatus y_Val_Update(float value)
{  
  tBleStatus ret;    
  uint8_t buff[4];
  float value_buff = value;
	
  memcpy(buff, &value_buff, 4);
  ret = aci_gatt_update_char_value(nucleoServHandle, yAxisValHandle, 0, 4, buff);
	
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating Y characteristic.\n") ;
    return BLE_STATUS_ERROR ;
  }
	PRINTF("Sent data\n") ;
  return BLE_STATUS_SUCCESS;	
}
/**
 * @brief  Update Nucleo characteristic value.
 *
 * @param  Array of float values
 * @retval Status
 */
tBleStatus z_Val_Update(float value)
{  
  tBleStatus ret;    
  uint8_t buff[4];
  float value_buff = value;
	
  memcpy(buff, &value_buff, 4);
  ret = aci_gatt_update_char_value(nucleoServHandle, zAxisValHandle, 0, 4, buff);
	
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating Z characteristic.\n") ;
    return BLE_STATUS_ERROR ;
  }
	PRINTF("Sent data\n") ;
  return BLE_STATUS_SUCCESS;	
}

/**
 * @brief  This function is called attribute value corresponding to 
 *         ledButtonCharHandle characteristic gets modified.
 * @param  Handle of the attribute
 * @param  Size of the modified attribute data
 * @param  Pointer to the modified attribute data
 * @retval None
 */
void Attribute_Modified_Nucleo(uint16_t handle, uint8_t data_length, uint8_t *att_data){
	if(handle == (angleValHandle + 1)){
		if(angle_index < 10){
			float temp;
			memcpy(&temp, att_data, data_length);
			angles[angle_index] = temp;
		}
		angle_index = (angle_index >= 9) ? 0 : ++angle_index;
		if(angle_index == 9){
			for(uint8_t i = 0; i < 10; i++){
				printf("angles[%d] = %f\n", i, angles[i]);
			}
		}
	}else{
		printf("Unknown characteristic was modified\n");
	}
}