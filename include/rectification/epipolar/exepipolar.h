/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXEPIPOLAR_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXEPIPOLAR_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXEPIPOLAR_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXEPIPOLAR_EXPORTS
#define EXEPIPOLAR_EXPORTS
#endif
