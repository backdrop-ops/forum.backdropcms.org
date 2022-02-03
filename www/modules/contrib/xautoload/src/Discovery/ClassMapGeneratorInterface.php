<?php


namespace Backdrop\xautoload\Discovery;

interface ClassMapGeneratorInterface {

  /**
   * @param string[] $paths
   *
   * @return string[]
   */
  function wildcardPathsToClassmap($paths);
} 