# Enter the arena

The idea is simple. **You**, the player, enter 
an underground arena (somehow) and now you have 
to fight for your life.

## The fighters
- You
- Some weird hooded midgets
- ???
- [Someone/something able to throw fireballs]
- A minotaur
- ???

## Your armaments
- Fists
- An axe
- A pickaxe
- An old revolver
- A double barrel sawnoff

## How do you fight?
You enter the arena against a preset number of enemies 
(maybe an unscripted survival mode can be added in the future).
A weapon is thrown to you based on how far in the fighting you've come.
You pick up that weapon and start figthing.
You're pretty agile, so you can move around quite a bit (and dodge, too).
You can take at most 3 hits (no HP, 3 strikes and you're out).
You can block and parry (block at the right time).
Since attacks have different types (blunt, sharp, strong, fire, etc.),
you can only block and parry some of them (depending on what weapon you're parrying with).

## What, why?
	  Look, this **isn't** some fantasy setting with deep lore,
	  this is a video game where _you stick axes in the 
	  heads of hooded midgets_ and _really angry giant cows with legs_.
	  Don't go expecting a solid setting to be established.
	  (I'm not really creative with universe design to begin with,
	   so that's the main reason.)

## What tech do you need?
- Rendering (got that)
- Lighting + Shadows (got first, need second)
- Animations
- Collision detection (do it with Bullet)

## What art do you need?
- The player character's hands (model, rig, animations)
- All the weapons (got all but without any hand animations)
- All the enemies (Got midgets and minotaur, need 3 others)
- The arena
- Maybe some voices (I'll do those, I don't really want to waste other people's time).
- Sounds (Guns, shells, footsteps, ambiance, music).

## What's the visual style?
Flat colors seem to work pretty well, so that's probably what's going to stay.
Another posibility is low-resolution textures, 64x64 textures with nearest neighbour filtering.

## What's the realistic time frame this could come out in?
The goal is just before summer begins (so the end of May).
Since I haven't really developed a game before, 
I don't know what's realistic, only what the goal is.

-------

## THE MENUS

+--------------------+
|   ++-----------+   |
|   || MAIN MENU |   |     +------------------------------------+
|   ++-----------+   |     |              SETTINGS              |
|                    |     +------------------------------------+
|   ++-----------+   |     |                                    |
|   ||   PLAY    |   |     |  Resolution: ______ by ______      |
|   ++-----------+   |     |                                    |
|   ++-----------+   |     |  Sound volume: |---------o--| 95%  |
|   || SETTINGS  |   |     |                                    |
|   ++-----------+   |     |  Music volume: |------o-----| 50%  |
|   ++-----------+   |     |                                    |
|   ||   EXIT    |   |     |       [SAVE]         [BACK]        |
|   ++-----------+   |     |                                    |
|                    |     +------------------------------------+
+--------------------+

------
