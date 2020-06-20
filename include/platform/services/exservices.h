/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXSERVICES_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXERVICES_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXERVICES_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXERVICES_EXPORTS
#define EXERVICES_EXPORTS
#endif
