#pragma once
#ifndef _MONAD_TTS_H_
#define _MONAD_TTS_H_

void PushTTSDefinition( const std::wstring & pLang, ULONG_PTR pTime, const std::wstring & pVoice );
void SayTime( const SYSTEMTIME & pTime );

#endif // _MONAD_TTS_H_