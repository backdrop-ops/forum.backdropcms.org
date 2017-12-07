<?php

/**
 * @file
 * Custom Backdrop RequestMehod class for Google reCAPTCHA library.
 */

namespace ReCaptcha\RequestMethod;

use ReCaptcha\RequestMethod;
use ReCaptcha\RequestParameters;

/**
 * Sends POST requests to the reCAPTCHA service.
 */
class BackdropPost implements RequestMethod {

  /**
   * URL to which requests are POSTed.
   * @const string
   */
  const SITE_VERIFY_URL = 'https://www.google.com/recaptcha/api/siteverify';

  /**
   * Submit the POST request with the specified parameters.
   *
   * @param RequestParameters $params Request parameters
   * @return string Body of the reCAPTCHA response
   */
  public function submit(RequestParameters $params) {

    $options = array(
      'headers' => array(
        'Content-type' => 'application/x-www-form-urlencoded',
      ),
      'method' => 'POST',
      'data' => $params->toQueryString(),
    );
    $response = backdrop_http_request(self::SITE_VERIFY_URL, $options);

    return isset($response->data) ? $response->data : '';
  }
}
