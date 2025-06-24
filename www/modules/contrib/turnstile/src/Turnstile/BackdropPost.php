<?php

/**
 * @file
 * Custom Backdrop request method class for Turnstile.
 */

namespace Turnstile;

/**
 * Sends POST requests to the Turnstile service.
 */
class BackdropPost implements RequestMethod {

  /**
   * Submit the POST request with the specified parameters.
   *
   * @param array $params
   *   Request parameters.
   *
   * @return \stdClass
   *   Body of the Turnstile response.
   */
  public function submit($url, array $params) {
    $options = array(
      'headers' => array(
        'Content-type' => 'application/x-www-form-urlencoded',
      ),
      'method' => 'POST',
      'data' => http_build_query($params, '', '&'),
    );
    $response = backdrop_http_request($url, $options);

    if ($response->code == 200 && isset($response->data)) {
      // The service request was successful.
      $result = $response->data;
    } elseif ($response->code < 0) {
      // Negative status codes typically point to network or socket issues.
      $result = '{"success": false, "error-codes": ["connection-failed"]}';
    } else {
      // Positive none 200 status code typically means the request has failed.
      $result = '{"success": false, "error-codes": ["bad-response"]}';
    }

    return json_decode($result);
  }

}
