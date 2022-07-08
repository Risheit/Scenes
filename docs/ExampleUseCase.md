# Example Use Case for Scenes
The following covers how Scenes may be used to run a text-based RPG. 
Areas and locations are created through Scenes, and current game states are decided through Sections reading the EventLog. 
Combat is handled by a separate library, but the calling of combat and combat results are handled through Scenes.

The RPG is about a player wandering through 3 rooms: a battle room, a shop, and a house. 
Within the battle room the player fights monsters, losing HP and gaining money. 
Within the shop, the player can buy items to make the combat easier by spending money.
In the house, the player can sleep to regain HP.


## Starting the Game
```
class CombatManager { 
	... 

	static CombatManager cm; 
} 

class Player { 
	int hp; 
	int money; 
	... 

    void dealDamage(...);
} 

class Monster { 
	... 
} 

```

The CombatManager singleton is created, dealing with the external game states and objects that Scenes doesn't control.

The class Player provides information on the game's player, while the class Monster provides information about monsters that the player will fight in the battle room.

```
void initializeVarFunc(...);

main() 
{ 
    Player player{...};
    Monster monster{...};
	... 
    EventMap emap = {
      {"initializeVar", Event([player, monster](string s) -> int {
            player.hp = 10;
            ...
            return 0; 
      }},
      {"dealDamage", Reader::makeEvent(player.dealDamage, ...)},
      {"dealDamage", Reader:makeEvent(monster.chooseMonster, ...)};
      ...
    };
	... 
} 
```

The object that reads through and manages Scenes scripts is called a `Reader`.
While Reader comes with a standard set of Events that it can always run, creating custom events requires the Reader to be given a custom Event Map, a map from string Event names to the events that they call. 
These Events can be initialized through an Event constructor or through the `Reader::makeEvent` function.

## Initializing Scenes
```
main() 
{ 
... 
	Reader<ExternalVars> reader = Reader<ExternalVars>( 
		"\PathtoBaseSceneFiles\", 
		"\PathtoSaveData\", 
		emap
	); 
... 
} 
```

A new Reader object, `reader`, is created and provided with required file data to construct properly, along with the `emap` map to access custom Events. 
`reader` instantiates the following variables:

```
sizet  linesRead = 0;
EventLog eventLog = EventLog(linesRead); 
Log sceneLog = Log(linesRead);
string sceneLoc = "\PathtoBaseSceneFiles\"; 
string saveLoc = "\PathtoSaveData\"; 
string nextScene = ""; 
```

When loading new Scenes, `reader` will attempt to find the corresponding Scene file from `saveLoc`, and failing that, attempt to load the file from `sceneLoc`. 
Failing to load from `sceneLoc` will cause a critical exception.


## Beginning Reading
```
main()
{ 
	... 
	reader.read(); 
	... 
} 
```

The reading is begun from the main function. To begin with,
`reader` will check for the existence of a `Scenes` directory in `saveLoc`.
If a Scenes directory doesn't exist, `reader` will create it and create and place `Globals.json` in it. 

```
SectionList curScene = findEntrance(); 
SceneLog pastScenes;
```

`reader` begins reading by deducing which Scene File to begin the script on. 
`reader` will attempt to load the saved active Scene File from `Globals.json`.
If `Globals.json` has no saved Scene, `reader` will default to initially loading from an `Entrance` Scene File in `sceneLoc`. Failing to load from `sceneLoc` will cause a critical exception.

`pastScenes` is a log with scene names as keys and a vector of the lines that they were called at as values.


## Running the Entrance
The Scene File contains a number of Sections.
Important Lines along with their events in each Section are highlighted.

```
Entrance.scene: 
	BeginningSection:
		Line ... - Event initializeVar 
		... 
		Line ... - Event findSecret
	SecretSection: Active only if findSecret returned 1 
		...
		Line ... - Event setNextScene SecretRoom, 
		...
	RegularSection: Active only if findSecret returned 0 
		...
		Line ... - Event setNextScene BattleRoom
		...
	...
```

The sections are loaded into `curScene` in the order that they are read, creating the queue:

```
BeginningSection,
SecretSection,
RegularSection
```

the Event `initializeVar` is run first. This Event calls a lambda function to initialize various class variables at the start of the game.
During the end of the `BeginningSection` the Event `findSecret` is run.
The results of this Event affect which sections are read during the rest of the Scene. 

After `BeginningSection` is read, it is popped from the queue. 
Before reading `SecretSection`, `reader` queries the Event log for `"findSecret, 1"`. If a non-empty vector is returned, then `SecretSection` is read, otherwise it is popped from the queue. The reading of `RegulerSection` is similar, with `reader` querying `"findSecret, 0"`.

The Event `setNextScene` loads the given Scene, and is required at some point in the Scene to prevent the reading from ending.


## Running the Battle Room
```
BattleRoom.scene:
	...
	TrapSection: Active only if triggeredTrap is 1 since start of scene
		Line "Trap!" - Event dealDamage
	CombatSection:
		...
		Line ... - Event chooseMonster
		Line ... - Event beginCombat
		...
	...
```

`TrapSection` is only read if the player has triggered a trap since entering the room. This is accomplished by `reader` initially querying `sceneLog` for `BattleRoom` to get the final value in the result vector, finding at what line the player initially entered the room. 
`reader` then queries `eventLog` for `triggeredTrap` to get the final value in the result vector. 
If the line that the player triggered the trap is greater than or equal to the line the player initially entered the room, the player is considered to have triggered a trap upon entering the room.

`dealDamage` and `chooseMonster` access the `player.dealRandomDamage()` method and the `monster.chooseMonster` method respectively through the event map `emap`.

```
BattleRoom.scene:
	...
	EndingSection:
		Line ... - Event chooseNextRoom
	NextBattleSection:
		...
		Line ... - Event setNextScene BattleRoom
		...
	HouseSection: Active only if chooseNextRoom returned 1
		...
		Line ... - Event setNextScene House
		...
	ShopSection: Active only if chooseNextRoom returned 2
		...
		Line ... - Event setNextScene Shop
		...
```

`NextBattleSection` is active by default, ensuring that a new scene is always selected. 
However, `HouseSection` and `ShopSection` can override this decision based on the results of `chooseNextRoom`.
