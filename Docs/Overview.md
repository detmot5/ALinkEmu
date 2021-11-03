# ALinkEmu
## Microcontroller and System on Chip emulation tool.


It will be available to use through following interfaces
 - API - it should be able to be controlled from any c++ software using API interface like below: 
	``` C++
		ALinkEmu::ChipRef chip = ALinkEmu::UndefinedChip;

		std::vector<std::string> availableChipNames = ALinkEmu::GetChipList();

		try {
			// look for ATMega328p module (shared library) in dedicated
			// Chip modules folder
			// ImportChipModule will throw an exception if module will is invalid.
			chip = ALinkEmu::MakeChip("ATmega328p");
		} catch(ALinkEmu::BadChipModuleException& e) {
			...
			std::exit(EXIT_FAILURE);
		}
		
		// you can also check chip ref validity using:
		chip.IsNull();


		try {
			std::ifstream file("firmware.bin");
			// Using program loader to not store program loading in ChipRef class.
			ALinkEmu::FlashImageLoader loader;
			ALinkEmu::FlashImage binary = loader.FromStream(&file);
			chip.SetFlashImage(binary);
		} catch(ALinkEmu::BadProgramException& e) {
			...
		}


		ALinkEmu::EmulationEngine emu;

		emu.AttachChip(chip);

		// Allocates memory, creates threads for cpu and basic peripherals
		// like interrupt controller, watchdog etc.
		// sets whole chip to initial state with default config
		// Should take some configuration args like map fusebits lockbits
		chip.Setup();

		// Starts exectuion of program - should be done asynchronously, so it's necesarry to create infinte loop in main trhead.
		chip.Run();
	```
	- Nodejs and python interface
	- GUI using electron(node), c++ qt and pyqt

### 1. MVP 

Create emulation of atmega328p
 - All instructions 
 - Memory and registers
 - Bootloader
 - Clock
 - Interupt controller
 - Peripherals

After that, implement other AVR8 chips and during it create unified way
to implement chips and peripherals emulations.


#### Architecture of ALinkEmu MVP

``` plantuml

	interface Chip {
	 void Start();
	 void Run();
	 void Restart();
	 void LoadFlashImage(flashImage: FlashImage&&);
	 ChipInfo GetChipInfo();
	 - void LoadInstruction(instruction: Instruction);
	}


	class ATMega328p implements Chip {
		- AVR::CPU cpu;
		- AVR::InterruptController irqc;
		- AVR::USARTModule USART0;
		- AVR::SPIModule spi;
		- AVR::I2CModule i2c;
		- AVR::Watchdog wdt;
		- AVR::SystemClock clock;


		void Run();
		void Start();
		void Restart();
		void LoadFlashImage(image: FlashImage&&)
	}

	


	class FlashImage {
		std::vector<uint8_t> flash;
	}

	FlashImage -> ATMega328p 

	class FlashImageLoader {
		FlashImage FromStream(stream: std::istream);
		FlashImage FromFile(fileName: const std::string&);
		FlashImage FromBlob(data: const uint8_t*, size: size_t);
	}


```


### 2. Future features

#### Virtual serial port
Emulated device will create virtual serial port in operating system, 
which will be available to use like physical serial port.
For instance you will be able to listen and write to it using Arduino Serial Monitor.
Also there will be possibility to attach virtual serial port to pshysical serial port and communicate with physical device (for example an Arduino board)

In Linux systems it's avalilable to do from user space. 
In Windows, virtual serial port driver will be necessary (thanks Bill)

#### OneChip

Imagine to have one big development board which can emulate lots of microcontrollers and SoC's.
OneChip will be based on raspberryPI or some big STM32 uC and will have a lot of GPIO ports which will can be mapped to any chip pinout.

OneChip based on stm will be only a bridge between emulated device on programmer's computer and real hardware.

OneChip will communicate with host computer using USB to reach smallest delays as possible

But OneChip based on raspberry will be fully funtional chip emulator.

