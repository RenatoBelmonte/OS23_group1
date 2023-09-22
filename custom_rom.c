#include "qemu/osdep.h"
#include "qapi/error.h" /* provides error_fatal() handler */
#include "hw/sysbus.h" /* provides all sysbus registering func */
#include "hw/misc/custom_rom.h"

#define TYPE_CUSTOM_ROM "custom_rom"
typedef struct CustomRomState CustomRomState;
DECLARE_INSTANCE_CHECKER(CustomRomState, CUSTOM_ROM, TYPE_CUSTOM_ROM)

#define REG_ID 	0x0
#define CHIP_ID	0xBA000001

struct CustomRomState {
	SysBusDevice parent_obj;
	MemoryRegion iomem;
	uint64_t chip_id;
};

static uint64_t custom_rom_read(void *opaque, hwaddr addr, unsigned int size)
{
	CustomRomState *s = opaque;
	
	switch (addr) {
	case REG_ID:
		return s->chip_id;
		break;
	
	default:
		return 0xDEADBEEF;
		break;
	}

	return 0;
}

static const MemoryRegionOps custom_rom_ops = {
	.read = custom_rom_read,
	.endianness = DEVICE_NATIVE_ENDIAN,
};

static void custom_rom_instance_init(Object *obj)
{
	CustomRomState *s = CUSTOM_ROM(obj);

	/* allocate memory map region */
	memory_region_init_io(&s->iomem, obj, &custom_rom_ops, s, TYPE_CUSTOM_ROM, 0x100);
	sysbus_init_mmio(SYS_BUS_DEVICE(obj), &s->iomem);

	s->chip_id = CHIP_ID;
}

/* create a new type to define the info related to our device */
static const TypeInfo custom_rom_info = {
	.name = TYPE_CUSTOM_ROM,
	.parent = TYPE_SYS_BUS_DEVICE,
	.instance_size = sizeof(CustomRomState),
	.instance_init = custom_rom_instance_init,
};

static void custom_rom_register_types(void)
{
    type_register_static(&custom_rom_info);
}

type_init(custom_rom_register_types)

/*
 * Create the Custom Rom device.
 */
DeviceState *custom_rom_create(hwaddr addr)
{
	DeviceState *dev = qdev_new(TYPE_CUSTOM_ROM);
	sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
	sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, addr);
	return dev;
}