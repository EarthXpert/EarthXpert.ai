/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXIO_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXIO_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXIO_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXIO_EXPORTS
#define EXIO_EXPORTS
#endif
