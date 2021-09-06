/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXGIS_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXGIS_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXGIS_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXGIS_EXPORTS
#define EXGIS_EXPORTS
#endif
