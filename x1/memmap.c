unsigned int pow(unsigned int base, unsigned int exponent) {
    unsigned int ans = 1;
    for (unsigned int i = 0; i < exponent; i++) {
        ans *= base;
    }
    return ans;
}

void printhex(unsigned int num,unsigned int *addr) {
	digit=8;
	print("0x");
	while (digit) {
		unsigned int weight = pow(16, digit - 1);
		unsigned int ans = num / weight;
		if (ans < 10)
			*addr |= '0' + ans;
		else
			*addr |= 'A' + ans - 10;
		num -= ans * weight;
		digit--;
	}
}

void print(char *string,unsigned int *addr) {
    do {
        *addr |= *string;
        string++;
    } while (*string != '\0');
}

void main(){
	unsigned int TTBCR,addrmask,TTBR0,TTBR1,*table0,*table1,ram=0,ahp=0;
	asm volatile(
		"mrc p15, 0, %0, c2, c0, 2\n"
		"mrc p15, 0, %1, c2, c0, 0\n"
		"mrc p15, 0, %2, c2, c0, 1\n"
		: "=r" (TTBCR),"=r" (TTBR0),"=r" (TTBR1)
	);
	addrmask=TTBCR&0x7;
	for(unsigned int i=0;i<0xffffffff;i+=4096){
		if(i>(0xffffffff>>addrmask)){
			table0=((TTBR1&0xFFFFC000)|((i>>19)<<2));
		}else{
			table0=((TTBR0&0xFFFFC000)|(((i<<addrmask)>>(19+addrmask))<<2));
		}

		if((*table0&0x3)==0)continue; //Invalid
		else if((*table0&0x3)==1){ //Page Table
			table1=((*table&0xFFFFFC00)|((i&0xFF000)>>9));
			if(*table1&0x3==1){//large page
				if(((*table&0xFFFF0000)|(i&0xFFFF))==0x60000000)ram=i;
				else if(((*table&0xFFFF0000)|(i&0xFFFF0000))==0x40000000)ahp=i;
			}else{//small page
				if(((*table&0xFFFFF000)|(i&0xFFF))==0x60000000)ram=i;
				else if(((*table&0xFFFFF000)|(i&0xFFF))==0x40000000)ahp=i;
			}
		}else if((*table0&0x40000)){ //Section
			if(((*table0&0xFFF00000)|(i&0xFFFFF))==0x60000000)ram=i;
			else if(((*table0&0xFFF00000)|(i&0xFFFFF))==0x40000000)ahp=i;
		}else continue; //Supersection
	}
	if(ahp){
		*(unsigned int *)(ahp+0x2D0000+0x8) |= 0x2 //UART4 reset
		*(unsigned int *)(ahp+0x2E0000+0x8) |= 0x2 //UART5 reset
		*(unsigned int *)(ahp+0x800000+0x260000+0x8) |= 0x2 //UART6 reset
		*(unsigned int *)(ahp+0x800000+0x270000+0x8) |= 0x2 //UART7 reset

		*(unsigned int *)(ahp+0x2D0000+0x8) &= ~0x2 //UART4 unreset
		*(unsigned int *)(ahp+0x2E0000+0x8) &= ~0x2 //UART5 unreset
		*(unsigned int *)(ahp+0x800000+0x260000+0x8) &= ~0x2 //UART6 unreset
		*(unsigned int *)(ahp+0x800000+0x270000+0x8) &= ~0x2 //UART7 unreset

		*(unsigned int *)(ahp+0x2D0000+0x18) |= 0x80000 //UART4 Tx Enable
		*(unsigned int *)(ahp+0x2E0000+0x18) |= 0x80000 //UART5 Tx Enable
		*(unsigned int *)(ahp+0x800000+0x260000+0x18) |= 0x80000 //UART6 Tx Enable
		*(unsigned int *)(ahp+0x800000+0x270000+0x18) |= 0x80000 //UART7 Tx Enable

		print("UART4=",(unsigned int *)ahp+0x2D0000+0x1C);
		printhex(ahp+0x2D0000+0x1C,(unsigned int *)ahp+0x2D0000+0x1C);
		print("\n",(unsigned int *)ahp+0x2D0000+0x1C);

		print("UART5=",(unsigned int *)ahp+0x2E0000+0x1C);
		printhex(ahp+0x2E0000+0x1C,(unsigned int *)ahp+0x2E0000+0x1C);
		print("\n",(unsigned int *)ahp+0x2E0000+0x1C);

		print("UART6=",(unsigned int *)ahp+0x800000+0x260000+0x1C);
		printhex(ahp+0x800000+0x260000+0x1C,(unsigned int *)ahp+0x800000+0x260000+0x1C);
		print("\n",(unsigned int *)ahp+0x800000+0x260000+0x1C);

		print("UART6=",(unsigned int *)ahp+0x800000+0x270000+0x1C);
		printhex(ahp+0x800000+0x270000+0x1C,(unsigned int *)ahp+0x800000+0x270000+0x1C);
		print("\n",(unsigned int *)ahp+0x800000+0x270000+0x1C);

		if(ram){
			print("DRAM=",(unsigned int *)ahp+0x2D0000+0x1C);
			printhex(ram,(unsigned int *)ahp+0x2D0000+0x1C);
			print("\n",(unsigned int *)ahp+0x2D0000+0x1C);

			print("DRAM=",(unsigned int *)ahp+0x2E0000+0x1C);
			printhex(ram,(unsigned int *)ahp+0x2E0000+0x1C);
			print("\n",(unsigned int *)ahp+0x2E0000+0x1C);

			print("DRAM=",(unsigned int *)ahp+0x800000+0x260000+0x1C);
			printhex(ram,(unsigned int *)ahp+0x800000+0x260000+0x1C);
			print("\n",(unsigned int *)ahp+0x800000+0x260000+0x1C);

			print("DRAM=",(unsigned int *)ahp+0x800000+0x270000+0x1C);
			printhex(ram,(unsigned int *)ahp+0x800000+0x270000+0x1C);
			print("\n",(unsigned int *)ahp+0x800000+0x270000+0x1C);
		}
		
	}
}
