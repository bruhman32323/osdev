#include <stdint.h>
#include <drivers/ahci/ahci.h>
#include <drivers/terminal/terminal.h>
#include <std/util.h>
#include <std/string.h>
#include <std/inline.h>

// AHCI driver, source: https://wiki.osdev.org/AHCI

static void trace_ahci(const char* message, int i) {
    terminal_writestring(message);
    terminal_writeline(itoa(i));
}

// Check device type
static int check_type(HBA_PORT *port) {
	uint32_t ssts = port->ssts;
 
	uint8_t ipm = (ssts >> 8) & 0x0F;
	uint8_t det = ssts & 0x0F;
 
	if (det != HBA_PORT_DET_PRESENT)	// Check drive status
		return AHCI_DEV_NULL;
	if (ipm != HBA_PORT_IPM_ACTIVE)
		return AHCI_DEV_NULL;
 
	switch (port->sig)
	{
	case SATA_SIG_ATAPI:
		return AHCI_DEV_SATAPI;
	case SATA_SIG_SEMB:
		return AHCI_DEV_SEMB;
	case SATA_SIG_PM:
		return AHCI_DEV_PM;
	default:
		return AHCI_DEV_SATA;
	}
}

void probe_port(HBA_MEM *abar) {
	// Search disk in implemented ports
	uint32_t pi = abar->pi;
	int i = 0;
	while (i<32)
	{
		if (pi & 1)
		{
			int dt = check_type(&abar->ports[i]);
			if (dt == AHCI_DEV_SATA)
			{
				trace_ahci("SATA drive found at port: ", i);
			}
			else if (dt == AHCI_DEV_SATAPI)
			{
				trace_ahci("SATAPI drive found at port: ", i);
			}
			else if (dt == AHCI_DEV_SEMB)
			{
				trace_ahci("SEMB drive found at port: ", i);
			}
			else if (dt == AHCI_DEV_PM)
			{
				trace_ahci("PM drive found at port: ", i);
			}
			else
			{
				//trace_ahci("No drive found at port: ", i);
			}
		}
 
		pi >>= 1;
		i ++;
	}
}

uint32_t pci_config_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint64_t tmp = 0;

    /* create configuration address as per Figure 1 */
    address = (uint32_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

    /* write out the address */
    outportl(0xCF8, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    if (offset == 0x24)
        tmp = inportb(0xCFC) ;
    else
        tmp = (uint16_t)( (inportb(0xCFC) >> ((offset & 2) * 8) ) & 0xffff);
    return (tmp);
}

uint16_t get_vendor_id(uint8_t bus, uint8_t slot)
{
    return pci_config_read_word(bus, slot, 0, 0);
}

uint16_t get_device_id(uint8_t bus, uint8_t slot)
{
    return pci_config_read_word(bus, slot, 0, 2);
}

uint64_t check_device(uint8_t bus, uint8_t device) {
    //uint8_t function = 0;

    uint16_t vendorID = get_vendor_id(bus, device);
    if(vendorID == 0xFFFF) return 0;        // Device doesn't exist
    uint16_t deviceID = get_device_id(bus, device);

    return pci_config_read_word(bus, device, 0, 0x24);
    return 0;
 }

uint64_t check_all_buses(void)
{
    uint16_t bus;
    uint8_t device;
    uint64_t bar5;

    for(bus = 0; bus < 256; bus++)
    {
        for(device = 0; device < 32; device++)
        {
            bar5 = check_device(bus, device);
            if (bar5 == 0) return 0;
            probe_port((HBA_MEM*)bar5);
            return bar5;
        }
    }
    return 0;
}
 
// Start command engine
void start_cmd(HBA_PORT *port)
{
	// Wait until CR (bit15) is cleared
	while (port->cmd & HBA_PxCMD_CR);
 
	// Set FRE (bit4) and ST (bit0)
	port->cmd |= HBA_PxCMD_FRE;
	port->cmd |= HBA_PxCMD_ST; 
}
 
// Stop command engine
void stop_cmd(HBA_PORT *port)
{
	// Clear ST (bit0)
	port->cmd &= ~HBA_PxCMD_ST;
 
	// Clear FRE (bit4)
	port->cmd &= ~HBA_PxCMD_FRE;
 
	// Wait until FR (bit14), CR (bit15) are cleared
	while(1)
	{
		if (port->cmd & HBA_PxCMD_FR)
			continue;
		if (port->cmd & HBA_PxCMD_CR)
			continue;
		break;
	}
 
}

void port_rebase(HBA_PORT *port, int portno)
{
	stop_cmd(port);	// Stop command engine
 
	// Command list offset: 1K*portno
	// Command list entry size = 32
	// Command list entry maxim count = 32
	// Command list maxim size = 32*32 = 1K per port
	port->clb = AHCI_BASE + (portno<<10);
	port->clbu = 0;
	memset((void*)(port->clb), 0, 1024);
 
	// FIS offset: 32K+256*portno
	// FIS entry size = 256 bytes per port
	port->fb = AHCI_BASE + (32<<10) + (portno<<8);
	port->fbu = 0;
	memset((void*)(port->fb), 0, 256);
 
	// Command table offset: 40K + 8K*portno
	// Command table size = 256*32 = 8K per port
	HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)(port->clb);
	for (int i=0; i<32; i++)
	{
		cmdheader[i].prdtl = 8;	// 8 prdt entries per command table
					// 256 bytes per command table, 64+16+48+16*8
		// Command table offset: 40K + 8K*portno + cmdheader_index*256
		cmdheader[i].ctba = AHCI_BASE + (40<<10) + (portno<<13) + (i<<8);
		cmdheader[i].ctbau = 0;
		memset((void*)cmdheader[i].ctba, 0, 256);
	}
 
	start_cmd(port);	// Start command engine
}

void ahci_init(void) {
    uint64_t bar5 = check_all_buses();
    if (bar5 == 0) {
        terminal_writeline("Error: Could not find AHCI device");
        return;
    }

    terminal_writestring("Found AHCI device at: ");
    terminal_writeline(itoa(bar5));
}