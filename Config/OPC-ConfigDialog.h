#pragma once
#ifndef _OPC_CONFIG_DIALOG_H_
#define _OPC_CONFIG_DIALOG_H_

#include <Windows.h>

extern long g_wGlosnosc;
float Windows2Volume( long pdwVolume );
void SetVolumeFromRegistry();
extern LPCWSTR Profile;


int MyDoConfig( const HINSTANCE p_hinst );

#endif // _OPC_CONFIG_DIALOG_H_