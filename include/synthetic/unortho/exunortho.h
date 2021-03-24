/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXUNORTHO_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXUNORTHO_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXUNORTHO_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXUNORTHO_EXPORTS
#define EXUNORTHO_EXPORTS
#endif
