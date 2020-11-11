/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXEXAI_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXEXAI_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXEXAI_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXEXAI_EXPORTS
#define EXEXAI_EXPORTS
#endif
