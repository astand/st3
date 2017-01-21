/* ------------------------------------------------------------------------- *
 * Main debug drive config
 * ------------------------------------------------------------------------- */

#pragma once


/* If this global definition enable -> task debug is demploy --------------- */
#define TASKDBG

#define GSM_DEBUG
#define GSM2_DEBUG
#define PROT_DEBUG
#define MODEM_DEBUG
#define COMMON_DEBUG
//#define GPS_DBG
//#define GPS_2DBG

#define FalsePrintf while(0)((int (*)(const char *, ...))0)
/* Gsm low level debug ----------------------------------------------------- */
#if defined (TASKDBG) && defined(GSM_DEBUG)
#define DBG_Gsm printf
#else
#define DBG_Gsm FalsePrintf
#endif

/* Gsm high level debug ---------------------------------------------------- */
#if defined (TASKDBG) && defined (GSM2_DEBUG)
#define DBG_2Gsm printf
#else
#define DBG_2Gsm FalsePrintf
#endif

/* Prot debug -------------------------------------------------------------- */
#if defined (TASKDBG) && defined (PROT_DEBUG)
#define DBG_Prot printf
#else
#define DBG_Prot FalsePrintf
#endif

/* Prot binary protocol debug ---------------------------------------------- */
#if defined (TASKDBG) && defined (MODEM_DEBUG)
#define DBG_Modem printf
#else
#define DBG_Modem FalsePrintf
#endif

/* common thread debug ----------------------------------------------------- */
#if defined (TASKDBG) && defined (COMMON_DEBUG)
#define DBG_Common printf
#else
#define DBG_Common FalsePrintf
#endif

/* Gps raw data output ----------------------------------------------------- */
#if defined (TASKDBG) && defined (GPS_DBG)
#define DBG_Gps printf
#else
#define DBG_Gps FalsePrintf
#endif

/* Gps high level debug ---------------------------------------------------- */
#if defined (TASKDBG) && defined (GPS_2DBG)
#define DBG_2Gps printf
#else
#define DBG_2Gps FalsePrintf
#endif
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- *
 * Number for SMS answer
 * ------------------------------------------------------------------------- */
/*Number for SMS answer */
//#define SMS_HOME

#ifdef SMS_HOME
#define DBG_PHONE_ACK		"+79299095608"
#define DBG_PHONE_SET   strcpy((char *)Inc_Phone_In, DBG_PHONE_ACK)
#else
#define DBG_PHONE_SET   while(0)
#endif

