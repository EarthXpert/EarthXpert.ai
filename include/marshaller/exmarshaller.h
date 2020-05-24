/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef EXMARSHALLER_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define EXMARSHALLER_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define EXMARSHALLER_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef EXMARSHALLER_EXPORTS
#define EXMARSHALLER_EXPORTS
#endif
