Cloudflare Turnstile for Backdrop
=================================

The Cloudflare Turnstile module uses the Turnstile web service as an alternative
to CAPTCHA to protect forms. It can be embedded into any website without sending
traffic through Cloudflare and works without showing visitors a CAPTCHA.

Turnstile widget types include:

* A non-interactive challenge.
* A non-intrusive interactive challenge (such as clicking a button), if
  the visitor is a suspected bot.
* An invisible challenge to the browser.

For more information on what Turnstile is, please visit:
    <https://developers.cloudflare.com/turnstile>

Requirements
------------

* Turnstile module depends on the CAPTCHA module.
  <https://backdropcms.org/project/captcha>

Installation
------------

* Enable Cloudflare Turnstile and CAPTCHA modules using the official
  Backdrop CMS instructions at <https://backdropcms.org/guide/modules>.

Configuration
-------------

1. You'll now find a Turnstile tab on the CAPTCHA
   administration page available at:
       `admin/config/people/captcha/turnstile`
2. Register at:
       <https://cloudflare.com/>
3. Copy the site and secret keys from Cloudflare to the Turnstile
   settings.
4. Visit the CAPTCHA administration page and set where you
   want the Turnstile form to be presented:
       `admin/config/people/captcha`

Current Maintainers
-------------------

* [Herb v/d Dool](https://github.com/herbdool)
* This module is currently seeking co-maintainers.

Credit
------

Ported to Backdrop by [herbdool](https://github.com/herbdool). Created for
Drupal 9+ and maintained by [greatmatter](https://www.drupal.org/u/greatmatter).
Backported to Drupal 7 by Jibus. Thanks to the Cloudflare team for their amazing
CAPTCHA replacement.

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.
