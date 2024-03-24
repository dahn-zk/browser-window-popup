# browser window popup

a basic unpublished example of a Windows native Firefox extension to quickly open Google Translate with a selected text in a popup window from anywhere on the system . you can select any text and press `ctlr`+`c`+`c` , on which a browser popup window will open with the translation of the selected text .

made just for fun and learning . i low key wanted to have something like this polished and published , but i did not have resources to make it serious project out of it . so only showing how it could be aproached for anyone interested . 

if you find this useful , leave a star .


## why

the problem is that any other free and/or open-source solutions for quick translator/dictionary/wikis/search/etc popup-ing could only do the following :

- open a popup with custom GUI for existing online or offline services , e.g. GoldenDict and alternatives .
- open a full tab in your browser , e.g. SnipDo and alternatives .
- work only inside of a browser .


## how

since a browser extension alone has no access to what a user selects on other apps , i use [native messaging][msg] stack .
the extension spawns the application that runs in the background . the app waits for the key sequence and sends a message with a text and cursor position data to the extension .


### limitations

- the application uses Windows clipboard to copy a selected text with the first `c` while `ctrl` is held . so , not to overcomplicate , `ctrl`+`c`+`c` is hardcoded and not configurable yet . the clipboard method is the most reliable way to actually get the selected text from other applications as many do not expose to the OS what you select , e.g. browsers . as a bonus , this solution works nice with OCR tools like NormCap which recognise text from an area-screenshot and copy it to the clipboard anyway .
- the browser must already be open . i don't like this , but it's the nature of the extensions .


## setup and usage

1. build a C++ [application][app] with CMake . i used CLion and MinQW toolchain . no libs required except from Windows API which should be installed by default .
2. run a [registry update script][reg] with PowerShell . this is needed for Firefox to find the application .
3. add an [extension][ext] to your Firefox as a temporary addon . [how to : Mozilla docs][mdn] .


[app]: application/
[ext]: extension/
[reg]: application/registry_update.ps1
[mdn]: https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/Your_first_WebExtension#trying_it_out
[msg]: https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/Native_messaging