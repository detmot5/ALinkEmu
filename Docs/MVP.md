# MVP

1. Emulation of AVR CPU to run simple program and 7then to emulate first microcontroller chip - ATmega328p
	- Emulation of memory and memory addressing system for RAM FLASH AND EEPROM It's worth to mention that the AVR cpu is build based on Harvad architecture and it's little endian
	- Implementation of basic cpu registers in memory address space 
		![](Img/MemoryMap.png)
		![](Img/GPWR.png)
		_Images from ATMega328P datasheet_
	- Implementing instructions (page 330 of documentation)
		- Create a clear and universal way to implement instructions
		- All few hundred instructions need to be implemented
		- Every instuction MUST HAVE its unit test
	- Initialization procedure
	- Reset procedure
	- Implementation of basic IO registers (PORTx, PINx, DDRx)
	- Basic clock implementation
	- All modules of chip must have unit tests (especially instructions)
	
	
2. Chip interfacing 	
	- Universal Implementation of loading whole program from bin or hex file (ProgramLoader or something) 
	- Serializing of chip image snapshot or transmitting Chip image in real time. Something like pseudo code below
	``` C++
		BaseChipSerializer* serializer = chip.CreateSerializer();
		JsonObject obj = serializer.Serialize();
	```
	- We should be able also restore chip state from serialized snapshot
	- Simple QT interface in Sandbox project to debug app and create demos
	- Create EmulationEngine which will provide resources that emulation needs. Will run emultion on separate thread to provide maximum performance minimize delays caused by operations in main thread like chip data serializing
	``` C++
		ALinkEmu::EmulationEngine engine;
		// Still not sure how chip creating process will finally look.
		// Maybe using ALinkEmu::ChipRef = std::shared_ptr<BaseChip>?
		std::shared_ptr<BaseChip> chip = ALinkEmu::CreateChip("ATmega328P");
		engine.AttachChip(chip);
		engine.StartMultithreadedEmulation();

		// Add infinite loop or do other tasks to not stop main thread.
	```
