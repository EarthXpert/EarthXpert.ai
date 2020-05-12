/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef WX3RDPARTY_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define WX3RDPARTY_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define WX3RDPARTY_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef WX3RDPARTY_EXPORTS
#define WX3RDPARTY_EXPORTS
#endif
