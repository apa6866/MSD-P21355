# MSD-P21355
Software for P21355 test fixture
format = <Opcode> <Command> <Parameter>                       *("Parameter" not always applicable)     
  
Opcodes:  LD (main load), MN (matching network),  RC (rc load), RF (rf band)
Commands: G (get) S (set) H (help)
Parameters: tbd

--NOTES--
  - Verbose 'Help' printed when 'V' added as parameter
  - Looking to try to keep parameters as a 3-character input
