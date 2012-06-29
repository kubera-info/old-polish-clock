#include "DXUT.h"
#include "BackHookCore-$$$.h"

void MakeBackHook( CMeshBuilderPlain & bh )
{
#ifdef DEBUG
	VERTEXPLAIN t1, t2, t3, t4;

t1.Position.x = 79;t1.Position.y = 0;t2.Position.x = 82;t2.Position.y = 0;t3.Position.x = 82;t3.Position.y = 1;t4.Position.x = 79;t4.Position.y = 1;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 78;t1.Position.y = 1;t2.Position.x = 84;t2.Position.y = 1;t3.Position.x = 84;t3.Position.y = 2;t4.Position.x = 78;t4.Position.y = 2;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 30;t1.Position.y = 2;t2.Position.x = 36;t2.Position.y = 2;t3.Position.x = 36;t3.Position.y = 3;t4.Position.x = 30;t4.Position.y = 3;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 78;t1.Position.y = 2;t2.Position.x = 85;t2.Position.y = 2;t3.Position.x = 85;t3.Position.y = 3;t4.Position.x = 78;t4.Position.y = 3;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 26;t1.Position.y = 3;t2.Position.x = 44;t2.Position.y = 3;t3.Position.x = 44;t3.Position.y = 4;t4.Position.x = 26;t4.Position.y = 4;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 77;t1.Position.y = 3;t2.Position.x = 85;t2.Position.y = 3;t3.Position.x = 85;t3.Position.y = 4;t4.Position.x = 77;t4.Position.y = 4;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 22;t1.Position.y = 4;t2.Position.x = 49;t2.Position.y = 4;t3.Position.x = 49;t3.Position.y = 5;t4.Position.x = 22;t4.Position.y = 5;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 77;t1.Position.y = 4;t2.Position.x = 86;t2.Position.y = 4;t3.Position.x = 86;t3.Position.y = 5;t4.Position.x = 77;t4.Position.y = 5;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 4;t2.Position.x = 139;t2.Position.y = 4;t3.Position.x = 139;t3.Position.y = 5;t4.Position.x = 137;t4.Position.y = 5;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 20;t1.Position.y = 5;t2.Position.x = 52;t2.Position.y = 5;t3.Position.x = 52;t3.Position.y = 6;t4.Position.x = 20;t4.Position.y = 6;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 77;t1.Position.y = 5;t2.Position.x = 86;t2.Position.y = 5;t3.Position.x = 86;t3.Position.y = 6;t4.Position.x = 77;t4.Position.y = 6;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 135;t1.Position.y = 5;t2.Position.x = 144;t2.Position.y = 5;t3.Position.x = 144;t3.Position.y = 6;t4.Position.x = 135;t4.Position.y = 6;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 18;t1.Position.y = 6;t2.Position.x = 55;t2.Position.y = 6;t3.Position.x = 55;t3.Position.y = 7;t4.Position.x = 18;t4.Position.y = 7;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 77;t1.Position.y = 6;t2.Position.x = 86;t2.Position.y = 6;t3.Position.x = 86;t3.Position.y = 7;t4.Position.x = 77;t4.Position.y = 7;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 134;t1.Position.y = 6;t2.Position.x = 145;t2.Position.y = 6;t3.Position.x = 145;t3.Position.y = 7;t4.Position.x = 134;t4.Position.y = 7;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 15;t1.Position.y = 7;t2.Position.x = 58;t2.Position.y = 7;t3.Position.x = 58;t3.Position.y = 8;t4.Position.x = 15;t4.Position.y = 8;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 76;t1.Position.y = 7;t2.Position.x = 87;t2.Position.y = 7;t3.Position.x = 87;t3.Position.y = 8;t4.Position.x = 76;t4.Position.y = 8;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 133;t1.Position.y = 7;t2.Position.x = 146;t2.Position.y = 7;t3.Position.x = 146;t3.Position.y = 8;t4.Position.x = 133;t4.Position.y = 8;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 13;t1.Position.y = 8;t2.Position.x = 60;t2.Position.y = 8;t3.Position.x = 60;t3.Position.y = 9;t4.Position.x = 13;t4.Position.y = 9;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 76;t1.Position.y = 8;t2.Position.x = 87;t2.Position.y = 8;t3.Position.x = 87;t3.Position.y = 9;t4.Position.x = 76;t4.Position.y = 9;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 132;t1.Position.y = 8;t2.Position.x = 146;t2.Position.y = 8;t3.Position.x = 146;t3.Position.y = 9;t4.Position.x = 132;t4.Position.y = 9;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 11;t1.Position.y = 9;t2.Position.x = 61;t2.Position.y = 9;t3.Position.x = 61;t3.Position.y = 10;t4.Position.x = 11;t4.Position.y = 10;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 76;t1.Position.y = 9;t2.Position.x = 88;t2.Position.y = 9;t3.Position.x = 88;t3.Position.y = 10;t4.Position.x = 76;t4.Position.y = 10;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 131;t1.Position.y = 9;t2.Position.x = 147;t2.Position.y = 9;t3.Position.x = 147;t3.Position.y = 10;t4.Position.x = 131;t4.Position.y = 10;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 10;t1.Position.y = 10;t2.Position.x = 62;t2.Position.y = 10;t3.Position.x = 62;t3.Position.y = 11;t4.Position.x = 10;t4.Position.y = 11;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 75;t1.Position.y = 10;t2.Position.x = 89;t2.Position.y = 10;t3.Position.x = 89;t3.Position.y = 11;t4.Position.x = 75;t4.Position.y = 11;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 130;t1.Position.y = 10;t2.Position.x = 147;t2.Position.y = 10;t3.Position.x = 147;t3.Position.y = 11;t4.Position.x = 130;t4.Position.y = 11;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 9;t1.Position.y = 11;t2.Position.x = 63;t2.Position.y = 11;t3.Position.x = 63;t3.Position.y = 12;t4.Position.x = 9;t4.Position.y = 12;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 75;t1.Position.y = 11;t2.Position.x = 90;t2.Position.y = 11;t3.Position.x = 90;t3.Position.y = 12;t4.Position.x = 75;t4.Position.y = 12;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 130;t1.Position.y = 11;t2.Position.x = 147;t2.Position.y = 11;t3.Position.x = 147;t3.Position.y = 12;t4.Position.x = 130;t4.Position.y = 12;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 8;t1.Position.y = 12;t2.Position.x = 64;t2.Position.y = 12;t3.Position.x = 64;t3.Position.y = 13;t4.Position.x = 8;t4.Position.y = 13;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 75;t1.Position.y = 12;t2.Position.x = 91;t2.Position.y = 12;t3.Position.x = 91;t3.Position.y = 13;t4.Position.x = 75;t4.Position.y = 13;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 129;t1.Position.y = 12;t2.Position.x = 147;t2.Position.y = 12;t3.Position.x = 147;t3.Position.y = 13;t4.Position.x = 129;t4.Position.y = 13;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 7;t1.Position.y = 13;t2.Position.x = 65;t2.Position.y = 13;t3.Position.x = 65;t3.Position.y = 14;t4.Position.x = 7;t4.Position.y = 14;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 75;t1.Position.y = 13;t2.Position.x = 92;t2.Position.y = 13;t3.Position.x = 92;t3.Position.y = 14;t4.Position.x = 75;t4.Position.y = 14;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 128;t1.Position.y = 13;t2.Position.x = 147;t2.Position.y = 13;t3.Position.x = 147;t3.Position.y = 14;t4.Position.x = 128;t4.Position.y = 14;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 6;t1.Position.y = 14;t2.Position.x = 66;t2.Position.y = 14;t3.Position.x = 66;t3.Position.y = 15;t4.Position.x = 6;t4.Position.y = 15;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 74;t1.Position.y = 14;t2.Position.x = 94;t2.Position.y = 14;t3.Position.x = 94;t3.Position.y = 15;t4.Position.x = 74;t4.Position.y = 15;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 127;t1.Position.y = 14;t2.Position.x = 148;t2.Position.y = 14;t3.Position.x = 148;t3.Position.y = 15;t4.Position.x = 127;t4.Position.y = 15;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 5;t1.Position.y = 15;t2.Position.x = 67;t2.Position.y = 15;t3.Position.x = 67;t3.Position.y = 16;t4.Position.x = 5;t4.Position.y = 16;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 74;t1.Position.y = 15;t2.Position.x = 97;t2.Position.y = 15;t3.Position.x = 97;t3.Position.y = 16;t4.Position.x = 74;t4.Position.y = 16;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 126;t1.Position.y = 15;t2.Position.x = 148;t2.Position.y = 15;t3.Position.x = 148;t3.Position.y = 16;t4.Position.x = 126;t4.Position.y = 16;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 4;t1.Position.y = 16;t2.Position.x = 68;t2.Position.y = 16;t3.Position.x = 68;t3.Position.y = 17;t4.Position.x = 4;t4.Position.y = 17;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 74;t1.Position.y = 16;t2.Position.x = 99;t2.Position.y = 16;t3.Position.x = 99;t3.Position.y = 17;t4.Position.x = 74;t4.Position.y = 17;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 124;t1.Position.y = 16;t2.Position.x = 148;t2.Position.y = 16;t3.Position.x = 148;t3.Position.y = 17;t4.Position.x = 124;t4.Position.y = 17;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 4;t1.Position.y = 17;t2.Position.x = 68;t2.Position.y = 17;t3.Position.x = 68;t3.Position.y = 18;t4.Position.x = 4;t4.Position.y = 18;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 74;t1.Position.y = 17;t2.Position.x = 101;t2.Position.y = 17;t3.Position.x = 101;t3.Position.y = 18;t4.Position.x = 74;t4.Position.y = 18;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 122;t1.Position.y = 17;t2.Position.x = 148;t2.Position.y = 17;t3.Position.x = 148;t3.Position.y = 18;t4.Position.x = 122;t4.Position.y = 18;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 3;t1.Position.y = 18;t2.Position.x = 69;t2.Position.y = 18;t3.Position.x = 69;t3.Position.y = 19;t4.Position.x = 3;t4.Position.y = 19;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 73;t1.Position.y = 18;t2.Position.x = 103;t2.Position.y = 18;t3.Position.x = 103;t3.Position.y = 19;t4.Position.x = 73;t4.Position.y = 19;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 120;t1.Position.y = 18;t2.Position.x = 148;t2.Position.y = 18;t3.Position.x = 148;t3.Position.y = 19;t4.Position.x = 120;t4.Position.y = 19;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 3;t1.Position.y = 19;t2.Position.x = 33;t2.Position.y = 19;t3.Position.x = 33;t3.Position.y = 20;t4.Position.x = 3;t4.Position.y = 20;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 42;t1.Position.y = 19;t2.Position.x = 105;t2.Position.y = 19;t3.Position.x = 105;t3.Position.y = 20;t4.Position.x = 42;t4.Position.y = 20;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 117;t1.Position.y = 19;t2.Position.x = 148;t2.Position.y = 19;t3.Position.x = 148;t3.Position.y = 20;t4.Position.x = 117;t4.Position.y = 20;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 20;t2.Position.x = 29;t2.Position.y = 20;t3.Position.x = 29;t3.Position.y = 21;t4.Position.x = 2;t4.Position.y = 21;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 46;t1.Position.y = 20;t2.Position.x = 107;t2.Position.y = 20;t3.Position.x = 107;t3.Position.y = 21;t4.Position.x = 46;t4.Position.y = 21;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 113;t1.Position.y = 20;t2.Position.x = 148;t2.Position.y = 20;t3.Position.x = 148;t3.Position.y = 21;t4.Position.x = 113;t4.Position.y = 21;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 21;t2.Position.x = 29;t2.Position.y = 21;t3.Position.x = 29;t3.Position.y = 22;t4.Position.x = 2;t4.Position.y = 22;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 49;t1.Position.y = 21;t2.Position.x = 148;t2.Position.y = 21;t3.Position.x = 148;t3.Position.y = 22;t4.Position.x = 49;t4.Position.y = 22;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 22;t2.Position.x = 30;t2.Position.y = 22;t3.Position.x = 30;t3.Position.y = 23;t4.Position.x = 1;t4.Position.y = 23;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 51;t1.Position.y = 22;t2.Position.x = 148;t2.Position.y = 22;t3.Position.x = 148;t3.Position.y = 23;t4.Position.x = 51;t4.Position.y = 23;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 23;t2.Position.x = 30;t2.Position.y = 23;t3.Position.x = 30;t3.Position.y = 24;t4.Position.x = 1;t4.Position.y = 24;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 53;t1.Position.y = 23;t2.Position.x = 148;t2.Position.y = 23;t3.Position.x = 148;t3.Position.y = 24;t4.Position.x = 53;t4.Position.y = 24;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 24;t2.Position.x = 31;t2.Position.y = 24;t3.Position.x = 31;t3.Position.y = 25;t4.Position.x = 1;t4.Position.y = 25;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 54;t1.Position.y = 24;t2.Position.x = 148;t2.Position.y = 24;t3.Position.x = 148;t3.Position.y = 25;t4.Position.x = 54;t4.Position.y = 25;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 25;t2.Position.x = 31;t2.Position.y = 25;t3.Position.x = 31;t3.Position.y = 26;t4.Position.x = 0;t4.Position.y = 26;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 55;t1.Position.y = 25;t2.Position.x = 148;t2.Position.y = 25;t3.Position.x = 148;t3.Position.y = 26;t4.Position.x = 55;t4.Position.y = 26;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 26;t2.Position.x = 32;t2.Position.y = 26;t3.Position.x = 32;t3.Position.y = 27;t4.Position.x = 0;t4.Position.y = 27;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 58;t1.Position.y = 26;t2.Position.x = 148;t2.Position.y = 26;t3.Position.x = 148;t3.Position.y = 27;t4.Position.x = 58;t4.Position.y = 27;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 27;t2.Position.x = 32;t2.Position.y = 27;t3.Position.x = 32;t3.Position.y = 28;t4.Position.x = 0;t4.Position.y = 28;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 57;t1.Position.y = 27;t2.Position.x = 148;t2.Position.y = 27;t3.Position.x = 148;t3.Position.y = 28;t4.Position.x = 57;t4.Position.y = 28;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 28;t2.Position.x = 32;t2.Position.y = 28;t3.Position.x = 32;t3.Position.y = 29;t4.Position.x = 0;t4.Position.y = 29;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 56;t1.Position.y = 28;t2.Position.x = 148;t2.Position.y = 28;t3.Position.x = 148;t3.Position.y = 29;t4.Position.x = 56;t4.Position.y = 29;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 29;t2.Position.x = 33;t2.Position.y = 29;t3.Position.x = 33;t3.Position.y = 30;t4.Position.x = 0;t4.Position.y = 30;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 56;t1.Position.y = 29;t2.Position.x = 148;t2.Position.y = 29;t3.Position.x = 148;t3.Position.y = 30;t4.Position.x = 56;t4.Position.y = 30;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 30;t2.Position.x = 33;t2.Position.y = 30;t3.Position.x = 33;t3.Position.y = 31;t4.Position.x = 0;t4.Position.y = 31;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 56;t1.Position.y = 30;t2.Position.x = 148;t2.Position.y = 30;t3.Position.x = 148;t3.Position.y = 31;t4.Position.x = 56;t4.Position.y = 31;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 31;t2.Position.x = 33;t2.Position.y = 31;t3.Position.x = 33;t3.Position.y = 32;t4.Position.x = 0;t4.Position.y = 32;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 55;t1.Position.y = 31;t2.Position.x = 149;t2.Position.y = 31;t3.Position.x = 149;t3.Position.y = 32;t4.Position.x = 55;t4.Position.y = 32;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 32;t2.Position.x = 33;t2.Position.y = 32;t3.Position.x = 33;t3.Position.y = 33;t4.Position.x = 0;t4.Position.y = 33;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 55;t1.Position.y = 32;t2.Position.x = 149;t2.Position.y = 32;t3.Position.x = 149;t3.Position.y = 33;t4.Position.x = 55;t4.Position.y = 33;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 33;t2.Position.x = 33;t2.Position.y = 33;t3.Position.x = 33;t3.Position.y = 34;t4.Position.x = 0;t4.Position.y = 34;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 54;t1.Position.y = 33;t2.Position.x = 149;t2.Position.y = 33;t3.Position.x = 149;t3.Position.y = 34;t4.Position.x = 54;t4.Position.y = 34;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 0;t1.Position.y = 34;t2.Position.x = 33;t2.Position.y = 34;t3.Position.x = 33;t3.Position.y = 35;t4.Position.x = 0;t4.Position.y = 35;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 54;t1.Position.y = 34;t2.Position.x = 149;t2.Position.y = 34;t3.Position.x = 149;t3.Position.y = 35;t4.Position.x = 54;t4.Position.y = 35;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 35;t2.Position.x = 33;t2.Position.y = 35;t3.Position.x = 33;t3.Position.y = 36;t4.Position.x = 1;t4.Position.y = 36;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 54;t1.Position.y = 35;t2.Position.x = 149;t2.Position.y = 35;t3.Position.x = 149;t3.Position.y = 36;t4.Position.x = 54;t4.Position.y = 36;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 1;t1.Position.y = 36;t2.Position.x = 33;t2.Position.y = 36;t3.Position.x = 33;t3.Position.y = 37;t4.Position.x = 1;t4.Position.y = 37;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 53;t1.Position.y = 36;t2.Position.x = 149;t2.Position.y = 36;t3.Position.x = 149;t3.Position.y = 37;t4.Position.x = 53;t4.Position.y = 37;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 37;t2.Position.x = 32;t2.Position.y = 37;t3.Position.x = 32;t3.Position.y = 38;t4.Position.x = 2;t4.Position.y = 38;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 53;t1.Position.y = 37;t2.Position.x = 115;t2.Position.y = 37;t3.Position.x = 115;t3.Position.y = 38;t4.Position.x = 53;t4.Position.y = 38;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 117;t1.Position.y = 37;t2.Position.x = 149;t2.Position.y = 37;t3.Position.x = 149;t3.Position.y = 38;t4.Position.x = 117;t4.Position.y = 38;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 38;t2.Position.x = 32;t2.Position.y = 38;t3.Position.x = 32;t3.Position.y = 39;t4.Position.x = 2;t4.Position.y = 39;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 53;t1.Position.y = 38;t2.Position.x = 114;t2.Position.y = 38;t3.Position.x = 114;t3.Position.y = 39;t4.Position.x = 53;t4.Position.y = 39;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 117;t1.Position.y = 38;t2.Position.x = 149;t2.Position.y = 38;t3.Position.x = 149;t3.Position.y = 39;t4.Position.x = 117;t4.Position.y = 39;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 2;t1.Position.y = 39;t2.Position.x = 32;t2.Position.y = 39;t3.Position.x = 32;t3.Position.y = 40;t4.Position.x = 2;t4.Position.y = 40;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 53;t1.Position.y = 39;t2.Position.x = 112;t2.Position.y = 39;t3.Position.x = 112;t3.Position.y = 40;t4.Position.x = 53;t4.Position.y = 40;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 118;t1.Position.y = 39;t2.Position.x = 149;t2.Position.y = 39;t3.Position.x = 149;t3.Position.y = 40;t4.Position.x = 118;t4.Position.y = 40;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 3;t1.Position.y = 40;t2.Position.x = 32;t2.Position.y = 40;t3.Position.x = 32;t3.Position.y = 41;t4.Position.x = 3;t4.Position.y = 41;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 52;t1.Position.y = 40;t2.Position.x = 109;t2.Position.y = 40;t3.Position.x = 109;t3.Position.y = 41;t4.Position.x = 52;t4.Position.y = 41;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 118;t1.Position.y = 40;t2.Position.x = 149;t2.Position.y = 40;t3.Position.x = 149;t3.Position.y = 41;t4.Position.x = 118;t4.Position.y = 41;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 3;t1.Position.y = 41;t2.Position.x = 31;t2.Position.y = 41;t3.Position.x = 31;t3.Position.y = 42;t4.Position.x = 3;t4.Position.y = 42;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 52;t1.Position.y = 41;t2.Position.x = 68;t2.Position.y = 41;t3.Position.x = 68;t3.Position.y = 42;t4.Position.x = 52;t4.Position.y = 42;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 75;t1.Position.y = 41;t2.Position.x = 106;t2.Position.y = 41;t3.Position.x = 106;t3.Position.y = 42;t4.Position.x = 75;t4.Position.y = 42;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 119;t1.Position.y = 41;t2.Position.x = 130;t2.Position.y = 41;t3.Position.x = 130;t3.Position.y = 42;t4.Position.x = 119;t4.Position.y = 42;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 135;t1.Position.y = 41;t2.Position.x = 149;t2.Position.y = 41;t3.Position.x = 149;t3.Position.y = 42;t4.Position.x = 135;t4.Position.y = 42;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 4;t1.Position.y = 42;t2.Position.x = 31;t2.Position.y = 42;t3.Position.x = 31;t3.Position.y = 43;t4.Position.x = 4;t4.Position.y = 43;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 52;t1.Position.y = 42;t2.Position.x = 65;t2.Position.y = 42;t3.Position.x = 65;t3.Position.y = 43;t4.Position.x = 52;t4.Position.y = 43;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 81;t1.Position.y = 42;t2.Position.x = 102;t2.Position.y = 42;t3.Position.x = 102;t3.Position.y = 43;t4.Position.x = 81;t4.Position.y = 43;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 119;t1.Position.y = 42;t2.Position.x = 129;t2.Position.y = 42;t3.Position.x = 129;t3.Position.y = 43;t4.Position.x = 119;t4.Position.y = 43;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 42;t2.Position.x = 149;t2.Position.y = 42;t3.Position.x = 149;t3.Position.y = 43;t4.Position.x = 137;t4.Position.y = 43;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 5;t1.Position.y = 43;t2.Position.x = 30;t2.Position.y = 43;t3.Position.x = 30;t3.Position.y = 44;t4.Position.x = 5;t4.Position.y = 44;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 52;t1.Position.y = 43;t2.Position.x = 63;t2.Position.y = 43;t3.Position.x = 63;t3.Position.y = 44;t4.Position.x = 52;t4.Position.y = 44;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 90;t1.Position.y = 43;t2.Position.x = 97;t2.Position.y = 43;t3.Position.x = 97;t3.Position.y = 44;t4.Position.x = 90;t4.Position.y = 44;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 119;t1.Position.y = 43;t2.Position.x = 128;t2.Position.y = 43;t3.Position.x = 128;t3.Position.y = 44;t4.Position.x = 119;t4.Position.y = 44;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 43;t2.Position.x = 149;t2.Position.y = 43;t3.Position.x = 149;t3.Position.y = 44;t4.Position.x = 137;t4.Position.y = 44;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 6;t1.Position.y = 44;t2.Position.x = 29;t2.Position.y = 44;t3.Position.x = 29;t3.Position.y = 45;t4.Position.x = 6;t4.Position.y = 45;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 53;t1.Position.y = 44;t2.Position.x = 61;t2.Position.y = 44;t3.Position.x = 61;t3.Position.y = 45;t4.Position.x = 53;t4.Position.y = 45;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 119;t1.Position.y = 44;t2.Position.x = 128;t2.Position.y = 44;t3.Position.x = 128;t3.Position.y = 45;t4.Position.x = 119;t4.Position.y = 45;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 44;t2.Position.x = 149;t2.Position.y = 44;t3.Position.x = 149;t3.Position.y = 45;t4.Position.x = 137;t4.Position.y = 45;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 6;t1.Position.y = 45;t2.Position.x = 28;t2.Position.y = 45;t3.Position.x = 28;t3.Position.y = 46;t4.Position.x = 6;t4.Position.y = 46;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 53;t1.Position.y = 45;t2.Position.x = 61;t2.Position.y = 45;t3.Position.x = 61;t3.Position.y = 46;t4.Position.x = 53;t4.Position.y = 46;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 120;t1.Position.y = 45;t2.Position.x = 128;t2.Position.y = 45;t3.Position.x = 128;t3.Position.y = 46;t4.Position.x = 120;t4.Position.y = 46;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 45;t2.Position.x = 149;t2.Position.y = 45;t3.Position.x = 149;t3.Position.y = 46;t4.Position.x = 138;t4.Position.y = 46;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 7;t1.Position.y = 46;t2.Position.x = 27;t2.Position.y = 46;t3.Position.x = 27;t3.Position.y = 47;t4.Position.x = 7;t4.Position.y = 47;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 54;t1.Position.y = 46;t2.Position.x = 60;t2.Position.y = 46;t3.Position.x = 60;t3.Position.y = 47;t4.Position.x = 54;t4.Position.y = 47;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 120;t1.Position.y = 46;t2.Position.x = 128;t2.Position.y = 46;t3.Position.x = 128;t3.Position.y = 47;t4.Position.x = 120;t4.Position.y = 47;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 46;t2.Position.x = 149;t2.Position.y = 46;t3.Position.x = 149;t3.Position.y = 47;t4.Position.x = 138;t4.Position.y = 47;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 9;t1.Position.y = 47;t2.Position.x = 26;t2.Position.y = 47;t3.Position.x = 26;t3.Position.y = 48;t4.Position.x = 9;t4.Position.y = 48;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 55;t1.Position.y = 47;t2.Position.x = 59;t2.Position.y = 47;t3.Position.x = 59;t3.Position.y = 48;t4.Position.x = 55;t4.Position.y = 48;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 120;t1.Position.y = 47;t2.Position.x = 128;t2.Position.y = 47;t3.Position.x = 128;t3.Position.y = 48;t4.Position.x = 120;t4.Position.y = 48;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 47;t2.Position.x = 149;t2.Position.y = 47;t3.Position.x = 149;t3.Position.y = 48;t4.Position.x = 138;t4.Position.y = 48;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 11;t1.Position.y = 48;t2.Position.x = 24;t2.Position.y = 48;t3.Position.x = 24;t3.Position.y = 49;t4.Position.x = 11;t4.Position.y = 49;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 56;t1.Position.y = 48;t2.Position.x = 59;t2.Position.y = 48;t3.Position.x = 59;t3.Position.y = 49;t4.Position.x = 56;t4.Position.y = 49;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 120;t1.Position.y = 48;t2.Position.x = 127;t2.Position.y = 48;t3.Position.x = 127;t3.Position.y = 49;t4.Position.x = 120;t4.Position.y = 49;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 48;t2.Position.x = 149;t2.Position.y = 48;t3.Position.x = 149;t3.Position.y = 49;t4.Position.x = 138;t4.Position.y = 49;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 15;t1.Position.y = 49;t2.Position.x = 21;t2.Position.y = 49;t3.Position.x = 21;t3.Position.y = 50;t4.Position.x = 15;t4.Position.y = 50;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 57;t1.Position.y = 49;t2.Position.x = 58;t2.Position.y = 49;t3.Position.x = 58;t3.Position.y = 50;t4.Position.x = 57;t4.Position.y = 50;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 121;t1.Position.y = 49;t2.Position.x = 127;t2.Position.y = 49;t3.Position.x = 127;t3.Position.y = 50;t4.Position.x = 121;t4.Position.y = 50;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 139;t1.Position.y = 49;t2.Position.x = 149;t2.Position.y = 49;t3.Position.x = 149;t3.Position.y = 50;t4.Position.x = 139;t4.Position.y = 50;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 58;t1.Position.y = 50;t2.Position.x = 59;t2.Position.y = 50;t3.Position.x = 59;t3.Position.y = 51;t4.Position.x = 58;t4.Position.y = 51;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 121;t1.Position.y = 50;t2.Position.x = 127;t2.Position.y = 50;t3.Position.x = 127;t3.Position.y = 51;t4.Position.x = 121;t4.Position.y = 51;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 139;t1.Position.y = 50;t2.Position.x = 149;t2.Position.y = 50;t3.Position.x = 149;t3.Position.y = 51;t4.Position.x = 139;t4.Position.y = 51;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 121;t1.Position.y = 51;t2.Position.x = 127;t2.Position.y = 51;t3.Position.x = 127;t3.Position.y = 52;t4.Position.x = 121;t4.Position.y = 52;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 139;t1.Position.y = 51;t2.Position.x = 149;t2.Position.y = 51;t3.Position.x = 149;t3.Position.y = 52;t4.Position.x = 139;t4.Position.y = 52;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 122;t1.Position.y = 52;t2.Position.x = 126;t2.Position.y = 52;t3.Position.x = 126;t3.Position.y = 53;t4.Position.x = 122;t4.Position.y = 53;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 139;t1.Position.y = 52;t2.Position.x = 149;t2.Position.y = 52;t3.Position.x = 149;t3.Position.y = 53;t4.Position.x = 139;t4.Position.y = 53;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 123;t1.Position.y = 53;t2.Position.x = 125;t2.Position.y = 53;t3.Position.x = 125;t3.Position.y = 54;t4.Position.x = 123;t4.Position.y = 54;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 139;t1.Position.y = 53;t2.Position.x = 149;t2.Position.y = 53;t3.Position.x = 149;t3.Position.y = 54;t4.Position.x = 139;t4.Position.y = 54;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 139;t1.Position.y = 54;t2.Position.x = 149;t2.Position.y = 54;t3.Position.x = 149;t3.Position.y = 55;t4.Position.x = 139;t4.Position.y = 55;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 139;t1.Position.y = 55;t2.Position.x = 148;t2.Position.y = 55;t3.Position.x = 148;t3.Position.y = 56;t4.Position.x = 139;t4.Position.y = 56;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 56;t2.Position.x = 148;t2.Position.y = 56;t3.Position.x = 148;t3.Position.y = 57;t4.Position.x = 138;t4.Position.y = 57;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 57;t2.Position.x = 148;t2.Position.y = 57;t3.Position.x = 148;t3.Position.y = 58;t4.Position.x = 138;t4.Position.y = 58;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 58;t2.Position.x = 148;t2.Position.y = 58;t3.Position.x = 148;t3.Position.y = 59;t4.Position.x = 138;t4.Position.y = 59;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 59;t2.Position.x = 148;t2.Position.y = 59;t3.Position.x = 148;t3.Position.y = 60;t4.Position.x = 138;t4.Position.y = 60;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 60;t2.Position.x = 148;t2.Position.y = 60;t3.Position.x = 148;t3.Position.y = 61;t4.Position.x = 138;t4.Position.y = 61;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 61;t2.Position.x = 147;t2.Position.y = 61;t3.Position.x = 147;t3.Position.y = 62;t4.Position.x = 138;t4.Position.y = 62;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 62;t2.Position.x = 147;t2.Position.y = 62;t3.Position.x = 147;t3.Position.y = 63;t4.Position.x = 138;t4.Position.y = 63;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 63;t2.Position.x = 147;t2.Position.y = 63;t3.Position.x = 147;t3.Position.y = 64;t4.Position.x = 137;t4.Position.y = 64;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 64;t2.Position.x = 146;t2.Position.y = 64;t3.Position.x = 146;t3.Position.y = 65;t4.Position.x = 137;t4.Position.y = 65;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 65;t2.Position.x = 146;t2.Position.y = 65;t3.Position.x = 146;t3.Position.y = 66;t4.Position.x = 137;t4.Position.y = 66;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 66;t2.Position.x = 146;t2.Position.y = 66;t3.Position.x = 146;t3.Position.y = 67;t4.Position.x = 137;t4.Position.y = 67;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 67;t2.Position.x = 146;t2.Position.y = 67;t3.Position.x = 146;t3.Position.y = 68;t4.Position.x = 137;t4.Position.y = 68;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 68;t2.Position.x = 146;t2.Position.y = 68;t3.Position.x = 146;t3.Position.y = 69;t4.Position.x = 137;t4.Position.y = 69;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 69;t2.Position.x = 146;t2.Position.y = 69;t3.Position.x = 146;t3.Position.y = 70;t4.Position.x = 137;t4.Position.y = 70;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 70;t2.Position.x = 145;t2.Position.y = 70;t3.Position.x = 145;t3.Position.y = 71;t4.Position.x = 137;t4.Position.y = 71;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 71;t2.Position.x = 145;t2.Position.y = 71;t3.Position.x = 145;t3.Position.y = 72;t4.Position.x = 137;t4.Position.y = 72;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 72;t2.Position.x = 145;t2.Position.y = 72;t3.Position.x = 145;t3.Position.y = 73;t4.Position.x = 137;t4.Position.y = 73;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 73;t2.Position.x = 144;t2.Position.y = 73;t3.Position.x = 144;t3.Position.y = 74;t4.Position.x = 137;t4.Position.y = 74;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 137;t1.Position.y = 74;t2.Position.x = 144;t2.Position.y = 74;t3.Position.x = 144;t3.Position.y = 75;t4.Position.x = 137;t4.Position.y = 75;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 75;t2.Position.x = 144;t2.Position.y = 75;t3.Position.x = 144;t3.Position.y = 76;t4.Position.x = 138;t4.Position.y = 76;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 76;t2.Position.x = 144;t2.Position.y = 76;t3.Position.x = 144;t3.Position.y = 77;t4.Position.x = 138;t4.Position.y = 77;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 138;t1.Position.y = 77;t2.Position.x = 143;t2.Position.y = 77;t3.Position.x = 143;t3.Position.y = 78;t4.Position.x = 138;t4.Position.y = 78;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );
t1.Position.x = 139;t1.Position.y = 78;t2.Position.x = 142;t2.Position.y = 78;t3.Position.x = 142;t3.Position.y = 79;t4.Position.x = 139;t4.Position.y = 79;
bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

bh.AddTriangle( t1, t2, t3, true );
bh.AddTriangle( t3, t4, t1, true );

#endif
}