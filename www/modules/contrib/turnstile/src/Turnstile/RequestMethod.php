<?php

namespace Turnstile;

interface RequestMethod
{
  /**
   * Submit the request with the specified parameters.
   *
   * @param string $url
   * @param array $params Request parameters
   *
   * @return \stdClass Body of the Turnstile response
   */
    public function submit($url, array $params);
}
