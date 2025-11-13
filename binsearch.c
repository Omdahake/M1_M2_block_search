#include<stdio.h>
typedef unsigned int u32;
#define R32(x) *((u32*)&mem[x])
#define e_size 4

int found;
int ans;
//Using binary search on fetch block
int binsearch(unsigned char *mem,int high,int key){
	int low =0;
	int mid = (low + high)/2;
	found = -1;
	ans = -1;
	while( low != (high)){
		mid = (low + high)/2;
		if(key > R32((mid*e_size) ) && key < R32(((mid+1)*e_size))){
			found = mid;
			low = mid;high = mid;

		}else if(key < R32((mid*e_size))){
			high = mid;
		}else if(key > R32((mid*e_size))){
			low = mid+1;
		}else{
			ans = mid;
			low = mid;high = mid;
		}
	}
	if(found != -1 ){
		return found;
	}else{
		return low;
	}
}
