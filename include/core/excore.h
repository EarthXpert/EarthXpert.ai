/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXCORE_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXCORE_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXCORE_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXCORE_EXPORTS
#define EXCORE_EXPORTS
#endif
