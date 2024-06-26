This repo contains the demo for Chaos Cards, a prototype research-purpose digital card game, with game rules in a similar style to Hearthstone. This file contains instructions for getting the demo running or compiling the demo, with some brief introduction on the file organization here. The information is also available on the website at https://sites.google.com/a/umn.edu/chaos-cards/downloads-instructions.

**Disclaimer: This repo is part of the demo code for both the AIIDE 20 poster and my thesis, and these two cooresponds to different versions (the thesis comes later, having more features like balance evaluation, and also having many bugs like memory leaks fixed), so do not expect things like being able to reproduce exact cards on the poster on the latest version (it is machine/environment dependent anyway), but the principle should still hold. Also it is not full code, neither for the poster nor for the thesis, primarily having the neural network parts ripped off, as we want to have a relatively simple and easy-to-setup demos.**

The demo should have the compilation pipeline mostly setup. We separately made the executable folders (not where the compilation would put the executables) at "demo/Windows/Executables" and "demo/Unix/Executables" (along with some data file the program may need to read or some files it may output) for convenience of running. **If you are able to directly run one of those executable, and do not wish to recompile or read the source files, you can ignore the rest of the page** and directly skip to the guides for demo controls (see the **"Demo Controls.md"** or visiting https://sites.google.com/a/umn.edu/chaos-cards/downloads-instructions/demo-controls).


## Files / Compilation Basics

* To compile the project, make sure JRE (Java Runtime Environment) is installed. The GIGL translator needs it to execute.

* At the top level directory of the repo (not the same place with the source files for Chaos Cards), the "ableC.jar" is the GIGL-C++ translator executable, the "include" folder contains the GIGL auxiliary library sources.

* In Chaos Cards source directory ("demo/Windows/ChaosCards/ChaosCards" or "demo/Unix/ChaosCards"), the "card.xc" is the GIGL source file defining and implementing the grammatical card structure and generator. The "card.generated.h" is the C++ file generated by GIGL translating from "card.xc", which is technically not required to initially exist if GIGL works.

* In Chaos Cards source directory, "main.cpp", "Player.h", "Player.cpp" are other C++ files needed for compilation.

* In Chaos Cards source directory, there are several text files (e.g. "match_card_data_evolved.txt") which are data produced by simulations. They are not needed for compilation, but some are needed for running certain modes of the demo without running other long simulations first.  These files has their copies in the separate executable folder for convenience of running.

* If interested in more details about how GIGL works and wanting to set up the compilation pipeline from scratch, please refer to https://sites.google.com/a/umn.edu/grammatical-item-generation-language/downloads-and-instructions.

* For running certain long automatated simulation modes of the demo, you may want to disable most of the console outputs (e.g. the detailed game states for those simulation). Due to having GIGL in the pipeline, we did not have a very nice way except that, we can suggesting un-commenting the "#define SUPPRESS_MATCH_PROMPTS" macro at the start of both "card.xc" and "Player.h" and re-compile (in this case the outputs for game states are disabled, which wouldn't be good for human playing the game).

* In case the GIGL translation fails, or it needs to be skipped for some reason, the provided "card.generated.h" can be directly used for compilation (in which case JRE will not be needed either). In this case, make sure "card.generated.h" is not damaged or removed (if it is then revert). The downside is "card.generated.h" is less human readable (it is a machine generated file) as a source file. In addition, the modification in the entry above (about the macro) would not be effective if the GIGL part is skipped, as the provided "card.generated.h" was produced without that macro defined, and all macros are already preprocessed there. See the guides below for Windows and Unix for how the skip the GIGL part respectively for each case.

* The guides for I/O controls when running the demo can be seen in **"Demo Controls.md"** or https://sites.google.com/a/umn.edu/chaos-cards/downloads-instructions/demo-controls.


## Compilation Guides for Windows (Visual Studio)

1. Load the solution file (ChaosCards.sln) using Visual Studio (originally created in Visual Studio 2019, may be upgradedable to later versions), check the compilation configuration mode is proper (Release/Debug, x86/x64). 

2. Compile from Visual Studio. You may direct run from Visual Studio, or find the executable at wherever Visual Studio put it (but make sure also to copy the txt files to where the executable is, if running modes requiring reading data).

3. In case some error occurs with messages like "Command CL is not found", try the following. Set up the path and environment variables for Visual Studio by starting "Developer Command Prompt for VSXXXX" (XXXX is the version of Visual Studio; this step can alternatively be changing the directory to the Visual Studio installation directory by normally starting "cmd"), then enter the command "vcvarsall x86" (or "vcvarsall x86_amd64" if the target platform is x64).

4. In case of GIGL failure, select Release_SkipGigl or Debug_SkipGigl for the compilation configuration mode to skip the process of translating from "card.xc", but rather directly using the existing "card.generated.h" for compilation. If you have not changed card.xc and but changed other source files, you may also use these modes to compile to be a little faster.


## Compilation for Unix (g++)

1. Use make to compile. The command would simply be "make". To clean executable and all intermediate files (including "card.generated.h" generated by GIGL) use command "make clean". 

2. The executable from the compilation will be "card.exe" in the same directory.

3. In case of GIGL failure, use the "Makefile_SkipGigl" to skip the process of translating from "card.xc", but rather directly using the existing "card.generated.h" for compilation. The command would be instead "make -f Makefile_SkipGigl". Here, for clean up you might want to use "make -f Makefile_SkipGigl clean" instead of just "make clean" to preserve "card.generated.h".
