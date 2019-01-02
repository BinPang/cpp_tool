#include<stdio.h>
#include<stdlib.h>


//gcc -shared -o libvis.so -fvisibility=hidden vis.c
//readelf -s libvis.so |grep hidden
__attribute ((visibility("default"))) void not_hidden (){
	printf("exported symbol\n");
}

void is_hidden (){
	printf("hidden one\n");
}
