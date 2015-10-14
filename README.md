# wandboard_led_display
This simple application is designated for interfacing with HD44789 led display connected to Wandboard computer.

Before running this app ypu should make sure, that GPIO`s are exported to user space and you have permission to write to them. You should also have some knowledge about electronics and display, as I do not provide any schematic for conecting. At least not yet.
Application use 6 GPIO`s, so this means that it works in 4bit mode and write only.