# OS23_group1
This work is licensed under a Creative Commons Attribution-Noncommercial 4.0 International License

# SOLUTION
Firstly, the device is described in the source file *custom_rom.c*, while in the header file *custom_rom.h* there is the declaration of the cration function.
In addition, you may add the following changes to the files already present in qemu directory to correctly configure your device:

* hw/misc/Config:
```
+ config CUSTOM_ROM
+    bool 
```
* hw/misc/meson.build
```
+ softmmu_ss.add(when: 'CONFIG_CUSTOM_ROM', if_true: files('custom_rom.c'))
```

* hw/riscv/Kconfig:
```
config RISCV_VIRT
    ...
+  select CUSTOM_ROM
```

* hw/riscv/virt.c:
```
+  #include "hw/misc/banana_rom.h"

static const MemMapEntry virt_memmap[] = {
    ...
+  [VIRT_CUSTOM_ROM] =  {  0x4000000,       0x100 },
    ...
    }

+ custom_rom_create(memmap[VIRT_CUSTOM_ROM].base);
```
* include/hw/riscv/virt.h:
```
enum {
    VIRT_DEBUG,
+   VIRT_CUSTOM_ROM,
      ...
}
```
