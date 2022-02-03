<?php


namespace Backdrop\xautoload\Tests\VirtualBackdrop;


class SystemListReset {

  /**
   * @var BackdropStatic
   */
  private $backdropStatic;

  /**
   * @var Cache
   */
  private $cache;

  /**
   * @param Cache $cache
   * @param BackdropStatic $backdropStatic
   */
  function __construct(Cache $cache, BackdropStatic $backdropStatic) {
    $this->cache = $cache;
    $this->backdropStatic = $backdropStatic;
  }

  /**
   * @see system_list_reset()
   */
  function systemListReset() {
    $this->backdropStatic->resetKey('system_list');
    $this->backdropStatic->resetKey('system_rebuild_module_data');
    $this->backdropStatic->resetKey('list_themes');
    $this->cache->cacheClearAll('bootstrap_modules', 'cache_bootstrap');
    $this->cache->cacheClearAll('system_list', 'cache_bootstrap');

  }
} 
