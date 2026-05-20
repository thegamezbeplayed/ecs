# Rat Swarm Game Design Document

## 1. High Concept

**Working Title:** *Rat Swarm*  
**Genre:** Top-down puzzle adventure with light stealth, tactics, and roguelike-inspired dungeon exploration  
**Perspective:** Top-down 2D pixel art  
**Core Fantasy:** You are a strange dungeon wizard who commands a living swarm of rats. Instead of fighting directly, you solve problems by growing, splitting, directing, sacrificing, and intimidating your rat army.

The player controls a fragile wizard who can magically command rats. The swarm is both a tool and a resource: it can crawl through spaces the wizard cannot reach, overwhelm weak enemies, scare NPCs, trigger switches, fetch keys, and manipulate dungeon systems. However, the swarm is vulnerable to traps, large enemies, fire, and careless use. The central tension is deciding when to use the rats aggressively, when to preserve them, and when to avoid danger entirely.

## 2. Design Pillars

### Swarm as Tool, Not Just Weapon
The rat swarm should feel useful in many situations beyond combat. Rats can fetch, distract, scare, squeeze through narrow gaps, hold pressure plates, and reveal hidden paths.

### Clever Avoidance Over Direct Combat
The wizard is not a warrior. Large enemies and fortified guards should feel like hazards to outsmart rather than targets to defeat. The best solution is often tricking, luring, frightening, or bypassing enemies.

### Growth Changes the Level
A small swarm can only interact with simple objects and narrow passages. A medium swarm can attack weak enemies and carry light objects. A large swarm can terrify NPCs, overpower groups, and trigger weight-based mechanisms. As the swarm grows, earlier spaces can be reinterpreted.

### Readable, Chunky Pixel Art
The game should lean into clear silhouettes, strong tile readability, expressive animations, and simple visual feedback so the player understands swarm size, danger, and puzzle state at a glance.

## 3. Target Experience

The player should feel like a mischievous dungeon tactician. Each room presents a small problem: a locked gate, a guard blocking the exit, a key inside a crawlspace, an enemy too strong to fight, or a lever behind spikes. The player studies the room, sends the rats where the wizard cannot go, and uses the swarm creatively to progress.

The tone can be darkly charming rather than grim. Rats are eerie but cute in motion. The wizard is odd, clever, and slightly pathetic without the swarm. Enemies should react with fear, disgust, arrogance, or panic depending on swarm size.

## 4. Core Loop

1. Enter a room or dungeon zone.
2. Observe exits, obstacles, enemies, NPCs, keys, switches, traps, and visible ratholes.
3. Command the rat swarm to scout, fetch, attack, distract, or activate mechanisms.
4. Grow the swarm by finding ratholes or rescuing captive rats.
5. Avoid hazards that can reduce swarm size or kill the wizard.
6. Solve the room objective and unlock the next space.
7. Collect items, upgrades, or spell modifiers that expand future puzzle options.

## 5. Player Character: The Wizard

### Role
The wizard is the central controllable character. They are physically weak but can channel magic through rats. The wizard must usually reach the exit personally, but often cannot do so without the swarm solving problems first.

### Baseline Abilities
- Walk around top-down dungeon rooms.
- Interact with doors, chests, altars, signs, NPCs, and level exits.
- Command the rat swarm to move to a target point.
- Recall the swarm back to the wizard.
- Highlight valid swarm interaction points such as levers, crawlspaces, pressure plates, enemy targets, and fetchable items.
- Cast limited utility spells that modify swarm behavior.

### Weaknesses
- Low health.
- Poor direct combat capability.
- Cannot enter narrow crawlspaces.
- Cannot pass through locked doors, enemy formations, or traps without help.
- Vulnerable when separated from the swarm.

### Possible Wizard Personality
The wizard could be a failed court mage, sewer mystic, plague scholar, or dungeon hermit. Their power is unusual and unsettling, but they treat the rats like trusted apprentices.

## 6. Rat Swarm System

### Swarm Size
The swarm is represented as a count or tier. The exact number can be abstracted for readability.

**Tiny Swarm**
- 1-5 rats.
- Can squeeze through narrow passages.
- Can fetch tiny keys or gems.
- Cannot fight effectively.
- Does not scare NPCs.

**Small Swarm**
- 6-15 rats.
- Can pull light levers.
- Can hold small pressure plates.
- Can distract one weak enemy.
- Can carry small items slowly.

**Medium Swarm**
- 16-35 rats.
- Can attack small enemies.
- Can scare timid NPCs.
- Can carry keys, bones, small crates, and magic components.
- Can split into two smaller groups for multi-switch puzzles.

**Large Swarm**
- 36-70 rats.
- Can make many NPCs flee.
- Can overwhelm weak enemy groups.
- Can push objects or block paths.
- Can trigger heavy pressure plates.
- Becomes harder to maneuver through hazards.

**Massive Swarm**
- 70+ rats.
- Can cause room-wide panic.
- Can break fragile barricades.
- Can temporarily block large enemies.
- Draws more attention and is vulnerable to area hazards.

### Growing the Swarm
The swarm grows by finding ratholes throughout levels. Ratholes can be placed as rewards, hidden resources, or puzzle tools.

Possible rathole types:
- **Common Rathole:** Adds a small number of rats.
- **Nest Rathole:** Adds many rats but may alert enemies.
- **Locked Rathole:** Requires a key, spell, or environmental action to open.
- **Cursed Rathole:** Adds rats with a drawback, such as attracting predators.
- **Shortcut Rathole:** Allows the swarm to travel between connected holes.

### Losing Rats
Rats are a spendable but valuable resource. The player should be allowed to recover from small losses, but repeated mistakes should make puzzles harder.

Rats can be lost by:
- Attacking enemies that are too strong.
- Crossing spikes, fire, poison, crushing traps, or water.
- Being caught by cats, goblin trappers, or giant rats.
- Triggering sacrificial mechanisms.
- Staying too long in hazardous zones.

### Swarm Commands
The control scheme should be simple and readable.

Core commands:
- **Send:** Move swarm to target location.
- **Recall:** Return swarm to wizard.
- **Interact:** Use swarm on highlighted object.
- **Attack:** Target a vulnerable enemy.
- **Fetch:** Bring a small object back.
- **Split:** Divide swarm into groups for multi-objective puzzles.
- **Regroup:** Merge split groups back into one swarm.

Optional advanced commands:
- **Scatter:** Rats spread out to avoid area attacks.
- **Hide:** Rats enter nearby holes or shadows.
- **Swarm Shape:** Form a bridge, blockade, or pressure mass.
- **Follow Mode:** Swarm follows closely behind wizard.
- **Guard Mode:** Swarm stays near wizard and attacks weak threats automatically.

## 7. Puzzle Design

Puzzles should combine spatial reasoning, enemy behavior, and swarm resource management.

### Puzzle Type: Narrow Passage Fetch
A key, switch, or item is behind bars or inside a tiny tunnel. The wizard cannot enter, but rats can. The player sends the swarm through, avoids hazards, collects the item, and returns.

Variations:
- Timed gate closes after switch activation.
- Tunnel contains rat traps.
- Item is too heavy for a tiny swarm, requiring more rats.
- Tunnel branches, with one path leading to extra rats and another to the objective.

### Puzzle Type: Enemy Displacement
An enemy blocks the exit. The player cannot beat them directly. The solution is to scare, lure, distract, or trick them into moving.

Examples:
- Grow swarm large enough to make a guard flee.
- Send a small swarm to steal food from a goblin, causing them to chase the rats away from a door.
- Trigger a noise-making object that draws an enemy to another room.
- Use rats to push a lure item near a trap, then bait the enemy onto it.

### Puzzle Type: Pressure Plate Coordination
The wizard and swarm must stand on different switches at the same time.

Variations:
- The swarm must split into two groups.
- One plate requires a large swarm to weigh it down.
- A plate opens a gate but exposes rats to danger.
- The wizard must cross while the rats hold the mechanism.

### Puzzle Type: Swarm Size Gating
The player sees a goal but needs enough rats to interact with it.

Examples:
- Five rats can retrieve a tiny key.
- Fifteen rats can pull a lever.
- Thirty rats can scare an NPC.
- Fifty rats can push a crate.
- Seventy rats can break a weak barricade.

### Puzzle Type: Avoid the Predator
Large enemies or predators cannot be defeated. The player must navigate the swarm around them.

Examples:
- A giant rat kills rats on contact.
- A goblin leader uses sweeping attacks that delete chunks of the swarm.
- A torch-bearing guard creates dangerous fire zones.
- A cat patrols narrow corridors and forces careful timing.

### Puzzle Type: Sacrifice vs Reward
Some optional paths require losing rats for loot, shortcuts, or upgrades.

Examples:
- Send rats through spikes to retrieve a rare charm.
- Use rats to jam a crusher permanently, losing some in the process.
- Sacrifice part of the swarm to feed a monster and slip past.

## 8. Combat and Enemy Rules

Combat should be fast, readable, and primarily puzzle-oriented. The player should not be expected to kite enemies for long periods. Instead, combat outcomes should be based on matchup clarity: small enemies are valid targets, large enemies are hazards.

### Enemy Categories

**Tiny Threats**
- Bats, spiders, slimes, small insects.
- Can be overwhelmed by small or medium swarms.
- Teach attack mechanics early.

**Small Enemies**
- Goblin scouts, weak skeletons, sewer pests.
- Can be defeated by medium swarms.
- May damage the swarm before dying.

**Large Enemies**
- Armored goblins, giant rats, ogres, heavy guards.
- Dangerous to the swarm.
- Should usually be avoided, distracted, trapped, or scared indirectly.

**Swarm-Fearing NPCs**
- Servants, merchants, cowardly guards, cultists.
- Flee when the swarm reaches a visible size threshold.
- Can be used as moving puzzle pieces.

**Swarm-Resistant NPCs**
- Priests, rat catchers, alchemists, bosses.
- Do not flee from size alone.
- Require special tactics or environmental manipulation.

### Example Enemy Behaviors

**Goblin Guard**
- Patrols a hallway.
- Blocks the exit if alerted.
- Will chase a small swarm if it steals an item.
- Flees from a large swarm unless near a leader.

**Goblin Trapper**
- Places rat traps.
- Dangerous to careless swarm movement.
- Can be distracted while the wizard sneaks past.

**Huge Rat**
- Territorial predator.
- Attacks the swarm, not the wizard first.
- Guards food piles, ratholes, or tunnels.
- May be lured away with cheese or bones.

**Torch Guard**
- Carries fire, creating a moving danger zone.
- Rats avoid flame unless forced.
- Can burn barricades, creating indirect puzzle solutions.

**Cowardly Noble**
- Blocks a door or holds a key.
- Runs from medium or large swarm.
- May run into a new position that unlocks or blocks routes.

## 9. Objectives and Level Goals

Levels should have clear primary objectives and optional secondary goals.

### Primary Objectives
- Reach the exit.
- Unlock the dungeon gate.
- Retrieve a key from a restricted area.
- Trick enemies away from the exit path.
- Rescue rats from cages or nests.
- Activate ancient switches using swarm coordination.
- Collect a required magical item.

### Optional Objectives
- Finish without losing more than a certain number of rats.
- Find all hidden ratholes.
- Collect optional treasure.
- Avoid scaring innocent NPCs.
- Complete a level without direct attacks.
- Rescue a rare albino rat.

## 10. Progression

Progression should expand what the swarm can do without overcomplicating the command scheme.

### Wizard Upgrades
- **Longer Command Range:** Send rats farther away.
- **Stronger Recall:** Rats return faster and avoid hazards better.
- **Split Command:** Divide swarm into two or more groups.
- **Rat Sense:** Reveals hidden ratholes, crawlspaces, and smell trails.
- **Fear Aura:** Lowers the swarm size needed to scare NPCs.
- **Swarm Shield:** Rats can briefly protect the wizard from weak attacks.

### Swarm Upgrades
- **Carry Training:** Rats can move heavier objects.
- **Bite Frenzy:** Better against weak enemies.
- **Soft Paws:** Reduced trap activation chance.
- **Sewer Memory:** Rats can use discovered ratholes as shortcuts.
- **Pack Discipline:** Less scatter when damaged.
- **Plague Spark:** Rats can disable magical wards or contaminate food lures.

### Consumable Items
- **Cheese:** Lure rats or enemies.
- **Rotten Meat:** Distract predators.
- **Smoke Bomb:** Hide wizard or swarm briefly.
- **Rat Whistle:** Instant recall.
- **Bone Charm:** Prevents a small amount of swarm loss.
- **Lantern Oil:** Creates or removes fire-based hazards depending on use.

## 11. Level Structure

The game can be organized as a series of handcrafted dungeon rooms grouped into themed zones. Each zone introduces new mechanics and remixes previous ones.

### Zone 1: The Cellar
Focus: Basic swarm movement, ratholes, fetching keys, simple switches.  
Enemies: Bats, weak goblins, cowardly servants.  
Goal: Escape the wizard's prison or laboratory.

### Zone 2: The Sewers
Focus: Narrow passages, predator avoidance, water hazards, connected ratholes.  
Enemies: Huge rats, slimes, rat catchers.  
Goal: Build the swarm and unlock the route into the keep.

### Zone 3: The Goblin Barracks
Focus: Enemy displacement, patrol manipulation, traps, intimidation.  
Enemies: Goblin guards, goblin trappers, torch guards.  
Goal: Sneak through enemy territory without being overwhelmed.

### Zone 4: The Crypt Library
Focus: Magical switches, cursed ratholes, split-swarm puzzles, undead enemies.  
Enemies: Skeletons, cursed mages, animated books.  
Goal: Recover a spell that deepens the wizard's control over the swarm.

### Zone 5: The Throne Burrow
Focus: Large swarm management, multi-room puzzles, high-risk hazards.  
Enemies: Goblin leader, giant rat queen, elite guards.  
Goal: Command the full rat army to escape, overthrow, or claim the dungeon.

## 12. Example Level Walkthrough

### Level: The Guarded Pantry

**Setup:**  
The wizard enters a pantry connected to a locked exit. A cowardly guard stands in front of the exit door. A key is visible behind a narrow grate. A rathole is hidden behind a stack of crates. A huge rat patrols near a food pile.

**Player Actions:**
1. Send the tiny swarm through the grate to retrieve the key.
2. Discover the hidden rathole by exploring near the crates.
3. Grow the swarm to medium size.
4. Avoid the huge rat, which would kill many rats if approached.
5. Send the swarm near the guard until he panics and flees from the exit.
6. Use the key on the exit door.
7. Optionally lure the huge rat away from the food pile to collect a bonus item.

**Lessons Taught:**
- Rats can fetch items through narrow spaces.
- Ratholes grow the swarm.
- Some enemies should be avoided.
- NPCs can be moved through fear.

## 13. Art Direction

The game should use a top-down pixel art style with clear dungeon readability. The referenced CraftPix roguelike kit is a strong starting point because it includes top-down pixel art tiles, UI elements, animated objects such as chests, doors, traps, and lockers, plus character and enemy sprites including a wizard, goblins, and a huge rat.

### Visual Priorities
- Distinct silhouettes for wizard, rats, goblins, guards, and huge enemies.
- Clear color language for interactable objects.
- Obvious danger tiles such as spikes, fire, poison, and traps.
- Swarm size readable at a glance.
- NPC fear state shown through animation, icons, or exaggerated movement.

### Rat Swarm Visuals
The swarm can be represented by many small rat sprites moving as a loose cluster. To keep performance and readability manageable, the game can display a capped number of visible rats while tracking the true swarm count internally.

Possible visual states:
- Calm follow formation.
- Directed stream toward a target.
- Attack cloud around a weak enemy.
- Panic scatter when damaged.
- Dense mass when holding a pressure plate.
- Fear aura when large enough to intimidate NPCs.

## 14. User Interface

### HUD Elements
- Wizard health.
- Current swarm size or tier.
- Rat loss warning indicator.
- Current command mode.
- Objective text.
- Collected keys/items.
- Optional objective progress.

### Interaction Feedback
- Highlight objects rats can interact with.
- Show required swarm size for interactions.
- Show danger warnings when targeting a strong enemy or hazard.
- Display NPC fear threshold when relevant.
- Use simple icons for fetch, attack, scare, split, and recall.

## 15. Controls

### Keyboard and Mouse Concept
- WASD: Move wizard.
- Left click: Send swarm / interact with target.
- Right click: Recall swarm.
- Space: Wizard interact.
- Shift: Split or regroup swarm.
- Number keys: Select swarm group.
- Tab: Toggle command mode or highlight interactables.

### Controller Concept
- Left stick: Move wizard.
- Right stick: Aim swarm command cursor.
- Right trigger: Send swarm.
- Left trigger: Recall swarm.
- Face button: Interact.
- Shoulder buttons: Cycle swarm groups.
- D-pad: Select command mode.

## 16. Audio Direction

Audio should reinforce the swarm fantasy and puzzle readability.

### Key Sounds
- Soft scratching and squeaking for swarm movement.
- Rising skitter intensity as swarm size grows.
- Sharp squeals when rats are harmed.
- Magical whistle or chime for wizard commands.
- Panicked vocal barks when NPCs flee.
- Heavy stomps or growls for enemies that should be avoided.

### Music Direction
- Mischievous dungeon ambience.
- Low strings, plucked instruments, sewer drips, and faint magical tones.
- Combat should increase tension but not become action-heavy.
- Puzzle completion should use short satisfying stingers.

## 17. MVP Scope

A strong prototype can be built with a small number of mechanics and levels.

### MVP Features
- Wizard movement.
- One controllable rat swarm.
- Swarm size count.
- Ratholes that add rats.
- Narrow passage fetch interaction.
- Pressure plate interaction.
- Weak enemy attack interaction.
- Strong enemy avoidance.
- NPC fear behavior based on swarm size.
- Locked door and key objective.
- Three to five handcrafted levels.

### MVP Enemies/NPCs
- Weak bat or slime.
- Goblin guard.
- Huge rat predator.
- Cowardly NPC who flees from large swarm.

### MVP Win Condition
Reach each level exit by using the swarm to collect keys, move enemies, activate switches, and avoid dangerous enemies.

## 18. Stretch Features

- Multiple swarm groups.
- Connected rathole fast travel.
- Upgrade shop or spellbook.
- Boss encounters built around puzzle solutions.
- Procedural room variants.
- Optional stealth scoring.
- Different rat types with special abilities.
- Wizard dialogue and NPC reactions.
- Environmental chain reactions.
- Swarm morale or hunger system.

## 19. Risks and Design Challenges

### Swarm Control Complexity
The game must avoid overwhelming the player with too many commands. Early prototypes should test whether send, recall, fetch, attack, and split are enough.

### Readability
A moving mass of rats can become visually noisy. The game needs strong outlines, simple formations, and clear target indicators.

### Puzzle Softlocks
Because rats can die, levels must avoid unwinnable states or provide recovery options. Possible solutions include replenishing ratholes, restart-room buttons, or minimum swarm regeneration.

### Enemy Clarity
Players must understand which enemies are safe to attack and which should be avoided. Use size, animation, sound, UI warnings, and early tutorials to teach this.

### Performance
Rendering dozens of individual rats may be expensive. The game can fake large swarms by mixing individual sprites near the edges with animated cluster sprites near the center.

## 20. Open Questions

- Is the game level-based, roguelike, or a handcrafted adventure with roguelike visuals?
- Should rat loss be permanent across levels or reset per room?
- Can the wizard die, or is losing the swarm the main failure state?
- Should the player directly control the wizard at all times, or switch between wizard and swarm control?
- Is the tone comedic, creepy, heroic, or villainous?
- Should the swarm be morally ambiguous, cute companions, or disposable minions?
- How many commands can the control scheme support before it becomes too complex?
- Should levels support multiple solutions or mostly designed puzzle answers?

## 21. Immediate Next Steps

1. Build a graybox prototype with one wizard, one swarm, one rathole, one key, one pressure plate, one weak enemy, one strong enemy, and one fleeing NPC.
2. Test whether commanding the swarm feels fun with only send and recall.
3. Add fetch and fear interactions.
4. Create three small rooms that each teach one mechanic.
5. Tune swarm size thresholds until growth feels meaningful but not grindy.
6. Replace graybox assets with the top-down roguelike pixel art kit once mechanics are readable.
