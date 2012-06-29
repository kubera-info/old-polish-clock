#include "DXUT.h"
#include "SetupMonadWave.h"
#include <mmsystem.h>

volatile bool g_bMute = false;

void MonadPlayWave( const INT pWave )
{
	if( !g_bMute && !g_bSilent )
		PlaySound( MAKEINTRESOURCE( pWave ), GetModuleHandle( NULL ), SND_RESOURCE | SND_ASYNC );
}