<?php


namespace Backdrop\xautoload\Tests\VirtualBackdrop;



class BackdropBootstrap {

  /**
   * @var HookSystem
   */
  private $hookSystem;

  /**
   * @var ModuleList
   */
  private $moduleList;

  /**
   * @var BackdropLoad
   */
  private $backdropLoad;

  /**
   * Replicates the static $has_run in module_load_all()
   *
   * @var bool
   */
  private $moduleLoadAllHasRun = FALSE;

  /**
   * @param BackdropLoad $backdropLoad
   * @param HookSystem $hookSystem
   * @param ModuleList $moduleList
   */
  function __construct(BackdropLoad $backdropLoad, HookSystem $hookSystem, ModuleList $moduleList) {
    $this->backdropLoad = $backdropLoad;
    $this->hookSystem = $hookSystem;
    $this->moduleList = $moduleList;
  }

  /**
   * @see backdrop_bootstrap()
   */
  function boot() {
    $this->backdropBootstrapVariables();
    $this->backdropBootstrapPageHeader();
    $this->backdropBootstrapFull();
  }

  /**
   * @see _backdrop_bootstrap_variables()
   */
  private function backdropBootstrapVariables() {
    $this->moduleLoadAll(TRUE);
  }

  /**
   * @see _backdrop_bootstrap_page_header()
   */
  private function backdropBootstrapPageHeader() {
    $this->bootstrapInvokeAll('boot');
  }

  /**
   * @see _backdrop_bootstrap_full()
   */
  private function backdropBootstrapFull() {
    $this->moduleLoadAll();
    $this->menuSetCustomTheme();
    $this->hookSystem->moduleInvokeAll('init');
  }

  /**
   * @see menu_set_custom_theme()
   */
  private function menuSetCustomTheme() {
    $this->hookSystem->moduleInvokeAll('custom_theme');
  }

  /**
   * Replicates module_load_all()
   *
   * @see module_load_all()
   *
   * @param bool|null $bootstrap
   *
   * @return bool
   */
  private function moduleLoadAll($bootstrap = FALSE) {
    if (isset($bootstrap)) {
      foreach ($this->moduleList->moduleList(TRUE, $bootstrap) as $module) {
        $this->backdropLoad->backdropLoad('module', $module);
      }
      // $has_run will be TRUE if $bootstrap is FALSE.
      $this->moduleLoadAllHasRun = !$bootstrap;
    }
    return $this->moduleLoadAllHasRun;
  }

  /**
   * @see bootstrap_invoke_all()
   *
   * @param string $hook
   */
  private function bootstrapInvokeAll($hook) {
    // Bootstrap modules should have been loaded when this function is called, so
    // we don't need to tell module_list() to reset its internal list (and we
    // therefore leave the first parameter at its default value of FALSE). We
    // still pass in TRUE for the second parameter, though; in case this is the
    // first time during the bootstrap that module_list() is called, we want to
    // make sure that its internal cache is primed with the bootstrap modules
    // only.
    foreach ($this->moduleList->moduleList(FALSE, TRUE) as $module) {
      $this->backdropLoad->backdropLoad('module', $module);
      PureFunctions::moduleInvoke($module, $hook);
    }
  }
} 
