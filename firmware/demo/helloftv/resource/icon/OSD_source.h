/* !!! Do not manually modify this file in the production stage !!!
 * This file is automatically created by OSDEdit.exe,
 * and used in the production stage.*/

#ifndef __OSD_SOURCE_H
#define __OSD_SOURCE_H

//Custom color index information
#define       PALETTE_R_COLOR_ERROR                   	0xff
#define       PALETTE_R_COLOR_RED                     	0xfe
#define       PALETTE_R_COLOR_GREEN                   	0xfd
#define       PALETTE_R_COLOR_BLUE                    	0xfc
#define       PALETTE_R_COLOR_WHITE                   	0xfb
#define       PALETTE_R_COLOR_BLACK                   	0xfa
#define       PALETTE_R_COLOR_TRANSFER                	0xf9
#define       PALETTE_R_COLOR_TBLACK                  	0xf8
#define       PALETTE_R_COLOR_YELLOW                  	0xf7
#define       PALETTE_R_COLOR_GARY1                   	0xf6
#define       PALETTE_R_COLOR_BLUE1                   	0xf5
#define       PALETTE_R_COLOR_DBLUE                   	0xf4
#define       PALETTE_R_COLOR_BLUE2                   	0xf3
#define       PALETTE_R_COLOR_GARY2                   	0xf2
#define       PALETTE_R_COLOR_GARY3                   	0xf1

//OSD Icon index information
#define       OSD_ICONBARNSEL                     	0
#define       OSD_ICONBARSEL                      	1
#define       OSD_ICONLEDOFF                      	2
#define       OSD_ICONLEDON                       	3
#define       OSD_ICONMENUAUDIO                   	4
#define       OSD_ICONMENUBACK                    	5
#define       OSD_ICONMENUBATTERY                 	6
#define       OSD_ICONMENUCLOCK                   	7
#define       OSD_ICONMENUDELALL                  	8
#define       OSD_ICONMENUDELONE                  	9
#define       OSD_ICONMENUEV                      	10
#define       OSD_ICONMENUFORMAT                  	11
#define       OSD_ICONMENUGSENSOR                 	12
#define       OSD_ICONMENUHZ                      	13
#define       OSD_ICONMENUIMAGEQ                  	14
#define       OSD_ICONMENULANGUAGE                	15
#define       OSD_ICONMENULIGHTNORMAL             	16
#define       OSD_ICONMENULOCK                    	17
#define       OSD_ICONMENULOOPRECORD              	18
#define       OSD_ICONMENUMONITOR                 	19
#define       OSD_ICONMENUMOON                    	20
#define       OSD_ICONMENUMOTION                  	21
#define       OSD_ICONMENUMUSIC                   	22
#define       OSD_ICONMENUMUSICPAUSE              	23
#define       OSD_ICONMENUMUSICPLAY               	24
#define       OSD_ICONMENUPOWEROFF                	25
#define       OSD_ICONMENURESET                   	26
#define       OSD_ICONMENURESOLUTION              	27
#define       OSD_ICONMENUSCRENNOFF               	28
#define       OSD_ICONMENUSTRAMP                  	29
#define       OSD_ICONMENUTV                      	30
#define       OSD_ICONMENUUNLOCK                  	31
#define       OSD_ICONMENUVERSION                 	32
#define       OSD_ICONMENUVOLUME                  	33
#define       OSD_ICONMTBACK                      	34
#define       OSD_ICONMTBACK1                     	35
#define       OSD_ICONMTBATTERY0                  	36
#define       OSD_ICONMTBATTERY1                  	37
#define       OSD_ICONMTBATTERY2                  	38
#define       OSD_ICONMTBATTERY3                  	39
#define       OSD_ICONMTBATTERY4                  	40
#define       OSD_ICONMTBATTERY5                  	41
#define       OSD_ICONMTFORWARD                   	42
#define       OSD_ICONMTFORWARD1                  	43
#define       OSD_ICONMTIROFF                     	44
#define       OSD_ICONMTIRON                      	45
#define       OSD_ICONMTLEFT                      	46
#define       OSD_ICONMTLOCK                      	47
#define       OSD_ICONMTMENU                      	48
#define       OSD_ICONMTMENU1                     	49
#define       OSD_ICONMTMICOFF                    	50
#define       OSD_ICONMTMICON                     	51
#define       OSD_ICONMTMORE                      	52
#define       OSD_ICONMTMOTION                    	53
#define       OSD_ICONMTOFF                       	54
#define       OSD_ICONMTON                        	55
#define       OSD_ICONMTON1                       	56
#define       OSD_ICONMTON2                       	57
#define       OSD_ICONMTPARKOFF                   	58
#define       OSD_ICONMTPARKON                    	59
#define       OSD_ICONMTPAUSE                     	60
#define       OSD_ICONMTPAUSE1                    	61
#define       OSD_ICONMTPHOTO                     	62
#define       OSD_ICONMTPHOTOFOCUSRED             	63
#define       OSD_ICONMTPHOTOFOCUSYELLOW          	64
#define       OSD_ICONMTPHOTOSCORETIPS            	65
#define       OSD_ICONMTPLAY                      	66
#define       OSD_ICONMTPLAY1                     	67
#define       OSD_ICONMTPLAY2                     	68
#define       OSD_ICONMTRECORD                    	69
#define       OSD_ICONMTRECORD1                   	70
#define       OSD_ICONMTRIGHT                     	71
#define       OSD_ICONMTSDCNORMAL                 	72
#define       OSD_ICONMTSDCNULL                   	73
#define       OSD_ICONMTU                         	74
#define       OSD_ICONMTU2                        	75
#define       OSD_ICONMTWIFICON                   	76
#define       OSD_ICONMTWIFIDISCON                	77
#define       OSD_ICONPICIDENTIFYOFF              	78
#define       OSD_ICONPICIDENTIFYON               	79
#define       OSD_ICONSEOFF                       	80
#define       OSD_ICONSEON                        	81
#define       OSD_ICONSROFF                       	82
#define       OSD_ICONSRON                        	83
#define       OSD_ICON_PHOTOEDIT_FRAME0_SEL       	84
#define       OSD_ICON_PHOTOEDIT_FRAME0_UNSEL     	85
#define       OSD_ICON_PHOTOEDIT_FRAME1_SEL       	86
#define       OSD_ICON_PHOTOEDIT_FRAME1_UNSEL     	87
#define       OSD_ICON_PHOTOEDIT_FRAME2_SEL       	88
#define       OSD_ICON_PHOTOEDIT_FRAME2_UNSEL     	89
#define       OSD_ICON_PHOTOEDIT_FRAME3_SEL       	90
#define       OSD_ICON_PHOTOEDIT_FRAME3_UNSEL     	91
#define       OSD_ICON_PHOTOEDIT_FRAME4_SEL       	92
#define       OSD_ICON_PHOTOEDIT_FRAME4_UNSEL     	93
#define       OSD_PICIDLANG                       	94
#define       OSD_PICIDPGUP                       	95
#define       OSD_PICIDSOUND                      	96


#endif
