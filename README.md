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


# Introduction to this repo
Odometre is the nick name given to this project. It is basically an attitude
(orientation) indicator intended for a bicycle. It uses nRF5 SDK v12.3.0, but
Nordic Semiconductor has since discontinued this SDK and has moved to using
the [Zephyr project](https://www.zephyrproject.org/).

Because v12.3.0 was the last stable release for the board (PCA20006) used in
this project, it made sense to start from that release instead of attempting
to make the Zephyr project support the discontinued PCA20006 board.


# What Odometre does
Read data from an inertial measurement unit (IMU) and put that up on a Nokia
5110 display. There are two buttons on the board and pressing a button either
takes you to the next page of information or to the previous.


**When I this document refers to the 'source directory', assume the path below
`nRF5_SDK_12.3.0_d7731ad_ses.support/examples_segger/ble_peripheral/ble_app_template/pca10028/s130/ses_official`


# Diary

- Assuming you have installed `openocd` v0.11.0, start the GDB server. You also
need to have the stlink-v2 debugger. It plugs to the SWD interface.
`openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/share/openocd/scripts/target/nrf51.cfg`

Segger Embedded Studio (SES) runs a gdb client.
above. Open the project file `nRF5_SDK_12.3.0_d7731ad_ses.support/examples_segger/ble_peripheral/ble_app_template/pca10028/s130/ses_official/segger_support.emProject`
in SES to get started. When you press F5 to start debugging, SES will connect
to the gdb server started above and you can step through and set breakpoints
from the GUI. This is in face most convenient.

If you want to program the softdevice, then connect to the gdb server and run
the command below. **Only** use mass_erase to program the softdevice. To
program your application, use the same commands below but without the line with
`mass_erase`. The Softdevice sits at base of flash, then your application
follows.
```
telnet localhost 4444
reset
halt
nrf51 mass_erase 0
program <path to hex file> verify
```

- Never used this command because I didn't have a JLink debugger
`nrfjprog --family NRF51 --eraseall --port 3333 --ip 127.0.0.1`


- Initially, I was compiling my application my going to the examples directory
and calling 'make' in the 'armgcc' folder. This was because I couldn't get SES
to work with this nRF5 SDK.

There is no SES project file released by Nordic for v12.3.0. Newer SDK versions
have such an SES project file that you can open in SES to
run the example programs.
[This video](https://www.youtube.com/watch?v=o_9Lmm0SYr8) did come in handy at
some point when I was still compiling within make.

There is a file `/usr/bin/cmsis-conf` for configuring your sdk_config.h file.
This is a shell script that encapsulates the call to the CMSIS configuration
wizard. You can find this configuration wizard available on the internet.

**Adafruit-related nuances follows**
I first attempted to use the latest Adafruit library for the Nokia display, but
this proved to be too much of a haserl, so I decided to use the initial library
that Adafruit had before the split their graphics library into separate repos.
That initial library worked like a charm. These files are
`<source directory>/{PCD8544.cpp,PCD8544.h,glcdfont.h}`

- `undefined reference to vtable for Adafruit_GFX`
  I used the outdated library from a forum [here](https://forum.pjrc.com/threads/10-Adafruit-PCD8544-LCD-library).
  The latest library was causing thw above link error and other similar ones.

- To be sure the display was working, I connected it to an Arduino Nano, but
  my Ubuntu would not detect the Nano.
  This [link](https://forum.arduino.cc/t/ubuntu-arduino-ide-not-showing-any-ports/1043925) had the solution.

- [Something about constructors](https://devzone.nordicsemi.com/f/nordic-q-a/42453/undefined-reference-to-vtable-for-__cxxabiv1-__si_class_type_info)

- [You should define virtual functions](https://gcc.gnu.org/faq.html#vtables).
  In the link, find the heading "When building C++, the linker says my
  constructors, destructors or virtual tables are undefined, but I defined them"

- You might be looking for a function [defined in a C++](https://gcc.gnu.org/legacy-ml/gcc-help/2009-10/msg00320.html)library `libsupc++`

- Helpful [link](https://isocpp.org/wiki/faq/mixing-c-and-cpp) when mixing C++ and C.

- `__cxa_new_handler` is apparently defined to be `0`. This [link](https://github.com/llvm/llvm-project/issues/23612) may or may not be related.
  In the assembly snippet below, if `__cxa_new_handler` is 0, we "abort". Ihis 0 comparison (`NULL`) is because the handler is not defined.
  You need to enable exception support in SES; otherwise, some handlers would likely be defined as null. Read [this](https://wiki.segger.com/C%2B%2B_Support_in_Embedded_Studio).
  When I would eventually update all library options in the project configuration to use C++ exceptions, the `new` handler was defined and I could catch the exception.
  It was then clear to me that there was infact insufficient memory, which was much to my suprise. I guess I didn't realise how memory it takes to create a C++ object.
  Long story short, I was forced to move away from using classes.

```
    F7FFFF89    bl 0x00026194 <malloc>
    2800        cmp r0, #0
    D109        bne 0x0002629A
    F000F811    bl 0x000262AC <std::get_new_handler()>
    2800        cmp r0, #0
    D006        beq 0x0002629C
    4780        blx r0
    0020        movs r0, r4
    F7FFFF7F    bl 0x00026194 <malloc>
    2800        cmp r0, #0
    D0F5        beq 0x00026286
    BD10        pop {r4, pc}
    F7F8F942    bl 0x0001E524 <abort>
    E7EC        b 0x0002627C
    46C0        nop
<operator new(unsigned int, std::nothrow_t const&)>
    B510        push {r4, lr}
    F7FFFFE5    bl 0x00026274 <operator new(unsigned int)>
    BD10        pop {r4, pc}
<std::get_new_handler()>
    4B01        ldr r3, =0x200027F4 <__cxa_new_handler>
    6818        ldr r0, [r3]
    4770        bx lr
    46C0        nop
```


- The first argument to the SVCALL is meant to be uint16_t, but it was defined
  as uint8_t. This caused build errors. See the links below for context.
  this [link](https://devzone.nordicsemi.com/f/nordic-q-a/13019/compiling-with-g-error-asm-operand-0-probably-doesn-t-match-constraints)
  has more information about why it was a problem. It was a known [bug](https://devzone.nordicsemi.com/f/nordic-q-a/2232/upgrading-to-sdk-5-2-0-breaks-build)

- Define a global macro in SES [here](https://studio.segger.com/index.htm?https://studio.segger.com/ide_project_macros.htm)

- You can develop withouth SES as [this guy shows](https://www.youtube.com/watch?v=o_9Lmm0SYr8). Just call make in your project.


**Some useful links on the Nordic forum**
- [clock setup](https://devzone.nordicsemi.com/f/nordic-q-a/4118/what-is-the-high-frequency-clock-model-for-nrf51)

- Dealing with data loss when [interrupted](https://devzone.nordicsemi.com/f/nordic-q-a/294/what-s-the-maximum-of-baud-rate-supported-of-uart) for long by the softdevice

- Something about why `APP_UART_COMMUNICATION_ERROR` [here](https://devzone.nordicsemi.com/f/nordic-q-a/60038/app_uart_communication_error/245875)


For the initial module, I used a level converter between the nRF and the module and that worked as expected. With this new module, I didn't. I kept having frame errors and missing bits. So I powered the module with 3.3V (previously 5V and level converter) and plugged the module directly to the nRF bypassed the logic level converter. It worked! I actually got this idea to bypass after noticing that no matter what voltage you put on the module, the rx/tx line was 3.3V logic.

Some images are [here](https://drive.google.com/drive/folders/1jXyxWfyETwIEblGPjH74MOFDfMVSOtNx?usp=sharing)


### 26 Aug 2024

Transistors Q4 came with pads unexposed, so I fixed the board manually by scratching away
the silkscreen. I used a PCB library of the internet, and turns out the fault was from
that library. Never expected a fault from a reliable source. I used Diptrace, but I'm sure
a more intelligent EDA software would have warned me right away.

From the last time I prototyped on a breadboard (several months back), I was able to use
the LCD. After assembling on a PCB, the LCD printed garbage characters and sometimes
nothing. Few times it printed something meaningful, but I was unable to reproduce this.
Having no oscilloscope to check the SPI signals, I was left with speculations. 

My first suspicion was insufficient power (perhaps wrong calculations without accounting enough
loses), so I aimed to see how much power I could draw from the board. I put my multimiter
to the 200mA range and short-circuited VCC and GND at some random place after the power IC. I
forgot I had no overcurrent protection, so I damaged few ICs. Annoyingly, this 3V3 power IC
became a short-circuit when it broke. I thought it was just the ICs, but whenever I changed it,
it smoked. I wasn't able to find the short, so I came to the conclusion that the board didn't
have enough protection anyway, so I parked this prototype and aimed to do it in [Flux.ai](www.flux.ai).

Gotta say it's annoying to put all that time and effort into soldering and assembling only
to blow everything up and start again.

**Future Improvements**
- Place SMDs and space them such that it's easy to change them if something went wrong.
- Use proper connectors and accommodate a test supply input rather than the hack with
  jumper cables. Make it easy to temporarily connect modules before you solder them.
- Add over-current protection. Make it hard to do something dumb. Add 10% to power supply
  requirements to account for loses.
- Test power supply circuitry before anything else.