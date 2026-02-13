#include<stdio.h>
#include<stddef.h>

struct Block{
	size_t size;
	int free;
	struct Block *next;
};
char memory_pool[1024];
struct Block *freeList = (struct Block*)memory_pool;  //LÝSTENÝN EN BAÞINDAKÝ BLOÐU TUTAN ÝÞARETÇÝ

void initialize(){
	//ÝLK BLOÐUN BOYUTU
	freeList->size = 1024 - sizeof(struct Block);  //TOPLAM ALAN-BLOÐUN ALANI
	freeList->free = 1;  //BAÞTA HER YER BOÞ
	freeList->next=NULL;
}
void split(struct Block *fitting_slot, size_t size){
	struct Block *new_block = (struct Block*)((char*)fitting_slot + sizeof(struct Block) + size);
	
	new_block->size = fitting_slot->size - size - sizeof(struct Block);
	new_block->free = 1;
	new_block->next = fitting_slot->next;
	 //ESKÝ BLOÐU ÝSTENEN BOYUTA ÝNDÝR VE YENÝ BLOÐA BAÐLA
	fitting_slot->size = size;
	fitting_slot->free = 0;
	fitting_slot->next = new_block;
}
void *my_malloc(size_t size){
	struct Block *curr = freeList;
	
	while(curr != NULL){
		if(curr->free && curr->size >= size){//EÐER BLOK BOÞSA VE ÝSTENEN BOYUTTAN BÜYÜK VEYA EÞÝTSE
			if(curr->size > (size + sizeof(struct Block))){
				split(curr,size);
				printf("%zu byte yer ayrildi ve kalan bellek yeni blok olarak bolundu.\n",size);
			}else{
			curr->free =0; //ARTK DOLU
			printf("%zu byte yer basariyla ayrildi.\n",size);
		}
			return (void*)(curr + 1); //KULLLANICIYA METADATA ADRESÝNÝ DEÐÝL,BLOKTAN HEMEN SONRA BAÞLYAN VERÝ ALANIIN ADRESÝNÝ VERÝYORUZ.
		}
		curr= curr->next;//BÝR SONRAKÝ BLOÐA BAÐLA
	}
	printf("Hata: YEterli bos bellek bulunamadi.\n");
	return NULL;
}
void my_free(void* ptr) {
    if (ptr == NULL) return;

    // Kullanýcýnýn verdiði adres verinin baþladýðý yer. 
    // Metadata'ya (tabelaya) ulaþmak için bir adým geri gitmeliyiz.
    struct Block* curr = (struct Block*)ptr - 1;
    curr->free = 1;
    printf("Bellek serbest birakildi.\n");

    // YAN YANA BOÞ BLOKLARI BÝRLEÞTÝRME
    struct Block* temp = freeList;
    while (temp != NULL && temp->next != NULL) {
        if (temp->free && temp->next->free) {
            temp->size += (sizeof(struct Block) + temp->next->size);
            temp->next = temp->next->next;
            printf("Iki bos blok birlestirildi.\n");
        }
        temp = temp->next;
    }
}
int main() {
    initialize();

    printf("--- Yer Ayirma ---\n");
    int* p1 = (int*)my_malloc(100);
    int* p2 = (int*)my_malloc(200);

    printf("\n--- Bellek Bosaltma ---\n");
    my_free(p1); // p1'i serbest býraktýk
    my_free(p2); // p2'yi serbest býraktýk 

    printf("\n--- Tekrar Yer Ayirma (Birlestirme Testi) ---\n");

    void* p3 = my_malloc(300);

    return 0;
}




