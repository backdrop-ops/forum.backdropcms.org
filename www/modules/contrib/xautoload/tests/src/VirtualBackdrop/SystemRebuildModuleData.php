<?php


namespace Backdrop\xautoload\Tests\VirtualBackdrop;


class SystemRebuildModuleData {

  /**
   * @var BackdropStatic
   */
  private $backdropStatic;

  /**
   * @var SystemBuildModuleData
   */
  private $systemBuildModuleData;

  /**
   * @var SystemTable
   */
  private $systemTable;

  /**
   * @var ModuleBuildDependencies
   */
  private $moduleBuildDependencies;

  /**
   * @var SystemListReset
   */
  private $systemListReset;

  /**
   * @param BackdropStatic $backdropStatic
   * @param ModuleBuildDependencies $moduleBuildDependencies
   * @param SystemTable $systemTable
   * @param SystemBuildModuleData $systemBuildModuleData
   * @param SystemListReset $systemListReset
   */
  function __construct(
    BackdropStatic $backdropStatic,
    ModuleBuildDependencies $moduleBuildDependencies,
    SystemTable $systemTable,
    SystemBuildModuleData $systemBuildModuleData,
    SystemListReset $systemListReset
  ) {
    $this->backdropStatic = $backdropStatic;
    $this->moduleBuildDependencies = $moduleBuildDependencies;
    $this->systemTable = $systemTable;
    $this->systemBuildModuleData = $systemBuildModuleData;
    $this->systemListReset = $systemListReset;
  }

  /**
   * Rebuild, save, and return data about all currently available modules.
   *
   * @see system_rebuild_module_data()
   *
   * @return array[]
   */
  public function systemRebuildModuleData() {
    $modules_cache = &$this->backdropStatic->get('system_rebuild_module_data');
    // Only rebuild once per request. $modules and $modules_cache cannot be
    // combined into one variable, because the $modules_cache variable is reset by
    // reference from system_list_reset() during the rebuild.
    if (!isset($modules_cache)) {
      $modules = $this->systemBuildModuleData->systemBuildModuleData();
      ksort($modules);
      $this->systemTable->systemGetFilesDatabase($modules, 'module');
      $this->systemUpdateFilesDatabase($modules, 'module');
      $modules = $this->moduleBuildDependencies->moduleBuildDependencies($modules);
      $modules_cache = $modules;
    }
    return $modules_cache;
  }

  /**
   * @see system_update_files_database()
   *
   * @param object[] $files
   * @param string $type
   */
  private function systemUpdateFilesDatabase($files, $type) {
    $this->systemTable->systemUpdateFilesDatabase($files, $type);

    // If any module or theme was moved to a new location, we need to reset the
    // system_list() cache or we will continue to load the old copy, look for
    // schema updates in the wrong place, etc.
    $this->systemListReset->systemListReset();
  }
} 
