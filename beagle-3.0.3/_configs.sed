s/#undef  *\([ABCDEFGHIJKLMNOPQRSTUVWXYZ_]\)/#undef BEAGLE_\1/
s/#undef  *\([abcdefghijklmnopqrstuvwxyz]\)/#undef _beagle_\1/
s/#define  *\([ABCDEFGHIJKLMNOPQRSTUVWXYZ_][abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_]*\)\(.*\)/#ifndef BEAGLE_\1 \
#define BEAGLE_\1 \2 \
#endif/
s/#define  *\([abcdefghijklmnopqrstuvwxyz][abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_]*\)\(.*\)/#ifndef _beagle_\1 \
#define _beagle_\1 \2 \
#endif/
