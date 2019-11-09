Forum: forum.backdropcms.org
============================

This is the repo for the Forum.BackdropCMS.org Forum.

Development
-----------

The forum site uses [Lando](https://docs.lando.dev/) for local development. Here is how to get started:

Get Lando
---------

* If you don't have Lando download it: [Get Lando](https://lando.dev/download/)

Get the Code
------------

* Download with git:
  * `git clone git@github.com:backdrop-ops/forum.backdropcms.org.git`
* Move into the project root:
  * `cd forum.backdropcms.org`

Get a Database
--------------

In order to any development on a Backdrop site you'll need the database. For the forum you can get the database from https://sanitize.backdropcms.org. This site is protected by a username and password. You can request the credentials from @jenlampton.

If you are just getting started with forum and Backdrop community some you can request a copy of a recent database from @docwilmot, @jenlampton, @serundeputy, @quicksketch.

* Visit https://sanitize.backdropcms.org
* Download the latest sanitized backup of forum database
  * This will be a link of the form: forum-November-9-2019-100706-sanatized.sql.gz
    * Choose the most recent date.

Importing the Database
-----------------------

Once you've downloaded the code and database move the database file the project root that contains the forum code. Assuming that you are already in the forum's project root and the DB was downloaded to your `~/Downloads` directory:

* `cp ~/Downloads/forum-November-9-2019-100706-sanatized.sql.gz .`
  * Change the gz filename to reflect the date of your backup.

* Start Lando
  * `lando start`
* Import the DB
  * `lando.dev db-import forum-November-9-2019-100706-sanatized.sql.gz`
    * Change the date to reflect your file

Working on an Issue
-------------------

* Pick an issue from: https://github.com/backdrop-ops/forum.backdropcms.org/issues
* Create a branch to work on
  * `git checkout -b ISSUENUMBER/briefDescriptor`
    * Replace ISSUENUMBER with your issue number
    * Replace briefDescriptor w/ a description of your issue (no spaces)
  * Do your work on your branch
  * Add and commit your files
  * PUsh up yor branch
    * `git push origin ISSUENUMBER/briefDescription`
  * Submit a PR via the GitHub web UI
  * Link your PR in the original issue
    * Label the issue as `has PR`
    * Request review by commenting on the issue and at tagging the person(s) that started the issue
