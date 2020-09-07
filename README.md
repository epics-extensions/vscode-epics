# [vscode-epics](https://marketplace.visualstudio.com/items?itemName=nsd.vscode-epics), EYE (Enlight Your Epics)

This VS Code extension provides syntax highlighting for EPICS database, template, substitutions and startup files, and streamdevice prototype files.

## Features

- [x] Syntax highlighting for:
  - [x] database and template files (.db, .dbd and .template)
  - [x] substitutions files (.substitutions, .sub, and .subs)
  - [x] startup files (.cmd and .iocsh)
  - [x] streamdevice prototype files (.proto)
  - [x] SNL files (.st extensions are considered as .c extension and some keywords are highlighted)

![Syntax highlighting](https://raw.githubusercontent.com/NSenaud/vscode-epics/master/images/vscode_epics.gif)

- [ ] Snippets for:
  - [x] database and template files (.db and .template)
  - [x] substitutions files (.substitutions, .sub, and .subs)
  - [x] startup files (.cmd and .iocsh)
  - [x] C source code
  - [ ] streamdevice prototype files (.proto)
  - [x] SNL files (over charge of your .c extension)
  - [x] we-test (scenario and suite)

| Tips: to see all the available snippets per file type, write "epics." and a list will show up (file has to have a known extension). |
| --- |

- [ ] Compilation facilities

- [ ] Linter for:
  - [ ] database and template files
  - [ ] substitutions files
  - [ ] startup files
  - [ ] streamdevice prototype files
  - [ ] SNL files

## Requirements

VS Code 1.5 or newer version.

## Extension Settings

None yet!

## Known Issues

Every field types are not recognized yet, only the more commons.

## Credits

Snippets are mainly coming from [language-epics](https://github.com/mmllski/language-epics) Atom plugin, [agaget](https://github.com/agaget) and [vnadot](https://github.com/vnadot).

## to do
- [issues](https://github.com/NSenaud/vscode-epics/issues)
- weTest:
  - new scheme
  - add field value to snippet test
- mbbi: add alarm?
- add CP or PP by default ?
- field to add: OROC, OOPT ...?
- dbLoadTemplate(char ⋆subfile, char ⋆substitutions): dbLoadTemplate("../../db/iocTest.substitutions", "ASYN_RFFI_PORT=${ASYN_RFFI_PORT}")
- dbLoadRecords() ...
- dtyp: asy32..
- ao: egul eguf prec drvl hopr...
- ai ao: add alarms
- mbbi alarms
- raw soft channel vs soft channel
