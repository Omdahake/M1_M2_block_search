#include<stdio.h>
#include"binsearch.c"
typedef unsigned long long int u64;
typedef unsigned int u32;
typedef unsigned short int u16;
typedef unsigned char ch8;
#define len 256
#define N 4096 
#define B_size *((u16*)&m1[len - 2]) //store block size of m2
#define E_size *((u16*)&m1[len - 4]) //store elements size in each block
#define E_cnt *((u16*)&m1[len - 8])  //store total given elements count
#define m      *((u32*)&m1[len - 12]) 
#define element_in_block m1[len - 13]   //elements in one block
#define C_par m1[len - 14]   //count of partitions
#define T_B *((u16*)&m1[len - 16])  //total number of blocks
                                    //#define R32(x) (*((u32*)&m2[x]))  //Read 32 bytes 
#define R32_m1(x) (*((u32*)&m1[x]))     //Read 32 bytes 
#define L_I *((u16*)&m1[len - 18]) //Last index of current output
#define s_of_fetch_block (len-158) // startin index in m1 to fetch blocks 
#define ans_I (len - 191) //staring index in m1 to sote ans
#define ciel(x,y) ((x%y==0?x/y:(x/y)+1))
ch8 m2[N]; // m2 declaration
ch8 m1[len]; // m1 declaration
#include"set_block.c"
int main(void){
        FILE *fp;
        fp = fopen("new","r");
        fread((void *)m1,sizeof(ch8),8,fp);
        fclose(fp);

        int i = 8;
	set_block_size();
        T_B = ciel(E_cnt,element_in_block);//total block which contains the data

        int temp = T_B;
        C_par = 0;
        //calculated the total patitions that will be required
        int sum=0;
        while(temp >1 ){
                sum = sum + ciel(temp,(element_in_block/2));
                temp = ciel(temp,(element_in_block/2));
        }
        printf("value of sum %d \n",sum);
        //L_I is index of the last block end + 1 where abstraction will be stored       
        L_I = 7 + ( T_B * B_size) + 1;
        printf("size of block is %d \n",B_size);
        printf("size of Ele is %d \n",E_size);
        printf("count of Ele is %d \n",E_cnt);
        printf("total num of Ele in one block is %d \n",element_in_block);
        printf("counts of total blocks %d \n",T_B);
        printf("counts of total  partitions %d \n",C_par);
        printf("starting index %d  \n",s_of_fetch_block);
        printf("last index is %u \n",L_I);

       temp = T_B;

	fp = fopen("new","r");
	u32 val=2;
	u32 Index=0;
	 i=1;
	 int k;
	rewind(fp); // Move to the beginning of the file
	fseek(fp,8,SEEK_SET);
	while (fread(&val, sizeof(u32), 1, fp)) {
		//printf("Index %u vale %u \n",Index, val);
		Index = Index + 1;
		if((i==1)){
			printf(" first:%d ",val);

		}else if(i==element_in_block){
			printf(" last:%d\n ",val);
			i=0;
		k++;
		}
		if((Index)==E_cnt){
			printf("\n\n new updated data\n\n");
			k=-1;
		}
		if(k == ciel(temp,(element_in_block/2))){
			printf("new partition count %d:\n\n",k);
			temp = ciel(temp,(element_in_block/2));
			k=0;
			
		}
		i++;
		
	}
	Index=0;
	fseek(fp,8,SEEK_SET);
        while (fread(&val, sizeof(u32), 1, fp)) {
                printf("Index %u vlue %u \n",Index, val);
                Index++;

        }
        fclose(fp);



}

