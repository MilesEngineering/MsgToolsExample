#ifndef MSG_ALIASES_H__
#define MSG_ALIASES_H__

#include "Core.h"

#define PrintfMessage               Core_PrintfMessage
#define PrintfIDMessage             Core_PrintfIDMessage
#define SetDebugThresholdMessage    Core_SetDebugThresholdMessage
#define GetThreadInfoMessage        Core_GetThreadInfoMessage
#define ThreadInfoMessage           Core_ThreadInfoMessage
#define GetDeviceInfoMessage        Core_GetDeviceInfoMessage
#define DeviceInfoMessage           Core_DeviceInfoMessage
#define LowLevelFaultsMessage       Core_LowLevelFaultsMessage
#define GetResetInfoMessage         Core_GetResetInfoMessage
#define ResetInfoMessage            Core_ResetInfoMessage
#define ClearConfigSettingsMessage  Core_ClearConfigSettingsMessage

/* These don't exist in the example app, and the code in ucplatform that uses them is protected by ifdefs.

#define GetConfigKeysMessage        AppExample_GetConfigKeysMessage
#define GetConfigValueMessage       AppExample_GetConfigValueMessage
#define SetConfigValueMessage       AppExample_SetConfigValueMessage
#define DeleteConfigValueMessage    AppExample_DeleteConfigValueMessage
#define CurrentConfigKeysMessage    AppExample_CurrentConfigKeysMessage
#define CurrentConfigValueMessage   AppExample_CurrentConfigValueMessage
*/

#define CORE_INTERFACE_ID        MESSAGES_CORE_FILE_HASH

#endif
