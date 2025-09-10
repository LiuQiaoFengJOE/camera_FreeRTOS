#ifndef  __RES_INFOR_H
#define  __RES_INFOR_H






#define  R_ID_MASK                 0x00ffffff
#define  R_ID_CHECK                0xff000000
#define  R_ID_TYPE_ICON            0x80000000
#define  R_ID_TYPE_STR             0x81000000
#define  R_ID_TYPE_ASCII           0x82000000
#define  R_ID_TYPE_LAYOUT          0x83000000
#define  R_ID_TYPE_PROGRESSBAR     0x84000000
#define  R_ID_TYPE_VIEW            0x85000000
#define  R_ID_TYPE_DIALOG          0x86000000
#define  R_ID_TYPE_MENU            0x87000000
#define  R_ID_TYPE_WAV_NAME        0x88000000
#define  T_CMD_NULL                  0x0000
#define  T_CMD_ACTIVITY            0x1000
#define  T_CMD_TASK                 0x2000
#define  T_CMD_KEY                  0x3000
#define  T_CMD_MASK                 0xf000

#define  WAV_ID(id)   ((id)|R_ID_TYPE_WAV_NAME)
#define  ID_TYPE(id)  ((id)&R_ID_CHECK)
#define  ID_NUM(id)  ((id)&R_ID_MASK)

typedef struct R_STRING_S
{
	unsigned int r_id;
    char *string;

	unsigned short width;
	unsigned short height;

	unsigned int r_name;
	
}R_STRING_T;
typedef struct R_ICON_S
{
    unsigned long   r_id;

    unsigned short  *r_data;
    
    unsigned long  int timeout;
    
    unsigned short  r_width;
    unsigned short  r_height;

	unsigned long   r_tcolor;

	unsigned int    r_name;

	//char       r_name[32];
    
}R_ICON_T;









#endif

