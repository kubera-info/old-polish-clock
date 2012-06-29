#include "DXUT.h"
#include "Light.h"
#include "MonadMath.h"

XMVECTOR g_lightPosition;

const float g_xDir = (float) ( MonadMath::GetRandomULongPtr( 0, 1 ) << 1 ) - 1;

