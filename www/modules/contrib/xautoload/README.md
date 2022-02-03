X Autoload
==========

xautoload is a comprehensive and yet highly efficient PHP class loading suite.

Most importantly, it provides Drupal-8-style PSR-4 (and the old D8 PSR-0-style in "lib") autoloading for Backdrop. But it is also a great tool for 3rd party autoloading.

Allows to use the xautoload cache options for traditional core and contrib classes.


Class loading for Backdrop modules
----------------------------------

- D8-style PSR-4 module namespaces (under "$module_dir/src/..").
- Old D8-style PSR-0 module namespaces (under "$module_dir/lib/..").
- PHP 5.2 compatibility pattern ("PEAR-flat") with prefix/underscore class names instead of namespaces.


Class loading for 3rd party libraries
-------------------------------------
- hook_xautoload() to register additional namespaces with PSR-4, PSR-0 or whatever you like.
- Libraries API: Namespace registration for libraries directly from hook_libraries_info().
- Can scan downloaded libraries to build a classmap. (which will be cached)
- Can read and understand composer.json files from downloaded libraries.
- Can process composer-generated autoload files in a vendor/composer directory, to register all this stuff to the xautoload class loader.
(all of these can be registered from hook_xautoload() or hook_libraries_info())


How to use
----------
- Install this module using the official Backdrop CMS instructions at
https://backdropcms.org/guide/modules
- Optionally, enable APC and other cache options on Administration > Configuration > Development > Performance. The more checkboxes, the better!
- Add as a dependency in your modules that need it: `dependencies[] = xautoload (>= 1.x-5.0)`
- Now create your PSR-4 class files, e.g.
File: `"$module_dir/src/Foo/Bar.php"`
```php
namespace Backdrop\modulename\Foo;
class Bar {..}
```
- More details: https://www.drupal.org/node/1976196


Issues
------

Bugs and Feature requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/xautoload/issues


Current Maintainers
-------------------

- Attila Vasas (https://github.com/vasasa).
- Seeking additional maintainers.


Credits
-------

- Ported to Backdrop CMS by Attila Vasas (https://github.com/vasasa).
- Originally written for Drupal by Andreas Hennings (https://www.drupal.org/u/donquixote).


License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.
