/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef WXCORE_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define WXCORE_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define WXCORE_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef WXCORE_EXPORTS
#define WXCORE_EXPORTS
#endif
