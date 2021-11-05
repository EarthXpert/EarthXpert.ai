/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXBBA_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXBBA_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXBBA_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXBBA_EXPORTS
#define EXBBA_EXPORTS
#endif
