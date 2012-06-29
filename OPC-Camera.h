#pragma once
#ifndef _OPC_CAMERA_H_
#define _OPC_CAMERA_H_

#include "MonadEvents.h"

struct COPCCamera : public MonadEvent::COnEvent
{
	COPCCamera();
	virtual void OnFrameMoveStub();
} mCamera;

#endif // _OPC_CAMERA_H_