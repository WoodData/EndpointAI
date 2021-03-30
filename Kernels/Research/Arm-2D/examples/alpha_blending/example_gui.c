/*
 * Copyright (c) 2009-2021 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*============================ INCLUDES ======================================*/
#include <stdio.h>
#include "platform.h"
#include "example_gui.h"
#include "./display_adapter/display_adapter.h"
#include "./controls/controls.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-braces"
#   pragma clang diagnostic ignored "-Wunused-const-variable"
#endif

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define arm_2d_layer(__TILE_ADDR, __TRANS, __X, __Y, ...)                       \
    {                                                                           \
        .ptTile = (__TILE_ADDR),                                                \
        .tRegion.tLocation.iX = (__X),                                          \
        .tRegion.tLocation.iY = (__Y),                                          \
        .chTransparency = (__TRANS),                                            \
        __VA_ARGS__                                                             \
    }

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ LOCAL VARIABLES ===============================*/

declare_tile(c_tLogoCMSIS)
implement_tile(c_tLogoCMSIS, 163, 65, arm_2d_color_rgb565_t);

declare_tile(c_tLayerA)
implement_tile(c_tLayerA, 100, 100, arm_2d_color_rgb565_t);


declare_tile(c_tLayerB)
implement_tile(c_tLayerB, 200, 50, arm_2d_color_rgb565_t);

/*! picture helium */
extern const uint8_t c_bmpHelium[320 * 256 * sizeof(uint16_t)];
const static arm_2d_tile_t c_tPictureHeliun = {
    .tRegion = {
        .tSize = {
            .iWidth = 320,
            .iHeight = 256
        },
    },
    .tInfo.bIsRoot = true,
    .phwBuffer = (uint16_t *)c_bmpHelium,
};


/*! picture cmsis_logo */
extern const uint8_t c_bmpCMSISLogo[163 * 65 * sizeof(uint32_t)];
const static arm_2d_tile_t c_tPictureCMSISLogo = {
    .tRegion = {
        .tSize = {
            .iWidth = 163,
            .iHeight = 65
        },
    },
    .tInfo = {
        .bIsRoot = true,
        .bHasEnforcedColour = true,
        .tColourInfo = {
            .chScheme = ARM_2D_COLOUR_RGBA8888,
        },
    },
    .pwBuffer = (uint32_t *)c_bmpCMSISLogo,
};

/*! picture small logo smile */
extern const uint16_t c_bmpSmile [32 * 32];
const static arm_2d_tile_t c_tPictureSmile = {
    .tRegion = {
        .tSize = {
            .iWidth = 32,
            .iHeight = 32
        },
    },
    .tInfo.bIsRoot = true,
    .phwBuffer = (uint16_t *)c_bmpSmile,
};

extern const uint8_t c_bmpSun[57*56*sizeof(uint16_t)];
const static arm_2d_tile_t c_tPictureSun = {
    .tRegion = {
        .tSize = {
            .iWidth = 57,
            .iHeight = 56
        },
    },
    .tInfo.bIsRoot = true,
    .phwBuffer = (uint16_t *)c_bmpSun,
};

extern const uint8_t c_bmpWhiteDot[19*20*sizeof(uint16_t)];
extern const arm_2d_tile_t c_tPictureWhiteDot;


static arm_2d_layer_t s_ptRefreshLayers[] = {
    arm_2d_layer(&c_tPictureHeliun, 0, -50, -100),
    arm_2d_layer(&c_tLayerA, 128, 10, 80),
    arm_2d_layer(&c_tLayerB, 144, 50, 150),
    arm_2d_layer(&c_tPictureSun, 0, 0, 0, 
                    .bIsIrregular = true, 
                    .hwMaskingColour = GLCD_COLOR_WHITE),
};

static floating_range_t s_ptFloatingBoxes[] = {
    {
        .tRegion = {{0-100, 0-100}, {320 + 200, 256 + 200}},
        .ptLayer = &s_ptRefreshLayers[0],
        .tOffset = {-1, -1},
    },
    {
        .tRegion = {{0, 0}, {320, 240}},
        .ptLayer = &s_ptRefreshLayers[1],
        .tOffset = {5, -2},
    },
    {
        .tRegion = {{0, 0}, {320, 240}},
        .ptLayer = &s_ptRefreshLayers[2],
        .tOffset = {-2, 4},
    },
    {
        .tRegion = {{-100, -100}, {320+200, 240+200}},
        .ptLayer = &s_ptRefreshLayers[3],
        .tOffset = {5, 5},
    },
};

/*============================ IMPLEMENTATION ================================*/

static volatile uint32_t s_wSystemTimeInMs = 0;
static volatile bool s_bTimeout = false;
extern void platform_1ms_event_handler(void);

void platform_1ms_event_handler(void)
{
    s_wSystemTimeInMs++; 
    if (!(s_wSystemTimeInMs & (_BV(10) - 1))) {
        s_bTimeout = true;
    }
}

void example_gui_init(void)
{
    controls_init();
    
    

    s_ptRefreshLayers[0].wMode = ARM_2D_CP_MODE_FILL;

    arm_2d_rgb16_fill_colour(s_ptRefreshLayers[1].ptTile, NULL, GLCD_COLOR_RED);
    arm_2d_rgb16_fill_colour(s_ptRefreshLayers[2].ptTile, NULL, GLCD_COLOR_GREEN);
    arm_2d_convert_colour_to_rgb565(&c_tPictureCMSISLogo, &c_tLogoCMSIS);

    arm_foreach(arm_2d_layer_t, s_ptRefreshLayers) {
        arm_2d_region_t tRegion = _->tRegion;
        if (!tRegion.tSize.iHeight) {
            tRegion.tSize.iHeight = _->ptTile->tRegion.tSize.iHeight;
        }
        if (!tRegion.tSize.iWidth) {
            tRegion.tSize.iWidth = _->ptTile->tRegion.tSize.iWidth;
        }
        
        _->tRegion = tRegion;
    }
}

static void example_update_boxes(floating_range_t *ptBoxes, uint_fast16_t hwCount)
{
    ASSERT(NULL != ptBoxes);
    ASSERT(hwCount > 0);

    do {
        arm_2d_region_t tOldRegion = ptBoxes->ptLayer->tRegion;
        if (   (tOldRegion.tLocation.iX + tOldRegion.tSize.iWidth + ptBoxes->tOffset.iX)
            >= ptBoxes->tRegion.tLocation.iX + ptBoxes->tRegion.tSize.iWidth) {
            ptBoxes->tOffset.iX = -ptBoxes->tOffset.iX;
        }

        if (    (tOldRegion.tLocation.iX + ptBoxes->tOffset.iX)
            <   (ptBoxes->tRegion.tLocation.iX)) {
            ptBoxes->tOffset.iX = -ptBoxes->tOffset.iX;
        }

        if (   (tOldRegion.tLocation.iY + tOldRegion.tSize.iHeight + ptBoxes->tOffset.iY)
            >= ptBoxes->tRegion.tLocation.iY + ptBoxes->tRegion.tSize.iHeight) {
            ptBoxes->tOffset.iY = -ptBoxes->tOffset.iY;
        }

        if (    (tOldRegion.tLocation.iY + ptBoxes->tOffset.iY)
            <   (ptBoxes->tRegion.tLocation.iY)) {
            ptBoxes->tOffset.iY = -ptBoxes->tOffset.iY;
        }

        ptBoxes->ptLayer->tRegion.tLocation.iX += ptBoxes->tOffset.iX;
        ptBoxes->ptLayer->tRegion.tLocation.iY += ptBoxes->tOffset.iY;

        ptBoxes++;

    }while(--hwCount);
}

void example_gui_do_events(void)
{
    static uint32_t s_wCounter = 0;

    example_update_boxes(s_ptFloatingBoxes, dimof(s_ptFloatingBoxes));
    
    if (s_bTimeout) {                       //!< every 1024 ms
        s_bTimeout = false;
        
        if((s_wCounter & 0x0F) < 0x04) {
            s_ptRefreshLayers[0].wMode =    ARM_2D_CP_MODE_FILL;
        } else if((s_wCounter & 0x0F) < 0x08) {
            s_ptRefreshLayers[0].wMode =    ARM_2D_CP_MODE_FILL       | 
                                            ARM_2D_CP_MODE_X_MIRROR;
        } else if((s_wCounter & 0x0F) < 0x0C) {
            s_ptRefreshLayers[0].wMode =    ARM_2D_CP_MODE_FILL       | 
                                            ARM_2D_CP_MODE_Y_MIRROR;
        } else {
            s_ptRefreshLayers[0].wMode =    ARM_2D_CP_MODE_FILL       | 
                                            ARM_2D_CP_MODE_X_MIRROR   | 
                                            ARM_2D_CP_MODE_Y_MIRROR;
        }
        
        s_wCounter++;
    }
}



static void __draw_layers(   arm_2d_tile_t *ptFrameBuffer,
                            arm_2d_layer_t *ptLayers, 
                            uint_fast16_t hwCount)
{
    ASSERT(NULL != ptLayers);
    ASSERT(hwCount > 0);

    static const arm_2d_region_t tFillRegion = {-200, -100, 320 + 200, 240 + 100 };
    

    do {
        
        arm_2d_rgb16_tile_copy( &c_tLogoCMSIS,
                                ptFrameBuffer,
                                &tFillRegion,
                                ptLayers[0].wMode);
       
        
        arm_2d_rgb16_tile_copy_with_colour_masking( &c_tPictureSun,
                                                    s_ptRefreshLayers[2].ptTile,
                                                    NULL,
                                                    GLCD_COLOR_WHITE,
                                                    ARM_2D_CP_MODE_FILL);
        
        
        arm_foreach(arm_2d_layer_t, ptLayers, hwCount, ptLayer) {
            arm_2d_region_t tRegion = ptLayer->tRegion;

            if (ptLayer->bIsIrregular) {
                arm_2d_rgb16_tile_copy_with_colour_masking( ptLayer->ptTile,
                                            ptFrameBuffer,
                                            &tRegion,
                                            ptLayer->hwMaskingColour,
                                            ARM_2D_CP_MODE_COPY);
            } else {
                if (ptLayer->chTransparency) {
                    arm_2d_rgb565_alpha_blending(   ptLayer->ptTile,
                                                    ptFrameBuffer,
                                                    &tRegion,
                                                    255 - ptLayer->chTransparency);
                } else {
                    arm_2d_rgb16_tile_copy( ptLayer->ptTile,
                                            ptFrameBuffer,
                                            &tRegion,
                                            ARM_2D_CP_MODE_COPY);
                }
            }
        }
        
        //! show progress wheel
        busy_wheel_show(ptFrameBuffer);
        
    } while (0);
}

void example_gui_refresh(arm_2d_tile_t *ptFrameBuffer)
{
    __draw_layers(ptFrameBuffer, s_ptRefreshLayers, dimof(s_ptRefreshLayers));
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif


