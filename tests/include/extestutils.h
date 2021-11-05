/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXTESTUTILS_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXTESTUTILS_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXTESTUTILS_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXTESTUTILS_EXPORTS
#define EXTESTUTILS_EXPORTS
#endif
