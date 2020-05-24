/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXEPIPOLARMODEL_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXEPIPOLARMODEL_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXEPIPOLARMODEL_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXEPIPOLARMODEL_EXPORTS
#define EXEPIPOLARMODEL_EXPORTS
#endif
