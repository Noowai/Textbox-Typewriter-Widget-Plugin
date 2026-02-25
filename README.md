# Textbox-Typewriter-Widget-Plugin
Unreal Engine Plugin for Textbox widget with typewriter mechanic.

This is a sourceversin of the plugin tested on UE 4.27 and UE 5.7.

**Installation**

To add plugin to your project:
1. Download plugin .zip
2. Unpack to the following folder ..\YourProject\Plugins\
3. If you don't have 'Plugins' folder in your project folder, create it.
4. Start the project normally. If Unreal asks you to rebuild modules, agree to it.

**Using the widget**

To add typewriter widget to your menu layout. Find 'Typewriter Textbox' in Palet tab, where all other widget elements are available, add it to your laout.
When selected, in details panel you'll find 'Typewriter' section with basic setup.

Here you can specify:
- Speed - how many letters should appear per second
- Speed Up Coeff - How much to speed up the typing in case you need, for instance when you press a button to skip
- Delay At Start - How long to wait for typing after widget appeared on the screen
- Type Sound - Sound to play for each appearing letter
- Should Pause - Typing can be optionnaly paused on various symbols, for instance each whitespace etc.
- Pause Time - delay when pausing on a symbol
- Pause on Characters - list of symbols to pause on. Just type every symbol you want to pause. All symbols must be typed in a row, order doesn't matter.
- Autostart (in Advanced Section) - this check can be ticked off in case you need to control when typing starts.

**Additionnal Capabilities**

Typewriter Widget has several specific functions to use:
- StartTypewriting - this function is for begin typewriting effect mannually when Autostart is off
- SkipTypewriting - will instanlty output the whole text
- SwitchSpeedUp - rurns the Speed Up Coeff on and off during typewriting.
