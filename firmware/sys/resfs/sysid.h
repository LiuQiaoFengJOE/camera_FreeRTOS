/**@file sysid.h
 * @brief System pre-defined ID
 * @note
 * System-Engineers        can not #include sysid.h directly, uses sys-id.h instead.
 * Application-Engineers   can not #include sysid.h directly, uses sys-id.h instead.
 * Application-Engineers                                 can not modify sysid.h!
 * Configuration-Engineers can     #include sysid.h, but can not modify sysid.h!
 */

#ifndef __RESFS_SYSID_H__
#define __RESFS_SYSID_H__

SYS_RESID()
DEF_RESID(RESID_NONE)
DEF_RESID(RESID_FONT)
DEF_RESID(RESID_LANGSTR)
END_RESID(RESID_SYSMAX, 2)

SYS_FONTID()
SET_FONTID(FONTID_MASK, 0x0F)
SET_FONTID(FONTID_PANEL, 0x10)
SET_FONTID(FONTID_OUTLINE, 0x20)
SET_FONTID(FONTID_LANGSTR, 0)
DEF_FONTID(FONTID_DEFAULT)
END_FONTID(FONTID_SYSMAX, 0)

SYS_COLORID()
SET_COLORID(COLORID_BLACK, 0xFA)
END_COLORID(COLORID_DUMMY, 0)

SYS_CTRLID()
DEF_CTRLID(CTRLID_GROUP)
DEF_CTRLID(CTRLID_TEXT)
DEF_CTRLID(CTRLID_LANGSTR)
END_CTRLID(CTRLID_SYSMAX, 0)

#endif /* __RESFS_SYSID_H__ */
