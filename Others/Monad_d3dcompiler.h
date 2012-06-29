//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
//  File:       D3DCompiler.h
//  Content:    D3D Compilation Types and APIs
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __D3DCOMPILER_H__
#define __D3DCOMPILER_H__

#define D3DCOMPILE_DEBUG                          (1 << 0)
#define D3DCOMPILE_ENABLE_STRICTNESS              (1 << 11)

#ifdef __cplusplus
extern "C" {

//----------------------------------------------------------------------------
// D3DCreateBlob:
// -----------------------
// Create an ID3DBlob instance.
//----------------------------------------------------------------------------

HRESULT WINAPI
D3DCreateBlob(_In_ SIZE_T Size,
              _Out_ ID3DBlob** ppBlob);
}
#endif //__cplusplus
    
#endif // #ifndef __D3DCOMPILER_H__
