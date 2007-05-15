
#include "exdll.h"
#define NSIS_API extern "C" VOID
#define NSIS_ARGS HWND hwndParent, int string_size, char *variables, stack_t **stacktop

// Poni¿sze funkcje s¹ zgodne ze standardem osadzonych wtyczek instalatora NSIS.
// Komentarze dotycz¹ce dzia³ania funkcji zosta³y umieszczone w pliku KuberaInfoPlugin.cpp.
NSIS_API UniqueInstallation ( NSIS_ARGS );
NSIS_API VerifyBinary ( NSIS_ARGS );
NSIS_API IsWindowsServer ( NSIS_ARGS );
NSIS_API Unload ( NSIS_ARGS );
