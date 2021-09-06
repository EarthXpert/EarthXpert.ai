/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXAI_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXAI_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXAI_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXAI_EXPORTS
#define EXAI_EXPORTS
#endif
