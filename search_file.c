#include<stdio.h>
#include<stdlib.h>
#include"binsearch.c"
typedef unsigned long long int u64;
typedef unsigned int u32;
typedef unsigned short int u16;
typedef unsigned char ch8;
#define len 1024
#define N 4096 
#define B_size *((u16*)&m1[len - 2]) //store block size of m2
#define E_size *((u16*)&m1[len - 4]) //store elements size in each block
#define E_cnt *((u16*)&m1[len - 8])  //store total given elements count
#define m      *((u32*)&m1[len - 12]) 
#define element_in_block m1[len - 13]   //elements in one block
#define C_par m1[len - 14]   //count of partitions
#define T_B *((u16*)&m1[len - 16])  //total number of blocks
                    //#define R32(x) (*((u32*)&m2[x]))     //Read 32 bytes 
#define R32_m1(x) (*((u32*)&m1[x]))     //Read 32 bytes 
#define L_I *((u16*)&m1[len - 18]) //Last index of current output
                   //#define Cap_B 4    //capacity of the block 
#define s_of_fetch_block (len-158) // startin index in m1 to fetch blocks 
#define ans_I (len - 288) //staring index in m1 to sote ans
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
    if(E_cnt==1)sum=1;
    printf("value of sum %d \n",sum);
    //L_I is index of the last block end + 1 where abstraction will be stored    
    L_I = 7 + ( T_B * B_size) + 1;
    printf("size of block is %d \n",B_size);
    printf("size of Ele is %d \n",E_size);
    printf("count of Ele is %d \n",E_cnt);
    printf("total num of Ele in one block is %d \n",element_in_block);
    printf("counts of total blocks %d \n",T_B);
    printf("counts of total  partitions %d \n",C_par);
    printf("starting index of fetch %d  \n",s_of_fetch_block);
    printf("starting index of ans_I %d  \n",ans_I);
    printf("last index is %u \n",L_I);       
    m=8;
    int j=0;
    FILE *m2_file = fopen("new","r+");

    int fetch_pointer = 8 ; //index to read from the m2
    fseek(m2_file,fetch_pointer ,SEEK_SET);    
    fread((void *)&m1[s_of_fetch_block],E_size,(element_in_block * Cap_B ),m2_file);
    int r_cnt = E_cnt;
    int w_cnt = 0;
    int flag = 0;
    i = ans_I;
    j = s_of_fetch_block;
    while(sum != 0 && E_cnt!=0){

        if(j >= (s_of_fetch_block + (Cap_B*B_size)) || r_cnt == 0){
            //fetch the new blocks to write
	    if(r_cnt ==0){
		r_cnt = w_cnt ;
		w_cnt = 0;
	    }	
            if(flag == 0){
                //fetch_pointer = fetch_pointer +(E_size * element_in_block * Cap_B);
                fetch_pointer = fetch_pointer + (j-s_of_fetch_block);
                fseek(m2_file,fetch_pointer ,SEEK_SET);
                fread((void *)&m1[s_of_fetch_block],E_size,(element_in_block * Cap_B ),m2_file);
            }else{
                flag = 0;
                fseek(m2_file,fetch_pointer,SEEK_SET);
                fread((void *)&m1[s_of_fetch_block],E_size,(element_in_block * Cap_B),m2_file);
            }
           // i = ans_I;
            j = s_of_fetch_block;

        }
        if(i < (ans_I + (B_size)) && r_cnt >= element_in_block){
           //storing the first and the last element from fetch block inside the m1 memory
	   R32_m1(i) = R32_m1(j);
            i = i + E_size;
            j = (j + (E_size * (element_in_block - 1)));
            R32_m1(i) = R32_m1(j);
            i = i + E_size;
            j = j + E_size;
            r_cnt = r_cnt - element_in_block;
            w_cnt += 2;
        }
	if(i < (ans_I + (B_size)) && r_cnt < element_in_block && r_cnt != 0 && j < (s_of_fetch_block+(Cap_B*B_size))){

            // if in last box less element_in_block elements comes store there first and the last
            R32_m1(i) = R32_m1(j);
            i = i + E_size;j = j + E_size;r_cnt -= 1;w_cnt += 1;
            while(r_cnt > 1){
                j = j + E_size;
                r_cnt -= 1;
            }
            if(r_cnt == 1){
                R32_m1(i) = R32_m1(j);
                i+=E_size;j+=E_size;r_cnt-=1;w_cnt += 1;
            }
	    int t=(i-(ans_I))/E_size;
            while(i < (ans_I+B_size)-E_size)
            {
                R32_m1(i)=0;
                i += E_size;


            }
	    //Storing the count of element in the block last element position is less than block size element exitst
	    R32_m1(i)=t;
	    i+=E_size;
	    
            fetch_pointer = fetch_pointer +( B_size*((j-s_of_fetch_block)/B_size + 1 )) ;
            flag = 1;


        }
        if(i >= (ans_I + (B_size))||r_cnt ==0){
            //now ans block is full so writing it at end of the file
            fseek(m2_file , 0 , SEEK_END);
            fwrite((void *)&m1[ans_I],E_size,element_in_block,m2_file);
	    i = ans_I;
            sum = sum - 1;

        }


    }

////////////////////////////////////////////////////////////Searching logic ///////////////////////////////////////////////

int file=0;
while(file<=E_cnt){
	temp=T_B;
        int c_p=0;
	int num=rand()%600;
                while(temp>1){

                        c_p++;
                        temp =ciel(temp,(element_in_block/2));
                }
	if(E_cnt==0)c_p = 1;
         i=0;
        int catch_i=0;
	int s_p =0;
int element=E_cnt % element_in_block; //set initial element value
        while(c_p+1 != 0 && E_cnt !=0){
		element = element_in_block;
		temp = c_p ; s_p = T_B;
		int block = T_B;
		int l=0;
			l=T_B;
			l=(ciel(block,(element_in_block/2)));
		//setting start point to fetch box;
		while(temp>1){
			block = ciel(block,(element_in_block/2));
			s_p = s_p +block ;
			l=(ciel(block,(element_in_block/2)));
			temp-=1;//terminate

		}
		//set element to tell binsearch number of element
		if(c_p == 0){
			s_p = 0;
			l = T_B;
		}
                fseek(m2_file,(s_p*B_size+8)+(i*B_size),SEEK_SET);
                fread((void *)&m1[s_of_fetch_block],E_size,element_in_block,m2_file);
		if((i+1)==l){
			element=m1[s_of_fetch_block+ (element_in_block-1)*E_size];
			
		}
                catch_i = binsearch(&m1[s_of_fetch_block],element,num);
		i =(i*(element_in_block/2) + (catch_i/2)) ;
                c_p -= 1;

                }

	if(ans==-1 && E_cnt !=0){ //ans is a name of flag name in binsearch
		printf("%d 0\n",num);
	}else if(E_cnt!=0){
		printf("%d 1\n",num);
	}
	file +=1;
}

}
