#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H

/* This is a temporary workaround for an issue with the peripheral library "Exists"
   functions that causes superfluous warnings.  It "nulls" out the definition of
   The PLIB function attribute that causes the warning.  Once that issue has been
   resolved, this definition should be removed. */
#define _PLIB_UNSUPPORTED
#include "bsp_config.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Clock System Service Configuration Options
*/
#define SYS_CLK_FREQ                        40000000
#define SYS_CLK_BUS_PERIPHERAL_1            40000000
#define SYS_CLK_CONFIG_PRIMARY_XTAL         8000000ul
#define SYS_CLK_CONFIG_SECONDARY_XTAL       32768ul
   
/*** Common System Service Configuration ***/

#define SYS_BUFFER  false
#define SYS_QUEUE   false


/*** Ports System Service Configuration ***/

#define SYS_PORT_AD1PCFG        ~0xffff
#define SYS_PORT_CNPUE          0x0
#define SYS_PORT_CNEN           0x0
  

#define SYS_PORT_D_TRIS         0xfff8
#define SYS_PORT_D_ODC          0x0


#endif // _SYSTEM_CONFIG_H