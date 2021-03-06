/*!
 * File generated by platform wizard. DO NOT MODIFY
 *
 */

metaonly module Platform inherits xdc.platform.IPlatform {

    config ti.platforms.generic.Platform.Instance CPU =
        ti.platforms.generic.Platform.create("CPU", {
            clockRate:      1250,                                       
            catalogName:    "ti.catalog.c6000",
            deviceName:     "TMS320C6678",
            customMemoryMap:
           [          
                ["MSMCSRAM_MASTER", 
                     {
                        name: "MSMCSRAM_MASTER",
                        base: 0x0c000000,                    
                        len: 0x00100000,                    
                        space: "code/data",
                        access: "RWX",
                     }
                ],
                ["L2SRAM", 
                     {
                        name: "L2SRAM",
                        base: 0x00800000,                    
                        len: 0x00040000,                    
                        space: "code/data",
                        access: "RWX",
                     }
                ],
                ["MSMCSRAM_SLAVE", 
                     {
                        name: "MSMCSRAM_SLAVE",
                        base: 0x0c100000,                    
                        len: 0x00100000,                    
                        space: "code/data",
                        access: "RWX",
                     }
                ],
                ["MSMCSRAM_IPC", 
                     {
                        name: "MSMCSRAM_IPC",
                        base: 0x0c200000,                    
                        len: 0x00200000,                    
                        space: "code/data",
                        access: "RWX",
                     }
                ],
                ["DDR3", 
                     {
                        name: "DDR3",
                        base: 0x80000000,                    
                        len: 0x40000000,                    
                        space: "code/data",
                        access: "RWX",
                     }
                ],
           ],
          l2Mode: "256k",
          l1PMode: "32k",
          l1DMode: "32k",

    });
    
instance :
    
    override config string codeMemory  = "L2SRAM";   
    override config string dataMemory  = "L2SRAM";                                
    override config string stackMemory = "L2SRAM";

    config String l2Mode = "256k";
    config String l1PMode = "32k";
    config String l1DMode = "32k";
}
