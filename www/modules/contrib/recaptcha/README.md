ReCAPTCHA
========

Uses the [reCAPTCHA](https://www.google.com/recaptcha) web service to improve the CAPTCHA system and protect email addresses.

reCAPTCHA helps power massive-scale online collaboration.

Versions
--------
This port is currently just of the 2.x version which implements the new NoCAPTCHA reCAPTCHA.

Requirements
------------
reCAPTCHA depends on the CAPTCHA module.

Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules

- Go to Configuration > People > CAPTCHA > reCAPTCHA
  (admin/config/people/captcha/recaptcha).
- Enter the site key and private key that you receive after signing up for the reCAPTCHA service through Google.
- Click "Save Configuration"
- Go to Configuration > People > CAPTCHA (admin/config/people/captcha) to configure on which forms the ReCAPTCHA will appear and for whom.

Known Issues
------------

- cURL requests fail because of outdated root certificate. The reCAPTCHA module
  may not able to connect to Google servers and fails to verify the answer.
  
  See https://www.drupal.org/node/2481341 for more information.

License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.

Current Maintainers
-------------------

This module is currently maintained for Backdrop by Herb v/d Dool.

Credits
-------

Ported to Backdrop by Herb v/d Dool (https://github.com/herbdool/)

This module was originally written for Drupal (https://drupal.org/project/recaptcha). Drupal maintainers are: [hass](https://www.drupal.org/u/hass), [diolan](https://www.drupal.org/u/diolan), [liam-morland](https://www.drupal.org/u/liam-morland), [id.medion](https://www.drupal.org/u/id.medion), [RobLoach](https://www.drupal.org/u/robloach).
