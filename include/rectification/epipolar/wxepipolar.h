/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef WXEPIPOLAR_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define WXEPIPOLAR_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define WXEPIPOLAR_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef WXEPIPOLAR_EXPORTS
#define WXEPIPOLAR_EXPORTS
#endif
