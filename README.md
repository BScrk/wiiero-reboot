# Wiiero Reboot - A 2D Platform Game Engine for Wii, Linux and MacOS

```
    __      _ .__  .__                           
   /  \    / \(__) (__)  ____ ______   ____              _____
   \   \/\/  /|  | |  |_/ __ \\_  _ \ /  _ \            / -   \
    \       //   |/   |\  ___/ |  |\/(  <_> )   ,_______\_    /
     \__/\_/ \__ \\__ \ \___  >|__|   \____/    '--===--[_)~  \__/\
                \/   \/     \/  by BScrk                   \______/

Release 1.6 (4 players)                                       REBOOT
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

## 4 Players Game Modes

Wiiero offers multiple competitive game modes for 4 players locally with both **Free-For-All (FFA)** and **Team-based (2v2)** gameplay:

### Deathmatch (FFA - 4 Players)

The classic elimination mode where every player fights for themselves. Aggressive combat, weapon mastery, and survival skills are key. Use the terrain to your advantage and watch out for splash damage!

**Objective:** Be the last worm standing by eliminating all opponents.

**Rules:**
- Each player starts with a limited number of lives (configurable in options)
- Every death costs one life
- When a player runs out of lives, they're eliminated
- Last player with lives remaining wins
- Suicide penalties apply!

---

### Game of Tag (FFA - 4 Players)

A frantic elimination mode where time is your enemy. Stay aggressive when tagged, evasive when safe. Time management is critical, don't let the clock run out on you!

**Objective:** Avoid being "IT" when the timer runs out.

**Rules:**
- One random player starts as "IT" (tagged)
- The tagged player's time counter continuously decreases
- Kill another player to transfer the tag to them
- When a player's timer reaches 0, they're **eliminated** (Game Over)
- Last player alive wins!

---

### Game of Tag: Team Battle (2v2 - 4 Players)

Cooperative tag gameplay with team dynamics! Time is pooled: if P1 has 30s and P3 has 60s, the team has 90s total
Protect your teammate, killing enemies transfers the tag. Communication is key!

**Teams:**
- **Team 1:** Player 1 (P1) + Player 3 (P3)
- **Team 2:** Player 2 (P2) + Player 4 (P4)

**Objective:** Prevent your team from running out of time.

**Rules:**
- Team time is **shared** between teammates
- When a team member is tagged, the **team's collective time** decreases
- Tag transfers between any players (even teammates can accidentally tag each other!)
- When a team's total time reaches 0, that team loses
- **Respawning:** Dead players respawn but automatically become tagged

---

### Capture the Flag: Team Battle (2v2)

Classic CTF with intense team-based flag stealing action. In Capture the Flag, teamwork is everything, one player strikes fast to steal the enemy flag while their teammate lays down cover fire and clears the path. Defend your own base, intercept enemy carriers, and control key chokepoints to dominate the map. Every move counts, every second matters... communicate, coordinate, and crush the opposition. 
Victory belongs to the team that moves as one.

**Teams:**
- **Team 1:** Player 1 (P1) + Player 3 (P3)
- **Team 2:** Player 2 (P2) + Player 4 (P4)

**Objective:** Capture the enemy flag and return it to your base!

**Map Setup:**
- 2 Team Bases (houses) - one per team
- 2 Flags - one per team

**Rules:**
- **Capture:** Touch the enemy flag to pick it up
- **Score:** Return the enemy flag to YOUR team's base while carrying it
- **Defense:** Only one player can carry a flag at a time
- **Drop mechanic:** If a flag carrier dies, the flag drops at their location
- First team to reach the flag limit wins (configurable in options)
- **Team scoring:** Flags captured by P1 or P3 count for Team 1, P2 or P4 for Team 2


---

**Note:** All game modes support customizable options including time limits, lives, flag counts, weapon loadouts, and map settings. Configure these in the Options menu before starting a match!

## Todo
Macro todo list :
* SDL 1.2 Mac / Linux build project ✅
* Mac gamepads supports via IOKit ✅
* **SDL2 upgrade ✅** (branch: migration/sdl2)
* Mac builds (SDL2) ✅
* Linux port & builds ✅
* Linux gamepads supports (via SDL 2) ✅
* Mac gamepads supports (SDL2 + IOKit) ✅
* 4 players Version ✅
* Windows port & builds ⏳
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

Wiiero supports most HID-compliant gamepads (Xbox, PlayStation, Switch Pro) with automatic detection via SDL2 GameController API.

| Action | Button |
|--------|--------|
| Move | D-Pad or Left Analog Stick |
| Aim | Right Analog Stick (360°) |
| Fire | `B` (Circle) or `R2` Trigger |
| Jump | `A` (Cross) or `L3` (Left Stick Click) |
| Dig (crop) | `X` (Square) |
| Show Weapon | `Y` (Triangle) or `L1`/`R1` |
| Previous Weapon | `Y` + D-Pad ← or `L1` + D-Pad ← |
| Next Weapon | `Y` + D-Pad → or `R1` + D-Pad → |
| Ninja Hook | `L2` Trigger or `R3` (Right Stick Click) |
| Pause | `Start` button |
| Menu | `Back` / `Select` button |
| OK (menu) | `B` (Circle) |
| Cancel (menu) | `A` (Cross) or `X` (Square) |

**Notes:**
- Gamepads are auto-detected when connected (hot-plug supported)
- Supports up to 4 gamepads simultaneously (Player 1-4)
- Right stick provides 360° analog aiming
- Keyboard fallback is always available 
- Players 1 and 2 can mix keyboard + gamepad input


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

BScrk - Wiiero Author

BScrk & Kyfran- 4 Players Wiiero Reboot

Sound Track - Binaerpilot www.binaerpilot.no

Have Fun !


## Changelog 


2025-10-20 BScrk & Kyfran <wiiero@free.fr>

 * Wiiero version: Release 1.6 (4 Players Edition)
 * **4 Players Local Multiplayer**
   - Complete refactoring for 4 simultaneous players support
   - Split-screen rendering with 4 independent cameras
   - Individual player HUD and status display
   - Support for up to 4 gamepads simultaneously
 * **Enhanced Game Modes**
   - Deathmatch: 4-player Free-For-All battle royale
   - Game of Tag: 4-player elimination mode with time pressure
   - Game of Tag Team (2v2): Cooperative team-based tag gameplay
   - Capture the Flag Team (2v2): Classic CTF with team dynamics
   - Team composition: Team 1 (P1+P3) vs Team 2 (P2+P4)
 * **Enhanced Gamepad Support**
   - SDL2 GameController API integration
   - Auto-detection and hot-plug support for all gamepads
   - Xbox, PlayStation, Switch Pro controllers compatibility
   - 360° analog aiming with right stick
   - Dual analog controls (movement + aim)
 * **Audio System Improvements**
   - SDL2_mixer integration
   - Music playback support
   - In-game sound effects
   - Improved audio performance
 * **Bug Fixes**
   - Fixed player visibility issues after game restart
   - Fixed camera focus synchronization for multiplayer
   - Fixed pointer conversion warnings in weapons system
   - Improved rendering performance for 4 viewports
 * **Gameplay Enhancements**
   - Enhanced weapon selection interface for 4 players
   - Optimized collision detection for multiplayer
   - Improved player spawning system
   - Better game mode support (Deathmatch, CTF, Tag) for 4 players
 * **Code Quality**
   - Modernized codebase architecture
   - Better memory management
   - Enhanced cross-platform compatibility
   - Improved debug and profiling tools

2025-01-17 BScrk

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

2025-10-16 BScrk

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
