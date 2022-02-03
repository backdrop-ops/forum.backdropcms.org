<?php


namespace Backdrop\xautoload\Discovery;

class CachedClassMapGenerator implements ClassMapGeneratorInterface {

  /**
   * @var ClassMapGeneratorInterface
   */
  protected $decorated;

  /**
   * @var \Backdrop\xautoload\BackdropSystem\BackdropSystemInterface
   */
  protected $system;

  /**
   * @param ClassMapGeneratorInterface $decorated
   * @param \Backdrop\xautoload\BackdropSystem\BackdropSystemInterface $system
   */
  function __construct($decorated, $system) {
    $this->decorated = $decorated;
    $this->system = $system;
  }

  /**
   * @param string[] $paths
   *
   * @return string[]
   */
  function wildcardPathsToClassmap($paths) {
    // Attempt to load from cache.
    $cid = 'xautoload:wildcardPathsToClassmap:' . md5(serialize($paths));
    $cache = $this->system->cacheGet($cid);
    if ($cache && isset($cache->data)) {
      return $cache->data;
    }
    // Resolve cache miss and save.
    $map = $this->decorated->wildcardPathsToClassmap($paths);
    $this->system->cacheSet($cid, $map);

    return $map;
  }
}
