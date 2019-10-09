//
//  flash.h
//  testFATFS
//
//  Created by 付誉阳 on 2019/10/7.
//  Copyright © 2019 付誉阳. All rights reserved.
//

#ifndef flash_h
#define flash_h

#include <stdio.h>

#define uint32_t    unsigned int
#define uint16_t    unsigned short
#define uint8_t     unsigned char

#define SECTOR_SIZE     (4*1024)
#define BLOCK_SIZE      (SECTOR_SIZE*8)
#define FLASH_SIZE      (32*1024*1024)
#define FLASH_DEF_VAL   (0xFF)

//static void flash_sector_erase(uint32_t address,uint32_t length);
void flash_erase(uint32_t address,uint32_t len);
void flash_write(uint32_t address,uint8_t *buff,uint32_t len);
void flash_read(uint32_t address,uint8_t *buff,uint32_t len);

#endif /* flash_h */
