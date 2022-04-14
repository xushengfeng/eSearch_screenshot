#pragma once
#ifndef OS_H
#define OS_H

/* Python versions under 2.5 don't support this macro, but it's not
 * terribly difficult to replicate: */
#if !defined(IS_MACOS) && defined(__APPLE__) && defined(__MACH__)
#define IS_MACOS
#endif

#if !defined(IS_WINDOWS) && (defined(WIN32) || defined(_WIN32) || \
                             defined(__WIN32__) || defined(__WINDOWS__))
#define IS_WINDOWS
#endif

#if defined(__linux__) || defined(__linux)
#define IS_LINUX
#endif

#endif /* OS_H */
