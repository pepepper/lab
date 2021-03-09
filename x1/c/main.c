#include "common.h"
#include "pcc.h"
#include "imx-regs.h"
#include "fsl_lpuart.h"

#define US1_TDRE	(1 << 7)
#define US1_RDRF	(1 << 5)
#define US1_OR		(1 << 3)
#define UC2_TE		(1 << 3)
#define UC2_RE		(1 << 2)
#define CFIFO_TXFLUSH	(1 << 7)
#define CFIFO_RXFLUSH	(1 << 6)
#define SFIFO_RXOF	(1 << 2)
#define SFIFO_RXUF	(1 << 0)

#define STAT_LBKDIF	(1 << 31)
#define STAT_RXEDGIF	(1 << 30)
#define STAT_TDRE	(1 << 23)
#define STAT_RDRF	(1 << 21)
#define STAT_IDLE	(1 << 20)
#define STAT_OR		(1 << 19)
#define STAT_NF		(1 << 18)
#define STAT_FE		(1 << 17)
#define STAT_PF		(1 << 16)
#define STAT_MA1F	(1 << 15)
#define STAT_MA2F	(1 << 14)
#define STAT_FLAGS	(STAT_LBKDIF | STAT_RXEDGIF | STAT_IDLE | STAT_OR | \
			 STAT_NF | STAT_FE | STAT_PF | STAT_MA1F | STAT_MA2F)

#define CTRL_ORIE	(1 << 27)
#define CTRL_NEIE	(1 << 26)
#define CTRL_FEIE	(1 << 25)
#define CTRL_PEIE	(1 << 24)
#define CTRL_TIE	(1 << 23)
#define CTRL_TCIE	(1 << 22)
#define CTRL_RIE	(1 << 21)
#define CTRL_ILIE	(1 << 20)
#define CTRL_TE		(1 << 19)
#define CTRL_RE		(1 << 18)
#define CTRL_MA1IE	(1 << 15)
#define CTRL_MA2IE	(1 << 14)

#define FIFO_TXOFE  (1 << 9)
#define FIFO_RXUFE  (1 << 8)
#define FIFO_TXFE   (1 << 7)
#define FIFO_RXFE   (1 << 3)

static struct lpuart_fsl_reg32* regs[] = {
	(struct lpuart_fsl_reg32*)0x4103A000,
	(struct lpuart_fsl_reg32*)0x4103B000,
	(struct lpuart_fsl_reg32*)0x410AB000,
	(struct lpuart_fsl_reg32*)0x410AC000,
	(struct lpuart_fsl_reg32*)0x402D0000,
	(struct lpuart_fsl_reg32*)0x402E0000,
	(struct lpuart_fsl_reg32*)0x40A60000,
	(struct lpuart_fsl_reg32*)0x40A70000,
};

static void lpuart_read32(u32 *addr, u32 *val) {
    *(u32 *)val = readl(addr);
}

static void lpuart_write32(u32 *addr, u32 val) {
    writel(val, addr);
}

// clk

static enum scg_clk pcc_clksrc[2][7] = {
    {
        SCG_NIC1_BUS_CLK,
        SCG_NIC1_CLK,
        SCG_DDR_CLK,
        SCG_APLL_PFD2_CLK,
        SCG_APLL_PFD1_CLK,
        SCG_APLL_PFD0_CLK,
        USB_PLL_OUT,
    },
    {
        SCG_SOSC_DIV2_CLK,
        MIPI_PLL_OUT,
        SCG_FIRC_DIV2_CLK,
        SCG_ROSC_CLK,
        SCG_NIC1_BUS_CLK,
        SCG_NIC1_CLK,
        SCG_APLL_PFD3_CLK,
    },
};

static struct pcc_entry pcc_arrays[] = {
	{PCC2_RBASE, DMA1_PCC2_SLOT,		CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC2_RBASE, RGPIO1_PCC2_SLOT,		CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC2_RBASE, FLEXBUS0_PCC2_SLOT,	CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC2_RBASE, SEMA42_1_PCC2_SLOT,	CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC2_RBASE, DMA1_CH_MUX0_PCC2_SLOT,	CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC2_RBASE, SNVS_PCC2_SLOT,		CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC2_RBASE, CAAM_PCC2_SLOT,		CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC2_RBASE, LPTPM4_PCC2_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC2_RBASE, LPTPM5_PCC2_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC2_RBASE, LPIT1_PCC2_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC2_RBASE, LPSPI2_PCC2_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC2_RBASE, LPSPI3_PCC2_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC2_RBASE, LPI2C4_PCC2_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC2_RBASE, LPI2C5_PCC2_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC2_RBASE, LPUART4_PCC2_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC2_RBASE, LPUART5_PCC2_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC2_RBASE, FLEXIO1_PCC2_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC2_RBASE, USBOTG0_PCC2_SLOT,		CLKSRC_PER_PLAT, PCC_HAS_DIV},
	{PCC2_RBASE, USBOTG1_PCC2_SLOT,		CLKSRC_PER_PLAT, PCC_HAS_DIV},
	{PCC2_RBASE, USBPHY_PCC2_SLOT,		CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC2_RBASE, USB_PL301_PCC2_SLOT,	CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC2_RBASE, USDHC0_PCC2_SLOT,		CLKSRC_PER_PLAT, PCC_HAS_DIV},
	{PCC2_RBASE, USDHC1_PCC2_SLOT,		CLKSRC_PER_PLAT, PCC_HAS_DIV},
	{PCC2_RBASE, WDG1_PCC2_SLOT,		CLKSRC_PER_BUS,	PCC_HAS_DIV},
	{PCC2_RBASE, WDG2_PCC2_SLOT,		CLKSRC_PER_BUS, PCC_HAS_DIV},

	{PCC3_RBASE, LPTPM6_PCC3_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC3_RBASE, LPTPM7_PCC3_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC3_RBASE, LPI2C6_PCC3_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC3_RBASE, LPI2C7_PCC3_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC3_RBASE, LPUART6_PCC3_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC3_RBASE, LPUART7_PCC3_SLOT,		CLKSRC_PER_BUS, PCC_NO_DIV},
	{PCC3_RBASE, VIU0_PCC3_SLOT,		CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC3_RBASE, DSI0_PCC3_SLOT,		CLKSRC_PER_BUS, PCC_HAS_DIV},
	{PCC3_RBASE, LCDIF0_PCC3_SLOT,		CLKSRC_PER_PLAT, PCC_HAS_DIV},
	{PCC3_RBASE, MMDC0_PCC3_SLOT,		CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC3_RBASE, PORTC_PCC3_SLOT,		CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC3_RBASE, PORTD_PCC3_SLOT,		CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC3_RBASE, PORTE_PCC3_SLOT,		CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC3_RBASE, PORTF_PCC3_SLOT,		CLKSRC_NO_PCS, PCC_NO_DIV},
	{PCC3_RBASE, GPU3D_PCC3_SLOT,		CLKSRC_PER_PLAT, PCC_NO_DIV},
	{PCC3_RBASE, GPU2D_PCC3_SLOT,		CLKSRC_PER_PLAT, PCC_NO_DIV},
};

int pcc_clock_get_clksrc(enum pcc_clk clk, enum scg_clk *src)
{
	u32 reg, val, clksrc_type;

	clksrc_type = pcc_arrays[clk].clksrc;
	if (clksrc_type >= CLKSRC_NO_PCS) {
		return 1;
	}

	reg = pcc_arrays[clk].pcc_base + pcc_arrays[clk].pcc_slot * 4;
	val = readl(reg);

	if (!(val & PCC_PR_MASK)) {
		return 2;
	}

	val &= PCC_PCS_MASK;
	val = (val >> PCC_PCS_OFFSET);

	if (!val) {
		return 3;
	}

	*src = pcc_clksrc[clksrc_type][val - 1];
	return 0;
}

u32 pcc_clock_get_rate(enum pcc_clk clk)
{
	u32 reg, val, rate, frac, div;
	enum scg_clk parent;
	int ret;

	ret = pcc_clock_get_clksrc(clk, &parent);
	if (ret)
		return 0;

	rate = scg_clk_get_rate(parent);

	if (pcc_arrays[clk].div == PCC_HAS_DIV) {
		reg = pcc_arrays[clk].pcc_base + pcc_arrays[clk].pcc_slot * 4;
		val = readl(reg);

		frac = (val & PCC_FRAC_MASK) >> PCC_FRAC_OFFSET;
		div = (val & PCC_PCD_MASK) >> PCC_PCD_OFFSET;

		rate = rate * (frac + 1) / (div + 1);
	}

	return rate;
}

static void setbrg(struct lpuart_fsl_reg32* reg, int baudrate)
{
	u32 sbr, osr, baud_diff, tmp_osr, tmp_sbr, tmp_diff, tmp;
	u32 clk = pcc_clock_get_rate(PER_CLK_LPUART4);

	baud_diff = baudrate;
	osr = 0;
	sbr = 0;

	for (tmp_osr = 4; tmp_osr <= 32; tmp_osr++) {
		tmp_sbr = (clk / (baudrate * tmp_osr));

		if (tmp_sbr == 0)
			tmp_sbr = 1;

		/*calculate difference in actual buad w/ current values */
		tmp_diff = (clk / (tmp_osr * tmp_sbr));
		tmp_diff = tmp_diff - baudrate;

		/* select best values between sbr and sbr+1 */
		if (tmp_diff > (baudrate - (clk / (tmp_osr * (tmp_sbr + 1))))) {
			tmp_diff = baudrate - (clk / (tmp_osr * (tmp_sbr + 1)));
			tmp_sbr++;
		}

		if (tmp_diff <= baud_diff) {
			baud_diff = tmp_diff;
			osr = tmp_osr;
			sbr = tmp_sbr;
		}
	}

	tmp = readl(&reg->baud);

	if ((osr > 3) && (osr < 8))
		tmp |= LPUART_BAUD_BOTHEDGE_MASK;

	tmp &= ~LPUART_BAUD_OSR_MASK;
	tmp |= LPUART_BAUD_OSR(osr-1);

	tmp &= ~LPUART_BAUD_SBR_MASK;
	tmp |= LPUART_BAUD_SBR(sbr);

	/* explicitly disable 10 bit mode & set 1 stop bit */
	tmp &= ~(LPUART_BAUD_M10_MASK | LPUART_BAUD_SBNS_MASK);

    writel(tmp, &reg->baud);
}

static void serial_putc(struct lpuart_fsl_reg32* reg, const char c) {
    u32 stat;

    while (1) {
        lpuart_read32(&reg->stat, &stat);

        if (stat & STAT_TDRE) {
            break;
        }
    }

    lpuart_write32(&reg->data, c);
}

struct iomux {
    u16 buf;
    u8 mode;
    u8 pull;
};


void iomux() {
    u32 pcc_u4;
    struct iomux imu4;

    pcc_u4 = readl(0x403f00b4);
    pcc_u4 |= (1 << 30);
    writel(pcc_u4, 0x403f00b4);

    imu4.buf = 0b0011; // PTC2  U4 TX
    imu4.mode = 0b0100; // PTC2  U4 TX
    imu4.pull = 0b11; // PTC2  U4 TX
    writel(*((u32 *)&imu4), (u32 *)0x40ac0008);

    //writel(0x0000000a, 0x400f0004);

    //writel(0x000fffff, 0x400f0004);
    //writel(0x00000fff, 0x400f0044);

    //writel(0x0000ffff, 0x400f0084);
    //writel(0x000fffff, 0x400f00c4);
}

void main() {
    u32 ctrl, baud, i;

    writel(0x00000004, 0x400f0004);

    iomux();

    lpuart_write32(&regs[4]->global, 0);  // De-assert reset:w

    lpuart_read32(&regs[4]->baud, &baud);
    baud &= ~(1 << 15);  // LBKDIE
    baud &= ~(1 << 14);  // RXEDGIE
    lpuart_write32(&regs[4]->baud, baud);

    lpuart_read32(&regs[4]->ctrl, &ctrl);

    ctrl &= ~CTRL_ORIE;
    ctrl &= ~CTRL_NEIE;
    ctrl &= ~CTRL_FEIE;
    ctrl &= ~CTRL_PEIE;
    ctrl &= ~CTRL_TIE;
    ctrl &= ~CTRL_TCIE;
    ctrl &= ~CTRL_RIE;
    ctrl &= ~CTRL_ILIE;
    ctrl &= ~CTRL_MA1IE;
    ctrl &= ~CTRL_MA2IE;

    ctrl &= ~CTRL_RE;
    ctrl &= ~CTRL_TE;

    lpuart_write32(&regs[4]->ctrl, ctrl);
    lpuart_write32(&regs[4]->modir, 0);
    lpuart_write32(&regs[4]->fifo, ~(FIFO_TXFE | FIFO_RXFE));
    lpuart_write32(&regs[4]->match, 0);

    setbrg(regs[4], 115200);

    ctrl |= CTRL_RE;
    ctrl |= CTRL_TE;

    lpuart_write32(&regs[4]->ctrl, ctrl);

    // putc
    while (1) {
        for (i=0; i<8; i++) {
            serial_putc(regs[4], 'Y');
            serial_putc(regs[4], '\n');
        }
    }

    asm volatile(
        "mov pc, lr\n"
    );
}
