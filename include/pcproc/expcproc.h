/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXPCPROC_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXPCPROC_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXPCPROC_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXPCPROC_EXPORTS
#define EXPCPROC_EXPORTS
#endif
