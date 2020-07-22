#pragma once
#include "TrecLib.h"
#include "TString.h"

// Debugging functions meant to log the operation of AnaGame

/**
 * enum class: LogType
 * Purpose: Deescribes the type of Logging to be done
 */
typedef enum class LogType {
	lt_memory,
	lt_code,
	lt_pointer,
	lt_bnf
}LogType;

/**
 * Function: Log
 * Purpose: Applies logging to a specific logging file
 * Parameters: LogType lt - The type of log to document
 *				 TString& logMessage - The message to write to the file
 * Returns: void
 */
void _TREC_LIB_DLL Log(LogType lt, TString& logMessage);


/**
 * Function: CloseLog
 * Purpose: Closes the Logging Module (called by DLL Main on shutdown)
 * Parameters: void
 * Returns: void
 */
void CloseLog();