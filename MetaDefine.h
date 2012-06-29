#pragma once

#ifndef _MONAD_DEFINE_H_
#define _MONAD_DEFINE_H_

// #define MONAD_DIRECT3D9_MODE /* Forces D3D9c only */
#define _MONAD_SCREENSAVER_ /* Close app. on event */
#define _MONAD_ANIME_ /* Animation (movement of camera */
#define _FACE_BLENDING_ /* Classic vs. changing clockface */
#define GETTIME GetLocalTime( &g_aktual_czas ); /* Algorithm for changin time in app. */
// #define GETTIME { g_aktual_czas.wHour = 3; g_aktual_czas.wMinute = 1; g_aktual_czas.wSecond = 0; g_aktual_czas.wMilliseconds = 0; /* for the icon */ }
#define HARDWARE_EXCEPTIONS

#endif // _MONAD_DEFINE_H_