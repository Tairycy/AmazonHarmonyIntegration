/*******************************************************************************
  MPLAB Harmony LCC Generated Driver Implementation File

  File Name:
    drv_gfx_lcc_generic.c

  Summary:
    Build-time generated implementation for the LCC Driver.

  Description:
    Build-time generated implementation for the LCC Driver.

    Created with MPLAB Harmony Version 2.05
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2016 released Microchip Technology Inc.  All rights reserved.

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

#include "framework/gfx/driver/controller/lcc/drv_gfx_lcc_generic.h"

#include <xc.h>
#include <sys/attribs.h>

#include "peripheral/osc/plib_osc.h"
#include "peripheral/pmp/plib_pmp.h"
#include "peripheral/tmr/plib_tmr.h"

#ifndef PMADDR_OVERFLOW
#define  PMADDR_OVERFLOW               65536
#endif

#define PIXEL_DRAW_PER_DMA_TX          20
volatile uint8_t DrawCount = 0;        /* The current status of how many pixels have been drawn inbetween a DMA IR*/
volatile uint8_t overflowcount;        /* The count for the amount of overflows that have happened in the PMP Adress*/

#define SRAM_ADDR_CS0  0xE0000000
#define SRAM_ADDR_CS1  0xE0040000

static SYS_DMA_CHANNEL_HANDLE dmaHandle = SYS_DMA_CHANNEL_HANDLE_INVALID;

#define PMP_ADDRESS_LINES 0xffff

#define PMP_DATA_LENGTH PMP_DATA_SIZE_16_BITS 

#define MAX_LAYER_COUNT 1
#define DISPLAY_WIDTH   480
#define DISPLAY_HEIGHT  272

uint16_t GraphicsFrame[DISPLAY_WIDTH];

const char* DRIVER_NAME = "LCC";
static uint32_t supported_color_formats = GFX_COLOR_MASK_RGB_565;

uint32_t state;

#define DRV_GFX_LCC_DMA_CHANNEL_INDEX     DMA_CHANNEL_1
#define DRV_GFX_LCC_DMA_TRIGGER_SOURCE    DMA_TRIGGER_TIMER_2
#define DRV_GFX_LCC_TMR_INDEX             TMR_ID_2
#define DRV_GFX_LCC_DMA_TRANSFER_LENGTH   2

/**** Hardware Abstraction Interfaces ****/
enum
{
    INIT = 0,
    RUN
};

static int DRV_GFX_LCC_Start();

GFX_Context* cntxt;

uint16_t HBackPorch;
uint32_t VER_BLANK;

uint32_t DISP_HOR_FRONT_PORCH;
uint32_t DISP_HOR_RESOLUTION;
uint32_t DISP_HOR_BACK_PORCH;
uint32_t DISP_HOR_PULSE_WIDTH;

uint32_t DISP_VER_FRONT_PORCH;
uint32_t DISP_VER_RESOLUTION;
uint32_t DISP_VER_BACK_PORCH;
uint32_t DISP_VER_PULSE_WIDTH;

int16_t line = 0;
uint32_t offset = 0;
uint16_t pixels = 0;
uint32_t hSyncs = 0;
    
uint32_t vsyncPeriod = 0;
uint32_t vsyncPulseDown = 0;
uint32_t vsyncPulseUp = 0;
uint32_t vsyncEnd = 0;


uint16_t HBackPorch;

uint32_t _frameAddress;
uint32_t _previousAddress;

void lccStopDisplayRefresh(void)
{
    GFX_Context* context = GFX_ActiveContext();
    GFX_Layer* layer;
    
    layer = context->layer.active;

    while(DCH1CSIZ != 1);
    
    while(DrawCount > PIXEL_DRAW_PER_DMA_TX);
	
    DMACONbits.SUSPEND = 1;
	
    while(PMMODEbits.BUSY == 1); //WAIT for DMA transfer to be suspended

    BSP_SRAM_A15StateSet(_frameAddress >> 15);
    BSP_SRAM_A16StateSet(_frameAddress >> 16);
    
    BSP_SRAM_A17StateSet(layer->buffer_write_idx);
    BSP_SRAM_A18StateSet(layer->buffer_write_idx>>1);

    //Save previous address value
    _previousAddress = PMADDR;
    PMADDR = _frameAddress;    
}

void lccStartDisplayRefresh(void)
{
    GFX_Context* context = GFX_ActiveContext();
    GFX_Layer* layer;
    
    layer = context->layer.active;

    PMADDR = _previousAddress;

    BSP_SRAM_A15StateSet(overflowcount);
    BSP_SRAM_A16StateSet(overflowcount >> 1);
    BSP_SRAM_A17StateSet(layer->buffer_read_idx);
    BSP_SRAM_A18StateSet(layer->buffer_read_idx>>1);	
    DMACONbits.SUSPEND = 0;	

    DrawCount++;	
}

// function that returns the information for this driver
GFX_Result driverLCCInfoGet(GFX_DriverInfo* info)
{
	if(info == NULL)
        return GFX_FAILURE;

	// populate info struct
    strcpy(info->name, DRIVER_NAME);
    info->color_formats = supported_color_formats;
    info->layer_count = MAX_LAYER_COUNT;
    
    return GFX_SUCCESS;
}

static GFX_Result lccUpdate()
{
    GFX_Context* context = GFX_ActiveContext();
   
    if(context == NULL)
        return GFX_FAILURE;
    
    if(state == INIT)
    {
        if(DRV_GFX_LCC_Start() != 0)
            return GFX_FAILURE;
        
        state = RUN;
    }
    
    return GFX_SUCCESS;
}

static void lccDestroy(GFX_Context* context)
{	
	// driver specific shutdown tasks
	if(context->driver_data != GFX_NULL)
	{
		context->memory.free(context->driver_data);
		context->driver_data = GFX_NULL;
	}
	
	// general default shutdown
	defDestroy(context);
}

static GFX_Result layerBufferCountSet(uint32_t count)
{
    GFX_Context* context = GFX_ActiveContext();
    
    if(count == 0 || count > 2)
        return GFX_FAILURE;
   
    context->layer.active->buffer_count = count;
        
    return GFX_FAILURE;
}

static GFX_Result layerBufferAddressSet(uint32_t idx, GFX_Buffer address)
{
    idx = 0;
    address = address;
    
    return GFX_FAILURE;
}

static GFX_Result layerBufferAllocate(uint32_t idx)
{
    idx = 0;
    
    return GFX_FAILURE;
}

static GFX_Color pixelGet(const GFX_PixelBuffer* buffer, const GFX_Point* pnt)
{
	GFX_Color clr;
	
	lccStopDisplayRefresh();
	
    clr = GFX_PixelBufferGet(buffer, pnt);
	
	lccStartDisplayRefresh();
    
    return clr;
}

static GFX_Result pixelArrayGet(GFX_BufferSelection source,
                                const GFX_Rect* rect,
                                GFX_PixelBuffer* result)
{
    GFX_Context* context = GFX_ActiveContext();
    GFX_Layer* layer;
    GFX_PixelBuffer* sourceBuffer;
	GFX_Result res;
    
    layer = context->layer.active;
    
    if(source == GFX_BUFFER_READ)
        sourceBuffer = &layer->buffers[layer->buffer_read_idx].pb;
    else
        sourceBuffer = &layer->buffers[layer->buffer_write_idx].pb;
        
	lccStopDisplayRefresh();
		
    res = GFX_PixelBufferAreaGet(sourceBuffer,
                                 rect,
                                 &context->memory,
                                 result);
								 
	lccStartDisplayRefresh();
								 
	return res;
}

static GFX_Result pixelSet(const GFX_PixelBuffer* buffer,
                           const GFX_Point* pnt,
                           GFX_Color color)
{
	GFX_Result res = GFX_SUCCESS;
	
	lccStopDisplayRefresh();
	
    _frameAddress = (uint32_t)(((pnt->y)*(DISP_HOR_RESOLUTION))+(pnt->x));
    PMDIN = color;
    
	//res = GFX_PixelBufferSet_Unsafe(buffer, pnt, color);
	
	lccStartDisplayRefresh();
	
	return res;
}

static void layerSwapped(GFX_Layer* layer)
{
    uint32_t addr = 0;
    
    addr = (uint32_t)layer->buffers[layer->buffer_read_idx].pb.pixels;
    addr = addr >> 18;
    
    BSP_SRAM_A17StateSet((addr & 0x1));
    BSP_SRAM_A18StateSet((addr & 0x2) >> 1);
	//BSP_SRAM_A19StateSet((addr & 0x4) >> 2);
}

static GFX_Result lccInitialize(GFX_Context* context)
{
	cntxt = context;
	
	// general default initialization
	if(defInitialize(context) == GFX_FAILURE)
		return GFX_FAILURE;	
		
	// override default HAL functions with LCC specific implementations
    context->hal.update = &lccUpdate;
    context->hal.destroy = &lccDestroy;
	context->hal.layerBufferCountSet = &layerBufferCountSet;
    context->hal.layerBufferAddressSet = &layerBufferAddressSet;
    context->hal.layerBufferAllocate = &layerBufferAllocate;
	context->hal.layerSwapped = &layerSwapped;
	
	context->hal.drawPipeline[GFX_PIPELINE_GCU].pixelSet = &pixelSet;
	context->hal.drawPipeline[GFX_PIPELINE_GCU].pixelGet = &pixelGet;
	context->hal.drawPipeline[GFX_PIPELINE_GCU].pixelArrayGet = &pixelArrayGet;
	
	context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].pixelSet = &pixelSet;
	context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].pixelGet = &pixelGet;
	context->hal.drawPipeline[GFX_PIPELINE_GCUGPU].pixelArrayGet = &pixelArrayGet;
	
	// driver specific initialization tasks	
	// initialize all layer color modes
    GFX_PixelBufferCreate(DISPLAY_WIDTH,
                          DISPLAY_HEIGHT,
                          GFX_COLOR_MODE_RGB_565,
                          (void*)SRAM_ADDR_CS0,
                          &context->layer.layers[0].buffers[0].pb);
            
    context->layer.layers[0].buffers[0].state = GFX_BS_MANAGED;
	
    GFX_PixelBufferCreate(DISPLAY_WIDTH,
                          DISPLAY_HEIGHT,
                          GFX_COLOR_MODE_RGB_565,
                          (void*)SRAM_ADDR_CS1,
                          &context->layer.layers[0].buffers[1].pb);

    context->layer.layers[0].buffers[1].state = GFX_BS_MANAGED;
    
    context->layer.layers[0].buffer_count = 1; // second buffer is disabled at startup
	
	VER_BLANK = context->display_info->attributes.vert.pulse_width +
	            context->display_info->attributes.vert.back_porch +
				context->display_info->attributes.vert.front_porch - 1;
	
	HBackPorch = context->display_info->attributes.horz.pulse_width +
	             context->display_info->attributes.horz.back_porch;
	
	DISP_HOR_FRONT_PORCH = context->display_info->attributes.horz.front_porch;
	DISP_HOR_RESOLUTION = DISPLAY_WIDTH;
	DISP_HOR_BACK_PORCH = context->display_info->attributes.horz.back_porch;
	DISP_HOR_PULSE_WIDTH = context->display_info->attributes.horz.pulse_width;
	
	DISP_VER_FRONT_PORCH = context->display_info->attributes.vert.front_porch;
	DISP_VER_RESOLUTION = DISPLAY_HEIGHT;
	DISP_VER_BACK_PORCH = context->display_info->attributes.vert.back_porch;
	DISP_VER_PULSE_WIDTH = context->display_info->attributes.vert.pulse_width;

    BSP_LCD_HSYNCOff();
    BSP_LCD_VSYNCOff();
    BSP_LCD_CSOff();
    BSP_LCD_RESETOff();
    
    BSP_LCD_BACKLIGHTOff();
    BSP_LCD_DEOff();
    
    BSP_LCD_RESETOn();
    BSP_LCD_CSOn();
	
    /* Disable the PMP module */
    PLIB_PMP_Disable(0);

    PLIB_PMP_OperationModeSelect(0, PMP_MASTER_READ_WRITE_STROBES_INDEPENDENT);

    /* pins polarity setting */
    PLIB_PMP_ReadWriteStrobePolaritySelect(0, 1 - ((cntxt->display_info->attributes.inv_left_shift)));
    PLIB_PMP_WriteEnableStrobePolaritySelect(0, PMP_POLARITY_ACTIVE_LOW);

    PLIB_PMP_ReadWriteStrobePortEnable(0);
    PLIB_PMP_WriteEnableStrobePortEnable(0);

    PLIB_PMP_DataSizeSelect(0, PMP_DATA_LENGTH);

    /* wait states setting */
    PLIB_PMP_WaitStatesDataHoldSelect(0, PMP_DATA_HOLD_1);
    PLIB_PMP_WaitStatesDataSetUpSelect(0, PMP_DATA_WAIT_ONE);
    PLIB_PMP_WaitStatesStrobeSelect(0, PMP_STROBE_WAIT_3);

    /* setting the hardware for the required interrupt mode */
    PLIB_PMP_InterruptModeSelect(0, PMP_INTERRUPT_EVERY_RW_CYCLE);

    PLIB_PMP_AddressIncrementModeSelect(0, PMP_ADDRESS_AUTO_INCREMENT);

    /* Enable the PMP module */
    PLIB_PMP_Enable(0);
    /* Ports Setting */
    PLIB_PMP_AddressPortEnable(0, PMP_ADDRESS_LINES);
    PLIB_PMP_AddressSet(0, 0);

    BSP_SRAM_A15Off();
    BSP_SRAM_A16Off();
    BSP_SRAM_A17Off();
    BSP_SRAM_A18Off();
	
    //BSP_SRAM_A19Off();


    /*Turn Backlight on*/
    BSP_LCD_BACKLIGHTOn();

	return GFX_SUCCESS;
}

// function that initialized the driver context
GFX_Result driverLCCContextInitialize(GFX_Context* context)
{
	// set driver-specific data initialization function address
	context->hal.initialize = &lccInitialize; 
	
	// set driver-specific destroy function address
    context->hal.destroy = &lccDestroy;
	
	return GFX_SUCCESS;
}

static int DRV_GFX_LCC_Start()
{
	/*Suspend DMA Module*/
    SYS_DMA_Suspend();
	    
    /* Allocate DMA channel */
    dmaHandle = SYS_DMA_ChannelAllocate(DRV_GFX_LCC_DMA_CHANNEL_INDEX);
    
	if (SYS_DMA_CHANNEL_HANDLE_INVALID == dmaHandle)
        return -1;
		
    // set the transfer parameters: source & destination address,
    // source & destination size, number of bytes per event
    SYS_DMA_ChannelTransferAdd(dmaHandle, (void *) &PMDIN, 2, &GraphicsFrame[0],
    HBackPorch, DRV_GFX_LCC_DMA_TRANSFER_LENGTH);

    /* Enable the transfer done interrupt, when all buffer transferred */
    PLIB_DMA_ChannelXINTSourceEnable(dmaHandle, DRV_GFX_LCC_DMA_CHANNEL_INDEX,
            DMA_INT_BLOCK_TRANSFER_COMPLETE);

    SYS_INT_SourceEnable(INT_SOURCE_DMA_0 + DRV_GFX_LCC_DMA_CHANNEL_INDEX);

    // set the transfer event control: what event is to start the DMA transfer
    SYS_DMA_ChannelSetup(dmaHandle,
            SYS_DMA_CHANNEL_OP_MODE_BASIC,
            DRV_GFX_LCC_DMA_TRIGGER_SOURCE);

    PLIB_TMR_PrescaleSelect(DRV_GFX_LCC_TMR_INDEX, TMR_PRESCALE_VALUE_1);
    PLIB_TMR_Period16BitSet(DRV_GFX_LCC_TMR_INDEX, 1);
    PLIB_TMR_Start(DRV_GFX_LCC_TMR_INDEX);

    // once we configured the DMA channel we can enable it
    SYS_DMA_ChannelEnable(dmaHandle);
	
	SYS_INT_VectorPrioritySet(INT_VECTOR_DMA1, INT_PRIORITY_LEVEL7);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_DMA1, INT_SUBPRIORITY_LEVEL0);

    /*Unsuspend DMA Module*/
    SYS_DMA_Resume();
	
	return GFX_SUCCESS;
}

void DRV_GFX_LCC_DisplayRefresh(void)
{
    static uint8_t GraphicsState = ACTIVE_PERIOD;
    static uint16_t remaining = 0;
    static short line = 0;
    DrawCount = 0;

    switch(GraphicsState)
    {
        case ACTIVE_PERIOD:
		{
            remaining = DISP_HOR_RESOLUTION;
            GraphicsState = BLANKING_PERIOD;
            DCH1CSIZ = DISP_HOR_RESOLUTION;

            if(line >= 0)
            {
                PMADDR = ((PMP_ADDRESS_LINES) & ((line) * DISP_HOR_RESOLUTION));

                if((line) == (DISP_VER_RESOLUTION))
                {
					if(cntxt->layer.active->vsync == GFX_TRUE &&
					   cntxt->layer.active->swap == GFX_TRUE)
						GFX_LayerSwap(cntxt->layer.active);
				
                    BSP_LCD_VSYNCOff();
                    line = (-VER_BLANK);

                    BSP_SRAM_A15StateSet(0);
                    BSP_SRAM_A16StateSet(0);
                    overflowcount = 0;
				
               }
               else
               {
                    BSP_LCD_DEOn();
					
					if((PMADDR_OVERFLOW - PMADDR) < (DISP_HOR_RESOLUTION))       
					//if((PMADDR_OVERFLOW - PMADDR) < (remaining))       
					{   
						  GraphicsState = OVERFLOW;      //Do Overflow routine
						  
						  PLIB_DMA_ChannelXDestinationSizeSet(DMA_ID_0, DRV_GFX_LCC_DMA_CHANNEL_INDEX, (uint16_t)(((PMADDR_OVERFLOW)- PMADDR)<<1));
						  PLIB_DMA_ChannelXINTSourceFlagClear(DMA_ID_0, DRV_GFX_LCC_DMA_CHANNEL_INDEX, DMA_INT_BLOCK_TRANSFER_COMPLETE);
						  PLIB_DMA_ChannelXEnable(DMA_ID_0,DRV_GFX_LCC_DMA_CHANNEL_INDEX);						  
                          //remaining = remaining - ((PMADDR_OVERFLOW)- PMADDR);
						  remaining = DISP_HOR_RESOLUTION - ((PMADDR_OVERFLOW)- PMADDR);
						  
						  return;
					}           
                }
            }
            break;
	    }
        case OVERFLOW: //Adjust the address lines that aren't part of PMP
	    {
            GraphicsState = BLANKING_PERIOD;     //goto Front Porch
			
            BSP_SRAM_A15StateSet(++overflowcount);
            BSP_SRAM_A16StateSet(overflowcount & 0x1);
			
			break;
		}
        case BLANKING_PERIOD:   //Front Porch then Back Porch Start
		{
            BSP_LCD_HSYNCOff();
            BSP_LCD_DEOff();
            GraphicsState = PMDIN;
            while(PMMODEbits.BUSY == 1);
            BSP_LCD_HSYNCOn();
            BSP_LCD_VSYNCOn();
            //Setup DMA Back Porch
            remaining = HBackPorch;
            GraphicsState = ACTIVE_PERIOD;   
            line++;
            DCH1CSIZ = 1;

            break;
		}
        default:
		{
            break;
		}
    }

    //Setup DMA Transfer
    PLIB_DMA_ChannelXDestinationSizeSet(DMA_ID_0, DRV_GFX_LCC_DMA_CHANNEL_INDEX, (uint16_t) (remaining << 1));
    PLIB_DMA_ChannelXINTSourceFlagClear(DMA_ID_0, DRV_GFX_LCC_DMA_CHANNEL_INDEX, DMA_INT_BLOCK_TRANSFER_COMPLETE);
    SYS_DMA_ChannelEnable(dmaHandle);
}
  


void __ISR(_DMA0_VECTOR + DMA_CHANNEL_1, ipl7AUTO) _LCCRefreshISR(void)
{
    SYS_INT_SourceStatusClear(INT_SOURCE_DMA_0 + DMA_CHANNEL_1);
	
    DRV_GFX_LCC_DisplayRefresh();
}
