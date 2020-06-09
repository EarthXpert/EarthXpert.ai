/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EX3RDPARTY_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EX3RDPARTY_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EX3RDPARTY_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EX3RDPARTY_EXPORTS
#define EX3RDPARTY_EXPORTS
#endif
