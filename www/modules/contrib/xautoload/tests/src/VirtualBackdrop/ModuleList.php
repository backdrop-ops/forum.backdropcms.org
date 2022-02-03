<?php


namespace Backdrop\xautoload\Tests\VirtualBackdrop;


/**
 * Replicates module_list() with its static cache variables.
 */
class ModuleList {

  /**
   * @var BackdropGetFilename
   */
  private $backdropGetFilename;

  /**
   * @var SystemList
   */
  private $systemList;

  /**
   * @var array
   */
  private $list = array();

  /**
   * @var string[]
   */
  private $moduleListSorted;

  /**
   * @var BackdropStatic
   */
  private $backdropStatic;

  /**
   * @param BackdropGetFilename $backdropGetFilename
   * @param SystemList $systemList
   * @param BackdropStatic $backdropStatic
   */
  function __construct(BackdropGetFilename $backdropGetFilename, SystemList $systemList, BackdropStatic $backdropStatic) {
    $this->backdropGetFilename = $backdropGetFilename;
    $this->systemList = $systemList;
    $this->backdropStatic = $backdropStatic;
  }

  /**
   * Replicates module_list(FALSE, FALSE, $sort, $fixed_list)
   *
   * @param array $fixed_list
   * @param bool $sort
   *
   * @return string[]
   */
  function setModuleList($fixed_list, $sort = FALSE) {

    foreach ($fixed_list as $name => $module) {
      $this->backdropGetFilename->backdropSetFilename('module', $name, $module['filename']);
      $this->list[$name] = $name;
    }

    if ($sort) {
      return $this->moduleListSorted();
    }

    return $this->list;
  }

  /**
   * Replicates module_list($refresh, $bootstrap_refresh, $sort, NULL)
   *
   * @see module_list()
   *
   * @param bool $refresh
   * @param bool $bootstrap_refresh
   * @param bool $sort
   *
   * @return string[]
   */
  function moduleList($refresh = FALSE, $bootstrap_refresh = FALSE, $sort = FALSE) {

    if (empty($this->list) || $refresh) {
      $this->list = array();
      $sorted_list = NULL;
      if ($refresh) {
        // For the $refresh case, make sure that system_list() returns fresh
        // data.
        $this->backdropStatic->resetKey('system_list');
      }
      if ($bootstrap_refresh) {
        $this->list = $this->systemList->systemListBootstrap();
      }
      else {
        // Not using backdrop_map_assoc() here as that requires common.inc.
        $this->list = array_keys($this->systemList->systemListModuleEnabled());
        $this->list = !empty($this->list)
          ? array_combine($this->list, $this->list)
          : array();
      }
    }

    if ($sort) {
      return $this->moduleListSorted();
    }

    if (count($this->list)) {
      # HackyLog::log($this->list);
    }

    return $this->list;
  }

  /**
   * @return string[]
   */
  private function moduleListSorted() {
    if (!isset($this->moduleListSorted)) {
      $this->moduleListSorted = $this->list;
      ksort($this->moduleListSorted);
    }
    return $this->moduleListSorted;
  }
} 
