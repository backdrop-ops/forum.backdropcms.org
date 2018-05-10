Similar entries
===============

Provides a default View with block display for listing published content that
is similar to the content being viewed.

This module uses MySQL's FULLTEXT indexing for MyISAM tables, and should
automatically add the index when you activate the module for the first time.


Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules

- Place the Similar Entries block into any layout at Administration >
  Structure > Laouts (admin/structure/layouts).

- To customize the Similar entries block, navigate to Administration >
  Structure > Views (admin/structure/views) and edit the view named Similar
  Entries. The Similar Entries view uses a special contextual filter. This
  contextual filter must be present for the view to to determine similarity.

- Similar entries indexes field tables when cron runs. If you add a new field
  you can force Similar Entries to index it immediately by navigating to
  Administer > Reports > Status report (admin/reports/status) and clicking
  'Run cron manually'.


Documentation
-------------

Additional documentation is located in the Wiki:
https://github.com/backdrop-contrib/similar/wiki/Documentation

Issues
------

Bugs and Feature requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/similar/issues

Current Maintainers
-------------------

- Jen Lampton (https://github.com/username/jenlampton)
- Seeking additional maintainers

Credits
-------

- Ported to Backdrop CMS by [Jen Lampton](https://github.com/username/jenlampton).
- Maintained for Drupal by [David Kent Norman](http://deekayen.net).
- Maintained for Drupal by [Jordan Halterman](https://www.drupal.org/u/jordojuice).
- Maintained for Drupal by [Arnab Nandi](http://arnab.org).
- Originally written for Drupal by [arnabdotorg](https://www.drupal.org/user/8611).

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.



