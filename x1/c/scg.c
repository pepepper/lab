// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2016 Freescale Semiconductor, Inc.
 */

#include "common.h"
//#include <div64.h>
//#include <asm/io.h>
//#include <errno.h>
#include "imx-regs.h"
#include "pcc.h"
//#include <asm/arch/sys_proto.h>

scg_p scg1_regs = (scg_p)SCG1_RBASE;

////////////////////////////////////////////////////////////////
static u32 scg_src_get_rate(enum scg_clk clksrc)
{
	u32 reg;

	switch (clksrc) {
	case SCG_SOSC_CLK:
		reg = readl(&scg1_regs->sosccsr);
		if (!(reg & SCG_SOSC_CSR_SOSCVLD_MASK))
			return 0;

		return 24000000;
	case SCG_FIRC_CLK:
		reg = readl(&scg1_regs->firccsr);
		if (!(reg & SCG_FIRC_CSR_FIRCVLD_MASK))
			return 0;

		return 48000000;
	case SCG_SIRC_CLK:
		reg = readl(&scg1_regs->sirccsr);
		if (!(reg & SCG_SIRC_CSR_SIRCVLD_MASK))
			return 0;

		return 16000000;
	case SCG_ROSC_CLK:
		reg = readl(&scg1_regs->rtccsr);
		if (!(reg & SCG_ROSC_CSR_ROSCVLD_MASK))
			return 0;

		return 32768;
	default:
		break;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
static u32 scg_sircdiv_get_rate(enum scg_clk clk)
{
	u32 reg, val, rate;
	u32 shift, mask;

	switch (clk) {
	case SCG_SIRC_DIV1_CLK:
		mask = SCG_SIRCDIV_DIV1_MASK;
		shift = SCG_SIRCDIV_DIV1_SHIFT;
		break;
	case SCG_SIRC_DIV2_CLK:
		mask = SCG_SIRCDIV_DIV2_MASK;
		shift = SCG_SIRCDIV_DIV2_SHIFT;
		break;
	case SCG_SIRC_DIV3_CLK:
		mask = SCG_SIRCDIV_DIV3_MASK;
		shift = SCG_SIRCDIV_DIV3_SHIFT;
		break;
	default:
		return 0;
	}

	reg = readl(&scg1_regs->sirccsr);
	if (!(reg & SCG_SIRC_CSR_SIRCVLD_MASK))
		return 0;

	reg = readl(&scg1_regs->sircdiv);
	val = (reg & mask) >> shift;

	if (!val) /*clock disabled*/
		return 0;

	rate = scg_src_get_rate(SCG_SIRC_CLK);
	rate = rate / (1 << (val - 1));

	return rate;
}

////////////////////////////////////////////////////////////////
static u32 scg_fircdiv_get_rate(enum scg_clk clk)
{
	u32 reg, val, rate;
	u32 shift, mask;

	switch (clk) {
	case SCG_FIRC_DIV1_CLK:
		mask = SCG_FIRCDIV_DIV1_MASK;
		shift = SCG_FIRCDIV_DIV1_SHIFT;
		break;
	case SCG_FIRC_DIV2_CLK:
		mask = SCG_FIRCDIV_DIV2_MASK;
		shift = SCG_FIRCDIV_DIV2_SHIFT;
		break;
	case SCG_FIRC_DIV3_CLK:
		mask = SCG_FIRCDIV_DIV3_MASK;
		shift = SCG_FIRCDIV_DIV3_SHIFT;
		break;
	default:
		return 0;
	}

	reg = readl(&scg1_regs->firccsr);
	if (!(reg & SCG_FIRC_CSR_FIRCVLD_MASK))
		return 0;

	reg = readl(&scg1_regs->fircdiv);
	val = (reg & mask) >> shift;

	if (!val) /*clock disabled*/
		return 0;

	rate = scg_src_get_rate(SCG_FIRC_CLK);
	rate = rate / (1 << (val - 1));

	return rate;
}

///////////////////////////////////////////////////////////////////////
static u32 scg_soscdiv_get_rate(enum scg_clk clk)
{
	u32 reg, val, rate;
	u32 shift, mask;

	switch (clk) {
	case SCG_SOSC_DIV1_CLK:
		mask = SCG_SOSCDIV_DIV1_MASK;
		shift = SCG_SOSCDIV_DIV1_SHIFT;
		break;
	case SCG_SOSC_DIV2_CLK:
		mask = SCG_SOSCDIV_DIV2_MASK;
		shift = SCG_SOSCDIV_DIV2_SHIFT;
		break;
	case SCG_SOSC_DIV3_CLK:
		mask = SCG_SOSCDIV_DIV3_MASK;
		shift = SCG_SOSCDIV_DIV3_SHIFT;
		break;
	default:
		return 0;
	}

	reg = readl(&scg1_regs->sosccsr);
	if (!(reg & SCG_SOSC_CSR_SOSCVLD_MASK))
		return 0;

	reg = readl(&scg1_regs->soscdiv);
	val = (reg & mask) >> shift;

	if (!val) /*clock disabled*/
		return 0;

	rate = scg_src_get_rate(SCG_SOSC_CLK);
	rate = rate / (1 << (val - 1));

	return rate;
}

///////////////////////////////////////////////////////////////
static u32 scg_apll_pfd_get_rate(enum scg_clk clk)
{
	u32 reg, val, rate;
	u32 shift, mask, gate, valid;

	switch (clk) {
	case SCG_APLL_PFD0_CLK:
		gate = SCG_PLL_PFD0_GATE_MASK;
		valid = SCG_PLL_PFD0_VALID_MASK;
		mask = SCG_PLL_PFD0_FRAC_MASK;
		shift = SCG_PLL_PFD0_FRAC_SHIFT;
		break;
	case SCG_APLL_PFD1_CLK:
		gate = SCG_PLL_PFD1_GATE_MASK;
		valid = SCG_PLL_PFD1_VALID_MASK;
		mask = SCG_PLL_PFD1_FRAC_MASK;
		shift = SCG_PLL_PFD1_FRAC_SHIFT;
		break;
	case SCG_APLL_PFD2_CLK:
		gate = SCG_PLL_PFD2_GATE_MASK;
		valid = SCG_PLL_PFD2_VALID_MASK;
		mask = SCG_PLL_PFD2_FRAC_MASK;
		shift = SCG_PLL_PFD2_FRAC_SHIFT;
		break;
	case SCG_APLL_PFD3_CLK:
		gate = SCG_PLL_PFD3_GATE_MASK;
		valid = SCG_PLL_PFD3_VALID_MASK;
		mask = SCG_PLL_PFD3_FRAC_MASK;
		shift = SCG_PLL_PFD3_FRAC_SHIFT;
		break;
	default:
		return 0;
	}

	reg = readl(&scg1_regs->apllpfd);
	if (reg & gate || !(reg & valid))
		return 0;

	clk_debug("scg_apll_pfd_get_rate reg 0x%x\n", reg);

	val = (reg & mask) >> shift;
	rate = decode_pll(PLL_A7_APLL);

	rate = rate / val * 18;

	clk_debug("scg_apll_pfd_get_rate rate %u\n", rate);

	return rate;
}

///////////////////////////////////////////////////////////////////nn
static u32 scg_spll_pfd_get_rate(enum scg_clk clk)
{
	u32 reg, val, rate;
	u32 shift, mask, gate, valid;

	switch (clk) {
	case SCG_SPLL_PFD0_CLK:
		gate = SCG_PLL_PFD0_GATE_MASK;
		valid = SCG_PLL_PFD0_VALID_MASK;
		mask = SCG_PLL_PFD0_FRAC_MASK;
		shift = SCG_PLL_PFD0_FRAC_SHIFT;
		break;
	case SCG_SPLL_PFD1_CLK:
		gate = SCG_PLL_PFD1_GATE_MASK;
		valid = SCG_PLL_PFD1_VALID_MASK;
		mask = SCG_PLL_PFD1_FRAC_MASK;
		shift = SCG_PLL_PFD1_FRAC_SHIFT;
		break;
	case SCG_SPLL_PFD2_CLK:
		gate = SCG_PLL_PFD2_GATE_MASK;
		valid = SCG_PLL_PFD2_VALID_MASK;
		mask = SCG_PLL_PFD2_FRAC_MASK;
		shift = SCG_PLL_PFD2_FRAC_SHIFT;
		break;
	case SCG_SPLL_PFD3_CLK:
		gate = SCG_PLL_PFD3_GATE_MASK;
		valid = SCG_PLL_PFD3_VALID_MASK;
		mask = SCG_PLL_PFD3_FRAC_MASK;
		shift = SCG_PLL_PFD3_FRAC_SHIFT;
		break;
	default:
		return 0;
	}

	reg = readl(&scg1_regs->spllpfd);
	if (reg & gate || !(reg & valid))
		return 0;

	clk_debug("scg_spll_pfd_get_rate reg 0x%x\n", reg);

	val = (reg & mask) >> shift;
	rate = decode_pll(PLL_A7_SPLL);

	rate = rate / val * 18;

	clk_debug("scg_spll_pfd_get_rate rate %u\n", rate);

	return rate;
}

static u32 scg_apll_get_rate(void)
{
	u32 reg, val, rate;

	reg = readl(&scg1_regs->apllcfg);
	val = (reg & SCG_PLL_CFG_PLLSEL_MASK) >> SCG_PLL_CFG_PLLSEL_SHIFT;

	if (!val) {
		/* APLL clock after two dividers */
		rate = decode_pll(PLL_A7_APLL);

		val = (reg & SCG_PLL_CFG_POSTDIV1_MASK) >>
			SCG_PLL_CFG_POSTDIV1_SHIFT;
		rate = rate / (val + 1);

		val = (reg & SCG_PLL_CFG_POSTDIV2_MASK) >>
			SCG_PLL_CFG_POSTDIV2_SHIFT;
		rate = rate / (val + 1);
	} else {
		/* APLL PFD clock */
		val = (reg & SCG_PLL_CFG_PFDSEL_MASK) >>
			SCG_PLL_CFG_PFDSEL_SHIFT;
		rate = scg_apll_pfd_get_rate(SCG_APLL_PFD0_CLK + val);
	}

	return rate;
}

static u32 scg_spll_get_rate(void)
{
	u32 reg, val, rate;

	reg = readl(&scg1_regs->spllcfg);
	val = (reg & SCG_PLL_CFG_PLLSEL_MASK) >> SCG_PLL_CFG_PLLSEL_SHIFT;

	clk_debug("scg_spll_get_rate reg 0x%x\n", reg);

	if (!val) {
		/* APLL clock after two dividers */
		rate = decode_pll(PLL_A7_SPLL);

		val = (reg & SCG_PLL_CFG_POSTDIV1_MASK) >>
			SCG_PLL_CFG_POSTDIV1_SHIFT;
		rate = rate / (val + 1);

		val = (reg & SCG_PLL_CFG_POSTDIV2_MASK) >>
			SCG_PLL_CFG_POSTDIV2_SHIFT;
		rate = rate / (val + 1);

		clk_debug("scg_spll_get_rate SPLL %u\n", rate);

	} else {
		/* APLL PFD clock */
		val = (reg & SCG_PLL_CFG_PFDSEL_MASK) >>
			SCG_PLL_CFG_PFDSEL_SHIFT;
		rate = scg_spll_pfd_get_rate(SCG_SPLL_PFD0_CLK + val);

		clk_debug("scg_spll_get_rate PFD %u\n", rate);
	}

	return rate;
}

static enum scg_clk scg_scs_array[4] = {
	SCG_SOSC_CLK, SCG_SIRC_CLK, SCG_FIRC_CLK, SCG_ROSC_CLK,
};

/////////////////////////////////////////////////////////////////////
static u32 scg_sys_get_rate(enum scg_clk clk)
{
	u32 reg, val, rate;

	if (clk != SCG_CORE_CLK && clk != SCG_BUS_CLK)
		return 0;

	reg = readl(&scg1_regs->csr);
	val = (reg & SCG_CCR_SCS_MASK) >> SCG_CCR_SCS_SHIFT;

	clk_debug("scg_sys_get_rate reg 0x%x\n", reg);

	switch (val) {
	case SCG_SCS_SYS_OSC:
	case SCG_SCS_SLOW_IRC:
	case SCG_SCS_FAST_IRC:
	case SCG_SCS_RTC_OSC:
		rate = scg_src_get_rate(scg_scs_array[val]);
		break;
	case 5:
		rate = scg_apll_get_rate();
		break;
	case 6:
		rate = scg_spll_get_rate();
		break;
	default:
		return 0;
	}

	clk_debug("scg_sys_get_rate parent rate %u\n", rate);

	val = (reg & SCG_CCR_DIVCORE_MASK) >> SCG_CCR_DIVCORE_SHIFT;

	rate = rate / (val + 1);

	if (clk == SCG_BUS_CLK) {
		val = (reg & SCG_CCR_DIVBUS_MASK) >> SCG_CCR_DIVBUS_SHIFT;
		rate = rate / (val + 1);
	}

	return rate;
}

///////////////////////////////////////////////////////////////
u32 decode_pll(enum pll_clocks pll)
{
	u32 reg,  pre_div, infreq, mult;
	u32 num, denom;

	/*
	 * Alought there are four choices for the bypass src,
	 * we choose OSC_24M which is the default set in ROM.
	 */
	switch (pll) {
	case PLL_A7_SPLL:
		reg = readl(&scg1_regs->spllcsr);

		if (!(reg & SCG_SPLL_CSR_SPLLVLD_MASK))
			return 0;

		reg = readl(&scg1_regs->spllcfg);

		pre_div = (reg & SCG_PLL_CFG_PREDIV_MASK) >>
			   SCG_PLL_CFG_PREDIV_SHIFT;
		pre_div += 1;

		mult = (reg & SCG1_SPLL_CFG_MULT_MASK) >>
			   SCG_PLL_CFG_MULT_SHIFT;

		infreq = (reg & SCG_PLL_CFG_CLKSRC_MASK) >>
			   SCG_PLL_CFG_CLKSRC_SHIFT;
		if (!infreq)
			infreq = scg_src_get_rate(SCG_SOSC_CLK);
		else
			infreq = scg_src_get_rate(SCG_FIRC_CLK);

		num = readl(&scg1_regs->spllnum);
		denom = readl(&scg1_regs->splldenom);

		infreq = infreq / pre_div;

		return infreq * mult + infreq * num / denom;

	case PLL_A7_APLL:
		reg = readl(&scg1_regs->apllcsr);

		if (!(reg & SCG_APLL_CSR_APLLVLD_MASK))
			return 0;

		reg = readl(&scg1_regs->apllcfg);

		pre_div = (reg & SCG_PLL_CFG_PREDIV_MASK) >>
			   SCG_PLL_CFG_PREDIV_SHIFT;
		pre_div += 1;

		mult = (reg & SCG_APLL_CFG_MULT_MASK) >>
			   SCG_PLL_CFG_MULT_SHIFT;

		infreq = (reg & SCG_PLL_CFG_CLKSRC_MASK) >>
			   SCG_PLL_CFG_CLKSRC_SHIFT;
		if (!infreq)
			infreq = scg_src_get_rate(SCG_SOSC_CLK);
		else
			infreq = scg_src_get_rate(SCG_FIRC_CLK);

		num = readl(&scg1_regs->apllnum);
		denom = readl(&scg1_regs->aplldenom);

		infreq = infreq / pre_div;

		return infreq * mult + infreq * num / denom;

	case PLL_USB:
		reg = readl(&scg1_regs->upllcsr);

		if (!(reg & SCG_UPLL_CSR_UPLLVLD_MASK))
			return 0;

		return 480000000u;

	case PLL_MIPI:
		return 480000000u;
	default:
		break;
	}

	return 0;
}


// //////////////////////////////////////////////////////////////////////////
u32 scg_clk_get_rate(enum scg_clk clk)
{
	switch (clk) {
	case SCG_SIRC_DIV1_CLK:
	case SCG_SIRC_DIV2_CLK:
	case SCG_SIRC_DIV3_CLK:
		return scg_sircdiv_get_rate(clk);

	case SCG_FIRC_DIV1_CLK:
	case SCG_FIRC_DIV2_CLK:
	case SCG_FIRC_DIV3_CLK:
		return scg_fircdiv_get_rate(clk);

	case SCG_SOSC_DIV1_CLK:
	case SCG_SOSC_DIV2_CLK:
	case SCG_SOSC_DIV3_CLK:
		return scg_soscdiv_get_rate(clk);

	case SCG_CORE_CLK:
	case SCG_BUS_CLK:
		return scg_sys_get_rate(clk);

	case SCG_SPLL_PFD0_CLK:
	case SCG_SPLL_PFD1_CLK:
	case SCG_SPLL_PFD2_CLK:
	case SCG_SPLL_PFD3_CLK:
		return scg_spll_pfd_get_rate(clk);

	case SCG_APLL_PFD0_CLK:
	case SCG_APLL_PFD1_CLK:
	case SCG_APLL_PFD2_CLK:
	case SCG_APLL_PFD3_CLK:
		return scg_apll_pfd_get_rate(clk);

	case USB_PLL_OUT:
		return decode_pll(PLL_USB);

	case MIPI_PLL_OUT:
		return decode_pll(PLL_MIPI);

	case SCG_SOSC_CLK:
	case SCG_FIRC_CLK:
	case SCG_SIRC_CLK:
	case SCG_ROSC_CLK:
		return scg_src_get_rate(clk);
	default:
		return 0;
	}
}

/* A7 domain system clock source is SPLL */
#define SCG1_RCCR_SCS_NUM	((SCG_SCS_SYS_PLL) << SCG_CCR_SCS_SHIFT)

/* A7 Core clck = SPLL PFD0 / 1 = 500MHz / 1 = 500MHz */
#define SCG1_RCCR_DIVCORE_NUM	((0x0)  << SCG_CCR_DIVCORE_SHIFT)
#define SCG1_RCCR_CFG_MASK	(SCG_CCR_SCS_MASK | SCG_CCR_DIVBUS_MASK)

/* A7 Plat clck = A7 Core Clock / 2 = 250MHz / 1 = 250MHz */
#define SCG1_RCCR_DIVBUS_NUM	((0x1)  << SCG_CCR_DIVBUS_SHIFT)
#define SCG1_RCCR_CFG_NUM	(SCG1_RCCR_SCS_NUM | SCG1_RCCR_DIVBUS_NUM)

/* POSTDIV2 = 1 */
#define SCG1_SPLL_CFG_POSTDIV2_NUM	((0x0)  << SCG_PLL_CFG_POSTDIV2_SHIFT)
/* POSTDIV1 = 1 */
#define SCG1_SPLL_CFG_POSTDIV1_NUM	((0x0)  << SCG_PLL_CFG_POSTDIV1_SHIFT)

/* MULT = 22 */
#define SCG1_SPLL_CFG_MULT_NUM		((22)   << SCG_PLL_CFG_MULT_SHIFT)

/* PFD0 output clock selected */
#define SCG1_SPLL_CFG_PFDSEL_NUM	((0) << SCG_PLL_CFG_PFDSEL_SHIFT)
/* PREDIV = 1 */
#define SCG1_SPLL_CFG_PREDIV_NUM	((0x0)  << SCG_PLL_CFG_PREDIV_SHIFT)
/* SPLL output clocks (including PFD outputs) selected */
#define SCG1_SPLL_CFG_BYPASS_NUM	((0x0)  << SCG_PLL_CFG_BYPASS_SHIFT)
/* SPLL PFD output clock selected */
#define SCG1_SPLL_CFG_PLLSEL_NUM	((0x1)  << SCG_PLL_CFG_PLLSEL_SHIFT)
/* Clock source is System OSC */
#define SCG1_SPLL_CFG_CLKSRC_NUM	((0x0)  << SCG_PLL_CFG_CLKSRC_SHIFT)
#define SCG1_SPLL_CFG_NUM_24M_OSC	(SCG1_SPLL_CFG_POSTDIV2_NUM	| \
					 SCG1_SPLL_CFG_POSTDIV1_NUM     | \
					 (22 << SCG_PLL_CFG_MULT_SHIFT) | \
					 SCG1_SPLL_CFG_PFDSEL_NUM       | \
					 SCG1_SPLL_CFG_PREDIV_NUM       | \
					 SCG1_SPLL_CFG_BYPASS_NUM       | \
					 SCG1_SPLL_CFG_PLLSEL_NUM       | \
					 SCG1_SPLL_CFG_CLKSRC_NUM)
/*413Mhz = A7 SPLL(528MHz) * 18/23 */
#define SCG1_SPLL_PFD0_FRAC_NUM		((23) << SCG_PLL_PFD0_FRAC_SHIFT)

/* DDR clock source is APLL PFD0 (396MHz) */
#define SCG1_DDRCCR_DDRCS_NUM		((0x0) << SCG_DDRCCR_DDRCS_SHIFT)
/* DDR clock = APLL PFD0 / 1 = 396MHz / 1 = 396MHz */
#define SCG1_DDRCCR_DDRDIV_NUM		((0x1) << SCG_DDRCCR_DDRDIV_SHIFT)
/* DDR clock = APLL PFD0 / 2 = 396MHz / 2 = 198MHz */
#define SCG1_DDRCCR_DDRDIV_LF_NUM	((0x2) << SCG_DDRCCR_DDRDIV_SHIFT)
#define SCG1_DDRCCR_CFG_NUM		(SCG1_DDRCCR_DDRCS_NUM  | \
					 SCG1_DDRCCR_DDRDIV_NUM)
#define SCG1_DDRCCR_CFG_LF_NUM		(SCG1_DDRCCR_DDRCS_NUM  | \
					 SCG1_DDRCCR_DDRDIV_LF_NUM)

/* SCG1(A7) APLLCFG configurations */
/* divide by 1 <<28 */
#define SCG1_APLL_CFG_POSTDIV2_NUM      ((0x0) << SCG_PLL_CFG_POSTDIV2_SHIFT)
/* divide by 1 <<24 */
#define SCG1_APLL_CFG_POSTDIV1_NUM      ((0x0) << SCG_PLL_CFG_POSTDIV1_SHIFT)
/* MULT is 22  <<16 */
#define SCG1_APLL_CFG_MULT_NUM          ((22)  << SCG_PLL_CFG_MULT_SHIFT)
/* PFD0 output clock selected  <<14 */
#define SCG1_APLL_CFG_PFDSEL_NUM        ((0) << SCG_PLL_CFG_PFDSEL_SHIFT)
/* PREDIV = 1	<<8 */
#define SCG1_APLL_CFG_PREDIV_NUM        ((0x0) << SCG_PLL_CFG_PREDIV_SHIFT)
/* APLL output clocks (including PFD outputs) selected	<<2 */
#define SCG1_APLL_CFG_BYPASS_NUM        ((0x0) << SCG_PLL_CFG_BYPASS_SHIFT)
/* APLL PFD output clock selected <<1 */
#define SCG1_APLL_CFG_PLLSEL_NUM        ((0x0) << SCG_PLL_CFG_PLLSEL_SHIFT)
/* Clock source is System OSC <<0 */
#define SCG1_APLL_CFG_CLKSRC_NUM        ((0x0) << SCG_PLL_CFG_CLKSRC_SHIFT)

/*
 * A7 APLL = 24MHz / 1 * 22 / 1 / 1 = 528MHz,
 * system PLL is sourced from APLL,
 * APLL clock source is system OSC (24MHz)
 */
#define SCG1_APLL_CFG_NUM_24M_OSC (SCG1_APLL_CFG_POSTDIV2_NUM     |   \
				   SCG1_APLL_CFG_POSTDIV1_NUM     |   \
				   (22 << SCG_PLL_CFG_MULT_SHIFT) |   \
				   SCG1_APLL_CFG_PFDSEL_NUM       |   \
				   SCG1_APLL_CFG_PREDIV_NUM       |   \
				   SCG1_APLL_CFG_BYPASS_NUM       |   \
				   SCG1_APLL_CFG_PLLSEL_NUM       |   \
				   SCG1_APLL_CFG_CLKSRC_NUM)

/* PFD0 Freq = A7 APLL(528MHz) * 18 / 27 = 352MHz */
#define SCG1_APLL_PFD0_FRAC_NUM (27)


/* SCG1(A7) FIRC DIV configurations */
/* Disable FIRC DIV3 */
#define SCG1_FIRCDIV_DIV3_NUM           ((0x0) << SCG_FIRCDIV_DIV3_SHIFT)
/* FIRC DIV2 = 48MHz / 1 = 48MHz */
#define SCG1_FIRCDIV_DIV2_NUM           ((0x1) << SCG_FIRCDIV_DIV2_SHIFT)
/* Disable FIRC DIV1 */
#define SCG1_FIRCDIV_DIV1_NUM           ((0x0) << SCG_FIRCDIV_DIV1_SHIFT)

/* SCG1(A7) NICCCR configurations */
/* NIC clock source is DDR clock (396/198MHz) */
#define SCG1_NICCCR_NICCS_NUM		((0x1) << SCG_NICCCR_NICCS_SHIFT)

/* NIC0 clock = DDR Clock / 2 = 396MHz / 2 = 198MHz */
#define SCG1_NICCCR_NIC0_DIV_NUM	((0x1) << SCG_NICCCR_NIC0_DIV_SHIFT)
/* NIC0 clock = DDR Clock / 1 = 198MHz / 1 = 198MHz */
#define SCG1_NICCCR_NIC0_DIV_LF_NUM	((0x0) << SCG_NICCCR_NIC0_DIV_SHIFT)
/* NIC1 clock = NIC0 Clock / 1 = 198MHz / 2 = 198MHz */
#define SCG1_NICCCR_NIC1_DIV_NUM	((0x0) << SCG_NICCCR_NIC1_DIV_SHIFT)
/* NIC1 bus clock = NIC1 Clock / 3 = 198MHz / 3 = 66MHz */
#define SCG1_NICCCR_NIC1_DIVBUS_NUM	((0x2) << SCG_NICCCR_NIC1_DIVBUS_SHIFT)
#define SCG1_NICCCR_CFG_NUM		(SCG1_NICCCR_NICCS_NUM      | \
					 SCG1_NICCCR_NIC0_DIV_NUM   | \
					 SCG1_NICCCR_NIC1_DIV_NUM   | \
					 SCG1_NICCCR_NIC1_DIVBUS_NUM)

/* SCG1(A7) FIRC DIV configurations */
/* Enable FIRC DIV3 */
#define SCG1_SOSCDIV_DIV3_NUM		((0x1) << SCG_SOSCDIV_DIV3_SHIFT)
/* FIRC DIV2 = 48MHz / 1 = 48MHz */
#define SCG1_SOSCDIV_DIV2_NUM		((0x1) << SCG_SOSCDIV_DIV2_SHIFT)
/* Enable FIRC DIV1 */
#define SCG1_SOSCDIV_DIV1_NUM		((0x1) << SCG_SOSCDIV_DIV1_SHIFT)

