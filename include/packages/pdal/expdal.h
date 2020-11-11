/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXPDAL_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXPDAL_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXPDAL_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXPDAL_EXPORTS
#define EXPDAL_EXPORTS
#endif
