/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXORTHO_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXORTHO_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXORTHO_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXORTHO_EXPORTS
#define EXORTHO_EXPORTS
#endif
