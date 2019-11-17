Mentions
========

The Mentions module offers Twitter like functionality, recording all references
to a user's username - using the [@username] or [@#uid] filter format - from
various locations, providing a centralized page to track all mentions.



Features
--------------------------------------------------------------------------------

* Tracks Mentions on any Entity.
* An Input filter to convert [@username] or [@#uid] to the user's profile.
* Customizable input ([@username], [@#uid]) and output (@username) patterns,
  including support for the Token module.
* Optional: autocompletion of mentions using the jQuery textcomplete library.
  This currently does not work with CKEditor   and consequently has not been 
  tested. A configuration option is available to prevent loading the 
  textcomplete library.
* Integrations with:
  * Machine name - Use a Machine name field as the mention source.
  * Rules - React to created, updated and deleted mentions.
  * Views - Display a list of all mentions, mentions by user, and more.



jQuery textcomplete - Autocompletion of mentions (not working with CKEditor)
--------------------------------------------------------------------------------

The jQuery textcomplete library adds autocompletion of mentions, to install it
follow these steps:

1. (optional) Download and install the Libraries API module.
    http://backdrop.org/project/libraries

2. Download the jQuery textcomplete library and extract and move it into your
   libraries folder as 'textcomplete' (eg. sites/all/libraries/textcomplete).
    https://github.com/Decipher/jquery-textcomplete/archive/master.zip



Alternatively, you can use the Drush command 'mentions-library' or the Drush
make entry provided below:

  libraries[textcomplete][download][type] = get
  libraries[textcomplete][download][url] = https://github.com/Decipher/jquery-textcomplete/archive/master.zip



Usage / Configuration
--------------------------------------------------------------------------------

Once installed, the Mentions filter needs to be enabled on your desired Text
formats, this can be done via the Text formats page.
* http://[www.yoursite.com/path/to/backdrop]/admin/config/content/formats

Customisation settings for input and output patterns are available via the
Mentions configuration form:
* http://[www.yoursite.com/path/to/backdrop]/admin/config/content/mentions


License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

Current Maintainers
-------------------

- docwilmot (https://github.com/docwilmot)

Credit
-------

Mentions was written and is maintained for Drupal by Stuart Clark (deciphered).
- http://stuar.tc/lark

