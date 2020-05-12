/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef WXPROJECT_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define WXPROJECT_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define WXPROJECT_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef WXPROJECT_EXPORTS
#define WXPROJECT_EXPORTS
#endif
