/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXPROJECT_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXPROJECT_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXPROJECT_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXPROJECT_EXPORTS
#define EXPROJECT_EXPORTS
#endif
