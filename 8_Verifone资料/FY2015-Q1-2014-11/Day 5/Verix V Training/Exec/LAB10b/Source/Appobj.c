#include <string.h>
#include "svc.h"
#include "ucldefine.h"
#include "..\include\appobj.h"

// For Ethernet

static char baudRateCO = Rt_9600; // always Rt_9600
static char formatCO = Fmt_A8N1|Fmt_DTR|Fmt_RTS; // for Ethernet it must be always Fmt_A8N1
static char protocolCO = P_char_mode;
static char parameterCO = 0x00;
static unsigned char hangUpStringCO[MAX_HANGUP_STR_LEN] = "ATH";
static unsigned char initStringCO[MAX_INIT_STR_LEN] = "AT+iE0";
static unsigned char startUpStringCO[MAX_STARTUP_STR_LEN] =  "AT+iBDRA";//"ATB0&K0%C0\N3&D2+MS=V32S7=60";
static unsigned char modelNoCO[MAX_MODEL_SIZE] =  "ETHERNET_ICHIP"; 



short GetParamCO(short paramId, unsigned char *buffer, short size)
{
	if((buffer == NULL) ||(size <= 0))
		return E_UCL_INVALID_PARM;
	
	memset(buffer,0,size);
	
	switch (paramId)
	{

	case BAUDRATE:
			buffer[0] = baudRateCO;
			buffer[1] = '\0';
			return SINGLE_CHAR_SIZE;
	
	case FORMAT:
			buffer[0] = formatCO;
			buffer[1] = '\0';
			return SINGLE_CHAR_SIZE;
	case PARAMETER:
			buffer[0] = parameterCO;
			buffer[1] = '\0';
			return SINGLE_CHAR_SIZE;
	case PROTOCOL:
			buffer[0] = protocolCO;
			buffer[1] = '\0';
			return SINGLE_CHAR_SIZE;

	case INIT_STRING:
			memcpy(buffer, initStringCO,MAX_INIT_STR_LEN);
			return MAX_INIT_STR_LEN;
	case HANGUP_STRING:
			memcpy(buffer, hangUpStringCO,MAX_HANGUP_STR_LEN);
			return MAX_HANGUP_STR_LEN;
	case STARTUP_STRING:
			memcpy(buffer, startUpStringCO,MAX_STARTUP_STR_LEN);
			return MAX_STARTUP_STR_LEN;
	case MODEL_NO:
			memcpy(buffer, modelNoCO,MAX_MODEL_SIZE);
			return MAX_MODEL_SIZE;
	default:
		break;
	}
	return UCL_FAILURE;
};


short SetParamCO(short paramId, unsigned char *buffer, short size)
{
	if(buffer == NULL)
		return UCL_FAILURE;
	if(size == 0)
		return UCL_FAILURE;
	switch (paramId)
	{
	case BAUDRATE:
			baudRateCO = buffer[0];
			break;
	case FORMAT:
			formatCO = buffer[0];
			break;

	case PROTOCOL:
			protocolCO = buffer[0];
			break;
	case PARAMETER:
			parameterCO = buffer[0];
			break;
	case INIT_STRING:
			memset(initStringCO, '\0', MAX_INIT_STR_LEN);
			memcpy(initStringCO, buffer,size);
			return size;
	case HANGUP_STRING:
			memset(hangUpStringCO, '\0', MAX_HANGUP_STR_LEN);
			memcpy(hangUpStringCO, buffer,size);
			return size;
	case STARTUP_STRING:
			memset(startUpStringCO, '\0', MAX_STARTUP_STR_LEN);
			memcpy(startUpStringCO, buffer,size);
			return size;
	case MODEL_NO:
			memset(modelNoCO, '\0', MAX_MODEL_SIZE);
			memcpy(modelNoCO, buffer,size);
			return size;
	default:

		return UCL_FAILURE;
	}
	return UCL_SUCCESS;
};

short NotifyCO(short state)
{
	return state;
}


AppObj appObj;

AppObj *CreateAppObj(short type)
{
	switch (type)
	{
	case 4:
		appObj.super.GetParam = GetParamCO;
		appObj.super.Notify = NotifyCO;
		appObj.SetParam = SetParamCO;
		break;


	default:
		return NULL;
	}
	return  &appObj;
};
