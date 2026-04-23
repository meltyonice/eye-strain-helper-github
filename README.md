# Eye Strain Helper
A Geometry Dash mod that helps prevent eye strain without interrupting your attempts.

## Features
- [x] Optional ability to skip breaks
- [x] Configurable time between breaks
- [x] Configurable duration of breaks
- [x] Option to start breaks when hitting a checkpoint in platformer mode
- [x] Option to automatically pause after breaks
- [x] Safe Eyes integration w/a custom plugin
    - [x] Option to block Safe Eyes breaks while playing levels
    - [x] Option to prefer Safe Eyes breaks while in the editor
- More planned!


## Build instructions
Note: If you want to build the Safe Eyes plugin, you'll need to have cargo installed.
```sh
# to build just the gd mod:
make mod
# to build and install the safe eyes plugin along with the listen server:
make safeeyes
# to build everything:
make all
```

## Attribution
`on_stop_break.ogg` sourced from [Safe Eyes](https://github.com/slgobinath/safeeyes/tree/master)