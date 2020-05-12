/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef WXIO_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define WXIO_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define WXIO_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef WXIO_EXPORTS
#define WXIO_EXPORTS
#endif
