/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXSFM_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXSFM_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXSFM_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXSFM_EXPORTS
#define EXSFM_EXPORTS
#endif
