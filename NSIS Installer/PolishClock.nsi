;NSIS Modern User Interface
;Multilingual Script (Polish, Russian, English)

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General

  SetCompressor /SOLID /FINAL lzma
  ; Wtyczka sіuїy dodatkowej weryfikacji systemu podczas pracy instalatora
  ; Musi byж skompilowana, aby instalator zostaі skonsolidowany
  !addplugindir "../Kubera.Info.Plugin/Release/"

  ; Informacja o programie instalacyjnym
  BrandingText "NSIS Installer"

  ; W tym miejscu jest zapisywana konfiguracja zegara
  ; Parametry rejestru
  !define INTERNAL_NAME "Old Polish Clock"
  ; GUID instalatora oraz wygaszacza
  !define GUID "{BC1C9890-958A-4D9B-A652-71BEA29F338C}"
  !define REG_ENTRY_DOMAIN      "SOFTWARE\www.kubera.info"
  !define REG_ENTRY_SCREENSAVER "SOFTWARE\www.kubera.info\Screen Saver"
  !define REG_ENTRY_NAME        "SOFTWARE\www.kubera.info\Screen Saver\Clock3D"
  !define REG_ENTRY_CONF        "SOFTWARE\www.kubera.info\Screen Saver\Clock3D\Config-2.0"

  ; W tym miejscu moїna oczekiwaж ustawieс ekranu
  !define REG_ENTRY_SCRNSAVE "Control Panel\Desktop"

  ; Ustawienie pracy instalatora
  !define REG_ENTRY_UNINSTALL "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${GUID}"
  !define CLOCK_FILE_EXE "$SYSDIR\ZegarXIX.scr"
  !define /date NOW "%Y%m%d"
  !define CLOCK_SOURCE_EXE "..\Zegar\Release\Zegar.scr"
  OutFile "..\OldPolishClockSetup2.0.exe"

  ; Folder, do ktуrego trafi deinstalator wygaszacza
  !define INFO_INSTALL_DIR "$PROGRAMFILES\www.kubera.info"
  InstallDir "${INFO_INSTALL_DIR}\${INTERNAL_NAME}"

  ; Nazwa aplikacji usuwaj№cej
  !define UNINSTALL_EXE "$INSTDIR\Uninstall.exe"

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU"
  !define MUI_LANGDLL_REGISTRY_KEY "${REG_ENTRY_CONF}"
  !define MUI_LANGDLL_REGISTRY_VALUENAME "InstallerLanguage"


;--------------------------------
; Banners
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "..\Zasoby\un-banner-zegar.bmp"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "..\Zasoby\banner-zegar.bmp"

;--------------------------------
;Pages
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE $(MUILicense)
#  !insertmacro MUI_PAGE_COMPONENTS
#  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English" # first language is the default language
  !insertmacro MUI_LANGUAGE "Russian"
  !insertmacro MUI_LANGUAGE "Polish"

;--------------------------------
;License Language String

  LicenseLangString MUILicense ${LANG_ENGLISH} "..\Licenses\lic-en.rtf"
  LicenseLangString MUILicense ${LANG_POLISH} "..\Licenses\lic-pl.rtf"
  LicenseLangString MUILicense ${LANG_RUSSIAN} "..\Licenses\lic-ru.rtf"

  LangString InstName ${LANG_ENGLISH} "'Clock' Screen Saver"
  LangString InstName ${LANG_POLISH} "Wygaszacz 'Zegar'"
  LangString InstName ${LANG_RUSSIAN} "Заставка 'Часы'"

  LangString AreYouSure ${LANG_ENGLISH} "Do you want to overwrite previous installation?"
  LangString AreYouSure ${LANG_POLISH} "Czy chcesz zast№piж poprzedni№ instalacjк?"
  LangString AreYouSure ${LANG_RUSSIAN} "Хочеш ли помэнять прэдыдуцую установку?"

  LangString TooOld ${LANG_ENGLISH} "Deployed version is more recent than this installer."
  LangString TooOld ${LANG_POLISH} "Istniej№ca wersja jest nowsza, niї aktualnie instalowana."
  LangString TooOld ${LANG_RUSSIAN} "Настояцая версия является новшеством по сровнению с актуально установливаемой."

  LangString WeAreSorry ${LANG_ENGLISH} "We are sorry."
  LangString WeAreSorry ${LANG_POLISH} "Przepraszamy."
  LangString WeAreSorry ${LANG_RUSSIAN} "Извините."

  LangString OldWindows ${LANG_ENGLISH} "This version of Windows is not supported."
  LangString OldWindows ${LANG_POLISH} "Ta wersja Widnows nie jest wspierana."
  LangString OldWindows ${LANG_RUSSIAN} "Эта версия Widnows не обслужевается."

  LangString OldDirectX ${LANG_ENGLISH} "DirectX 2006 October or newer is required."
  LangString OldDirectX ${LANG_POLISH} "Wymagany jest DirectX 2006 October lub nowszy."
  LangString OldDirectX ${LANG_RUSSIAN} "Требуется версия DirectX 2006 October или бoлее новая."

  LangString UnknownVersion ${LANG_ENGLISH} "In your system is installed unknown version of the Clock."
  LangString UnknownVersion ${LANG_POLISH} "W systemie jest zainstalowana nieznana wersja Zegara."
  LangString UnknownVersion ${LANG_RUSSIAN} "В системе установленая неизвестная версия Чясoв."

  LangString InstallationCanceled ${LANG_ENGLISH} "Installation process will be stopped."
  LangString InstallationCanceled ${LANG_POLISH} "Instalacja zostanie przerwana."
  LangString InstallationCanceled ${LANG_RUSSIAN} "Установка остановлена."

  LangString Already ${LANG_ENGLISH} "The installer is already running."
  LangString Already ${LANG_POLISH} "Inny program instalacyjny jest uruchomiony."
  LangString Already ${LANG_RUSSIAN} "Открыто другой установочный процэсс 'Чясов'."

  LangString IsWindowsServer ${LANG_ENGLISH} "Do you really want to install Screen Saver on this server?"
  LangString IsWindowsServer ${LANG_POLISH} "Czy napewno chcesz zainstalowaж wygaszacz w tym serwerze?"
  LangString IsWindowsServer ${LANG_RUSSIAN} "Хочешь ли наверняка установить заставку в этом сервере?"

;--------------------------------
; Instalacja w њrodowisku Windows Vista i pуџniejszym
  RequestExecutionLevel admin
  
; Nazwa projektu
  Name $(InstName)
  
; Zasуb wersji skonsolidowany z plikiem wykonawczym
  VIProductVersion "2.0.0.3"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "Clock (XIX/XX century)"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "GNU Screen Saver"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "kubera.info"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalTrademarks" "Wіodzimierz O. Kubera"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Copyright © 2003-2007 kubera.info, wszelkie prawa zastrzeїone"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "Old Polish Clock Installer"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "2.0.0.3, 2007-03-11; Windows XP and newer with DirectX (2007 February)"


;--------------------------------
;Reserve Files

  ;These files should be inserted before other files in the data block
  ;Keep these lines before any File command
  ;Only for solid compression (by default, solid compression is enabled for BZIP2 and LZMA)

  !insertmacro MUI_RESERVEFILE_LANGDLL

!macro CompareVersions LocalModule Module
; Porуwnanie wersji Zegar w instalatorze z wersj№ w Windows
   GetDllVersionLocal ${LocalModule} $R0 $R1
   GetDllVersion ${Module} $R2 $R3
   IntCmpU $R0 $R2 ClockEq ClockOld ClockNew

ClockEq:
   IntCmpU $R1 $R3 ClockEq2 ClockOld ClockNew
ClockEq2:
   StrCpy $R9 "same"
   Goto ClockExit
ClockOld:
   StrCpy $R9 "old"
   Goto ClockExit
ClockNew:
   StrCpy $R9 "new"
ClockExit:
!macroend


;--------------------------------
;Installer Sections

Section
  ; Minimum Windows XP...
  ReadRegStr $R0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
  IfErrors OldWindowsException
   StrCmp $R0 '4.0' OldWindowsException
   StrCmp $R0 '5.0' OldWindowsException
   GoTo WindowsVersionOk

  OldWindowsException:
   MessageBox MB_OK|MB_ICONEXCLAMATION "$(WeAreSorry) $(OldWindows) $(InstallationCanceled)"
   DetailPrint "$(OldWindows) $(InstallationCanceled)"
   Abort

  ; Odpowiednia (nowa) wersja DirectX...
  WindowsVersionOk:
  IfFileExists "$SYSDIR\d3dx9_32.dll" DirectXVersionOk 0
   MessageBox MB_OK|MB_ICONEXCLAMATION "$(WeAreSorry) $(OldDirectX) $(InstallationCanceled)"
   DetailPrint "$(OldDirectX) $(InstallationCanceled)"
   Abort

  ; Unikatowy proces instalacyjny zegara?
  DirectXVersionOk:
   KuberaInfoPlugin::UniqueInstallation /NOUNLOAD
   Pop $1
   StrCmp $1 "unique" UniqueOk
   MessageBox MB_OK|MB_ICONEXCLAMATION "$(Already) $(InstallationCanceled)"
   DetailPrint "$(Already) $(InstallationCanceled)"
   Abort

  ; Ostrzeїenie przed instalacj№ zegara w serwerze lub kontrolerze domeny.
  ; (w niektуrych systemach zegar moїe zmniejszyж wydajnoњж)
  UniqueOk:
   KuberaInfoPlugin::IsWindowsServer /NOUNLOAD
   Pop $1
   StrCmp "server" $1 0 ServerOk
   MessageBox MB_OK|MB_ICONEXCLAMATION $(IsWindowsServer) IDYES ServerOk IDNO FullAbort

  ; Sprawdџ czy w systemie nie byі uprzednio zainstalowany plik o takiej samej nazwie...
  ; Czy ten plik jest Starym Zegarem?
  ServerOk:
   KuberaInfoPlugin::VerifyBinary /NOUNLOAD ${CLOCK_FILE_EXE} ${GUID}
   Pop $1
   StrCmp $1 "friend" BinaryOk
   MessageBox MB_OK|MB_ICONEXCLAMATION "$(UnknownVersion) $(InstallationCanceled)"
   DetailPrint "$(UnknownVersion) $(InstallationCanceled)"
   Abort

  ; Upgrade, stop?
  BinaryOk:
   !insertmacro CompareVersions ${CLOCK_SOURCE_EXE} ${CLOCK_FILE_EXE}
    StrCmp $R9 "old" AbortClockInstall
    StrCmp $R9 "same" AskInstallingMe
    StrCmp $R9 "new" NewClockInstall
   Abort

  ; Nie instaluj starszej wersji, niї bieї№ca
AbortClockInstall:
   MessageBox MB_OK|MB_ICONEXCLAMATION "$(WeAreSorry) $(TooOld) $(InstallationCanceled)"
   DetailPrint "$(TooOld) $(InstallationCanceled)"
FullAbort:
   Abort

  ; Ta sama wersja, czy zmiana jest niezbкdna?
AskInstallingMe:
   MessageBox MB_YESNO|MB_ICONQUESTION $(AreYouSure) IDYES NewClockInstall IDNO FullAbort

NewClockInstall:
  ;Create uninstaller
  File /oname=${CLOCK_FILE_EXE} ${CLOCK_SOURCE_EXE}
  WriteRegStr HKLM ${REG_ENTRY_UNINSTALL} "DisplayVersion" "2.0"
  WriteRegStr HKLM ${REG_ENTRY_UNINSTALL} "Publisher" "Wlodzimierz O. Kubera"
  WriteRegDWORD HKLM ${REG_ENTRY_UNINSTALL} "NoModify" 1
  WriteRegDWORD HKLM ${REG_ENTRY_UNINSTALL} "NoRepair" 1
  WriteRegStr HKLM ${REG_ENTRY_UNINSTALL} "HelpLink" "http://screensaver.kubera.info"
  WriteRegStr HKLM ${REG_ENTRY_UNINSTALL} "URLInfoAbout" "http://screensaver.kubera.info"
  WriteRegStr HKLM ${REG_ENTRY_UNINSTALL} "InstallDate" ${NOW}
  WriteRegStr HKLM ${REG_ENTRY_UNINSTALL} "UninstallString" ${UNINSTALL_EXE}
  WriteRegStr HKLM ${REG_ENTRY_UNINSTALL} "Contact" "Wlodzimierz O. Kubera <wlodzimierz@kubera.info>"
  WriteRegStr HKLM ${REG_ENTRY_UNINSTALL} "DisplayIcon" ${CLOCK_FILE_EXE}
  WriteRegStr HKLM ${REG_ENTRY_UNINSTALL} "DisplayName" $(InstName)
  WriteRegStr HKLM ${REG_ENTRY_UNINSTALL} "ProductGuid" ${GUID}
  IfFileExists $INSTDIR DelUninst 0
   CreateDirectory $INSTDIR
DelUninst:
  WriteUninstaller ${UNINSTALL_EXE}
  ; Wybierz zegar jako wygaszacz
  WriteRegStr HKCU "${REG_ENTRY_SCRNSAVE}" "SCRNSAVE.EXE" ${CLOCK_FILE_EXE}
  ; Otwуrz aplet wygaszacza ekranu
  Exec 'Rundll32.exe shell32.dll,Control_RunDLL Desk.cpl,@0, 1'
SectionEnd



;--------------------------------
;Installer Functions

Function .onInit

  !insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd


;--------------------------------
;Uninstaller Section

Section "Uninstall"
  KuberaInfoPlugin::UniqueInstallation /NOUNLOAD
  Pop $1
  StrCmp $1 "unique" UninstallUniqueOk
  MessageBox MB_OK|MB_ICONEXCLAMATION "$(Already) $(InstallationCanceled)"
  DetailPrint "$(Already) $(InstallationCanceled)"
  Abort

UninstallUniqueOk:
  Delete ${CLOCK_FILE_EXE}
  Delete ${UNINSTALL_EXE}

; Usun foldery tak, їeby nie skasowaж zmian dokonanych przez uїytkownika
  RMDir "$INSTDIR"
  RMDir "${INFO_INSTALL_DIR}"

; Wyrejestruj wpis usuwania Zegara
  DeleteRegKey HKLM ${REG_ENTRY_UNINSTALL}

; Usuс wpisy Zegara z rejestru dla kaїdego uїytkownika z osobna
  StrCpy $0 0
  loop:
    EnumRegKey $1 HKU "" $0
    StrCmp $1 "" done
    IntOp $0 $0 + 1
    DeleteRegKey HKU "$1\${REG_ENTRY_NAME}"
    DeleteRegKey /ifempty HKU "$1\${REG_ENTRY_SCREENSAVER}"
    DeleteRegKey /ifempty HKU "$1\${REG_ENTRY_DOMAIN}"
    Goto loop
  done:

SectionEnd

Function .onGUIEnd
  KuberaInfoPlugin::Unload
FunctionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE

FunctionEnd

Function un.onGUIEnd
  KuberaInfoPlugin::Unload
FunctionEnd