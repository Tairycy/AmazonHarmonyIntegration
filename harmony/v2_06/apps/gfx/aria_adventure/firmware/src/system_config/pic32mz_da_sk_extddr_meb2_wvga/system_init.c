/*******************************************************************************
  System Initialization File

  File Name:
    system_init.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources, such as the
    sysObj structure that contains the object handles to all the MPLAB Harmony
    module objects in the system.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
// <editor-fold defaultstate="collapsed" desc="Configuration Bits">

/*** DEVCFG0 ***/

#pragma config DEBUG =      OFF
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx2
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
#pragma config FECCCON =    OFF_UNLOCKED
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     PG_ALL
#pragma config SMCLR =      MCLR_NORM
#pragma config SOSCGAIN =   GAIN_LEVEL_3
#pragma config SOSCBOOST =  ON
#pragma config POSCGAIN =   GAIN_LEVEL_3
#pragma config POSCBOOST =  ON
#pragma config POSCFGAIN =  GAIN_G3
#pragma config POSCAGCDLY =  POSCAGCDLY1
#pragma config POSCAGCRNG =  RANGE_1X
#pragma config POSCAGC =  OFF
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF

/*** DEVCFG1 ***/

#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    ON
#pragma config IESO =       ON
#pragma config POSCMOD =    EC
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSECME
#pragma config WDTPS =      PS1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT31
#pragma config FDMTEN =     OFF
/*** DEVCFG2 ***/

#pragma config FPLLIDIV =   DIV_3
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_POSC
#pragma config FPLLMULT =   MUL_50
#pragma config FPLLODIV =   DIV_2
#pragma config VBATBOREN =  ON
#pragma config DSBOREN =    ON
#pragma config DSWDTPS =    DSPS32
#pragma config DSWDTOSC =   LPRC
#pragma config DSWDTEN =    OFF
#pragma config FDSEN =      ON
#pragma config UPLLFSEL =   FREQ_24MHZ
/*** DEVCFG3 ***/

#pragma config USERID =     0xffff
#pragma config EXTDDRSIZE = DDR_SIZE_128MB
#pragma config FMIIEN =     OFF
#pragma config FETHIO =     ON
#pragma config PGL1WAY =    ON
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON

/*** DEVCFG4 ***/

#pragma config SWDTPS = SPS1048576

/*** BF1SEQ0 ***/

#pragma config TSEQ =       0x0000
#pragma config CSEQ =       0xffff
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="DRV_CODEC_AK4953 Initialization Data">
/*** CODEC Driver Initialization Data ***/
const DRV_AK4953_INIT drvak4953Codec0InitData =
{
    .moduleInit.value = SYS_MODULE_POWER_RUN_FULL,
    .i2sDriverModuleIndex = DRV_AK4953_I2S_DRIVER_MODULE_INDEX_IDX0,
    .i2cDriverModuleIndex = DRV_AK4953_I2C_DRIVER_MODULE_INDEX_IDX0,

    .volume = DRV_AK4953_VOLUME,



};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_I2C Initialization Data">
// *****************************************************************************
/* I2C Driver Initialization Data
*/

const DRV_I2C_INIT drvI2C0InitData =
{
    .i2cId = DRV_I2C_PERIPHERAL_ID_IDX0,
    .i2cMode = DRV_I2C_OPERATION_MODE_IDX0,
    .portSCL = DRV_SCL_PORT_IDX0,
	.pinSCL  = DRV_SCL_PIN_POSITION_IDX0,
	.portSDA = DRV_SDA_PORT_IDX0,
	.pinSDA  = DRV_SDA_PIN_POSITION_IDX0,
    .baudRate = DRV_I2C_BAUD_RATE_IDX0,
    .busspeed = DRV_I2C_SLEW_RATE_CONTROL_IDX0,
    .buslevel = DRV_I2C_SMBus_SPECIFICATION_IDX0,
    .mstrInterruptSource = DRV_I2C_MASTER_INT_SRC_IDX0,
    .errInterruptSource = DRV_I2C_ERR_MZ_INT_SRC_IDX0,

};


const DRV_I2C_INIT drvI2C1InitData =
{
    .i2cId = DRV_I2C_PERIPHERAL_ID_IDX1,
    .i2cMode = DRV_I2C_OPERATION_MODE_IDX1,
    .portSCL = DRV_SCL_PORT_IDX1,
	.pinSCL  = DRV_SCL_PIN_POSITION_IDX1,
	.portSDA = DRV_SDA_PORT_IDX1,
	.pinSDA  = DRV_SDA_PIN_POSITION_IDX1,
    .buslevel = DRV_I2C_SMBus_SPECIFICATION_IDX1,
    .baudRate = DRV_I2C_BAUD_RATE_IDX1,
    .busspeed = DRV_I2C_SLEW_RATE_CONTROL_IDX1,
    .mstrInterruptSource = DRV_I2C_MASTER_INT_SRC_IDX1,
    .errInterruptSource = DRV_I2C_ERR_MZ_INT_SRC_IDX1,

};



// </editor-fold>
 // <editor-fold defaultstate="collapsed" desc="DRV_I2S Initialization Data">
/*** I2S Driver Initialization Data ***/
const DRV_I2S_INIT drvI2S0InitData =
{
    .moduleInit.value = DRV_I2S_POWER_STATE_IDX0,
    .spiID = DRV_I2S_PERIPHERAL_ID_IDX0, 
    .usageMode = DRV_I2S_USAGE_MODE_IDX0,
    .baudClock = SPI_BAUD_RATE_CLK_IDX0,
    .baud = DRV_I2S_BAUD_RATE,
    .clockMode = DRV_I2S_CLK_MODE_IDX0,
    .audioCommWidth = SPI_AUDIO_COMM_WIDTH_IDX0,
    .audioTransmitMode = SPI_AUDIO_TRANSMIT_MODE_IDX0,
    .inputSamplePhase = SPI_INPUT_SAMPLING_PHASE_IDX0,
    .protocolMode = DRV_I2S_AUDIO_PROTOCOL_MODE_IDX0,
    .queueSizeTransmit = QUEUE_SIZE_TX_IDX0,
    .queueSizeReceive = QUEUE_SIZE_RX_IDX0,
    .dmaChannelTransmit = DRV_I2S_TX_DMA_CHANNEL_IDX0,
    .txInterruptSource = DRV_I2S_TX_INT_SRC_IDX0,    
    .dmaInterruptTransmitSource = DRV_I2S_TX_DMA_SOURCE_IDX0,    
    .dmaChannelReceive = DMA_CHANNEL_NONE,
};





// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_INPUT_MXT336T Initialization Data">
/*** MXT336T Driver Initialization Data ***/

const DRV_MXT336T_INIT drvMXT336TInitData =
{
    .drvOpen                     = DRV_I2C_Open,
    .orientation                 = 0,
    .horizontalResolution        = 800,
    .verticalResolution          = 480,
	
};

// </editor-fold>
/*** TMR Driver Initialization Data ***/

const DRV_TMR_INIT drvTmr0InitData =
{
    .moduleInit.sys.powerState = DRV_TMR_POWER_STATE_IDX0,
    .tmrId = DRV_TMR_PERIPHERAL_ID_IDX0,
    .clockSource = DRV_TMR_CLOCK_SOURCE_IDX0,
    .prescale = DRV_TMR_PRESCALE_IDX0,
    .mode = DRV_TMR_OPERATION_MODE_16_BIT,
    .interruptSource = DRV_TMR_INTERRUPT_SOURCE_IDX0,
    .asyncWriteEnable = false,
};

// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************

/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Module Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="SYS_CLK Initialization Data">
// *****************************************************************************
/* System Clock Initialization Data
*/
const SYS_CLK_INIT sysClkInit =
{
    .moduleInit = {0},
    .systemClockFrequencyHz = SYS_CLK_FREQ,
    .waitTillComplete = true,
    .onWaitInstruction = SYS_CLK_ON_WAIT,
};
// </editor-fold>
//<editor-fold defaultstate="collapsed" desc="SYS_DMA Initialization Data">
/*** System DMA Initialization Data ***/

const SYS_DMA_INIT sysDmaInit =
{
	.sidl = SYS_DMA_SIDL_DISABLE,

};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_INPUT Initialization Data">
// *****************************************************************************
/* Input System Initialization Data
*/

const SYS_INPUT_Init sysInputInit =
{
    0
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_TMR Initialization Data">
/*** TMR Service Initialization Data ***/
const SYS_TMR_INIT sysTmrInitData =
{
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},
    .drvIndex = DRV_TMR_INDEX_0,
    .tmrFreq = 1000, 
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
    See prototype in system/common/sys_module.h.
 */

void SYS_Initialize ( void* data )
{
    /**************DO NOT REMOVE***Enable MPLL and REFCLK 5********************/
    SYS_DEVCON_SystemUnlock ( );
    /* Memory PLL */
	PLIB_DEVCON_MPLLVregEnable( DEVCON_ID_0 );
	while (!PLIB_DEVCON_MPLLVregIsReady(DEVCON_ID_0));

	PLIB_DEVCON_MPLLVrefSet(DEVCON_ID_0, DEVCON_MPLL_VREF_EXT);
    PLIB_DEVCON_MPLLInputDivSet( DEVCON_ID_0, 1 );
    PLIB_DEVCON_MPLLMultiplierSet ( DEVCON_ID_0, 25 );
    PLIB_DEVCON_MPLLODiv1Set( DEVCON_ID_0, DEVCON_MPLL_ODIV_3 );
    PLIB_DEVCON_MPLLODiv2Set( DEVCON_ID_0, DEVCON_MPLL_ODIV_1 );
    PLIB_DEVCON_MPLLEnable( DEVCON_ID_0 );
	while(!PLIB_DEVCON_MPLLIsReady( DEVCON_ID_0 ));
    
    /* ROSEL System Clock SYSCLK */
    PLIB_OSC_ReferenceOscBaseClockSelect ( OSC_ID_0, OSC_REFERENCE_5, 0 );
    /* RODIV */
    PLIB_OSC_ReferenceOscDivisorValueSet ( OSC_ID_0, OSC_REFERENCE_5, 1 );
    /* ROTRIM */
    PLIB_OSC_ReferenceOscTrimSet ( OSC_ID_0, OSC_REFERENCE_5, 0 );

    PLIB_OSC_ReferenceOscEnable ( OSC_ID_0, OSC_REFERENCE_5 );
    /* Disable REFCLK5_OE*/
    PLIB_OSC_ReferenceOutputDisable ( OSC_ID_0, OSC_REFERENCE_5 );    

    SYS_DEVCON_SystemLock ( );
    /**************************************************************************/
    
    /* Core Processor Initialization */
    SYS_CLK_Initialize( NULL );
    SYS_DEVCON_Initialize(SYS_DEVCON_INDEX_0, (SYS_MODULE_INIT*)NULL);
    SYS_DEVCON_PerformanceConfig(SYS_CLK_SystemFrequencyGet());

    /* Board Support Package Initialization */
    BSP_Initialize();        

    /* Initialize Drivers */
    sysObj.drvI2C0 = DRV_I2C_Initialize(DRV_I2C_INDEX_0, (SYS_MODULE_INIT *)&drvI2C0InitData);
    sysObj.drvI2C1 = DRV_I2C_Initialize(DRV_I2C_INDEX_1, (SYS_MODULE_INIT *)&drvI2C1InitData);


    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C1_MASTER, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C1_MASTER, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C1_BUS, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C1_BUS, INT_SUBPRIORITY_LEVEL0);

    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C2_MASTER, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C2_MASTER, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C2_BUS, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C2_BUS, INT_SUBPRIORITY_LEVEL0);

    sysObj.drvI2S0 = DRV_I2S_Initialize(DRV_I2S_INDEX_0, (SYS_MODULE_INIT *)&drvI2S0InitData);


    sysObj.sysDma = SYS_DMA_Initialize((SYS_MODULE_INIT *)&sysDmaInit);
    SYS_INT_VectorPrioritySet(INT_VECTOR_DMA0, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_DMA0, INT_SUBPRIORITY_LEVEL0);

    SYS_INT_SourceEnable(INT_SOURCE_DMA_0);


    sysObj.drvak4953Codec0 = DRV_AK4953_Initialize(DRV_AK4953_INDEX_0, (SYS_MODULE_INIT *)&drvak4953Codec0InitData);

    sysObj.drvMXT336T = DRV_MXT336T_Initialize(0, (SYS_MODULE_INIT *)&drvMXT336TInitData);
    /* Disable SDHC module */
	PLIB_POWER_PeripheralModuleDisable(POWER_ID_0, POWER_MODULE_SDHC);

    sysObj.drvTmr0 = DRV_TMR_Initialize(DRV_TMR_INDEX_0, (SYS_MODULE_INIT *)&drvTmr0InitData);

    SYS_INT_VectorPrioritySet(INT_VECTOR_T1, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_T1, INT_SUBPRIORITY_LEVEL0);
 
 
  
    // initialize the GFX hardware abstraction layer
    GFX_Initialize();


    /* Initialize System Services */
    SYS_PORTS_Initialize();
    SYS_INP_Init((SYS_MODULE_INIT*)&sysInputInit);


    /*** Interrupt Service Initialization Code ***/
    SYS_INT_Initialize();

    /*Setup the INT_SOURCE_EXTERNAL_4 and Enable it*/
    SYS_INT_VectorPrioritySet(INT_VECTOR_INT4, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_INT4, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_ExternalInterruptTriggerSet(INT_EXTERNAL_INT_SOURCE4,INT_EDGE_TRIGGER_FALLING);
    SYS_INT_SourceEnable(INT_SOURCE_EXTERNAL_4);





    SYS_MEMORY_Initialize();

    /*** TMR Service Initialization Code ***/
    sysObj.sysTmr  = SYS_TMR_Initialize(SYS_TMR_INDEX_0, (const SYS_MODULE_INIT  * const)&sysTmrInitData);

    /* Initialize Middleware */
 
    // initialize UI library
    LibAria_Initialize();


    /* Enable Global Interrupts */
    SYS_INT_Enable();

    /* Initialize the Application */
    APP_Initialize();
}


/*******************************************************************************
 End of File
*/

