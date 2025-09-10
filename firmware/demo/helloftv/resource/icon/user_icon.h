/****************************************************************************
       ***             ***                      MAXLIB-GRAPHC                  
      ** **           ** **                                                    
     **   **         **   **            THE MAXLIB FOR IMAGE SHOW PROCESS      
    **     **       **     **                                                  
   **       **     **       **              MAX ROURCE ICON MANAGEMENT         
  **         **   **         **                                                
 **           ** **           **              (C) COPYRIGHT 2016 MAX           
**             ***             **                                              
                                                                               
* File Name   : user_icon.c                                                    
* Author      : Mark.Douglas                                                   
* Version     : V0100                                                          
* Date        : 05/25/2016                                                     
* Description : This file for maxlib resource icon managemant                  
*                                                                              
* History     :                                                                
* 2016-05-25  :                                                                
*      <1>.This is created by mark,set version as v0103.                       
*      <2>.Add basic functions.                                                
******************************************************************************/

#ifndef USER_ICON_H 
   #define USER_ICON_H



#define  TRANSPARENT_COLOR    16

enum
{
   R_ID_ICON_BARNSEL=R_ID_TYPE_ICON, 
   R_ID_ICON_BARSEL,
   R_ID_ICON_LEDOFF,
   R_ID_ICON_LEDON,
   R_ID_ICON_MENUAUDIO,
   R_ID_ICON_MENUBACK,
   R_ID_ICON_MENUBATTERY,
   R_ID_ICON_MENUCLOCK,
   R_ID_ICON_MENUDELALL,
   R_ID_ICON_MENUDELONE,
   R_ID_ICON_MENUEV,
   R_ID_ICON_MENUFORMAT,
   R_ID_ICON_MENUGSENSOR,
   R_ID_ICON_MENUHZ,
   R_ID_ICON_MENUIMAGEQ,
   R_ID_ICON_MENULANGUAGE,
   R_ID_ICON_MENULIGHTNORMAL,
   R_ID_ICON_MENULOCK,
   R_ID_ICON_MENULOOPRECORD,
   R_ID_ICON_MENUMONITOR,
   R_ID_ICON_MENUMOON,
   R_ID_ICON_MENUMOTION,
   R_ID_ICON_MENUMUSIC,
   R_ID_ICON_MENUMUSICPAUSE,
   R_ID_ICON_MENUMUSICPLAY,
   R_ID_ICON_MENUPOWEROFF,
   R_ID_ICON_MENURESET,
   R_ID_ICON_MENURESOLUTION,
   R_ID_ICON_MENUSCRENNOFF,
   R_ID_ICON_MENUSTRAMP,
   R_ID_ICON_MENUTV,
   R_ID_ICON_MENUUNLOCK,
   R_ID_ICON_MENUVERSION,
   R_ID_ICON_MENUVOLUME,
   R_ID_ICON_MTBACK,
   R_ID_ICON_MTBACK1,
   R_ID_ICON_MTBATTERY0,
   R_ID_ICON_MTBATTERY1,
   R_ID_ICON_MTBATTERY2,
   R_ID_ICON_MTBATTERY3,
   R_ID_ICON_MTBATTERY4,
   R_ID_ICON_MTBATTERY5,
   R_ID_ICON_MTFORWARD,
   R_ID_ICON_MTFORWARD1,
   R_ID_ICON_MTIROFF,
   R_ID_ICON_MTIRON,
   R_ID_ICON_MTLEFT,
   R_ID_ICON_MTLOCK,
   R_ID_ICON_MTMENU,
   R_ID_ICON_MTMENU1,
   R_ID_ICON_MTMICOFF,
   R_ID_ICON_MTMICON,
   R_ID_ICON_MTMORE,
   R_ID_ICON_MTMOTION,
   R_ID_ICON_MTOFF,
   R_ID_ICON_MTON,
   R_ID_ICON_MTON1,
   R_ID_ICON_MTON2,
   R_ID_ICON_MTPARKOFF,
   R_ID_ICON_MTPARKON,
   R_ID_ICON_MTPAUSE,
   R_ID_ICON_MTPAUSE1,
   R_ID_ICON_MTPHOTO,
   R_ID_ICON_MTPHOTOFOCUSRED,
   R_ID_ICON_MTPHOTOFOCUSYELLOW,
   R_ID_ICON_MTPHOTOSCORETIPS,
   R_ID_ICON_MTPLAY,
   R_ID_ICON_MTPLAY1,
   R_ID_ICON_MTPLAY2,
   R_ID_ICON_MTRECORD,
   R_ID_ICON_MTRECORD1,
   R_ID_ICON_MTRIGHT,
   R_ID_ICON_MTSDCNORMAL,
   R_ID_ICON_MTSDCNULL,
   R_ID_ICON_MTU,
   R_ID_ICON_MTU2,
   R_ID_ICON_MTWIFICON,
   R_ID_ICON_MTWIFIDISCON,
   R_ID_ICON_PICIDENTIFYOFF,
   R_ID_ICON_PICIDENTIFYON,
   R_ID_ICON_SEOFF,
   R_ID_ICON_SEON,
   R_ID_ICON_SROFF,
   R_ID_ICON_SRON,
   R_ID_ICON__PHOTOEDIT_FRAME0_SEL,
   R_ID_ICON__PHOTOEDIT_FRAME0_UNSEL,
   R_ID_ICON__PHOTOEDIT_FRAME1_SEL,
   R_ID_ICON__PHOTOEDIT_FRAME1_UNSEL,
   R_ID_ICON__PHOTOEDIT_FRAME2_SEL,
   R_ID_ICON__PHOTOEDIT_FRAME2_UNSEL,
   R_ID_ICON__PHOTOEDIT_FRAME3_SEL,
   R_ID_ICON__PHOTOEDIT_FRAME3_UNSEL,
   R_ID_ICON__PHOTOEDIT_FRAME4_SEL,
   R_ID_ICON__PHOTOEDIT_FRAME4_UNSEL,
   R_ID_ICON_PICIDLANG,
   R_ID_ICON_PICIDPGUP,
   R_ID_ICON_PICIDSOUND,

  R_ICON_MAX
};
extern R_ICON_T User_Icon_Table[];




#endif

