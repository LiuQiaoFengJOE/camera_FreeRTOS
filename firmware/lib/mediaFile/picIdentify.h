
#ifndef PICIDENTIFY_H
#define PICIDENTIFY_H
#include "task/app.h"
#include "typedef.h"

#ifndef PIC_IDENTIFY_EN
#define PIC_IDENTIFY_EN   1
#endif

typedef enum{
	PICID_LANGID_ENG = 1,
	PICID_LANGID_SCH,
	PICID_LANGID_MAX
};
typedef enum{
	PICID_ROLE_DOCTOR = 1,
	PICID_ROLE_POET,
	PICID_ROLE_STORYTLLER,
	PICID_ROLE_TEACHER,
	
	PICID_ROLE_NAYSAYER,
	PICID_ROLE_COOK,
    PICID_ROLE_FLATTERER,
	PICID_ROLE_NEWTON,
	PICID_ROLE_DARWIN,
	PICID_ROLE_SOCRATES,
    PICID_ROLE_DETECTOR,
	PICID_ROLE_MAX,
};

typedef enum{
	AIPIC_STYLE_WATERCOLORPINK = 0,
	AIPIC_STYLE_CHINESESTYLE,
	AIPIC_STYLE_CHILDRENDRAW2,
	AIPIC_STYLE_CHILDRENDRAW3,
	AIPIC_STYLE_CHILDRENDRAW4,
	AIPIC_STYLE_CHILDRENDRAW5,
	AIPIC_STYLE_CHILDRENDRAW6,
	AIPIC_STYLE_LINEDRAW,
	AIPIC_STYLE_LINEART,
	AIPIC_STYLE_MAX
};

#define AI_MAX_ITEM 100

typedef struct PicAiRet_S{
	char *cc;
	char *ccCur;

	char *text[AI_MAX_ITEM];
	char *url[AI_MAX_ITEM];// audio or pic url
	int textNum;
	int urlNum;
	int reCode;
}PicAiRet_T;

int picAiInit(void);
int picAiUnInit(void);
int picAiDownloadFile(const char *url, const char *output_filename);

int picIdtfyStart(char *pic);
int picIdtfyUploadAud(char *aud);
void picIdtfySetLangid(u8 langid);
void picIdtfySetUserRole(int userRole);
void *picIdtfyContentGet(void);
void *picIdtfyContentGetCancel(void);

int picIdtfySwStart(char *pic);
void *picIdtfySwContentGet(void);

int ljftoonStart(char *pic);
void *ljftoonGet(void);

int ljfStart(char *pic);
void ljfSetNetType(int netType);
void *ljfGet(void);
#endif


