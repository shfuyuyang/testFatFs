/*
 * app_moviesave.c
 *
 *  Created on: 2019Äê10ÔÂ9ÈÕ
 *      Author: fangxing
 */
//#include <string.h>
#include "ff.h"
#include "flash.h"
#include "app_moviesave.h"

uint8_t workSpace[SECTOR_SIZE]={0};

static FATFS fs;
static FIL fp;
static DIRF dp;

uint32_t movieIDStr2Int(char *str)
{
	uint32_t result=0;
	uint32_t i=0;
	for(i=0;str[i]!='.'&&str[i]!='\0';i++)
	{
		result*=10;
		result+=(str[i]-'0');
	}
	return result;
}

uint8_t slotID2dir(uint8_t slotID,char *nameString)
{
	int i=0;
	for(i=SLOT_STRING_LEN;i>=0;i--)
	{
		if(SLOT_STRING_LEN==i)
		{
			nameString[i]='\0';
		}
		else
		{
			nameString[i]=(char)((slotID%10)+'0');
			slotID/=10;
		}
	}
	return 0;
}

uint8_t movieID2nameString(uint32_t movieID,char *nameString)
{

	int i=0;
	for(i=MOVIE_STRING_LEN;i>=0;i--)
	{
		if(MOVIE_STRING_LEN==i)
		{
			nameString[i]='.';
		}
		else
		{
			nameString[i]=(char)((movieID%10)+'0');
			movieID/=10;
		}
	}
	nameString[MOVIE_STRING_LEN+1]='b';
	nameString[MOVIE_STRING_LEN+2]='i';
	nameString[MOVIE_STRING_LEN+3]='n';
	nameString[MOVIE_STRING_LEN+4]='\0';
	return 0;
}

uint8_t GetSlotInfo(uint8_t slotID,uint32_t *movieIDList,uint8_t *movieIDListSize)
{
	FRESULT result;
	char path[20]={'\0'};
	slotID2dir(slotID,path);
	result=f_opendir(&dp,path);
	if(FR_OK==result)
	{
		FILINFO fi;
		*movieIDListSize=0;
		f_readdir(&dp,&fi);
		while(fi.fname[0]!='\0')
		{
			movieIDList[*movieIDListSize]=movieIDStr2Int(fi.fname);
			(*movieIDListSize)++;
			f_readdir(&dp,&fi);
		}
	}
	else
	{
		return RE_Slot_Invalid;
	}
	return RE_SuccessAll;
}
uint8_t GetSlotInfoAll(uint8_t **movieIDList,uint8_t *validSlotNum,uint8_t *movieIDListSize)
{
    return 0;
}

uint8_t ReadMovie(uint8_t slotID ,uint32_t movieID, MovieFrame * movieFrame)
{

#define MAX_PATH_LEN	20

	FRESULT result=FR_OK;
	static char path[MAX_PATH_LEN]={'\0'};
	uint32_t len=0;
	if(movieFrame->index==0)	//first frame
	{
		uint32_t pathlen=0;
		slotID2dir(slotID,path);
		result=f_opendir(&dp,path);
		pathlen=(uint32_t)strlen(path);
		if(FR_NO_PATH==result)
		{
			return RE_Slot_Invalid;
		}
		path[pathlen++]='/';
		path[pathlen]='\0';
		movieID2nameString(movieID,path+pathlen);
		pathlen=(uint32_t)strlen(path);
		result=f_open(&fp,path,FA_READ);
		if(FR_OK!=result)return RE_FlashError;
		result=f_read(&fp, (uint8_t *)movieFrame, sizeof(MovieFrame), &len);
		if(FR_OK!=result)return RE_FlashError;
		result=f_read(&fp, movieFrame->frameData, movieFrame->frameLen, &len);
		if(FR_OK!=result)return RE_FlashError;
	}
	else if(movieFrame->index+movieFrame->frameLen>=movieFrame->movieLen) 	//last frame
	{
		result=f_read(&fp, (uint8_t *)movieFrame, sizeof(MovieFrame), &len);
		if(FR_OK!=result)return RE_FlashError;
		result=f_read(&fp, movieFrame->frameData, movieFrame->frameLen, &len);
		if(FR_OK!=result)return RE_FlashError;
		result=f_close(&fp);
		if(FR_OK!=result)return RE_FlashError;
	}
	else		//middle frame
	{
		result=f_read(&fp, (uint8_t *)movieFrame, sizeof(MovieFrame), &len);
		if(FR_OK!=result)return RE_FlashError;
		result=f_read(&fp, movieFrame->frameData, movieFrame->frameLen, &len);
		if(FR_OK!=result)return RE_FlashError;
	}
	return RE_SuccessAll;

}
uint8_t WriteMovie(uint8_t slotID ,uint32_t movieID,MovieFrame * movieFrame)
{
#define MAX_PATH_LEN	20

	FRESULT result=FR_OK;
	char path[MAX_PATH_LEN]={'\0'};
	uint32_t len=0;

	if(movieFrame->index==0)	//first frame
	{
		uint32_t pathlen=0;
		slotID2dir(slotID,path);
		result=f_opendir(&dp,path);
		pathlen=(uint32_t)strlen(path);
		if(FR_NO_PATH==result)
		{
			result=f_mkdir(path);
			if(FR_OK==result)
			{
				result=f_opendir(&dp,path);
			}
			else
			{
				return RE_Slot_Invalid;
			}
		}
		path[pathlen++]='/';
		path[pathlen]='\0';
		movieID2nameString(movieID,path+pathlen);
		pathlen=(uint32_t)strlen(path);
		result=f_open(&fp,path,FA_CREATE_NEW|FA_WRITE);
		if(FR_OK!=result)return RE_FlashError;
		result=f_write(&fp, (uint8_t *)movieFrame, sizeof(MovieFrame), &len);
		if(FR_OK!=result)return RE_FlashError;
		result=f_write(&fp, movieFrame->frameData, movieFrame->frameLen, &len);
		if(FR_OK!=result)return RE_FlashError;
	}
	else if(movieFrame->index+movieFrame->frameLen>=movieFrame->movieLen) 	//last frame
	{
		result=f_write(&fp, (uint8_t *)movieFrame, sizeof(MovieFrame), &len);
		if(FR_OK!=result)return RE_FlashError;
		result=f_write(&fp, movieFrame->frameData, movieFrame->frameLen, &len);
		if(FR_OK!=result)return RE_FlashError;
		result=f_close(&fp);
		if(FR_OK!=result)return RE_FlashError;
	}
	else		//middle frame
	{
		result=f_write(&fp, (uint8_t *)movieFrame, sizeof(MovieFrame), &len);
		if(FR_OK!=result)return RE_FlashError;
		result=f_write(&fp, movieFrame->frameData, movieFrame->frameLen, &len);
		if(FR_OK!=result)return RE_FlashError;
	}
	return RE_SuccessAll;
}
uint8_t EnableMovie(uint8_t slotID ,uint32_t movieID,uint32_t movieEnable)
{
    FRESULT result=FR_OK;
    uint32_t i=0;
    MovieFrame mf;
    
    char path[MAX_PATH_LEN]={'\0'};
    uint32_t pathlen=0;
    slotID2dir(slotID,path);
    pathlen=(uint32_t)strlen(path);
    path[pathlen++]='/';
    path[pathlen]='\0';
    movieID2nameString(movieID,path+pathlen);
    pathlen=(uint32_t)strlen(path);
    
    result=f_open(&fp, path, FA_READ);
    result=f_read(&fp, (uint8_t*)&mf, sizeof(mf), &i);
    result=f_close(&fp);
    
    mf.movieEnable=movieEnable;
    
    result=f_open(&fp, path, FA_WRITE);
    result=f_write(&fp, (uint8_t*)&mf, sizeof(mf), &i);
    f_close(&fp);
    
    return 0;
}
uint8_t DisableMovie(uint8_t slotID ,uint32_t movieID)
{
    
    return 0;
}
uint8_t DeleteMovie(uint8_t slotID ,uint32_t movieID)
{

	FRESULT result=FR_OK;
    char path[MAX_PATH_LEN]={'\0'};

	uint32_t pathlen=0;
	slotID2dir(slotID,path);
	pathlen=(uint32_t)strlen(path);
	path[pathlen++]='/';
	path[pathlen]='\0';
	movieID2nameString(movieID,path+pathlen);
	pathlen=(uint32_t)strlen(path);
	result=f_unlink(path);
	if(FR_OK!=result&&FR_NO_FILE!=result&&FR_NO_PATH!=result)return RE_FlashError;

	return RE_SuccessAll;
}
uint8_t DeleteMovieAll()
{
	FRESULT result=0;

	flash_erase(0,FLASH_SIZE);
	result = f_mkfs("", FM_ANY, SECTOR_SIZE, workSpace,sizeof(workSpace));
	if(FR_OK!=result)return RE_FlashError;
	result=f_mount(&fs, "0:", 1);
	if(FR_OK!=result)return RE_FlashError;
	return RE_SuccessAll;
}

uint8_t LoadMovieSave()
{
	FRESULT result=0;
	result=f_mount(&fs, "0:", 1);
	if(FR_OK!=result)return RE_FlashError;
	return RE_SuccessAll;
}
