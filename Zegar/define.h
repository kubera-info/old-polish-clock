#ifndef _DEFINE_H_
#define _DEFINE_H_

// Ró¿ne...
//#define OBROT
//#define LUMINACJA

// Ogólne
// Wielkoœæ œwiata (85% rozmiaru ekranu - maksimum)
#define SWIAT			(8500)
// Pozycje obiektów na osi Z
#define WYCOFANIE		(.20f)
#define TARCZA_Z		(WYCOFANIE * 2.00f)
#define GODZ_Z			(WYCOFANIE * 1.20f)
#define MIN_Z			(WYCOFANIE * 1.40f)
#define STOZEK_Z        (WYCOFANIE * 1.50f)

// Tarcza
#define IL (128) // Sta³a il. trójk¹tów

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
	// Krzyz
#define IL_W_KRZYZ (12)
#define START ((float) 5.2 / 7)
	// Kolo
#define IL_KOLO (64)

// Muterka
#define WSP_KATA (0.22f)
#define WSP_KATA_SRUBY (0.55f)
#define WSPOLCZ_PRZEKROJ (2.7f)

// Ogólne
#define M_PI_MUL_2 ((float) (M_PI * 2))
#define M_PI_DIV_2 ((float) M_PI_2)
#define M_PI_DIV_4 ((float) (M_PI / 4))
#define ASERT(x) {if(FAILED(x)) throw(CBladCichy(#x, __LINE__, __FILE__));}
#define ASERTNZ(x) {if(!x) throw(CBladCichy(#x, __LINE__, __FILE__));}
#define FUNC_TRACE (lstrcpy(g_blad, _T(__FUNCTION__)))

// Rejestr
#define REGODNOSNIK _T("SOFTWARE\\kubera.info\\Screen Saver\\Zegar\\Ver 1")
#define REGGLOS _T("glosnosc")
#define MINGLOS (0) // Cisza
#define MAXGLOS (110) // 110 decybeli
#define REGWIELKOSC _T("wielkosc")
#define MINWIELKOSC (2500) // 25%
#define MAXWIELKOSC (8500) // 85%
#define REGJAKOSC _T("jakosc")
#define MINJAKOSC ( 250) // Ma³o trójk¹tów, dla starych modeli kart
#define MAXJAKOSC (1750) // Wiele trójk¹tów dla akceleratorów
#define PRZEL(x) ((WORD) ((float) (x) * g_wJakoscGr / 1000))
#define REGSEKUNDA _T("sekunda") // Czy pokazaæ wskazówkê sekundow¹?
#define REGBLAD _T("blad")

#endif