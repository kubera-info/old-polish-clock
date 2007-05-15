// #########################################
// define.h - pakiet ogólny, makrodefinicje.
// #########################################

#ifndef _DEFINE_H_
#define _DEFINE_H_

#define I18N_TYTUL (0)
#define I18N_KONFIG_TYTUL (1)

#ifdef _DEBUG

// Makrodefinicjia uruchamia rysowanie figur bez u¿ycia mesh-y.
// Opcja umo¿liwia wygenerowanie mesh-y w trybie Debug.
#define CPP_RAW_PRIMITIVE

// Sta³e oœwietlenie (w wygaszaczu ulega sinusoidalnym zmianom jego natê¿enie).
// Opcja przydatna przy opracowaniu Zegara.
#define LUMINACJA

#ifdef CPP_RAW_PRIMITIVE
// #define CPP_RAW_DUMP

// Makro aktywuje szyfrowanie plików z katalogu "Zasoby",
// które zostan¹ umieszczone w folderze "Zaszyfrowane".
//#define ENCRYPT_PLIKI

#endif

#endif
#if defined(CPP_RAW_PRIMITIVE)

// Ogólne
// Pozycje obiektów na osi Z.
#define SWIAT			(8500)
#define PRZEL(x) ((DWORD) ((float) (x) * g_wJakoscGr / 1000))
#define OZDOBA_ASPECT_RATIO (1693.0f / 2032.0f)
#define WYCOFANIE		(.20f)
#define TARCZA_Z		(WYCOFANIE * 2.00f)
#define GODZ_Z			(WYCOFANIE * 1.20f)
#define MIN_Z			(WYCOFANIE * 1.40f)
#define STOZEK_Z        (WYCOFANIE * 1.50f)
#define SZKLO           (0.2f)

// Tarcza
#define IL (128) // Sta³a il. trójk¹tów.

// Wskazowki
// Koniczynka
#define IL_W_KONICZ (32)
#define PROM_X_MALY (0.53f)
#define PROM_Y_MALY (0.74f)
#define PROM_X_DUZY (0.94f)
#define PROM_Y_DUZY (1.00f)
#define WSP_MALA_KONICZ (1.32) // £uk elipsy (WSP_KONICZ * 2PI)
#define WSP_DUZA_KONICZ (1.11) // £uk elipsy (WSP_KONICZ * 2PI)
#define KAT_KONICZ(x) ((float) (M_PI * x))
#define START_KONICZ(x) ((float) (M_PI * (1.5 - 0.5 * (x))))
// Krzy¿
#define IL_W_KRZYZ (12)
#define START ((float) 5.2 / 7)
// Ko³o
#define IL_KOLO (64)

// Nakrêtka
#define WSP_KATA (0.22f)
#define WSP_KATA_SRUBY (0.55f)
#define WSPOLCZ_PRZEKROJ (2.7f)

#endif

// Ogólne
//#define WIDEN2(x) L ## x
//#define WIDEN(x) WIDEN2(x)
#define FUNC_TRACE 
//(wcscpy_s(g_blad, sizeof(g_blad) / sizeof(WCHAR), WIDEN(__FUNCTION__)))

// Rejestr
#define REGODNOSNIK L"SOFTWARE\\www.kubera.info\\Screen Saver\\Clock3D\\Config-2.0"
#define REGGLOS L"SoundVolume"
#define MINGLOS (0) // -100 decybeli, cisza
#define MAXGLOS (1000) // 0 decybeli, maksimum
#define REGSEKUNDA L"SecondHand" // Czy pokazaæ wskazówkê sekundow¹?
#define REGOPADANIE L"SoundDecay" // Czy dŸwiêk zagara ma zanikaæ, a¿eby nie przeszkadzaæ?
#define ZFAR (100.0f)

#define SAVERWNDCLASS L"D3DClockWindowClass"
#endif