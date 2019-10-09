/*
 * app_moviesave.h
 *
 *  Created on: 2019��10��9��
 *      Author: fangxing
 */

#ifndef SYSTEM_APPLICATION_APP_MOVIESAVE_H_
#define SYSTEM_APPLICATION_APP_MOVIESAVE_H_

#define SLOT_STRING_LEN         3
#define MOVIE_STRING_LEN        5
#define MAX_PATH_LEN            20



enum RESULT
{
   RE_SuccessAll=0,
   RE_SuccessNow,
   RE_Slot_Empty,
   RE_Slot_Invalid,
   RE_Movie_Not_In_Slot,
   RE_FlashFull,
   RE_FlashError
};

enum MovieEnable
{
    M_Enable=0,
    M_Disable
};

//movie֡�ṹ�嶨��
typedef struct
{
	uint32_t loop;			//movie�ļ�ѭ������
	uint32_t loopEnable;			//����ֵ1��Ԥ����
	uint32_t movieEnable;			//����ֵ2��Ԥ����
	uint32_t movieLen;		//��ʾmoive�ļ��ܳ���
	uint32_t index;			//��ʾ��ǰ֡�������ڵ�movie�ļ���λ��
	uint32_t frameLen;		//��ʾ��ǰ֡�ĳ���
	uint8_t* frameData;		//���ڴ洢֡����
} MovieFrame;

uint32_t movieIDStr2Int(char *str);

uint8_t slotID2dir(uint8_t slotID,char *nameString);
uint8_t movieID2nameString(uint32_t movieID,char *nameString);
uint8_t GetSlotInfo(uint8_t slotID,uint32_t *movieIDList,uint8_t *movieIDListSize);
uint8_t GetSlotInfoAll(uint8_t **movieIDList,uint8_t *validSlotNum,uint8_t *movieIDListSize);
uint8_t ReadMovie(uint8_t slotID ,uint32_t movieID,MovieFrame * movieFrame);
uint8_t WriteMovie(uint8_t slotID ,uint32_t movieID,MovieFrame * movieFrame);
uint8_t EnableMovie(uint8_t slotID ,uint32_t movieID,uint32_t movieEnable);
//uint8_t DisableMovie(uint8_t slotID ,uint32_t movieID);
uint8_t DeleteMovie(uint8_t slotID ,uint32_t movieID);
uint8_t DeleteMovieAll(void);
uint8_t LoadMovieSave(void);


#endif /* SYSTEM_APPLICATION_APP_MOVIESAVE_H_ */
