<?php

namespace Backdrop\xautoload\CacheManager;

use Backdrop\xautoload\BackdropSystem\BackdropSystemInterface;
use Backdrop\xautoload\Util;

class CacheManager {

  /**
   * @var string
   */
  protected $prefix;

  /**
   * @var \Backdrop\xautoload\BackdropSystem\BackdropSystemInterface
   */
  protected $system;

  /**
   * @var CacheManagerObserverInterface[]
   */
  protected $observers = array();

  /**
   * @param string $prefix
   * @param \Backdrop\xautoload\BackdropSystem\BackdropSystemInterface $system
   */
  protected function __construct($prefix, BackdropSystemInterface $system) {
    $this->prefix = $prefix;
    $this->system = $system;
  }

  /**
   * This method has side effects, so it is not the constructor.
   *
   * @param \Backdrop\xautoload\BackdropSystem\BackdropSystemInterface $system
   *
   * @return CacheManager
   */
  static function create(BackdropSystemInterface $system) {
    $prefix = $system->variableGet(XAUTOLOAD_VARNAME_CACHE_PREFIX, NULL);
    $manager = new self($prefix, $system);
    if (empty($prefix)) {
      $manager->renewCachePrefix();
    }
    return $manager;
  }

  /**
   * @param CacheManagerObserverInterface $observer
   */
  function observeCachePrefix($observer) {
    $observer->setCachePrefix($this->prefix);
    $this->observers[] = $observer;
  }

  /**
   * Renew the cache prefix, save it, and notify all observers.
   */
  function renewCachePrefix() {
    $this->prefix = Util::randomString();
    $this->system->variableSet(XAUTOLOAD_VARNAME_CACHE_PREFIX, $this->prefix);
    foreach ($this->observers as $observer) {
      $observer->setCachePrefix($this->prefix);
    }
  }
}
