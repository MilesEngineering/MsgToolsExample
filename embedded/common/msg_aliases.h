#ifndef MSG_ALIASES_H__
#define MSG_ALIASES_H__

#include "Core.h"

/* You need to #include msgtools generated header files that define the following messages,
   or messages with the following Aliases:
    DebugPrintf
    DebugPrintfID
    SetDebugThreshold
    GetStreamDebugInfo
    DebugStreamInfo
    DebugDeviceInfo
    GetResetInfo
    ResetInfo
    LowLevelFaults
    GetDeviceDebugInfo
*/

/* Configuration settings functionality is enabled by defining messages with the following
   Aliases.  The code is protected by #ifdef, so if you don't define these aliases the
   functionality is disabled.  Because ifdef checks are used, you must define these as
   aliases and not just as regular message names:
    ClearConfigSettings
    GetConfigKeysMessage
    GetConfigValueMessage
    SetConfigValueMessage
    DeleteConfigValueMessage
    CurrentConfigKeysMessage
    CurrentConfigValueMessage
*/

#define CORE_INTERFACE_ID        MESSAGES_CORE_FILE_HASH

#endif
