/**
 * File Name: registers.cpp
 * Author: Vishank Singh
 * Github: https://github.com/VishankSingh
 */

#include "registers.h"
#include "pch.h"

const std::unordered_set<std::string> valid_registers = {
    "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
    "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19",
    "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "x29",
    "x30", "x31",

    "zero", 
    "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", 
    "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", 
    "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", 
    "t3", "t4", "t5", "t6",

    "ft0", "ft1", "ft2", "ft3", "ft4", "ft5", "ft6", "ft7",
    "fs0", "fs1", "fa0", "fa1", "fa2", "fa3", "fa4", "fa5",
    "fa6", "fa7", "fs2", "fs3", "fs4", "fs5", "fs6", "fs7",
    "fs8", "fs9", "fs10", "fs11", "ft8", "ft9", "ft10", "ft11",
    "ft12", "ft13", "ft14", "ft15", "ft16", "ft17", "ft18", "ft19",
    "ft20", "ft21", "ft22", "ft23", "ft24", "ft25", "ft26", "ft27",
    "ft28", "ft29", "ft30", "ft31",

    "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "v8", "v9",
    "v10", "v11", "v12", "v13", "v14", "v15", "v16", "v17", "v18", "v19",
    "v20", "v21", "v22", "v23", "v24", "v25", "v26", "v27", "v28", "v29",
    "v30", "v31",
};

const std::unordered_map<std::string, std::string> reg_alias_to_name = {
    { "zero","x0"  },
    { "ra",  "x1"  },
    { "sp",  "x2"  },
    { "gp",  "x3"  },
    { "tp",  "x4"  },
    { "t0",  "x5"  },
    { "t1",  "x6"  },
    { "t2",  "x7"  },
    { "s0",  "x8"  },
    { "s1",  "x9"  },
    { "a0",  "x10" },
    { "a1",  "x11" },
    { "a2",  "x12" },
    { "a3",  "x13" },
    { "a4",  "x14" },
    { "a5",  "x15" },
    { "a6",  "x16" },
    { "a7",  "x17" },
    { "s2",  "x18" },
    { "s3",  "x19" },
    { "s4",  "x20" },
    { "s5",  "x21" },
    { "s6",  "x22" },
    { "s7",  "x23" },
    { "s8",  "x24" },
    { "s9",  "x25" },
    { "s10", "x26" },
    { "s11", "x27" },
    { "t3",  "x28" },
    { "t4",  "x29" },
    { "t5",  "x30" },
    { "t6",  "x31" },

    { "x0", "x0"},
    { "x1", "x1"},
    { "x2", "x2"},
    { "x3", "x3"},
    { "x4", "x4"},
    { "x5", "x5"},
    { "x6", "x6"},
    { "x7", "x7"},
    { "x8", "x8"},
    { "x9", "x9"},
    { "x10", "x10"},
    { "x11", "x11"},
    { "x12", "x12"},
    { "x13", "x13"},
    { "x14", "x14"},
    { "x15", "x15"},
    { "x16", "x16"},
    { "x17", "x17"},
    { "x18", "x18"},
    { "x19", "x19"},
    { "x20", "x20"},
    { "x21", "x21"},
    { "x22", "x22"},
    { "x23", "x23"},
    { "x24", "x24"},
    { "x25", "x25"},
    { "x26", "x26"},
    { "x27", "x27"},
    { "x28", "x28"},
    { "x29", "x29"},
    { "x30", "x30"},
    { "x31", "x31"},

    { "ft0", "ft0" },
    { "ft1", "ft1" },
    { "ft2", "ft2" },
    { "ft3", "ft3" },
    { "ft4", "ft4" },
    { "ft5", "ft5" },
    { "ft6", "ft6" },
    { "ft7", "ft7" },
    { "fs0", "fs0" },
    { "fs1", "fs1" },
    { "fa0", "fa0" },
    { "fa1", "fa1" },
    { "fa2", "fa2" },
    { "fa3", "fa3" },
    { "fa4", "fa4" },
    { "fa5", "fa5" },
    { "fa6", "fa6" },

};


bool isValidRegister(const std::string& reg) {
    return valid_registers.find(reg) != valid_registers.end();
}
