# Wiiero Reboot - A 2D Platform Game Engine for Wii, Linux and MacOS

```
    __      _ .__  .__                           
   /  \    / \(__) (__)  ____ ______   ____              _____
   \   \/\/  /|  | |  |_/ __ \\_  _ \ /  _ \            / -   \
    \       //   |/   |\  ___/ |  |\/(  <_> )   ,_______\_    /
     \__/\_/ \__ \\__ \ \___  >|__|   \____/    '--===--[_)~  \__/\
                \/   \/     \/  by BScrk                   \______/

Release 1.4                                                REBOOT
```

Wiiero is under GPL licence.

Wiiero is Liero game clone developed for Wii platform.
Wiiero is also available on PSP and PC (Linux/Windows/MacOS) platform.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

---

## About the Game

Wiiero is a clone of Liero specially developed for Wii.

Liero is a fight game between two worms armed with heavy weapons of all kinds battling in underground caves. It was created by the Finnish programmer Joosa Riekkinen in 1998. The game is often described as a real-time version of Worms, or a 2D version of Quake III. Its last release was version 1.33, released in 1999.

You play Wiiero exactly the same way as you played Liero: same game modes, same original weapons, coming with new ones like vermifuge, holy grenade, or even plasma canon! Wiiero is a two player game, using the wiimote alone or both wiimote and nunchuk on the Wii and keyboard on PC.

## Game Modes

In Wiiero game you can fight your opponent in 3 different game modes: Deathmatch, Capture the flag and Game of tag.

* **Deathmatch**: The goal of the "deathmatch" mode is simply killing the enemy by hurting him the most possible. Lives are limited and the player with no more lives will be the loser. You can define the number of lives for both players in the "option" menu.

* **Game of Tag**: The goal of the "game of tag" is to kill the enemy for him to become the "tag". When a player is the "tag", his time counter decreases. When your counter reaches 0, the game is over and you lost. However, you can kill your enemy before that time and he becomes the "tag", his counter will therefor begin decreasing. The time limit can be modified in the "option" menu.

* **Capture the Flag**: Finally, in the "capture the flag" mode, you will need to get your opponent's flag somewhere on the map and take it back to your home, giving you 1 point. However, to pick up your opponent's flag, he must not be in possession of your own flag. Otherwise you will need to kill him! The first player accumulating the defined number of flags wins. You can modify the limit of flags in the "option" menu.

## Todo
Macro todo list :
* SDL 1.2 Mac / Linux build project ✅
* Mac gamepads supports via IOKit ✅
* **SDL2 upgrade ✅** (branch: migration/sdl2)
* Mac builds (SDL2) ✅
* Windows port & builds ⏳
* Linux port & builds ⏳
* Linux gamepads supports (via SDL 2) ⏳
* Mac gamepads supports (SDL2 + IOKit) ✅
* Win gamepads supports (via SDL 2) ⏳
* Gamepads config option ⏳


## Installation / Build Guides

### Linux Guide

#### Installation

```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev makedepend
```

#### Build

```bash
make clean
make depend
make compil
```

#### Run

```bash
./bin/wiiero
```

### MacOS Guide

#### Installation

Wiiero requires SDL2, SDL2_image and SDL2_mixer libraries. Install them using Homebrew:

```bash
brew install sdl2 sdl2_image sdl2_mixer makedepend
```

**Note**: The Makefile automatically detects macOS (both Intel and Apple Silicon) and configures the appropriate paths using Homebrew and SDL2.

#### Build

The build system automatically detects your platform and uses the correct flags:

```bash
make clean
make depend
make compil
```

For development/debug build with profiling:

```bash
make dev
```

#### Run

```bash
./bin/wiiero
```

#### Controls

Wiiero on macOS supports both **keyboard** and **gamepads** (Xbox, PlayStation, Switch Pro controllers via IOKit HID).

##### Keyboard Controls

**Player 1:**

| Action | Key |
|--------|-----|
| Move Up | `I` |
| Move Down | `K` |
| Move Left | `J` |
| Move Right | `L` |
| Fire | `D` |
| Jump | `A` |
| Dig (crop) | `J` + `L` |
| Show Weapon | `Z` |
| Previous Weapon | `Z` + `J` |
| Next Weapon | `Z` + `L` |
| Ninja Hook | `A` + `Z` |
| Pause | `Space` |
| Menu | `Escape` |
| OK (menu) | `D` |
| Cancel (menu) | `A` |

**Player 2 (Numpad):**

| Action | Key |
|--------|-----|
| Move Up | `8` (Numpad) |
| Move Down | `5` (Numpad) |
| Move Left | `4` (Numpad) |
| Move Right | `6` (Numpad) |
| Fire | `Page Up` |
| Jump | `Insert` |
| Dig (crop) | `4` + `6` (Numpad) |
| Show Weapon | `Home` |
| Previous Weapon | `Home` + `4` |
| Next Weapon | `Home` + `6` |
| Ninja Hook | `Insert` + `Home` |
| Pause | `Space` |
| Menu | `Escape` |
| OK (menu) | `Page Up` |
| Cancel (menu) | `Insert` |

**Global:**
- `Tab` - Flip screen mode
- `Escape` - Return to menu

##### Gamepad Controls

Wiiero supports most HID-compliant gamepads (Xbox, PlayStation, Switch Pro) with automatic detection.

| Action | Button |
|--------|--------|
| Move | Left Analog Stick |
| Fire | `Y` button |
| Jump | `B` button |
| Dig (crop) | `A` button |
| Show Weapon | `X` button (hold) |
| Previous Weapon | `X` + Left Stick ← |
| Next Weapon | `X` + Right Stick → |
| Ninja Hook | `L` or `R` triggers |
| Pause | `Select` / `Back` button |
| Menu | `Start` button |
| OK (menu) | `Y` or `B` |
| Cancel (menu) | `A` or `X` |

**Notes:**
- Gamepads are auto-detected when connected
- Player 1 uses first gamepad, Player 2 uses second gamepad
- Keyboard fallback is always available
- Both players can use keyboard + gamepad simultaneously


### Wii Guide

#### Installation

If your are using the Homebrew Channel you can just copy the "Wiiero" directory from this downloaded archive to the apps directory of the root of your SD card. 


#### Controls

##### Wiimote Only (Horizontal mode)

| Action | Button |
|--------|--------|
| fire | 2 |
| jump | 1 |
| crop | B |
| show weapon | A |
| prev weapon | A + 1 |
| next weapon | A + 2 |
| ninja hook | 1 + 2 |
| up | Cross Up |
| down | Cross Down |
| left | Cross Left |
| right | Cross Right |
| pause | Plus |
| menu | Home |
| ok | 2 |
| cancel | 1 |

##### Wiimote & Nunchuck (Vertical mode)

| Action | Button |
|--------|--------|
| fire | B |
| jump | C |
| crop | A |
| show weapon | Cross Down |
| prev weapon | Cross Left |
| next weapon | Cross Right |
| ninja hook | Z |
| up | Nunchuck Up |
| down | Nunchuck Down |
| left | Nunchuck Left |
| right | Nunchuck Right |
| pause | Minus |
| menu | Home |
| ok | A |
| cancel | B |

##### GameCube Pad

| Action | Button |
|--------|--------|
| fire | B |
| jump | A |
| crop | X |
| show weapon | Cross Down |
| prev weapon | Cross Left |
| next weapon | Cross Right |
| ninja hook | R |
| up | Stick Up |
| down | Stick Down |
| left | Stick Left |
| right | Stick Right |
| pause | Z |
| menu | Start |
| ok | A |
| cancel | B or X |

##### Classic Pad

| Action | Button |
|--------|--------|
| fire | Y |
| jump | B |
| crop | A |
| show weapon | X |
| prev weapon | X + Left |
| next weapon | X + Right |
| ninja hook | R |
| up | Cross Up |
| down | Cross Down |
| left | Cross Left |
| right | Cross Right |
| pause | Minus |
| menu | Home |
| ok | B or Y |
| cancel | A or X |


### PSP Guide

TODO


## Credits

Joosa Riekkinen - Original Liero Author
BScrk - Wiiero Reboot Author
Have Fun !

## Changelog 

2025-01-17 BScrk (Luca Benevolo) <wiiero@free.fr>

 * Wiiero version: Release 1.5 (SDL2 Migration)
 * **Complete SDL2 migration**
   - Migrated from SDL 1.2 to SDL2
   - SDL_Window + SDL_Renderer architecture
   - Hardware acceleration support
   - Better performance and compatibility
 * Updated build system for SDL2
 * macOS and Linux SDL2 support
 * Maintained backward compatibility with existing code structure
 * IOKit gamepad support preserved

2025-10-16 BScrk (Luca Benevolo) <wiiero@free.fr>

 * Wiiero version: Release 1.4
 * Project Reboot
 * Mac OS support (sdl12-compat)
 * Game controllers supports via IOKit
 * Bugfixes
 * Better menu navigation
 * Bug Grenade added :)


2009-10-30 BScrk (Luca Benevolo) <wiiero@free.fr>

 * Wiiero version: Release 1.3
 * Deadlock on final screen fixed
 * Improved GC and Classic contollers usage
 * Coredump if GC controller pluged before wiiero start fixed


2009-10-23 BScrk (Luca Benevolo) <wiiero@free.fr>

  * Wiiero version: Release 1.2
  * Weapon names corrections
  * Rope tension control added :
    Hold "Change weapon" button down, you can now adjust the length
    of the rope by "Aim up" and "Aim down" keys.
  * "Restart game" option added
  * Classic & GC controls support added
  * Explosive weapon explode on player contact
  * Rendering engine optimisation (+15%)
  * New option menu page.
  * Sound added (Music & ingame sounds)
  * Language manager added :
    - Wiiero translation files are stored into the "lang" directory
    - You'll find a translation template into the "lang" directory, feel free
      to send me your translation files.
    - French, Italian, Hogorian and Spanish translation files added
  

2008-10-18 BScrk (Luca Benevolo) <wiiero@free.fr>

  * Wiiero version: Release 1.1
  * Wii Resolution config implemented (Warning : Can slow down the game in case of large explosions)
  * Nunchuck unplugging fixed
  * Wiimote detection improved (ingame switch on support)
  * Some English & typo correction (thanks to Flark)
  * Missile weapon command implemented
  * Some others little fixes
  * Some weapons characteristics changed
  * Weapons name reviewed
  * 1 new weapon : Controled nuke larpa !
  * Scrolling effect at the resurrection of a player 


2008-10-10 BScrk (Luca Benevolo) <wiiero@free.fr>

  * Wiiero version: Release 1.0
  * First Wii release
  * "How To Play" & "About" menu added
  * Resolution support (PC)
  * Some little fixes


2008-09-18 BScrk (Luca Benevolo) <wiiero@free.fr>

  * Wiiero version: Beta 3
  * Configuration options added
  * Load & save config implementation done
  * Performance profiling


2008-08-12 BScrk (Luca Benevolo) <wiiero@free.fr>

  * Wiiero version: Beta 2
  * First Wii Port
  * Some bug fixes
  * PC & PSP sound layer
  * Game modes implementation done


2008-07-08 BScrk (Luca Benevolo) <wiiero@free.fr>

  * Wiiero version: Beta 1
  * First PSP Port
  * Performance profiling
  * extra weapons implementation done
  * Some bug fixes


2008-06-20 BScrk (Luca Benevolo) <wiiero@free.fr>

  * Wiiero version: Alpha 1
  * Weapons implementation done


2008-05-20 BScrk (Luca Benevolo) <wiiero@free.fr>

  * beginning of the project
