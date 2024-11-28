Feeds
=====

An import and aggregation framework for Backdrop. Import or aggregate data as
nodes, users, taxonomy terms or simple database records.

Features
--------

* Pluggable import configurations consisting of fetchers (get data) parsers
  (read and transform data) and processors (create content on Backdrop).
  * HTTP upload (with optional PubSubHubbub support).
  * File upload.
  * CSV, RSS, Atom parsing.
  * Creates nodes or terms.
  * Creates lightweight database records if [Data](http://drupal.org/project/data)
    module is installed (not ported yet).
  * Additional fetchers/parsers or processors can be added by an object-oriented
    plugin system.
  * Granular mapping of parsed data to content elements.
* Import configurations can be piggybacked on nodes (thus using nodes to track
  subscriptions to feeds) or they can be used on a standalone form.
* Unlimited number of import configurations.
* Export feeds importer configurations.
* Optional libraries module support.
* Previewing source content before importing.

Requirements
------------

* [Job Scheduler](https://github.com/backdrop-contrib/job_scheduler)
* PHP safe mode is not supported, depending on your Feeds Importer configuration
  safe mode may cause no problems though.

Installation
------------

* Install Feeds, Feeds Admin UI.
* To get started quick, install one or all of the following Feature modules:
  Feeds News, Feeds Import, Feeds Fast News (more info below).
* Make sure (cron)[https://backdropcms.org/user-guide/setting-cron] is correctly configured.
* Go to `import/` to import data.

SimplePie Installation
----------------------

To install the SimplePie parser plugin, complete the following steps:

  1. Download (SimplePie)[http://simplepie.org/downloads]. The recommended
     version is: 1.5.6.
  2. Decompress the downloaded zip file.
  3. Rename the uncompressed folder to `simplepie`. For example rename
     `simplepie-1.5` to `simplepie`.
  4. Move the folder to `/libraries`. If the folder does not exist, created it.
     The final directory structure should be `/libraries/simplepie`.
  5. In order for your site to recognize this library, you have two options.
     Either install the [Libraries](https://backdropcms.org/project/libraries)
     module, or,
     put this line in your `settings.php` file:
     `$config['feeds.settings']['feeds_library_dir'] = 'libraries';`
  6. Flush the Backdrop cache.
  7. The SimplePie parser should be available now in the list of parsers.

PubSubHubbub support
--------------------

Feeds supports the (PubSubHubbub publish/subscribe protocol)[http://code.google.com/p/pubsubhubbub/].
Follow these steps to set it up for your site.

* Go to admin/structure/feeds and edit (override) the importer configuration you
would like to use for PubSubHubbub.
* Choose the HTTP Fetcher if it is not already selected.
* On the HTTP Fetcher, click on 'settings' and check "Use PubSubHubbub".
* Optionally you can use a designated hub such as <http://superfeedr.com/> or
your own. If a designated hub is specified, every feed on this importer
configuration will be subscribed to this hub, no matter what the feed itself
specifies.

Libraries support
-----------------

If you are using the Libraries module, you can place external libraries in the
Libraries module's search path (for instance `/libraries`). The only external
library used at the moment is SimplePie.

Libraries found in the libraries search path are preferred over libraries in
`feeds/libraries/`.

API Overview
------------

See "The developer's guide to Feeds": <http://drupal.org/node/622700>

Testing
-------

See "The developer's guide to Feeds": <http://drupal.org/node/622700>

Debugging
---------

Set the Backdrop config variable `feeds_debug` to TRUE. This will create a file
`/tmp/feeds_[my_site_location].log`. Use `tail -f` on the command line to get a
live view of debug output. You can either set it in `feeds.settings.json` in
your active configuration folder or by putting
`$config['feeds.settings']['feeds_debug'] = TRUE;` into the `settings.php` file.

Note: at the moment, only PubSubHubbub related actions are logged.

Performance
-----------

See "The site builder's guide to Feeds": <http://drupal.org/node/622698>

Hidden settings
---------------

Hidden settings are variables that you can define by either updating them in
`feeds.settings.json` in your active configuration folder or overriding them in
`settings.php` file like this example: `$config['feeds.settings']['NAME'] = VALUE;`.

Name:        `feeds_debug`\
Default:     `FALSE`\
Description: Set to TRUE for enabling debug output to
`/BACKDROPTMPDIR/feeds_[sitename].log`

Name:        `feeds_importer_class`\
Default:     `'FeedsImporter'`\
Description: The class to use for importing feeds.

Name:        `feeds_source_class`\
Default:     `'FeedsSource'`\
Description: The class to use for handling feed sources.

Name:        `feeds_data_$importer_id`\
Default:     `feeds_data_$importer_id`\
Description: The table used by FeedsDataProcessor to store feed items. Usually a
FeedsDataProcessor builds a table name from a prefix (feeds_data_) and the
importer's id ($importer_id). This default table name can be overridden by
defining a variable with the same name.

Name:        `feeds_process_limit`\
Default:     `50`\
Description: The number of nodes feed node processor creates or deletes in one
page load.

Name:        `http_request_timeout`\
Default:     `15`\
Description: Timeout in seconds to wait for an HTTP get request to finish.
Note:        This setting could be overridden per importer in admin UI:
`admin/structure/feeds/<your_importer>/settings/<your_fetcher> page.`

Name:        `feeds_never_use_curl`\
Default:     `FALSE`\
Description: Flag to stop feeds from using its cURL for http requests. See
`http_request_use_curl()`.

Name:        `feeds_use_mbstring`\
Default:     `TRUE`\
Description: The extension mbstring is used to convert encodings during parsing.
The reason that this can be turned off is to be able to test Feeds behavior when
the extension is not available.

Import Preview
--------------

Ever get frustated by debugging your Feeds importers, because you could not get
a hang of what's actually in the source? Are you trying to get your config for
XPath Parser right? Or do you wonder if your configured Feeds Tampers leads to
the desired result? You can preview the results before importing. As a bonus, it
scans your source also for unmapped elements. This is especially handy in case
of importing CSV files, where you can quickly see which columns of your CSV file
are not mapped yet.

How the preview works
---------------------

The importer configuration page has a section called "Preview". In there,
there is a form that looks almost exactly like the standalone import form
provided by Feeds. Put in your source like you would normally do during a real
import.

Import Preview will then *fetch* and *parse* your source and then show you
the parsed result in a series of tables for the first 50 records. Note that the
*process* step is completely skipped during the preview.

Finally, when you're good with the preview, you can continue to the import form
and re-input your source to trigger the real import.

Note:

* You will only get a preview of the parsed result, not the end result. The
  content to import may still be modified during processing.
* Sources provided at the preview form will not overwrite the sources provided
  at the import form, and vice versa. This means that if you want to import a
  source that you just previewed, you need to put it in again at the import
  form.
* Previewing a source will not trigger an import.
* Only the first 50 records of the source will be shown. This limit may be
  configurable in the future. Right now, it depends on the `feeds_process_limit`
  setting.
* You can navigate through the results using the left and right arrow keys on
  your keyboard (given that your browser supports this).

Glossary
--------

See "Feeds glossary": <http://drupal.org/node/622710>

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.

Maintainers
-----------

* [Andy Shillingford](https://github.com/docwilmot/)
* [Herb v/d Dool](https://github.com/herbdool)

Credits
-------

Drupal version maintained by:

* [MegaChriz](https://www.drupal.org/u/MegaChriz)
* [twistor](https://www.drupal.org/u/twistor)
* [tristanoneil](https://www.drupal.org/u/tristanoneil)
* [franz](https://www.drupal.org/u/franz)
* [febbraro](https://www.drupal.org/u/febbraro)
