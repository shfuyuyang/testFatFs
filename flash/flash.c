//
//  flash.c
//  testFATFS
//
//  Created by 付誉阳 on 2019/10/7.
//  Copyright © 2019 付誉阳. All rights reserved.
//

#include "flash.h"
#include <string.h>

uint8_t flash_memory[FLASH_SIZE]={FLASH_DEF_VAL};

static void flash_sector_erase(uint32_t address,uint32_t length)
{
    uint32_t addr=0,len=0;
    addr=(address/SECTOR_SIZE)*SECTOR_SIZE;
    len=length+address%SECTOR_SIZE;
    while(len)
    {
        memset(flash_memory+addr, FLASH_DEF_VAL, SECTOR_SIZE);
        if(len>SECTOR_SIZE)
        {
            len-=SECTOR_SIZE;
        }
        else
        {
            len=0;
        }
    }
}

void flash_erase(uint32_t address,uint32_t len)
{
    flash_sector_erase(address, len);
}

void flash_write(uint32_t address,uint8_t *buff,uint32_t len)
{
    uint32_t i=0;
    for(i=0;i<len;i++)
    {
        flash_memory[address+i]&=buff[i];
    }
}

void flash_read(uint32_t address,uint8_t *buff,uint32_t len)
{
    memcpy(buff, flash_memory+address, len);
}


