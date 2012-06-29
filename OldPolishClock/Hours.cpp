#include "DXUT.h"
#include "Hours.h"
#include "MonadMath.h"

static LONG_PTR RandomHour = 12;

void IntializeToMidday()
{
	RandomHour = 12;
}

ULONG_PTR GetNewRandomHour12()
{
	return MonadMath::GetRandomULongPtr( 1, 12 );
}

