# LPP 3DS Next

## Todo

**Note that a lot of things i tested are still not Working!!!**

- [X] Core Engine
  - [X] File Loading
  - [X] FTP Support on latest devkitarm
  - [X] Fix Archive.cpp
  - [X] Replace sf2d with Citro2D
  - [X] Fix Graphics Engine
- [X] Camera Module
- [X] Controls Module
- [X] Core Module
- [X] Graphics Module
- [X] Keyboard Module
- [X] Mic Module
- [X] Network Module
- [X] Render Module
- [X] Screen Module
- [X] System Module
- [X] Timer Module
- [ ] Video Module

## Building

As i switched to cmake build system, it is super easy to build the project...

Just make sure to have the folowing packages installed:

```bash
# Note that you would need devkit7 repo in pacman.conf for bannerool and makerom
pacman -S 3ds-dev 3ds-portlibs # optional bannertool makerom
```

You need `git, cmake` as well.

Then run

```bash
git submodule update --init --recursive # If not alredy done
mkdir -p build && cd build
cmake .. --toolchain /opt/devkitpro/cmake/3DS.cmake
make
```

### Optional: Update Shader

```bash
python3 tools/build_shader.py
```

## Description

**Lua Player Plus 3DS** is the first lua interpreter made for Nintendo 3DS.

The interpreter now runs under Lua 5.4.7

Official documentation: http://rinnegatamante.it/lpp-3ds_doc.html<br>
Official support board: http://rinnegatamante.it/board/forumdisplay.php?fid=12

## Features

##### Graphics

* Advanced 2D Graphics Rendering (CPU and GPU)
* Basic 3D Rendering
* Native support to TTF fonts
* Native support to BMP/PNG/JPG images
* Native support to JPGV video files
* Native support to stereoscopic 3D effect

##### Sounds

* Sound system with support for WAV/AIFF/OGG audio files
* Support to different interpolation modes (None, Linear, Polyphase)
* Complete access to Microphone for sound registration

##### Controls

* Controls system with full console support
* Built-in Keyboard system

##### Network

* Easy to use networking basic features
* Built-in Mail Sender system
* Complete sockets system support
* Built-in debug FTP server
* SSL connections support

##### System

* Access to a lot of Nintendo syscalls
* Native support to CIA files managing
* Native support to 3DSX and SMDH files
* Native support to ZIP archives
* I/O access for SD card and Extdata archives
* Native support to console Camera to take photos
* Native support to ARM9 payloads loading

##### Utilities

* Timer system
* Advanced arithmetical functions support (math library) (this is a lie cause it uses the default lua math lib)

# Nightly Builds

Not yet

# Showcase
3ds Homebrews using llp-3ds-next

* [Cookie Clicker](http://gbatemp.net/threads/w-i-p-cookie-clicker-for-3ds.460836/)

Here are some 3DS homebrews made using Lua Player Plus 3DS:

##### Games
* [Labyrinth 3D](https://rinnegatamante.it/board/showthread.php?tid=18)
* [Blackjack 3DS](http://gbatemp.net/threads/release-blackjack-3ds.398776/)
* [Asteroids 3D](https://rinnegatamante.it/board/showthread.php?tid=25)
* [PixelRoad](https://rinnegatamante.it/board/showthread.php?tid=11)
* [3DSudoku](https://rinnegatamante.it/board/showthread.php?tid=19)
* [WANT3D](http://rinnegatamante.it/board/showthread.php?tid=23)
* [4Rows](https://rinnegatamante.it/board/showthread.php?tid=32)
* [3DSHangman](https://rinnegatamante.it/board/showthread.php?tid=33)
* [3DS Briscola](https://rinnegatamante.it/board/showthread.php?tid=17)
* [EUCLIOD](http://gbatemp.net/threads/eucliod-a-homebrew-bullet-hell-for-the-3ds.419553/)
* [RPS 3D](https://rinnegatamante.it/board/showthread.php?tid=43)
* [Cookie Clicker](http://gbatemp.net/threads/w-i-p-cookie-clicker-for-3ds.460836/)
* [TriaAl](https://gbatemp.net/threads/release-triaal-deemo-clone-for-nintendo-3ds.430308/)

##### Utilities
* [CHMM2](https://rinnegatamante.it/board/showthread.php?tid=9)
* [Sunshell](https://rinnegatamante.it/board/showthread.php?tid=8)
* [ORGANIZ3D](https://rinnegatamante.it/board/showthread.php?tid=24)
* [NASA](https://rinnegatamante.it/board/showthread.php?tid=16)
* [Manga Reader](http://gbatemp.net/threads/release-manga-reader.399276/)
* [Alarm Clock](http://gbatemp.net/threads/release-alarm-clock.397624/)
* [SOON!](http://gbatemp.net/threads/soon-super-cool-installer-a-new-cool-legal-open-source-cia-installer.387268/)
* [Bird Whisperer](http://gbatemp.net/threads/release-bird-whisperer-a-notification-creater.404669/)
* [Dice 3DS](https://gbatemp.net/threads/release-dice-3ds-roll-some-dice-pretty-straightforward.400086/)
* [LiveScore 3DS](http://gbatemp.net/threads/release-livescore-3ds.414693/)
* [Tinker](http://gbatemp.net/threads/tinker-shuffle-menuhax-theme-manager.407445/)
* [MCIT](http://gbatemp.net/threads/release-miiverse-custom-image-tool.415505/)
* [Video Channels Creator](https://rinnegatamante.it/board/showthread.php?tid=10)
* [BLEND3R](https://rinnegatamante.it/board/showthread.php?tid=26)
* [Homebr3w](https://github.com/Wolvan/Homebr3w)
* [luaIrc](http://rinnegatamante.it/board/showthread.php?tid=136)
* [FileKong](https://gbatemp.net/threads/release-filekong-ftp-client-for-3ds.429071/)

##### Engines & Interpreters
* [VN3DS](http://gbatemp.net/threads/w-i-p-vn3ds-visual-novel-interpreter-for-3ds-homebrew.396564/)
* [RayCast3D Engine](https://rinnegatamante.it/board/showthread.php?tid=5)
* [Crimson Scripter](http://gbatemp.net/threads/crimson-scripter.415152/)

##### Updaters
* [StarUpdater](https://gbatemp.net/threads/release-starupdater-a-luma3ds-updater.427999/)
* [encTitleKeysUpdater](http://gbatemp.net/threads/release-enctitlekeysupdater-for-freeshop.429912/)
* [Corbenik CFW Updater](http://rinnegatamante.it/board/showthread.php?tid=85)
* [EasyRPG Updater](http://rinnegatamante.it/board/showthread.php?tid=61)

# Compiling the source

Repo with custom lua makefile later here.

# Credits

* Tobi-D7 for lpp-3ds-next, including to port everything to the latest versions of the libraries 
  and fix all the mistakes made in lpp-3ds
* Everyone involved in libctru creation
* Rinnegatamante for lpp-3ds
* fincs for citro2d
* fincs for citro3
* Sean Barrett for stb_truetype
* Everyone involved in lodepng, zlib, mpg123, libogg, vorbisfile libraries
* Misledz for the Lua Player Plus 3DS logo
* EasyRPG Team for the AudioDecoder used for MP3 support
* ksanislo for the network module improvements
* Special thanks to Aurelio and ihaveamac for testing interpreter features
