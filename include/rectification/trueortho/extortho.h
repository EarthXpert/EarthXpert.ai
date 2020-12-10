/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXTORTHO_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXTORTHO_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXTORTHO_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXTORTHO_EXPORTS
#define EXTORTHO_EXPORTS
#endif
