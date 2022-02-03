Unique Field
============

The Unique Field module provides a way to require that specified fields
or characteristics of a node are unique. This includes the node's title,
author, language, taxonomy terms, and other fields.

Without this module, Drupal and CCK do not prevent multiple nodes from
having the same title or the same value in a given field.

For example, if you have a content type with a Date field and there
should only be one node per date, you could use this module to prevent a
node from being saved with a date already used in another node.


Installation
------------

- Install this module using the official Backdrop CMS instructions at 
  https://backdropcms.org/guide/modules
- Use release 1.x-1.0.0 of Unique Field for Backdrop 1.14.4 and earlier versions: 
  https://github.com/backdrop-contrib/unique_field/releases/tag/1.x-1.0.0


Usage
-----
This module adds additional options to the administration page for each
content type (i.e. admin/structure/types/manage/<content type>) for
specifying which fields must be unique. The administrator may specify
whether each field must be unique or whether the fields in combination must
be unique. Also, the administrator can choose whether the fields must be
unique among all other nodes or only among nodes from the given node's
content type.

Alternatively, you can select the 'single node' scope, which allows you
to require that the specified fields are each unique on that node. For
example, if a node has multiple, separate user reference fields, this
setting will require that no user is selected more than once on one node.


Self tests
----------
The module contains three self tests:
- core tests
- Date module tests
- References module tests

Date module is a core module, but if you want to test the References module,
then you need to download it and to place into `BACKDROP_ROOT/modules`. 
Download: https://backdropcms.org/project/references

Try this:
Administration > Configuration > Development > Testing > Unique Field


Issues
------

Bugs and Feature requests should be reported in the Issue Queue: 
https://github.com/backdrop-contrib/unique_field/issues


Current Maintainers
-------------------

- Attila Vasas (https://github.com/vasasa).


Credits
-------

- Ported to Backdrop CMS by Attila Vasas (https://github.com/vasasa).
- Originally written for Drupal by Joe Turgeon (https://www.drupal.org/u/arithmetric).


License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.


Screenshot
----------
Additional options to the page for each content type:
![Vertical](https://github.com/backdrop-contrib/unique_field/blob/1.x-1.x/images/screenshot.png)
