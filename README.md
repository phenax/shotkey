# HotKeyThingy
A suckless hot key daemon for X inspired by the suckless philosophy with configurable custom modes.

~200 LOC. ~20KB binary.


## Installation
Clone the repo and run
```bash
sudo make install
```


## Configuring
You can edit `config.h` to configure key bindings.

* `shell` - Configure the shell
* `modes` - A mapping defining all mode keys bindings
* `keys` - All normal mode key bindings
* `on_mode_change` - Hook script to listen to mode changes. Useful for status bar integration
* `mode_properties` - Some meta information about mode. (Label is useful for `on_mode_change`)

You can checkout the [personal branch](https://github.com/phenax/hotkeythingy/tree/personal) for my personal config.

Read the man page for more details.
