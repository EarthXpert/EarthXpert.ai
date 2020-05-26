/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXUTILS_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXUTILS_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXUTILS_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXUTILS_EXPORTS
#define EXUTILS_EXPORTS
#endif
