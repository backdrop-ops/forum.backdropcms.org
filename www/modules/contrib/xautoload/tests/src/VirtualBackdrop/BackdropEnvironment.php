<?php


namespace Backdrop\xautoload\Tests\VirtualBackdrop;


use Backdrop\xautoload\Tests\Mock\MockBackdropSystem;

class BackdropEnvironment {

  /**
   * @var self
   */
  private static $staticInstance;

  /**
   * @var BackdropComponentContainer
   */
  private $components;

  /**
   * @var ExampleModulesInterface
   */
  private $exampleModules;

  /**
   * @param ExampleModulesInterface $exampleModules
   */
  function __construct(ExampleModulesInterface $exampleModules) {
    $this->components = new BackdropComponentContainer($exampleModules);
    $this->exampleModules = $exampleModules;
  }

  function setStaticInstance() {
    self::$staticInstance = $this;
  }

  /**
   * @return BackdropEnvironment
   */
  static function getInstance() {
    return self::$staticInstance;
  }

  /**
   * @return MockBackdropSystem
   */
  function getMockBackdropSystem() {
    return $this->components->MockBackdropSystem;
  }

  /**
   * @return Cache
   */
  function getCache() {
    return $this->components->Cache;
  }

  /**
   * @return SystemTable
   */
  function getSystemTable() {
    return $this->components->SystemTable;
  }

  /**
   * Simulates Backdrop's \module_enable()
   *
   * @param string[] $module_list
   *   Array of module names.
   * @param bool $enable_dependencies
   *   TRUE, if dependencies should be enabled too.
   *
   * @return bool
   */
  function moduleEnable(array $module_list, $enable_dependencies = TRUE) {
    $this->components->ModuleEnable->moduleEnable($module_list, $enable_dependencies);
  }

  /**
   * Replicates the Backdrop bootstrap.
   */
  public function boot() {
    $this->components->BackdropBoot->boot();
  }

  /**
   * Version of systemUpdateBootstrapStatus() with no side effects.
   *
   * @see _system_update_bootstrap_status()
   */
  public function initBootstrapStatus() {
    $bootstrap_modules = $this->exampleModules->getBootstrapModules();
    $this->components->SystemTable->setBootstrapModules($bootstrap_modules);
  }

  /**
   * @param string $name
   *
   * @return mixed
   */
  public function librariesLoad($name) {
    return $this->components->LibrariesLoad->librariesLoad($name);
  }

}
