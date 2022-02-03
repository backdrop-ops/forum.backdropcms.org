<?php


namespace Backdrop\xautoload\Tests\VirtualBackdrop;


class BackdropLoad {

  /**
   * @var array
   */
  private $files = array();

  /**
   * @var BackdropGetFilename
   */
  private $backdropGetFilename;

  /**
   * @param BackdropGetFilename $backdropGetFilename
   */
  function __construct(BackdropGetFilename $backdropGetFilename) {
    $this->backdropGetFilename = $backdropGetFilename;
  }

  /**
   * @see backdrop_load()
   */
  function backdropLoad($type, $name) {

    if (isset($this->files[$type][$name])) {
      return TRUE;
    }

    $filename = $this->backdropGetFilename->backdropGetFilename($type, $name);

    if ($filename) {
      include_once $filename;
      $this->files[$type][$name] = TRUE;

      return TRUE;
    }

    return FALSE;
  }
} 
