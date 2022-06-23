# Scenes
<!--
## Purpose
The purpose of a scene is to have a constant background that can be 
used for a variable set of lines and characters, and can dynamically
change depending on what happens in the story.
-->

A `Scene` object is text representation of the current game area that
the player is in. Each `Scene` is composed of multiple `Section` 
objects, which may be active or inactive depending on the current game state.
Each `Section` is composed of multiple `Line` objects.

During reading of `Lines`, the current game state can change 
in the form of activating/deactivating `Sections`; 
changing the current `Scene`; or logging an `Event`, 
a flag that can affect the properties of `Scenes` or `Sections` or
run a function call to a different library.

## Using Scenes
Scenes is run through a `ScriptReader` object. The calling program
instantiates the `ScriptReader`, providing it with the location
of the directory containing the required scripts and 
the directory to store modified scripts and other Scene information,
along with the name of the script to load in initially.

A reading is begun by calling `.read()`. Script reading is paused
when coming across `Events` with a `pause` flag. Script reading 
is completed when the reader is unable to load the next 
scene or when `.stop()` is called.

## Script
### JSON Storage
<!-- Description when you know what you want a scene to 
be stored as.

Each scene is stored in a separate file and loaded individually.
Each scene file is stored as an array of script sections dictionaries, 
mapping each section name as a key to an array of lines.
-->

<!-- Example json script when you've known what 
you want your scene object to look like.
```

```
-->
### Event
An `Event` is an object that acts as a wrapper for a functor or as a message that is logged by the `ScriptReader`. 
An `Event` can correspond to a `Line` object. When an `Event` is called,
the time (represented by the number of `Lines` that have been read) 
of the `Event`, along with the name of the `Event` object 
and a unique hash to allow for querying.

> The hash is created using the Event's name and its functor.

### Script Reading
Scripts are managed through a `ScriptReader` object.
A `ScriptReader` logs how many lines have been read during the
playing of the game, all `Events` that are 
called by a line object when it is read, and 
handles querying events.

A `ScriptReader` also stores a log of what `Scenes` have 
been displayed, along with what `Scene` is slotted to be 
displayed next. A log of an `Event` includes its name and how many lines 
into the game it was called.

Scenes are loaded from a file into a `Scene` object to be read. By default,
every `Section` in a `Scene` is queued in the order that it
is written in the script, but individual `Lines` can modify
that queue by inserting or removing sections, or moving to another 
`Scene`. Lines in a `Section` are read in the same order that
they are written to a script. During the reading of a `Section`, it can query the `ScriptReader`
to determine how it should act based on the current logged `Events`. 

## Lines
`Line` objects are text strings that are displayed to the player.
Each `Line` contains any number of `Events` that are run when a line is read.

<!--along with how the
game state will be affected by the result of those `Events`.
-->
## Sections
`Section` objects are containers for `Line` objects and contain
whether the section is currently active/inactive, along with
information on the conditions required to activate/deactivate it.

## Scenes
`Scene` objects are containers for `Section` objects and contain
methods required to modify each. 
