# SHotKey
A Suckless Hot Key daemon for X inspired by the suckless philosophy with configurable custom modes.

~200 LOC. ~20KB binary.


<a href="https://liberapay.com/phenax/donate"><img alt="Donate using Liberapay" src="https://liberapay.com/assets/widgets/donate.svg"></a>


## Installation
Clone the repo and run
```bash
sudo make install
```


## Configuring
Fork this repo and configure it to your likings.
You can edit `config.h` to configure key bindings.

* `shell` - Configure the shell used for executing the commands
* `modes` - A mapping defining all mode keys bindings
* `keys` - All normal mode key bindings
* `on_mode_change` - Hook script to listen to mode changes. Useful for status bar integration
* `mode_properties` - Some meta information about mode. (Label is useful for `on_mode_change`)

You can checkout the [personal branch](https://github.com/phenax/shotkey/tree/personal) for my personal config.

Read the man page for more details.
