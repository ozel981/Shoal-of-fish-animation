##ENGLISH## (Instrukcja po polsku poniżej)

# Shoal-of-fish-animation
Program animate shoal of fish.

Fish kinds:
Red - fast
White - stadard
Yellow - small
Green - big, no grouping, no common direction with another fish

All fish (except the green ones) in the school group together and stick to the same direction. 

Program work on CPU or GPU.

# How to run

- Copy freeglut.dll [Shoal-of-fish-animation/ShoalOfFishAnimation/ShoalOfFishAnimation/lib-vc2017/freeglut.dll] to Windows folder on your system disk.

- If any problem with running set VS Project Properties: 
  1. Configuration Properties -> General -> Platform Toolset set to Visual Studio 2015 (v140)
  2. C/C++ -> General -> Additional Include Directories -> schoud contains $(ProjectDir)include and $(CudaToolkitIncludeDir)
  3. Linker -> General -> Additional Library Directories -> schoud contains $(ProjectDir)lib-vc2017 and $(CudaToolkitLibDir)
  4. Linker -> Input -> Additional Dependencies -> shoud contains freeglut.lib; glew32s.lib; glfw3.lib; opengl32.lib; cudart.lib; kernel32.lib; user32.lib; gdi32.lib; winspool.lib; comdlg32.lib; advapi32.lib; shell32.lib; ole32.lib; oleaut32.lib; uuid.lib; odbc32.lib; odbccp32.lib; %(AdditionalDependencies)

# Instructions

- Space stops/starts the animation.
- File Constant.h [Shoal-of-fish-animation/ShoalOfFishAnimation/ShoalOfFishAnimation/Source/Constant/Constant.h] contains all fish paramiters like (count, view range, size) and window paramiters like (size, and inner matrix size)
- 
The fish run away from the mouse pointer.

# Requirements

- CUDA liblary at least 9.0v
- GPU NVIDIA Driver

##POLSKI##

# Animacja ławicy ryb

Program przedstawia animacje ławicy ryb.

Rodzaje ryb:
Czerwone-szybkie
Białe-standardowe
Żółte-małe (z racji na rozmiar często grupują się w centrach innych ławic)
Zielone-duże, nie grupują się, i nie kierują się tam, gdzie ich sąsiedzi.

Wszystkie ryby (poza zielonymi) grupują się w ławice, a także płyną w średnim kierunku ich lokalnych sąsiadów (lokalni-w zasięgu widoku).

Program działa na CPU i GPU

# Jak uruchomić

- Proszę skopiować freeglut.dll [Shoal-of-fish-animation/ShoalOfFishAnimation/ShoalOfFishAnimation/lib-vc2017/freeglut.dll] do folderu Windows na dysku systemowym.

- Jeżeli aplikacja dalej nie chce się skompilować/uruchomić VS Project Properties:
1. Configuration Properties -> General -> Platform Toolset proszę ustawić Visual Studio 2015 (v140)
2. C/C++ -> General -> Additional Include Directories -> powinno zawierać $(ProjectDir)include and $(CudaToolkitIncludeDir)
3. Linker -> General -> Additional Library Directories -> powinno zawierać $(ProjectDir)lib-vc2017 and $(CudaToolkitLibDir)
4. Linker -> Input -> Additional Dependencies -> powinno zawierać freeglut.lib; glew32s.lib; glfw3.lib; opengl32.lib; cudart.lib; kernel32.lib; user32.lib; gdi32.lib; winspool.lib; comdlg32.lib; advapi32.lib; shell32.lib; ole32.lib; oleaut32.lib; uuid.lib; odbc32.lib; odbccp32.lib; %(AdditionalDependencies)

# Instrukcja

- Space wstrzymuje/rozpoczyna animacje.
- Plik Constant.h [Shoal-of-fish-animation/ShoalOfFishAnimation/ShoalOfFishAnimation/Source/Constant/Constant.h] zawiera wszystkie parametry ryb jak np. (rozmiar, zasięg widzenia itp) parametry okna jak np. (rozmiar, rozmiar wewnętrznej matrycy)
- Ryby uciekają przed wskaźnikiem myszy.

# Wymagania

- biblioteka CUDA, przynajmniej 9.0v
- sterownik karty graficznej NVIDIA
