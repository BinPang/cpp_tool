long multi2(long, long);

void multstore(long x, long y, long *dest) {
	long t = multi2(x, y);
	*dest = t;
}
