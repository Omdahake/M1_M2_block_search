#include<stdio.h>
typedef unsigned long long int u64;
typedef unsigned int u32;
typedef unsigned short int u16;
typedef unsigned char ch8;
#define len 2048 
//#define N 514
#define num_of_elements (513 )
#define num (num_of_elements+2)
#define R32(x) *((unsigned int*)&b[x])
#define element_size 4
#define block 32 
#define element_in_block (block/element_size)
int main(void){

	int N= (num_of_elements+(8-(num_of_elements%8) )+2) ;
	u32 a[N];
	a[0]=590341;
	a[1]=num_of_elements;
	u32 i = 2;
	u32 j=1;
	while(i < num){
		a[i]=j;
		i++;
		j++;
	}
	int k=(num_of_elements % element_in_block);
	if(k!=0){
		while(k<element_in_block){

			a[i]=0;
			i++;
			k++;
		}
		a[i-1]=(num_of_elements%element_in_block);

	}else{
		N=(num_of_elements + 2);
	}
	


	FILE *fp = fopen("new","w");
	fwrite((void *)a,sizeof(u32),N,fp);
	fclose(fp);



}
