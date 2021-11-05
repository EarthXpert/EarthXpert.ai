/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXPLATFORM_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXPLATFORM_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXPLATFORM_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXPLATFORM_EXPORTS
#define EXPLATFORM_EXPORTS
#endif
