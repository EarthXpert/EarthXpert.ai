/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXGDAL_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXGDAL_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXGDAL_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXGDAL_EXPORTS
#define EXGDAL_EXPORTS
#endif
