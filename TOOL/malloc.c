#include "malloc.h"	   

vu32 MemUsed = 0;

//�ڴ��(32�ֽڶ���)
u8 mem1base[MEM1_MAX_SIZE];																//�ڲ�SRAM�ڴ��
//�ڴ������
u32 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];																//�ڲ�SRAM�ڴ��MAP
//�ڴ��������	   
const u32 memtblsize=MEM1_ALLOC_TABLE_SIZE;	//�ڴ����С
const u32 memblksize=MEM1_BLOCK_SIZE;					//�ڴ�ֿ��С
const u32 memsize=MEM1_MAX_SIZE;							//�ڴ��ܴ�С

//�ڴ����������
struct _m_mallco_dev mallco_dev=
{
	my_mem_init,						//�ڴ��ʼ��
	my_mem_perused,						//�ڴ�ʹ����
	mem1base,			//�ڴ��
	mem1mapbase,//�ڴ����״̬��
	0,  		 					//�ڴ����δ����
};

//�����ڴ�
//*des:Ŀ�ĵ�ַ
//*src:Դ��ַ
//n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
void mymemcpy(void *des,void *src,u32 n)  
{  
    u8 *xdes=des;
	u8 *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  
//�����ڴ�
//*s:�ڴ��׵�ַ
//c :Ҫ���õ�ֵ
//count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
void mymemset(void *s,u8 c,u32 count)  
{  
    u8 *xs = s;  
    while(count--)*xs++=c;  
}	
//�ڴ������ʼ��  
//memx:�����ڴ��
void my_mem_init(void)  
{  
    mymemset(mallco_dev.memmap,0,memtblsize*4);	//�ڴ�״̬����������  
 	mallco_dev.memrdy=1;								//�ڴ������ʼ��OK  
}  
//��ȡ�ڴ�ʹ����
//memx:�����ڴ��
//����ֵ:ʹ����(������10��,0~1000,����0.0%~100.0%)
u32 my_mem_perused(void) 
{  
    return MemUsed;  
}  
//�ڴ����(�ڲ�����)
//memx:�����ڴ��
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:0XFFFFFFFF,��������;����,�ڴ�ƫ�Ƶ�ַ 
u32 my_mem_malloc(u32 size)  
{  
    signed long offset=0;  
    u32 nmemb;	//��Ҫ���ڴ����  
	u32 cmemb=0;//�������ڴ����
    u32 i;  
    if(!mallco_dev.memrdy)mallco_dev.init();//δ��ʼ��,��ִ�г�ʼ�� 
    if(size==0)return 0XFFFFFFFF;//����Ҫ����
    nmemb=size/memblksize;  	//��ȡ��Ҫ����������ڴ����
    if(size%memblksize)nmemb++;  
	MemUsed += nmemb * MEM1_BLOCK_SIZE;
    for(offset=memtblsize-1;offset>=0;offset--)//���������ڴ������  
    {     
		if(!mallco_dev.memmap[offset])cmemb++;//�������ڴ��������
		else cmemb=0;								//�����ڴ������
		if(cmemb==nmemb)							//�ҵ�������nmemb�����ڴ��
		{
            for(i=0;i<nmemb;i++)  					//��ע�ڴ��ǿ� 
                mallco_dev.memmap[offset+i]=nmemb;    
            return (offset*memblksize);//����ƫ�Ƶ�ַ  
		}
    }  
    return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
}  
//�ͷ��ڴ�(�ڲ�����) 
//memx:�����ڴ��
//offset:�ڴ��ַƫ��
//����ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;  
u8 my_mem_free(u32 offset)  
{  
    int i;  
    if(!mallco_dev.memrdy)//δ��ʼ��,��ִ�г�ʼ��
	{
		mallco_dev.init();    
        return 1;//δ��ʼ��  
    }  
    if(offset<memsize)//ƫ�����ڴ����. 
    {  
        int index=offset/memblksize;			//ƫ�������ڴ�����  
        int nmemb=mallco_dev.memmap[index];	//�ڴ������
		MemUsed -= nmemb * MEM1_BLOCK_SIZE;
        for(i=0;i<nmemb;i++)  						//�ڴ������
            mallco_dev.memmap[index+i]=0;   
        return 0;  
    }else return 2;//ƫ�Ƴ�����.  
}
//�ͷ��ڴ�(�ⲿ����) 
//memx:�����ڴ��
//ptr:�ڴ��׵�ַ 
void myfree(void *ptr)  
{  
	u32 offset;   
	if(ptr==NULL)return;//��ַΪ0.  
 	offset=(u32)ptr-(u32)mallco_dev.membase;     
    my_mem_free(offset);	//�ͷ��ڴ�    
	ptr = NULL;
}  
//�����ڴ�(�ⲿ����)
//memx:�����ڴ��
//size:�ڴ��С(�ֽ�)
//����ֵ:���䵽���ڴ��׵�ַ.
void *mymalloc(u32 size)  
{  
    u32 offset;   
	offset=my_mem_malloc(size);  	   	 	   
    if(offset==0XFFFFFFFF)return NULL;  
    else 
	{
		//mymemset((void*)((u32)mallco_dev.membase[memx]+offset),0,size);
		return (void*)((u32)mallco_dev.membase+offset);  
	}
}  
//���·����ڴ�(�ⲿ����)
//memx:�����ڴ��
//*ptr:���ڴ��׵�ַ
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:�·��䵽���ڴ��׵�ַ.
void *myrealloc(void *ptr,u32 size)  
{  
    u32 offset;    
    offset=my_mem_malloc(size);   	
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((u32)mallco_dev.membase+offset),ptr,size);	//�������ڴ����ݵ����ڴ�   
        myfree(ptr);  											  		//�ͷž��ڴ�
        return (void*)((u32)mallco_dev.membase+offset);  				//�������ڴ��׵�ַ
    }  
}
