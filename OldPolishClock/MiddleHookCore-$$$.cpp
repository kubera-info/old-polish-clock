#include "DXUT.h"
#include "MiddleHookCore-$$$.h"

void MakeHook( CMeshBuilderPlain & mh )
{
#ifdef DEBUG
	VERTEXPLAIN t1, t2, t3, t4;

t1.Position.x = 2;t1.Position.y = 0;t2.Position.x = 20;t2.Position.y = 0;t3.Position.x = 20;t3.Position.y = 1;t4.Position.x = 2;t4.Position.y = 1;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 1;t2.Position.x = 25;t2.Position.y = 1;t3.Position.x = 25;t3.Position.y = 2;t4.Position.x = 1;t4.Position.y = 2;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 2;t2.Position.x = 26;t2.Position.y = 2;t3.Position.x = 26;t3.Position.y = 3;t4.Position.x = 0;t4.Position.y = 3;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 3;t2.Position.x = 28;t2.Position.y = 3;t3.Position.x = 28;t3.Position.y = 4;t4.Position.x = 0;t4.Position.y = 4;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 4;t2.Position.x = 29;t2.Position.y = 4;t3.Position.x = 29;t3.Position.y = 5;t4.Position.x = 0;t4.Position.y = 5;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 5;t2.Position.x = 29;t2.Position.y = 5;t3.Position.x = 29;t3.Position.y = 6;t4.Position.x = 0;t4.Position.y = 6;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 81;t1.Position.y = 5;t2.Position.x = 87;t2.Position.y = 5;t3.Position.x = 87;t3.Position.y = 6;t4.Position.x = 81;t4.Position.y = 6;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 6;t2.Position.x = 30;t2.Position.y = 6;t3.Position.x = 30;t3.Position.y = 7;t4.Position.x = 0;t4.Position.y = 7;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 77;t1.Position.y = 6;t2.Position.x = 90;t2.Position.y = 6;t3.Position.x = 90;t3.Position.y = 7;t4.Position.x = 77;t4.Position.y = 7;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 7;t2.Position.x = 30;t2.Position.y = 7;t3.Position.x = 30;t3.Position.y = 8;t4.Position.x = 0;t4.Position.y = 8;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 76;t1.Position.y = 7;t2.Position.x = 92;t2.Position.y = 7;t3.Position.x = 92;t3.Position.y = 8;t4.Position.x = 76;t4.Position.y = 8;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 8;t2.Position.x = 31;t2.Position.y = 8;t3.Position.x = 31;t3.Position.y = 9;t4.Position.x = 1;t4.Position.y = 9;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 75;t1.Position.y = 8;t2.Position.x = 93;t2.Position.y = 8;t3.Position.x = 93;t3.Position.y = 9;t4.Position.x = 75;t4.Position.y = 9;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 9;t2.Position.x = 31;t2.Position.y = 9;t3.Position.x = 31;t3.Position.y = 10;t4.Position.x = 1;t4.Position.y = 10;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 74;t1.Position.y = 9;t2.Position.x = 95;t2.Position.y = 9;t3.Position.x = 95;t3.Position.y = 10;t4.Position.x = 74;t4.Position.y = 10;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 10;t2.Position.x = 10;t2.Position.y = 10;t3.Position.x = 10;t3.Position.y = 11;t4.Position.x = 1;t4.Position.y = 11;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 18;t1.Position.y = 10;t2.Position.x = 32;t2.Position.y = 10;t3.Position.x = 32;t3.Position.y = 11;t4.Position.x = 18;t4.Position.y = 11;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 73;t1.Position.y = 10;t2.Position.x = 96;t2.Position.y = 10;t3.Position.x = 96;t3.Position.y = 11;t4.Position.x = 73;t4.Position.y = 11;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 11;t2.Position.x = 9;t2.Position.y = 11;t3.Position.x = 9;t3.Position.y = 12;t4.Position.x = 1;t4.Position.y = 12;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 19;t1.Position.y = 11;t2.Position.x = 32;t2.Position.y = 11;t3.Position.x = 32;t3.Position.y = 12;t4.Position.x = 19;t4.Position.y = 12;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 72;t1.Position.y = 11;t2.Position.x = 96;t2.Position.y = 11;t3.Position.x = 96;t3.Position.y = 12;t4.Position.x = 72;t4.Position.y = 12;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 12;t2.Position.x = 9;t2.Position.y = 12;t3.Position.x = 9;t3.Position.y = 13;t4.Position.x = 1;t4.Position.y = 13;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 19;t1.Position.y = 12;t2.Position.x = 33;t2.Position.y = 12;t3.Position.x = 33;t3.Position.y = 13;t4.Position.x = 19;t4.Position.y = 13;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 72;t1.Position.y = 12;t2.Position.x = 97;t2.Position.y = 12;t3.Position.x = 97;t3.Position.y = 13;t4.Position.x = 72;t4.Position.y = 13;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 13;t2.Position.x = 9;t2.Position.y = 13;t3.Position.x = 9;t3.Position.y = 14;t4.Position.x = 1;t4.Position.y = 14;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 19;t1.Position.y = 13;t2.Position.x = 33;t2.Position.y = 13;t3.Position.x = 33;t3.Position.y = 14;t4.Position.x = 19;t4.Position.y = 14;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 71;t1.Position.y = 13;t2.Position.x = 98;t2.Position.y = 13;t3.Position.x = 98;t3.Position.y = 14;t4.Position.x = 71;t4.Position.y = 14;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 14;t2.Position.x = 9;t2.Position.y = 14;t3.Position.x = 9;t3.Position.y = 15;t4.Position.x = 1;t4.Position.y = 15;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 19;t1.Position.y = 14;t2.Position.x = 34;t2.Position.y = 14;t3.Position.x = 34;t3.Position.y = 15;t4.Position.x = 19;t4.Position.y = 15;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 71;t1.Position.y = 14;t2.Position.x = 98;t2.Position.y = 14;t3.Position.x = 98;t3.Position.y = 15;t4.Position.x = 71;t4.Position.y = 15;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 15;t2.Position.x = 9;t2.Position.y = 15;t3.Position.x = 9;t3.Position.y = 16;t4.Position.x = 1;t4.Position.y = 16;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 20;t1.Position.y = 15;t2.Position.x = 35;t2.Position.y = 15;t3.Position.x = 35;t3.Position.y = 16;t4.Position.x = 20;t4.Position.y = 16;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 71;t1.Position.y = 15;t2.Position.x = 99;t2.Position.y = 15;t3.Position.x = 99;t3.Position.y = 16;t4.Position.x = 71;t4.Position.y = 16;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 16;t2.Position.x = 9;t2.Position.y = 16;t3.Position.x = 9;t3.Position.y = 17;t4.Position.x = 1;t4.Position.y = 17;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 20;t1.Position.y = 16;t2.Position.x = 99;t2.Position.y = 16;t3.Position.x = 99;t3.Position.y = 17;t4.Position.x = 20;t4.Position.y = 17;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 17;t2.Position.x = 9;t2.Position.y = 17;t3.Position.x = 9;t3.Position.y = 18;t4.Position.x = 2;t4.Position.y = 18;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 20;t1.Position.y = 17;t2.Position.x = 99;t2.Position.y = 17;t3.Position.x = 99;t3.Position.y = 18;t4.Position.x = 20;t4.Position.y = 18;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 18;t2.Position.x = 9;t2.Position.y = 18;t3.Position.x = 9;t3.Position.y = 19;t4.Position.x = 2;t4.Position.y = 19;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 21;t1.Position.y = 18;t2.Position.x = 99;t2.Position.y = 18;t3.Position.x = 99;t3.Position.y = 19;t4.Position.x = 21;t4.Position.y = 19;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 19;t2.Position.x = 9;t2.Position.y = 19;t3.Position.x = 9;t3.Position.y = 20;t4.Position.x = 2;t4.Position.y = 20;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 21;t1.Position.y = 19;t2.Position.x = 99;t2.Position.y = 19;t3.Position.x = 99;t3.Position.y = 20;t4.Position.x = 21;t4.Position.y = 20;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 20;t2.Position.x = 9;t2.Position.y = 20;t3.Position.x = 9;t3.Position.y = 21;t4.Position.x = 2;t4.Position.y = 21;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 21;t1.Position.y = 20;t2.Position.x = 99;t2.Position.y = 20;t3.Position.x = 99;t3.Position.y = 21;t4.Position.x = 21;t4.Position.y = 21;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 21;t2.Position.x = 9;t2.Position.y = 21;t3.Position.x = 9;t3.Position.y = 22;t4.Position.x = 2;t4.Position.y = 22;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 22;t1.Position.y = 21;t2.Position.x = 99;t2.Position.y = 21;t3.Position.x = 99;t3.Position.y = 22;t4.Position.x = 22;t4.Position.y = 22;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 22;t2.Position.x = 9;t2.Position.y = 22;t3.Position.x = 9;t3.Position.y = 23;t4.Position.x = 2;t4.Position.y = 23;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 22;t1.Position.y = 22;t2.Position.x = 99;t2.Position.y = 22;t3.Position.x = 99;t3.Position.y = 23;t4.Position.x = 22;t4.Position.y = 23;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 23;t2.Position.x = 9;t2.Position.y = 23;t3.Position.x = 9;t3.Position.y = 24;t4.Position.x = 2;t4.Position.y = 24;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 70;t1.Position.y = 23;t2.Position.x = 99;t2.Position.y = 23;t3.Position.x = 99;t3.Position.y = 24;t4.Position.x = 70;t4.Position.y = 24;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 24;t2.Position.x = 9;t2.Position.y = 24;t3.Position.x = 9;t3.Position.y = 25;t4.Position.x = 2;t4.Position.y = 25;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 71;t1.Position.y = 24;t2.Position.x = 99;t2.Position.y = 24;t3.Position.x = 99;t3.Position.y = 25;t4.Position.x = 71;t4.Position.y = 25;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 25;t2.Position.x = 9;t2.Position.y = 25;t3.Position.x = 9;t3.Position.y = 26;t4.Position.x = 2;t4.Position.y = 26;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 72;t1.Position.y = 25;t2.Position.x = 98;t2.Position.y = 25;t3.Position.x = 98;t3.Position.y = 26;t4.Position.x = 72;t4.Position.y = 26;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 26;t2.Position.x = 9;t2.Position.y = 26;t3.Position.x = 9;t3.Position.y = 27;t4.Position.x = 2;t4.Position.y = 27;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 72;t1.Position.y = 26;t2.Position.x = 98;t2.Position.y = 26;t3.Position.x = 98;t3.Position.y = 27;t4.Position.x = 72;t4.Position.y = 27;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 3;t1.Position.y = 27;t2.Position.x = 9;t2.Position.y = 27;t3.Position.x = 9;t3.Position.y = 28;t4.Position.x = 3;t4.Position.y = 28;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 73;t1.Position.y = 27;t2.Position.x = 97;t2.Position.y = 27;t3.Position.x = 97;t3.Position.y = 28;t4.Position.x = 73;t4.Position.y = 28;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 3;t1.Position.y = 28;t2.Position.x = 9;t2.Position.y = 28;t3.Position.x = 9;t3.Position.y = 29;t4.Position.x = 3;t4.Position.y = 29;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 74;t1.Position.y = 28;t2.Position.x = 97;t2.Position.y = 28;t3.Position.x = 97;t3.Position.y = 29;t4.Position.x = 74;t4.Position.y = 29;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 3;t1.Position.y = 29;t2.Position.x = 9;t2.Position.y = 29;t3.Position.x = 9;t3.Position.y = 30;t4.Position.x = 3;t4.Position.y = 30;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 75;t1.Position.y = 29;t2.Position.x = 96;t2.Position.y = 29;t3.Position.x = 96;t3.Position.y = 30;t4.Position.x = 75;t4.Position.y = 30;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 3;t1.Position.y = 30;t2.Position.x = 9;t2.Position.y = 30;t3.Position.x = 9;t3.Position.y = 31;t4.Position.x = 3;t4.Position.y = 31;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 76;t1.Position.y = 30;t2.Position.x = 95;t2.Position.y = 30;t3.Position.x = 95;t3.Position.y = 31;t4.Position.x = 76;t4.Position.y = 31;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 3;t1.Position.y = 31;t2.Position.x = 9;t2.Position.y = 31;t3.Position.x = 9;t3.Position.y = 32;t4.Position.x = 3;t4.Position.y = 32;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 78;t1.Position.y = 31;t2.Position.x = 94;t2.Position.y = 31;t3.Position.x = 94;t3.Position.y = 32;t4.Position.x = 78;t4.Position.y = 32;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 3;t1.Position.y = 32;t2.Position.x = 9;t2.Position.y = 32;t3.Position.x = 9;t3.Position.y = 33;t4.Position.x = 3;t4.Position.y = 33;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 79;t1.Position.y = 32;t2.Position.x = 92;t2.Position.y = 32;t3.Position.x = 92;t3.Position.y = 33;t4.Position.x = 79;t4.Position.y = 33;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 3;t1.Position.y = 33;t2.Position.x = 9;t2.Position.y = 33;t3.Position.x = 9;t3.Position.y = 34;t4.Position.x = 3;t4.Position.y = 34;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 81;t1.Position.y = 33;t2.Position.x = 91;t2.Position.y = 33;t3.Position.x = 91;t3.Position.y = 34;t4.Position.x = 81;t4.Position.y = 34;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 4;t1.Position.y = 34;t2.Position.x = 9;t2.Position.y = 34;t3.Position.x = 9;t3.Position.y = 35;t4.Position.x = 4;t4.Position.y = 35;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 4;t1.Position.y = 35;t2.Position.x = 8;t2.Position.y = 35;t3.Position.x = 8;t3.Position.y = 36;t4.Position.x = 4;t4.Position.y = 36;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 5;t1.Position.y = 36;t2.Position.x = 8;t2.Position.y = 36;t3.Position.x = 8;t3.Position.y = 37;t4.Position.x = 5;t4.Position.y = 37;mh.AddTriangle( t1, t2, t3, true );mh.AddTriangle( t3, t4, t1, true );
mh.AddTriangle( t1, t2, t3, true );
mh.AddTriangle( t3, t4, t1, true );
#endif
}