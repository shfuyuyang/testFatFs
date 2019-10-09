//
//  main.c
//  testFATFS
//
//  Created by 付誉阳 on 2019/10/8.
//  Copyright © 2019 付誉阳. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "ff.h"
#include "flash.h"
#include "app_moviesave.h"

//uint8_t workSpace[SECTOR_SIZE]={0};

uint8_t buffer[300];

uint32_t movieIDList[20]={0};
uint32_t movieIDList_i=0;

MovieFrame mf;

int main(int argc, const char * argv[]) {
    
    unsigned int i=0,j=0,k=0;
    for(i=0;i<300;i++)
    {
        buffer[i]=(unsigned char)i;
    }
    i=0;
    
    mf.index=0;
    mf.movieLen=300;
    mf.frameData=buffer;
    
#if 1
    
    DeleteMovieAll();
    
    for(i=1;i<200;i++)
    {
        for(k=0;k<5;k++)
        {
            mf.index=0;
            while(mf.index<mf.movieLen)
            {
                uint32_t res=0;
                mf.frameLen=300;
                mf.movieLen=300*50;
                mf.frameData[0]=i;
                mf.frameData[1]=k;
                mf.frameData[2]=0x55;
                res=WriteMovie(i,k,&mf);
                if(res!=0)
                {
                    mf.loop=0;
                }
                mf.index+=mf.frameLen;
            }
        }
        if(i>200)
        {
            return 0;
        }
    }
    
#endif
    
    //DeleteMovieAll();
    
    LoadMovieSave();
    
    EnableMovie(149, 4,M_Disable);
    
    mf.frameLen=sizeof(mf);
    mf.index=0;
    
    ReadMovie(149,4,&mf);
    
    mf.index=300*50;
    ReadMovie(149,4,&mf);
    
    GetSlotInfo(130, movieIDList, &movieIDList_i);
    
    DeleteMovie(130, 3);
    
    GetSlotInfo(130, movieIDList, &movieIDList_i);
    
    
    
    if(mf.index==99)
    {
        return 0;
    }

    return 0;
}
