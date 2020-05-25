/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXSENSORMODEL_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXSENSORMODEL_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXSENSORMODEL_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXSENSORMODEL_EXPORTS
#define EXSENSORMODEL_EXPORTS
#endif
