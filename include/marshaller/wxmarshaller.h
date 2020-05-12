/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#pragma once

#ifdef WXMARSHALLER_API
#if (defined _WIN32 || defined WINCE || defined __CYGWIN__)
#define WXMARSHALLER_EXPORTS __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define WXMARSHALLER_EXPORTS __attribute__((visibility("default")))
#endif
#endif

#ifndef WXMARSHALLER_EXPORTS
#define WXMARSHALLER_EXPORTS
#endif
