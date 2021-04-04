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
#include <time.h>

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


extern const uint8_t c_bmpWhiteDot[19*20*sizeof(uint16_t)];
extern const arm_2d_tile_t c_tPictureWhiteDot;


extern const uint8_t c_bmpSmallWhiteDot[];
const arm_2d_tile_t c_tileSmallWhiteDot = {
    .tRegion = {
        .tSize = {
            .iWidth = 7,
            .iHeight = 7
        },
    },
    .tInfo.bIsRoot = true,
    .phwBuffer = (uint16_t *)c_bmpSmallWhiteDot,
};


const arm_2d_tile_t c_tileSemisphere = {
    .tRegion = {
        .tLocation = {
            .iX = 0,
            .iY = 0,
        },
        .tSize = {
            .iWidth = 4,
            .iHeight = 7
        },
    },
    .tInfo.bIsRoot = false,
    .ptParent = (arm_2d_tile_t *)&c_tileSmallWhiteDot,
};

/*============================ IMPLEMENTATION ================================*/

static volatile uint32_t s_wSystemTimeInMs = 0;
//static volatile bool s_bTimeout = false;
extern void platform_1ms_event_handler(void);

static volatile uint_fast8_t s_chDemoProgress = 0;

void platform_1ms_event_handler(void)
{
    s_wSystemTimeInMs++; 
    if (!(s_wSystemTimeInMs & (_BV(4) - 1))) {
        //! every 256 ms
        
        srand(clock()); //!< update rand seed
        
        s_chDemoProgress += ((uint64_t)rand() * 5ll) / (uint64_t)RAND_MAX;
        //s_chDemoProgress++;
        if (s_chDemoProgress >= 1000) {
            s_chDemoProgress = 1000;
        }
        
    }
}

void example_gui_init(void)
{
    controls_init();
    
}


void example_gui_do_events(void)
{

}


void show_progress_bar(arm_2d_tile_t *ptFrameBuffer, int_fast16_t iProgress)
{
    int_fast16_t iWidth = ptFrameBuffer->tRegion.tSize.iWidth * 3 >> 3;         //!< 40% Width
    
    arm_2d_region_t tBarRegion = {
        .tLocation = {
           .iX = (ptFrameBuffer->tRegion.tSize.iWidth - iWidth) / 2,
           .iY = (ptFrameBuffer->tRegion.tSize.iHeight - c_tileSmallWhiteDot.tRegion.tSize.iHeight) / 2,
        },
        .tSize = {
            .iWidth = iWidth,
            .iHeight = c_tileSmallWhiteDot.tRegion.tSize.iHeight,
        },
    };
    
    //! draw a white box
    arm_2d_rgb16_fill_colour(ptFrameBuffer, &tBarRegion, GLCD_COLOR_WHITE);
    
    
    //! draw semispheres
    do {
        arm_2d_region_t tSemisphere = {
            .tSize = c_tileSemisphere.tRegion.tSize,
            .tLocation = {
                .iX = tBarRegion.tLocation.iX - c_tileSemisphere.tRegion.tSize.iWidth,
                .iY = tBarRegion.tLocation.iY,
            },
        };
        arm_2d_rgb16_tile_copy( &c_tileSemisphere,          //!< source tile
                                ptFrameBuffer,              //!< display buffer
                                &tSemisphere,               //!< region to draw
                                ARM_2D_CP_MODE_COPY);       //!< copy only
                               
        tSemisphere.tLocation.iX = tBarRegion.tLocation.iX + tBarRegion.tSize.iWidth;
        arm_2d_rgb16_tile_copy( &c_tileSemisphere,          //!< source tile
                                ptFrameBuffer,              //!< display buffer
                                &tSemisphere,               //!< region to draw
                                ARM_2D_CP_MODE_COPY |       //!< copy with x-mirroring
                                ARM_2D_CP_MODE_X_MIRROR );   
    } while(0);
    
    //! draw inner bar
    tBarRegion.tSize.iHeight-=2;
    tBarRegion.tSize.iWidth-=2;
    tBarRegion.tLocation.iX += 1;
    tBarRegion.tLocation.iY += 1;
    arm_2d_rgb16_fill_colour(ptFrameBuffer, &tBarRegion, GLCD_COLOR_BLACK);
    
    
    //! calculate the width of the inner stripe 
    tBarRegion.tSize.iWidth = tBarRegion.tSize.iWidth * iProgress / 1000;
    
    //! draw the inner stripe
    arm_2d_rgb16_fill_colour(ptFrameBuffer, &tBarRegion, GLCD_COLOR_WHITE);
    
}


void example_gui_refresh(arm_2d_tile_t *ptFrameBuffer)
{
    arm_2d_rgb16_fill_colour(ptFrameBuffer, NULL, GLCD_COLOR_BLACK);
    
    
    show_progress_bar(ptFrameBuffer, s_chDemoProgress);
    //busy_wheel_show(ptFrameBuffer);
}


#if defined(__clang__)
#   pragma clang diagnostic pop
#endif


