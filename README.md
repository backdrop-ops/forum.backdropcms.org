forum.backdropcms.org3
=====================

This is the repo for the Forum.BackdropCMS.org Forum.

Development
-----------

The forum site uses [Lando](https://docs.lando.dev/) for local development. Here
is how to get started:

* If you don't have Lando, download it: [Get Lando](https://lando.dev/download/)
* Fork this repository into your own GitHub account
* Download with git:
  * `git clone git@github.com:[YOUR_USERNAME]/forum.backdropcms.org.git`
* Move into the project root:
  * `cd forum.backdropcms.org`
* Start Lando
  * `lando start`
* Download and import the DB
  * `lando pull-db`
* Download and import the files
  * `lando pull-files`

The database and files come from https://sanitize.backdropcms.org. This site is
protected by a username and password. You can request the credentials via
[Zulip](https://backdrop.zulipchat.com/login/).

Working on an Issue
-------------------

* Pick an issue from:
  https://github.com/backdrop-ops/forum.backdropcms.org/issues
* Create a branch to work on
  * `git checkout -b ISSUENUMBER/briefDescriptor`
    * Replace ISSUENUMBER with your issue number
    * Replace briefDescriptor w/ a description of your issue (no spaces)
  * Do your work on your branch
  * Add and commit your files
  * Push up your branch
    * `git push origin ISSUENUMBER/briefDescription`
  * Submit a PR via the GitHub web UI
  * Link your PR in the original issue
    * Label the issue as `has PR`
    * Request review by commenting on the issue and 'at' tagging the person(s)
      that started the issue
