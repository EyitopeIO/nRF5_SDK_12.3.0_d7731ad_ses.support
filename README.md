# Segger Embedded Studio support for NRF5_SDK_12_3_0_d7732ad

- Ses projects added to examples_segger folder.
- Project in "/examples_segger/ble_peripheral/ble_app_template/pca10028/s130/ses_official"
directly created at SES (without import keil project).

# What has changed?

- Inside "hardfault_handler_gcc.c" file,

// "   ldr   r1, =__StackTop                  \n"

// "   ldr   r2, =__StackLimit                \n"

"   ldr   r1, =__stack_end__                \n"

"   ldr   r2, =__stack_start__              \n"


Following lines changed as above according to "https://devzone.nordicsemi.com/f/nordic-q-a/23232/hardfault_handler-for-segger-embedded-studio".

# Odometre intro
I bought it a few years back and saw it lying around. Why not try a new project?
This is based of the PCA20006 board from Nordic Semiconductors, a Bluetooth Low
Energy device. It runs its own "operating system" called SoftDevice and this is
what the application was built upon i.e. using the abstractions instead of bare-
metal development. This SDK is no longer developed and is relatively old. Nordic
has since ported all to the Zephyr Project.

# Command line and how-to

Start the GDB server. Assumes you installed openocd and its dependencies
`openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/share/openocd/scripts/target/nrf51.cfg`

You may use an IDE to debug, otherwise, connect to the GDB server via telnet. Segger Embedded Studio was used for debugging.
You first need to program the softdevice. **Only** use mass_erase to program the softdevice. To program you application, use the
same commands below but skip the `mass_erase` line. The Softdevice sits at base of flash, then your application follows.
```
telnet localhost 4444
reset
halt
nrf51 mass_erase 0
program <path to hex file> verify
```

Never used this command because I didn't have a JLink
`nrfjprog --family NRF51 --eraseall --port 3333 --ip 127.0.0.1`

#### Internal configurations
* Pull up resistors in P008 and P018 is enabled

#### Self.comments

I would eventually learn that I didn't need Segger Embedded Studio (SES) for deve-
lopment, and I could have simply called a makefile in armgcc to do compile all. It didn't occur
to me that the SDK was fairly self-contained and using SES was uneccessary for development.
There is no SES project file released by Nordic for v12.3.0 to work with SES. You have to configure all yourself. When it became important to debug, I went through the pain of configuring the IDE. I would eventually find this current repo (which I forked) to be sufficient and continue development from there [This guy's video](https://www.youtube.com/watch?v=o_9Lmm0SYr8) may help

There is a file `/usr/bin/cmsis-conf` for configuring your sdk_config.h file. This is a
shell scriptt that encapsulates the CMSIS configuration wizard. You can find it on the internet and I think it ships with the SDK too.


# Useful links and references

### Adafruit
- `undefined reference to `vtable for Adafruit_GFX`
I used the outdated library from a forum [here](https://forum.pjrc.com/threads/10-Adafruit-PCD8544-LCD-library). The latest library
was causing the link error above.
- To be sure the display was working, I connected it to an Arduino Nano, but my Ubuntu would not detect the Nano.
This [link](https://forum.arduino.cc/t/ubuntu-arduino-ide-not-showing-any-ports/1043925) had the solution.

### Using C++
- [Something about constructors](https://devzone.nordicsemi.com/f/nordic-q-a/42453/undefined-reference-to-vtable-for-__cxxabiv1-__si_class_type_info)
- [You should define virtual functions](https://gcc.gnu.org/faq.html#vtables). See the heading "When building C++, the linker says my constructors, destructors or virtual tables are undefined, but I defined them"
- You might be looking for a function [defined in a C++](https://gcc.gnu.org/legacy-ml/gcc-help/2009-10/msg00320.html) library `libsupc++`
- Helpful [link](https://isocpp.org/wiki/faq/mixing-c-and-cpp) when mixing C++ and C.

### SDK, IDE, Nordic, and other specific nuances
- The first argument to the SVCALL is meant to be uint16_t, but it was defined as uint8_t. Although already implemented in the SDK,
this [link](https://devzone.nordicsemi.com/f/nordic-q-a/13019/compiling-with-g-error-asm-operand-0-probably-doesn-t-match-constraints) has more information about why it was a problem. It was a known [bug](https://devzone.nordicsemi.com/f/nordic-q-a/2232/upgrading-to-sdk-5-2-0-breaks-build)
- Define a global macro in SES [here](https://studio.segger.com/index.htm?https://studio.segger.com/ide_project_macros.htm)
