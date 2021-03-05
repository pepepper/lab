/*
 * ResetKitHelper
 * Soft/hard reset the electronic dictionary.
 * 
 * Copyright (C) 2012 T. Kawada <tcppjp [ at ] gmail.com>
 *
 * This file is licensed in MIT license.
 *
 * Permission is hereby granted, free of charge, to any person obtaining 
 * a copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 *
 */

#include <windows.h>
#include <stdint.h>

#define RESETKITHELPER_API __declspec(dllexport)

#include "ResetKitHelper.h"

#pragma mark - EDNA2 Specific

// only for 0x80000000 - 0x80100000
#define EDNA2Register(addr) (*(volatile uint32_t *)((addr) + 0x30000000))

unsigned int pow(unsigned int base, unsigned int exponent) {
    unsigned int ans = 1;
    for (unsigned int i = 0; i < exponent; i++) {
        ans *= base;
    }
    return ans;
}

void print(char *string,unsigned int *addr) {
    do {
        *addr |= *string;
        string++;
    } while (*string != '\0');
}

unsigned int div(unsigned int dividend,unsigned int divisor){
	unsigned int ans;
	ans=0;
	while(dividend>divisor){
		dividend-=divisor;
		ans++;
	}
	return ans;
}

void printhex(unsigned int num,unsigned int *addr) {
	int digit=8;
	print("0x",addr);
	while (digit) {
		unsigned int weight = pow(16, digit - 1);
		unsigned int ans = div(num,weight);
		if (ans < 10)
			*addr |= '0' + ans;
		else
			*addr |= 'A' + ans - 10;
		num -= ans * weight;
		digit--;
	}
}

extern "C" RESETKITHELPER_API BOOL RKH_IOControl(DWORD handle, DWORD dwIoControlCode, DWORD *pInBuf, DWORD nInBufSize, DWORD *pOutBuf, DWORD nOutBufSize,
												 PDWORD pBytesReturned){
	SetLastError(0);
	switch (dwIoControlCode){
	case IOCTL_RKH_DO_SOFT_RESET:
		wchar_t buf[256];
		unsigned int TTBCR,addrmask,TTBR0,TTBR1,*table0,*table1,ram=0,ahp=0,descriptor;
		asm volatile(
			"msr	cpsr_c, #211\n"	// to supervisor mode
			"mrs	r0, cpsr\n"
			"orr	r0,r0,#0x80\n"
			"msr	cpsr_c,r0\n"	//interrupt disable
			"mov	r0,#1\n"
			//"mrc	p15,0,r9,c1,c0,0\n" // read ctrl regs
			//"bic	r9, r9, #4\n" // disable DCache
			//"bic	r9, r9, #4096\n" // disable ICache
			//"mcr	p15,0,r9,c1,c0,0\n" // write ctrl regs
			//"mov	r9, #0\n"
			//"mcr	p15,0,r9,c7,c7,0\n" // invalidate cache
			//"mcr	p15,0,r9,c8,c7,0\n" // invalidate tlb
			:::"r0"
		);
		OutputDebugString(L"stop interrupt\r\n");
		asm volatile(
			"mrc p15, 0, %1, c2, c0, 0\n"
			: "=r" (TTBCR),"=r" (TTBR0),"=r" (TTBR1)
		);
		swprintf(buf, L"TTBR=%#x\r\n", TTBR0);
		OutputDebugString(buf);
		for(unsigned int i=0;i<0xffffffff;i+=4096){
			table0=(unsigned int *)((TTBR0&0xFFFFC000)|((i>>19)<<2));
			swprintf(buf, L"table0 addr=%#x\r\n", table0);
			OutputDebugString(buf);
			asm volatile(
				"mrc	p15,0,r10,c1,c0,0\n" // read ctrl regs
				"mov	r9,r10\n"
				"bic	r10, r10, #1\n"
				"mcr	p15,0,r10,c1,c0,0\n"// disable MMU
				"ldr	%0, [%1]\n"
				"mcr	p15,0,r9,c1,c0,0\n" // enable MMU
				"nop\n"
				"nop\n"
				"nop\n"
				"nop\n"
				"nop\n"
				"nop\n"
				"nop\n"
				"nop\n"
				"nop\n"
				"nop\n"
				"nop\n"
				"nop\n"
				"nop\n"
				"nop\n"
				"nop\n"
				:"=r" (descriptor): "r" (table0):"r10","r9","r8"
			);
			swprintf(buf, L"table0=%#x\r\n", descriptor);
			OutputDebugString(buf);
			if((descriptor&0x3)==0)continue; //Invalid
			else if((descriptor&0x3)==1){ //Page Table
				table1=(unsigned int *)((descriptor&0xFFFFFC00)|((i&0xFF000)>>9));
				swprintf(buf, L"table1 addr=%#x\r\n", table1);
				OutputDebugString(buf);
				asm volatile(
					"mrc	p15,0,r10,c1,c0,0\n" // read ctrl regs
					"mov	r9,r10\n"
					"bic	r10, r10, #1\n"
					"mcr	p15,0,r10,c1,c0,0\n"// disable MMU
					"ldr	%0, [%1]\n"
					"mcr	p15,0,r9,c1,c0,0\n" // enable MMU
					"nop\n"
					"nop\n"
					"nop\n"
					"nop\n"
					"nop\n"
					"nop\n"
					"nop\n"
					"nop\n"
					"nop\n"
					"nop\n"
					"nop\n"
					"nop\n"
					"nop\n"
					"nop\n"
					"nop\n"
				: "=r" (table1):"r" (descriptor):"r10","r9","r8"
				);
				if(descriptor&0x3==1){//large page
					if(((descriptor&0xFFFF0000)|(i&0xFFFF))==0x40000000)ram=i;
					else if(((descriptor&0xFFFF0000)|(i&0xFFFF0000))==0x80070000)ahp=i;
				}else{//small page
					if(((descriptor&0xFFFFF000)|(i&0xFFF))==0x40000000)ram=i;
					else if(((descriptor&0xFFFFF000)|(i&0xFFF))==0x80070000)ahp=i;
				}
			}else if((descriptor&0x40000)){ //Section
				if(((descriptor&0xFFF00000)|(i&0xFFFFF))==0x40000000)ram=i;
				else if(((descriptor&0xFFF00000)|(i&0xFFFFF))==0x80070000)ahp=i;
			}else continue; //Supersection
		}

		swprintf(buf, L"RAM=%#x\r\nNear UART=%#x\r\n", ram, ahp);
		OutputDebugString(buf);
		/*
		if(ahp){
			*((unsigned int *)(ahp+0x4000+0x30)) |= 0x101; //UART Tx Enable
			*((unsigned int *)(ahp+0x4000+0x38)) &= 0x0; //UART Intr disable
			print("DUART=",(unsigned int *)ahp+0x4000);
			printhex(ahp+0x2D0000+0x1C,(unsigned int *)ahp+0x4000);
			print("\n",(unsigned int *)ahp+0x4000);
	
			if(ram){
				print("DRAM=",(unsigned int *)ahp+0x4000);
				printhex(ram,(unsigned int *)ahp+0x4000);
				print("\n",(unsigned int *)ahp+0x4000);
			}
		
		}
		*/
		while(1);
	}
	return FALSE;
}

extern "C" RESETKITHELPER_API BOOL RKH_Read(DWORD handle, LPVOID pBuffer, DWORD dwNumBytes)
{
	SetLastError(ERROR_INVALID_FUNCTION);
	return FALSE;
}

extern "C" RESETKITHELPER_API BOOL RKH_Write(DWORD handle, LPVOID pBuffer, DWORD dwNumBytes)
{
	SetLastError(ERROR_INVALID_FUNCTION);
	return FALSE;
}

extern "C" RESETKITHELPER_API DWORD RKH_Seek(DWORD handle, long lDistance, DWORD dwMoveMethod)
{
	SetLastError(ERROR_INVALID_FUNCTION);
	return FALSE;
}

extern "C" RESETKITHELPER_API void RKH_PowerUp(void)
{
	OutputDebugString(L"ResetKit: resuming.");
}

extern "C" RESETKITHELPER_API void RKH_PowerDown(void)
{
}

extern "C" RESETKITHELPER_API DWORD RKH_Init(LPCTSTR pContext, DWORD dwBusContext)
{

	void *ctx;
	ctx = (void *)LocalAlloc(LPTR, sizeof(4));

	return (DWORD)ctx;
}

extern "C" RESETKITHELPER_API DWORD RKH_Open(DWORD dwData, DWORD dwAccess, DWORD dwShareMode)
{

	//SetProcPermissions(-1);

	void *hnd = (void *)LocalAlloc(LPTR, 4);
	return (DWORD)hnd;
}

extern "C" RESETKITHELPER_API BOOL RKH_Close(DWORD handle)
{
	LocalFree((void *)handle);

	return TRUE;
}

extern "C" RESETKITHELPER_API BOOL RKH_Deinit(DWORD dwContext)
{

	LocalFree((void *)dwContext);
	return TRUE;
}

extern "C" BOOL APIENTRY DllMainCRTStartup(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
