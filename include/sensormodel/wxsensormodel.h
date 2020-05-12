/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef WXSENSORMODEL_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define WXSENSORMODEL_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define WXSENSORMODEL_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef WXSENSORMODEL_EXPORTS
#define WXSENSORMODEL_EXPORTS
#endif
