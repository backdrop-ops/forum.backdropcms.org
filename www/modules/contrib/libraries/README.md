Libraries API (Deprecated)
==========================

**NOTE:** The Libraries API contrib module has been deprecated as equivalent
functionality has been added to Backdrop core. This module will not receive any
further updates.

If a module you're trying to use depends on Libraries API, please suggest to the
module maintainer that they update their module to use core's library functions
instead.

If you're a module maintainer and currently depend on Libraries API, please
replace Libraries API-specific functions with their Backdrop core equivalents:

`hook_libraries_info()` -> [`hook_library_info()`](https://api.backdropcms.org/api/backdrop/core%21modules%21system%21system.api.php/function/hook_library_info/1)  
`hook_libraries_info_alter()` -> [`hook_library_info_alter()`](https://api.backdropcms.org/api/backdrop/core%21modules%21system%21system.api.php/function/hook_library_info_alter/1)  
`libraries_load()` -> [`backdrop_add_library()`](https://api.backdropcms.org/api/backdrop/core%21includes%21common.inc/function/backdrop_add_library/1)  
`libraries_info()` -> [`backdrop_get_library()`](https://api.backdropcms.org/api/backdrop/core%21includes%21common.inc/function/backdrop_get_library/1)  
Note that this isn't simply a matter of renaming the functions. You will need to
rewrite some of your code. Check the API documentation for more information.

---

This module is a Backdrop port of Drupal's contributed module
'[Libraries](https://www.drupal.org/project/libraries)'. Libraries API provides
external library handling for other Backdrop modules.

Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.

Current Maintainers
-------------------

- Peter Anderson (https://github.com/BWPanda).

Credits
-------

- Ported to Backdrop CMS by Graham Oliver (https://github.com/Graham-72).
- Originally written for Drupal by Daniel Kudwien
  (https://www.drupal.org/u/sun).

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

