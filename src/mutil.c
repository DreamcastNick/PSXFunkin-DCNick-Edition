/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "mutil.h"

#include "timer.h"

//Sine table
static const s16 sine_table[0x140] = {
	0,6,12,18,25,31,37,43,49,56,62,68,74,80,86,92,
	97,103,109,115,120,126,131,136,142,147,152,157,162,167,171,176,
	181,185,189,193,197,201,205,209,212,216,219,222,225,228,231,234,
	236,238,241,243,244,246,248,249,251,252,253,254,254,255,255,255,
	256,255,255,255,254,254,253,252,251,249,248,246,244,243,241,238,
	236,234,231,228,225,222,219,216,212,209,205,201,197,193,189,185,
	181,176,171,167,162,157,152,147,142,136,131,126,120,115,109,103,
	97,92,86,80,74,68,62,56,49,43,37,31,25,18,12,6,
	0,-6,-12,-18,-25,-31,-37,-43,-49,-56,-62,-68,-74,-80,-86,-92,
	-97,-103,-109,-115,-120,-126,-131,-136,-142,-147,-152,-157,-162,-167,-171,-176,
	-181,-185,-189,-193,-197,-201,-205,-209,-212,-216,-219,-222,-225,-228,-231,-234,
	-236,-238,-241,-243,-244,-246,-248,-249,-251,-252,-253,-254,-254,-255,-255,-255,
	-256,-255,-255,-255,-254,-254,-253,-252,-251,-249,-248,-246,-244,-243,-241,-238,
	-236,-234,-231,-228,-225,-222,-219,-216,-212,-209,-205,-201,-197,-193,-189,-185,
	-181,-176,-171,-167,-162,-157,-152,-147,-142,-136,-131,-126,-120,-115,-109,-103,
	-97,-92,-86,-80,-74,-68,-62,-56,-49,-43,-37,-31,-25,-18,-12,-6,
	0,6,12,18,25,31,37,43,49,56,62,68,74,80,86,92,
	97,103,109,115,120,126,131,136,142,147,152,157,162,167,171,176,
	181,185,189,193,197,201,205,209,212,216,219,222,225,228,231,234,
	236,238,241,243,244,246,248,249,251,252,253,254,254,255,255,255,
};

static const s16 smooth_table[256] = {
	24 ,48 ,73 ,97 ,122 ,146 ,170 ,194 ,218 ,242 ,266 ,289 ,313 ,336 ,359 ,382 ,404 ,426 ,448 ,470 ,492 ,513 ,534 ,554 ,574 ,594 ,614 ,633 ,652 ,670 ,688 ,706 ,723 ,739 ,756 ,772 ,787 ,802 ,816 ,830 ,843 ,856 ,869 ,881 ,892 ,903 ,913 ,923 ,932 ,940 ,948 ,956 ,963 ,969 ,975 ,980 ,984 ,988 ,992 ,994 ,997 ,998 ,999 ,999 ,999 ,998 ,997 ,995 ,992 ,989 ,985 ,981 ,976 ,970 ,964 ,957 ,950 ,942 ,934 ,925 ,915 ,905 ,894 ,883 ,871 ,859 ,846 ,833 ,819 ,805 ,790 ,775 ,759 ,743 ,727 ,710 ,692 ,674 ,656 ,637 ,618 ,599 ,579 ,559 ,538 ,518 ,496 ,475 ,453 ,431 ,409 ,387 ,364 ,341 ,318 ,295 ,271 ,247 ,224 ,200 ,176 ,152 ,127 ,103 ,79 ,54 ,30 ,5 ,-18 ,-43 ,-67 ,-92 ,-116 ,-140 ,-165 ,-189 ,-213 ,-237 ,-260 ,-284 ,-307 ,-331 ,-354 ,-376 ,-399 ,-421 ,-443 ,-465 ,-487 ,-508 ,-529 ,-550 ,-570 ,-590 ,-609 ,-629 ,-647 ,-666 ,-684 ,-702 ,-719 ,-736 ,-752 ,-768 ,-783 ,-798 ,-813 ,-827 ,-840 ,-853 ,-866 ,-878 ,-889 ,-900 ,-911 ,-920 ,-930 ,-938 ,-947 ,-954 ,-961 ,-968 ,-973 ,-979 ,-983 ,-987 ,-991 ,-994 ,-996 ,-998 ,-999 ,-999 ,-999 ,-999 ,-997 ,-995 ,-993 ,-990 ,-986 ,-982 ,-977 ,-972 ,-966 ,-959 ,-952 ,-944 ,-936 ,-927 ,-917 ,-907 ,-897 ,-886 ,-874 ,-862 ,-849 ,-836 ,-823 ,-808 ,-794 ,-779 ,-763 ,-747 ,-730 ,-713 ,-696 ,-678 ,-660 ,-642 ,-623 ,-603 ,-584 ,-563 ,-543 ,-522 ,-501 ,-480 ,-458 ,-436 ,-414 ,-392 ,-369 ,-346 ,-323 ,-300 ,-277 ,-253 ,-229 ,-205 ,-181 ,-157 ,-133 ,-108 ,-84 ,-60 ,-35 ,-11  
};

//Math utility functions
s16 MUtil_Sin(u8 x)
{
	return sine_table[x];
}

s16 MUtil_Cos(u8 x)
{
	return (sine_table + 0x40)[x];
}

s16 smooth(u8 x)
{
	return smooth_table[x];
}

fixed_t lerp(fixed_t current, fixed_t target, fixed_t speed) {
    fixed_t new_position = current + FIXED_MUL((target - current), speed);
    return new_position;
}

void MUtil_RotatePoint(POINT *p, s16 s, s16 c)
{
	short px = p->x;
	short py = p->y;
	p->x = ((px * c) >> 8) - ((py * s) >> 8);
	p->y = ((px * s) >> 8) + ((py * c) >> 8);
}
