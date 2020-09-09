/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXGUI_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXGUI_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXGUI_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXGUI_EXPORTS
#define EXGUI_EXPORTS
#endif
