#include "gfx/hal/inc/gfx_display.h"
#include "gfx/hal/inc/gfx_common.h"


GFX_DisplayInfo GFX_DisplayInfoList[] =
{
    {
	    "",  // description
		GFX_COLOR_MODE_RGB_565,  // color mode
		{
			0,  // x position (always 0)
			0,  // y position (always 0)
			800,  // display width
			480, // display height
		},
		{
		    16,  // data bus width
		    {
				128,  // horizontal pulse width
				129,  // horizontal back porch
				2,  // horizontal front porch
		    },
		    {
				2,  // vertical pulse width
				41,  // vertical back porch
				2,  // vertical front porch
		    },
			0,  // inverted left shift
		},
	},
};