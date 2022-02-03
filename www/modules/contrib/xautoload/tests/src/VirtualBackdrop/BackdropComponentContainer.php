<?php


namespace Backdrop\xautoload\Tests\VirtualBackdrop;

use Backdrop\xautoload\Tests\Mock\MockBackdropSystem;

/**
 *
 * @property ModuleEnable ModuleEnable
 * @property BackdropGetFilename BackdropGetFilename
 * @property SystemUpdateBootstrapStatus SystemUpdateBootstrapStatus
 * @property SystemRebuildModuleData SystemRebuildModuleData
 * @property SystemListReset SystemListReset
 * @property SystemTable SystemTable
 * @property ModuleList ModuleList
 * @property HookSystem HookSystem
 * @property BackdropStatic BackdropStatic
 * @property SystemList SystemList
 * @property Cache Cache
 * @property ModuleBuildDependencies ModuleBuildDependencies
 * @property SystemBuildModuleData SystemBuildModuleData
 * @property LibrariesInfo LibrariesInfo
 * @property LibrariesLoad LibrariesLoad
 * @property BackdropBootstrap BackdropBoot
 * @property BackdropLoad BackdropLoad
 * @property MockBackdropSystem MockBackdropSystem
 */
class BackdropComponentContainer {

  /**
   * @var object[]
   */
  private $components = array();

  /**
   * @var ExampleModulesInterface
   */
  private $exampleModules;

  /**
   * @param ExampleModulesInterface $exampleModules
   */
  function __construct(ExampleModulesInterface $exampleModules) {
    $this->exampleModules = $exampleModules;
  }

  /**
   * Magic getter for a Backdrop component.
   *
   * @param string $key
   *
   * @return object
   *
   * @throws \Exception
   */
  function __get($key) {
    if (array_key_exists($key, $this->components)) {
      return $this->components[$key];
    }
    $method = 'get' . $key;
    if (!method_exists($this, $method)) {
      throw new \Exception("Unsupported key '$key' for BackdropComponentContainer.");
    }
    return $this->components[$key] = $this->$method($this);
  }

  /**
   * @return SystemTable
   *
   * @see BackdropComponentContainer::SystemTable
   */
  protected function getSystemTable() {
    return new SystemTable();
  }

  /**
   * @return Cache
   *
   * @see BackdropComponentContainer::Cache
   */
  protected function getCache() {
    return new Cache();
  }

  /**
   * @return BackdropStatic
   *
   * @see BackdropComponentContainer::BackdropStatic
   */
  protected function getBackdropStatic() {
    return new BackdropStatic();
  }

  /**
   * @return BackdropGetFilename
   *
   * @see BackdropComponentContainer::BackdropGetFilename
   */
  protected function getBackdropGetFilename() {
    return new BackdropGetFilename($this->SystemTable, $this->exampleModules);
  }

  /**
   * @return HookSystem
   *
   * @see BackdropComponentContainer::HookSystem
   */
  protected function getHookSystem() {
    return new HookSystem(
      $this->BackdropStatic,
      $this->Cache,
      $this->ModuleList);
  }

  /**
   * @return ModuleEnable
   *
   * @see BackdropComponentContainer::ModuleEnable
   */
  protected function getModuleEnable() {
    return new ModuleEnable(
      $this->BackdropGetFilename,
      $this->HookSystem,
      $this->ModuleList,
      $this->SystemTable,
      $this->SystemListReset,
      $this->SystemRebuildModuleData,
      $this->SystemUpdateBootstrapStatus);
  }

  /**
   * @return ModuleList
   *
   * @see BackdropComponentContainer::ModuleList
   */
  protected function getModuleList() {
    return new ModuleList(
      $this->BackdropGetFilename,
      $this->SystemList,
      $this->BackdropStatic);
  }

  /**
   * @return SystemListReset
   *
   * @see BackdropComponentContainer::SystemListReset
   */
  protected function getSystemListReset() {
    return new SystemListReset(
      $this->Cache,
      $this->BackdropStatic);
  }

  /**
   * @return ModuleBuildDependencies
   *
   * @see BackdropComponentContainer::ModuleBuildDependencies
   */
  protected function getModuleBuildDependencies() {
    return new ModuleBuildDependencies();
  }

  /**
   * @return SystemBuildModuleData
   *
   * @see BackdropComponentContainer::SystemBuildModuleData
   */
  protected function getSystemBuildModuleData() {
    return new SystemBuildModuleData(
      $this->exampleModules,
      $this->HookSystem);
  }

  /**
   * @return SystemRebuildModuleData
   *
   * @see BackdropComponentContainer::SystemRebuildModuleData
   */
  protected function getSystemRebuildModuleData() {
    return new SystemRebuildModuleData(
      $this->BackdropStatic,
      $this->ModuleBuildDependencies,
      $this->SystemTable,
      $this->SystemBuildModuleData,
      $this->SystemListReset);
  }

  /**
   * @return SystemUpdateBootstrapStatus
   *
   * @see BackdropComponentContainer::SystemUpdateBootstrapStatus
   */
  protected function getSystemUpdateBootstrapStatus() {
    return new SystemUpdateBootstrapStatus(
      $this->HookSystem,
      $this->SystemTable,
      $this->SystemListReset);
  }

  /**
   * @return SystemList
   *
   * @see BackdropComponentContainer::SystemList
   */
  protected function getSystemList() {
    return new SystemList(
      $this->Cache,
      $this->SystemTable,
      $this->BackdropGetFilename,
      $this->BackdropStatic);
  }

  /**
   * @return LibrariesInfo
   *
   * @see BackdropComponentContainer::LibrariesInfo
   */
  protected function getLibrariesInfo() {
    return new LibrariesInfo(
      $this->BackdropStatic,
      $this->HookSystem);
  }

  /**
   * @return LibrariesLoad
   *
   * @see BackdropComponentContainer::LibrariesLoad
   */
  protected function getLibrariesLoad() {
    return new LibrariesLoad(
      $this->BackdropStatic,
      $this->Cache,
      $this->LibrariesInfo);
  }

  /**
   * @return BackdropBootstrap
   *
   * @see BackdropComponentContainer::BackdropBoot
   */
  protected function getBackdropBoot() {
    return new BackdropBootstrap(
      $this->BackdropLoad,
      $this->HookSystem,
      $this->ModuleList);
  }

  /**
   * @return MockBackdropSystem
   *
   * @see BackdropComponentContainer::MockBackdropSystem
   */
  protected function getMockBackdropSystem() {
    return new MockBackdropSystem($this);
  }

  /**
   * @return BackdropLoad
   *
   * @see BackdropComponentContainer::BackdropLoad
   */
  protected function getBackdropLoad() {
    return new BackdropLoad(
      $this->BackdropGetFilename);
  }

} 
