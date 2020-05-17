/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef WXSFM_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define WXSFM_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define WXSFM_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef WXSFM_EXPORTS
#define WXSFM_EXPORTS
#endif
