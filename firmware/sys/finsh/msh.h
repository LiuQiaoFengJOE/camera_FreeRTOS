/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-03-30     Bernard      the first verion for FinSH
 * 2024-01-04     Haifeng Li   adapt to freeRTOS
 */

#ifndef __M_SHELL__
#define __M_SHELL__

int msh_exec(char *cmd, rt_size_t length);
void msh_auto_complete(char *prefix);

int msh_exec_module(const char *cmd_line, int size);
int msh_exec_script(const char *cmd_line, int size);

#ifdef FINSH_USING_OPTION_COMPLETION
void msh_opt_auto_complete(char *prefix);
#endif /* FINSH_USING_OPTION_COMPLETION */
#endif
